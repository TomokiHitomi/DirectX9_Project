//=============================================================================
//
// �J�������� [camera.cpp]
// Author : GP12A295 25 �l���F��
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

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
D3DXVECTOR3 GetLookAtVct(void);
void SetCameraAt(void);
void SetCameraAngle(E_STAGE eStage);
void SetCameraAtMove(int nCamera);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA					cameraWk[CAMERA_MAX];
int						g_nCameraMode = CAMERA_GAME;
int						g_nCameraGameMode = CAMERA_NORMAL;
int						g_nCameraTag = 0;

//=============================================================================
// ����������
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
// �I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCamera(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	MODEL	*model = GetModel(0);
	RETICLE *reticle = GetReticle(0);
	PLAYER *player = GetPlayer(0);
	ENEMY *enemy;

	// �J�����㉺���]�p
	int nCameraReverse = 1;

	D3DXVECTOR3 vecVl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecVr = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDotTest = 0.0f;

	D3DXVECTOR3 vecRet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 vecBasePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Y���J�����T�X�y���V����
	float fHeight = 0.0f;

	// Joy-con�p�W���C��
	D3DXVECTOR3 vecGyro = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �N�H�[�^�j�I���p�ϐ�
	// ��]���a��ݒ�
	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, camera->fLength, 0.0f);
	// ���݂̊p�x�ɐ����ȉ�]���x�N�g����ݒ�
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0, sinf(camera->fHAngle + D3DX_PI * 0.5f));


	switch (GetStage())
	{
	case STAGE_TITLE:
		// TPS�����_���v�Z
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


		// �J���������_���Z�b�g
		camera->posCameraAt += (camera->posCameraAtTps - camera->posCameraAt) / 10;

		// �N�H�[�^�j�I������
		QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
			&camera->posCameraEye);
		// ��]���ɐݒu
		camera->posCameraEye += camera->posCameraAt;



		break;
	case STAGE_GAME:
		if (!GetPauseFlag())
		{
			// �΂߉�]���ɑ��x���Z
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

			// �J�������E��]�i�L�[�{�[�h�j
			if (GetKeyboardPress(DIK_LEFT) && GetKeyboardPress(DIK_RIGHT))
			{
				// ���������͏����Ȃ�
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

			// �J�������E��]�i�}�E�X�j
			if ((float)GetMobUseX() && !IsButtonPressed(0, R_BUTTON_X))
			{
				camera->fHAngleDiff -= ((float)GetMobUseX()) * CAMERA_ROT_MOUSE_X;
				//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
			}

			// �J�������E��]�iJoy-con�j
			if (IsButtonPressed(0, RSTICK_LEFT) || IsButtonPressed(0, RSTICK_RIGHT))
			{
				camera->fHAngleDiff -= CAMERA_ROT_SPEED * GetStick(0, PAD_STICK_R_X);
				//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
			}

			// �J�������[�h�ŃX�C�b�`
			switch (g_nCameraMode)
			{
			case CAMERA_GAME:	// GAME�J�������[�h

				// �J�����㉺���]
				if (GetKeyboardTrigger(DIK_F2))
				{
					camera->bCameraReverse = camera->bCameraReverse ? false : true;
				}
				if (camera->bCameraReverse)
				{
					nCameraReverse *= -1;
				}

				// �J�����㉺��]�i�L�[�{�[�h�j
				if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
				{
					// ���������͏����Ȃ�
				}
				else if (GetKeyboardPress(DIK_UP))
				{
					camera->fVAngleDiff -= camera->fRotSpeed * nCameraReverse;
				}
				else if (GetKeyboardPress(DIK_DOWN))
				{
					camera->fVAngleDiff += camera->fRotSpeed * nCameraReverse;
				}
				// �J�����㉺��]�i�}�E�X�j
				if ((float)GetMobUseY() && !IsButtonPressed(0, R_BUTTON_X))
				{
					camera->fVAngleDiff += ((float)GetMobUseY()) * CAMERA_ROT_MOUSE_Y * nCameraReverse;
				}

				//// �J�����㉺��]�iJoy-con�j
				//if (IsButtonPressed(0, RSTICK_UP) || IsButtonPressed(0, RSTICK_DOWN))
				//{
				//	camera->fVAngleDiff += CAMERA_ROT_SPEED  * GetStick(0, PAD_STICK_R_Y) * nCameraReverse;
				//}

				//// �Ə����[�h�ؑցi������TPS���[�h�j
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


				// Joy-con�c�A���O�����Z�b�g
				if (IsButtonPressed(0, R_BUTTON_Y))
				{
					camera->fVAngleDiff = CAMERA_V_ANGLE_GAME;
				}

				// Joy-con�W���C���擾
				vecGyro = GetGyro();

				// Joy-con�W���C�����p�x�ɓK�p
				camera->fVAngleDiff -= vecGyro.x * nCameraReverse;
				camera->fHAngleDiff -= vecGyro.y;
				camera->fHAngleDiff += vecGyro.z;

				//camera->fVAngleDiff -= GetRglSlider(PAD_SLIDER_V);
				//camera->fHAngleDiff -= GetRglSlider(PAD_SLIDER_H);

				camera->fVAngle += (camera->fVAngleDiff - camera->fVAngle) * CAMERA_ROT_SPEED_AUTO;
				camera->fHAngle += (camera->fHAngleDiff - camera->fHAngle) * CAMERA_ROT_SPEED_AUTO;

				// �J���������E
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

				// �Ǐ]�J��������
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

				// �J���������_���Z�b�g
				camera->posCameraAt = D3DXVECTOR3(
					(model->posModel.x
						+ (camera->vecCameraAtPos.x * cos(camera->fHAngle + D3DX_PI * 1.5f))
						+ (camera->vecCameraAtPos.z * cos(camera->fHAngle + D3DX_PI * 2.0f))),
					fHeight,
					(model->posModel.z
						+ (camera->vecCameraAtPos.x * sin(camera->fHAngle + D3DX_PI * 1.5f))
						+ (camera->vecCameraAtPos.z * sin(camera->fHAngle + D3DX_PI * 2.0f))));

				// �N�H�[�^�j�I������
				QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
					&camera->posCameraEye);
				// ��]���ɐݒu
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

			case CAMERA_VIEW:	// �r���[�J�������[�h
				/******************** �J�������_�ړ� ********************/


				// �J�����㉺��]
				if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
				{
					// ���������͏����Ȃ�
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

				// �J���������E
				if (camera->fVAngle > CAMERA_V_ANGLE_VIEW_MAX)
				{
					camera->fVAngle = CAMERA_V_ANGLE_VIEW_MAX;
				}
				else if (camera->fVAngle < CAMERA_V_ANGLE_VIEW_MIN)
				{
					camera->fVAngle = CAMERA_V_ANGLE_VIEW_MIN;
				}

				// �΂߈ړ����Ɉړ����x���Z
				if ((GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S)) && (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D)))
				{
					camera->fMoveSpeed = CAMERA_MOVE_SLANT;
				}
				else
				{
					camera->fMoveSpeed = CAMERA_MOVE_SPEED;
				}

				// Y���W�㏸
				if (GetKeyboardPress(DIK_Q) && GetKeyboardPress(DIK_E))
				{
					// ���������͏����Ȃ�
				}
				// �㏸
				if (GetKeyboardPress(DIK_Q))
				{
					camera->posCameraEye.y += 2.0f;
				}
				// ���~
				else if (GetKeyboardPress(DIK_E))
				{
					camera->posCameraEye.y -= 2.0f;
				}

				// �O�㓯�������͈ړ��Ȃ�
				if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_S))
				{
					// ���������͏����Ȃ�
				}
				// �O�ړ�
				else if (GetKeyboardPress(DIK_W))
				{
					camera->posCameraEye.x += cosf(camera->fHAngle) * camera->fMoveSpeed;
					camera->posCameraEye.z += sinf(camera->fHAngle) * camera->fMoveSpeed;
				}
				// ��ړ�
				else if (GetKeyboardPress(DIK_S))
				{
					camera->posCameraEye.x += cosf(camera->fHAngle + D3DX_PI) * camera->fMoveSpeed;
					camera->posCameraEye.z += sinf(camera->fHAngle + D3DX_PI) * camera->fMoveSpeed;
				}

				// ���E���������͈ړ��Ȃ�
				if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
				{
				}
				// ���ړ�
				else if (GetKeyboardPress(DIK_A))
				{
					camera->posCameraEye.x += cosf(camera->fHAngle + D3DX_PI * 0.5f) * camera->fMoveSpeed;
					camera->posCameraEye.z += sinf(camera->fHAngle + D3DX_PI * 0.5f) * camera->fMoveSpeed;
				}
				// �E�ړ�
				else if (GetKeyboardPress(DIK_D))
				{
					camera->posCameraEye.x += cosf(camera->fHAngle + D3DX_PI * 1.5f) * camera->fMoveSpeed;
					camera->posCameraEye.z += sinf(camera->fHAngle + D3DX_PI * 1.5f) * camera->fMoveSpeed;
				}

				/******************** �J���������_�ړ� ********************/

				// �N�H�[�^�j�I������
				QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
					&camera->posCameraAt);
				// ��]���ɐݒu
				camera->posCameraAt += camera->posCameraEye;
				break;
			}
		}

		// �f�o�b�O�p
