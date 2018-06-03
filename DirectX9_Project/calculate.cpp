//=============================================================================
//
// 計算処理 [calculate.cpp]
// Author : GP12A295 25 人見友基
//
// 計算処理全般を行うcpp
//
//=============================================================================
#include "calculate.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//=============================================================================
// 内積
//=============================================================================
float DotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	float ans = 0;
	//ans = (vl->x * vr->x) + (vl->y * vr->y) + (vl->z * vr->z);
	ans = D3DXVec3Dot(vl, vr);
	return(ans);
}

//=============================================================================
// 外積
//=============================================================================
D3DXVECTOR3 *CrossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	//ret->x = (vl->y * vr->z) - (vl->z * vr->y);
	//ret->y = (vl->z * vr->x) - (vl->x * vr->z);
	//ret->z = (vl->x * vr->y) - (vl->y * vr->x);
	D3DXVec3Cross(ret, vl, vr);
	return(ret);
}

//=============================================================================
// XZの角度算出処理（atan2エラー防止付き）
//=============================================================================
float PreventionAtan2XZ(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vecTa.x = pos1.x - pos2.x;
	vecTa.z = pos1.z - pos2.z;

	// atan2のエラー防止
	if (vecTa.x == 0 && vecTa.z == 0)
	{
		vecTa.x += 0.00001;
	}

	// 角度計算
	float fHAngle = atan2(vecTa.z, vecTa.x);

	return fHAngle;
}

//=============================================================================
// PI調整処理
//=============================================================================
float PiCalculateTitle(float fAngle)
{
	// 90度を超えたら360度を引く
	if (fAngle >= D3DX_PI * 0.53f)
	{
		fAngle -= D3DX_PI;
	}
	else if (fAngle < -D3DX_PI * 0.53f)
	{
		fAngle += D3DX_PI;
	}
	return fAngle;
}

//=============================================================================
// PI調整処理（180度）
//=============================================================================
float PiCalculate180(float fAngle)
{
	// 180度を超えたら360度を引く
	if (fAngle >= D3DX_PI)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	else if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
	return fAngle;
}

//=============================================================================
// PI調整処理（360度）
//=============================================================================
float PiCalculate360(float fAngle)
{
	// 360度を超えたら360度を引く
	if (fAngle > D3DX_PI * 2)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	else if (fAngle < 0.0f)
	{
		fAngle += D3DX_PI * 2.0f;
	}
	return fAngle;
}

//=============================================================================
// クォータニオン処理
//=============================================================================
void QuaternionCalculate(D3DXVECTOR3 *pOrigin, D3DXVECTOR3 *pAxis, float fAngle, D3DXVECTOR3 *pPos)
{
	// 反数、処理結果用の変数を定義
	D3DXQUATERNION qReversal, qAnswer;
	// 回転させたい点の初期位置
	D3DXQUATERNION qOrigin(pOrigin->x, pOrigin->y, pOrigin->z, 0.0f);
	// 単位クォータニオン
	D3DXQUATERNION qQuaternion(0.0f, 0.0f, 0.0f, 1.0f);

	// 軸pAxis回りの回転クォータニオンを生成
	D3DXQuaternionRotationAxis(&qQuaternion, pAxis, fAngle);
	// XYZマイナスの共役を算出
	D3DXQuaternionConjugate(&qReversal, &qQuaternion);
	// qReversal・qOrigin・qQuaternionを行列計算
	D3DXQuaternionMultiply(&qAnswer, &qReversal, &qOrigin);
	// qAnswerに回転処理後の結果を格納
	D3DXQuaternionMultiply(&qAnswer, &qAnswer, &qQuaternion);

	// 回転処理の結果を反映
	pPos->x = qAnswer.x;
	pPos->y = qAnswer.y;
	pPos->z = qAnswer.z;
}

//=============================================================================
// カラーパレット呼び出し処理
//=============================================================================
D3DXCOLOR SetColorPallet(int nColor)
{
	D3DXCOLOR temp;
	//temp = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	switch (nColor)
	{
	case COLOR_PALLET_WHITE:		// ホワイト
		temp = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_RED:			// レッド
		temp = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_YELLOW:		// イエロー
		temp = D3DXCOLOR(0.9f, 0.9f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_GREEN:		// グリーン
		temp = D3DXCOLOR(0.0f, 1.0f, 0.05f, 1.0f);
		break;
	case COLOR_PALLET_CYAN:			// シアン
		temp = D3DXCOLOR(0.0f, 0.9f, 0.9f, 1.0f);
		break;
	case COLOR_PALLET_BLUE:			// ブルー
		temp = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_MAGENTA:		// マゼンダ
		temp = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_BROWN:		// ブラウン
		temp = D3DXCOLOR(0.6f, 0.4f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_LIGHTGREEN:	// ライトグリーン
		temp = D3DXCOLOR(0.3f, 0.9f, 0.3f, 1.0f);
		break;
	case COLOR_PALLET_ORANGE:		// オレンジ
		temp = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_BLACK:		// ブラック
		temp = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_LIGHTBLUE:	// ブルー
		temp = D3DXCOLOR(0.4f, 0.7f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_GRAY:			// グレイ
		temp = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		break;
	}
	return temp;
}

//=============================================================================
// int配列のシャッフル関数
//=============================================================================
void SetShuffle(int *nShuffle, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		int j = rand() % nSize;
		int t = nShuffle[i];
		nShuffle[i] = nShuffle[j];
		nShuffle[j] = t;
	}
}