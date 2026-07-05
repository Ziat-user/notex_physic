/*==============================================================================
   レンダリング管理 [renderer.cpp]
==============================================================================*/
#include "renderer.h"

using namespace DirectX;

D3D_FEATURE_LEVEL            g_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*                g_D3DDevice = nullptr;
ID3D11DeviceContext*         g_ImmediateContext = nullptr;
IDXGISwapChain* g_SwapChain = nullptr;
ID3D11RenderTargetView* g_RenderTargetView = nullptr;
ID3D11DepthStencilView* g_DepthStencilView = nullptr;
ID3D11VertexShader* g_VertexShader = nullptr;
ID3D11PixelShader* g_PixelShader = nullptr;
ID3D11InputLayout* g_VertexLayout = nullptr;
ID3D11Buffer* g_ConstantBuffer = nullptr;
ID3D11Buffer* g_MaterialBuffer = nullptr;
ID3D11Buffer* g_DrawModeBuffer = nullptr;
XMMATRIX                 g_WorldMatrix;
XMMATRIX                 g_ViewMatrix;
XMMATRIX                 g_ProjectionMatrix;
ID3D11DepthStencilState* g_DepthStateEnable = nullptr;
ID3D11DepthStencilState* g_DepthStateDisable = nullptr;

ID3D11Device* GetDevice(void) { return g_D3DDevice; }
ID3D11DeviceContext* GetDeviceContext(void) { return g_ImmediateContext; }

