#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>

// ���_�f�[�^
struct Vertex
{
	DirectX::XMFLOAT3 Position;	// �ʒu���W
	DirectX::XMFLOAT3 Normal;	// �@��
	DirectX::XMFLOAT2 UV;		// UV���W
	DirectX::XMFLOAT3 Tangent;	// �ڋ��
	DirectX::XMFLOAT4 Color;	// ���_�F
};

struct alignas(256) Transform
{
	DirectX::XMMATRIX World;	// ���[���h�s��
	DirectX::XMMATRIX View;		// �r���[�s��
	DirectX::XMMATRIX Proj;		// ���e�s��
};