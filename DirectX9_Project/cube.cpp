//=============================================================================
//
// キューブ処理 [cube.cpp]
// Author : GP12A295 25 人見友基 
//
//=============================================================================
#include "cube.h"
#include "main.h"
#include "calculate.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	*g_pD3DTextureCube;		// テクスチャへのポインタ
D3DMATERIAL9		*g_pD3DMatMeshCube;

LPD3DXMESH			g_pD3DXMeshCube;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatCube;	// マテリアル情報へのポインタ
DWORD				g_nNumMatCube;			// マテリアル情報の数
D3DXMATRIX			g_mtxWorldCube;			// ワールドマトリックス

CUBE		cubeWk[CUBE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCube(int nType)
{
	CUBE *cube = &cubeWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < CUBE_MAX; i++, cube++)
	{
		cube->bUse = true;
		cube->pos = D3DXVECTOR3(0.0, 0.0f, 0.0f);
		cube->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		cube->scl = D3DXVECTOR3(CUBE_SCALE, CUBE_SCALE, CUBE_SCALE);
	}

	if (nType == 0)
	{
		// モデル関係の初期化
		g_pD3DTextureCube = NULL;
		g_pD3DXMeshCube = NULL;
		g_pD3DXBuffMatCube = NULL;
		g_nNumMatCube = 0;

		// モデルデータ読み込み
		if (FAILED(D3DXLoadMeshFromX(CUBE_MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,			// 使用するメモリのオプション
			pDevice,					// デバイス
			NULL,						// 未使用
			&g_pD3DXBuffMatCube,		// マテリアルデータ
			NULL,						// 未使用
			&g_nNumMatCube,			// D3DXMATERIAL構造体の数
			&g_pD3DXMeshCube)))		// メッシュデータへのポインタ
		{
			return E_FAIL;
		}

		// マテリアル情報を取り出す
		D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatCube->GetBufferPointer();
		g_pD3DMatMeshCube = new D3DMATERIAL9[g_nNumMatCube];		// メッシュ情報を確保
		g_pD3DTextureCube = new LPDIRECT3DTEXTURE9[g_nNumMatCube];// テクスチャを確保
		for (int i = 0; i < g_nNumMatCube; i++)
		{
			g_pD3DMatMeshCube[i] = d3Mat[i].MatD3D;			// マテリアル情報セット
			g_pD3DMatMeshCube[i].Ambient = g_pD3DMatMeshCube[i].Diffuse;// 環境光初期化
			g_pD3DTextureCube[i] = NULL;	// テクスチャ初期化

			// 使用しているテクスチャがあれば読み込む
			if (d3Mat[i].pTextureFilename != NULL &&
				lstrlen(d3Mat[i].pTextureFilename) > 0)
			{
				// テクスチャ読み込み
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					d3Mat[i].pTextureFilename,
					&g_pD3DTextureCube[i])))
				{
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCube(void)
{
	for (int i = 0; i < g_nNumMatCube; i++)
	{	// テクスチャの開放
		if (g_pD3DTextureCube[i] != NULL)
		{
			g_pD3DTextureCube[i]->Release();
			g_pD3DTextureCube[i] = NULL;
		}
	}

	if(g_pD3DXMeshCube != NULL)
	{// メッシュの開放
		g_pD3DXMeshCube->Release();
		g_pD3DXMeshCube = NULL;
	}

	if(g_pD3DXBuffMatCube != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatCube->Release();
		g_pD3DXBuffMatCube = NULL;
	}

	delete[]g_pD3DMatMeshCube;
	delete[]g_pD3DTextureCube;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCube(void)
{
	CUBE *cube = &cubeWk[0];

	for (int i = 0; i < CUBE_MAX; i++, cube++)
	{
		if (cube->bUse)
		{
			// モデル回転
			cube->rot.x += CUBE_ROT;
			cube->rot.y += CUBE_ROT;
			cube->rot.z += CUBE_ROT;
			cube->rot.x = PiCalculate180(cube->rot.y);
			cube->rot.y = PiCalculate180(cube->rot.y);
			cube->rot.z = PiCalculate180(cube->rot.y);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCube(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	CUBE *cube = &cubeWk[0];

	for (int i = 0; i < CUBE_MAX; i++, cube++)
	{
		if (cube->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldCube);

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, cube->scl.x, cube->scl.y, cube->scl.z);
			D3DXMatrixMultiply(&g_mtxWorldCube, &g_mtxWorldCube, &mtxScl);

			// 【R】回1転を反映(YawPitchRollはy,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, cube->rot.y, cube->rot.x, cube->rot.z);
			D3DXMatrixMultiply(&g_mtxWorldCube, &g_mtxWorldCube, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, cube->pos.x, cube->pos.y, cube->pos.z);
			D3DXMatrixMultiply(&g_mtxWorldCube, &g_mtxWorldCube, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldCube);

			/******************** ビューポート変換 ********************/
			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);
			//// マテリアル情報に対するポインタの取得
			//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatCube->GetBufferPointer();

			for (int j = 0; j < g_nNumMatCube; j++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&g_pD3DMatMeshCube[j]);
				// テクスチャの設定（NULL:テクスチャ無し）
				pDevice->SetTexture(0, g_pD3DTextureCube[j]);
				// 描画
				g_pD3DXMeshCube->DrawSubset(j);
			}
			// マテリアルを元に戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 出現処理
//=============================================================================
int SetCube(void)
{
	CUBE *cube = &cubeWk[0];

	for (int i = 0; i < CUBE_MAX; i++, cube++)
	{
		if (!cube->bUse)
		{
			cube->bUse = true;
			return i;
		}
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void SetCubePos(int nIndex, D3DXVECTOR3 pos)
{
	CUBE *cube = &cubeWk[nIndex];
	cube->pos = pos;
}

//=============================================================================
// カラー設定処理
//=============================================================================
void SetCubeColor(int nColor)
{
	D3DXCOLOR colorTemp = (1.0f, 1.0f, 1.0f, 1.0f);
	//switch(nColor)
	//{
	//case BULLET_SPECIAL_FIRE:
	//	colorTemp = SetColorPallet(COLOR_PALLET_ORANGE);
	//	break;
	//case BULLET_SPECIAL_THUNDER:
	//	colorTemp = SetColorPallet(COLOR_PALLET_YELLOW);
	//	break;
	//case BULLET_SPECIAL_ICE:
	//	colorTemp = SetColorPallet(COLOR_PALLET_CYAN);
	//	break;
	//case BULLET_SPECIAL_WIND:
	//	colorTemp = SetColorPallet(COLOR_PALLET_LIGHTGREEN);
	//	break;
	//case BULLET_SPECIAL_WATER:
	//	colorTemp = SetColorPallet(COLOR_PALLET_BLUE);
	//	break;
	//case BULLET_SPECIAL_EARTH:
	//	colorTemp = SetColorPallet(COLOR_PALLET_BROWN);
	//	break;
	//}
	//g_pD3DMatMeshCube->Diffuse = (D3DCOLORVALUE)colorTemp;
	//g_pD3DMatMeshCube->Ambient = (D3DCOLORVALUE)(colorTemp * 0.5f);
	//g_pD3DMatMeshCube->Specular = (D3DCOLORVALUE)colorTemp;
	//colorTemp = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
	//g_pD3DMatMeshCube->Emissive = (D3DCOLORVALUE)colorTemp;
	//g_pD3DMatMeshCube->Power = 1.0f;
}

//=============================================================================
// 解放処理
//=============================================================================
void ReleaseCube(int nIndex)
{
	CUBE *cube = &cubeWk[nIndex];
	cube->bUse = false;
}

//=============================================================================
// 取得関数
//=============================================================================
CUBE *GetCube(int no)
{
	return(&cubeWk[no]);
}
