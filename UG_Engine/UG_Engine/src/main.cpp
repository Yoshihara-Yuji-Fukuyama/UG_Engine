#include <Windows.h>		// Windows APIを使用するための基本ヘッダー
#include <dxgi1_6.h>		// DirectX Graphics Infrastructure（DXGI）ヘッダー
#include <d3dcompiler.h>	// シェーダーをコンパイルするためのヘッダー
#include <wrl.h>			// Microsoft C++/WinRTで使用されるヘッダー（スマートポインタComPtrを使用するため)
#include "d3dx12.h"
#include <iostream>
#include "CApplication.h"

//using namespace Microsoft::WRL;	// Microsoft C++/WinRT
//
//ComPtr<ID3D12Device> gDevice;						// GPUデバイスの管理
//ComPtr<ID3D12CommandQueue> gCommandQueue;			// 
//ComPtr<IDXGISwapChain4> gSwapChain;					// 
//ComPtr<ID3D12DescriptorHeap> gRtvHeap;				// 
//ComPtr<ID3D12Resource> gRenderTargets[2];			// ダブルバッファ用
//ComPtr<ID3D12CommandAllocator> gCommandAllocator;	// 
//ComPtr<ID3D12GraphicsCommandList> gCommandList;		// 
//ComPtr<ID3D12Fence> gFence;							// 
//UINT64 gFenceValue = 0;
//
//
//// DirectXの初期化関数
//bool InitializeDirectX12(HWND gHWnd)
//{
//	HRESULT hr = S_OK;
//
//	// DXGIファクトリの作成
//	// GPUにアクセスできるようにする
//	ComPtr<IDXGIFactory4> factory;
//	hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
//	if (FAILED(hr))
//	{
//		std::cout << "DXGIファクトリの作成失敗" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// ハードウェアデバイスの作成
//	hr = D3D12CreateDevice
//	(
//		nullptr,				// 自動的にGPUを選択
//		D3D_FEATURE_LEVEL_11_0,	// 必要なDirectXの機能レベル
//		IID_PPV_ARGS(&gDevice)	// 作成するデバイスのポインタ
//	);
//	if (FAILED(hr))
//	{
//		std::cout << "ハードウェアデバイスの作成失敗" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// コマンドキューの作成
//	// GPUに送るコマンドを管理する
//	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
//	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// 通常の描画コマンド
//
//	hr = gDevice->CreateCommandQueue		// GPUにコマンドを送るキューを作成
//	(
//		&queueDesc, 
//		IID_PPV_ARGS(&gCommandQueue)
//	);
//	if (FAILED(hr))
//	{
//		std::cout << "コマンドキューの作成失敗" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// スワップチェーンの作成
//	// フレームバッファを表示するためのバッファを管理する
//	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
//	swapChainDesc.BufferCount = 2;	// ダブルバッファ
//	swapChainDesc.Width = WINDOW_WIDTH;
//	swapChainDesc.Height = WINDOW_HEIGHT;
//	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//	swapChainDesc.SampleDesc.Count = 1;
//
//	// ウィンドウハンドルにバッファを関連付ける
//	ComPtr<IDXGISwapChain1> swapChain;
//	hr = factory->CreateSwapChainForHwnd
//	(
//		gCommandQueue.Get(),gHWnd,
//		&swapChainDesc,nullptr,nullptr,
//		&swapChain
//	);
//	if (FAILED(hr))
//	{
//		std::cout << "ウィンドウハンドルに関連付け失敗:" << std::hex << hr << std::endl;
//		return false;
//	}
//
//	hr = swapChain.As(&gSwapChain);
//	if (FAILED(hr))
//	{
//		std::cout << "スワップチェーンの作成失敗" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// レンダーターゲットビューの作成
//	// 描画結果を表示する
//	// RTV用のヒープを作成
//	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
//	rtvHeapDesc.NumDescriptors = 2;	// 描画バッファの数
//	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//
//	hr = gDevice->CreateDescriptorHeap
//	(
//		&rtvHeapDesc,
//		IID_PPV_ARGS(&gRtvHeap)
//	);
//	if (FAILED(hr))
//	{
//		std::cout << "ヒープの作成失敗" << std::hex << hr << std::endl;
//		return false;
//	}
//
//	// RTVハンドルを取得
//	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
//		gRtvHeap->GetCPUDescriptorHandleForHeapStart());
//	
//	// バックバッファを取得してRTVを作成
//	for (UINT i = 0; i < 2; i++)
//	{
//		hr = gSwapChain->GetBuffer(i, IID_PPV_ARGS(&gRenderTargets[i]));
//		if (FAILED(hr))
//		{
//			std::cout << "バッファの取得失敗" << std::hex << hr << std::endl;
//			return false;
//		}
//
//		// レンダーターゲットを設定
//		gDevice->CreateRenderTargetView(gRenderTargets[i].Get(), 
//			nullptr, rtvHandle);
//
//		rtvHandle.Offset(1, 
//			gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
//	}
//
//
//	// コマンドアロケータ（コマンドを格納する場所）と
//	// コマンドリスト（GPUに送るコマンドを記録するリスト）の作成
//	hr = gDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
//		IID_PPV_ARGS(&gCommandAllocator));
//	if (FAILED(hr))
//	{
//		std::cout << "コマンドアロケータの作成失敗" << std::hex << hr << std::endl;
//		return false;
//	}
//
//	hr = gDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
//		gCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&gCommandList));
//	if (FAILED(hr))
//	{
//		std::cout << "コマンドリストの作成失敗" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// フェンスの作成
//	// GPUのコマンドが完了したかを確認する
//	// CPUとGPUの同期に使用
//	hr = gDevice->CreateFence(gFenceValue, D3D12_FENCE_FLAG_NONE,
//		IID_PPV_ARGS(&gFence));
//	if (FAILED(hr))
//	{
//		std::cout << "フェンスの作成失敗" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	return true;
//}
//
//
//// 描画処理
//void RenderFrame()
//{
//	HRESULT hr;
//
//	// フェンスの待機（GPUが終了するのを待つ）
//	if (gFence->GetCompletedValue() < gFenceValue)
//	{
//		hr = gFence->SetEventOnCompletion(gFenceValue, nullptr);
//		if (FAILED(hr))
//		{
//			std::cout << "フェンスのイベント設定失敗" << std::hex << hr << std::endl;
//		}
//
//		// イベントが発生するのを待機
//		WaitForSingleObject(nullptr, INFINITE);
//	}
//
//
//	// コマンドアロケータをリセット
//	hr = gCommandAllocator->Reset();
//	if (FAILED(hr))
//	{
//		std::cout << "コマンドアロケータのリセット失敗" << std::hex << hr << std::endl;
//		return;
//	}
//
//
//	// コマンドリストのリセット
//	hr = gCommandList->Reset(gCommandAllocator.Get(), nullptr);
//	if (FAILED(hr))
//	{
//		std::cout << "コマンドリストのリセット失敗" << std::hex << hr << std::endl;
//		return;
//	}
//
//
//	// レンダーターゲットを設定
//	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition
//	(
//		gRenderTargets[0].Get(),
//		D3D12_RESOURCE_STATE_PRESENT,
//		D3D12_RESOURCE_STATE_RENDER_TARGET
//	);
//	gCommandList->ResourceBarrier(1, &barrier);
//
//
//	// 描画対象となるRTVを設定
//	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(gRtvHeap->GetCPUDescriptorHandleForHeapStart(),
//		0, gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
//	gCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
//
//
//	// 背景色をクリア
//	const FLOAT clearColor[] = { 0.0f,0.0f,0.0f,1.0f };	// 黒色
//	gCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
//
//
//	// リソースの状態を"Present"に戻す
//	barrier = CD3DX12_RESOURCE_BARRIER::Transition
//	(
//		gRenderTargets[0].Get(),
//		D3D12_RESOURCE_STATE_RENDER_TARGET,
//		D3D12_RESOURCE_STATE_PRESENT
//	);
//	gCommandList->ResourceBarrier(1, &barrier);
//
//
//	// コマンドリストの終了
//	hr = gCommandList->Close();
//	if (FAILED(hr))
//	{
//		std::cout << "コマンドリストの終了失敗" << std::hex << hr << std::endl;
//		return;
//	}
//
//
//	// コマンドリストをGPUに送る
//	ID3D12CommandList* ppCommandLists[] = { gCommandList.Get() };
//	gCommandQueue->ExecuteCommandLists(1, ppCommandLists);
//
//
//	// GPUの処理が終わるのを待つ
//	gFenceValue++;
//	hr = gCommandQueue->Signal(gFence.Get(), gFenceValue);
//	if (FAILED(hr))
//	{
//		std::cout << "GPUのシグナル送信失敗" << std::hex << hr << std::endl;
//		return;
//	}
//
//
//	// スワップチェーンのバッファを切り替え
//	gSwapChain->Present(1, 0);	// 1フレーム待機
//}


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

	//// ウィンドウの作成
	//if (!CreateMainWindow(hInstance, nCmdShow))
	//{
	//	return -1;
	//}

	//// DirectXの初期化
	//if (!InitializeDirectX12(hwnd))
	//{
	//	MessageBox(hwnd, L"DirectXの初期化に失敗しました", L"エラー", MB_OK);
	//	return -1;
	//}

	StartApp(TEXT(ENGINE_TITLE));

	return 0;
}