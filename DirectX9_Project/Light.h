//=============================================================================
//
// ���C�g���� [light.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	LIGHT_MAX				(5)						// ���C�g�̐�

#define LIGHT_DIFFUSE_1			(0.2f)
#define LIGHT_DIFFUSE_2			(0.1f)
#define LIGHT_AMBIENT			(0.6f)


#define LIGHT_SUB_DIFFUSE		(0.3f)
#define LIGHT_SUB_XZ			(0.8)
#define LIGHT_SUB_Y				(1.0f)

//*****************************************************************************
// �񋓌^��`
//*****************************************************************************
enum
{
	LIGHT0,
	LIGHT1,
	LIGHT2,
	LIGHT3
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitLight(void);
void SetLight(int nLight, bool bFlag);

#endif