void SetDepthEnable(bool Enable)
{
    if (Enable) g_ImmediateContext->OMSetDepthStencilState(g_DepthStateEnable, NULL);
    else        g_ImmediateContext->OMSetDepthStencilState(g_DepthStateDisable, NULL);
}
void SetWorldViewProjection2D(void)
{
    g_WorldMatrix = XMMatrixIdentity();
    g_ViewMatrix = XMMatrixIdentity();
    g_ProjectionMatrix = XMMatrixOrthographicOffCenterLH(
        0.0f, (FLOAT)SCREEN_WIDTH, (FLOAT)SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
    XMMATRIX wvp = XMMatrixTranspose(g_WorldMatrix * g_ViewMatrix * g_ProjectionMatrix);
    XMFLOAT4X4 m; XMStoreFloat4x4(&m, wvp);
    GetDeviceContext()->UpdateSubresource(g_ConstantBuffer, 0, NULL, &m, 0, 0);
}
void SetWorldMatrix(XMMATRIX W)
{
    g_WorldMatrix = W;
    XMMATRIX wvp = XMMatrixTranspose(g_WorldMatrix * g_ViewMatrix * g_ProjectionMatrix);
    XMFLOAT4X4 m; XMStoreFloat4x4(&m, wvp);
    GetDeviceContext()->UpdateSubresource(g_ConstantBuffer, 0, NULL, &m, 0, 0);
}
void SetViewMatrix(XMMATRIX V)
{
    g_ViewMatrix = V;
    XMMATRIX wvp = XMMatrixTranspose(g_WorldMatrix * g_ViewMatrix * g_ProjectionMatrix);
    XMFLOAT4X4 m; XMStoreFloat4x4(&m, wvp);
    GetDeviceContext()->UpdateSubresource(g_ConstantBuffer, 0, NULL, &m, 0, 0);
}
void SetProjectionMatrix(XMMATRIX P)
{
    g_ProjectionMatrix = P;
    XMMATRIX wvp = XMMatrixTranspose(g_WorldMatrix * g_ViewMatrix * g_ProjectionMatrix);
    XMFLOAT4X4 m; XMStoreFloat4x4(&m, wvp);
    GetDeviceContext()->UpdateSubresource(g_ConstantBuffer, 0, NULL, &m, 0, 0);
}
void SetMaterial(MATERIAL Material)
{
    GetDeviceContext()->UpdateSubresource(g_MaterialBuffer, 0, NULL, &Material, 0, 0);
}
void SetDrawMode(int mode)
{
    DRAW_MODE_BUFFER buf = { mode, { 0.0f, 0.0f, 0.0f } };
    GetDeviceContext()->UpdateSubresource(g_DrawModeBuffer, 0, NULL, &buf, 0, 0);
}

HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    HRESULT hr = S_OK;

    // ── SwapChain & Device ──────────────────────────────────────────────────
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = SCREEN_WIDTH;
    sd.BufferDesc.Height = SCREEN_HEIGHT;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
        NULL, 0, D3D11_SDK_VERSION, &sd,
        &g_SwapChain, &g_D3DDevice, &g_FeatureLevel, &g_ImmediateContext);
    if (FAILED(hr)) return hr;

    // ── RenderTargetView ────────────────────────────────────────────────────
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) return hr;
    hr = g_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_RenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) return hr;

    // ── DepthStencil ────────────────────────────────────────────────────────
    ID3D11Texture2D* depthTexture = nullptr;
    D3D11_TEXTURE2D_DESC td = {};
    td.Width = sd.BufferDesc.Width;
    td.Height = sd.BufferDesc.Height;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    td.SampleDesc = sd.SampleDesc;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    hr = g_D3DDevice->CreateTexture2D(&td, NULL, &depthTexture);
    if (FAILED(hr)) return hr;

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
    dsvd.Format = td.Format;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    hr = g_D3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &g_DepthStencilView);
    //  DSV 作成後すぐに depthTexture を解放
    depthTexture->Release();
    depthTexture = nullptr;
    if (FAILED(hr)) return hr;

    g_ImmediateContext->OMSetRenderTargets(1, &g_RenderTargetView, g_DepthStencilView);

    // ── Viewport ───────────────────────────────────────────────────────────
    D3D11_VIEWPORT vp = {};
    vp.Width = (FLOAT)SCREEN_WIDTH;
    vp.Height = (FLOAT)SCREEN_HEIGHT;
    vp.MaxDepth = 1.0f;
    g_ImmediateContext->RSSetViewports(1, &vp);

    // ── RasterizerState ────────────────────────────────────────────────────
    D3D11_RASTERIZER_DESC rd = {};
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_NONE;
    rd.DepthClipEnable = TRUE;
    ID3D11RasterizerState* rs = nullptr;
    hr = g_D3DDevice->CreateRasterizerState(&rd, &rs);
    if (FAILED(hr)) return hr;
    g_ImmediateContext->RSSetState(rs);
    //  RSSetState 後すぐに解放
    rs->Release();
    rs = nullptr;

    // ── BlendState ─────────────────────────────────────────────────────────
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    float bf[4] = {};
    ID3D11BlendState* blendState = nullptr;
    hr = g_D3DDevice->CreateBlendState(&blendDesc, &blendState);
    if (FAILED(hr)) return hr;
    g_ImmediateContext->OMSetBlendState(blendState, bf, 0xffffffff);
    //  OMSetBlendState 後すぐに解放
    blendState->Release();
    blendState = nullptr;

    // ── DepthStencilState ──────────────────────────────────────────────────
    D3D11_DEPTH_STENCIL_DESC dsd = {};
    dsd.DepthEnable = TRUE;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsd.DepthFunc = D3D11_COMPARISON_LESS;
    hr = g_D3DDevice->CreateDepthStencilState(&dsd, &g_DepthStateEnable);
    if (FAILED(hr)) return hr;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    hr = g_D3DDevice->CreateDepthStencilState(&dsd, &g_DepthStateDisable);
    if (FAILED(hr)) return hr;
    g_ImmediateContext->OMSetDepthStencilState(g_DepthStateDisable, NULL);

    // ── SamplerState ───────────────────────────────────────────────────────
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    ID3D11SamplerState* samplerState = nullptr;
    hr = g_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);
    if (FAILED(hr)) return hr;
    g_ImmediateContext->PSSetSamplers(0, 1, &samplerState);
    //  PSSetSamplers 後すぐに解放
    samplerState->Release();
    samplerState = nullptr;

    // ── Vertex Shader ──────────────────────────────────────────────────────
    ID3DBlob* pErrorBlob = nullptr; //  nullptr で初期化
    ID3DBlob* pVSBlob = nullptr;

    hr = D3DCompileFromFile(L"shader.hlsl", NULL, NULL, "VertexShaderPolygon", "vs_4_0",
        D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrorBlob);
    if (FAILED(hr)) {
        if (pErrorBlob) {
            OutputDebugStringA("[Renderer] VS Compile Error: ");
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
            pErrorBlob->Release();
            pErrorBlob = nullptr;
        }
        else {
            OutputDebugStringA("[Renderer] VS Compile Error: shader.hlsl が見つからない、または読み込めません。");
        }
        return hr;
    }
    hr = g_D3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_VertexShader);
    if (FAILED(hr)) { pVSBlob->Release(); return hr; }

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    hr = g_D3DDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
        pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_VertexLayout);
    pVSBlob->Release();
    if (FAILED(hr)) return hr;

    // ── Pixel Shader ───────────────────────────────────────────────────────
    ID3DBlob* pPSBlob = nullptr;
    hr = D3DCompileFromFile(L"shader.hlsl", NULL, NULL, "PixelShaderPolygon", "ps_4_0",
        D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &pErrorBlob);
    if (FAILED(hr)) {
        //  pErrorBlob の null チェックを追加
        if (pErrorBlob) {
            OutputDebugStringA("[Renderer] PS Compile Error: ");
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
            pErrorBlob->Release();
            pErrorBlob = nullptr;
        }
        else {
            OutputDebugStringA("[Renderer] PS Compile Error: shader.hlsl が見つからない、または読み込めません。");
        }
        return hr;
    }
    hr = g_D3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_PixelShader);
    pPSBlob->Release();
    if (FAILED(hr)) return hr;

    // ── Constant Buffers ───────────────────────────────────────────────────
    D3D11_BUFFER_DESC hbd = {};
    hbd.Usage = D3D11_USAGE_DEFAULT;
    hbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hbd.StructureByteStride = sizeof(float);
    // b0: WVP (VS)
    hbd.ByteWidth = sizeof(XMMATRIX);
    hr = g_D3DDevice->CreateBuffer(&hbd, NULL, &g_ConstantBuffer);
    if (FAILED(hr)) return hr;
    g_ImmediateContext->VSSetConstantBuffers(0, 1, &g_ConstantBuffer);
    // b1: Material (VS)
    hbd.ByteWidth = sizeof(MATERIAL);
    hr = g_D3DDevice->CreateBuffer(&hbd, NULL, &g_MaterialBuffer);
    if (FAILED(hr)) return hr;
    g_ImmediateContext->VSSetConstantBuffers(1, 1, &g_MaterialBuffer);
    // b2: DrawMode (PS)
    DRAW_MODE_BUFFER initMode = { 0, { 0.0f, 0.0f, 0.0f } };
    hbd.ByteWidth = sizeof(DRAW_MODE_BUFFER);
    D3D11_SUBRESOURCE_DATA initData = {}; initData.pSysMem = &initMode;
    hr = g_D3DDevice->CreateBuffer(&hbd, &initData, &g_DrawModeBuffer);
    if (FAILED(hr)) return hr;
    g_ImmediateContext->PSSetConstantBuffers(2, 1, &g_DrawModeBuffer);

    g_ImmediateContext->IASetInputLayout(g_VertexLayout);
    g_ImmediateContext->VSSetShader(g_VertexShader, NULL, 0);
    g_ImmediateContext->PSSetShader(g_PixelShader, NULL, 0);
    return S_OK;
}

