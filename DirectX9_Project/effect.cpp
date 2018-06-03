//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "camera.h"
#include "effect.h"
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
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void InitStatusEffect(LPDIRECT3DDEVICE9 pDevice, int nEffect);
void SetVtxEffect(int nWk, float fSizeX, float fSizeY);
void SetDiffuseEffect(int nWk, D3DXCOLOR col);
void SetTexEffect(int nWk, int nTex);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

EFFECT					effectWk[EFFECT_MAX];

D3DXMATRIX				g_mtxWorldEffect;

// �V�F�[�_�[�֘A
ID3DXBuffer *errorEffect = 0;
ID3DXEffect *effectshader;
UINT numPassEffect;
HRESULT hrEffect;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(int nType)
{
	EFFECT *effect = &effectWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* path = SHADER_EFFECT;
	// �t�@�C��( const char* path )����V�F�[�_�[�t�@�C����ǂݍ��ݓǂݍ���
	hrEffect = D3DXCreateEffectFromFile(pDevice, path, 0, 0, 0, 0, &effectshader, &errorEffect);
	if (FAILED(hrEffect))
	{
		// �G���[
		return S_FALSE;
	}
	// �g�p����e�N�j�b�N���`
	hrEffect = effectshader->SetTechnique("Tec01");
	if (FAILED(hrEffect)) {

		// �G���[
		return S_FALSE;
	}


	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_EFFECT,
			&g_pD3DTextureEffect);
	}

	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		// �ʒu�E��]�E�X�P�[���̐ݒ�
		effect->posEffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect->rotEffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect->sclEffect = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		InitStatusEffect(pDevice, i);

	}

	// ���_���̍쐬�i���[�J�����W�̐ݒ�j
	MakeVertexEffect(pDevice);
	return S_OK;
}

