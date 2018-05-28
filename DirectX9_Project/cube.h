//=============================================================================
//
// �L���[�u���� [cube.h]
// Author : GP12A295 25 �l���F�� 
//
//=============================================================================
#ifndef _CUBE_H_
#define _CUBE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CUBE_MODEL		"DATA/MODEL/cube.x"							// �ǂݍ��ރ��f����

#define CUBE_MAX		(2)
#define CUBE_SCALE		(0.03f)
#define CUBE_SIZE		(20.0f)
#define CUBE_ROT		(0.02f)

typedef struct
{
	D3DXVECTOR3		pos;			// �n�ʂ̈ʒu
	D3DXVECTOR3		rot;			// �n�ʂ̌���(��])
	D3DXVECTOR3		scl;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X

	bool			bUse;
}CUBE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCube(int nType);
void UninitCube(void);
void UpdateCube(void);
void DrawCube(void);
int SetCube(void);
void SetCubePos(int nIndex, D3DXVECTOR3 pos);
void SetCubeColor(int nColor);
void ReleaseCube(int nIndex);
CUBE *GetCube(int no);

#endif
