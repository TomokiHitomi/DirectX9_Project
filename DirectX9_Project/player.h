//=============================================================================
//
// ���f������ [player.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "SkinMeshX.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	PLAYER_MODEL		"data/MODEL/Alicia/AliciaSolidMMD.x"							// �ǂݍ��ރ��f����
#define	PLAYER_MODEL		"DATA/MODEL/hackadoll/hackadollMMD.x"							// �ǂݍ��ރ��f����

//#define	PLAYER_CAR		"data/PLAYER/Tune/Tune.x"							// �ǂݍ��ރ��f����

#define PLAYER_MAX						(1)

//#define PLAYER_SCALE						(0.45f)
#define PLAYER_SCALE						(0.27f)
#define PLAYER_SIZE						(1.0f)

#define PLAYER_POS_X						(0.0f)
#define PLAYER_POS_Z						(0.0f)

#define PLAYER_FLOAT_SPEED				(0.03f)		// ���V���x
#define PLAYER_FLOAT_LENGTH				(0.1f)		// ���V���x

#define PLAYER_ALPHA						(200)
#define PLAYER_CENTER					(30)

#define PLAYER_HEIGHT_EYE				(45.0f)
#define PLAYER_HEIGHT_WEAPON			(35.0f)
#define PLAYER_H_ANGLE					(D3DX_PI * 1.5f)

#define PLAYER_MOVE_SPEED				(5)
#define PLAYER_MOVE_BOOST				(10)
#define PLAYER_MOVE_INERTIA				(0.03f)

#define PLAYER_ROT_SPEED					(0.1f)
#define PLAYER_ROT_AUTO_SPEED			(0.2f)

#define PLAYER_INVISIBLE_COUNT			(20)

#define PLAYER_TAG_COUNT					(30)		// �^�[�Q�b�g�����J�E���g

/* �X�e�[�^�X */
#define PLAYER_JUMP_POWER				(8.0f)		// �W�����v�p���[
#define PLAYER_JUMP_POWER_DOWN			(0.07f)		// �W�����v�p���[

#define PLAYER_STATUS_HP					(40.0f)	// HP
#define PLAYER_STATUS_MP					(1000.0f)	// MP
#define PLAYER_STATUS_MP_SUB				(10.0f)		// MP�̌��Z��
#define PLAYER_STATUS_MP_LIMITER			(100.0f)	// MP�̘A���g�p����
#define PLAYER_STATUS_MP_RECOVER			(5.0f)		// MP�̉񕜑��x

// �}�W�b�N�X�e�[�^�X
#define PLAYER_STATUS_NORMAL				(100.0f)	// �m�[�}��
#define PLAYER_STATUS_FIRE				(100.0f)	// �t�@�C�A
#define PLAYER_STATUS_THUNDER			(100.0f)	// �T���_�[
#define PLAYER_STATUS_ICE				(100.0f)	// �A�C�X
#define PLAYER_STATUS_WIND				(100.0f)	// �E�B���h
#define PLAYER_STATUS_WARER				(100.0f)	// �E�H�[�^�[
#define PLAYER_STATUS_EARTH				(100.0f)	// �A�[�X

// �}�W�b�N�̌��Z��
#define PLAYER_STATUS_NORMAL_SUB			(0.5f)		// �m�[�}��
#define PLAYER_STATUS_FIRE_SUB			(20.0f)		// �t�@�C�A
#define PLAYER_STATUS_THUNDER_SUB		(20.0f)		// �T���_�[
#define PLAYER_STATUS_ICE_SUB			(20.0f)		// �A�C�X
#define PLAYER_STATUS_WIND_SUB			(20.0f)		// �E�B���h
#define PLAYER_STATUS_WATER_SUB			(20.0f)		// �E�H�[�^�[
#define PLAYER_STATUS_EARTH_SUB			(20.0f)		// �A�[�X
#define PLAYER_STATUS_MAGIC_RECOVER		(0.03f)		// �}�W�b�N�̉񕜑��x

#define PLAYER_STATUS_CHARGE_MAX			(5)			// �ő�`���[�W
#define PLAYER_STATUS_CHARGE_SPEED		(12)		// �`���[�W�X�s�[�h

/* �A�^�b�N */
#define PLAYER_ATTACK_NORMAL				(10)		// �ʏ�U���C���^�[�o��
#define PLAYER_ATTACK_FIRE				(25)		// �t�@�C�A�C���^�[�o��
#define PLAYER_ATTACK_THUNDER			(25)		// �T���_�[�C���^�[�o��
#define PLAYER_ATTACK_ICE				(25)		// �A�C�X�C���^�[�o��
#define PLAYER_ATTACK_WIND				(25)		// �E�B���h�C���^�[�o��
#define PLAYER_ATTACK_WATER				(25)		// �E�H�[�^�[�C���^�[�o��
#define PLAYER_ATTACK_EARTH				(25)		// �A�[�X�C���^�[�o��


