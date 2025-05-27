#pragma once
#include <dxgi.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <dxgi1_4.h>

#include "RED4ext/GpuApi/Buffer.hpp"
#include "RED4ext/GpuApi/CommandListContext.hpp"
#include "RED4ext/GpuApi/CommandListContext-inl.hpp"
#include "RED4ext/GpuApi/D3D12MemAlloc.hpp"
#include "RED4ext/GpuApi/D3D12MemAlloc-inl.hpp"
#include "RED4ext/GpuApi/DeviceData.hpp"
#include "RED4ext/GpuApi/DeviceData-inl.hpp"
#include "RED4ext/GpuApi/SwapChain.hpp"

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

    inline HWND g_MainWindow = nullptr;

    HRESULT __fastcall hookPresentD3D12(IDXGISwapChain3* swapChain, UINT SyncInterval, UINT Flags);

    void __fastcall hookExecuteCommandListsD3D12(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);

    inline HRESULT(__fastcall* oPresentD3D12)(IDXGISwapChain3*, UINT, UINT) = nullptr;
    //inline void(__fastcall* oExecuteCommandListsD3D12)(ID3D12CommandQueue*, UINT, ID3D12CommandList*) = nullptr;

    typedef void (STDMETHODCALLTYPE* ExecuteCommandLists_t)(ID3D12CommandQueue*, UINT, ID3D12CommandList* const*);
    inline ExecuteCommandLists_t oExecuteCommandLists = nullptr;

    //bool SubmitImGuiRender(IDXGISwapChain3* swapChain);
    //bool CreateDescriptorHeaps();
    //bool InitializeD3D12(IDXGISwapChain3* swapChain);
    //bool CreateBackBuffers(IDXGISwapChain3* swapChain);

    inline uint32_t g_WorkingSwapChainID = 32;

    inline RED4ext::GpuApi::SDeviceData* g_CachedDeviceData = nullptr;
    inline ID3D12GraphicsCommandList* g_CommandList = nullptr;
    inline ID3D12Fence* g_Fence = nullptr;
    inline HANDLE g_FenceEvent = nullptr;
    inline UINT64 g_FenceValue = 0;

}
