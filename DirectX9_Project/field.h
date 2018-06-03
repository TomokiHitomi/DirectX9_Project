//=============================================================================
//
// �t�B�[���h���� [field.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/snow.jpg"						// �ǂݍ��ރe�N�X�`���t�@�C����

#define	FIELD_SIZE_X		(100.0f)			// �n�ʂ̃T�C�Y(X����)
#define	FIELD_SIZE_Z		(100.0f)			// �n�ʂ̃T�C�Y(Z����)

#define FIELD_X				(15)
#define FIELD_Z				(15)
#define FIELD_MAX			(FIELD_X * FIELD_Z)	// �t�B�[���h�̍ő吔

typedef struct
{
	D3DXVECTOR3				posField;			// �n�ʂ̈ʒu
	D3DXVECTOR3				rotField;			// �n�ʂ̌���(��])
	D3DXVECTOR3				sclField;			// �n�ʂ̑傫��(�X�P�[��)

	D3DXMATRIX				mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX				mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX				mtxWorld;			// ���[���h�}�g���b�N�X
}FIELD;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitField(int nType);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
FIELD *GetField(int no);

#endif
