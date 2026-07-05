/*================================================================
    共通ヘッダー [main.h]
                                            Author :
                                            Date   :
    --------------------------------------------------------------
================================================================*/

#pragma once
#pragma warning(push)
#pragma warning(disable:4005)

#define NOMINMAX                      // Windows.h の min/max マクロを無効化
#define _CRT_SECURE_NO_WARNINGS         // scanf のwarning防止

#include <stdio.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800      // 警告対処

#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)

#include <DirectXMath.h>


//*********************************************************************
// 定数定義
//*********************************************************************

inline constexpr int SCREEN_WIDTH = 1920; // ウィンドウの幅
inline constexpr int SCREEN_HEIGHT = 1080; // ウィンドウの高さ

inline float screen_split_width(int split, int num = 1)
{
    return static_cast<float>(SCREEN_WIDTH) * static_cast<float>(num) / static_cast<float>(split);
}

inline float screen_split_height(int split, int num = 1)
{
    return static_cast<float>(SCREEN_HEIGHT) * static_cast<float>(num) / static_cast<float>(split);
}

//*********************************************************************
// プロトタイプ宣言
//*********************************************************************