#ifdef _DEBUG
		// �J�����؂�ւ�
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
		PrintDebugProc("�yCANERA�z\n");

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
		// ���U���g�����_���v�Z
		camera->posCameraAtTps = D3DXVECTOR3(
			(model->posModel.x + CAMERA_RESULT_LENGTH_SIDE * cos(camera->fHAngle + D3DX_PI * 1.5f)),
			model->posModel.y + CAMERA_RESULT_HEIGHT,
			(model->posModel.z + CAMERA_RESULT_LENGTH_SIDE * sin(camera->fHAngle + D3DX_PI * 1.5f)));


		camera->fHAngle += 0.003f;
		camera->fHAngle = PiCalculate360(camera->fHAngle);
#ifdef _DEBUG
		PrintDebugProc("Angle[V:%f  H:%f]\n", camera->fVAngle, camera->fHAngle);
#endif



		// �J���������_���Z�b�g
		camera->posCameraAt += (camera->posCameraAtTps - camera->posCameraAt) / 10;

		// �N�H�[�^�j�I������
		QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
			&camera->posCameraEye);
		// ��]���ɐݒu
		camera->posCameraEye += camera->posCameraAt;

		break;
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCameraAt(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	MODEL	*model = GetModel(0);
	ENEMY	*enemy;



	// �^�[�Q�b�g��0�ȊO�̏ꍇ�̓G�l�~�[�ɃZ�b�g
	if (g_nCameraTag != 0)
	{
		//enemy = GetEnemy(g_nCameraTag - 1);
		//// �J���������_���G�l�~�[�ɃZ�b�g
		//camera->posCameraAt = enemy->posEnemy;
		//// �J���������_�̍����𒲐�
		//camera->posCameraAt.y = enemy->posEnemy.y + MODEL_HEIGHT_EYE;

		g_nCameraGameMode = CAMERA_TARGET;
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	/******************** �r���[�C���O�ϊ� ********************/
	// �r���[�}�g���N�X�̏�����
	D3DXMatrixIdentity(&camera->mtxView);

	// �r���[�}�g���N�X�̍쐬
	D3DXMatrixLookAtLH(&camera->mtxView,
		&camera->posCameraEye,	// ���_
		&camera->posCameraAt,	// �����_
		&camera->vecCameraUp);	// �����

								// �r���[�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);


	/******************** �v���W�F�N�V�����ϊ� ********************/
	// �v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&camera->mtxProjection);

	// �v���W�F�N�V�����}�g���N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&camera->mtxProjection,
		VIEW_ANGLE,			// �r���[���ʂ̎���p
		VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l�i�߂��ƕ`�悵�Ȃ��j
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l�i�����ƕ`�悵�Ȃ��j

							// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
}

//=============================================================================
// �J�����Ƒ΂ɂȂ��]�s����擾
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
// �J�����Ƒ΂ɂȂ�x�N�g�����擾
//=============================================================================
D3DXVECTOR3 GetLookAtVct(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	D3DXVECTOR3	vecLookAtVct;
	vecLookAtVct = camera->posCameraEye - camera->posCameraAt;
	return vecLookAtVct;
}

//=============================================================================
// �J�����̌����̎擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	return camera->rotCamera;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	return camera->mtxView;
}

