//#pragma once
//#include <d3d12.h>
//#include <string>
//#include <unordered_map>
//
//class CTexture;
//
//// テクスチャ管理クラス
//class CTextureManager
//{
//public:
//	// インスタンスを取得
//	static CTextureManager* Instance();
//
//	// コンストラクタ
//	CTextureManager();
//	// デストラクタ
//	~CTextureManager();
//
//	// テクスチャを取得
//	CTexture GetTexture(const std::wstring& filePath);
//private:
//	// インスタンス
//	static CTextureManager* spInstance;
//
//	// デバイスのポインタ
//	ID3D12Device* mpDevice;
//
//	// テクスチャのキャッシュ
//	// ファイルパスで対応するテクスチャデータを取り出せる
//	std::unordered_map<std::wstring, CTexture> mTextureCache;
//};