#include "CEngine.h"
#include <iostream>
#include <d3dx12.h>

// �O���[�o���ϐ�
CEngine* gEngine;

// �G���W����������
bool CEngine::Init(HWND hwnd, UINT windowWidth, UINT windowHeight)
{
	mFrameBufferWidth = windowWidth;
	mFrameBufferHeight = windowHeight;
	mHWnd = hwnd;

	// �f�o�C�X�𐶐�
	if (!CreateDevice())
	{
		std::cout << "�f�o�C�X�̐������s" << std::endl;
		return false;
	}
	// �R�}���h�L���[�𐶐�
	if (!CreateCommandQueue())
	{
		std::cout << "�R�}���h�L���[�̐������s" << std::endl;
		return false;
	}
	// �X���b�v�`�F�C���𐶐�
	if (!CreateSwapChain())
	{
		std::cout << "�X���b�v�`�F�C���̐������s" << std::endl;
		return false;
	}
	// �R�}���h���X�g�ƃR�}���h�A���P�[�^�𐶐�
	if (!CreateCommandList())
	{
		std::cout << "�R�}���h���X�g�ƃR�}���h�A���P�[�^�̐������s" << std::endl;
		return false;
	}
	// �t�F���X�𐶐�
	if (!CreateFence())
	{
		std::cout << "�t�F���X�̐������s" << std::endl;
		return false;
	}

	// �r���[�|�[�g�𐶐�
	CreateViewPort();
	// �V�U�[�Z�`�𐶐�
	CreateScissorRect();

	// �����_�[�^�[�Q�b�g�𐶐�
	if (!CreateRenderTarget())
	{
		std::cout << "�����_�[�^�[�Q�b�g�̐������s" << std::endl;
		return false;
	}

	// �f�v�X�X�e���V���o�b�t�@�̐���
	if (!CreateDepthStencil())
	{
		std::cout << "�f�v�X�X�e���V���o�b�t�@�̐������s" << std::endl;
		return false;
	}

	std::cout << "�`��G���W���̏���������" << std::endl;

	return true;
}

// �`��̊J�n
void CEngine::BeginRender()
{
}

// �`��̏I��
void CEngine::EndRender()
{
}

// �`�抮����҂���
void CEngine::WaitRender()
{
}

// �f�o�C�X���擾
ID3D12Device6* CEngine::GetDevice()
{
	return nullptr;
}

// �R�}���h���X�g���擾
ID3D12GraphicsCommandList* CEngine::GetCommandList()
{
	return nullptr;
}

UINT CEngine::CurrentBackBufferIndex()
{
	return 0;
}

// �f�o�C�X�𐶐�
bool CEngine::CreateDevice()
{
	HRESULT hr = D3D12CreateDevice
	(
		nullptr,				// �����I��GPU��I��
		D3D_FEATURE_LEVEL_11_0,	// �K�v��DirectX�̋@�\���x��
		IID_PPV_ARGS(			// �쐬����f�o�C�X�̃|�C���^
			mpDevice.ReleaseAndGetAddressOf())	// ��UmDevice���N���A���Ă��犄�蓖�Ă�
	);
	return SUCCEEDED(hr);
}

// �R�}���h�L���[�𐶐�
bool CEngine::CreateCommandQueue()
{
	// �R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;			// �R�}���h�𒼐ڔ��s
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;// �ʏ�̗D��x
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;			// �t���O�Ȃ�
	desc.NodeMask = 0;	// �f�t�H���g��GPU�m�[�h

	// �R�}���h�L���[�𐶐�
	HRESULT hr = mpDevice->CreateCommandQueue
	(
		&desc,	// �R�}���h�L���[�̐ݒ�
		IID_PPV_ARGS(mpQueue.ReleaseAndGetAddressOf())	// ��U�N���A���Ċ��蓖��
	);

	return SUCCEEDED(hr);
}

// �X���b�v�`�F�C���𐶐�
bool CEngine::CreateSwapChain()
{
	// DXGI�t�@�N�g���\�̐���
	IDXGIFactory4* factory = nullptr;
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		std::cout << "DXGI�t�@�N�g���̐������s" << std::hex << hr << std::endl;
		return false;
	}

	// �X���b�v�`�F�C���̐ݒ�
	DXGI_SWAP_CHAIN_DESC desc = {};
	// �𑜓x
	desc.BufferDesc.Width = mFrameBufferWidth;	// ��
	desc.BufferDesc.Height = mFrameBufferHeight;// ����
	// �`��X�V�p�x�i1/60 = 60FPS�j
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	// �X�L�������C������
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// �X�P�[�����O
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// �J���[�t�H�[�}�b�g
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �}���`�T���v�����O�i�A���`�G�C���A�X�Ȃ��j
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	// �o�b�t�@�̎g����
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �����_�[�^�[�Q�b�g�Ƃ��Ďg�p
	// �o�b�t�@�̐�
	desc.BufferCount = FRAME_BUFFER_COUNT;
	// �`��E�B���h�E
	desc.OutputWindow = mHWnd;	// �K�p����E�B���h�E�̃n���h��
	desc.Windowed = TRUE;		// �E�B���h�E���[�h�œ���
	// ��ʍX�V�̍ۂɁA�O�̃t���[�����̂ĂĐV�����t���[����\������ݒ�
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// �t���X�N���[�����̉𑜓x�ύX������
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �X���b�v�`�F�C���𐶐�
	IDXGISwapChain* swapChain = nullptr;
	hr = factory->CreateSwapChain
	(
		mpQueue.Get(),	// �R�}���h�L���[
		&desc,			// �X���b�v�`�F�C���̐ݒ�
		&swapChain		// �X���b�v�`�F�C��
	);
	if (FAILED(hr))
	{
		factory->Release();
		return false;
	}

	// IDXGISwapChain3���擾
	hr = swapChain->QueryInterface(IID_PPV_ARGS(mpSwapChain.ReleaseAndGetAddressOf()));
	if (FAILED(hr))
	{
		factory->Release();
		swapChain->Release();
		return false;
	}

	// �o�b�N�o�b�t�@�ԍ����擾
	mCurrentBackBufferIndex = mpSwapChain->GetCurrentBackBufferIndex();

	factory->Release();
	swapChain->Release();
	return true;
}

// �R�}���h���X�g�ƃR�}���h�A���P�[�^�𐶐�
bool CEngine::CreateCommandList()
{
	// �R�}���h�A���P�[�^�̍쐬
	HRESULT hr;
	for (size_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		hr = mpDevice->CreateCommandAllocator
		(
			D3D12_COMMAND_LIST_TYPE_DIRECT,	// �ʏ�̕`��
			IID_PPV_ARGS(mpAllocator[i].ReleaseAndGetAddressOf())	// �R�}���h�A���P�[�^�̃|�C���^
		);
	}
	if (FAILED(hr))
	{
		return false;
	}

	// �R�}���h���X�g�̐���
	hr = mpDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,	// �ʏ�̕`��
		mpAllocator[mCurrentBackBufferIndex].Get(),	// �R�}���h�A���P�[�^�̕R�Â�
		nullptr,
		IID_PPV_ARGS(&mpCommandList)	// �쐬�����R�}���h���X�g�̃|�C���^
	);
	if (FAILED(hr))
	{
		return false;
	}

	// �R�}���h���X�g�͊J����Ă����Ԃō쐬�����̂ŁA��U����
	mpCommandList->Close();

	return true;
}

// �t�F���X�𐶐�
bool CEngine::CreateFence()
{
	for (auto i = 0u; i < FRAME_BUFFER_COUNT; i++)
	{
		mFenceValue[i] = 0;
	}

	// �t�F���X�𐶐�
	HRESULT hr = mpDevice->CreateFence
	(
		0,
		D3D12_FENCE_FLAG_NONE,	// �ʏ�̃t�F���X
		IID_PPV_ARGS(mpFence.ReleaseAndGetAddressOf())	// �t�F���X�̃|�C���^
	);
	if (FAILED(hr))
	{
		return false;
	}

	// �o�b�N�o�b�t�@�p�̃t�F���X�l��1���₷
	mFenceValue[mCurrentBackBufferIndex]++;

	// �������s���Ƃ��̃C�x���g�n���h���𐶐�
	mFenceEvent = CreateEvent
	(
		nullptr,	// �Z�L�����e�B�����i�f�t�H���g�j
		FALSE,		// �������Z�b�g
		FALSE,		// �������
		nullptr		// ���O�i�����j
	);

	return mFenceEvent != nullptr;
}

// �r���[�|�[�g�𐶐�
void CEngine::CreateViewPort()
{
	mViewport.TopLeftX = 0;	// �����X���W
	mViewport.TopLeftY = 0;	// �����Y���W
	mViewport.Width = static_cast<float>(mFrameBufferWidth);	// ��
	mViewport.Height = static_cast<float>(mFrameBufferHeight);	// ����
	mViewport.MinDepth = 0.0f;	// �ŏ��[�x�i��O�̃I�u�W�F�N�g�j
	mViewport.MaxDepth = 1.0f;	// �ő�[�x�i���̃I�u�W�F�N�g�j
}

// �V�U�[�Z�a�𐶐�
void CEngine::CreateScissorRect()
{
	mScissor.left = 0;						// ���[��X���W
	mScissor.right = mFrameBufferWidth;		// �E�[��X���W
	mScissor.top = 0;						// ��[��Y���W
	mScissor.bottom = mFrameBufferHeight;	// ���[��Y���W
}

