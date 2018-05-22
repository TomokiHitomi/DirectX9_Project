//=============================================================================
//
// フィールド処理 [field.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/snow.jpg"						// 読み込むテクスチャファイル名

#define	FIELD_SIZE_X		(100.0f)			// 地面のサイズ(X方向)
#define	FIELD_SIZE_Z		(100.0f)			// 地面のサイズ(Z方向)

#define FIELD_X				(15)
#define FIELD_Z				(15)
#define FIELD_MAX			(FIELD_X * FIELD_Z)	// フィールドの最大数

typedef struct
{
	D3DXVECTOR3				posField;			// 地面の位置
	D3DXVECTOR3				rotField;			// 地面の向き(回転)
	D3DXVECTOR3				sclField;			// 地面の大きさ(スケール)

	D3DXMATRIX				mtxView;			// ビューマトリックス
	D3DXMATRIX				mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX				mtxWorld;			// ワールドマトリックス
}FIELD;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitField(int nType);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
FIELD *GetField(int no);

#endif
