//=============================================================================
//
// ���f������ [player.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "player.h"
#include "Light.h"
#include "input.h"
#include "camera.h"

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
void CheckActionPlayer(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PLAYER				playerWk[PLAYER_MAX];
D3DXMATRIX			g_mtxWorldPlayer;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int nType)
{
	PLAYER *player = &playerWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E��]�E�X�P�[���̐ݒ�
	player->prs.pos = D3DXVECTOR3(PLAYER_POS_X, 0.0f, PLAYER_POS_Z);
	player->prs.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	player->prs.scl = D3DXVECTOR3(PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE);

	player->nJumpCount = 0;
	player->nJumpFlag = 0;
	player->nRecoilCount = 0;
	player->bOverdraw = false;
	player->bRecoilFlag = false;
	player->bShot = false;
	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	//if (nType == 0)
	//{
		player->m_CSkinMesh.Init(pDevice, PLAYER_MODEL);
	//}

	//player->m_CSkinMesh.ChangeAnim((DWORD)ANIME01);

	player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_ANIME);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *player = GetPlayer(0);
	//���b�V���I�u�W�F�N�g�Ǘ��N���X�������Ǘ�
	player->m_CSkinMesh.Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *player = GetPlayer(0);
#ifdef _DEBUG
	PrintDebugProc("�y PLAYER �z\n");
#endif

	CheckActionPlayer();


	player->m_CSkinMesh.Update(g_mtxWorldPlayer);

#ifdef _DEBUG
	PrintDebugProc("Pos[X:%f Y:%f Z:%f]\n", player->prs.pos.x, player->prs.pos.y, player->prs.pos.z);
	PrintDebugProc("Anime[%d]  AnimeTime[%d]\n", PLAYER_ANIME_MAX - player->m_CSkinMesh.GetAnimTrack() - 1, player->m_CSkinMesh.GetAnimTime());
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	//SetLight(LIGHT2, TRUE);
	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	player->m_CSkinMesh.Draw(pDevice, player->prs);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//SetLight(LIGHT2, FALSE);
}

//=============================================================================
// �A�j���[�V�����ݒ�֐�
//=============================================================================
void SetPlayerAnime(int nPlayer,DWORD dwAnime, FLOAT fShift)
{
	PLAYER *player = GetPlayer(nPlayer);
	//MODEL *model = GetModel(0);
	//DWORD dwTemp = dAnime;

	//if (model->bDash)
	//{
	//	player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_DASH);
	//}
	//else
	//{
	//	player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_NORMAL);
	//}

	//dAnime = ANIME_MAX - dAnime - 1;
	if (dwAnime != player->m_CSkinMesh.GetAnimTrack())
	{
		player->m_CSkinMesh.ChangeAnim(dwAnime,fShift);

	//	if (dwTemp == ANIME08)
	//	{
	//		player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_ATTACK);
	//		if (player->m_CSkinMesh.GetAnimTime() < 30)
	//		{
	//			player->m_CSkinMesh.SetAnimTime(30);
	//		}
	//	}
	//	else
	//	{
	//		if (model->bDash)
	//		{
	//			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_DASH);
	//		}
	//		else
	//		{
	//			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_NORMAL);
	//		}
	//	}
	};
}

