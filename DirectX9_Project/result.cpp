//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "result.h"

/* 全体で必要なインクルード*/
#include "input.h"
#include "camera.h"

/* Debug */
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

//=============================================================================
// 更新処理
//=============================================================================
void ResultScene::Update(void)
{
	UpdateInput();		// 入力


	UpdateCamera();		// カメラ

#ifdef _DEBUG
	UpdateDebugProc();
	PrintDebugProc("ResultScene[Update]\n");
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void ResultScene::Draw(void)
{
	SetCamera();		// カメラ

#ifdef _DEBUG
	PrintDebugProc("ResultScene[Draw]\n");
	DrawDebugProc();
#endif
}

//=============================================================================
// コンストラクタ処理
//=============================================================================
ResultScene::ResultScene(void)
{

}

//=============================================================================
// デストラクタ処理
//=============================================================================
ResultScene::~ResultScene(void)
{

}