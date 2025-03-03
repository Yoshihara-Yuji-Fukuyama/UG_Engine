#include "CConstantBuffer.h"

CConstantBuffer::CConstantBuffer(size_t size)
{
}

bool CConstantBuffer::IsCreated()
{
	return false;
}

D3D12_GPU_VIRTUAL_ADDRESS CConstantBuffer::GetAddress() const
{
	return D3D12_GPU_VIRTUAL_ADDRESS();
}

D3D12_CONSTANT_BUFFER_VIEW_DESC CConstantBuffer::ViewDesc()
{
	return D3D12_CONSTANT_BUFFER_VIEW_DESC();
}

void* CConstantBuffer::GetPtr() const
{
	return nullptr;
}
