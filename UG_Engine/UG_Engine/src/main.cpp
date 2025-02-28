#include "CApplication.h"
#include <iostream>

#include <DirectXTex.h>
#include <d3dx12.h>

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
#if _DEBUG
	// コンソールウィンドウの作成
	AllocConsole();
	// 標準出力をコンソールにリダイレクト
	FILE* pFile;
	freopen_s(&pFile, "CON", "w", stdout);
#endif

	StartApp(TEXT(ENGINE_TITLE));

	return 0;
}