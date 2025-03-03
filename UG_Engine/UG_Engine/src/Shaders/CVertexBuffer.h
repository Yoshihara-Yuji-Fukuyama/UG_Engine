#pragma once
#include <d3d12.h>

// ���_�o�b�t�@�N���X
class CVertexBuffer
{
public:
	// �R���X�g���N�^�Œ��_�o�b�t�@�𐶐�
	CVertexBuffer(size_t size, size_t stride, const void* initData);
	// ���_�o�b�t�@�r���[���擾
	D3D12_VERTEX_BUFFER_VIEW View() const;
	// �o�b�t�@�̐����ɐ������������擾
	bool IsCreated();

private:
	// �o�b�t�@�̐����ɐ������������擾
	bool mIsCreated = false;
	// �o�b�t�@
	ComPtr<ID3D12Resource> mpBuffer = nullptr;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW mView = {};

	// �R�s�[�΍�
	CVertexBuffer(const CVertexBuffer&) = delete;
	// �������΍�
	void operator = (const CVertexBuffer&) = delete;
};