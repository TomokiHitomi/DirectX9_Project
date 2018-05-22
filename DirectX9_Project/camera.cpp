//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "model.h"
#include "calculate.h"
#include "bullet.h"
#include "reticle.h"
#include "enemy.h"
#include "pause.h"
#include "player.h"

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
void SetCameraAngle(E_STAGE eStage);
void SetCameraAtMove(int nCamera);

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

	for (int i = 0; i < CAMERA_MAX; i++, camera++)
	{
		switch (g_nCameraMode)
		{
		case CAMERA_GAME:
			SetCameraAngle(GetStage());

			camera->fHAngleMargin = 0.0f;
			camera->bCameraReverse = false;

			break;
		case CAMERA_VIEW:
			camera->posCameraEye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
			camera->posCameraAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			camera->fVAngle = CAMERA_V_ANGLE * 1.3f;
			camera->fHAngle = CAMERA_H_ANGLE;
			camera->fHAngleMargin = 0.0f;
			camera->fLength = CAMERA_LENGTH_VIEW;
			camera->fLengthTemp = 0;
			break;
		}
		camera->fMoveSpeed = CAMERA_MOVE_SPEED;
		camera->nSetCount = 0;
		g_nCameraMode++;
		camera->nMode = CAMERA_NORMAL;
	}

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
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	MODEL	*model = GetModel(0);
	RETICLE *reticle = GetReticle(0);
	PLAYER *player = GetPlayer(0);
	ENEMY *enemy;

	// カメラ上下反転用
	int nCameraReverse = 1;

	D3DXVECTOR3 vecVl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecVr = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDotTest = 0.0f;

	D3DXVECTOR3 vecRet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 vecBasePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Y軸カメラサスペンション
	float fHeight = 0.0f;

	// Joy-con用ジャイロ
	D3DXVECTOR3 vecGyro = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// クォータニオン用変数
	// 回転半径を設定
	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, camera->fLength, 0.0f);
	// 現在の角度に垂直な回転軸ベクトルを設定
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0, sinf(camera->fHAngle + D3DX_PI * 0.5f));


	switch (GetStage())
	{
	case STAGE_TITLE:
		// TPS注視点を計算
		camera->posCameraAtTps = D3DXVECTOR3(
			(model->posModel.x + CAMERA_TITLE_LENGTH_SIDE * cos(camera->fHAngle + D3DX_PI * 1.5f)),
			model->posModel.y + CAMERA_TITLE_HEIGHT,
			(model->posModel.z + CAMERA_TITLE_LENGTH_SIDE * sin(camera->fHAngle + D3DX_PI * 1.5f)));


		camera->fHAngle += 0.003f;
		camera->fHAngle = PiCalculate360(camera->fHAngle);
		camera->fVAngle += 0.001f;
		camera->fVAngle = PiCalculateTitle(camera->fVAngle);

		if (camera->fVAngle < -0.6f && camera->fVAngle > -0.7f)
		{
			camera->fVAngle += 1.3f;
		}
#ifdef _DEBUG
		PrintDebugProc("Angle[V:%f  H:%f]\n", camera->fVAngle, camera->fHAngle);
#endif


		// カメラ注視点をセット
		camera->posCameraAt += (camera->posCameraAtTps - camera->posCameraAt) / 10;

		// クォータニオン処理
		QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
			&camera->posCameraEye);
		// 回転軸に設置
		camera->posCameraEye += camera->posCameraAt;



		break;
	case STAGE_GAME:
		if (!GetPauseFlag())
		{
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

			// カメラ左右回転（Joy-con）
			if (IsButtonPressed(0, RSTICK_LEFT) || IsButtonPressed(0, RSTICK_RIGHT))
			{
				camera->fHAngleDiff -= CAMERA_ROT_SPEED * GetStick(0, PAD_STICK_R_X);
				//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
			}

			// カメラモードでスイッチ
			switch (g_nCameraMode)
			{
			case CAMERA_GAME:	// GAMEカメラモード

				// カメラ上下反転
				if (GetKeyboardTrigger(DIK_F2))
				{
					camera->bCameraReverse = camera->bCameraReverse ? false : true;
				}
				if (camera->bCameraReverse)
				{
					nCameraReverse *= -1;
				}

				// カメラ上下回転（キーボード）
				if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
				{
					// 同時押しは処理なし
				}
				else if (GetKeyboardPress(DIK_UP))
				{
					camera->fVAngleDiff -= camera->fRotSpeed * nCameraReverse;
				}
				else if (GetKeyboardPress(DIK_DOWN))
				{
					camera->fVAngleDiff += camera->fRotSpeed * nCameraReverse;
				}
				// カメラ上下回転（マウス）
				if ((float)GetMobUseY() && !IsButtonPressed(0, R_BUTTON_X))
				{
					camera->fVAngleDiff += ((float)GetMobUseY()) * CAMERA_ROT_MOUSE_Y * nCameraReverse;
				}

				//// カメラ上下回転（Joy-con）
				//if (IsButtonPressed(0, RSTICK_UP) || IsButtonPressed(0, RSTICK_DOWN))
				//{
				//	camera->fVAngleDiff += CAMERA_ROT_SPEED  * GetStick(0, PAD_STICK_R_Y) * nCameraReverse;
				//}

				//// 照準モード切替（押下中TPSモード）
				//if (IsMobUseRightPressed())
				//{
				//	SetCameraGameMode(CAMERA_TPS);
				//}
				//else if (g_nCameraGameMode == CAMERA_TPS)
				//{
				//	SetCameraGameMode(CAMERA_NORMAL);
				//}

				if (player->bShot)
				{
					SetCameraModeLength(CAMERA_TPS);
				}
				else
				{
					SetCameraModeLength(CAMERA_NORMAL);
				}


				// Joy-con縦アングルリセット
				if (IsButtonPressed(0, R_BUTTON_Y))
				{
					camera->fVAngleDiff = CAMERA_V_ANGLE_GAME;
				}

				// Joy-conジャイロ取得
				vecGyro = GetGyro();

				// Joy-conジャイロを角度に適用
				camera->fVAngleDiff -= vecGyro.x * nCameraReverse;
				camera->fHAngleDiff -= vecGyro.y;
				camera->fHAngleDiff += vecGyro.z;

				//camera->fVAngleDiff -= GetRglSlider(PAD_SLIDER_V);
				//camera->fHAngleDiff -= GetRglSlider(PAD_SLIDER_H);

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

				// 追従カメラ処理
				SetCameraAtMove(g_nCameraMode);

				fHeight = model->posModel.y - camera->posCameraAt.y + MODEL_HEIGHT_EYE;
				//if (fHeight > 0.0f)
				//{
					fHeight = camera->posCameraAt.y + fHeight / CAMERA_HEIGHT_SUSPENSION;
				//}
				//else if (fHeight < 0.0f)
				//{
				//	fHeight = camera->posCameraAt.y - fHeight / CAMERA_HEIGHT_SUSPENSION;
				//}

				// カメラ注視点をセット
				camera->posCameraAt = D3DXVECTOR3(
					(model->posModel.x
						+ (camera->vecCameraAtPos.x * cos(camera->fHAngle + D3DX_PI * 1.5f))
						+ (camera->vecCameraAtPos.z * cos(camera->fHAngle + D3DX_PI * 2.0f))),
					fHeight,
					(model->posModel.z
						+ (camera->vecCameraAtPos.x * sin(camera->fHAngle + D3DX_PI * 1.5f))
						+ (camera->vecCameraAtPos.z * sin(camera->fHAngle + D3DX_PI * 2.0f))));

				// クォータニオン処理
				QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
					&camera->posCameraEye);
				// 回転軸に設置
				camera->posCameraEye += camera->posCameraAt;

				enemy = GetEnemy(0);

				vecVl = camera->posCameraEye - camera->posCameraAt;
				vecVr = camera->posCameraEye - enemy->posEnemy;

				//vecVl.y = 0.0f;
				//vecVr.y = 0.0f;

				//vecVl.x = 0.0f;
				//vecVr.x = 0.0f;
				//vecVl.z = 0.0f;
				//vecVr.z = 0.0f;

				D3DXVec3Normalize(&vecVr, &vecVr);
				D3DXVec3Normalize(&vecVl, &vecVl);

				fDotTest = DotProduct(&vecVl, &vecVr);
#ifdef _DEBUG
				PrintDebugProc("CameraDot[%f]\n", fDotTest);
#endif


				*CrossProduct(&vecRet, &vecVl, &vecVr);
#ifdef _DEBUG
				PrintDebugProc("CameraCross[X:%f Y:%f Z:%f]\n", vecRet.x, vecRet.y, vecRet.z);
#endif

				//if (fDotTest < 0.999f)
				//{
				//	camera->fHAngle += 0.01f;
				//}


				break;

			case CAMERA_VIEW:	// ビューカメラモード
				/******************** カメラ視点移動 ********************/


				// カメラ上下回転
				if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
				{
					// 同時押しは処理なし
				}
				else if (GetKeyboardPress(DIK_UP))
				{
					//if (camera->fVAngle < CAMERA_V_ANGLE_GAME_MIN)
					//{
					//	camera->fVAngle = CAMERA_V_ANGLE_GAME_MIN;
					//}
					camera->fVAngle -= camera->fRotSpeed;
				}
				else if (GetKeyboardPress(DIK_DOWN))
				{
					//if (camera->fVAngle > CAMERA_V_ANGLE_GAME_MAX)
					//{
					//	camera->fVAngle = CAMERA_V_ANGLE_GAME_MAX;
					//}
					camera->fVAngle += camera->fRotSpeed;
				}

				if ((float)GetMobUseY())
				{
					camera->fVAngle -= ((float)GetMobUseY()) * CAMERA_ROT_MOUSE_Y;
				}

				// カメラ可動限界
				if (camera->fVAngle > CAMERA_V_ANGLE_VIEW_MAX)
				{
					camera->fVAngle = CAMERA_V_ANGLE_VIEW_MAX;
				}
				else if (camera->fVAngle < CAMERA_V_ANGLE_VIEW_MIN)
				{
					camera->fVAngle = CAMERA_V_ANGLE_VIEW_MIN;
				}

				// 斜め移動時に移動速度減算
				if ((GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S)) && (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D)))
				{
					camera->fMoveSpeed = CAMERA_MOVE_SLANT;
				}
				else
				{
					camera->fMoveSpeed = CAMERA_MOVE_SPEED;
				}

				// Y座標上昇
				if (GetKeyboardPress(DIK_Q) && GetKeyboardPress(DIK_E))
				{
					// 同時押しは処理なし
				}
				// 上昇
				if (GetKeyboardPress(DIK_Q))
				{
					camera->posCameraEye.y += 2.0f;
				}
				// 下降
				else if (GetKeyboardPress(DIK_E))
				{
					camera->posCameraEye.y -= 2.0f;
				}

				// 前後同時押しは移動なし
				if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_S))
				{
					// 同時押しは処理なし
				}
				// 前移動
				else if (GetKeyboardPress(DIK_W))
				{
					camera->posCameraEye.x += cosf(camera->fHAngle) * camera->fMoveSpeed;
					camera->posCameraEye.z += sinf(camera->fHAngle) * camera->fMoveSpeed;
				}
				// 後移動
				else if (GetKeyboardPress(DIK_S))
				{
					camera->posCameraEye.x += cosf(camera->fHAngle + D3DX_PI) * camera->fMoveSpeed;
					camera->posCameraEye.z += sinf(camera->fHAngle + D3DX_PI) * camera->fMoveSpeed;
				}

				// 左右同時押しは移動なし
				if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
				{
				}
				// 左移動
				else if (GetKeyboardPress(DIK_A))
				{
					camera->posCameraEye.x += cosf(camera->fHAngle + D3DX_PI * 0.5f) * camera->fMoveSpeed;
					camera->posCameraEye.z += sinf(camera->fHAngle + D3DX_PI * 0.5f) * camera->fMoveSpeed;
				}
				// 右移動
				else if (GetKeyboardPress(DIK_D))
				{
					camera->posCameraEye.x += cosf(camera->fHAngle + D3DX_PI * 1.5f) * camera->fMoveSpeed;
					camera->posCameraEye.z += sinf(camera->fHAngle + D3DX_PI * 1.5f) * camera->fMoveSpeed;
				}

				/******************** カメラ注視点移動 ********************/

				// クォータニオン処理
				QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
					&camera->posCameraAt);
				// 回転軸に設置
				camera->posCameraAt += camera->posCameraEye;
				break;
			}
		}

		// デバッグ用