//=============================================================================
// �v���W�F�N�V�����}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxProjection(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];
	return camera->mtxProjection;
}


//=============================================================================
// �J�����擾�֐�
//=============================================================================
CAMERA *GetCamera(int no)
{
	return(&cameraWk[no]);
}

//=============================================================================
// �J�������[�h�ύX�֐�
//=============================================================================
void SetCameraMode(int no)
{
	CAMERA *camera = &cameraWk[0];
	g_nCameraMode = no;
}

//=============================================================================
// �J�������[�h�擾�֐�
//=============================================================================
int *GetCameraMode(void)
{
	return(&g_nCameraMode);
}


//=============================================================================
// �J�������[�h�ύX�֐�
//=============================================================================
void SetCameraGameMode(int no)
{
	CAMERA *camera = &cameraWk[0];

	// ���[�h�؂�ւ����s����ꍇ
	if (g_nCameraGameMode != no)
	{

	}
}

//=============================================================================
// �Q�[���J�������[�h�擾�֐�
//=============================================================================
int *GetCameraGameMode(void)
{
	return(&g_nCameraGameMode);
}

//=============================================================================
// �J�����^�[�Q�b�g�ύX�֐�
//=============================================================================
void SetCameraTag(int no)
{
	CAMERA *camera = &cameraWk[0];
	g_nCameraTag = no;
}

