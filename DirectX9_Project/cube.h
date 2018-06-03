//=============================================================================
//
// キューブ処理 [cube.h]
// Author : GP12A295 25 人見友基 
//
//=============================================================================
#ifndef _CUBE_H_
#define _CUBE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CUBE_MODEL		"DATA/MODEL/cube.x"							// 読み込むモデル名

#define CUBE_MAX		(2)
#define CUBE_SCALE		(0.03f)
#define CUBE_SIZE		(20.0f)
#define CUBE_ROT		(0.02f)

typedef struct
{
	D3DXVECTOR3		pos;			// 地面の位置
	D3DXVECTOR3		rot;			// 地面の向き(回転)
	D3DXVECTOR3		scl;			// 地面の大きさ(スケール)

	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス

	bool			bUse;
}CUBE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCube(int nType);
void UninitCube(void);
void UpdateCube(void);
void DrawCube(void);
int SetCube(void);
void SetCubePos(int nIndex, D3DXVECTOR3 pos);
void SetCubeColor(int nColor);
void ReleaseCube(int nIndex);
CUBE *GetCube(int no);

#endif
