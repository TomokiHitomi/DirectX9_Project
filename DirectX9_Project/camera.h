//=============================================================================
//
// カメラ処理 [camera.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MAX				(2)
#define CAMERA_ANGLE_MARGIN		(0.1f)

#define CAMERA_MOVE_SPEED		(7)
#define CAMERA_MOVE_SLANT		(CAMERA_MOVE_SPEED * 0.71f)
#define CAMERA_ROT_SPEED		(0.05f)
#define CAMERA_ROT_SLANT		(CAMERA_ROT_SPEED * 0.71f)
#define CAMERA_ROT_SPEED_AUTO	(0.2f)


#define CAMERA_V_ANGLE			(D3DX_PI)
#define CAMERA_H_ANGLE			(D3DX_PI * 0.5f)

#define CAMERA_H_ANGLE_MARGIN	(D3DX_PI / 180 * 45)
#define CAMERA_MARGIN_SPEED		(D3DX_PI / 180)
#define CAMERA_LENGTH_SPEED			(10.0f)
#define CAMERA_LENGTH_AUTO_SPEED	(30.0f)

#define CAMERA_ROT_MOUSE_X			(0.002f)
#define CAMERA_ROT_MOUSE_Y			(0.002f)

#define CAMERA_HEIGHT_SUSPENSION	(3)

#define ROOT_2					(1.414213f)

#define	POS_X_CAM				(-1000.0f)										// カメラの初期位置(X座標)
#define	POS_Y_CAM				(500.0f)										// カメラの初期位置(Y座標)
#define	POS_Z_CAM				(0.0f)										// カメラの初期位置(Z座標)

#define	VIEW_ANGLE				(D3DXToRadian(35.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z				(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z				(5000.0f)										// ビュー平面のFarZ値

/************************* GAME *************************/
#define CAMERA_GAME_ANGLE						(D3DX_PI*0.5f)
#define CAMERA_GAME_LENGTH						(30.0f)
#define CAMERA_GAME_HEIGHT						(35.0f)

#define CAMERA_LENGTH_GAME_NORMAL				(200.0f)
#define CAMERA_LENGTH_GAME_TPS					(150.0f)
#define CAMERA_LENGTH_GAME_MIN					(50.0f)
#define CAMERA_LENGTH_GAME_MAX					(500.0f)
#define CAMERA_LENGTH_GAME_SPEED				(2.0f)
#define CAMERA_V_ANGLE_GAME						(4.71f)
#define CAMERA_V_ANGLE_GAME_MAX					(6.28f)
#define CAMERA_V_ANGLE_GAME_MIN					(3.16f)

/* 追従カメラ設定 */
#define CAMERA_GAME_MOVE_SPEED					(0.25f)	// 移動中のカメラ移動スピード
#define CAMERA_GAME_MOVE_SPEED_BOOST			(1.0f)	// ブースト中のカメラ移動スピード
#define CAMERA_GAME_MOVE_SPEED_AUTO				(0.25f)	// 自動カメラ移動スピード
/* 通常移動時 */
#define CAMERA_GAME_MOVE_MARGIN_FRONT			(40.0f)	// 前方移動限界値
#define CAMERA_GAME_MOVE_MARGIN_LEAR			(10.0f)	// 後方移動限界値
#define CAMERA_GAME_MOVE_MARGIN_SIDE			(50.0f) // 左右移動限界値
#define CAMERA_GAME_MOVE_MARGIN_SIDE_STAY		(40.0f) // 追従カメラ固定位置
/* ブースト時 */
#define CAMERA_GAME_MOVE_MARGIN_BOOST_FRONT		(85.0f)	// 前方移動限界値
#define CAMERA_GAME_MOVE_MARGIN_BOOST_LEAR		(25.0f)	// 後方動限界値
#define CAMERA_GAME_MOVE_MARGIN_BOOST_SIDE		(60.0f) // 左右移動限界値

/************************* TITLE *************************/
#define CAMERA_TITLE_HEIGHT			(40.0f)
#define CAMERA_TITLE_V_ANGLE		(-D3DX_PI*1.5f)
#define CAMERA_TITLE_H_ANGLE		(D3DX_PI*0.5f)
#define CAMERA_TITLE_LENGTH			(40.0f)
#define CAMERA_TITLE_LENGTH_SIDE	(9.0f)

/************************* RESULT *************************/
#define CAMERA_RESULT_HEIGHT		(40.0f)
#define CAMERA_RESULT_V_ANGLE		(-D3DX_PI*1.5f)
#define CAMERA_RESULT_H_ANGLE		(D3DX_PI)
#define CAMERA_RESULT_LENGTH		(35.0f)
#define CAMERA_RESULT_LENGTH_SIDE	(9.0f)

/************************* VIEW *************************/
#define CAMERA_LENGTH_VIEW		(15.0f)
#define CAMERA_V_ANGLE_VIEW		(D3DX_PI * 1.65f)
#define CAMERA_V_ANGLE_VIEW_MAX	(6.28f)
#define CAMERA_V_ANGLE_VIEW_MIN	(3.15f)



typedef struct
{
	D3DXVECTOR3			posCameraEye;				// カメラの視点
	D3DXVECTOR3			posCameraAt;				// カメラの注視点
	D3DXVECTOR3			vecCameraUp;				// カメラの上方向ベクトル
	D3DXVECTOR3			rotCamera;					// カメラの回転
	D3DXVECTOR3			vecTag;

	D3DXVECTOR3			posCameraAtTps;					// ポジション
	D3DXVECTOR3			posCameraAtNormal;				// ポジション

	D3DXVECTOR3			vecCameraAtPos;

	D3DXMATRIX			mtxView;				// ビューマトリックス
	D3DXMATRIX			mtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX			mtxWorld;				// ワールドマトリックス

	float				fHAngleMargin;
	float				fHAngle;
	float				fVAngle;
	float				fHAngleDiff;
	float				fVAngleDiff;
	float				fLength;
	float				fLengthTemp;
	float				fMoveSpeed;
	float				fRotSpeed;

	int					nCameraMode;
	int					nSetCount;
	int					nMode;

	bool				bCameraReverse;
}CAMERA;

enum
{
	CAMERA_GAME,
	CAMERA_VIEW
};

enum
{
	CAMERA_NORMAL,
	CAMERA_TPS,
	CAMERA_TARGET
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
D3DXMATRIX GetInvRotateMat(D3DXVECTOR3 pos);
D3DXVECTOR3 GetRotCamera(void);
D3DXMATRIX GetMtxView(void);
D3DXMATRIX GetMtxProjection(void);
CAMERA *GetCamera(int no);
int *GetCameraMode(void);
void SetCameraGameMode(int no);
int *GetCameraGameMode(void);
void SetCameraTag(int no);
void SetCameraModeLength(int nCamera);
#endif