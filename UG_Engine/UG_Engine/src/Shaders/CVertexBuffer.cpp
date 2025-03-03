#include "CVertexBuffer.h"
#include "CEngine.h"
#include <d3dx12.h>
#include <iostream>

// �R���X�g���N�^�Œ��_�o�b�t�@�𐶐�
CVertexBuffer::CVertexBuffer(size_t size, size_t stride, const void* initData)
{
	// �q�[�v�v���p�e�B
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�̐ݒ�
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	// ���\�[�X�𐶐�
	auto hr = gEngine->GetDevice()->CreateCommittedResource
	(
		&prop,									// �q�[�v�v���p�e�B
		D3D12_HEAP_FLAG_NONE,					// �q�[�v�t���O
		&desc,									// ���\�[�X�̐ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,		// ���\�[�X�̏������
		nullptr,								// �N���A���Ȃ�
		IID_PPV_ARGS(mpBuffer.GetAddressOf())	// ���\�[�X�̏o�̓|�C���^
	);
	if (FAILED(hr))
	{
		std::cout << "���_�o�b�t�@���\�[�X�̐������s" << std::endl;
		return;
	}

	// ���_�o�b�t�@�r���[�̐ݒ�
	mView.BufferLocation = mpBuffer->GetGPUVirtualAddress();
	mView.SizeInBytes = static_cast<UINT>(size);
	mView.StrideInBytes = static_cast<UINT>(stride);

	// �}�b�s���O
	if (initData != nullptr)
	{
		void* ptr = nullptr;
		hr = mpBuffer->Map(0, nullptr, &ptr);
		if (FAILED(hr))
		{
			std::cout << "���_�o�b�t�@�}�b�s���O�Ɏ��s" << std::endl;
			return;
		}

		// ���_�f�[�^���}�b�s���O��ɐݒ�
		memcpy(ptr, initData, size);

		// �}�b�s���O����
		mpBuffer->Unmap(0, nullptr);
	}

	mIsCreated = true;
}

// ���_�o�b�t�@�r���[���擾
D3D12_VERTEX_BUFFER_VIEW CVertexBuffer::View() const
{
	return mView;
}

// �o�b�t�@�̐����ɐ������������擾
bool CVertexBuffer::IsCreated()
{
	return mIsCreated;
}