#ifdef _DEBUG
		// カメラ切り替え
		if (GetKeyboardTrigger(DIK_RSHIFT))
		{
			if (g_nCameraMode)
			{
				g_nCameraMode = CAMERA_GAME;
			}
			else
			{
				g_nCameraMode = CAMERA_VIEW;
			}
		}

		camera = &cameraWk[0];
		PrintDebugProc("【CANERA】\n");

		//PrintDebugProc("Mode[%d]  GameMode[%d]  Tag[%d]\n", g_nCameraMode, g_nCameraGameMode, g_nCameraTag);

		switch (g_nCameraMode)
		{
		case CAMERA_GAME:
			PrintDebugProc("Mode[CAMERA_GAME]  ");
			break;
		case CAMERA_VIEW:
			PrintDebugProc("Mode[CAMERA_VIEW]  ");
			break;
		}

		switch (g_nCameraGameMode)
		{
		case CAMERA_NORMAL:
			PrintDebugProc("GameMode[CAMERA_NORMAL]  ");
			break;
		case CAMERA_TPS:
			PrintDebugProc("GameMode[CAMERA_TPS   ]  ");
			break;
		case CAMERA_TARGET:
			PrintDebugProc("GameMode[CAMERA_TARGET]  ");
			break;
		}
		PrintDebugProc("Tag[%d]\n", g_nCameraTag);

		for (int i = 0; i < CAMERA_MAX; i++, camera++)
		{
			PrintDebugProc("CameraEye[X:%f Y : %f Z : %f]\n",
				camera->posCameraEye.x, camera->posCameraEye.y, camera->posCameraEye.z);
			PrintDebugProc("CameraAt [X:%f Y : %f Z : %f]\n",
				camera->posCameraAt.x, camera->posCameraAt.y, camera->posCameraAt.z);
			PrintDebugProc("CameraAng[V:%f  H:%f]  Camera[MoveS:%f]  CameraLength[%f]\n",
				camera->fVAngle, camera->fHAngle, camera->fMoveSpeed, camera->fLength);
			PrintDebugProc("\n");
		}
