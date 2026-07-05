/*==============================================================================
   レンダリング管理 [renderer.h]
==============================================================================*/
#pragma once
#include "../core/main.h"
// 頂点構造体

struct VERTEX_3D
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT4 Diffuse;
    DirectX::XMFLOAT2 TexCoord;
};

// マテリアル構造体
struct MATERIAL
{
    DirectX::XMFLOAT4 Ambient;
    DirectX::XMFLOAT4 Diffuse;
    DirectX::XMFLOAT4 Specular;
    DirectX::XMFLOAT4 Emission;
    float              Shininess;
    float              Dummy[3];
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
void SetWorldMatrix(DirectX::XMMATRIX WorldMatrix);
void SetViewMatrix(DirectX::XMMATRIX ViewMatrix);
void SetProjectionMatrix(DirectX::XMMATRIX ProjectionMatrix);
void SetMaterial(MATERIAL Material);
void SetDrawMode(int mode);
