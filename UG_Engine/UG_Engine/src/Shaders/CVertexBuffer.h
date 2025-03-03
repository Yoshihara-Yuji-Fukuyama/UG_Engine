#pragma once
#include <d3d12.h>

// 頂点バッファクラス
class CVertexBuffer
{
public:
	// コンストラクタで頂点バッファを生成
	CVertexBuffer(size_t size, size_t stride, const void* initData);
	// 頂点バッファビューを取得
	D3D12_VERTEX_BUFFER_VIEW View() const;
	// バッファの生成に成功したかを取得
	bool IsCreated();

private:
	// バッファの生成に成功したかを取得
	bool mIsCreated = false;
	// バッファ
	ComPtr<ID3D12Resource> mpBuffer = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW mView = {};

	// コピー対策
	CVertexBuffer(const CVertexBuffer&) = delete;
	// 代入操作対策
	void operator = (const CVertexBuffer&) = delete;
};