// �����_�[�^�[�Q�b�g�𐶐�
bool CEngine::CreateRenderTarget()
{
	// RTV�p�̃f�B�X�N���v�^�q�[�v�i���\�[�X�ւ̎Q�Ə����i�[���郁�����̈�j���쐬
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FRAME_BUFFER_COUNT;		// �t���[���o�b�t�@�̐�����RTV���m��
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// RTV�p�̃f�B�X�N���v�^�q�[�v
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// GPU�A�N�Z�X�s�v

	HRESULT hr = mpDevice->CreateDescriptorHeap
	(
		&desc,	// �ݒ�
		IID_PPV_ARGS(mpRtvHeap.ReleaseAndGetAddressOf())	// RTV�q�[�v�̃|�C���^
	);
	if (FAILED(hr))
	{
		std::cout << "RTV�p�̃f�B�X�N���v�^�q�[�v�̐������s" << std::hex << hr << std::endl;
		return false;
	}

	// �f�B�X�N���v�^�̃T�C�Y���擾
	mRtvDescriptorSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// RTV�q�[�v�̍ŏ��̃A�h���X���擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mpRtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		// �o�b�N�o�b�t�@���擾
		mpSwapChain->GetBuffer(i, IID_PPV_ARGS(mpRenderTargets[i].ReleaseAndGetAddressOf()));
		// �����_�[�^�[�Q�b�g�̐���
		mpDevice->CreateRenderTargetView
		(
			mpRenderTargets[i].Get(),	// �����_�[�^�[�Q�b�g
			nullptr,					// �f�t�H���g�i�����K�p�j
			rtvHandle					// �f�B�X�N���v�^�q�[�v�Ɋi�[
		);
		// ����RTV�p�̃A�h���X��
		rtvHandle.ptr += mRtvDescriptorSize;
	}

	return true;
}

// �[�x�X�e���V���o�b�t�@�𐶐�
bool CEngine::CreateDepthStencil()
{
	// DSV�p�̃f�B�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;	// 1��
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;		// DSV�p�̃f�B�X�N���v�^�q�[�v
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// GPU�A�N�Z�X�s�v

	HRESULT hr = mpDevice->CreateDescriptorHeap
	(
		&heapDesc,	// �ݒ�
		IID_PPV_ARGS(&mpDsvHeap)	// DSV�q�[�v�̃|�C���^
	);
	if (FAILED(hr))
	{
		std::cout << "DSV�p�̃f�B�X�N���v�^�q�[�v�̐������s" << std::hex << hr << std::endl;
		return false;
	}

	// �f�B�X�N���v�^�̃T�C�Y���擾
	mDsvDescriptorSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// �[�x�X�e���V���o�b�t�@�̐���
	D3D12_CLEAR_VALUE dsvClearValue;	// �����N���A�l�̐ݒ�
	dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;	// 32�r�b�g���������_�[�x�t�H�[�}�b�g
	dsvClearValue.DepthStencil.Depth = 1.0f;		// �[�x�͍ő�l
	dsvClearValue.DepthStencil.Stencil = 0;			// �X�e���V���l��0

	// �o�b�t�@��e�N�X�`�����i�[����q�[�v�̃v���p�e�B
	// �œK�ȃf�t�H���g�q�[�v���g�p
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	// �쐬���郊�\�[�X�̐ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc
	(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,	// ���\�[�X��2D�e�N�X�`��
		0,	// MipMap���x��
		mFrameBufferWidth,	// �e�N�X�`���̕�
		mFrameBufferHeight,	// �e�N�X�`���̍���
		1,	// �[�x
		1,	// MipMap�̃��x����
		DXGI_FORMAT_D32_FLOAT,	// �t�H�[�}�b�g
		1,	// �T���v���̐�
		0,	// �J�n�̃p�f�B���O
		D3D12_TEXTURE_LAYOUT_UNKNOWN,	// �e�N�X�`���̃��C�A�E�g
		// �[�x�X�e���V�����\�[�X�Ƃ��Ďg�p���邱�Ƃ��w��
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL |
		// �V�F�[�_���\�[�X�r���[�Ƃ��ė��p�ł��Ȃ��悤�ɂ���
		D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE
	);

	// �������q�[�v�̊m�ۂƃ��\�[�X�̐���
	hr = mpDevice->CreateCommittedResource
	(
		&heapProp,							// �q�[�v�̃v���p�e�B
		D3D12_HEAP_FLAG_NONE,				// �q�[�v�̃t���O
		&resourceDesc,						// ���\�[�X�̐ݒ�
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	// �������\�[�X��ԁi�[�x�������݁j
		&dsvClearValue,						// �N���A�l�̐ݒ�
		IID_PPV_ARGS(mpDepthStencilBuffer.ReleaseAndGetAddressOf())	// �[�x�X�e���V���o�b�t�@�̃|�C���^
	);
	if (FAILED(hr))
	{
		std::cout << "�������q�[�v�̊m�ۂƃ��\�[�X�̐������s" << std::hex << hr << std::endl;
		return false;
	}

	// �f�B�X�N���v�^�𐶐�
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mpDsvHeap->GetCPUDescriptorHandleForHeapStart();

	// �[�x�X�e���V���r���[�𐶐�
	mpDevice->CreateDepthStencilView
	(
		mpDepthStencilBuffer.Get(),	// �[�x�X�e���V���o�b�t�@
		nullptr,	// �[�x�X�e���V���r���[�̐ݒ�i���ɐݒ�Ȃ��j
		dsvHandle	// �f�B�X�N���v�^�q�[�v�ɓo�^���邽�߂̃n���h��
	);

	return true;
}
