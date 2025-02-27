#include "CEngine.h"
#include <iostream>

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

// �����_�[�^�[�Q�b�g�𐶐�
bool CEngine::CreateRenderTarget()
{
	return false;
}

// �k�x�X�e���V���o�b�t�@�𐶐�
bool CEngine::CreateDepthStencil()
{
	return false;
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
	desc.BufferDesc.Width = mFrameBufferWidth;
	desc.BufferDesc.Height = mFrameBufferHeight;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = FRAME_BUFFER_COUNT;
	desc.OutputWindow = mHWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �X���b�v�`�F�C���𐶐�
	IDXGISwapChain* swapChain = nullptr;
	hr = factory->CreateSwapChain
	(
		mpQueue.Get(),
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
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(mpAllocator[i].ReleaseAndGetAddressOf())
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
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mpAllocator[mCurrentBackBufferIndex].Get(),
		nullptr,
		IID_PPV_ARGS(&mpCommandList)
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
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(mpFence.ReleaseAndGetAddressOf())
	);
	if (FAILED(hr))
	{
		return false;
	}

	mFenceValue[mCurrentBackBufferIndex]++;

	// �������s���Ƃ��̃C�x���g�n���h���𐶐�
	mFenceEvent = CreateEvent
	(
		nullptr,
		FALSE,
		FALSE,
		nullptr
	);

	return mFenceEvent != nullptr;
}

// �r���[�|�[�g�𐶐�
void CEngine::CreateViewPort()
{
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mFrameBufferWidth);
	mViewport.Height = static_cast<float>(mFrameBufferHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

// �V�U�[�Z�a�𐶐�
void CEngine::CreateScissorRect()
{
	mScissor.left = 0;
	mScissor.right = mFrameBufferWidth;
	mScissor.top = 0;
	mScissor.bottom = mFrameBufferHeight;
}
