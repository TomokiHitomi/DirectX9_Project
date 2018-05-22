//=============================================================================
//
// 計算処理 [calculate.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _CALCULATE_H_
#define _CALCULATE_H_

#include "main.h"

//*****************************************************************************
// 列挙型定義
//*****************************************************************************
enum
{
	COLOR_PALLET_WHITE,
	COLOR_PALLET_RED,
	COLOR_PALLET_YELLOW,
	COLOR_PALLET_GREEN,
	COLOR_PALLET_CYAN,
	COLOR_PALLET_BLUE,
	COLOR_PALLET_MAGENTA,
	COLOR_PALLET_BROWN,
	COLOR_PALLET_LIGHTGREEN,
	COLOR_PALLET_ORANGE,
	COLOR_PALLET_BLACK,
	COLOR_PALLET_LIGHTBLUE,
	COLOR_PALLET_GRAY
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float DotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr);
D3DXVECTOR3 *CrossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr);
float PreventionAtan2XZ(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
float PiCalculateTitle(float fAngle);
float PiCalculate180(float fAngle);
float PiCalculate360(float fAngle);
void QuaternionCalculate(D3DXVECTOR3 *pInit, D3DXVECTOR3 *pC, float Rad, D3DXVECTOR3 *ret);
D3DXCOLOR SetColorPallet(int nColor);
void SetShuffle(int *nShuffle, int nSize);

#endif
