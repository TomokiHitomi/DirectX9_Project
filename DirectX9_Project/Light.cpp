//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "light.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#include "camera.h"
#include "calculate.h"
#include "input.h"
#include "cube.h"
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
D3DLIGHT9				g_aLight[LIGHT_MAX];		// ���C�g���

#ifdef _DEBUG
POINTLIGHT				g_tagPointLight[LIGHT_POINT_MAX];
bool					g_bLight[LIGHT_MAX];
int						g_nColor[LIGHT_MAX];
int						g_nSelect;
#endif

//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;

	/********** ���C�g0 **********/

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ���C�g0�̃^�C�v�̐ݒ�
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g0�̊g�U���̐ݒ�
	g_aLight[0].Diffuse = D3DXCOLOR(LIGHT_DIFFUSE_0, LIGHT_DIFFUSE_0, LIGHT_DIFFUSE_0, 1.0f);

	// ���C�g0�̋��ʔ��ˌ��̐ݒ�
	g_aLight[0].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���C�g0�̊����̐ݒ�
	//g_aLight[0].Ambient = D3DXCOLOR(LIGHT_AMBIENT, LIGHT_AMBIENT, LIGHT_AMBIENT, 1.0f);

	// ���C�g0�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.01f, -0.54f, -0.842f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ���C�g0�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, &g_aLight[0]);

	// ���C�g0���g�p��Ԃ�
	pDevice->LightEnable(0, TRUE);


	/********** ���C�g1 **********/

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	// ���C�g1�̃^�C�v�̐ݒ�
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g1�̊g�U���̐ݒ�
	g_aLight[1].Diffuse = D3DXCOLOR(LIGHT_DIFFUSE_1, LIGHT_DIFFUSE_1, LIGHT_DIFFUSE_1, 1.0f);

	// ���C�g1�̋��ʔ��ˌ��̐ݒ�
	//	g_aLight[1].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g1�̊����̐ݒ�
	//	g_aLight[1].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g1�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.01f, 0.54f, -0.842f);
	//vecDir = D3DXVECTOR3(-0.0f, -0.6f, -1.0f);
	//vecDir = D3DXVECTOR3(-0.0f, -1.0f, 0.8f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// ���C�g1�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(1, &g_aLight[1]);

	// ���C�g1���g�p��Ԃ�
	pDevice->LightEnable(1, TRUE);

	// �c��̃��C�g��������
	for (int i = 2; i < LIGHT_MAX; i++)
	{
		// D3DLIGHT9�\���̂�0�ŃN���A����
		ZeroMemory(&g_aLight[i], sizeof(D3DLIGHT9));
		g_aLight[i].Type = D3DLIGHT_SPOT;
		g_aLight[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//g_aLight[i].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//g_aLight[i].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[i].Direction, &vecDir);
		pDevice->SetLight(i, &g_aLight[i]);
		pDevice->LightEnable(i, FALSE);
	}

	// ���C�e�B���O���[�h��ON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

#ifdef _DEBUG
	g_nSelect = 0;

	for (int i = 0; i < LIGHT_MAX; i++)
	{
		if (i == LIGHT_MAIN)
		{
			g_bLight[i] = true;
		}
		else
		{
			g_bLight[i] = false;
		}
		g_nColor[i] = 0;
	}
	for (int i = 0; i < LIGHT_POINT_MAX; i++)
	{
		g_tagPointLight[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_tagPointLight[i].posCenter = D3DXVECTOR3(0.0f, 25.0f, 0.0f);
		g_tagPointLight[i].fHAngle = 0.0f + i * D3DX_PI;
		g_tagPointLight[i].fVAngle = 0.0f;
	}
#endif
}

//=============================================================================
// ���C�g�ݒ菈��
//=============================================================================
void SetLight(int nLight, bool bFlag)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->LightEnable(nLight, bFlag);
}

