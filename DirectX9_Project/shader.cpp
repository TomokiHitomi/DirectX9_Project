////=============================================================================
////
//// �V�F�[�_���� [shader.cpp]
//// Author : GP12A295 25 �l���F��
////
////=============================================================================
//#include "shader.h"
//#include "camera.h"
//
///* Debug */
//#ifdef _DEBUG
//#include "debugproc.h"
//#endif
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//
////*****************************************************************************
//// �O���[�o���ϐ�
////*****************************************************************************
//
////=============================================================================
//// �X�V����
////=============================================================================
//void Shader::Update(void)
//{
//
//}
//
////=============================================================================
//// �`�揈��
////=============================================================================
//void Shader::Draw(LPDIRECT3DTEXTURE9 pD3DTexture)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[��, ��], ���s�ړ�
//
//	CAMERA *camera = GetCamera(0);
//	D3DXMATRIX mtxView = GetMtxView();
//	D3DXMATRIX mtxProjection = GetMtxProjection();
//
//	// �e�N�X�`�����Z�b�g
//	pEffect->SetTexture("tex", pD3DTexture);
//	// �V�F�[�_�[�̊J�n�AnumPass�ɂ͎w�肵�Ă���e�N�j�b�N�ɒ�`���Ă���pass�̐����ς���
//	pEffect->Begin(&numPassEffect, 0);
//
//	for (int i = 0; i < EFFECT_MAX; i++, effect++)
//	{
//		if (effect->bUse)
//		{
//			/******************** ���[���h�ϊ� ********************/
//			// ���[���h�}�g���N�X�̏�����
//			D3DXMatrixIdentity(&g_mtxWorldEffect);
//
//			// �r���{�[�h��
//			g_mtxWorldEffect._11 = mtxView._11;
//			g_mtxWorldEffect._12 = mtxView._21;
//			g_mtxWorldEffect._13 = mtxView._31;
//			g_mtxWorldEffect._21 = mtxView._12;
//			g_mtxWorldEffect._22 = mtxView._22;
//			g_mtxWorldEffect._23 = mtxView._32;
//			g_mtxWorldEffect._31 = mtxView._13;
//			g_mtxWorldEffect._32 = mtxView._23;
//			g_mtxWorldEffect._33 = mtxView._33;
//
//
//			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
//			D3DXMatrixScaling(&mtxScl, effect->sclEffect.x, effect->sclEffect.y, effect->sclEffect.z);
//			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxScl);
//
//			//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
//			//D3DXMatrixRotationYawPitchRoll(&mtxRot, effect->rotEffect.y, effect->rotEffect.x, effect->rotEffect.z);
//			//D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &GetInvRotateMat(effect->posEffect));
//			////D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxRot);
//
//			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
//			D3DXMatrixTranslation(&mtxTranslate, effect->posEffect.x, effect->posEffect.y, effect->posEffect.z);
//			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTranslate);
//
//			//// ���[���h�}�g���N�X�̐ݒ�
//			//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);
//
//			/******************** �r���[�|�[�g�ϊ� ********************/
//			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
//			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));
//
//			// ���_�t�H�[�}�b�g�̐ݒ�
//			pDevice->SetFVF(FVF_VERTEX_3D);
//
//			// �K�v�ȃO���[�o������^����
//			pEffect->SetMatrix("proj", &mtxProjection);
//			pEffect->SetMatrix("view", &mtxView);
//			pEffect->SetMatrix("world", &g_mtxWorldEffect);
//
//			//// �e�N�X�`���̐ݒ�
//			//pDevice->SetTexture(0, g_pD3DTextureEffect);
//
//
//			// �p�X���w�肵�ĊJ�n
//			pEffect->BeginPass(0);
//
//
//			// �|���S���̕`��
//			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
//
//			// �V�F�[�_�I��
//			pEffect->EndPass();
//
//		}
//	}
//	pEffect->End();
//
//}
//
////=============================================================================
//// �R���X�g���N�^����
////=============================================================================
//Shader::Shader(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	const char* path = SHADER_SPRITE;
//	// �t�@�C��( const char* path )����V�F�[�_�[�t�@�C����ǂݍ��ݓǂݍ���
//	hrEffect = D3DXCreateEffectFromFile(pDevice, path, 0, 0, 0, 0, &pEffect, &errorEffect);
//	if (FAILED(hrEffect))
//	{
//		// �G���[
//		//return S_FALSE;
//		return;
//	}
//	// �g�p����e�N�j�b�N���`
//	hrEffect = pEffect->SetTechnique("Tec01");
//	if (FAILED(hrEffect)) {
//
//		// �G���[
//		//return S_FALSE;
//		return;
//	}
//}
//
////=============================================================================
//// �f�X�g���N�^����
////=============================================================================
//Shader::~Shader(void)
//{
//	if (pEffect != NULL)
//	{	// �V�F�[�_�[�����
//		pEffect->Release();
//	}
//}