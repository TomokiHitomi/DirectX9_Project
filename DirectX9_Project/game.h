//=============================================================================
//
// ゲーム処理 [game.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class GameScene :public BaseScene
{   // :public  BaseScene
public:
	GameScene(void);
	~GameScene(void);
	void Update(void);
	void Draw(void);
private:

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif