//#pragma once
//#include <d3d12.h>
//#include <string>
//#include <unordered_map>
//
//class CTexture;
//
//// �e�N�X�`���Ǘ��N���X
//class CTextureManager
//{
//public:
//	// �C���X�^���X���擾
//	static CTextureManager* Instance();
//
//	// �R���X�g���N�^
//	CTextureManager();
//	// �f�X�g���N�^
//	~CTextureManager();
//
//	// �e�N�X�`�����擾
//	CTexture GetTexture(const std::wstring& filePath);
//private:
//	// �C���X�^���X
//	static CTextureManager* spInstance;
//
//	// �f�o�C�X�̃|�C���^
//	ID3D12Device* mpDevice;
//
//	// �e�N�X�`���̃L���b�V��
//	// �t�@�C���p�X�őΉ�����e�N�X�`���f�[�^�����o����
//	std::unordered_map<std::wstring, CTexture> mTextureCache;
//};