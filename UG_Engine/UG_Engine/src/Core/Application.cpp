#include "Application.h"
#include <iostream>
#include "CEngine.h"
#include "CEngine3D.h"

// グローバル変数

HINSTANCE gHInstance;	// アプリケーションのインスタンスハンドル
HWND gHWnd = NULL;		// ウィンドウハンドル

// ウィンドウプロシージャ（ウィンドウのメッセージを処理する関数）
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		// ウィンドウが閉じられた場合
	case WM_DESTROY:

		// メッセージループ終了
		PostQuitMessage(0);

		break;
	default:
		break;
	}

	// デフォルト処理を実行
	return DefWindowProc(hwnd, msg, wp, lp);
}

// ウィンドウの生成
void InitWindow(const TCHAR* appName)
{
	// HINSTANCEを取得
	gHInstance = GetModuleHandle(nullptr);
	if (gHInstance == nullptr)
	{
		std::cout << "HINSTANCEの取得失敗" << std::endl;
		return;
	}

	// ウィンドウの設定
	WNDCLASSEX wc = {};					// 構造体を初期化
	wc.cbSize = sizeof(WNDCLASSEX);		// サイズを設定
	wc.style = CS_HREDRAW | CS_VREDRAW;	// サイズが変更されたら再描画する
	wc.lpfnWndProc = WndProc;			// メッセージ処理関数を設定
	wc.hIcon = LoadIcon(gHInstance, IDI_APPLICATION);		// アプリのアイコンを設定
	wc.hCursor = LoadCursor(gHInstance, IDC_ARROW);			// マウスカーソルを設定
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);	// 背景色を設定
	wc.lpszMenuName = nullptr;	// メニューを設定
	wc.lpszClassName = appName;	// ウィンドウクラスの名前
	wc.hIconSm = LoadIcon(gHInstance, IDI_APPLICATION);		// タスクバーでのアイコンを設定

	// ウィンドウクラスの登録
	RegisterClassEx(&wc);

	// ウィンドウサイズの設定
	RECT rect = {};
	rect.right = static_cast<LONG>(WINDOW_WIDTH);
	rect.bottom = static_cast<LONG>(WINDOW_HEIGHT);

	// ウィンドウサイズを調整
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rect, style, FALSE);

	// ウィンドウの生成
	gHWnd = CreateWindowEx
	(
		0,						// 拡張スタイル
		appName,				// クラス名
		appName,				// ウィンドウタイトル
		style,					// スタイル
		CW_USEDEFAULT,			// 初期位置
		CW_USEDEFAULT,			// 初期位置
		rect.right - rect.left,	// ウィンドウサイズ
		rect.bottom - rect.top,	// ウィンドウサイズ
		nullptr,				// 親ウィンドウ
		nullptr,				// メニュー
		gHInstance,				// インスタンスハンドル
		nullptr					// 追加パラメータ
	);

	if (gHWnd == nullptr)
	{
		std::cout << "ウィンドウの生成失敗" << std::endl;
		return;
	}
	else
	{
		std::cout << "ウィンドウの生成成功" << std::endl;
	}

	// ウィンドウを表示
	ShowWindow(gHWnd, SW_SHOWNORMAL);

	// ウィンドウにフォーカスする
	SetFocus(gHWnd);
}

// メイン処理ループ
void MainLoop()
{
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		/*
		メッセージを取り出して処理する
		メッセージが存在する場合はtrue
		*/
		if (PeekMessage(
			&msg,				// 受け取ったメッセージを格納
			nullptr,			// ウィンドウハンドルを指定（nullptrなのでアプリ全体）
			0, 0,				// メッセージの範囲（0なので全てのメッセージ）
			PM_REMOVE == TRUE))	// メッセージをキューから削除して取り出す
		{
			// キーボード入力を変換
			TranslateMessage(&msg);
			// メッセージをウィンドウプロシージャに渡す
			DispatchMessage(&msg);
		}
		else
		{
			// 更新
			gEngine3D->Update();
			// 描画開始
			gEngine->BeginRender();
			// 描画
			gEngine3D->Render();
			// 描画終了
			gEngine->EndRender();
		}
	}
}

// アプリの開始
void StartApp(const TCHAR* appName)
{
	// ウィンドウを生成
	InitWindow(appName);

	// 描画エンジンの初期化
	gEngine = new CEngine();
	if (!gEngine->Init(gHWnd, WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		return;
	}

	// 描画エンジン3Dの初期化
	gEngine3D = new CEngine3D();
	if (!gEngine3D->Init())
	{
		return;
	}

	// メイン処理ループ
	MainLoop();
}
