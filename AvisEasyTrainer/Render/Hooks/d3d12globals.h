#pragma once
#include <dxgi.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <dxgi1_4.h>

namespace render::hooks::d3d12
{
    // DX12 globals
    inline ID3D12Device* g_Device = nullptr;
    inline ID3D12CommandQueue* g_CommandQueue = nullptr;

    inline ID3D12DescriptorHeap* g_ImGuiHeap = nullptr;
    inline ID3D12DescriptorHeap* g_RtvHeap = nullptr;

    inline ID3D12CommandAllocator* g_Allocators[3]{};
    inline ID3D12Resource* g_BackBuffers[3]{};
    inline D3D12_CPU_DESCRIPTOR_HANDLE g_RtvHandles[3]{};
    inline UINT g_RtvSize = 0;
    inline bool g_Initialized = false;
    inline bool g_LoggedNullCommandQueue = false;
    inline bool g_LoggedBackBufferError = false;

    inline HWND mainWindow = nullptr;

    HRESULT __fastcall hookPresentD3D12(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
    void __fastcall hookExecuteCommandListsD3D12(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);

    inline HRESULT(__fastcall* oPresentD3D12)(IDXGISwapChain3*, UINT, UINT) = nullptr;
    inline void(__fastcall* oExecuteCommandListsD3D12)(ID3D12CommandQueue*, UINT, ID3D12CommandList*) = nullptr;

    bool SubmitImGuiRender(IDXGISwapChain3* swapChain);
    bool CreateDescriptorHeaps();
    bool InitializeD3D12(IDXGISwapChain3* swapChain);
    bool CreateBackBuffers(IDXGISwapChain3* swapChain);
}
