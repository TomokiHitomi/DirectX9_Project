//=============================================================================
//
// メイン処理 [main.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#include <stdio.h>
#include <time.h>
#include <vector>
#include <iostream>

#ifdef _DEBUG
// デバッグコンソール用
#include <io.h>
#include <Fcntl.h>
#endif

#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		("AppClass")						// ウインドウのクラス名
#define WINDOW_NAME		("DirectX9 LightTest")// ウインドウのキャプション名
#define SCREEN_SCALE	(1.0f)

#define SCREEN_WIDTH	(1280 * SCREEN_SCALE)	// ウインドウの幅
#define SCREEN_HEIGHT	(720 * SCREEN_SCALE)	// ウインドウの高さ

#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの幅
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの高さ

#define	NUM_VERTEX		(4)		// 頂点数
#define	NUM_POLYGON		(2)		// ポリゴン数

#define DEBUG_RECT		(50)	// デバッグ

#define MOUSE_MARGIN	(0)

// ３Ｄポリゴン頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 上記３Ｄポリゴン頂点フォーマットに合わせ^た構造体を定義
typedef struct
{
	D3DXVECTOR3	vtx;		// 頂点座標
	D3DXVECTOR3	nor;		// 法線ベクトル
	D3DCOLOR	diffuse;	// 反射光
	D3DXVECTOR2	tex;		// テクスチャ座標
} VERTEX_3D;

// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;	// 頂点座標
	float		rhw;	// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;	// 反射光
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

#define START_STAGE		(STAGE_TITLE)	// 開始時のステージ

// ステージ
enum E_STAGE
{
	STAGE_TITLE,
	STAGE_TUTORIAL,
	STAGE_CREDIT,
	STAGE_GAME,
	STAGE_RESULT,
	STAGE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
int *GetTotalCount(void);
void InitSystem(int nType);
void SetStage(E_STAGE eStage);
E_STAGE GetStage(void);
void SetEndFlag(void);

#endif