//=============================================================================
//
// �J�������� [camera.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
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

#define	POS_X_CAM				(-1000.0f)										// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM				(500.0f)										// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM				(0.0f)										// �J�����̏����ʒu(Z���W)

#define	VIEW_ANGLE				(D3DXToRadian(35.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z				(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z				(5000.0f)										// �r���[���ʂ�FarZ�l

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

/* �Ǐ]�J�����ݒ� */
#define CAMERA_GAME_MOVE_SPEED					(0.25f)	// �ړ����̃J�����ړ��X�s�[�h
#define CAMERA_GAME_MOVE_SPEED_BOOST			(1.0f)	// �u�[�X�g���̃J�����ړ��X�s�[�h
#define CAMERA_GAME_MOVE_SPEED_AUTO				(0.25f)	// �����J�����ړ��X�s�[�h
/* �ʏ�ړ��� */
#define CAMERA_GAME_MOVE_MARGIN_FRONT			(40.0f)	// �O���ړ����E�l
#define CAMERA_GAME_MOVE_MARGIN_LEAR			(10.0f)	// ����ړ����E�l
#define CAMERA_GAME_MOVE_MARGIN_SIDE			(50.0f) // ���E�ړ����E�l
#define CAMERA_GAME_MOVE_MARGIN_SIDE_STAY		(40.0f) // �Ǐ]�J�����Œ�ʒu
/* �u�[�X�g�� */
#define CAMERA_GAME_MOVE_MARGIN_BOOST_FRONT		(85.0f)	// �O���ړ����E�l
#define CAMERA_GAME_MOVE_MARGIN_BOOST_LEAR		(25.0f)	// ��������E�l
#define CAMERA_GAME_MOVE_MARGIN_BOOST_SIDE		(60.0f) // ���E�ړ����E�l

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
	D3DXVECTOR3			posCameraEye;				// �J�����̎��_
	D3DXVECTOR3			posCameraAt;				// �J�����̒����_
	D3DXVECTOR3			vecCameraUp;				// �J�����̏�����x�N�g��
	D3DXVECTOR3			rotCamera;					// �J�����̉�]
	D3DXVECTOR3			vecTag;

	D3DXVECTOR3			posCameraAtTps;					// �|�W�V����
	D3DXVECTOR3			posCameraAtNormal;				// �|�W�V����

	D3DXVECTOR3			vecCameraAtPos;

	D3DXMATRIX			mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX			mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			mtxWorld;				// ���[���h�}�g���b�N�X

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
// �v���g�^�C�v�錾
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