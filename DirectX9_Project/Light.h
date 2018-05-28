//=============================================================================
//
// ライト処理 [light.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	LIGHT_MAX				(6)						// ライトの数

//#define LIGHT_DIFFUSE_0			(1.0f)
//#define LIGHT_DIFFUSE_1			(0.2f)
#define LIGHT_DIFFUSE_2			(0.1f)
//#define LIGHT_AMBIENT			(0.6f)

#define LIGHT_DIFFUSE_0			(1.0f)
#define LIGHT_DIFFUSE_1			(1.0f)
#define LIGHT_AMBIENT			(0.1f)


#define LIGHT_SUB_DIFFUSE		(0.3f)
#define LIGHT_SUB_XZ			(0.8)
#define LIGHT_SUB_Y				(1.0f)

#define LIGHT_AMBIENT_POINT		(0.1f)
#define LIGHT_AMBIENT_SPOT		(0.1f)

#ifdef _DEBUG
#define LIGHT_POINT_MAX			(2)
#define LIGHT_POINT_ROT_LENGTH	(25.0f)
#define LIGHT_POINT_ROT_SPEED_H	(0.03f)
#define LIGHT_POINT_ROT_SPEED_V	(0.01f)
#endif

//*****************************************************************************
// 構造体定義
//*****************************************************************************
#ifdef _DEBUG
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posCenter;
	float fHAngle;
	float fVAngle;
	int nCubeIdx;
}POINTLIGHT;
#endif
//*****************************************************************************
// 列挙型定義
//*****************************************************************************
enum
{
	LIGHT_MAIN,
	LIGHT_SUB,
	LIGHT_SPOT_UP,
	LIGHT_SPOT_CAMERA,
	LIGHT_POINT_1,
	LIGHT_POINT_2,

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLight(void);
void SetLight(int nLight, bool bFlag);
void SetLightPoint(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 pos,
	FLOAT fRange, FLOAT fAttenuation);
void SetLightSpot(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 pos, D3DXVECTOR3 vecDir,
	FLOAT fRange, FLOAT fAttenuation, FLOAT fTheta, FLOAT fPhi);
void SetLightDirectional(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 vecDir);

#ifdef _DEBUG
void UpdateDebugLight(void);
#endif

#endif
