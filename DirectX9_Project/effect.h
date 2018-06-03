//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	TEXTURE_EFFECT	"data/TEXTURE/effect000.jpg"						// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_EFFECT	"DATA/TEXTURE/effect.png"						// �ǂݍ��ރe�N�X�`���t�@�C����
#define SHADER_EFFECT	"sprite.fx"

// �e�N�X�`����������
#define TEXTURE_PATTERN_DIVIDE_X_EFFECT	(1)
#define TEXTURE_PATTERN_DIVIDE_Y_EFFECT	(1)
// �e�N�X�`�������p�^�[��
#define TEXTURE_PATTERN_NU_EFFECT		(TEXTURE_PATTERN_DIVIDE_X_EFFECT*TEXTURE_PATTERN_DIVIDE_Y_EFFECT)

// ���e�X�g�l
#define	EFFECT_ALPHA		(30)

// �T�C�Y
#define	EFFECT_SIZE_X		(10.0f)
#define	EFFECT_SIZE_Y		(10.0f)

#define EFFECT_SIZE_CHANGE	(0.2f)
#define EFFECT_ALPHA_CHANGE	(0.04f)

// �ő吔
#define EFFECT_MAX			(100000)

// RGBA
#define EFFECT_DIF_R		(0.8f)
#define EFFECT_DIF_G		(0.8f)
#define EFFECT_DIF_B		(0.0f)
#define EFFECT_DIF_A		(1.0f)


typedef struct
{
	D3DXVECTOR3				posEffect;				// �n�ʂ̈ʒu
	D3DXVECTOR3				rotEffect;				// �n�ʂ̌���(��])
	D3DXVECTOR3				sclEffect;				// �n�ʂ̑傫��(�X�P�[��)

	D3DXVECTOR2				vec2Size;

	D3DXCOLOR				colorEffect;

	int						nUseCount;				// ���ŃJ�E���g
	int						nTex;

	float					fSizeChange;
	float					fAlphaChange;
	FLOAT					fSize;

	bool					bUse;					// �g�p�t���O
}EFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(int nType);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(int nTex, D3DXVECTOR2 vec2Size, D3DXCOLOR color, D3DXVECTOR3 vecPos, float fSizeChange, float fAlphaChange);

#endif