//=============================================================================
// �A�j���[�V�����ݒ�֐�
//=============================================================================
void CheckActionPlayer(void)
{
	PLAYER *player = GetPlayer(0);


	//if (player->bRecoilFlag)
	//{
	//	player->nRecoilCount++;
	//	if (player->nRecoilCount > PLAYER_RECOIL_TIME)
	//	{
	//		player->bRecoilFlag = false;
	//		player->nRecoilCount = 0;
	//	}
	//}
	//else if ((GetKeyboardPress(DIK_V) || IsMobUseRightPressed() || IsButtonPressed(0, R_BUTTON_R)) && CheckMagicModel(0, GetTypeMagic()))
	//{	// �}�W�b�N�T�[�N���`���[�W
	//	player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_ATTACK);
	//	SetPlayerAnime(0, PLAYER_ANIME_OVERDRAW);
	//	player->bOverdraw = true;
	//}
	//else if ((GetKeyboardRelease(DIK_V) || IsMobUseRightReleased() || IsButtonReleased(0, R_BUTTON_R)) && player->bOverdraw)
	//{
	//	SetPlayerAnime(0, PLAYER_ANIME_RECOIL);
	//	player->bRecoilFlag = true;
	//	player->bOverdraw = false;
	//}
	//else if (model->bJump && player->nJumpFlag == PLAYER_JUMP_START)
	//{
	//	player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_JUMP);
	//	SetPlayerAnime(0, PLAYER_ANIME_JUMP_START);
	//	player->nJumpFlag = PLAYER_JUMP_LOOP;
	//}
	//else if (player->nJumpFlag == PLAYER_JUMP_LOOP)
	//{
	//	player->nJumpCount++;
	//	if (player->nJumpCount > PLAYER_JUMP_TIME_START)
	//	{
	//		SetPlayerAnime(0, PLAYER_ANIME_JUMP_LOOP);
	//		player->nJumpFlag = PLAYER_JUMP_END;
	//		player->nJumpCount = 0;
	//	}
	//}
	//else if (player->nJumpFlag == PLAYER_JUMP_END)
	//{
	//	if (!model->bJump)
	//	{
	//		SetPlayerAnime(0, PLAYER_ANIME_JUMP_END);
	//		player->nJumpFlag = PLAYER_JUMP_LAST;
	//	}
	//}
	//else if (player->nJumpFlag == PLAYER_JUMP_LAST)
	//{
	//	// ���n���͉_�𑫌���
	//	SetCloud(model->posModel);
	//	player->nJumpCount++;
	//	if (player->nJumpCount > PLAYER_JUMP_TIME_END)
	//	{
	//		player->nJumpFlag = PLAYER_JUMP_START;
	//		player->nJumpCount = 0;
	//	}
	//}
	//else
	//{	
	//	player->bShot = false;
	//	if (IsMobUseLeftPressed() || GetKeyboardPress(DIK_B) || IsButtonPressed(0, R_BUTTON_ZR))
	//	{	// �m�[�}���o���b�g����
	//		player->bShot = true;
	//	}
	//	// �ړ�����
	//	if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_LEFT))
	//	{// ���ړ�
	//		if	(player->bShot)	{SetPlayerAnime(0, PLAYER_ANIME_AIM_LEFT);}
	//		else		{SetPlayerAnime(0, PLAYER_ANIME_RUN);}
	//	}
	//	else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
	//	{// �E�ړ�
	//		if (player->bShot)	{ SetPlayerAnime(0, PLAYER_ANIME_AIM_RIGHT); }
	//		else		{ SetPlayerAnime(0, PLAYER_ANIME_RUN); }
	//	}
	//	else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
	//	{// �O�ړ�
	//		if (player->bShot)	{ SetPlayerAnime(0, PLAYER_ANIME_AIM_FORWARD); }
	//		else		{ SetPlayerAnime(0, PLAYER_ANIME_RUN); }
	//	}
	//	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
	//	{// ��ړ�
	//		if (player->bShot)	{ SetPlayerAnime(0, PLAYER_ANIME_AIM_BACK); }
	//		else		{ SetPlayerAnime(0, PLAYER_ANIME_RUN); }
	//	}
	//	else
	//	{// �ړ��Ȃ�
	//		if (player->bShot)	{ SetPlayerAnime(0, PLAYER_ANIME_AIM_IDLE); }
	//		else		{ SetPlayerAnime(0, PLAYER_ANIME_IDLE); }
	//	}


	//	if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_A)
	//		|| GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D)
	//		|| IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, BUTTON_DOWN)
	//		|| IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, BUTTON_RIGHT)
	//		|| IsButtonPressed(0, LSTICK_UP) || IsButtonPressed(0, LSTICK_DOWN)
	//		|| IsButtonPressed(0, LSTICK_LEFT) || IsButtonPressed(0, LSTICK_RIGHT))
	//	{
	//		// �ړ����͉_�𑫌���
	//		SetCloud(model->posModel);
	//	}

	//	if (player->bShot)
	//	{
	//		if (model->bDash)
	//		{
	//			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_DASH_SHOT);
	//		}
	//		else
	//		{
	//			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_NORMAL_SHOT);
	//		}
	//		SetCameraModeLength(CAMERA_TPS);
	//	}
	//	else
	//	{
	//		if (model->bDash)
	//		{
	//			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_DASH);
	//		}
	//		else
	//		{
	//			player->m_CSkinMesh.SetAnimSpeed(SKIN_ANIME_SPEED_PLAYER_NORMAL);
	//		}
	//		SetCameraModeLength(CAMERA_NORMAL);
	//	}
	//}


}

//=============================================================================
// �擾�֐�
//=============================================================================
PLAYER *GetPlayer(int no)
{
	return(&playerWk[no]);
}