//=============================================================================
//
// �J�������� [camera.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "calculate.h"

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
	camera->fHAngle = CAMERA_H_ANGLE + D3DX_PI;
	camera->fVAngleDiff = CAMERA_V_ANGLE_GAME;
	camera->fHAngleDiff = CAMERA_H_ANGLE + D3DX_PI;
	camera->fLength = CAMERA_LENGTH_GAME_NORMAL;
	camera->fLengthTemp = CAMERA_LENGTH_GAME_NORMAL;

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
	CAMERA	*camera = &cameraWk[0];

	// �J�����㉺���]�p
	int nCameraReverse = 1;

	// �N�H�[�^�j�I���p�ϐ�
	// ��]���a��ݒ�
	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, camera->fLength, 0.0f);
	// ���݂̊p�x�ɐ����ȉ�]���x�N�g����ݒ�
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0, sinf(camera->fHAngle + D3DX_PI * 0.5f));


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

	// �}�E�X�z�C�[��
	long ModUseZ = GetMobUseZ();
	if (ModUseZ != 0)
	{
		camera->fLength -= ModUseZ * CAMERA_LENGTH_SPEED;
	}


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

	// �J���������_���Z�b�g
	//camera->posCameraAt = D3DXVECTOR3(
	//	(model->posModel.x
	//		+ (camera->vecCameraAtPos.x * cos(camera->fHAngle + D3DX_PI * 1.5f))
	//		+ (camera->vecCameraAtPos.z * cos(camera->fHAngle + D3DX_PI * 2.0f))),
	//	fHeight,
	//	(model->posModel.z
	//		+ (camera->vecCameraAtPos.x * sin(camera->fHAngle + D3DX_PI * 1.5f))
	//		+ (camera->vecCameraAtPos.z * sin(camera->fHAngle + D3DX_PI * 2.0f))));

	// �N�H�[�^�j�I������
	QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
		&camera->posCameraEye);
	// ��]���ɐݒu
	camera->posCameraEye += camera->posCameraAt;

#ifdef _DEBUG
	PrintDebugProc("�yCANERA�z\n");

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
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCameraAt(void)
{
	CAMERA	*camera = &cameraWk[g_nCameraMode];

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
