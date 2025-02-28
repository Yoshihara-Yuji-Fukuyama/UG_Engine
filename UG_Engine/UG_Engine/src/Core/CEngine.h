#pragma once
#include <d3d12.h>			// DirectX 12のAPIを使用するための主要なヘッダー
#include <dxgi.h>			// DXGIの基本機能
#include <dxgi1_4.h>		// DXGI 1.4の拡張

#pragma comment(lib, "d3d12.lib")	// d3d12ライブラリをリンク
#pragma comment(lib, "dxgi.lib")	// dxgiライブラリをリンク

// 描画エンジン
class CEngine
{
	// 定数
public:
	enum { FRAME_BUFFER_COUNT = 2 }; // ダブルバッファリングするので2


	// 初期化と描画
public:
	// エンジンを初期化
	bool Init(HWND hwnd, UINT windowWidth, UINT windowHeight);

	// 描画の開始
	void BeginRender();
	// 描画の終了
	void EndRender();


	// アクセス用
public:
	// デバイスを取得
	ID3D12Device6* GetDevice();
	// コマンドリストを取得
	ID3D12GraphicsCommandList* GetCommandList();
	UINT CurrentBackBufferIndex();


	// DirectX12の初期化に使う関数
private:
	// デバイスを生成
	bool CreateDevice();
	// コマンドキューを生成
	bool CreateCommandQueue();
	// スワップチェインを生成
	bool CreateSwapChain();
	// コマンドリスト）と
	// コマンドアロケータ（コマンドリストのメモリ管理するもの）を生成
	bool CreateCommandList();
	// フェンスを生成
	bool CreateFence();
	// ビューポートを生成
	void CreateViewPort();
	// シザー短径を生成
	void CreateScissorRect();


	// 描画に使うDirectX12のオブジェクト
private:
	HWND mHWnd;
	UINT mFrameBufferWidth = 0;
	UINT mFrameBufferHeight = 0;
	UINT mCurrentBackBufferIndex = 0;

	// GPUデバイス
	ComPtr<ID3D12Device6> mpDevice = nullptr;
	// コマンドキュー（コマンドの送信を管理）
	ComPtr<ID3D12CommandQueue> mpQueue = nullptr;
	// スワップチェイン（画面に描画するバッファを管理）
	ComPtr<IDXGISwapChain3> mpSwapChain = nullptr;
	// コマンドアロケータ（コマンドリストのメモリ管理）
	ComPtr<ID3D12CommandAllocator> mpAllocator[FRAME_BUFFER_COUNT] = { nullptr };
	// コマンドリスト（GPUに送る描画コマンドを記録）
	ComPtr<ID3D12GraphicsCommandList> mpCommandList = nullptr;
	// フェンスで使うイベント
	HANDLE mFenceEvent = nullptr;
	// フェンス（GPUの処理が終わったことを確認するための同期オブジェクト）
	ComPtr<ID3D12Fence> mpFence = nullptr;
	// フェンスの値（ダブルバッファリング用に2個）
	UINT64 mFenceValue[FRAME_BUFFER_COUNT];
	// ビューポート（レンダリング結果をどう表示するかの設定）
	D3D12_VIEWPORT mViewport;
	// シザー短形（どこからどこまでを画面に映すかの設定）
	D3D12_RECT mScissor;


	// 描画に使うオブジェクトと生成関数
private:
	// レンダーターゲットを生成
	bool CreateRenderTarget();	
	// 深度ステンシルバッファを生成
	bool CreateDepthStencil();

	// レンダーターゲットビューのディスクリプタサイズ
	UINT mRtvDescriptorSize = 0;
	// レンダーターゲットのディスクリプタヒープ（レンダーターゲットビューの管理）
	ComPtr<ID3D12DescriptorHeap>mpRtvHeap = nullptr;
	// レンダーターゲット（ダブルバッファリングするので2個）
	ComPtr<ID3D12Resource> mpRenderTargets[FRAME_BUFFER_COUNT] = { nullptr };

	// 深度ステンシルのディスクリプタ―サイズ
	UINT mDsvDescriptorSize = 0;
	// 深度ステンシルのディスクリプタヒープ（深度ステンシルの管理）
	ComPtr<ID3D12DescriptorHeap> mpDsvHeap = nullptr;
	// 深度ステンシルバッファ（こっちは1個でいい）
	ComPtr<ID3D12Resource> mpDepthStencilBuffer = nullptr;


	// 描画ループで使用
private:
	// 現在のフレームのレンダーターゲットを一時的に保存する変数
	ID3D12Resource* mCurrentRenderTarget = nullptr;
	// 描画完了を待つ処理
	void WaitRender();

};

// グローバル変数
extern CEngine* gEngine;