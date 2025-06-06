#pragma once
// This D3D12 hook system was developed primarily through vibe coding with ChatGPT.
// A lot of it came together through trial and error, and debug driven.
// Concepts were inspired by general DirectX12 hooking practices from like GTA 5, with most influence from CET.


#include "pch.h"
#include "Controls/controls.h"
#include "d3d12globals.h"
#include "Vendor/minhook/include/MinHook.h"
#include <RED4ext/GpuApi/DeviceData.hpp>

using namespace g_feature::g_math;

namespace render::hooks::d3d12
{

    using ExecuteCommandLists_t = void (STDMETHODCALLTYPE*)(
        ID3D12CommandQueue*, UINT, ID3D12CommandList* const*);
    using CreateCommandQueue_t = HRESULT(STDMETHODCALLTYPE*)(
        ID3D12Device*, const D3D12_COMMAND_QUEUE_DESC*, REFIID, void**);

    // Original function pointers
    static ExecuteCommandLists_t oExecuteCommandListsD3D12 = nullptr;
    static CreateCommandQueue_t oCreateCommandQueue = nullptr;

    // Capture the engine's queue from RED4ext DeviceData
    void EnsureRealCommandQueue()
    {
        if (!g_CommandQueue)
        {
            loghandler::sdk->logger->Info(loghandler::handle, "[QueueCapture] Entry");
            auto* deviceData = RED4ext::GpuApi::GetDeviceData();
            if (deviceData)
            {
                ID3D12CommandQueue* queue = deviceData->directCommandQueue.Get();
                loghandler::sdk->logger->InfoF(loghandler::handle,
                    "[QueueCapture] directCommandQueue pointer: %p", (void*)queue);
                if (queue)
                {
                    g_CommandQueue = queue;
                    g_CommandQueue->AddRef();
                    loghandler::sdk->logger->InfoF(loghandler::handle,
                        "[QueueCapture] Captured queue: %p", (void*)queue);
                }
                else
                {
                    loghandler::sdk->logger->Error(loghandler::handle,
                        "[QueueCapture] directCommandQueue is null");
                }
            }
            else
            {
                loghandler::sdk->logger->Error(loghandler::handle,
                    "[QueueCapture] GetDeviceData() returned null");
            }
            loghandler::sdk->logger->Info(loghandler::handle, "[QueueCapture] Exit");
        }
    }

    // Hook ID3D12Device::CreateCommandQueue (fallback, if needed)
    HRESULT STDMETHODCALLTYPE hookCreateCommandQueue(
        ID3D12Device* device,
        const D3D12_COMMAND_QUEUE_DESC* desc,
        REFIID riid,
        void** ppCommandQueue)
    {
        loghandler::sdk->logger->Info(loghandler::handle, "[HookCQ] Entry");
        HRESULT hr = oCreateCommandQueue(device, desc, riid, ppCommandQueue);
        loghandler::sdk->logger->InfoF(loghandler::handle, "[HookCQ] CreateCommandQueue hr=0x%08X", hr);
        if (SUCCEEDED(hr) && riid == __uuidof(ID3D12CommandQueue) && ppCommandQueue && *ppCommandQueue)
        {
            auto queue = static_cast<ID3D12CommandQueue*>(*ppCommandQueue);
            g_CommandQueue = queue;
            g_CommandQueue->AddRef();
            loghandler::sdk->logger->InfoF(loghandler::handle,
                "[HookCQ] Captured via hook: %p", (void*)queue);
        }
        loghandler::sdk->logger->Info(loghandler::handle, "[HookCQ] Exit");
        return hr;
    }

    void HookCreateCommandQueue()
    {
        loghandler::sdk->logger->Info(loghandler::handle, "[HookCQ] Entry");
        MH_Initialize();
        void** vtbl = *reinterpret_cast<void***>(g_Device);
        constexpr int IDX_CreateCommandQueue = 44;
        MH_CreateHook(vtbl[IDX_CreateCommandQueue],
            &hookCreateCommandQueue,
            reinterpret_cast<void**>(&oCreateCommandQueue));
        auto res = MH_EnableHook(vtbl[IDX_CreateCommandQueue]);
        loghandler::sdk->logger->InfoF(loghandler::handle,
            "[HookCQ] MH_EnableHook result = %d", res);
        loghandler::sdk->logger->Info(loghandler::handle, "[HookCQ] Exit");
    }

