//=============================================================================
//
// �L���[�u���� [cube.cpp]
// Author : GP12A295 25 �l���F�� 
//
//=============================================================================
#include "cube.h"
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

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	*g_pD3DTextureCube;		// �e�N�X�`���ւ̃|�C���^
D3DMATERIAL9		*g_pD3DMatMeshCube;

LPD3DXMESH			g_pD3DXMeshCube;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatCube;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatCube;			// �}�e���A�����̐�
D3DXMATRIX			g_mtxWorldCube;			// ���[���h�}�g���b�N�X

CUBE		cubeWk[CUBE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCube(int nType)
{
	CUBE *cube = &cubeWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < CUBE_MAX; i++, cube++)
	{
		cube->bUse = true;
		cube->pos = D3DXVECTOR3(0.0, 0.0f, 0.0f);
		cube->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		cube->scl = D3DXVECTOR3(CUBE_SCALE, CUBE_SCALE, CUBE_SCALE);
	}

	if (nType == 0)
	{
		// ���f���֌W�̏�����
		g_pD3DTextureCube = NULL;
		g_pD3DXMeshCube = NULL;
		g_pD3DXBuffMatCube = NULL;
		g_nNumMatCube = 0;

		// ���f���f�[�^�ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(CUBE_MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,			// �g�p���郁�����̃I�v�V����
			pDevice,					// �f�o�C�X
			NULL,						// ���g�p
			&g_pD3DXBuffMatCube,		// �}�e���A���f�[�^
			NULL,						// ���g�p
			&g_nNumMatCube,			// D3DXMATERIAL�\���̂̐�
			&g_pD3DXMeshCube)))		// ���b�V���f�[�^�ւ̃|�C���^
		{
			return E_FAIL;
		}

		// �}�e���A���������o��
		D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatCube->GetBufferPointer();
		g_pD3DMatMeshCube = new D3DMATERIAL9[g_nNumMatCube];		// ���b�V�������m��
		g_pD3DTextureCube = new LPDIRECT3DTEXTURE9[g_nNumMatCube];// �e�N�X�`�����m��
		for (int i = 0; i < g_nNumMatCube; i++)
		{
			g_pD3DMatMeshCube[i] = d3Mat[i].MatD3D;			// �}�e���A�����Z�b�g
			g_pD3DMatMeshCube[i].Ambient = g_pD3DMatMeshCube[i].Diffuse;// ����������
			g_pD3DTextureCube[i] = NULL;	// �e�N�X�`��������

			// �g�p���Ă���e�N�X�`��������Γǂݍ���
			if (d3Mat[i].pTextureFilename != NULL &&
				lstrlen(d3Mat[i].pTextureFilename) > 0)
			{
				// �e�N�X�`���ǂݍ���
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					d3Mat[i].pTextureFilename,
					&g_pD3DTextureCube[i])))
				{
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCube(void)
{
	for (int i = 0; i < g_nNumMatCube; i++)
	{	// �e�N�X�`���̊J��
		if (g_pD3DTextureCube[i] != NULL)
		{
			g_pD3DTextureCube[i]->Release();
			g_pD3DTextureCube[i] = NULL;
		}
	}

	if(g_pD3DXMeshCube != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshCube->Release();
		g_pD3DXMeshCube = NULL;
	}

	if(g_pD3DXBuffMatCube != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatCube->Release();
		g_pD3DXBuffMatCube = NULL;
	}

	delete[]g_pD3DMatMeshCube;
	delete[]g_pD3DTextureCube;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCube(void)
{
	CUBE *cube = &cubeWk[0];

	for (int i = 0; i < CUBE_MAX; i++, cube++)
	{
		if (cube->bUse)
		{
			// ���f����]
			cube->rot.x += CUBE_ROT;
			cube->rot.y += CUBE_ROT;
			cube->rot.z += CUBE_ROT;
			cube->rot.x = PiCalculate180(cube->rot.y);
			cube->rot.y = PiCalculate180(cube->rot.y);
			cube->rot.z = PiCalculate180(cube->rot.y);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCube(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	CUBE *cube = &cubeWk[0];

	for (int i = 0; i < CUBE_MAX; i++, cube++)
	{
		if (cube->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldCube);

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, cube->scl.x, cube->scl.y, cube->scl.z);
			D3DXMatrixMultiply(&g_mtxWorldCube, &g_mtxWorldCube, &mtxScl);

			// �yR�z��1�]�𔽉f(YawPitchRoll��y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, cube->rot.y, cube->rot.x, cube->rot.z);
			D3DXMatrixMultiply(&g_mtxWorldCube, &g_mtxWorldCube, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, cube->pos.x, cube->pos.y, cube->pos.z);
			D3DXMatrixMultiply(&g_mtxWorldCube, &g_mtxWorldCube, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldCube);

			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);
			//// �}�e���A�����ɑ΂���|�C���^�̎擾
			//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatCube->GetBufferPointer();

			for (int j = 0; j < g_nNumMatCube; j++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_pD3DMatMeshCube[j]);
				// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
				pDevice->SetTexture(0, g_pD3DTextureCube[j]);
				// �`��
				g_pD3DXMeshCube->DrawSubset(j);
			}
			// �}�e���A�������ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �o������
//=============================================================================
int SetCube(void)
{
	CUBE *cube = &cubeWk[0];

	for (int i = 0; i < CUBE_MAX; i++, cube++)
	{
		if (!cube->bUse)
		{
			cube->bUse = true;
			return i;
		}
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void SetCubePos(int nIndex, D3DXVECTOR3 pos)
{
	CUBE *cube = &cubeWk[nIndex];
	cube->pos = pos;
}

//=============================================================================
// �J���[�ݒ菈��
//=============================================================================
void SetCubeColor(int nColor)
{
	D3DXCOLOR colorTemp = (1.0f, 1.0f, 1.0f, 1.0f);
	//switch(nColor)
	//{
	//case BULLET_SPECIAL_FIRE:
	//	colorTemp = SetColorPallet(COLOR_PALLET_ORANGE);
	//	break;
	//case BULLET_SPECIAL_THUNDER:
	//	colorTemp = SetColorPallet(COLOR_PALLET_YELLOW);
	//	break;
	//case BULLET_SPECIAL_ICE:
	//	colorTemp = SetColorPallet(COLOR_PALLET_CYAN);
	//	break;
	//case BULLET_SPECIAL_WIND:
	//	colorTemp = SetColorPallet(COLOR_PALLET_LIGHTGREEN);
	//	break;
	//case BULLET_SPECIAL_WATER:
	//	colorTemp = SetColorPallet(COLOR_PALLET_BLUE);
	//	break;
	//case BULLET_SPECIAL_EARTH:
	//	colorTemp = SetColorPallet(COLOR_PALLET_BROWN);
	//	break;
	//}
	//g_pD3DMatMeshCube->Diffuse = (D3DCOLORVALUE)colorTemp;
	//g_pD3DMatMeshCube->Ambient = (D3DCOLORVALUE)(colorTemp * 0.5f);
	//g_pD3DMatMeshCube->Specular = (D3DCOLORVALUE)colorTemp;
	//colorTemp = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
	//g_pD3DMatMeshCube->Emissive = (D3DCOLORVALUE)colorTemp;
	//g_pD3DMatMeshCube->Power = 1.0f;
}

//=============================================================================
// �������
//=============================================================================
void ReleaseCube(int nIndex)
{
	CUBE *cube = &cubeWk[nIndex];
	cube->bUse = false;
}

//=============================================================================
// �擾�֐�
//=============================================================================
CUBE *GetCube(int no)
{
	return(&cubeWk[no]);
}
