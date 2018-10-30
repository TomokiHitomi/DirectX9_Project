//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "calculate.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
D3DXVECTOR3 GetLookAtVct(void);
void SetCameraAt(void);
void MoveCamera(int no);
void MoveCameraFunc(int no, float fAngle);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA					cameraWk[CAMERA_MAX];
int						g_nCameraMode = CAMERA_GAME;
int						g_nCameraGameMode = CAMERA_NORMAL;
int						g_nCameraTag = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	CAMERA *camera = &cameraWk[0];

	camera->fHAngleMargin = 0.0f;
	camera->bCameraReverse = false;
	camera->fMoveSpeed = CAMERA_MOVE_SPEED;
	camera->nSetCount = 0;
	camera->nMode = CAMERA_NORMAL;
	camera->posCameraEye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	camera->posCameraAt = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);
	camera->vecCameraAtPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	camera->posCameraAtTps = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);
	camera->posCameraAtNormal = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);
	camera->fVAngle = CAMERA_V_ANGLE_GAME;
	camera->fHAngle = CAMERA_H_ANGLE;
	camera->fVAngleDiff = CAMERA_V_ANGLE_GAME;
	camera->fHAngleDiff = CAMERA_H_ANGLE;
	camera->fLength = CAMERA_LENGTH_GAME_NORMAL;
	camera->fLengthTemp = CAMERA_LENGTH_GAME_NORMAL;

	g_nCameraMode = CAMERA_GAME;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCamera(void)
{
	CAMERA	*camera = &cameraWk[0];

	// カメラ上下反転用
	int nCameraReverse = 1;

	// クォータニオン用変数
	// 回転半径を設定
	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, camera->fLength, 0.0f);
	// 現在の角度に垂直な回転軸ベクトルを設定
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0, sinf(camera->fHAngle + D3DX_PI * 0.5f));


	// 斜め回転時に速度減算
	if ( ((GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN)) && (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT)))
		|| ((IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, BUTTON_DOWN)) && (IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, BUTTON_RIGHT)))
		|| ((IsButtonPressed(0, LSTICK_UP) || IsButtonPressed(0, LSTICK_DOWN)) && (IsButtonPressed(0, LSTICK_LEFT) || IsButtonPressed(0, LSTICK_RIGHT)))
		)
	{
		camera->fRotSpeed = CAMERA_ROT_SLANT;
	}
	else
	{
		camera->fRotSpeed = CAMERA_ROT_SPEED;
	}

	// カメラ左右回転（キーボード）
	if (GetKeyboardPress(DIK_LEFT) && GetKeyboardPress(DIK_RIGHT))
	{
		// 同時押しは処理なし
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		camera->fHAngleDiff += camera->fRotSpeed;
		//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
	}
	else if (GetKeyboardPress(DIK_RIGHT))
	{
		camera->fHAngleDiff -= camera->fRotSpeed;
		//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
	}

	// カメラ左右回転（マウス）
	if ((float)GetMobUseX() && !IsButtonPressed(0, R_BUTTON_X))
	{
		camera->fHAngleDiff -= ((float)GetMobUseX()) * CAMERA_ROT_MOUSE_X;
		//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
	}

	// カメラ上下回転（キーボード）
	if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
	{
		// 同時押しは処理なし
	}
	else if (GetKeyboardPress(DIK_UP))
	{
		camera->fVAngleDiff += camera->fRotSpeed * nCameraReverse;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		camera->fVAngleDiff -= camera->fRotSpeed * nCameraReverse;
	}
	// カメラ上下回転（マウス）
	if ((float)GetMobUseY() && !IsButtonPressed(0, R_BUTTON_X))
	{
		camera->fVAngleDiff -= ((float)GetMobUseY()) * CAMERA_ROT_MOUSE_Y * nCameraReverse;
	}

	// マウスホイール
	//long ModUseZ = GetMobUseZ();
	//if (ModUseZ != 0)
	//{
	//	camera->fLength -= ModUseZ * CAMERA_LENGTH_SPEED;
	//	if (camera->fLength < CAMERA_LENGTH_MIN) { camera->fLength = CAMERA_LENGTH_MIN; }
	//	else if (camera->fLength > CAMERA_LENGTH_MAX) { camera->fLength = CAMERA_LENGTH_MAX; }
	//}

	camera->fVAngle += (camera->fVAngleDiff - camera->fVAngle) * CAMERA_ROT_SPEED_AUTO;
	camera->fHAngle += (camera->fHAngleDiff - camera->fHAngle) * CAMERA_ROT_SPEED_AUTO;

	// カメラ可動限界
	if (camera->fVAngle > CAMERA_V_ANGLE_GAME_MAX)
	{
		camera->fVAngle = CAMERA_V_ANGLE_GAME_MAX;
		camera->fVAngleDiff = CAMERA_V_ANGLE_GAME_MAX;
	}
	else if (camera->fVAngle < CAMERA_V_ANGLE_GAME_MIN)
	{
		camera->fVAngle = CAMERA_V_ANGLE_GAME_MIN;
		camera->fVAngleDiff = CAMERA_V_ANGLE_GAME_MIN;
	}

	MoveCamera(0);

	// クォータニオン処理
	QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
		&camera->posCameraAt);
	// 回転軸に設置
	camera->posCameraAt += camera->posCameraEye;

