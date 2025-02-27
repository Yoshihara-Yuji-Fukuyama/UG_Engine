//#include "CTextureManager.h"
//#include "CTexture.h"
//#include "CTextureLoader.h"
//
//// �e�N�X�`���Ǘ��N���X�̃C���X�^���X
//CTextureManager* CTextureManager::spInstance = nullptr;
//
//// �C���X�^���X���擾
//CTextureManager* CTextureManager::Instance()
//{
//	if (spInstance == nullptr)
//	{
//		spInstance = new CTextureManager();
//	}
//	return spInstance;
//}
//
//// �R���X�g���N�^
//CTextureManager::CTextureManager()
//	: mpDevice(nullptr)
//{
//}
//
//// �f�X�g���N�^
//CTextureManager::~CTextureManager()
//{
//}
//
//// �e�N�X�`�����擾
//CTexture CTextureManager::GetTexture(const std::wstring& filePath)
//{
//	// ���Ƀt�@�C����ǂݍ���ł�����
//	if (mTextureCache.find(filePath) != mTextureCache.end())
//	{
//		// �ǂݍ��ݍς݂̃e�N�X�`����Ԃ�
//		return mTextureCache[filePath];
//	}
//
//	// �ǂݍ���ł��Ȃ��̂œǂݍ���
//	CTexture texture = CTextureLoader::LoadTextureFromFile(mpDevice, filePath);
//	// �e�N�X�`���𐶐��o������
//	if (texture.GetTexture())
//	{
//		// �L���b�V���ɕۑ�
//		mTextureCache[filePath] = texture;
//	}
//
//	return texture;
//}
