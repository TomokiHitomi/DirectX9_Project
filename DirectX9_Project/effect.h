//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	TEXTURE_EFFECT	"data/TEXTURE/effect000.jpg"						// 読み込むテクスチャファイル名
#define	TEXTURE_EFFECT	"DATA/TEXTURE/effect.png"						// 読み込むテクスチャファイル名
#define SHADER_EFFECT	"sprite.fx"

// テクスチャ内分割数
#define TEXTURE_PATTERN_DIVIDE_X_EFFECT	(1)
#define TEXTURE_PATTERN_DIVIDE_Y_EFFECT	(1)
// テクスチャ分割パターン
#define TEXTURE_PATTERN_NU_EFFECT		(TEXTURE_PATTERN_DIVIDE_X_EFFECT*TEXTURE_PATTERN_DIVIDE_Y_EFFECT)

// αテスト値
#define	EFFECT_ALPHA		(30)

// サイズ
#define	EFFECT_SIZE_X		(10.0f)
#define	EFFECT_SIZE_Y		(10.0f)

#define EFFECT_SIZE_CHANGE	(0.2f)
#define EFFECT_ALPHA_CHANGE	(0.04f)

// 最大数
#define EFFECT_MAX			(100000)

// RGBA
#define EFFECT_DIF_R		(0.8f)
#define EFFECT_DIF_G		(0.8f)
#define EFFECT_DIF_B		(0.0f)
#define EFFECT_DIF_A		(1.0f)


typedef struct
{
	D3DXVECTOR3				posEffect;				// 地面の位置
	D3DXVECTOR3				rotEffect;				// 地面の向き(回転)
	D3DXVECTOR3				sclEffect;				// 地面の大きさ(スケール)

	D3DXVECTOR2				vec2Size;

	D3DXCOLOR				colorEffect;

	int						nUseCount;				// 消滅カウント
	int						nTex;

	float					fSizeChange;
	float					fAlphaChange;
	FLOAT					fSize;

	bool					bUse;					// 使用フラグ
}EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(int nType);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(int nTex, D3DXVECTOR2 vec2Size, D3DXCOLOR color, D3DXVECTOR3 vecPos, float fSizeChange, float fAlphaChange);

#endif
