#pragma once
#include <wrl/client.h>

#define ENGINE_TITLE "UG_Engine"	// エンジン名

#define WINDOW_WIDTH	1280	// ウィンドウの幅
#define WINDOW_HEIGHT	 720	// ウィンドウの高さ

// リソースファイルフォルダ
#define RES_DIR "res\\"
// 領域開放をマクロ化
#define SAFE_DELETE(del) {if(del != nullptr) delete del; del = nullptr}
// 動的配列の領域開放をマクロ化
#define SAFE_DELETE_ARRAY(ary) {if(ary != nullptr) delete[] ary; ary = nullptr}

// スマートポインタ
template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;