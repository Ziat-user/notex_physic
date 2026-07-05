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
using namespace DirectX;


//*********************************************************************
// マクロ定義
//*********************************************************************
#define SCREEN_WIDTH    (1920)          // ウィンドウの幅
#define SCREEN_HEIGHT   (1080)          // ウィンドウの高さ


//*********************************************************************
// プロトタイプ宣言
//*********************************************************************