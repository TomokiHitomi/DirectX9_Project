//=============================================================================
//
// シーン処理 [title.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"

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
//始めはシーンを空にしておく
BaseScene *SceneManager::m_pScene = NULL;

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
		m_pScene = new TitleScene();	//タイトルシーンを現在のシーンにする
		break;
	case SCENE::GAME:
		m_pScene = new GameScene();		//ゲームシーンを現在のシーンにする
		break;
	case SCENE::RESULT:
		m_pScene = new ResultScene();	//リザルトシーンを現在のシーンにする
		break;
	default:
		break;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void SceneManager::Update()
{
	m_pScene->Update();		//現在のシーンの更新関数
}

//=============================================================================
// 描画処理
//=============================================================================
void SceneManager::Render()
{
	m_pScene->Draw();		//現在のシーンの描画関数
}

//=============================================================================
// コンストラクタ処理
//=============================================================================
SceneManager::SceneManager(void)
{
	ChangeScene(TITLE);
}

//=============================================================================
// デストラクタ処理
//=============================================================================
SceneManager::~SceneManager(void)
{

}