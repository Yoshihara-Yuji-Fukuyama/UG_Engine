#include "CEngine.h"
#include <iostream>

// グローバル変数
CEngine* gEngine;

// エンジンを初期化
bool CEngine::Init(HWND hwnd, UINT windowWidth, UINT windowHeight)
{
	mFrameBufferWidth = windowWidth;
	mFrameBufferHeight = windowHeight;
	mHWnd = hwnd;

	// デバイスを生成
	if (!CreateDevice())
	{
		std::cout << "デバイスの生成失敗" << std::endl;
		return false;
	}
	// コマンドキューを生成
	if (!CreateCommandQueue())
	{
		std::cout << "コマンドキューの生成失敗" << std::endl;
		return false;
	}
	// スワップチェインを生成
	if (!CreateSwapChain())
	{
		std::cout << "スワップチェインの生成失敗" << std::endl;
		return false;
	}
	// コマンドリストとコマンドアロケータを生成
	if (!CreateCommandList())
	{
		std::cout << "コマンドリストとコマンドアロケータの生成失敗" << std::endl;
		return false;
	}
	// フェンスを生成
	if (!CreateFence())
	{
		std::cout << "フェンスの生成失敗" << std::endl;
		return false;
	}

	// ビューポートを生成
	CreateViewPort();
	// シザー短形を生成
	CreateScissorRect();

	std::cout << "描画エンジンの初期化成功" << std::endl;

	return true;
}

// 描画の開始
void CEngine::BeginRender()
{
}

// 描画の終了
void CEngine::EndRender()
{
}

// レンダーターゲットを生成
bool CEngine::CreateRenderTarget()
{
	return false;
}

// 震度ステンシルバッファを生成
bool CEngine::CreateDepthStencil()
{
	return false;
}

// 描画完了を待つ処理
void CEngine::WaitRender()
{
}

// デバイスを取得
ID3D12Device6* CEngine::GetDevice()
{
	return nullptr;
}

// コマンドリストを取得
ID3D12GraphicsCommandList* CEngine::GetCommandList()
{
	return nullptr;
}

UINT CEngine::CurrentBackBufferIndex()
{
	return 0;
}

// デバイスを生成
bool CEngine::CreateDevice()
{
	HRESULT hr = D3D12CreateDevice
	(
		nullptr,				// 自動的にGPUを選択
		D3D_FEATURE_LEVEL_11_0,	// 必要なDirectXの機能レベル
		IID_PPV_ARGS(			// 作成するデバイスのポインタ
			mpDevice.ReleaseAndGetAddressOf())	// 一旦mDeviceをクリアしてから割り当てる
	);
	return SUCCEEDED(hr);
}

// コマンドキューを生成
bool CEngine::CreateCommandQueue()
{
	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;			// コマンドを直接発行
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;// 通常の優先度
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;			// フラグなし
	desc.NodeMask = 0;	// デフォルトのGPUノード

	// コマンドキューを生成
	HRESULT hr = mpDevice->CreateCommandQueue
	(
		&desc,	// コマンドキューの設定
		IID_PPV_ARGS(mpQueue.ReleaseAndGetAddressOf())	// 一旦クリアして割り当て
	);

	return SUCCEEDED(hr);
}

// スワップチェインを生成
bool CEngine::CreateSwapChain()
{
	// DXGIファクトリ―の生成
	IDXGIFactory4* factory = nullptr;
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		std::cout << "DXGIファクトリの生成失敗" << std::hex << hr << std::endl;
		return false;
	}

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = mFrameBufferWidth;
	desc.BufferDesc.Height = mFrameBufferHeight;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = FRAME_BUFFER_COUNT;
	desc.OutputWindow = mHWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェインを生成
	IDXGISwapChain* swapChain = nullptr;
	hr = factory->CreateSwapChain
	(
		mpQueue.Get(),
		&desc,			// スワップチェインの設定
		&swapChain		// スワップチェイン
	);
	if (FAILED(hr))
	{
		factory->Release();
		return false;
	}

	// IDXGISwapChain3を取得
	hr = swapChain->QueryInterface(IID_PPV_ARGS(mpSwapChain.ReleaseAndGetAddressOf()));
	if (FAILED(hr))
	{
		factory->Release();
		swapChain->Release();
		return false;
	}

	// バックバッファ番号を取得
	mCurrentBackBufferIndex = mpSwapChain->GetCurrentBackBufferIndex();

	factory->Release();
	swapChain->Release();
	return true;
}

// コマンドリストとコマンドアロケータを生成
bool CEngine::CreateCommandList()
{
	// コマンドアロケータの作成
	HRESULT hr;
	for (size_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		hr = mpDevice->CreateCommandAllocator
		(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(mpAllocator[i].ReleaseAndGetAddressOf())
		);
	}
	if (FAILED(hr))
	{
		return false;
	}

	// コマンドリストの生成
	hr = mpDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mpAllocator[mCurrentBackBufferIndex].Get(),
		nullptr,
		IID_PPV_ARGS(&mpCommandList)
	);
	if (FAILED(hr))
	{
		return false;
	}

	// コマンドリストは開かれている状態で作成されるので、一旦閉じる
	mpCommandList->Close();

	return true;
}

// フェンスを生成
bool CEngine::CreateFence()
{
	for (auto i = 0u; i < FRAME_BUFFER_COUNT; i++)
	{
		mFenceValue[i] = 0;
	}

	// フェンスを生成
	HRESULT hr = mpDevice->CreateFence
	(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(mpFence.ReleaseAndGetAddressOf())
	);
	if (FAILED(hr))
	{
		return false;
	}

	mFenceValue[mCurrentBackBufferIndex]++;

	// 同期を行うときのイベントハンドラを生成
	mFenceEvent = CreateEvent
	(
		nullptr,
		FALSE,
		FALSE,
		nullptr
	);

	return mFenceEvent != nullptr;
}

// ビューポートを生成
void CEngine::CreateViewPort()
{
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mFrameBufferWidth);
	mViewport.Height = static_cast<float>(mFrameBufferHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

// シザー短径を生成
void CEngine::CreateScissorRect()
{
	mScissor.left = 0;
	mScissor.right = mFrameBufferWidth;
	mScissor.top = 0;
	mScissor.bottom = mFrameBufferHeight;
}
