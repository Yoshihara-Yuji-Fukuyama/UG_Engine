#pragma once

// 3D���f���̕`��N���X
class CEngine3D
{
public:
	// ������
	bool Init();

	// �X�V
	void Update();
	// �`��
	void Render();
};

extern CEngine3D* gEngine3D;