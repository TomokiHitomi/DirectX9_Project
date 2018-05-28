//=============================================================================
//
// ライト処理 [light.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "light.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#include "camera.h"
#include "calculate.h"
#include "input.h"
#include "cube.h"
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
D3DLIGHT9				g_aLight[LIGHT_MAX];		// ライト情報

#ifdef _DEBUG
POINTLIGHT				g_tagPointLight[LIGHT_POINT_MAX];
bool					g_bLight[LIGHT_MAX];
int						g_nColor[LIGHT_MAX];
int						g_nSelect;
#endif

//=============================================================================
// 初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;

	/********** ライト0 **********/

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ライト0のタイプの設定
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// ライト0の拡散光の設定
	g_aLight[0].Diffuse = D3DXCOLOR(LIGHT_DIFFUSE_0, LIGHT_DIFFUSE_0, LIGHT_DIFFUSE_0, 1.0f);

	// ライト0の鏡面反射光の設定
	g_aLight[0].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライト0の環境光の設定
	//g_aLight[0].Ambient = D3DXCOLOR(LIGHT_AMBIENT, LIGHT_AMBIENT, LIGHT_AMBIENT, 1.0f);

	// ライト0の方向の設定
	vecDir = D3DXVECTOR3(-0.01f, -0.54f, -0.842f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ライト0をレンダリングパイプラインに設定
	pDevice->SetLight(0, &g_aLight[0]);

	// ライト0を使用状態に
	pDevice->LightEnable(0, TRUE);


	/********** ライト1 **********/

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	// ライト1のタイプの設定
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// ライト1の拡散光の設定
	g_aLight[1].Diffuse = D3DXCOLOR(LIGHT_DIFFUSE_1, LIGHT_DIFFUSE_1, LIGHT_DIFFUSE_1, 1.0f);

	// ライト1の鏡面反射光の設定
	//	g_aLight[1].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト1の環境光の設定
	//	g_aLight[1].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト1の方向の設定
	vecDir = D3DXVECTOR3(-0.01f, 0.54f, -0.842f);
	//vecDir = D3DXVECTOR3(-0.0f, -0.6f, -1.0f);
	//vecDir = D3DXVECTOR3(-0.0f, -1.0f, 0.8f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// ライト1をレンダリングパイプラインに設定
	pDevice->SetLight(1, &g_aLight[1]);

	// ライト1を使用状態に
	pDevice->LightEnable(1, TRUE);

	// 残りのライトを初期化
	for (int i = 2; i < LIGHT_MAX; i++)
	{
		// D3DLIGHT9構造体を0でクリアする
		ZeroMemory(&g_aLight[i], sizeof(D3DLIGHT9));
		g_aLight[i].Type = D3DLIGHT_SPOT;
		g_aLight[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//g_aLight[i].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//g_aLight[i].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[i].Direction, &vecDir);
		pDevice->SetLight(i, &g_aLight[i]);
		pDevice->LightEnable(i, FALSE);
	}

	// ライティングモードをON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

#ifdef _DEBUG
	g_nSelect = 0;

	for (int i = 0; i < LIGHT_MAX; i++)
	{
		if (i == LIGHT_MAIN)
		{
			g_bLight[i] = true;
		}
		else
		{
			g_bLight[i] = false;
		}
		g_nColor[i] = 0;
	}
	for (int i = 0; i < LIGHT_POINT_MAX; i++)
	{
		g_tagPointLight[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_tagPointLight[i].posCenter = D3DXVECTOR3(0.0f, 25.0f, 0.0f);
		g_tagPointLight[i].fHAngle = 0.0f + i * D3DX_PI;
		g_tagPointLight[i].fVAngle = 0.0f;
	}
#endif
}

//=============================================================================
// ライト設定処理
//=============================================================================
void SetLight(int nLight, bool bFlag)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->LightEnable(nLight, bFlag);
}

//=============================================================================
// ポイントライト設定処理
//=============================================================================
void SetLightPoint(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 pos,
	FLOAT fRange, FLOAT fAttenuation)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXVECTOR3 vecDir;

	g_aLight[nLight].Type = D3DLIGHT_POINT;									// タイプ
	g_aLight[nLight].Diffuse = xColor;										// 拡散光
	//g_aLight[nLight].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 鏡面反射光
	//g_aLight[nLight].Ambient = xColor * LIGHT_AMBIENT_POINT;				// 環境光
	g_aLight[nLight].Position = pos;										// 光源位置
	//vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);								// 光源方向
	//D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[nLight].Direction, &vecDir);
	g_aLight[nLight].Range = fRange;										// 光源有効距離
	//g_aLight[nLight].Falloff = 1.0f;										// 輝度減衰（スポット）
	//g_aLight[nLight].Attenuation0 = fAttenuation;							// 輝度減衰
	g_aLight[nLight].Attenuation1 = fAttenuation;
	//g_aLight[nLight].Attenuation2 = fAttenuation;
	//g_aLight[nLight].Theta = 1.0f;										// 内部角度（スポット）
	//g_aLight[nLight].Phi = 1.0f;											// 外部エッジ（スポット）

	pDevice->SetLight(nLight, &g_aLight[nLight]);							// レンダリングパイプラインに設定
}

