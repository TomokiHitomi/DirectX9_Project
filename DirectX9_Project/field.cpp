//=============================================================================
//
// �t�B�[���h���� [field.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "camera.h"
#include "field.h"
#include "input.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureField = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

FIELD					fieldWk[FIELD_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(int nType)
{
	FIELD *field = &fieldWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_FIELD,
			&g_pD3DTextureField);
	}

	for (int i = 0; i < FIELD_Z; i++)
	{
		for (int j = 0; j < FIELD_X; j++, field++)
		{
			// �ʒu�E��]�E�X�P�[���̐ݒ�
			field->posField = D3DXVECTOR3(0.0f + j * FIELD_SIZE_X * 2, 0.0f, 0.0f + i * FIELD_SIZE_Z * 2);
			field->rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			field->sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
	}

	// ���_���̍쐬�i���[�J�����W�̐ݒ�j
	MakeVertexField(pDevice);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	if(g_pD3DTextureField != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}

	if(g_pD3DVtxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
{
	FIELD *field = &fieldWk[0];
	//for (int i = 0; i < FIELD_MAX; i++, field++)
	//{
	//	// �����ړ�
	//	if (GetKeyboardPress(DIK_SPACE))
	//	{
	//		field->posField.y += 5;
	//	}

	//	if (GetKeyboardPress(DIK_LSHIFT))
	//	{
	//		field->posField.y -= 5;
	//	}


	//	// Y����]
	//	if (GetKeyboardPress(DIK_Q))
	//	{
	//		field->rotField.y += 0.05;
	//	}
	//	// Y�E�ړ�
	//	else if (GetKeyboardPress(DIK_E))
	//	{
	//		field->rotField.y -= 0.05;
	//	}


	//	// �g��
	//	if (GetKeyboardPress(DIK_Z))
	//	{
	//		field->sclField += D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	//	}
	//	// �k��
	//	else if (GetKeyboardPress(DIK_X))
	//	{
	//		field->sclField -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	//	}
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[��, ��], ���s�ړ�

	FIELD *field = &fieldWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);


	for (int i = 0; i < FIELD_MAX; i++, field++)
	{
		/******************** ���[���h�ϊ� ********************/
		// ���[���h�}�g���N�X�̏�����
		D3DXMatrixIdentity(&field->mtxWorld);

		// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
		D3DXMatrixScaling(&mtxScl, field->sclField.x, field->sclField.y, field->sclField.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxScl);

		// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, field->rotField.y, field->rotField.x, field->rotField.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxRot);

		// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
		D3DXMatrixTranslation(&mtxTranslate, field->posField.x, field->posField.y, field->posField.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &field->mtxWorld);


		/******************** �r���[�|�[�g�ϊ� ********************/
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureField);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	FIELD *field = &fieldWk[0];

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffField,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-FIELD_SIZE_X, 0.0f, FIELD_SIZE_Z);
		pVtx[1].vtx = D3DXVECTOR3(FIELD_SIZE_X, 0.0f, FIELD_SIZE_Z);
		pVtx[2].vtx = D3DXVECTOR3(-FIELD_SIZE_X, 0.0f, -FIELD_SIZE_Z);
		pVtx[3].vtx = D3DXVECTOR3(FIELD_SIZE_X, 0.0f, -FIELD_SIZE_Z);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffField->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �擾�֐�
//=============================================================================
FIELD *GetField(int no)
{
	return(&fieldWk[no]);
}