#include <Windows.h>		// Windows APIを使用するための基本ヘッダー
#include <d3d12.h>			// DirectX 12のAPIを使用するための主要なヘッダー
#include <dxgi1_6.h>		// DirectX Graphics Infrastructure（DXGI）ヘッダー
#include <d3dcompiler.h>	// シェーダーをコンパイルするためのヘッダー
#include <wrl.h>			// Microsoft C++/WinRTで使用されるヘッダー（スマートポインタComPtrを使用するため)
#include "d3dx12.h"
#include <iostream>

using namespace Microsoft::WRL;	// Microsoft C++/WinRT

// グローバル変数
HWND hwnd;	// ウィンドウハンドル

// ウィンドウプロシージャ（ウィンドウのメッセージを処理する関数）
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// ウィンドウが閉じられた場合
	case WM_DESTROY:

		// メッセージループ終了
		PostQuitMessage(0);
		
		return 0;
	}

	// デフォルト処理を実行
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// ウィンドウの作成
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	// ウィンドウクラス
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;			// ウィンドウプロシージャ
	wc.hInstance = hInstance;			// インスタンスハンドル
	wc.lpszClassName = L"WindowClass";	// ウィンドウクラスの名前

	// ウィンドウクラスを登録
	if (!RegisterClass(&wc))
	{
		std::cout << "ウィンドウクラスの登録失敗!" << std::endl;
		return false;
	}

	// ウィンドウの作成
	hwnd = CreateWindowEx
	(
		0,							// 拡張スタイル
		L"WindowClass",				// クラス名
		L"UG_Engine",				// ウィンドウタイトル
		WS_OVERLAPPEDWINDOW,		// スタイル
		CW_USEDEFAULT, CW_DEFAULT,	// 初期位置
		WINDOW_WIDTH, WINDOW_HEIGHT,// 初期サイズ
		nullptr,					// 親ウィンドウ
		nullptr,					// メニュー
		hInstance,					// インスタンスハンドル
		nullptr						// 追加パラメータ
	);

	if (hwnd == nullptr)
	{
		std::cout << "ウィンドウ作成失敗" << std::endl;
		return false;
	}
	else
	{
		std::cout << "ウィンドウ作成成功" << std::endl;
	}

	// ウィンドウを表示
	ShowWindow(hwnd, nCmdShow);
	return true;
}

ComPtr<ID3D12Device> g_device;						// GPUデバイスの管理
ComPtr<ID3D12CommandQueue> g_commandQueue;			// コマンドの送信を管理
ComPtr<IDXGISwapChain4> g_swapChain;				// 画面に描画するバッファを管理
ComPtr<ID3D12DescriptorHeap> g_rtvHeap;				// レンダーターゲットビューの管理
ComPtr<ID3D12Resource> g_renderTargets[2];			// ダブルバッファ用
ComPtr<ID3D12CommandAllocator> g_commandAllocator;	// コマンドリストのメモリ管理
ComPtr<ID3D12GraphicsCommandList> g_commandList;	// GPUに送る描画コマンドを記録
ComPtr<ID3D12Fence> g_fence;						// GPUの処理が終わったことを確認するための同期オブジェクト
UINT64 g_fenceValue = 0;