    // Create descriptor heaps for RTV and SRV/UAV
    bool CreateDescriptorHeaps()
    {
        loghandler::sdk->logger->Info(loghandler::handle, "[CDH] Entry");
        if (!g_Device)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[CDH] g_Device is null");
            return false;
        }

        D3D12_DESCRIPTOR_HEAP_DESC srvDesc{};
        srvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvDesc.NumDescriptors = 2048;
        srvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        HRESULT hr = g_Device->CreateDescriptorHeap(&srvDesc, IID_PPV_ARGS(&g_ImGuiHeap));
        loghandler::sdk->logger->InfoF(loghandler::handle, "[CDH] Create ImGuiHeap hr=0x%08X", hr);
        if (FAILED(hr)) return false;

        D3D12_DESCRIPTOR_HEAP_DESC rtvDesc{};
        rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvDesc.NumDescriptors = 3;
        rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        hr = g_Device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&g_RtvHeap));
        loghandler::sdk->logger->InfoF(loghandler::handle, "[CDH] Create RtvHeap hr=0x%08X", hr);
        if (FAILED(hr)) return false;

        g_RtvSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        loghandler::sdk->logger->InfoF(loghandler::handle,
            "[CDH] RTV descriptor increment size: %u", g_RtvSize);
        loghandler::sdk->logger->Info(loghandler::handle, "[CDH] Exit");
        return true;
    }

    // Create backbuffers and allocators
    bool CreateBackBuffers(IDXGISwapChain3* swapChain)
    {
        loghandler::sdk->logger->Info(loghandler::handle, "[CBB] Entry");
        if (!g_RtvHeap)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[CBB] g_RtvHeap is null");
            return false;
        }

        auto rtvHandle = g_RtvHeap->GetCPUDescriptorHandleForHeapStart();
        for (UINT i = 0; i < 3; ++i)
        {
            loghandler::sdk->logger->InfoF(loghandler::handle, "[CBB] Buffer %u setup", i);
            HRESULT hr = swapChain->GetBuffer(i, IID_PPV_ARGS(&g_BackBuffers[i]));
            loghandler::sdk->logger->InfoF(loghandler::handle, "[CBB] GetBuffer(%u) hr=0x%08X", i, hr);
            if (FAILED(hr)) return false;

            g_Device->CreateRenderTargetView(g_BackBuffers[i], nullptr, rtvHandle);
            g_RtvHandles[i] = rtvHandle;
            rtvHandle.ptr += g_RtvSize;

            hr = g_Device->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_DIRECT,
                IID_PPV_ARGS(&g_Allocators[i]));
            loghandler::sdk->logger->InfoF(loghandler::handle, "[CBB] CreateAllocator(%u) hr=0x%08X", i, hr);
            if (FAILED(hr)) return false;
        }

        loghandler::sdk->logger->Info(loghandler::handle, "[CBB] Exit");
        return true;
    }



    // Render submission with detailed micro-logging
    bool SubmitImGuiRender(IDXGISwapChain3* swapChain)
    {
        //loghandler::sdk->logger->Info(loghandler::handle, "[Submit] Entry");
        ImDrawData* drawData = ImGui::GetDrawData();
        if (!drawData || drawData->CmdListsCount == 0 || drawData->TotalVtxCount == 0)
        {
                loghandler::sdk->logger->Warn(loghandler::handle, "[Submit] No draw data, skipping");
            return false;
        }

        UINT frameIndex = swapChain->GetCurrentBackBufferIndex();
        //loghandler::sdk->logger->InfoF(loghandler::handle, "[Submit] FrameIndex=%u", frameIndex);
        if (frameIndex >= ARRAYSIZE(g_BackBuffers))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[Submit] Invalid frame index");
            return false;
        }

        auto* allocator = g_Allocators[frameIndex];
        HRESULT hr = allocator ? allocator->Reset() : E_FAIL;
        //loghandler::sdk->logger->InfoF(loghandler::handle, "[Submit] Allocator.Reset() == 0x%08X", hr);
        if (FAILED(hr)) return false;

        ID3D12GraphicsCommandList* cmdList = nullptr;
        hr = g_Device->CreateCommandList(
            0,
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            allocator,
            nullptr,
            IID_PPV_ARGS(&cmdList));
        //loghandler::sdk->logger->InfoF(loghandler::handle, "[Submit] CreateCommandList == 0x%08X", hr);
        if (FAILED(hr) || !cmdList) return false;

        // Transition to render target
       // loghandler::sdk->logger->Info(loghandler::handle, "[Submit] Barrier to RENDER_TARGET");
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = g_BackBuffers[frameIndex];
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        cmdList->ResourceBarrier(1, &barrier);

        // Set render target
        //loghandler::sdk->logger->Info(loghandler::handle, "[Submit] OMSetRenderTargets");
        cmdList->OMSetRenderTargets(1, &g_RtvHandles[frameIndex], FALSE, nullptr);

        // Set descriptor heap for ImGui
        //loghandler::sdk->logger->Info(loghandler::handle, "[Submit] SetDescriptorHeaps");
        ID3D12DescriptorHeap* heaps[] = { g_ImGuiHeap };
        cmdList->SetDescriptorHeaps(1, heaps);

        // Render ImGui
        //loghandler::sdk->logger->Info(loghandler::handle, ">> ImGui_ImplDX12_RenderDrawData");
        ImGui_ImplDX12_RenderDrawData(drawData, cmdList);
       // loghandler::sdk->logger->Info(loghandler::handle, "<< ImGui_ImplDX12_RenderDrawData");

        // Transition back to present
        //loghandler::sdk->logger->Info(loghandler::handle, "[Submit] Barrier to PRESENT");
        std::swap(barrier.Transition.StateBefore, barrier.Transition.StateAfter);
        cmdList->ResourceBarrier(1, &barrier);

        // Close command list
        hr = cmdList->Close();
       // loghandler::sdk->logger->InfoF(loghandler::handle, "[Submit] cmdList.Close() == 0x%08X", hr);
        if (FAILED(hr)) { cmdList->Release(); return false; }

        // Execute on real queue
        EnsureRealCommandQueue();
        if (g_CommandQueue)
        {
            //loghandler::sdk->logger->Info(loghandler::handle, "[Submit] ExecuteCommandLists");
            g_CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&cmdList));

            //loghandler::sdk->logger->InfoF(loghandler::handle, "[Submit] Signal fence %llu", g_FenceValue + 1);
            g_CommandQueue->Signal(g_Fence, ++g_FenceValue);
            UINT64 completed = g_Fence->GetCompletedValue();
            //loghandler::sdk->logger->InfoF(loghandler::handle, "[Submit] Fence.GetCompletedValue() == %llu", completed);
            if (completed < g_FenceValue)
            {
                //loghandler::sdk->logger->Info(loghandler::handle, "[Submit] Waiting on fence event");
                g_Fence->SetEventOnCompletion(g_FenceValue, g_FenceEvent);
                WaitForSingleObject(g_FenceEvent, INFINITE);
            }
        }
        else
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[Submit] CommandQueue null, skipping execution");
        }

        cmdList->Release();
        //loghandler::sdk->logger->Info(loghandler::handle, "[Submit] Exit");
        return true;
    }

    bool InitializeD3D12(IDXGISwapChain3* swapChain)
    {
        loghandler::sdk->logger->Info(loghandler::handle, "[Init] InitializeD3D12 Entry");
        if (!swapChain)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[Init] swapChain is null");
            return false;
        }

        HRESULT hr = swapChain->GetDevice(__uuidof(ID3D12Device), (void**)&g_Device);
        loghandler::sdk->logger->InfoF(loghandler::handle, "[Init] GetDevice == 0x%08X, g_Device=%p", hr, (void*)g_Device);
        if (FAILED(hr) || !g_Device)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[Init] Failed to retrieve D3D12 device");
            return false;
        }

        DXGI_SWAP_CHAIN_DESC desc{};
        hr = swapChain->GetDesc(&desc);
        loghandler::sdk->logger->InfoF(loghandler::handle, "[Init] GetDesc == 0x%08X, OutputWindow=%p", hr, (void*)desc.OutputWindow);
        if (FAILED(hr))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[Init] Failed to get swapchain description");
            return false;
        }
        g_MainWindow = desc.OutputWindow;

        loghandler::sdk->logger->Info(loghandler::handle, "[Init] Creating descriptor heaps and backbuffers");
        if (!CreateDescriptorHeaps() || !CreateBackBuffers(swapChain))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[Init] Heap or backbuffer creation failed");
            return false;
        }

        loghandler::sdk->logger->Info(loghandler::handle, "[Init] Hooking CreateCommandQueue");
        HookCreateCommandQueue();

        hr = g_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence));
        loghandler::sdk->logger->InfoF(loghandler::handle, "[Init] CreateFence == 0x%08X", hr);
        if (FAILED(hr))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[Init] Failed to create fence");
            return false;
        }
        g_FenceValue = 0;
        g_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        loghandler::sdk->logger->InfoF(loghandler::handle, "[Init] CreateEvent == %p", (void*)g_FenceEvent);
        if (!g_FenceEvent)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[Init] Failed to create fence event");
            return false;
        }

        if (!ImGui::GetCurrentContext())
        {
            loghandler::sdk->logger->Info(loghandler::handle, "[Init] Creating ImGui context");
            ImGui::CreateContext();
        }
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        loghandler::sdk->logger->Info(loghandler::handle, "[Init] Initializing ImGui Win32 and DX12");
        ImGui_ImplWin32_Init(g_MainWindow);
        ImGui_ImplDX12_Init(
            g_Device,
            3,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            g_ImGuiHeap,
            g_ImGuiHeap->GetCPUDescriptorHandleForHeapStart(),
            g_ImGuiHeap->GetGPUDescriptorHandleForHeapStart());


        controls::inputhook::Init(g_MainWindow);
        
        FontManager::InitializeFonts();
        
        render::ui::InitializeUIStyle();

        loghandler::sdk->logger->Info(loghandler::handle, "[Init] InitializeD3D12 Exit");
        return true;
    }

    // Present hook with entry/exit logging
    HRESULT __fastcall hookPresentD3D12(IDXGISwapChain3* swapChain, UINT SyncInterval, UINT Flags)
    {
        //loghandler::sdk->logger->Info(loghandler::handle, "[Present] Entry");
        try
        {
            static IDXGISwapChain3* last = nullptr;
            if (swapChain != last)
            {
                loghandler::sdk->logger->Warn(loghandler::handle, "[Present] SwapChain changed");
                last = swapChain;
            }

            if (!g_Initialized)
            {
                if (!InitializeD3D12(swapChain))
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[Present] InitializeD3D12 failed");
                    return oPresentD3D12(swapChain, SyncInterval, Flags);
                }
                g_Initialized = true;
            }

            ImGui_ImplWin32_NewFrame();
            ImGui_ImplDX12_NewFrame();
            ImGui::NewFrame();
    /*        ImGui::GetIO().MouseDrawCursor = controls::menuOpen;
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            controls::inputhook::SetMouseTrap(controls::menuOpen);*/
            render::ui::DrawMainMenu();

            ImGui::Render();
            if (ImGui::GetDrawData() && ImGui::GetDrawData()->CmdListsCount > 0)
            {
                if (!SubmitImGuiRender(swapChain))
                    loghandler::sdk->logger->Error(loghandler::handle, "[Present] SubmitImGuiRender failed");
            }
        }
        catch (const std::exception& ex)
        {
            loghandler::sdk->logger->ErrorF(loghandler::handle, "[Present] Exception: %s", ex.what());
        }
        catch (...)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[Present] Unknown exception");
        }
        //loghandler::sdk->logger->Info(loghandler::handle, "[Present] Exit");
        return oPresentD3D12(swapChain, SyncInterval, Flags);
    }

    // Optional fallback hook on ExecuteCommandLists
    void __fastcall hookExecuteCommandListsD3D12(
        ID3D12CommandQueue* queue,
        UINT NumCommandLists,
        ID3D12CommandList* const* ppCommandLists)
    {
        //loghandler::sdk->logger->Info(loghandler::handle, "[ExecCL] Entry");
        if (!g_CommandQueue)
        {
            g_CommandQueue = queue;
            g_CommandQueue->AddRef();
            //loghandler::sdk->logger->Info(loghandler::handle, "[ExecCL] Captured queue fallback");
        }
        oExecuteCommandListsD3D12(queue, NumCommandLists, ppCommandLists);
        //loghandler::sdk->logger->Info(loghandler::handle, "[ExecCL] Exit");
    }
}