#include "CEngine3D.h"
#include "CEngine.h"
#include "Application.h"
#include <d3dx12.h>
#include <iostream>
#include "CSharedStruct.h"
#include "CVertexBuffer.h"

CEngine3D* gEngine3D;

using namespace DirectX;

CVertexBuffer* vertexBuffer;

// ������
bool CEngine3D::Init()
{
	Vertex vertices[3] = {};
	vertices[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].Color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[1].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[2].Color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	auto vertexSize = sizeof(Vertex) * std::size(vertices);
	auto vertexStride = sizeof(Vertex);
	vertexBuffer = new CVertexBuffer(vertexSize, vertexStride, vertices);
	if (!vertexBuffer->IsCreated())
	{
		std::cout << "���_�o�b�t�@�̐������s" << std::endl;
		return false;
	}

	std::cout << "�`��G���W��3D�̏���������" << std::endl;
	return true;
}

// �X�V
void CEngine3D::Update()
{
}

// �`��
void CEngine3D::Render()
{
}