//=============================================================================
// スポットライト設定処理
//=============================================================================
void SetLightSpot(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 pos, D3DXVECTOR3 vecDir,
	FLOAT fRange, FLOAT fAttenuation, FLOAT fTheta, FLOAT fPhi)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aLight[nLight].Type = D3DLIGHT_SPOT;									// タイプ
	g_aLight[nLight].Diffuse = xColor;										// 拡散光
	g_aLight[nLight].Specular = xColor;										// 鏡面反射光
	//g_aLight[nLight].Ambient = xColor * LIGHT_AMBIENT_SPOT;					// 環境光
	g_aLight[nLight].Position = pos;										// 光源位置
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[nLight].Direction, &vecDir);	// 光源方向
	g_aLight[nLight].Range = fRange;										// 光源有効距離
	g_aLight[nLight].Falloff = 0.5f;										// 輝度減衰（スポット）
	//g_aLight[nLight].Attenuation0 = fAttenuation;							// 輝度減衰
	g_aLight[nLight].Attenuation1 = fAttenuation;
	//g_aLight[nLight].Attenuation2 = fAttenuation;
	g_aLight[nLight].Theta = fTheta;										// 内部角度（スポット）
	g_aLight[nLight].Phi = fPhi;											// 外部エッジ（スポット）

	pDevice->SetLight(nLight, &g_aLight[nLight]);							// レンダリングパイプラインに設定
}

//=============================================================================
// ライト設定処理
//=============================================================================
void SetLightDirectional(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 vecDir)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aLight[nLight].Type = D3DLIGHT_DIRECTIONAL;							// タイプ
	g_aLight[nLight].Diffuse = xColor;										// 拡散光
	//g_aLight[nLight].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 鏡面反射光
	//g_aLight[nLight].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 環境光
	//g_aLight[nLight].Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// 光源位置
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[nLight].Direction, &vecDir);	// 光源方向
	//g_aLight[nLight].Range = 1.0f;										// 光源有効距離
	//g_aLight[nLight].Falloff = 1.0f;										// 輝度減衰（スポット）
	//g_aLight[nLight].Attenuation0 = 1.0f;									// 輝度減衰
	//g_aLight[nLight].Attenuation1 = 1.0f;
	//g_aLight[nLight].Attenuation2 = 1.0f;
	//g_aLight[nLight].Theta = 1.0f;										// 内部角度（スポット）
	//g_aLight[nLight].Phi = 1.0f;											// 外部エッジ（スポット）

	pDevice->SetLight(nLight, &g_aLight[nLight]);							// レンダリングパイプラインに設定
}



