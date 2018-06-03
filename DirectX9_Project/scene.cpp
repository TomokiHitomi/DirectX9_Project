//=============================================================================
//
// シーン処理 [scene.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "scene.h"

/* Scene */
#include "title.h"
#include "game.h"
#include "result.h"

/* System */
#include "camera.h"
#include "light.h"
#include "input.h"
#include "fade.h"
//#include "sound.h"

/* デバッグ */
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
//始めはシーンを空にしておく
BaseScene *SceneManager::m_pScene = NULL;
SceneManager::SCENE SceneManager::m_nScene;

//=============================================================================
// シーン管理処理
//=============================================================================
void SceneManager::ChangeScene(SCENE scene)
{
	if (m_pScene != NULL)
	{
		delete m_pScene;
	}

	switch (scene)
	{	//引数のシーン
	case SCENE::TITLE:
		m_pScene = new TitleScene();	// タイトルシーンを現在のシーンにする
		break;
	case SCENE::GAME:
		m_pScene = new GameScene();		// ゲームシーンを現在のシーンにする
		break;
	case SCENE::RESULT:
		m_pScene = new ResultScene();	// リザルトシーンを現在のシーンにする
		break;
	default:
		break;
	}

	m_nScene = scene;
}

//=============================================================================
// 初期化処理
//=============================================================================
void SceneManager::Init(HINSTANCE hInst, HWND hWnd)
{
	m_nScene = GAME;		// 初期シーン番号を設定
	ChangeScene(m_nScene);	// 初期シーン設定
	InitLight();			// ライト
	InitInput(hInst, hWnd);	// 入力
	//InitSound(hWnd);		// サウンド
	InitCamera();			// カメラ
	InitFade();				// フェード

#ifdef _DEBUG
	InitDebugProc();		// デバッグ
#endif
}

//=============================================================================
// 終了処理
//=============================================================================
void SceneManager::Uninit()
{
	UninitInput();			// 入力
	UninitCamera();			// カメラ
	//UninitSound();			// サウンド
	UninitFade();			// フェード

#ifdef _DEBUG
	UninitDebugProc();		// デバッグ
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void SceneManager::Update()
{
	UpdateInput();			// 入力
	UpdateFade();			// フェード

#ifdef _DEBUG
	DebugScene();			// デバッグ用
#endif
	UpdateCamera();			// カメラ

	m_pScene->Update();		// 現在のシーンの更新関数

	//UpdateSound();			// サウンド
}

//=============================================================================
// 描画処理
//=============================================================================
void SceneManager::Draw()
{
	SetCamera();			// カメラ

	m_pScene->Draw();		// 現在のシーンの描画関数

	DrawFade();				// フェード

#ifdef _DEBUG
	DrawDebugProc();
#endif
}

//=============================================================================
// コンストラクタ処理
//=============================================================================
SceneManager::SceneManager(void)
{

}

//=============================================================================
// デストラクタ処理
//=============================================================================
SceneManager::~SceneManager(void)
{

}

#ifdef _DEBUG
//=============================================================================
// デバッグ処理
//=============================================================================
void SceneManager::DebugScene(void)
{
	UpdateDebugProc();

	PrintDebugProc("【 SCENE 】\n");
	switch (m_nScene)
	{
	case SCENE::TITLE:
		PrintDebugProc("Scene[TITLE]\n");
		break;
	case SCENE::GAME:
		PrintDebugProc("Scene[GAME]\n");
		break;
	case SCENE::RESULT:
		PrintDebugProc("Scene[RESULT]\n");
		break;
	}

	PrintDebugProc("\n");

	UpdateDebugLight();
}
#endif