/* �X�� */
#define PLAYER_TILT_ROT					(0.005f)		// �ړ����̌X�����x
#define PLAYER_TILT_ROT_BOOST			(0.01f)		// �u�[�X�g�̌X�����x
#define PLAYER_TILT_ROT_AUTO				(0.005f)		// �����ړ����x
/* �ʏ�ړ��� */
#define PLAYER_TILT_MARGIN_FRONT			(0.1f)		// �O���ړ����E�X��
#define PLAYER_TILT_MARGIN_LEAR			(0.1f)		// ����ړ����E�X��
#define PLAYER_TILT_MARGIN_SIDE			(0.1f)		// ���ʈړ����E�X��
/* �u�[�X�g�� */
#define PLAYER_TILT_MARGIN_BOOST_FRONT	(0.2f)		// �O���ړ����E�X��
#define PLAYER_TILT_MARGIN_BOOST_LEAR	(0.2f)		// ����ړ����E�X��
#define PLAYER_TILT_MARGIN_BOOST_SIDE	(0.2f)		// ���ʈړ����E�X��


#define SKIN_ANIME_SPEED_PLAYER_NORMAL_SHOT	(60.0f / 1500.0f)
#define SKIN_ANIME_SPEED_PLAYER_DASH_SHOT	(60.0f / 900.0f)
#define SKIN_ANIME_SPEED_PLAYER_NORMAL		(60.0f / 3600.0f)
#define SKIN_ANIME_SPEED_PLAYER_DASH		(60.0f / 2400.0f)
#define SKIN_ANIME_SPEED_PLAYER_JUMP		(60.0f / 3600.0f)
#define SKIN_ANIME_SPEED_PLAYER_ATTACK		(60.0f / 2000.0f)
#define SKIN_ANIME_SPEED_PLAYER_ANIME		(60.0f / 4800.0f)

#define PLAYER_JUMP_TIME_START			(30)
#define PLAYER_JUMP_TIME_END			(25)
#define PLAYER_RECOIL_TIME				(30)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
//typedef struct
//{
//	bool			bUse;
//	int				nAttackCount;
//	float			fStatus;
//	float			fStatusMax;
//	float			fStatusSub;
//}PLAYER_MAGIC;

typedef struct
{
	int				nIdx;
	bool			bUse;
}PLAYER_MAGICCIRCLE;

typedef struct
{
	CSkinMesh		m_CSkinMesh;
	PRS				prs;

	int				nJumpCount;
	int				nJumpFlag;

	int				nRecoilCount;
	bool			bOverdraw;
	bool			bRecoilFlag;
	bool			bShot;

	// �V���h�E�p
	int				nIdxShadow;
	float			fSizeShadow;
	D3DXCOLOR		colShadow;
	bool			bShadow;

	// �~�j�}�b�v�p
	bool			bUseMinimap;

}PLAYER;

// �ǋL�͋t���i�V�������̂���i�[�����j
enum PLAYER_ANIME
{
	PLAYER_ANIME_DRUNK,
	PLAYER_ANIME_RUN,
	PLAYER_ANIME_DANCE_SPIN,
	PLAYER_ANIME_DANCE_YMCA,

	PLAYER_ANIME_DEATH,
	PLAYER_ANIME_HEADSHOT,
	PLAYER_ANIME_IDLE_EX,
	PLAYER_ANIME_RECOIL,
	PLAYER_ANIME_OVERDRAW,

	PLAYER_ANIME_JUMP_END,
	PLAYER_ANIME_JUMP_LOOP,
	PLAYER_ANIME_JUMP_START,
	PLAYER_ANIME_AIM_RIGHT,
	PLAYER_ANIME_AIM_LEFT,

	PLAYER_ANIME_AIM_BACK,
	PLAYER_ANIME_AIM_FORWARD,
	PLAYER_ANIME_AIM_IDLE,
	PLAYER_ANIME_RIGHT,
	PLAYER_ANIME_LEFT,

	PLAYER_ANIME_BACK,
	PLAYER_ANIME_FORWARD,
	PLAYER_ANIME_IDLE,
	PLAYER_ANIME_HAND,
	PLAYER_ANIME_MAX
};

enum PLAYER_JUMP
{
	PLAYER_JUMP_START,
	PLAYER_JUMP_LOOP,
	PLAYER_JUMP_END,
	PLAYER_JUMP_LAST
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int nType);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayerAnime(int nPlayer, DWORD dwAnime, FLOAT fShift);
PLAYER *GetPlayer(int no);
#endif
