#include "CEngine.h"
#include <iostream>
#include <d3dx12.h>

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

	// レンダーターゲットを生成
	if (!CreateRenderTarget())
	{
		std::cout << "レンダーターゲットの生成失敗" << std::endl;
		return false;
	}

	// デプスステンシルバッファの生成
	if (!CreateDepthStencil())
	{
		std::cout << "デプスステンシルバッファの生成失敗" << std::endl;
		return false;
	}

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
	// 解像度
	desc.BufferDesc.Width = mFrameBufferWidth;	// 幅
	desc.BufferDesc.Height = mFrameBufferHeight;// 高さ
	// 描画更新頻度（1/60 = 60FPS）
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	// スキャンライン順序
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// スケーリング
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// カラーフォーマット
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// マルチサンプリング（アンチエイリアスなし）
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	// バッファの使い方
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// レンダーターゲットとして使用
	// バッファの数
	desc.BufferCount = FRAME_BUFFER_COUNT;
	// 描画ウィンドウ
	desc.OutputWindow = mHWnd;	// 適用するウィンドウのハンドル
	desc.Windowed = TRUE;		// ウィンドウモードで動作
	// 画面更新の際に、前のフレームを捨てて新しいフレームを表示する設定
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// フルスクリーン時の解像度変更を許可
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェインを生成
	IDXGISwapChain* swapChain = nullptr;
	hr = factory->CreateSwapChain
	(
		mpQueue.Get(),	// コマンドキュー
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
			D3D12_COMMAND_LIST_TYPE_DIRECT,	// 通常の描画
			IID_PPV_ARGS(mpAllocator[i].ReleaseAndGetAddressOf())	// コマンドアロケータのポインタ
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
		D3D12_COMMAND_LIST_TYPE_DIRECT,	// 通常の描画
		mpAllocator[mCurrentBackBufferIndex].Get(),	// コマンドアロケータの紐づけ
		nullptr,
		IID_PPV_ARGS(&mpCommandList)	// 作成したコマンドリストのポインタ
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
		D3D12_FENCE_FLAG_NONE,	// 通常のフェンス
		IID_PPV_ARGS(mpFence.ReleaseAndGetAddressOf())	// フェンスのポインタ
	);
	if (FAILED(hr))
	{
		return false;
	}

	// バックバッファ用のフェンス値を1増やす
	mFenceValue[mCurrentBackBufferIndex]++;

	// 同期を行うときのイベントハンドラを生成
	mFenceEvent = CreateEvent
	(
		nullptr,	// セキュリティ属性（デフォルト）
		FALSE,		// 自動リセット
		FALSE,		// 初期状態
		nullptr		// 名前（無名）
	);

	return mFenceEvent != nullptr;
}

// ビューポートを生成
void CEngine::CreateViewPort()
{
	mViewport.TopLeftX = 0;	// 左上のX座標
	mViewport.TopLeftY = 0;	// 左上のY座標
	mViewport.Width = static_cast<float>(mFrameBufferWidth);	// 幅
	mViewport.Height = static_cast<float>(mFrameBufferHeight);	// 高さ
	mViewport.MinDepth = 0.0f;	// 最小深度（手前のオブジェクト）
	mViewport.MaxDepth = 1.0f;	// 最大深度（奥のオブジェクト）
}

// シザー短径を生成
void CEngine::CreateScissorRect()
{
	mScissor.left = 0;						// 左端のX座標
	mScissor.right = mFrameBufferWidth;		// 右端のX座標
	mScissor.top = 0;						// 上端のY座標
	mScissor.bottom = mFrameBufferHeight;	// 下端のY座標
}

