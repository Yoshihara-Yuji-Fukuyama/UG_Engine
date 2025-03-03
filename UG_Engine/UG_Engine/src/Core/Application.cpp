#include "Application.h"
#include <iostream>
#include "CEngine.h"
#include "CEngine3D.h"

// �O���[�o���ϐ�

HINSTANCE gHInstance;	// �A�v���P�[�V�����̃C���X�^���X�n���h��
HWND gHWnd = NULL;		// �E�B���h�E�n���h��

// �E�B���h�E�v���V�[�W���i�E�B���h�E�̃��b�Z�[�W����������֐��j
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		// �E�B���h�E������ꂽ�ꍇ
	case WM_DESTROY:

		// ���b�Z�[�W���[�v�I��
		PostQuitMessage(0);

		break;
	default:
		break;
	}

	// �f�t�H���g���������s
	return DefWindowProc(hwnd, msg, wp, lp);
}

// �E�B���h�E�̐���
void InitWindow(const TCHAR* appName)
{
	// HINSTANCE���擾
	gHInstance = GetModuleHandle(nullptr);
	if (gHInstance == nullptr)
	{
		std::cout << "HINSTANCE�̎擾���s" << std::endl;
		return;
	}

	// �E�B���h�E�̐ݒ�
	WNDCLASSEX wc = {};					// �\���̂�������
	wc.cbSize = sizeof(WNDCLASSEX);		// �T�C�Y��ݒ�
	wc.style = CS_HREDRAW | CS_VREDRAW;	// �T�C�Y���ύX���ꂽ��ĕ`�悷��
	wc.lpfnWndProc = WndProc;			// ���b�Z�[�W�����֐���ݒ�
	wc.hIcon = LoadIcon(gHInstance, IDI_APPLICATION);		// �A�v���̃A�C�R����ݒ�
	wc.hCursor = LoadCursor(gHInstance, IDC_ARROW);			// �}�E�X�J�[�\����ݒ�
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);	// �w�i�F��ݒ�
	wc.lpszMenuName = nullptr;	// ���j���[��ݒ�
	wc.lpszClassName = appName;	// �E�B���h�E�N���X�̖��O
	wc.hIconSm = LoadIcon(gHInstance, IDI_APPLICATION);		// �^�X�N�o�[�ł̃A�C�R����ݒ�

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wc);

	// �E�B���h�E�T�C�Y�̐ݒ�
	RECT rect = {};
	rect.right = static_cast<LONG>(WINDOW_WIDTH);
	rect.bottom = static_cast<LONG>(WINDOW_HEIGHT);

	// �E�B���h�E�T�C�Y�𒲐�
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rect, style, FALSE);

	// �E�B���h�E�̐���
	gHWnd = CreateWindowEx
	(
		0,						// �g���X�^�C��
		appName,				// �N���X��
		appName,				// �E�B���h�E�^�C�g��
		style,					// �X�^�C��
		CW_USEDEFAULT,			// �����ʒu
		CW_USEDEFAULT,			// �����ʒu
		rect.right - rect.left,	// �E�B���h�E�T�C�Y
		rect.bottom - rect.top,	// �E�B���h�E�T�C�Y
		nullptr,				// �e�E�B���h�E
		nullptr,				// ���j���[
		gHInstance,				// �C���X�^���X�n���h��
		nullptr					// �ǉ��p�����[�^
	);

	if (gHWnd == nullptr)
	{
		std::cout << "�E�B���h�E�̐������s" << std::endl;
		return;
	}
	else
	{
		std::cout << "�E�B���h�E�̐�������" << std::endl;
	}

	// �E�B���h�E��\��
	ShowWindow(gHWnd, SW_SHOWNORMAL);

	// �E�B���h�E�Ƀt�H�[�J�X����
	SetFocus(gHWnd);
}

// ���C���������[�v
void MainLoop()
{
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		/*
		���b�Z�[�W�����o���ď�������
		���b�Z�[�W�����݂���ꍇ��true
		*/
		if (PeekMessage(
			&msg,				// �󂯎�������b�Z�[�W���i�[
			nullptr,			// �E�B���h�E�n���h�����w��inullptr�Ȃ̂ŃA�v���S�́j
			0, 0,				// ���b�Z�[�W�͈̔́i0�Ȃ̂őS�Ẵ��b�Z�[�W�j
			PM_REMOVE == TRUE))	// ���b�Z�[�W���L���[����폜���Ď��o��
		{
			// �L�[�{�[�h���͂�ϊ�
			TranslateMessage(&msg);
			// ���b�Z�[�W���E�B���h�E�v���V�[�W���ɓn��
			DispatchMessage(&msg);
		}
		else
		{
			// �X�V
			gEngine3D->Update();
			// �`��J�n
			gEngine->BeginRender();
			// �`��
			gEngine3D->Render();
			// �`��I��
			gEngine->EndRender();
		}
	}
}

// �A�v���̊J�n
void StartApp(const TCHAR* appName)
{
	// �E�B���h�E�𐶐�
	InitWindow(appName);

	// �`��G���W���̏�����
	gEngine = new CEngine();
	if (!gEngine->Init(gHWnd, WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		return;
	}

	// �`��G���W��3D�̏�����
	gEngine3D = new CEngine3D();
	if (!gEngine3D->Init())
	{
		return;
	}

	// ���C���������[�v
	MainLoop();
}