// DirectXの初期化関数
bool InitializeDirectX12(HWND hwnd)
{
	HRESULT hr = S_OK;

	// DXGIファクトリの作成
	// GPUにアクセスできるようにする
	ComPtr<IDXGIFactory4> factory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		std::cout << "Error at step Factory:" << std::hex << hr << std::endl;
		return false;
	}


	// ハードウェアデバイスの作成
	hr = D3D12CreateDevice
	(
		nullptr,				// 自動的にGPUを選択
		D3D_FEATURE_LEVEL_11_0,	// 必要なDirectXの機能レベル
		IID_PPV_ARGS(&g_device)	// 作成するデバイスのポインタ
	);
	if (FAILED(hr))
	{
		std::cout << "Error at step Device:" << std::hex << hr << std::endl;
		return false;
	}


	// コマンドキューの作成
	// GPUに送るコマンドを管理する
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// 通常の描画コマンド

	hr = g_device->CreateCommandQueue		// GPUにコマンドを送るキューを作成
	(
		&queueDesc, 
		IID_PPV_ARGS(&g_commandQueue)
	);
	if (FAILED(hr))
	{
		std::cout << "Error at step CommandQueue:" << std::hex << hr << std::endl;
		return false;
	}


	// スワップチェーンの作成
	// フレームバッファを表示するためのバッファを管理する
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;	// ダブルバッファ
	swapChainDesc.Width = WINDOW_WIDTH;
	swapChainDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	// ウィンドウハンドルにバッファを関連付ける
	ComPtr<IDXGISwapChain1> swapChain;
	hr = factory->CreateSwapChainForHwnd
	(
		g_commandQueue.Get(),hwnd,
		&swapChainDesc,nullptr,nullptr,
		&swapChain
	);
	if (FAILED(hr))
	{
		std::cout << "Error at step Hwnd:" << std::hex << hr << std::endl;
		return false;
	}

	hr = swapChain.As(&g_swapChain);
	if (FAILED(hr))
	{
		std::cout << "Error at step SwapChain:" << std::hex << hr << std::endl;
		return false;
	}


	// レンダーターゲットビューの作成
	// 描画結果を表示する
	// RTV用のヒープを作成
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2;	// 描画バッファの数
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hr = g_device->CreateDescriptorHeap
	(
		&rtvHeapDesc,
		IID_PPV_ARGS(&g_rtvHeap)
	);
	if (FAILED(hr))
	{
		std::cout << "Error at step Heap:" << std::hex << hr << std::endl;
		return false;
	}

	// RTVハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		g_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	
	// バックバッファを取得してRTVを作成
	for (UINT i = 0; i < 2; i++)
	{
		hr = g_swapChain->GetBuffer(i, IID_PPV_ARGS(&g_renderTargets[i]));
		if (FAILED(hr))
		{
			std::cout << "Error at step Buffer:" << std::hex << hr << std::endl;
			return false;
		}

		// レンダーターゲットを設定
		g_device->CreateRenderTargetView(g_renderTargets[i].Get(), 
			nullptr, rtvHandle);

		rtvHandle.Offset(1, 
			g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}


	// コマンドアロケータ（コマンドを格納する場所）と
	// コマンドリスト（GPUに送るコマンドを記録するリスト）の作成
	hr = g_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&g_commandAllocator));
	if (FAILED(hr))
	{
		std::cout << "Error at step Allocator:" << std::hex << hr << std::endl;
		return false;
	}

	hr = g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		g_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&g_commandList));
	if (FAILED(hr))
	{
		std::cout << "Error at step CommandList:" << std::hex << hr << std::endl;
		return false;
	}


	// フェンスの作成
	// GPUのコマンドが完了したかを確認する
	// CPUとGPUの同期に使用
	hr = g_device->CreateFence(g_fenceValue, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&g_fence));
	if (FAILED(hr))
	{
		std::cout << "Error at step Fence:" << std::hex << hr << std::endl;
		return false;
	}


	return true;
}


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


	// ウィンドウの作成
	if (!CreateMainWindow(hInstance, nCmdShow))
	{
		return -1;
	}

	// DirectXの初期化
	if (!InitializeDirectX12(hwnd))
	{
		MessageBox(hwnd, L"DirectXの初期化に失敗しました", L"エラー", MB_OK);
		return -1;
	}

	// メインループ
	MSG msg = {};
	while (true)
	{
		/*
		メッセージを取り出して処理する
		メッセージが存在する場合はtrue
		*/
		if (PeekMessage
		(
			&msg,		// 受け取ったメッセージを格納
			nullptr,	// ウィンドウハンドルを指定（nullptrなのでアプリケーション全体）
			0, 0,		// メッセージの範囲（0なので全てのメッセージ）
			PM_REMOVE	// メッセージをキューから削除して取り出す
		))
		{
			if (msg.message == WM_QUIT) break;	// WM_QUITが来たら終了
			// キーボードメッセージを変換する
			TranslateMessage(&msg);
			// メッセージをウィンドウプロシージャに送信する
			DispatchMessage(&msg);
		}
		// メッセージがない場合、
		// アプリケーションのメインループ
		else
		{
		}
	}


	return 0;
}