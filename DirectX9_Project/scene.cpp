//=============================================================================
//
// �V�[������ [scene.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "scene.h"

/* Scene */
#include "title.h"
#include "game.h"
#include "result.h"

/* System */
#include "camera.h"
#include "light.h"
#include "input.h"
#include "fade.h"
//#include "sound.h"

/* �f�o�b�O */
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
//�n�߂̓V�[������ɂ��Ă���
BaseScene *SceneManager::m_pScene = NULL;
SceneManager::SCENE SceneManager::m_nScene;

//=============================================================================
// �V�[���Ǘ�����
//=============================================================================
void SceneManager::ChangeScene(SCENE scene)
{
	if (m_pScene != NULL)
	{
		delete m_pScene;
	}

	switch (scene)
	{	//�����̃V�[��
	case SCENE::TITLE:
		m_pScene = new TitleScene();	// �^�C�g���V�[�������݂̃V�[���ɂ���
		break;
	case SCENE::GAME:
		m_pScene = new GameScene();		// �Q�[���V�[�������݂̃V�[���ɂ���
		break;
	case SCENE::RESULT:
		m_pScene = new ResultScene();	// ���U���g�V�[�������݂̃V�[���ɂ���
		break;
	default:
		break;
	}

	m_nScene = scene;
}

//=============================================================================
// ����������
//=============================================================================
void SceneManager::Init(HINSTANCE hInst, HWND hWnd)
{
	m_nScene = GAME;		// �����V�[���ԍ���ݒ�
	ChangeScene(m_nScene);	// �����V�[���ݒ�
	InitLight();			// ���C�g
	InitInput(hInst, hWnd);	// ����
	//InitSound(hWnd);		// �T�E���h
	InitCamera();			// �J����
	InitFade();				// �t�F�[�h

#ifdef _DEBUG
	InitDebugProc();		// �f�o�b�O
#endif
}

//=============================================================================
// �I������
//=============================================================================
void SceneManager::Uninit()
{
	UninitInput();			// ����
	UninitCamera();			// �J����
	//UninitSound();			// �T�E���h
	UninitFade();			// �t�F�[�h

#ifdef _DEBUG
	UninitDebugProc();		// �f�o�b�O
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void SceneManager::Update()
{
	UpdateInput();			// ����
	UpdateFade();			// �t�F�[�h

#ifdef _DEBUG
	DebugScene();			// �f�o�b�O�p
#endif
	UpdateCamera();			// �J����

	m_pScene->Update();		// ���݂̃V�[���̍X�V�֐�

	//UpdateSound();			// �T�E���h
}

//=============================================================================
// �`�揈��
//=============================================================================
void SceneManager::Draw()
{
	SetCamera();			// �J����

	m_pScene->Draw();		// ���݂̃V�[���̕`��֐�

	DrawFade();				// �t�F�[�h

#ifdef _DEBUG
	DrawDebugProc();
#endif
}

//=============================================================================
// �R���X�g���N�^����
//=============================================================================
SceneManager::SceneManager(void)
{

}

//=============================================================================
// �f�X�g���N�^����
//=============================================================================
SceneManager::~SceneManager(void)
{

}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void SceneManager::DebugScene(void)
{
	UpdateDebugProc();

	PrintDebugProc("�y SCENE �z\n");
	switch (m_nScene)
	{
	case SCENE::TITLE:
		PrintDebugProc("Scene[TITLE]\n");
		break;
	case SCENE::GAME:
		PrintDebugProc("Scene[GAME]\n");
		break;
	case SCENE::RESULT:
		PrintDebugProc("Scene[RESULT]\n");
		break;
	}

	PrintDebugProc("\n");

	UpdateDebugLight();
}
#endif