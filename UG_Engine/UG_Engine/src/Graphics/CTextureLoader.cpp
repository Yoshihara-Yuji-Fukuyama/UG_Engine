//#include "CTextureLoader.h"
//#include "CTexture.h"
//
//// テクスチャをファイルから読み込む
//CTexture CTextureLoader::LoadTextureFromFile(ID3D12Device* device,
//	const std::wstring& filePath)
//{
//	CTexture texture;
//
//	// 画像を読み込む
//	DirectX::ScratchImage image;
//	HRESULT hr = DirectX::LoadFromWICFile(filePath.c_str(),
//		DirectX::WIC_FLAGS_NONE, nullptr, image);
//	if (FAILED(hr))
//	{
//		std::wcout << filePath << "：画像読み込み失敗" << std::hex << hr << std::endl;
//		return texture;
//	}
//
//	// テクスチャを作成
//	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;
//	hr = DirectX::CreateTexture(device, image.GetMetadata(), &textureResource);
//	if (FAILED(hr))
//	{
//		std::wcout << filePath << "：テクスチャ作成失敗" << std::hex << hr << std::endl;
//		return texture;
//	}
//
//	// GPUにアップロード
//	DirectX::ScratchImage mipChain;
//	DirectX::GenerateMipMaps(*image.GetImages(), DirectX::TEX_FILTER_DEFAULT,
//		0, mipChain);
//
//	// テクスチャを設定
//	texture.SetTexture(textureResource.Get());
//
//	return texture;
//}