#ifdef _DEBUG
//=============================================================================
// ライト状態処理（デバッグ）
//=============================================================================
void UpdateDebugLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BOOL bEnable;

	PrintDebugProc("【 LIGHT 】\n");
	PrintDebugProc("ＷＡＳＤ：移動\n");
	PrintDebugProc("↑↓←→：カメラ\n");
	PrintDebugProc(" Ｒ／Ｆ ：ライト変更\n");
	PrintDebugProc("スペース：ライトOn/Off\n");
	PrintDebugProc(" Ｑ／Ｅ ：ライトカラー変更\n");
	PrintDebugProc("\n");

	// LightTestSpot

	if (GetKeyboardTrigger(DIK_R))
	{
		g_nSelect--;
		if (g_nSelect < 0)
		{
			g_nSelect = LIGHT_MAX - 1;
		}
	}
	if (GetKeyboardTrigger(DIK_F))
	{
		g_nSelect++;
		if (g_nSelect >= LIGHT_MAX)
		{
			g_nSelect = 0;
		}
	}

	if (GetKeyboardTrigger(DIK_Q))
	{
		g_nColor[g_nSelect]--;
		if (g_nColor[g_nSelect] < 0)
		{
			g_nColor[g_nSelect] = COLOR_PALLET_BROWN - 1;
		}
	}
	if (GetKeyboardTrigger(DIK_E))
	{
		g_nColor[g_nSelect]++;
		if (g_nColor[g_nSelect] >= COLOR_PALLET_BROWN)
		{
			g_nColor[g_nSelect] = 0;
		}
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		g_bLight[g_nSelect] = g_bLight[g_nSelect] ? false : true;
	}

	// ライトの点灯
	for (int i = 0; i < LIGHT_MAX; i++)
	{

		if (g_nSelect == i)
		{
			PrintDebugProc(">> ");
		}
		else
		{
			PrintDebugProc("   ");
		}
		switch (i)
		{
		case LIGHT_MAIN:
			PrintDebugProc("DIRECT_0:[");
			break;
		case LIGHT_SUB:
			PrintDebugProc("DIRECT_1:[");
			break;
		case LIGHT_SPOT_UP:
			PrintDebugProc(" SPOT_UP:[");
			break;
		case LIGHT_SPOT_CAMERA:
			PrintDebugProc("SPOT_CAM:[");
			break;
		case LIGHT_POINT_1:
			PrintDebugProc(" POINT_0:[");
			break;
		case LIGHT_POINT_2:
			PrintDebugProc(" POINT_1:[");
			break;
		}

		if (g_bLight[i])
		{
			PrintDebugProc("TRUE ]  ");
		}
		else
		{
			PrintDebugProc("FALSE]  ");
		}

		switch (g_nColor[i])
		{
		case COLOR_PALLET_WHITE:
			PrintDebugProc("COLOR:[WHITE]");
			break;
		case COLOR_PALLET_RED:
			PrintDebugProc("COLOR:[RED]");
			break;
		case COLOR_PALLET_YELLOW:
			PrintDebugProc("COLOR:[YELLOW]");
			break;
		case COLOR_PALLET_GREEN:
			PrintDebugProc("COLOR:[GREEN]");
			break;
		case COLOR_PALLET_CYAN:
			PrintDebugProc("COLOR:[CYAN]");
			break;
		case COLOR_PALLET_BLUE:
			PrintDebugProc("COLOR:[BLUE]");
			break;
		case COLOR_PALLET_MAGENTA:
			PrintDebugProc("COLOR:[MAGENTA]");
			break;
		}
		SetLight(i, g_bLight[i]);
		PrintDebugProc("\n");
	}
	PrintDebugProc("\n");

	SetLightDirectional(LIGHT_MAIN, SetColorPallet(g_nColor[LIGHT_MAIN]), D3DXVECTOR3(-0.01f, -0.54f, -0.842f));
	SetLightDirectional(LIGHT_SUB, SetColorPallet(g_nColor[LIGHT_SUB]), D3DXVECTOR3(-0.01f, 0.54f, -0.842f));

	SetLightSpot(LIGHT_SPOT_UP,
		SetColorPallet(g_nColor[LIGHT_SPOT_UP]),
		D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		D3DXVECTOR3(0.0f,-1.0f,0.0f), 300.0f, 0.01f, 0.5f, 1.0f);

	CAMERA *camera = GetCamera(0);
	D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
	D3DXVec3Normalize(&vecTemp, &vecTemp);

	SetLightSpot(LIGHT_SPOT_CAMERA,
		SetColorPallet(g_nColor[LIGHT_SPOT_CAMERA]),
		camera->posCameraEye,
		vecTemp, 300.0f, 0.02f, 0.3f, 0.3f);


	// LightTestPoint
	// 回転半径を設定
	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, LIGHT_POINT_ROT_LENGTH, 0.0f);
	D3DXVECTOR3 vecAxis;

	for (int i = 0; i < LIGHT_POINT_MAX; i++)
	{
		g_tagPointLight[i].fHAngle += LIGHT_POINT_ROT_SPEED_H;
		g_tagPointLight[i].fVAngle += LIGHT_POINT_ROT_SPEED_V;
		// 現在の角度に垂直な回転軸ベクトルを設定
		vecAxis = D3DXVECTOR3(cosf(g_tagPointLight[i].fHAngle), 0, sinf(g_tagPointLight[i].fHAngle));

		// クォータニオン処理
		QuaternionCalculate(&vecTa, &vecAxis, g_tagPointLight[i].fVAngle,
			&g_tagPointLight[i].pos);
		// 回転軸に設置
		g_tagPointLight[i].pos += g_tagPointLight[i].posCenter;
	}

	SetLightPoint(LIGHT_POINT_1, SetColorPallet(g_nColor[LIGHT_POINT_1]),
		g_tagPointLight[0].pos,
		40.0f, 0.01f);
	SetCubePos(0, g_tagPointLight[0].pos);

	SetLightPoint(LIGHT_POINT_2, SetColorPallet(g_nColor[LIGHT_POINT_2]),
		g_tagPointLight[1].pos,
		40.0f, 0.01f);
	SetCubePos(1, g_tagPointLight[1].pos);


	//for (int i = 0; i < LIGHT_MAX; i++)
	//{
	//	pDevice->GetLightEnable(i, &bEnable);
	//	PrintDebugProc("Ligth[%d] Enable[%d] Type[%d]\n", i, bEnable, g_aLight[i].Type);
	//	PrintDebugProc("Diffuse  [R:%f,G:%f,B:%f,A:%f]\n",
	//		g_aLight[i].Diffuse.r, g_aLight[i].Diffuse.g, g_aLight[i].Diffuse.b, g_aLight[i].Diffuse.a);
	//	PrintDebugProc("Specular [R:%f,G:%f,B:%f,A:%f]\n",
	//		g_aLight[i].Specular.r, g_aLight[i].Specular.g, g_aLight[i].Specular.b, g_aLight[i].Specular.a);
	//	PrintDebugProc("Ambient  [R:%f,G:%f,B:%f,A:%f]\n",
	//		g_aLight[i].Ambient.r, g_aLight[i].Ambient.g, g_aLight[i].Ambient.b, g_aLight[i].Ambient.a);
	//	if (g_aLight[i].Type != D3DLIGHT_DIRECTIONAL)
	//	{
	//		PrintDebugProc("Position [X:%f,Y:%f,Z:%f]\n",
	//			g_aLight[i].Position.x, g_aLight[i].Position.y, g_aLight[i].Position.z);
	//	}
	//}
	//PrintDebugProc("\n");
}
#endif