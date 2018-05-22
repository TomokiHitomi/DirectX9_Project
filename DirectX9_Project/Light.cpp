//=============================================================================
//
// ライト処理 [light.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "light.h"

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
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト0の鏡面反射光の設定
	g_aLight[0].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト0の環境光の設定
	g_aLight[0].Ambient = D3DXCOLOR(LIGHT_AMBIENT, LIGHT_AMBIENT, LIGHT_AMBIENT, 1.0f);

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


	/********** ライト2 **********/

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));

	// ライト2のタイプの設定
	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	// ライト2の拡散光の設定
	g_aLight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト2の鏡面反射光の設定
	g_aLight[2].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	 //ライト2の環境光の設定
	g_aLight[2].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト2の方向の設定
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[2].Direction, &vecDir);

	// ライト2をレンダリングパイプラインに設定
	pDevice->SetLight(2, &g_aLight[2]);

	// ライト2を無効
	pDevice->LightEnable(2, FALSE);


	///********** ライト3 **********/

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[3], sizeof(D3DLIGHT9));

	// ライト3のタイプの設定
	g_aLight[3].Type = D3DLIGHT_DIRECTIONAL;

	// ライト3の拡散光の設定
	g_aLight[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト3の鏡面反射光の設定
	//	g_aLight[3].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト3の環境光の設定
	//	g_aLight[3].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト3の方向の設定
	vecDir = D3DXVECTOR3(-0.01f, -0.54f, -0.842f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[3].Direction, &vecDir);

	// ライト3をレンダリングパイプラインに設定
	pDevice->SetLight(3, &g_aLight[3]);

	// ライト3を使用状態に
	pDevice->LightEnable(3, TRUE);


	///********** ライト4 **********/

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[4], sizeof(D3DLIGHT9));

	// ライト4のタイプの設定
	g_aLight[4].Type = D3DLIGHT_DIRECTIONAL;

	// ライト4の拡散光の設定
	g_aLight[4].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト4の鏡面反射光の設定
	//	g_aLight[4].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト4の環境光の設定
	//	g_aLight[4].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト4の方向の設定
	vecDir = D3DXVECTOR3(-0.01f, -0.54f, -0.842f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[4].Direction, &vecDir);

	// ライト4をレンダリングパイプラインに設定
	pDevice->SetLight(4, &g_aLight[4]);

	// ライト4を使用状態に
	pDevice->LightEnable(4, TRUE);


	///********** ライト5 **********/

	//// D3DLIGHT9構造体を0でクリアする
	//ZeroMemory(&g_aLight[5], sizeof(D3DLIGHT9));

	//// ライト5のタイプの設定
	//g_aLight[5].Type = D3DLIGHT_DIRECTIONAL;

	//// ライト5の拡散光の設定
	//g_aLight[5].Diffuse = D3DXCOLOR(LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, 1.0f);

	//// ライト5の鏡面反射光の設定
	////	g_aLight[5].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライト5の環境光の設定
	////	g_aLight[5].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライト5の方向の設定
	//vecDir = D3DXVECTOR3(0.0f, LIGHT_SUB_Y, -LIGHT_SUB_XZ);
	//D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[5].Direction, &vecDir);

	//// ライト5をレンダリングパイプラインに設定
	//pDevice->SetLight(5, &g_aLight[5]);

	//// ライト5を使用状態に
	//pDevice->LightEnable(5, TRUE);


	// ライティングモードをON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ライト設定処理
//=============================================================================
void SetLight(int nLight, bool bFlag)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->LightEnable(nLight, bFlag);
}