//=============================================================================
// �|�C���g���C�g�ݒ菈��
//=============================================================================
void SetLightPoint(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 pos,
	FLOAT fRange, FLOAT fAttenuation)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXVECTOR3 vecDir;

	g_aLight[nLight].Type = D3DLIGHT_POINT;									// �^�C�v
	g_aLight[nLight].Diffuse = xColor;										// �g�U��
	//g_aLight[nLight].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���ʔ��ˌ�
	//g_aLight[nLight].Ambient = xColor * LIGHT_AMBIENT_POINT;				// ����
	g_aLight[nLight].Position = pos;										// �����ʒu
	//vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);								// ��������
	//D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[nLight].Direction, &vecDir);
	g_aLight[nLight].Range = fRange;										// �����L������
	//g_aLight[nLight].Falloff = 1.0f;										// �P�x�����i�X�|�b�g�j
	//g_aLight[nLight].Attenuation0 = fAttenuation;							// �P�x����
	g_aLight[nLight].Attenuation1 = fAttenuation;
	//g_aLight[nLight].Attenuation2 = fAttenuation;
	//g_aLight[nLight].Theta = 1.0f;										// �����p�x�i�X�|�b�g�j
	//g_aLight[nLight].Phi = 1.0f;											// �O���G�b�W�i�X�|�b�g�j

	pDevice->SetLight(nLight, &g_aLight[nLight]);							// �����_�����O�p�C�v���C���ɐݒ�
}

//=============================================================================
// �X�|�b�g���C�g�ݒ菈��
//=============================================================================
void SetLightSpot(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 pos, D3DXVECTOR3 vecDir,
	FLOAT fRange, FLOAT fAttenuation, FLOAT fTheta, FLOAT fPhi)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aLight[nLight].Type = D3DLIGHT_SPOT;									// �^�C�v
	g_aLight[nLight].Diffuse = xColor;										// �g�U��
	g_aLight[nLight].Specular = xColor;										// ���ʔ��ˌ�
	//g_aLight[nLight].Ambient = xColor * LIGHT_AMBIENT_SPOT;					// ����
	g_aLight[nLight].Position = pos;										// �����ʒu
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[nLight].Direction, &vecDir);	// ��������
	g_aLight[nLight].Range = fRange;										// �����L������
	g_aLight[nLight].Falloff = 0.5f;										// �P�x�����i�X�|�b�g�j
	//g_aLight[nLight].Attenuation0 = fAttenuation;							// �P�x����
	g_aLight[nLight].Attenuation1 = fAttenuation;
	//g_aLight[nLight].Attenuation2 = fAttenuation;
	g_aLight[nLight].Theta = fTheta;										// �����p�x�i�X�|�b�g�j
	g_aLight[nLight].Phi = fPhi;											// �O���G�b�W�i�X�|�b�g�j

	pDevice->SetLight(nLight, &g_aLight[nLight]);							// �����_�����O�p�C�v���C���ɐݒ�
}

//=============================================================================
// ���C�g�ݒ菈��
//=============================================================================
void SetLightDirectional(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 vecDir)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aLight[nLight].Type = D3DLIGHT_DIRECTIONAL;							// �^�C�v
	g_aLight[nLight].Diffuse = xColor;										// �g�U��
	//g_aLight[nLight].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���ʔ��ˌ�
	//g_aLight[nLight].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ����
	//g_aLight[nLight].Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// �����ʒu
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[nLight].Direction, &vecDir);	// ��������
	//g_aLight[nLight].Range = 1.0f;										// �����L������
	//g_aLight[nLight].Falloff = 1.0f;										// �P�x�����i�X�|�b�g�j
	//g_aLight[nLight].Attenuation0 = 1.0f;									// �P�x����
	//g_aLight[nLight].Attenuation1 = 1.0f;
	//g_aLight[nLight].Attenuation2 = 1.0f;
	//g_aLight[nLight].Theta = 1.0f;										// �����p�x�i�X�|�b�g�j
	//g_aLight[nLight].Phi = 1.0f;											// �O���G�b�W�i�X�|�b�g�j

	pDevice->SetLight(nLight, &g_aLight[nLight]);							// �����_�����O�p�C�v���C���ɐݒ�
}



