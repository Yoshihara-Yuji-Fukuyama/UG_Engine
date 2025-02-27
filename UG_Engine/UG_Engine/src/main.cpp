#include <Windows.h>		// Windows API���g�p���邽�߂̊�{�w�b�_�[
#include <dxgi1_6.h>		// DirectX Graphics Infrastructure�iDXGI�j�w�b�_�[
#include <d3dcompiler.h>	// �V�F�[�_�[���R���p�C�����邽�߂̃w�b�_�[
#include <wrl.h>			// Microsoft C++/WinRT�Ŏg�p�����w�b�_�[�i�X�}�[�g�|�C���^ComPtr���g�p���邽��)
#include "d3dx12.h"
#include <iostream>
#include "CApplication.h"

//using namespace Microsoft::WRL;	// Microsoft C++/WinRT
//
//ComPtr<ID3D12Device> gDevice;						// GPU�f�o�C�X�̊Ǘ�
//ComPtr<ID3D12CommandQueue> gCommandQueue;			// 
//ComPtr<IDXGISwapChain4> gSwapChain;					// 
//ComPtr<ID3D12DescriptorHeap> gRtvHeap;				// 
//ComPtr<ID3D12Resource> gRenderTargets[2];			// �_�u���o�b�t�@�p
//ComPtr<ID3D12CommandAllocator> gCommandAllocator;	// 
//ComPtr<ID3D12GraphicsCommandList> gCommandList;		// 
//ComPtr<ID3D12Fence> gFence;							// 
//UINT64 gFenceValue = 0;
//
//
//// DirectX�̏������֐�
//bool InitializeDirectX12(HWND gHWnd)
//{
//	HRESULT hr = S_OK;
//
//	// DXGI�t�@�N�g���̍쐬
//	// GPU�ɃA�N�Z�X�ł���悤�ɂ���
//	ComPtr<IDXGIFactory4> factory;
//	hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
//	if (FAILED(hr))
//	{
//		std::cout << "DXGI�t�@�N�g���̍쐬���s" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// �n�[�h�E�F�A�f�o�C�X�̍쐬
//	hr = D3D12CreateDevice
//	(
//		nullptr,				// �����I��GPU��I��
//		D3D_FEATURE_LEVEL_11_0,	// �K�v��DirectX�̋@�\���x��
//		IID_PPV_ARGS(&gDevice)	// �쐬����f�o�C�X�̃|�C���^
//	);
//	if (FAILED(hr))
//	{
//		std::cout << "�n�[�h�E�F�A�f�o�C�X�̍쐬���s" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// �R�}���h�L���[�̍쐬
//	// GPU�ɑ���R�}���h���Ǘ�����
//	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
//	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// �ʏ�̕`��R�}���h
//
//	hr = gDevice->CreateCommandQueue		// GPU�ɃR�}���h�𑗂�L���[���쐬
//	(
//		&queueDesc, 
//		IID_PPV_ARGS(&gCommandQueue)
//	);
//	if (FAILED(hr))
//	{
//		std::cout << "�R�}���h�L���[�̍쐬���s" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// �X���b�v�`�F�[���̍쐬
//	// �t���[���o�b�t�@��\�����邽�߂̃o�b�t�@���Ǘ�����
//	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
//	swapChainDesc.BufferCount = 2;	// �_�u���o�b�t�@
//	swapChainDesc.Width = WINDOW_WIDTH;
//	swapChainDesc.Height = WINDOW_HEIGHT;
//	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//	swapChainDesc.SampleDesc.Count = 1;
//
//	// �E�B���h�E�n���h���Ƀo�b�t�@���֘A�t����
//	ComPtr<IDXGISwapChain1> swapChain;
//	hr = factory->CreateSwapChainForHwnd
//	(
//		gCommandQueue.Get(),gHWnd,
//		&swapChainDesc,nullptr,nullptr,
//		&swapChain
//	);
//	if (FAILED(hr))
//	{
//		std::cout << "�E�B���h�E�n���h���Ɋ֘A�t�����s:" << std::hex << hr << std::endl;
//		return false;
//	}
//
//	hr = swapChain.As(&gSwapChain);
//	if (FAILED(hr))
//	{
//		std::cout << "�X���b�v�`�F�[���̍쐬���s" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// �����_�[�^�[�Q�b�g�r���[�̍쐬
//	// �`�挋�ʂ�\������
//	// RTV�p�̃q�[�v���쐬
//	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
//	rtvHeapDesc.NumDescriptors = 2;	// �`��o�b�t�@�̐�
//	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//
//	hr = gDevice->CreateDescriptorHeap
//	(
//		&rtvHeapDesc,
//		IID_PPV_ARGS(&gRtvHeap)
//	);
//	if (FAILED(hr))
//	{
//		std::cout << "�q�[�v�̍쐬���s" << std::hex << hr << std::endl;
//		return false;
//	}
//
//	// RTV�n���h�����擾
//	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
//		gRtvHeap->GetCPUDescriptorHandleForHeapStart());
//	
//	// �o�b�N�o�b�t�@���擾����RTV���쐬
//	for (UINT i = 0; i < 2; i++)
//	{
//		hr = gSwapChain->GetBuffer(i, IID_PPV_ARGS(&gRenderTargets[i]));
//		if (FAILED(hr))
//		{
//			std::cout << "�o�b�t�@�̎擾���s" << std::hex << hr << std::endl;
//			return false;
//		}
//
//		// �����_�[�^�[�Q�b�g��ݒ�
//		gDevice->CreateRenderTargetView(gRenderTargets[i].Get(), 
//			nullptr, rtvHandle);
//
//		rtvHandle.Offset(1, 
//			gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
//	}
//
//
//	// �R�}���h�A���P�[�^�i�R�}���h���i�[����ꏊ�j��
//	// �R�}���h���X�g�iGPU�ɑ���R�}���h���L�^���郊�X�g�j�̍쐬
//	hr = gDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
//		IID_PPV_ARGS(&gCommandAllocator));
//	if (FAILED(hr))
//	{
//		std::cout << "�R�}���h�A���P�[�^�̍쐬���s" << std::hex << hr << std::endl;
//		return false;
//	}
//
//	hr = gDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
//		gCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&gCommandList));
//	if (FAILED(hr))
//	{
//		std::cout << "�R�}���h���X�g�̍쐬���s" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	// �t�F���X�̍쐬
//	// GPU�̃R�}���h���������������m�F����
//	// CPU��GPU�̓����Ɏg�p
//	hr = gDevice->CreateFence(gFenceValue, D3D12_FENCE_FLAG_NONE,
//		IID_PPV_ARGS(&gFence));
//	if (FAILED(hr))
//	{
//		std::cout << "�t�F���X�̍쐬���s" << std::hex << hr << std::endl;
//		return false;
//	}
//
//
//	return true;
//}
//
//
//// �`�揈��
//void RenderFrame()
//{
//	HRESULT hr;
//
//	// �t�F���X�̑ҋ@�iGPU���I������̂�҂j
//	if (gFence->GetCompletedValue() < gFenceValue)
//	{
//		hr = gFence->SetEventOnCompletion(gFenceValue, nullptr);
//		if (FAILED(hr))
//		{
//			std::cout << "�t�F���X�̃C�x���g�ݒ莸�s" << std::hex << hr << std::endl;
//		}
//
//		// �C�x���g����������̂�ҋ@
//		WaitForSingleObject(nullptr, INFINITE);
//	}
//
//
//	// �R�}���h�A���P�[�^�����Z�b�g
//	hr = gCommandAllocator->Reset();
//	if (FAILED(hr))
//	{
//		std::cout << "�R�}���h�A���P�[�^�̃��Z�b�g���s" << std::hex << hr << std::endl;
//		return;
//	}
//
//
//	// �R�}���h���X�g�̃��Z�b�g
//	hr = gCommandList->Reset(gCommandAllocator.Get(), nullptr);
//	if (FAILED(hr))
//	{
//		std::cout << "�R�}���h���X�g�̃��Z�b�g���s" << std::hex << hr << std::endl;
//		return;
//	}
//
//
//	// �����_�[�^�[�Q�b�g��ݒ�
//	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition
//	(
//		gRenderTargets[0].Get(),
//		D3D12_RESOURCE_STATE_PRESENT,
//		D3D12_RESOURCE_STATE_RENDER_TARGET
//	);
//	gCommandList->ResourceBarrier(1, &barrier);
//
//
//	// �`��ΏۂƂȂ�RTV��ݒ�
//	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(gRtvHeap->GetCPUDescriptorHandleForHeapStart(),
//		0, gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
//	gCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
//
//
//	// �w�i�F���N���A
//	const FLOAT clearColor[] = { 0.0f,0.0f,0.0f,1.0f };	// ���F
//	gCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
//
//
//	// ���\�[�X�̏�Ԃ�"Present"�ɖ߂�
//	barrier = CD3DX12_RESOURCE_BARRIER::Transition
//	(
//		gRenderTargets[0].Get(),
//		D3D12_RESOURCE_STATE_RENDER_TARGET,
//		D3D12_RESOURCE_STATE_PRESENT
//	);
//	gCommandList->ResourceBarrier(1, &barrier);
//
//
//	// �R�}���h���X�g�̏I��
//	hr = gCommandList->Close();
//	if (FAILED(hr))
//	{
//		std::cout << "�R�}���h���X�g�̏I�����s" << std::hex << hr << std::endl;
//		return;
//	}
//
//
//	// �R�}���h���X�g��GPU�ɑ���
//	ID3D12CommandList* ppCommandLists[] = { gCommandList.Get() };
//	gCommandQueue->ExecuteCommandLists(1, ppCommandLists);
//
//
//	// GPU�̏������I���̂�҂�
//	gFenceValue++;
//	hr = gCommandQueue->Signal(gFence.Get(), gFenceValue);
//	if (FAILED(hr))
//	{
//		std::cout << "GPU�̃V�O�i�����M���s" << std::hex << hr << std::endl;
//		return;
//	}
//
//
//	// �X���b�v�`�F�[���̃o�b�t�@��؂�ւ�
//	gSwapChain->Present(1, 0);	// 1�t���[���ҋ@
//}


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

	//// �E�B���h�E�̍쐬
	//if (!CreateMainWindow(hInstance, nCmdShow))
	//{
	//	return -1;
	//}

	//// DirectX�̏�����
	//if (!InitializeDirectX12(hwnd))
	//{
	//	MessageBox(hwnd, L"DirectX�̏������Ɏ��s���܂���", L"�G���[", MB_OK);
	//	return -1;
	//}

	StartApp(TEXT(ENGINE_TITLE));

	return 0;
}