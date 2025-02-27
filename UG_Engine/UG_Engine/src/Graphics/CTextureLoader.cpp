//#include "CTextureLoader.h"
//#include "CTexture.h"
//
//// �e�N�X�`�����t�@�C������ǂݍ���
//CTexture CTextureLoader::LoadTextureFromFile(ID3D12Device* device,
//	const std::wstring& filePath)
//{
//	CTexture texture;
//
//	// �摜��ǂݍ���
//	DirectX::ScratchImage image;
//	HRESULT hr = DirectX::LoadFromWICFile(filePath.c_str(),
//		DirectX::WIC_FLAGS_NONE, nullptr, image);
//	if (FAILED(hr))
//	{
//		std::wcout << filePath << "�F�摜�ǂݍ��ݎ��s" << std::hex << hr << std::endl;
//		return texture;
//	}
//
//	// �e�N�X�`�����쐬
//	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;
//	hr = DirectX::CreateTexture(device, image.GetMetadata(), &textureResource);
//	if (FAILED(hr))
//	{
//		std::wcout << filePath << "�F�e�N�X�`���쐬���s" << std::hex << hr << std::endl;
//		return texture;
//	}
//
//	// GPU�ɃA�b�v���[�h
//	DirectX::ScratchImage mipChain;
//	DirectX::GenerateMipMaps(*image.GetImages(), DirectX::TEX_FILTER_DEFAULT,
//		0, mipChain);
//
//	// �e�N�X�`����ݒ�
//	texture.SetTexture(textureResource.Get());
//
//	return texture;
//}
