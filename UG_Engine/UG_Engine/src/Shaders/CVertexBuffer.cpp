#include "CVertexBuffer.h"
#include "CEngine.h"
#include <d3dx12.h>
#include <iostream>

// コンストラクタで頂点バッファを生成
CVertexBuffer::CVertexBuffer(size_t size, size_t stride, const void* initData)
{
	// ヒーププロパティ
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソースの設定
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	// リソースを生成
	auto hr = gEngine->GetDevice()->CreateCommittedResource
	(
		&prop,									// ヒーププロパティ
		D3D12_HEAP_FLAG_NONE,					// ヒープフラグ
		&desc,									// リソースの設定
		D3D12_RESOURCE_STATE_GENERIC_READ,		// リソースの初期状態
		nullptr,								// クリアしない
		IID_PPV_ARGS(mpBuffer.GetAddressOf())	// リソースの出力ポインタ
	);
	if (FAILED(hr))
	{
		std::cout << "頂点バッファリソースの生成失敗" << std::endl;
		return;
	}

	// 頂点バッファビューの設定
	mView.BufferLocation = mpBuffer->GetGPUVirtualAddress();
	mView.SizeInBytes = static_cast<UINT>(size);
	mView.StrideInBytes = static_cast<UINT>(stride);

	// マッピング
	if (initData != nullptr)
	{
		void* ptr = nullptr;
		hr = mpBuffer->Map(0, nullptr, &ptr);
		if (FAILED(hr))
		{
			std::cout << "頂点バッファマッピングに失敗" << std::endl;
			return;
		}

		// 頂点データをマッピング先に設定
		memcpy(ptr, initData, size);

		// マッピング解除
		mpBuffer->Unmap(0, nullptr);
	}

	mIsCreated = true;
}

// 頂点バッファビューを取得
D3D12_VERTEX_BUFFER_VIEW CVertexBuffer::View() const
{
	return mView;
}

// バッファの生成に成功したかを取得
bool CVertexBuffer::IsCreated()
{
	return mIsCreated;
}
