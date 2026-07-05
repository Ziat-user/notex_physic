/*=================================================================
    File Name: sprite.cpp
===================================================================*/
#include "sprite.h"
using namespace DirectX; //  using はヘッダーでなく .cpp で宣言
static ID3D11Buffer* g_pVertexBuffer = nullptr;
void InitSprite()
{
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //  HRESULT チェックを追加
    HRESULT hr = GetDevice()->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
    if (FAILED(hr))
    {
        MessageBoxA(nullptr, "頂点バッファの作成に失敗しました。", "Error", MB_OK | MB_ICONERROR);
    }
}
void UninitSprite()
{
    if (g_pVertexBuffer) { g_pVertexBuffer->Release(); g_pVertexBuffer = nullptr; }
}
void UpdateSprite() {}
void DrawSprite(float x, float y, float width, float height)
{
    D3D11_MAPPED_SUBRESOURCE msr{};
    GetDeviceContext()->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    VERTEX_3D* v = static_cast<VERTEX_3D*>(msr.pData);
    v[0].Position = { x,         y,          0.0f }; v[0].TexCoord = { 0.0f, 0.0f };
    v[1].Position = { x + width, y,          0.0f }; v[1].TexCoord = { 1.0f, 0.0f };
    v[2].Position = { x,         y + height, 0.0f }; v[2].TexCoord = { 0.0f, 1.0f };
    v[3].Position = { x + width, y + height, 0.0f }; v[3].TexCoord = { 1.0f, 1.0f };
    //  Normal / Diffuse を明示的に初期化 (WRITE_DISCARD でゴミ値になるため)
    for (int i = 0; i < 4; i++) {
        v[i].Normal = { 0.0f, 0.0f, -1.0f };
        v[i].Diffuse = { 1.0f, 1.0f,  1.0f, 1.0f };
    }
    GetDeviceContext()->Unmap(g_pVertexBuffer, 0);
    SetWorldViewProjection2D();
    UINT stride = sizeof(VERTEX_3D), offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    GetDeviceContext()->Draw(4, 0);
}
void DrawSpriteRot(float x, float y, float width, float height, float rot)
{
    D3D11_MAPPED_SUBRESOURCE msr{};
    GetDeviceContext()->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    VERTEX_3D* v = static_cast<VERTEX_3D*>(msr.pData);
    v[0].Position = { -width * 0.5f, -height * 0.5f, 0.0f }; v[0].TexCoord = { 0.0f, 0.0f };
    v[1].Position = { width * 0.5f, -height * 0.5f, 0.0f }; v[1].TexCoord = { 1.0f, 0.0f };
    v[2].Position = { -width * 0.5f,  height * 0.5f, 0.0f }; v[2].TexCoord = { 0.0f, 1.0f };
    v[3].Position = { width * 0.5f,  height * 0.5f, 0.0f }; v[3].TexCoord = { 1.0f, 1.0f };
    //  Normal / Diffuse を明示的に初期化
    for (int i = 0; i < 4; i++) {
        v[i].Normal = { 0.0f, 0.0f, -1.0f };
        v[i].Diffuse = { 1.0f, 1.0f,  1.0f, 1.0f };
    }
    GetDeviceContext()->Unmap(g_pVertexBuffer, 0);
    SetWorldViewProjection2D();
    XMMATRIX rotMat = XMMatrixRotationZ(rot);
    XMMATRIX transMat = XMMatrixTranslation(x, y, 0.0f);
    SetWorldMatrix(rotMat * transMat);
    UINT stride = sizeof(VERTEX_3D), offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    GetDeviceContext()->Draw(4, 0);
}
void DrawLine(float x1, float y1, float x2, float y2, float bold)
{
    float dx = x2 - x1, dy = y2 - y1, len = sqrtf(dx * dx + dy * dy);
    if (len <= 0.0f) return;
    dx /= len; dy /= len;
    float nx = -dy * (bold * 0.5f), ny = dx * (bold * 0.5f);
    D3D11_MAPPED_SUBRESOURCE msr{};
    GetDeviceContext()->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    VERTEX_3D* v = static_cast<VERTEX_3D*>(msr.pData);
    v[0].Position = { x1 - nx, y1 - ny, 0.0f };
    v[1].Position = { x1 + nx, y1 + ny, 0.0f };
    v[2].Position = { x2 - nx, y2 - ny, 0.0f };
    v[3].Position = { x2 + nx, y2 + ny, 0.0f };
    //  TexCoord / Normal / Diffuse を明示的に初期化
    v[0].TexCoord = { 0.0f, 0.0f };
    v[1].TexCoord = { 1.0f, 0.0f };
    v[2].TexCoord = { 0.0f, 1.0f };
    v[3].TexCoord = { 1.0f, 1.0f };
    for (int i = 0; i < 4; i++) {
        v[i].Normal = { 0.0f, 0.0f, -1.0f };
        v[i].Diffuse = { 1.0f, 1.0f,  1.0f, 1.0f };
    }
    GetDeviceContext()->Unmap(g_pVertexBuffer, 0);
    SetWorldViewProjection2D();
    UINT stride = sizeof(VERTEX_3D), offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    GetDeviceContext()->Draw(4, 0);
}
// 修正: TexCoord を正しく設定
void DrawSpriteColor(float x, float y, float width, float height, Color4 color)
{
    D3D11_MAPPED_SUBRESOURCE msr{};
    GetDeviceContext()->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    VERTEX_3D* v = static_cast<VERTEX_3D*>(msr.pData);
    v[0].Position = { x,         y,          0.0f };
    v[1].Position = { x + width, y,          0.0f };
    v[2].Position = { x,         y + height, 0.0f };
    v[3].Position = { x + width, y + height, 0.0f };
    v[0].TexCoord = { 0.0f, 0.0f };
    v[1].TexCoord = { 1.0f, 0.0f };
    v[2].TexCoord = { 0.0f, 1.0f };
    v[3].TexCoord = { 1.0f, 1.0f };
    for (int i = 0; i < 4; i++) {
        v[i].Normal = { 0.0f, 0.0f, -1.0f };
        v[i].Diffuse = { color.r, color.g, color.b, color.a };
    }
    GetDeviceContext()->Unmap(g_pVertexBuffer, 0);
    SetWorldViewProjection2D();
    UINT stride = sizeof(VERTEX_3D), offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    GetDeviceContext()->Draw(4, 0);
}
// SDF 円描画  cx,cy = 円の中心座標  radius = 半径
void DrawCircleColor(float cx, float cy, float radius, Color4 color)
{
    float x = cx - radius;
    float y = cy - radius;
    float size = radius * 2.0f;
    D3D11_MAPPED_SUBRESOURCE msr{};
    GetDeviceContext()->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    VERTEX_3D* v = static_cast<VERTEX_3D*>(msr.pData);
    v[0].Position = { x,        y,        0.0f };
    v[1].Position = { x + size, y,        0.0f };
    v[2].Position = { x,        y + size, 0.0f };
    v[3].Position = { x + size, y + size, 0.0f };
    v[0].TexCoord = { 0.0f, 0.0f };
    v[1].TexCoord = { 1.0f, 0.0f };
    v[2].TexCoord = { 0.0f, 1.0f };
    v[3].TexCoord = { 1.0f, 1.0f };
    for (int i = 0; i < 4; i++) {
        v[i].Normal = { 0.0f, 0.0f, -1.0f };
        v[i].Diffuse = { color.r, color.g, color.b, color.a };
    }
    GetDeviceContext()->Unmap(g_pVertexBuffer, 0);
    SetWorldViewProjection2D();
    UINT stride = sizeof(VERTEX_3D), offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    SetDrawMode(1);          // 円 SDF モードに切替
    GetDeviceContext()->Draw(4, 0);
    SetDrawMode(0);          // 矩形モードに戻す
}
