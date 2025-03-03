#pragma once

// 3Dモデルの描画クラス
class CEngine3D
{
public:
	// 初期化
	bool Init();

	// 更新
	void Update();
	// 描画
	void Render();
};

extern CEngine3D* gEngine3D;