//=============================================================================
// �J�����^�[�Q�b�g�ύX�֐�
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
// �Ǐ]�J�����֐�
//=============================================================================
void SetCameraAtMove(int nCamera)
{
	CAMERA *camera = &cameraWk[nCamera];
	MODEL *model = GetModel(0);

	// �v���C���[�Ǐ]�J����
	if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
	{	// �O���ړ���		 
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// �V�t�g�L�[��������Ă���ꍇ
			if (camera->vecCameraAtPos.z < CAMERA_GAME_MOVE_MARGIN_BOOST_FRONT)
			{	// �u�[�X�g�ő勗���ȉ��ł���΃u�[�X�g�X�s�[�h�ňړ�
				camera->vecCameraAtPos.z += CAMERA_GAME_MOVE_SPEED_BOOST;
			}
		}
		else
		{	// �V�t�g�L�[��������Ă��Ȃ��ꍇ
			if (camera->vecCameraAtPos.z < CAMERA_GAME_MOVE_MARGIN_FRONT)
			{	// �ʏ�ő勗���ȉ��ł���Βʏ�X�s�[�h�ňړ�
				camera->vecCameraAtPos.z += CAMERA_GAME_MOVE_SPEED;
			}
			else if (camera->vecCameraAtPos.z > CAMERA_GAME_MOVE_MARGIN_FRONT)
			{	// �ʏ�ő勗���ȏ�ł���΃I�[�g�X�s�[�h�ňړ�
				camera->vecCameraAtPos.z -= CAMERA_GAME_MOVE_SPEED_AUTO;
			}
		}
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
	{	// ����ړ���
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// �V�t�g�L�[��������Ă���ꍇ
			if (camera->vecCameraAtPos.z > -CAMERA_GAME_MOVE_MARGIN_BOOST_LEAR)
			{	// �u�[�X�g�ő勗���ȉ��ł���΃u�[�X�g�X�s�[�h�ňړ�
				camera->vecCameraAtPos.z -= CAMERA_GAME_MOVE_SPEED_BOOST;
			}
		}
		else
		{	// �V�t�g�L�[��������Ă��Ȃ��ꍇ
			if (camera->vecCameraAtPos.z > -CAMERA_GAME_MOVE_MARGIN_LEAR)
			{	// �ʏ�ő勗���ȉ��ł���Βʏ�X�s�[�h�ňړ�
				camera->vecCameraAtPos.z -= CAMERA_GAME_MOVE_SPEED;
			}
			else if (camera->vecCameraAtPos.z < -CAMERA_GAME_MOVE_MARGIN_LEAR)
			{	// �ʏ�ő勗���ȏ�ł���΃I�[�g�X�s�[�h�ňړ�
				camera->vecCameraAtPos.z += CAMERA_GAME_MOVE_SPEED_AUTO;
			}
		}
	}
	else
	{	// �O��ړ����Ȃ�
		if (camera->vecCameraAtPos.z < 0.0f)
		{	// ���_������ɂ���ꍇ�͌��_�܂ŃI�[�g�X�s�[�h�ňړ�
			camera->vecCameraAtPos.z += CAMERA_GAME_MOVE_SPEED_AUTO;
		}
		if (camera->vecCameraAtPos.z > 0.0f)
		{	// ���_���O���ɂ���ꍇ�͌��_�܂ŃI�[�g�X�s�[�h�ňړ�
			camera->vecCameraAtPos.z -= CAMERA_GAME_MOVE_SPEED_AUTO;
		}
	}

	if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_LEFT))
	{	// �����ړ���
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// �V�t�g�L�[��������Ă���ꍇ
			if (camera->vecCameraAtPos.x > -(CAMERA_GAME_MOVE_MARGIN_BOOST_SIDE))
			{	// �u�[�X�g�ő勗���ȉ��ł���΃u�[�X�g�X�s�[�h�ňړ�
				camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED_BOOST;
			}
		}
		else
		{	// �V�t�g�L�[��������Ă��Ȃ��ꍇ
			if (camera->vecCameraAtPos.x > -(CAMERA_GAME_MOVE_MARGIN_SIDE))
			{	// �ʏ�ő勗���ȉ��ł���Βʏ�X�s�[�h�ňړ�
				camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED;
			}
			else if (camera->vecCameraAtPos.x < -(CAMERA_GAME_MOVE_MARGIN_SIDE))
			{	// �ʏ�ő勗���ȏ�ł���΃I�[�g�X�s�[�h�ňړ�
				camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED_AUTO;
			}
		}
	}
	else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
	{	// �E���ړ���
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// �V�t�g�L�[��������Ă���ꍇ
			if (camera->vecCameraAtPos.x < (CAMERA_GAME_MOVE_MARGIN_BOOST_SIDE))
			{	// �u�[�X�g�ő勗���ȉ��ł���΃u�[�X�g�X�s�[�h�ňړ�
				camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED_BOOST;
			}
		}
		else
		{	// �V�t�g�L�[��������Ă��Ȃ��ꍇ
			if (camera->vecCameraAtPos.x < (CAMERA_GAME_MOVE_MARGIN_SIDE))
			{	// �ʏ�ő勗���ȉ��ł���Βʏ�X�s�[�h�ňړ�
				camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED;
			}
			else if (camera->vecCameraAtPos.x >(CAMERA_GAME_MOVE_MARGIN_SIDE))
			{	// �ʏ�ő勗���ȏ�ł���΃I�[�g�X�s�[�h�ňړ�
				camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED_AUTO;
			}
		}
	}
	else
	{	// ���E�ړ����Ȃ�
		if (camera->vecCameraAtPos.x < -CAMERA_GAME_MOVE_MARGIN_SIDE_STAY)
		{	// �X�e�C�|�C���g��荶���ɂ���ꍇ�̓X�e�C�|�C���g�܂ŃI�[�g�X�s�[�h�ňړ�
			camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED_AUTO;
		}
		else if (camera->vecCameraAtPos.x > CAMERA_GAME_MOVE_MARGIN_SIDE_STAY)
		{	// �X�e�C�|�C���g���E���ɂ���ꍇ�̓X�e�C�|�C���g�܂ŃI�[�g�X�s�[�h�ňړ�
			camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED_AUTO;
		}
		else if (camera->vecCameraAtPos.x > 0.0f && camera->vecCameraAtPos.x < CAMERA_GAME_MOVE_MARGIN_SIDE_STAY)
		{	// ���_���E���A���X�e�C�|�C���g��荶���ɂ���ꍇ�̓X�e�C�|�C���g�܂ŃI�[�g�X�s�[�h�ňړ�
			camera->vecCameraAtPos.x += CAMERA_GAME_MOVE_SPEED_AUTO;
		}
		else if (camera->vecCameraAtPos.x <= 0.0f && camera->vecCameraAtPos.x > -CAMERA_GAME_MOVE_MARGIN_SIDE_STAY)
		{	// ���_��荶���A���X�e�C�|�C���g���E���ɂ���ꍇ�̓X�e�C�|�C���g�܂ŃI�[�g�X�s�[�h�ňړ�
			camera->vecCameraAtPos.x -= CAMERA_GAME_MOVE_SPEED_AUTO;
		}
	}
}

//=============================================================================
// �J�������[�h�ύX�֐�
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

	// ���_�����܂ňړ�
	if (camera->fLength > camera->fLengthTemp)
	{
		camera->fLength -= CAMERA_LENGTH_GAME_SPEED;
		// ���_�������z�����ꍇ�͎w�苗���ɌŒ�
		if (camera->fLength < camera->fLengthTemp)
		{
			camera->fLength = camera->fLengthTemp;
		}
	}
	else if (camera->fLength < camera->fLengthTemp)
	{
		camera->fLength += CAMERA_LENGTH_GAME_SPEED;
		// ���_�������z�����ꍇ�͎w�苗���ɌŒ�
		if (camera->fLength > camera->fLengthTemp)
		{
			camera->fLength = camera->fLengthTemp;
		}
	}
}