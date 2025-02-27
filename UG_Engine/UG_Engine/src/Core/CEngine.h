#pragma once
#include <d3d12.h>			// DirectX 12��API���g�p���邽�߂̎�v�ȃw�b�_�[
#include <dxgi.h>			// DXGI�̊�{�@�\
#include <dxgi1_4.h>		// DXGI 1.4�̊g��

class CEngine
{
	// �萔
public:
	enum { FRAME_BUFFER_COUNT = 2 }; // �_�u���o�b�t�@�����O����̂�2


	// �������ƕ`��
public:
	// �G���W����������
	bool Init(HWND hwnd, UINT windowWidth, UINT windowHeight);

	// �`��̊J�n
	void BeginRender();
	// �`��̏I��
	void EndRender();


	// �A�N�Z�X�p
public:
	// �f�o�C�X���擾
	ID3D12Device6* GetDevice();
	// �R�}���h���X�g���擾
	ID3D12GraphicsCommandList* GetCommandList();
	UINT CurrentBackBufferIndex();


	// DirectX12�̏������Ɏg���֐�
private:
	// �f�o�C�X�𐶐�
	bool CreateDevice();
	// �R�}���h�L���[�𐶐�
	bool CreateCommandQueue();
	// �X���b�v�`�F�C���𐶐�
	bool CreateSwapChain();
	// �R�}���h���X�g�ƃR�}���h�A���P�[�^�𐶐�
	bool CreateCommandList();
	// �t�F���X�𐶐�
	bool CreateFence();
	// �r���[�|�[�g�𐶐�
	void CreateViewPort();
	// �V�U�[�Z�a�𐶐�
	void CreateScissorRect();


	// �`��Ɏg��DirectX12�̃I�u�W�F�N�g
private:
	HWND mHWnd;
	UINT mFrameBufferWidth = 0;
	UINT mFrameBufferHeight = 0;
	UINT mCurrentBackBufferIndex = 0;

	// GPU�f�o�C�X
	ComPtr<ID3D12Device6> mpDevice = nullptr;
	// �R�}���h�L���[�i�R�}���h�̑��M���Ǘ��j
	ComPtr<ID3D12CommandQueue> mpQueue = nullptr;
	// �X���b�v�`�F�C���i��ʂɕ`�悷��o�b�t�@���Ǘ��j
	ComPtr<IDXGISwapChain3> mpSwapChain = nullptr;
	// �R�}���h�A���P�[�^�i�R�}���h���X�g�̃������Ǘ��j
	ComPtr<ID3D12CommandAllocator> mpAllocator[FRAME_BUFFER_COUNT] = { nullptr };
	// �R�}���h���X�g�iGPU�ɑ���`��R�}���h���L�^�j
	ComPtr<ID3D12GraphicsCommandList> mpCommandList = nullptr;
	// �t�F���X�Ŏg���C�x���g
	HANDLE mFenceEvent = nullptr;
	// �t�F���X�iGPU�̏������I��������Ƃ��m�F���邽�߂̓����I�u�W�F�N�g�j
	ComPtr<ID3D12Fence> mpFence = nullptr;
	// �t�F���X�̒l�i�_�u���o�b�t�@�����O�p��2�j
	UINT64 mFenceValue[FRAME_BUFFER_COUNT];
	// �r���[�|�[�g
	D3D12_VIEWPORT mViewport;
	// �V�U�[�Z�`
	D3D12_RECT mScissor;


	// �`��Ɏg���I�u�W�F�N�g�Ɛ����֐�
private:
	// �����_�[�^�[�Q�b�g�𐶐�
	bool CreateRenderTarget();	
	// �k�x�X�e���V���o�b�t�@�𐶐�
	bool CreateDepthStencil();

	// �����_�[�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�T�C�Y
	UINT mRtvDescriptorSize = 0;
	// �����_�[�^�[�Q�b�g�̃f�B�X�N���v�^�q�[�v�i�����_�[�^�[�Q�b�g�r���[�̊Ǘ��j
	ComPtr<ID3D12DescriptorHeap>mRtvHeap = nullptr;
	// �����_�[�^�[�Q�b�g�i�_�u���o�b�t�@�����O����̂�2�j
	ComPtr<ID3D12Resource> mRenderTargets[FRAME_BUFFER_COUNT] = { nullptr };

	// �k�x�X�e���V���̃f�B�X�N���v�^�\�T�C�Y
	UINT mDsvDescriptorSize = 0;
	// �k�x�X�e���V���̃f�B�X�N���v�^�q�[�v�i�k�x�X�e���V���̊Ǘ��j
	ComPtr<ID3D12DescriptorHeap> mDsvHeap = nullptr;
	// �k�x�X�e���V���o�b�t�@�i��������1�ł����j
	ComPtr<ID3D12Resource> mDepthStencilBuffer = nullptr;


	// �`�惋�[�v�Ŏg�p
private:
	// ���݂̃t���[���̃����_�[�^�[�Q�b�g���ꎞ�I�ɕۑ�����ϐ�
	ID3D12Resource* mCurrentRenderTarget = nullptr;
	// �`�抮����҂���
	void WaitRender();

};

// �O���[�o���ϐ�
extern CEngine* gEngine;