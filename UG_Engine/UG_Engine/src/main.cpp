#include <Windows.h>		// Windows API���g�p���邽�߂̊�{�w�b�_�[
#include <d3d12.h>			// DirectX 12��API���g�p���邽�߂̎�v�ȃw�b�_�[
#include <dxgi1_6.h>		// DirectX Graphics Infrastructure�iDXGI�j�w�b�_�[
#include <d3dcompiler.h>	// �V�F�[�_�[���R���p�C�����邽�߂̃w�b�_�[
#include <wrl.h>			// Microsoft C++/WinRT�Ŏg�p�����w�b�_�[�i�X�}�[�g�|�C���^ComPtr���g�p���邽��)
#include "d3dx12.h"
#include <iostream>

using namespace Microsoft::WRL;	// Microsoft C++/WinRT

// �O���[�o���ϐ�
HWND hwnd;	// �E�B���h�E�n���h��

// �E�B���h�E�v���V�[�W���i�E�B���h�E�̃��b�Z�[�W����������֐��j
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// �E�B���h�E������ꂽ�ꍇ
	case WM_DESTROY:

		// ���b�Z�[�W���[�v�I��
		PostQuitMessage(0);
		
		return 0;
	}

	// �f�t�H���g���������s
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// �E�B���h�E�̍쐬
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	// �E�B���h�E�N���X
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;			// �E�B���h�E�v���V�[�W��
	wc.hInstance = hInstance;			// �C���X�^���X�n���h��
	wc.lpszClassName = L"WindowClass";	// �E�B���h�E�N���X�̖��O

	// �E�B���h�E�N���X��o�^
	if (!RegisterClass(&wc))
	{
		std::cout << "�E�B���h�E�N���X�̓o�^���s!" << std::endl;
		return false;
	}

	// �E�B���h�E�̍쐬
	hwnd = CreateWindowEx
	(
		0,							// �g���X�^�C��
		L"WindowClass",				// �N���X��
		L"UG_Engine",				// �E�B���h�E�^�C�g��
		WS_OVERLAPPEDWINDOW,		// �X�^�C��
		CW_USEDEFAULT, CW_DEFAULT,	// �����ʒu
		WINDOW_WIDTH, WINDOW_HEIGHT,// �����T�C�Y
		nullptr,					// �e�E�B���h�E
		nullptr,					// ���j���[
		hInstance,					// �C���X�^���X�n���h��
		nullptr						// �ǉ��p�����[�^
	);

	if (hwnd == nullptr)
	{
		std::cout << "�E�B���h�E�쐬���s" << std::endl;
		return false;
	}
	else
	{
		std::cout << "�E�B���h�E�쐬����" << std::endl;
	}

	// �E�B���h�E��\��
	ShowWindow(hwnd, nCmdShow);
	return true;
}

ComPtr<ID3D12Device> g_device;						// GPU�f�o�C�X�̊Ǘ�
ComPtr<ID3D12CommandQueue> g_commandQueue;			// �R�}���h�̑��M���Ǘ�
ComPtr<IDXGISwapChain4> g_swapChain;				// ��ʂɕ`�悷��o�b�t�@���Ǘ�
ComPtr<ID3D12DescriptorHeap> g_rtvHeap;				// �����_�[�^�[�Q�b�g�r���[�̊Ǘ�
ComPtr<ID3D12Resource> g_renderTargets[2];			// �_�u���o�b�t�@�p
ComPtr<ID3D12CommandAllocator> g_commandAllocator;	// �R�}���h���X�g�̃������Ǘ�
ComPtr<ID3D12GraphicsCommandList> g_commandList;	// GPU�ɑ���`��R�}���h���L�^
ComPtr<ID3D12Fence> g_fence;						// GPU�̏������I��������Ƃ��m�F���邽�߂̓����I�u�W�F�N�g
UINT64 g_fenceValue = 0;

