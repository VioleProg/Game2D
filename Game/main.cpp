#include "main.h"
#include "Field/field.h" 
#include "Field/MapDefinitions.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <SpriteBatch.h> 
#include <memory>
#include "Settings/Resolution.h"
#include <CommonStates.h> 

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;
using namespace DirectX;

ComPtr<ID3D11Device>           g_device;
ComPtr<ID3D11DeviceContext>    g_context;
ComPtr<IDXGISwapChain>         g_swapChain;
ComPtr<ID3D11RenderTargetView> g_renderTarget;

std::unique_ptr<SpriteBatch>    g_spriteBatch;
std::unique_ptr<CommonStates>   g_states;
std::unique_ptr<Field>          g_field;

void OnResize(int width, int height) {
    if (!g_swapChain || !g_device || !g_context) return;

    float effectiveWidth, effectiveHeight;
    Resolution::GetEffectiveResolution((float)width, (float)height, effectiveWidth, effectiveHeight);

    g_context->OMSetRenderTargets(0, 0, 0);
    g_renderTarget.Reset();

    g_swapChain->ResizeBuffers(0, (UINT)effectiveWidth, (UINT)effectiveHeight, DXGI_FORMAT_UNKNOWN, 0);

    ComPtr<ID3D11Texture2D> pBackBuffer;
    if (FAILED(g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)))
        return;

    if (FAILED(g_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &g_renderTarget)))
        return;

    g_context->OMSetRenderTargets(1, g_renderTarget.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp = { 0.0f, 0.0f, effectiveWidth, effectiveHeight, 0.0f, 1.0f };
    g_context->RSSetViewports(1, &vp);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_SIZE:
        if (g_swapChain) 
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            OnResize(width, height);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void InitD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; 

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &scd, &g_swapChain, &g_device, nullptr, &g_context);

    RECT rc;
    GetClientRect(hWnd, &rc);
    OnResize(rc.right - rc.left, rc.bottom - rc.top);

    g_spriteBatch = std::make_unique<SpriteBatch>(g_context.Get());
    g_states = std::make_unique<CommonStates>(g_device.Get());
    g_field = std::make_unique<Field>();

    if (!g_field->Initialize(g_device.Get(), Maps::Village)) {
        MessageBox(hWnd, L"Erro ao carregar os Mapas", L"Erro", MB_ICONERROR);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WindowProc, 0, 0, hInstance,
                      nullptr, LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, L"DX11Class", nullptr };
    RegisterClassEx(&wc);

    const int initialWidth = static_cast<int>(Resolution::MinWidth);
    const int initialHeight = static_cast<int>(Resolution::MinHeight);

    RECT windowRect = { 0, 0, initialWidth, initialHeight };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    HWND hWnd = CreateWindowEx(0, L"DX11Class", L"Game 2D # VioleProg",
        WS_OVERLAPPEDWINDOW, 100, 100, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nShow);

    InitD3D(hWnd);

    MSG msg = { 0 };
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            float screenW = (float)(clientRect.right - clientRect.left);
            float screenH = (float)(clientRect.bottom - clientRect.top);

            float effectiveWidth, effectiveHeight;
            Resolution::GetEffectiveResolution(screenW, screenH, effectiveWidth, effectiveHeight);

            float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
            g_context->ClearRenderTargetView(g_renderTarget.Get(), color);

            auto transformMatrix = Resolution::GetTransformMatrix(screenW, screenH);

            g_spriteBatch->Begin(
                SpriteSortMode_Deferred,
                nullptr,
                g_states->PointClamp(),
                nullptr,
                nullptr,
                nullptr,
                transformMatrix
            );

            if (g_field) g_field->Render(g_spriteBatch.get());

            g_spriteBatch->End();

            g_swapChain->Present(1, 0);
        }
    }
    return (int)msg.wParam;
}