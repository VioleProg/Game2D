#include "main.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

ComPtr<ID3D11Device>           g_device;
ComPtr<ID3D11DeviceContext>    g_context;
ComPtr<IDXGISwapChain>         g_swapChain;
ComPtr<ID3D11RenderTargetView> g_renderTarget;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) { PostQuitMessage(0); return 0; }
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

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &scd, &g_swapChain, &g_device, nullptr, &g_context);

    ComPtr<ID3D11Texture2D> pBackBuffer;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    g_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &g_renderTarget);
    g_context->OMSetRenderTargets(1, g_renderTarget.GetAddressOf(), nullptr);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WindowProc, 0, 0, hInstance,
                      nullptr, LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, L"DX11Class", nullptr };
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(0, L"DX11Class", L"Game 2D # VioleProg",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nShow);

    InitD3D(hWnd);

    MSG msg = { 0 };
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
            g_context->ClearRenderTargetView(g_renderTarget.Get(), color);

            g_swapChain->Present(1, 0);
        }
    }
    return (int)msg.wParam;
}