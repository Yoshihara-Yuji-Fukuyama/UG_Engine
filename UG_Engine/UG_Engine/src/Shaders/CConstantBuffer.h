#pragma once
#include <d3dx12.h>

// �萔�o�b�t�@�N���X
class CConstantBuffer
{
public:
	// �R���X�g���N�^�Œ萔�o�b�t�@�𐶐�
	CConstantBuffer(size_t size);
	// �o�b�t�@�����ɐ������������擾
	bool IsCreated();
	// �o�b�t�@��GPU��̃A�h���X���擾
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const;
	// �萔�o�b�t�@�r���[��Ԃ�
	D3D12_CONSTANT_BUFFER_VIEW_DESC ViewDesc();

	// �萔�o�b�t�@�Ƀ}�b�s���O���ꂽ�|�C���^���擾
	void* GetPtr() const;

	template<typename T>
	T* GetPtr()
	{
		return reinterpret_cast<T*>(GetPtr());
	}

private:
	// �萔�o�b�t�@�����ɐ���������
	bool mIsCreated = false;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> mpBuffer;
	// �萔�o�b�t�@�r���[�̐ݒ�
	D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc;
	void* mpMappedPtr = nullptr;

	// �R�s�[�΍�
	CConstantBuffer(const CConstantBuffer&) = delete;
	// �������΍�
	void operator = (const CConstantBuffer&) = delete;
};