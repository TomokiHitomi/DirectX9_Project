//=============================================================================
//
// フィールド処理 [field.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "camera.h"
#include "field.h"
#include "input.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureField = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// 頂点バッファへのポインタ

FIELD					fieldWk[FIELD_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(int nType)
{
	FIELD *field = &fieldWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_FIELD,
			&g_pD3DTextureField);
	}

	for (int i = 0; i < FIELD_Z; i++)
	{
		for (int j = 0; j < FIELD_X; j++, field++)
		{
			// 位置・回転・スケールの設定
			field->posField = D3DXVECTOR3(0.0f + j * FIELD_SIZE_X * 2, 0.0f, 0.0f + i * FIELD_SIZE_Z * 2);
			field->rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			field->sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
	}

	// 頂点情報の作成（ローカル座標の設定）
	MakeVertexField(pDevice);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	if(g_pD3DTextureField != NULL)
	{// テクスチャの開放
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}

	if(g_pD3DVtxBuffField != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{
	FIELD *field = &fieldWk[0];
	//for (int i = 0; i < FIELD_MAX; i++, field++)
	//{
	//	// 高さ移動
	//	if (GetKeyboardPress(DIK_SPACE))
	//	{
	//		field->posField.y += 5;
	//	}

	//	if (GetKeyboardPress(DIK_LSHIFT))
	//	{
	//		field->posField.y -= 5;
	//	}


	//	// Y左回転
	//	if (GetKeyboardPress(DIK_Q))
	//	{
	//		field->rotField.y += 0.05;
	//	}
	//	// Y右移動
	//	else if (GetKeyboardPress(DIK_E))
	//	{
	//		field->rotField.y -= 0.05;
	//	}


	//	// 拡大
	//	if (GetKeyboardPress(DIK_Z))
	//	{
	//		field->sclField += D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	//	}
	//	// 縮小
	//	else if (GetKeyboardPress(DIK_X))
	//	{
	//		field->sclField -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール, 回転, 平行移動

	FIELD *field = &fieldWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);


	for (int i = 0; i < FIELD_MAX; i++, field++)
	{
		/******************** ワールド変換 ********************/
		// ワールドマトリクスの初期化
		D3DXMatrixIdentity(&field->mtxWorld);

		// 【S】スケールを反映(Multiplyは行列計算)
		D3DXMatrixScaling(&mtxScl, field->sclField.x, field->sclField.y, field->sclField.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxScl);

		// 【R】回転を反映(YawPitchRollはy,x,z)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, field->rotField.y, field->rotField.x, field->rotField.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxRot);

		// 【T】平行移動を反映(オブジェクトを配置している）
		D3DXMatrixTranslation(&mtxTranslate, field->posField.x, field->posField.y, field->posField.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxTranslate);

		// ワールドマトリクスの設定
		pDevice->SetTransform(D3DTS_WORLD, &field->mtxWorld);


		/******************** ビューポート変換 ********************/
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureField);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	FIELD *field = &fieldWk[0];

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffField,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-FIELD_SIZE_X, 0.0f, FIELD_SIZE_Z);
		pVtx[1].vtx = D3DXVECTOR3(FIELD_SIZE_X, 0.0f, FIELD_SIZE_Z);
		pVtx[2].vtx = D3DXVECTOR3(-FIELD_SIZE_X, 0.0f, -FIELD_SIZE_Z);
		pVtx[3].vtx = D3DXVECTOR3(FIELD_SIZE_X, 0.0f, -FIELD_SIZE_Z);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffField->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 取得関数
//=============================================================================
FIELD *GetField(int no)
{
	return(&fieldWk[no]);
}