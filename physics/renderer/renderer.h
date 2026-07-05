/*==============================================================================
   レンダリング管理 [renderer.h]
==============================================================================*/
#pragma once
#include "../core/main.h"

// 頂点構造体
struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};

// マテリアル構造体
struct MATERIAL
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Emission;
	float     Shininess;
	float     Dummy[3];
};

// 描画モードバッファ (b2, PS)  0=矩形  1=円SDF
struct DRAW_MODE_BUFFER
{
	int   DrawMode;
	float Padding[3];
};

HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void    UninitRenderer(void);
void    Clear(void);
void    Present(void);
ID3D11Device* GetDevice(void);
ID3D11DeviceContext* GetDeviceContext(void);
void SetDepthEnable(bool Enable);
void SetWorldViewProjection2D(void);
void SetWorldMatrix(XMMATRIX WorldMatrix);
void SetViewMatrix(XMMATRIX ViewMatrix);
void SetProjectionMatrix(XMMATRIX ProjectionMatrix);
void SetMaterial(MATERIAL Material);
void SetDrawMode(int mode); // 追加
