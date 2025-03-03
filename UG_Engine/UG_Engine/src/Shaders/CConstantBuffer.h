#pragma once
#include <d3dx12.h>

// 定数バッファクラス
class CConstantBuffer
{
public:
	// コンストラクタで定数バッファを生成
	CConstantBuffer(size_t size);
	// バッファ生成に成功したかを取得
	bool IsCreated();
	// バッファのGPU上のアドレスを取得
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const;
	// 定数バッファビューを返す
	D3D12_CONSTANT_BUFFER_VIEW_DESC ViewDesc();

	// 定数バッファにマッピングされたポインタを取得
	void* GetPtr() const;

	template<typename T>
	T* GetPtr()
	{
		return reinterpret_cast<T*>(GetPtr());
	}

private:
	// 定数バッファ生成に成功したか
	bool mIsCreated = false;
	// 定数バッファ
	ComPtr<ID3D12Resource> mpBuffer;
	// 定数バッファビューの設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc;
	void* mpMappedPtr = nullptr;

	// コピー対策
	CConstantBuffer(const CConstantBuffer&) = delete;
	// 代入操作対策
	void operator = (const CConstantBuffer&) = delete;
};