#endif
		break;

	case STAGE_RESULT:
		// リザルト注視点を計算
		camera->posCameraAtTps = D3DXVECTOR3(
			(model->posModel.x + CAMERA_RESULT_LENGTH_SIDE * cos(camera->fHAngle + D3DX_PI * 1.5f)),
			model->posModel.y + CAMERA_RESULT_HEIGHT,
			(model->posModel.z + CAMERA_RESULT_LENGTH_SIDE * sin(camera->fHAngle + D3DX_PI * 1.5f)));


		camera->fHAngle += 0.003f;
		camera->fHAngle = PiCalculate360(camera->fHAngle);
#ifdef _DEBUG
		PrintDebugProc("Angle[V:%f  H:%f]\n", camera->fVAngle, camera->fHAngle);
#endif



		// カメラ注視点をセット
		camera->posCameraAt += (camera->posCameraAtTps - camera->posCameraAt) / 10;

		// クォータニオン処理
		QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
			&camera->posCameraEye);
		// 回転軸に設置
		camera->posCameraEye += camera->posCameraAt;

		break;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCameraAt(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	MODEL	*model = GetModel(0);
	ENEMY	*enemy;



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
// カメラターゲット変更関数
//=============================================================================
void SetCameraAngle(E_STAGE eStage)
{
	CAMERA *camera = &cameraWk[0];
	switch (eStage)
	{
	case STAGE_TITLE:
		camera->posCameraEye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		camera->posCameraAt = D3DXVECTOR3(15000.0f, 4000.0f, 10000.0f);;
		camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		camera->posCameraAtTps = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);
		camera->posCameraAtNormal = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);

		camera->fVAngle = CAMERA_TITLE_V_ANGLE;
		camera->fHAngle = CAMERA_TITLE_H_ANGLE;
		camera->fLength = CAMERA_TITLE_LENGTH;
		break;
	case STAGE_GAME:
		camera->posCameraEye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		camera->posCameraAt = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);
		camera->vecCameraAtPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		camera->posCameraAtTps = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);
		camera->posCameraAtNormal = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);

		camera->fVAngle = CAMERA_V_ANGLE_GAME;
		camera->fHAngle = CAMERA_H_ANGLE + D3DX_PI;
		camera->fVAngleDiff = CAMERA_V_ANGLE_GAME;
		camera->fHAngleDiff = CAMERA_H_ANGLE + D3DX_PI;
		camera->fLength = CAMERA_LENGTH_GAME_NORMAL;
		camera->fLengthTemp = CAMERA_LENGTH_GAME_NORMAL;
		break;
	case STAGE_RESULT:			camera->posCameraEye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		camera->posCameraAt = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);
		camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		camera->posCameraAtTps = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);
		camera->posCameraAtNormal = D3DXVECTOR3(0.0f, CAMERA_GAME_HEIGHT, 0.0f);


		camera->fVAngle = CAMERA_RESULT_V_ANGLE;
		camera->fHAngle = CAMERA_RESULT_H_ANGLE;
		camera->fLength = CAMERA_RESULT_LENGTH;
		break;
	}
}