void UninitRenderer(void)
{
    if (g_DrawModeBuffer) { g_DrawModeBuffer->Release();   g_DrawModeBuffer = nullptr; }
    if (g_ConstantBuffer) { g_ConstantBuffer->Release();   g_ConstantBuffer = nullptr; }
    if (g_MaterialBuffer) { g_MaterialBuffer->Release();   g_MaterialBuffer = nullptr; }
    if (g_VertexLayout) { g_VertexLayout->Release();     g_VertexLayout = nullptr; }
    if (g_VertexShader) { g_VertexShader->Release();     g_VertexShader = nullptr; }
    if (g_PixelShader) { g_PixelShader->Release();      g_PixelShader = nullptr; }
    if (g_DepthStateEnable) { g_DepthStateEnable->Release();  g_DepthStateEnable = nullptr; }
    if (g_DepthStateDisable) { g_DepthStateDisable->Release(); g_DepthStateDisable = nullptr; }
    if (g_DepthStencilView) { g_DepthStencilView->Release();  g_DepthStencilView = nullptr; }
    if (g_ImmediateContext) { g_ImmediateContext->ClearState(); }
    if (g_RenderTargetView) { g_RenderTargetView->Release();  g_RenderTargetView = nullptr; }
    if (g_SwapChain) { g_SwapChain->Release();         g_SwapChain = nullptr; }
    if (g_ImmediateContext) { g_ImmediateContext->Release();  g_ImmediateContext = nullptr; }
    if (g_D3DDevice) { g_D3DDevice->Release();         g_D3DDevice = nullptr; }
}

void Clear(void)
{
    float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
    g_ImmediateContext->ClearRenderTargetView(g_RenderTargetView, ClearColor);
    g_ImmediateContext->ClearDepthStencilView(g_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Present(void) { g_SwapChain->Present(0, 0); }