#ifdef _DEBUG
//=============================================================================
// ���C�g��ԏ����i�f�o�b�O�j
//=============================================================================
void UpdateDebugLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BOOL bEnable;

	PrintDebugProc("�y LIGHT �z\n");
	PrintDebugProc("�v�`�r�c�F�ړ�\n");
	PrintDebugProc("���������F�J����\n");
	PrintDebugProc(" �q�^�e �F���C�g�ύX\n");
	PrintDebugProc("�X�y�[�X�F���C�gOn/Off\n");
	PrintDebugProc(" �p�^�d �F���C�g�J���[�ύX\n");
	PrintDebugProc("\n");

	// LightTestSpot

	if (GetKeyboardTrigger(DIK_R))
	{
		g_nSelect--;
		if (g_nSelect < 0)
		{
			g_nSelect = LIGHT_MAX - 1;
		}
	}
	if (GetKeyboardTrigger(DIK_F))
	{
		g_nSelect++;
		if (g_nSelect >= LIGHT_MAX)
		{
			g_nSelect = 0;
		}
	}

	if (GetKeyboardTrigger(DIK_Q))
	{
		g_nColor[g_nSelect]--;
		if (g_nColor[g_nSelect] < 0)
		{
			g_nColor[g_nSelect] = COLOR_PALLET_BROWN - 1;
		}
	}
	if (GetKeyboardTrigger(DIK_E))
	{
		g_nColor[g_nSelect]++;
		if (g_nColor[g_nSelect] >= COLOR_PALLET_BROWN)
		{
			g_nColor[g_nSelect] = 0;
		}
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		g_bLight[g_nSelect] = g_bLight[g_nSelect] ? false : true;
	}

	// ���C�g�̓_��
	for (int i = 0; i < LIGHT_MAX; i++)
	{

		if (g_nSelect == i)
		{
			PrintDebugProc(">> ");
		}
		else
		{
			PrintDebugProc("   ");
		}
		switch (i)
		{
		case LIGHT_MAIN:
			PrintDebugProc("DIRECT_0:[");
			break;
		case LIGHT_SUB:
			PrintDebugProc("DIRECT_1:[");
			break;
		case LIGHT_SPOT_UP:
			PrintDebugProc(" SPOT_UP:[");
			break;
		case LIGHT_SPOT_CAMERA:
			PrintDebugProc("SPOT_CAM:[");
			break;
		case LIGHT_POINT_1:
			PrintDebugProc(" POINT_0:[");
			break;
		case LIGHT_POINT_2:
			PrintDebugProc(" POINT_1:[");
			break;
		}

		if (g_bLight[i])
		{
			PrintDebugProc("TRUE ]  ");
		}
		else
		{
			PrintDebugProc("FALSE]  ");
		}

		switch (g_nColor[i])
		{
		case COLOR_PALLET_WHITE:
			PrintDebugProc("COLOR:[WHITE]");
			break;
		case COLOR_PALLET_RED:
			PrintDebugProc("COLOR:[RED]");
			break;
		case COLOR_PALLET_YELLOW:
			PrintDebugProc("COLOR:[YELLOW]");
			break;
		case COLOR_PALLET_GREEN:
			PrintDebugProc("COLOR:[GREEN]");
			break;
		case COLOR_PALLET_CYAN:
			PrintDebugProc("COLOR:[CYAN]");
			break;
		case COLOR_PALLET_BLUE:
			PrintDebugProc("COLOR:[BLUE]");
			break;
		case COLOR_PALLET_MAGENTA:
			PrintDebugProc("COLOR:[MAGENTA]");
			break;
		}
		SetLight(i, g_bLight[i]);
		PrintDebugProc("\n");
	}
	PrintDebugProc("\n");

	SetLightDirectional(LIGHT_MAIN, SetColorPallet(g_nColor[LIGHT_MAIN]), D3DXVECTOR3(-0.01f, -0.54f, -0.842f));
	SetLightDirectional(LIGHT_SUB, SetColorPallet(g_nColor[LIGHT_SUB]), D3DXVECTOR3(-0.01f, 0.54f, -0.842f));

	SetLightSpot(LIGHT_SPOT_UP,
		SetColorPallet(g_nColor[LIGHT_SPOT_UP]),
		D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		D3DXVECTOR3(0.0f,-1.0f,0.0f), 300.0f, 0.01f, 0.5f, 1.0f);

	CAMERA *camera = GetCamera(0);
	D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
	D3DXVec3Normalize(&vecTemp, &vecTemp);

	SetLightSpot(LIGHT_SPOT_CAMERA,
		SetColorPallet(g_nColor[LIGHT_SPOT_CAMERA]),
		camera->posCameraEye,
		vecTemp, 300.0f, 0.02f, 0.3f, 0.3f);


	// LightTestPoint
	// ��]���a��ݒ�
	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, LIGHT_POINT_ROT_LENGTH, 0.0f);
	D3DXVECTOR3 vecAxis;

	for (int i = 0; i < LIGHT_POINT_MAX; i++)
	{
		g_tagPointLight[i].fHAngle += LIGHT_POINT_ROT_SPEED_H;
		g_tagPointLight[i].fVAngle += LIGHT_POINT_ROT_SPEED_V;
		// ���݂̊p�x�ɐ����ȉ�]���x�N�g����ݒ�
		vecAxis = D3DXVECTOR3(cosf(g_tagPointLight[i].fHAngle), 0, sinf(g_tagPointLight[i].fHAngle));

		// �N�H�[�^�j�I������
		QuaternionCalculate(&vecTa, &vecAxis, g_tagPointLight[i].fVAngle,
			&g_tagPointLight[i].pos);
		// ��]���ɐݒu
		g_tagPointLight[i].pos += g_tagPointLight[i].posCenter;
	}

	SetLightPoint(LIGHT_POINT_1, SetColorPallet(g_nColor[LIGHT_POINT_1]),
		g_tagPointLight[0].pos,
		40.0f, 0.01f);
	SetCubePos(0, g_tagPointLight[0].pos);

	SetLightPoint(LIGHT_POINT_2, SetColorPallet(g_nColor[LIGHT_POINT_2]),
		g_tagPointLight[1].pos,
		40.0f, 0.01f);
	SetCubePos(1, g_tagPointLight[1].pos);


	//for (int i = 0; i < LIGHT_MAX; i++)
	//{
	//	pDevice->GetLightEnable(i, &bEnable);
	//	PrintDebugProc("Ligth[%d] Enable[%d] Type[%d]\n", i, bEnable, g_aLight[i].Type);
	//	PrintDebugProc("Diffuse  [R:%f,G:%f,B:%f,A:%f]\n",
	//		g_aLight[i].Diffuse.r, g_aLight[i].Diffuse.g, g_aLight[i].Diffuse.b, g_aLight[i].Diffuse.a);
	//	PrintDebugProc("Specular [R:%f,G:%f,B:%f,A:%f]\n",
	//		g_aLight[i].Specular.r, g_aLight[i].Specular.g, g_aLight[i].Specular.b, g_aLight[i].Specular.a);
	//	PrintDebugProc("Ambient  [R:%f,G:%f,B:%f,A:%f]\n",
	//		g_aLight[i].Ambient.r, g_aLight[i].Ambient.g, g_aLight[i].Ambient.b, g_aLight[i].Ambient.a);
	//	if (g_aLight[i].Type != D3DLIGHT_DIRECTIONAL)
	//	{
	//		PrintDebugProc("Position [X:%f,Y:%f,Z:%f]\n",
	//			g_aLight[i].Position.x, g_aLight[i].Position.y, g_aLight[i].Position.z);
	//	}
	//}
	//PrintDebugProc("\n");
}
#endif