//=============================================================================
// 追従カメラ関数
//=============================================================================
void SetCameraAtMove(int nCamera)
{
	CAMERA *camera = &cameraWk[nCamera];
	MODEL *model = GetModel(0);

	// プレイヤー追従カメラ
	if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
	{	// 前方移動時		 
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// シフトキーが押されている場合
			if (camera->vecCameraAtPos.z < CAMERA_GAME_MOVE_MARGIN_BOOST_FRONT)
			{	// ブースト最大距離以下であればブーストスピードで移動
				camera->vecCameraAtPos.z += CAMERA_GAME_MOVE_SPEED_BOOST;
			}
		}
		else
		{	// シフトキーが押されていない場合
			if (camera->vecCameraAtPos.z < CAMERA_GAME_MOVE_MARGIN_FRONT)
			{	// 通常最大距離以下であれば通常スピードで移動
				camera->vecCameraAtPos.z += CAMERA_GAME_MOVE_SPEED;
			}
			else if (camera->vecCameraAtPos.z > CAMERA_GAME_MOVE_MARGIN_FRONT)
			{	// 通常最大距離以上であればオートスピードで移動
				camera->vecCameraAtPos.z -= CAMERA_GAME_MOVE_SPEED_AUTO;
			}
		}
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
	{	// 後方移動時
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// シフトキーが押されている場合
			if (camera->vecCameraAtPos.z > -CAMERA_GAME_MOVE_MARGIN_BOOST_LEAR)
			{	// ブースト最大距離以下であればブーストスピードで移動
				camera->vecCameraAtPos.z -= CAMERA_GAME_MOVE_SPEED_BOOST;
			}
		}
		else
		{	// シフトキーが押されていない場合
			if (camera->vecCameraAtPos.z > -CAMERA_GAME_MOVE_MARGIN_LEAR)
			{	// 通常最大距離以下であれば通常スピードで移動
				camera->vecCameraAtPos.z -= CAMERA_GAME_MOVE_SPEED;
			}
			else if (camera->vecCameraAtPos.z < -CAMERA_GAME_MOVE_MARGIN_LEAR)
			{	// 通常最大距離以上であればオートスピードで移動
				camera->vecCameraAtPos.z += CAMERA_GAME_MOVE_SPEED_AUTO;
			}
		}
	}
	else
	{	// 前後移動がない
		if (camera->vecCameraAtPos.z < 0.0f)
		{	// 原点より後方にいる場合は原点までオートスピードで移動
			camera->vecCameraAtPos.z += CAMERA_GAME_MOVE_SPEED_AUTO;
		}
		if (camera->vecCameraAtPos.z > 0.0f)
		{	// 原点より前方にいる場合は原点までオートスピードで移動
			camera->vecCameraAtPos.z -= CAMERA_GAME_MOVE_SPEED_AUTO;
		}
	}

	if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_LEFT))
	{	// 左方移動時
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// シフトキーが押されている場合
			if (camera->vecCameraAtPos.x > -(CAMERA_GAME_MOVE_MARGIN_BOOST_SIDE))
			{	// ブースト最大距離以下であればブーストスピードで移動
				camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED_BOOST;
			}
		}
		else
		{	// シフトキーが押されていない場合
			if (camera->vecCameraAtPos.x > -(CAMERA_GAME_MOVE_MARGIN_SIDE))
			{	// 通常最大距離以下であれば通常スピードで移動
				camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED;
			}
			else if (camera->vecCameraAtPos.x < -(CAMERA_GAME_MOVE_MARGIN_SIDE))
			{	// 通常最大距離以上であればオートスピードで移動
				camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED_AUTO;
			}
		}
	}
	else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
	{	// 右方移動時
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// シフトキーが押されている場合
			if (camera->vecCameraAtPos.x < (CAMERA_GAME_MOVE_MARGIN_BOOST_SIDE))
			{	// ブースト最大距離以下であればブーストスピードで移動
				camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED_BOOST;
			}
		}
		else
		{	// シフトキーが押されていない場合
			if (camera->vecCameraAtPos.x < (CAMERA_GAME_MOVE_MARGIN_SIDE))
			{	// 通常最大距離以下であれば通常スピードで移動
				camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED;
			}
			else if (camera->vecCameraAtPos.x >(CAMERA_GAME_MOVE_MARGIN_SIDE))
			{	// 通常最大距離以上であればオートスピードで移動
				camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED_AUTO;
			}
		}
	}
	else
	{	// 左右移動がない
		if (camera->vecCameraAtPos.x < -CAMERA_GAME_MOVE_MARGIN_SIDE_STAY)
		{	// ステイポイントより左方にいる場合はステイポイントまでオートスピードで移動
			camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED_AUTO;
		}
		else if (camera->vecCameraAtPos.x > CAMERA_GAME_MOVE_MARGIN_SIDE_STAY)
		{	// ステイポイントより右方にいる場合はステイポイントまでオートスピードで移動
			camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED_AUTO;
		}
		else if (camera->vecCameraAtPos.x > 0.0f && camera->vecCameraAtPos.x < CAMERA_GAME_MOVE_MARGIN_SIDE_STAY)
		{	// 原点より右方、かつステイポイントより左方にいる場合はステイポイントまでオートスピードで移動
			camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED_AUTO;
		}
		else if (camera->vecCameraAtPos.x <= 0.0f && camera->vecCameraAtPos.x > -CAMERA_GAME_MOVE_MARGIN_SIDE_STAY)
		{	// 原点より左方、かつステイポイントより右方にいる場合はステイポイントまでオートスピードで移動
			camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED_AUTO;
		}
	}
}

//=============================================================================
// カメラモード変更関数
//=============================================================================
void SetCameraModeLength(int nCamera)
{
	CAMERA *camera = GetCamera(0);
	if (camera->nMode != nCamera)
	{
		camera->nMode = nCamera;
		switch (nCamera)
		{
		case CAMERA_NORMAL:
			camera->fLengthTemp = CAMERA_LENGTH_GAME_NORMAL;
			break;
		case CAMERA_TPS:
			camera->fLengthTemp = CAMERA_LENGTH_GAME_TPS;
			break;
		}
	}

	// 視点距離まで移動
	if (camera->fLength > camera->fLengthTemp)
	{
		camera->fLength -= CAMERA_LENGTH_GAME_SPEED;
		// 視点距離を越えた場合は指定距離に固定
		if (camera->fLength < camera->fLengthTemp)
		{
			camera->fLength = camera->fLengthTemp;
		}
	}
	else if (camera->fLength < camera->fLengthTemp)
	{
		camera->fLength += CAMERA_LENGTH_GAME_SPEED;
		// 視点距離を越えた場合は指定距離に固定
		if (camera->fLength > camera->fLengthTemp)
		{
			camera->fLength = camera->fLengthTemp;
		}
	}
}