// DirectX�̏������֐�
bool InitializeDirectX12(HWND hwnd)
{
	HRESULT hr = S_OK;

	// DXGI�t�@�N�g���̍쐬
	// GPU�ɃA�N�Z�X�ł���悤�ɂ���
	ComPtr<IDXGIFactory4> factory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		std::cout << "Error at step Factory:" << std::hex << hr << std::endl;
		return false;
	}


	// �n�[�h�E�F�A�f�o�C�X�̍쐬
	hr = D3D12CreateDevice
	(
		nullptr,				// �����I��GPU��I��
		D3D_FEATURE_LEVEL_11_0,	// �K�v��DirectX�̋@�\���x��
		IID_PPV_ARGS(&g_device)	// �쐬����f�o�C�X�̃|�C���^
	);
	if (FAILED(hr))
	{
		std::cout << "Error at step Device:" << std::hex << hr << std::endl;
		return false;
	}


	// �R�}���h�L���[�̍쐬
	// GPU�ɑ���R�}���h���Ǘ�����
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// �ʏ�̕`��R�}���h

	hr = g_device->CreateCommandQueue		// GPU�ɃR�}���h�𑗂�L���[���쐬
	(
		&queueDesc, 
		IID_PPV_ARGS(&g_commandQueue)
	);
	if (FAILED(hr))
	{
		std::cout << "Error at step CommandQueue:" << std::hex << hr << std::endl;
		return false;
	}


	// �X���b�v�`�F�[���̍쐬
	// �t���[���o�b�t�@��\�����邽�߂̃o�b�t�@���Ǘ�����
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;	// �_�u���o�b�t�@
	swapChainDesc.Width = WINDOW_WIDTH;
	swapChainDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	// �E�B���h�E�n���h���Ƀo�b�t�@���֘A�t����
	ComPtr<IDXGISwapChain1> swapChain;
	hr = factory->CreateSwapChainForHwnd
	(
		g_commandQueue.Get(),hwnd,
		&swapChainDesc,nullptr,nullptr,
		&swapChain
	);
	if (FAILED(hr))
	{
		std::cout << "Error at step Hwnd:" << std::hex << hr << std::endl;
		return false;
	}

	hr = swapChain.As(&g_swapChain);
	if (FAILED(hr))
	{
		std::cout << "Error at step SwapChain:" << std::hex << hr << std::endl;
		return false;
	}


	// �����_�[�^�[�Q�b�g�r���[�̍쐬
	// �`�挋�ʂ�\������
	// RTV�p�̃q�[�v���쐬
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2;	// �`��o�b�t�@�̐�
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hr = g_device->CreateDescriptorHeap
	(
		&rtvHeapDesc,
		IID_PPV_ARGS(&g_rtvHeap)
	);
	if (FAILED(hr))
	{
		std::cout << "Error at step Heap:" << std::hex << hr << std::endl;
		return false;
	}

	// RTV�n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		g_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	
	// �o�b�N�o�b�t�@���擾����RTV���쐬
	for (UINT i = 0; i < 2; i++)
	{
		hr = g_swapChain->GetBuffer(i, IID_PPV_ARGS(&g_renderTargets[i]));
		if (FAILED(hr))
		{
			std::cout << "Error at step Buffer:" << std::hex << hr << std::endl;
			return false;
		}

		// �����_�[�^�[�Q�b�g��ݒ�
		g_device->CreateRenderTargetView(g_renderTargets[i].Get(), 
			nullptr, rtvHandle);

		rtvHandle.Offset(1, 
			g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}


	// �R�}���h�A���P�[�^�i�R�}���h���i�[����ꏊ�j��
	// �R�}���h���X�g�iGPU�ɑ���R�}���h���L�^���郊�X�g�j�̍쐬
	hr = g_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&g_commandAllocator));
	if (FAILED(hr))
	{
		std::cout << "Error at step Allocator:" << std::hex << hr << std::endl;
		return false;
	}

	hr = g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		g_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&g_commandList));
	if (FAILED(hr))
	{
		std::cout << "Error at step CommandList:" << std::hex << hr << std::endl;
		return false;
	}


	// �t�F���X�̍쐬
	// GPU�̃R�}���h���������������m�F����
	// CPU��GPU�̓����Ɏg�p
	hr = g_device->CreateFence(g_fenceValue, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&g_fence));
	if (FAILED(hr))
	{
		std::cout << "Error at step Fence:" << std::hex << hr << std::endl;
		return false;
	}


	return true;
}


// WinMain�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
#if _DEBUG
	// �R���\�[���E�B���h�E�̍쐬
	AllocConsole();
	// �W���o�͂��R���\�[���Ƀ��_�C���N�g
	FILE* pFile;
	freopen_s(&pFile, "CON", "w", stdout);
#endif


	// �E�B���h�E�̍쐬
	if (!CreateMainWindow(hInstance, nCmdShow))
	{
		return -1;
	}

	// DirectX�̏�����
	if (!InitializeDirectX12(hwnd))
	{
		MessageBox(hwnd, L"DirectX�̏������Ɏ��s���܂���", L"�G���[", MB_OK);
		return -1;
	}

	// ���C�����[�v
	MSG msg = {};
	while (true)
	{
		/*
		���b�Z�[�W�����o���ď�������
		���b�Z�[�W�����݂���ꍇ��true
		*/
		if (PeekMessage
		(
			&msg,		// �󂯎�������b�Z�[�W���i�[
			nullptr,	// �E�B���h�E�n���h�����w��inullptr�Ȃ̂ŃA�v���P�[�V�����S�́j
			0, 0,		// ���b�Z�[�W�͈̔́i0�Ȃ̂őS�Ẵ��b�Z�[�W�j
			PM_REMOVE	// ���b�Z�[�W���L���[����폜���Ď��o��
		))
		{
			if (msg.message == WM_QUIT) break;	// WM_QUIT��������I��
			// �L�[�{�[�h���b�Z�[�W��ϊ�����
			TranslateMessage(&msg);
			// ���b�Z�[�W���E�B���h�E�v���V�[�W���ɑ��M����
			DispatchMessage(&msg);
		}
		// ���b�Z�[�W���Ȃ��ꍇ�A
		// �A�v���P�[�V�����̃��C�����[�v
		else
		{
		}
	}


	return 0;
}