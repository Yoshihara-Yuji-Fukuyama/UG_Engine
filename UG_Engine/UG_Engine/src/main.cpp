#include "CApplication.h"
#include <iostream>

#include <DirectXTex.h>
#include <d3dx12.h>

// WinMain�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
#if _DEBUG
	// �R���\�[���E�B���h�E�̍쐬
	AllocConsole();
	// �W���o�͂��R���\�[���Ƀ��_�C���N�g
	FILE* pFile;
	freopen_s(&pFile, "CON", "w", stdout);
#endif

	StartApp(TEXT(ENGINE_TITLE));

	return 0;
}