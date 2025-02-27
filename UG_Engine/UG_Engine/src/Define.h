#pragma once
#include <wrl/client.h>

#define ENGINE_TITLE "UG_Engine"	// �G���W����

#define WINDOW_WIDTH	1280	// �E�B���h�E�̕�
#define WINDOW_HEIGHT	 720	// �E�B���h�E�̍���

// ���\�[�X�t�@�C���t�H���_
#define RES_DIR "res\\"
// �̈�J�����}�N����
#define SAFE_DELETE(del) {if(del != nullptr) delete del; del = nullptr}
// ���I�z��̗̈�J�����}�N����
#define SAFE_DELETE_ARRAY(ary) {if(ary != nullptr) delete[] ary; ary = nullptr}

// �X�}�[�g�|�C���^
template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;