#ifdef _DEBUG
	PrintDebugProc("【CANERA】\n");
	PrintDebugProc("CameraEye[X:%f Y : %f Z : %f]\n",
		camera->posCameraEye.x, camera->posCameraEye.y, camera->posCameraEye.z);
	PrintDebugProc("CameraAt [X:%f Y : %f Z : %f]\n",
		camera->posCameraAt.x, camera->posCameraAt.y, camera->posCameraAt.z);
	PrintDebugProc("CameraAng[V:%f  H:%f]  CameraLength[%f]\n",
		camera->fVAngle, camera->fHAngle, camera->fLength);
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCameraAt(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];

	// ターゲットが0以外の場合はエネミーにセット
	if (g_nCameraTag != 0)
	{
		//enemy = GetEnemy(g_nCameraTag - 1);
		//// カメラ注視点をエネミーにセット
		//camera->posCameraAt = enemy->posEnemy;
		//// カメラ注視点の高さを調整
		//camera->posCameraAt.y = enemy->posEnemy.y + MODEL_HEIGHT_EYE;

		g_nCameraGameMode = CAMERA_TARGET;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	/******************** ビューイング変換 ********************/
	// ビューマトリクスの初期化
	D3DXMatrixIdentity(&camera->mtxView);

	// ビューマトリクスの作成
	D3DXMatrixLookAtLH(&camera->mtxView,
		&camera->posCameraEye,	// 視点
		&camera->posCameraAt,	// 注視点
		&camera->vecCameraUp);	// 上方向

								// ビューマトリクスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);


	/******************** プロジェクション変換 ********************/
	// プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&camera->mtxProjection);

	// プロジェクションマトリクスの作成
	D3DXMatrixPerspectiveFovLH(&camera->mtxProjection,
		VIEW_ANGLE,			// ビュー平面の視野角
		VIEW_ASPECT,		// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値（近いと描画しない）
		VIEW_FAR_Z);		// ビュー平面のFarZ値（遠いと描画しない）

							// プロジェクションマトリクスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
}

//=============================================================================
// カメラと対になる回転行列を取得
//=============================================================================
D3DXMATRIX GetInvRotateMat(D3DXVECTOR3 pos)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];

	D3DXMATRIX Inv;
	D3DXMatrixIdentity(&Inv);
	D3DXMatrixLookAtLH(&Inv, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &GetLookAtVct(), &camera->vecCameraUp);
	D3DXMatrixInverse(&Inv, NULL, &Inv);

	return Inv;
}

//=============================================================================
// カメラと対になるベクトルを取得
//=============================================================================
D3DXVECTOR3 GetLookAtVct(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	D3DXVECTOR3	vecLookAtVct;
	vecLookAtVct = camera->posCameraEye - camera->posCameraAt;
	return vecLookAtVct;
}

//=============================================================================
// カメラの向きの取得
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	return camera->rotCamera;
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	return camera->mtxView;
}

//=============================================================================
// プロジェクションマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxProjection(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	return camera->mtxProjection;
}


//=============================================================================
// カメラ取得関数
//=============================================================================
CAMERA *GetCamera(int no)
{
	return(&cameraWk[no]);
}

//=============================================================================
// カメラモード変更関数
//=============================================================================
void SetCameraMode(int no)
{
	CAMERA *camera = &cameraWk[0];
	g_nCameraMode = no;
}

//=============================================================================
// カメラモード取得関数
//=============================================================================
int *GetCameraMode(void)
{
	return(&g_nCameraMode);
}


//=============================================================================
// カメラモード変更関数
//=============================================================================
void SetCameraGameMode(int no)
{
	CAMERA *camera = &cameraWk[0];

	// モード切り替えが行われる場合
	if (g_nCameraGameMode != no)
	{

	}
}

//=============================================================================
// ゲームカメラモード取得関数
//=============================================================================
int *GetCameraGameMode(void)
{
	return(&g_nCameraGameMode);
}

//=============================================================================
// カメラターゲット変更関数
//=============================================================================
void SetCameraTag(int no)
{
	CAMERA *camera = &cameraWk[0];
	g_nCameraTag = no;
}

//=============================================================================
// カメラ移動関数
//=============================================================================
void MoveCamera(int no)
{
	CAMERA *camera = &cameraWk[no];
	// 移動処理
	if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_LEFT))
	{
		if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
		{// 左前移動
			MoveCameraFunc(no, camera->fHAngle + D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{// 左後移動
			MoveCameraFunc(no, camera->fHAngle + D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
		{// 左右同時押しは処理なし
		}
		else
		{// 左移動
			MoveCameraFunc(no, camera->fHAngle + D3DX_PI * 0.50f);
		}
		//SetPlayerAnime(0, ANIME05);
	}
	else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
	{
		if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_UP))
		{// 右前移動
			MoveCameraFunc(no, camera->fHAngle - D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{// 右後移動
			MoveCameraFunc(no, camera->fHAngle - D3DX_PI * 0.75f);
		}
		else
		{// 右移動
			MoveCameraFunc(no, camera->fHAngle - D3DX_PI * 0.50f);
		}
	}
	else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
	{// 前移動
		if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{// 前後同時押しは処理なし
		}
		else
		{
			MoveCameraFunc(no, camera->fHAngle);
		}
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
	{// 後移動
		MoveCameraFunc(no, camera->fHAngle + D3DX_PI);
	}
}

//=============================================================================
// 移動処理関数
//=============================================================================
void MoveCameraFunc(int no, float fAngle)
{
	CAMERA *camera = &cameraWk[no];
	camera->posCameraEye.x += cosf(fAngle) * CAMERA_MOVE_SPEED;
	camera->posCameraEye.z += sinf(fAngle) * CAMERA_MOVE_SPEED;
}