// レンダーターゲットを生成
bool CEngine::CreateRenderTarget()
{
	// RTV用のディスクリプタヒープ（リソースへの参照情報を格納するメモリ領域）を作成
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FRAME_BUFFER_COUNT;		// フレームバッファの数だけRTVを確保
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// RTV用のディスクリプタヒープ
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// GPUアクセス不要

	HRESULT hr = mpDevice->CreateDescriptorHeap
	(
		&desc,	// 設定
		IID_PPV_ARGS(mpRtvHeap.ReleaseAndGetAddressOf())	// RTVヒープのポインタ
	);
	if (FAILED(hr))
	{
		std::cout << "RTV用のディスクリプタヒープの生成失敗" << std::hex << hr << std::endl;
		return false;
	}

	// ディスクリプタのサイズを取得
	mRtvDescriptorSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// RTVヒープの最初のアドレスを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mpRtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		// バックバッファを取得
		mpSwapChain->GetBuffer(i, IID_PPV_ARGS(mpRenderTargets[i].ReleaseAndGetAddressOf()));
		// レンダーターゲットの生成
		mpDevice->CreateRenderTargetView
		(
			mpRenderTargets[i].Get(),	// レンダーターゲット
			nullptr,					// デフォルト（自動適用）
			rtvHandle					// ディスクリプタヒープに格納
		);
		// 次のRTV用のアドレスへ
		rtvHandle.ptr += mRtvDescriptorSize;
	}

	return true;
}

// 深度ステンシルバッファを生成
bool CEngine::CreateDepthStencil()
{
	// DSV用のディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;	// 1つ
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;		// DSV用のディスクリプタヒープ
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// GPUアクセス不要

	HRESULT hr = mpDevice->CreateDescriptorHeap
	(
		&heapDesc,	// 設定
		IID_PPV_ARGS(&mpDsvHeap)	// DSVヒープのポインタ
	);
	if (FAILED(hr))
	{
		std::cout << "DSV用のディスクリプタヒープの生成失敗" << std::hex << hr << std::endl;
		return false;
	}

	// ディスクリプタのサイズを取得
	mDsvDescriptorSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// 深度ステンシルバッファの生成
	D3D12_CLEAR_VALUE dsvClearValue;	// 初期クリア値の設定
	dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;	// 32ビット浮動小数点深度フォーマット
	dsvClearValue.DepthStencil.Depth = 1.0f;		// 深度は最大値
	dsvClearValue.DepthStencil.Stencil = 0;			// ステンシル値は0

	// バッファやテクスチャを格納するヒープのプロパティ
	// 最適なデフォルトヒープを使用
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	// 作成するリソースの設定
	CD3DX12_RESOURCE_DESC resourceDesc
	(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,	// リソースが2Dテクスチャ
		0,	// MipMapレベル
		mFrameBufferWidth,	// テクスチャの幅
		mFrameBufferHeight,	// テクスチャの高さ
		1,	// 深度
		1,	// MipMapのレベル数
		DXGI_FORMAT_D32_FLOAT,	// フォーマット
		1,	// サンプルの数
		0,	// 開始のパディング
		D3D12_TEXTURE_LAYOUT_UNKNOWN,	// テクスチャのレイアウト
		// 深度ステンシルリソースとして使用することを指定
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL |
		// シェーダリソースビューとして利用できないようにする
		D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE
	);

	// メモリヒープの確保とリソースの生成
	hr = mpDevice->CreateCommittedResource
	(
		&heapProp,							// ヒープのプロパティ
		D3D12_HEAP_FLAG_NONE,				// ヒープのフラグ
		&resourceDesc,						// リソースの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	// 初期リソース状態（深度書き込み）
		&dsvClearValue,						// クリア値の設定
		IID_PPV_ARGS(mpDepthStencilBuffer.ReleaseAndGetAddressOf())	// 深度ステンシルバッファのポインタ
	);
	if (FAILED(hr))
	{
		std::cout << "メモリヒープの確保とリソースの生成失敗" << std::hex << hr << std::endl;
		return false;
	}

	// ディスクリプタを生成
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mpDsvHeap->GetCPUDescriptorHandleForHeapStart();

	// 深度ステンシルビューを生成
	mpDevice->CreateDepthStencilView
	(
		mpDepthStencilBuffer.Get(),	// 深度ステンシルバッファ
		nullptr,	// 深度ステンシルビューの設定（特に設定なし）
		dsvHandle	// ディスクリプタヒープに登録するためのハンドル
	);

	return true;
}
