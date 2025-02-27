//#include "CTextureManager.h"
//#include "CTexture.h"
//#include "CTextureLoader.h"
//
//// テクスチャ管理クラスのインスタンス
//CTextureManager* CTextureManager::spInstance = nullptr;
//
//// インスタンスを取得
//CTextureManager* CTextureManager::Instance()
//{
//	if (spInstance == nullptr)
//	{
//		spInstance = new CTextureManager();
//	}
//	return spInstance;
//}
//
//// コンストラクタ
//CTextureManager::CTextureManager()
//	: mpDevice(nullptr)
//{
//}
//
//// デストラクタ
//CTextureManager::~CTextureManager()
//{
//}
//
//// テクスチャを取得
//CTexture CTextureManager::GetTexture(const std::wstring& filePath)
//{
//	// 既にファイルを読み込んでいたら
//	if (mTextureCache.find(filePath) != mTextureCache.end())
//	{
//		// 読み込み済みのテクスチャを返す
//		return mTextureCache[filePath];
//	}
//
//	// 読み込んでいないので読み込む
//	CTexture texture = CTextureLoader::LoadTextureFromFile(mpDevice, filePath);
//	// テクスチャを生成出来たら
//	if (texture.GetTexture())
//	{
//		// キャッシュに保存
//		mTextureCache[filePath] = texture;
//	}
//
//	return texture;
//}