//=============================================================================
// �ė��p����
//=============================================================================
void InitStatusEffect(LPDIRECT3DDEVICE9 pDevice, int nEffect)
{
	EFFECT *effect = &effectWk[nEffect];

	effect->bUse = false;
	effect->vec2Size = D3DXVECTOR2(EFFECT_SIZE_X, EFFECT_SIZE_Y);
	effect->colorEffect = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	effect->nTex = 0;
	effect->fSizeChange = 0.0f;
	effect->fAlphaChange = 0.0f;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	// �V�F�[�_�[�����
	effectshader->Release();

	if (g_pD3DTextureEffect != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect = NULL;
	}

	if (g_pD3DVtxBuffEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffEffect->Release();
		g_pD3DVtxBuffEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *effect = &effectWk[0];
	CAMERA *camera = GetCamera(0);

	// �G�t�F�N�g�ݒu
	if (GetKeyboardPress(DIK_SPACE))
	{
		SetEffect(0,
			D3DXVECTOR2(25, 25),
			SetColorPallet(COLOR_PALLET_MAGENTA),
			camera->posCameraAt,
			0.0001f,
			0.0001f);
	}

#ifdef _DEBUG
	PrintDebugProc("�y EFFECT �z\n");
	int nEffectCount = 0;
#endif

	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (effect->bUse)
		{
#ifdef _DEBUG
			// �g�p�G�t�F�N�g�����J�E���g
			nEffectCount++;
#endif
			effect->vec2Size.x -= effect->fSizeChange;
			effect->vec2Size.y -= effect->fSizeChange;
			effect->colorEffect.a -= effect->fAlphaChange;

			SetVtxEffect(i, effect->vec2Size.x, effect->vec2Size.y);
			SetDiffuseEffect(i, effect->colorEffect);
			SetTexEffect(i, effect->nTex);

			if (effect->vec2Size.x < 0 || effect->colorEffect.a < 0)
			{
				InitStatusEffect(pDevice, i);
			}
		}
	}

#ifdef _DEBUG
	PrintDebugProc("EffectMax:%d\n", nEffectCount);
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[��, ��], ���s�ړ�

	EFFECT *effect = &effectWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	D3DXMATRIX mtxView = GetMtxView();
	D3DXMATRIX mtxProjection = GetMtxProjection();

	//// ���e�X�g��L����
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, EFFECT_ALPHA);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	////pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z��r�Ȃ�

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �e�N�X�`�����Z�b�g
	effectshader->SetTexture("tex", g_pD3DTextureEffect);
	// �V�F�[�_�[�̊J�n�AnumPass�ɂ͎w�肵�Ă���e�N�j�b�N�ɒ�`���Ă���pass�̐����ς���
	effectshader->Begin(&numPassEffect, 0);

	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (effect->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldEffect);

			// �r���{�[�h��
			g_mtxWorldEffect._11 = mtxView._11;
			g_mtxWorldEffect._12 = mtxView._21;
			g_mtxWorldEffect._13 = mtxView._31;
			g_mtxWorldEffect._21 = mtxView._12;
			g_mtxWorldEffect._22 = mtxView._22;
			g_mtxWorldEffect._23 = mtxView._32;
			g_mtxWorldEffect._31 = mtxView._13;
			g_mtxWorldEffect._32 = mtxView._23;
			g_mtxWorldEffect._33 = mtxView._33;


			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, effect->sclEffect.x, effect->sclEffect.y, effect->sclEffect.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxScl);

			//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, effect->rotEffect.y, effect->rotEffect.x, effect->rotEffect.z);
			//D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &GetInvRotateMat(effect->posEffect));
			////D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, effect->posEffect.x, effect->posEffect.y, effect->posEffect.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTranslate);

			//// ���[���h�}�g���N�X�̐ݒ�
			//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);

			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �K�v�ȃO���[�o������^����
			effectshader->SetMatrix("proj", &mtxProjection);
			effectshader->SetMatrix("view", &mtxView);
			effectshader->SetMatrix("world", &g_mtxWorldEffect);

			//// �e�N�X�`���̐ݒ�
			//pDevice->SetTexture(0, g_pD3DTextureEffect);


			// �p�X���w�肵�ĊJ�n
			effectshader->BeginPass(0);


			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);

			// �V�F�[�_�I��
			effectshader->EndPass();

		}
	}
	effectshader->End();

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//// ���e�X�g�𖳌���
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * EFFECT_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffEffect,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < EFFECT_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-EFFECT_SIZE_X, -EFFECT_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EFFECT_SIZE_X, -EFFECT_SIZE_Y, 0.0f);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_EFFECT;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EFFECT;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVtxEffect(int nWk, float fSizeX, float fSizeY)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nWk * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseEffect(int nWk, D3DXCOLOR col)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nWk * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�֐�
//=============================================================================
void SetTexEffect(int nWk, int nTex)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nWk * 4);

		// �e�N�X�`�����W�̐ݒ�
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_EFFECT;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EFFECT;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
//void SetTextureEffect(int no, int cntPattern)
//{
//	EFFECT *effect = &effectWk[no];
//
//	// �e�N�X�`�����W�̐ݒ�
//	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_EFFECT;
//	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
//	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
//	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EFFECT;
//	effect->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
//	effect->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
//	effect->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
//	effect->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
//}

//=============================================================================
// �Ώۂɐݒu
//=============================================================================
void SetEffect(int nTex, D3DXVECTOR2 vec2Size, D3DXCOLOR color, D3DXVECTOR3 vecPos, float fSizeChange, float fAlphaChange)
{
	EFFECT *effect = &effectWk[0];

	// ���g�p��T��
	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (!effect->bUse)
		{
			effect->bUse = true;
			effect->posEffect = vecPos;
			effect->colorEffect = color;
			effect->colorEffect.a = 1.0f;
			effect->vec2Size = vec2Size;
			effect->fSizeChange = fSizeChange;
			effect->fAlphaChange = fAlphaChange;
			effect->nTex = nTex;

			SetVtxEffect(i, effect->vec2Size.x, effect->vec2Size.y);
			SetDiffuseEffect(i, effect->colorEffect);
			SetTexEffect(i, effect->nTex);
			return;
		}
	}
}