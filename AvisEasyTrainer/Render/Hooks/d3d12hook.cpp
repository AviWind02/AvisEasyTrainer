#include "pch.h"
#include "Controls/controls.h"

#include "d3d12globals.h"


namespace render::hooks::d3d12
{

    bool SubmitImGuiRender(IDXGISwapChain3* swapChain)
    {
        UINT frameIndex = swapChain->GetCurrentBackBufferIndex();
        if (frameIndex >= 3)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "Frame index out of range");
            return false;
        }

        ID3D12CommandAllocator* allocator = g_Allocators[frameIndex];
        if (!allocator || FAILED(allocator->Reset()))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "Command allocator reset failed");
            return false;
        }

        ID3D12GraphicsCommandList* cmdList = nullptr;
        if (FAILED(g_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, nullptr, IID_PPV_ARGS(&cmdList))))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "Failed to create command list");
            return false;
        }

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = g_BackBuffers[frameIndex];
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        cmdList->ResourceBarrier(1, &barrier);

        cmdList->OMSetRenderTargets(1, &g_RtvHandles[frameIndex], FALSE, nullptr);
        ID3D12DescriptorHeap* heaps[] = { g_ImGuiHeap };
        cmdList->SetDescriptorHeaps(1, heaps);
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);

        // Transition back
        std::swap(barrier.Transition.StateBefore, barrier.Transition.StateAfter);
        cmdList->ResourceBarrier(1, &barrier);

        if (FAILED(cmdList->Close()))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "Failed to close command list");
            return false;
        }

        if (g_CommandQueue)
        {
            g_CommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
        }
        else
        {
            loghandler::sdk->logger->Error(loghandler::handle, "CommandQueue null during draw");
        }

        cmdList->Release();
        return true;
    }

    bool CreateDescriptorHeaps()
    {
        D3D12_DESCRIPTOR_HEAP_DESC srvDesc = {};
        srvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvDesc.NumDescriptors = 2048;
        srvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        if (FAILED(g_Device->CreateDescriptorHeap(&srvDesc, IID_PPV_ARGS(&g_ImGuiHeap))))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "Failed to create ImGui heap");
            return false;
        }

        D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
        rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvDesc.NumDescriptors = 3;
        rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        if (FAILED(g_Device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&g_RtvHeap))))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "Failed to create RTV heap");
            return false;
        }

        g_RtvSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        return true;
    }


    bool InitializeD3D12(IDXGISwapChain3* swapChain)
    {
        loghandler::sdk->logger->Info(loghandler::handle, "Initializing D3D12 and ImGui...");

        if (!swapChain)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "SwapChain is null");
            return false;
        }

        if (FAILED(swapChain->GetDevice(__uuidof(ID3D12Device), (void**)&g_Device)) || !g_Device)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "Failed to get D3D12 device");
            return false;
        }

        DXGI_SWAP_CHAIN_DESC desc;
        if (FAILED(swapChain->GetDesc(&desc)))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "Failed to get swapchain description");
            return false;
        }

        mainWindow = desc.OutputWindow;

        if (!CreateDescriptorHeaps() || !CreateBackBuffers(swapChain))
            return false;

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(mainWindow);
        ImGui_ImplDX12_Init(g_Device, 3, DXGI_FORMAT_R8G8B8A8_UNORM,
            g_ImGuiHeap,
            g_ImGuiHeap->GetCPUDescriptorHandleForHeapStart(),
            g_ImGuiHeap->GetGPUDescriptorHandleForHeapStart());

        controls::inputhook::Init(mainWindow);

        loghandler::sdk->logger->Info(loghandler::handle, "ImGui and D3D12 context initialized");
        return true;
    }

    bool CreateBackBuffers(IDXGISwapChain3* swapChain)
    {
        using namespace render::hooks::d3d12;

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_RtvHeap->GetCPUDescriptorHandleForHeapStart();

        for (UINT i = 0; i < 3; i++)
        {
            if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&g_BackBuffers[i]))))
            {
                if (!g_LoggedBackBufferError)
                {
                    loghandler::sdk->logger->ErrorF(loghandler::handle, "Failed to get backbuffer %d", i);
                    g_LoggedBackBufferError = true;
                }
                return false;
            }

            g_Device->CreateRenderTargetView(g_BackBuffers[i], nullptr, rtvHandle);
            g_RtvHandles[i] = rtvHandle;
            rtvHandle.ptr += g_RtvSize;

            if (FAILED(g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_Allocators[i]))))
            {
                loghandler::sdk->logger->ErrorF(loghandler::handle, "Failed to create command allocator %d", i);
                return false;
            }
        }

        return true;
    }

    HRESULT __fastcall hookPresentD3D12(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
    {
        if (!g_CommandQueue && !g_LoggedNullCommandQueue)
        {
            loghandler::sdk->logger->Info(loghandler::handle, "CommandQueue is null on first frame");
            g_LoggedNullCommandQueue = true;
        }

        if (!g_Initialized)
        {
            if (!InitializeD3D12(pSwapChain))
                return oPresentD3D12(pSwapChain, SyncInterval, Flags);

            g_Initialized = true;
        }

        ImGuiIO& io = ImGui::GetIO();
        io.WantSetMousePos = false;
        io.WantCaptureMouse = true;
        io.WantCaptureKeyboard = true;

        ImGui_ImplWin32_NewFrame();
        ImGui_ImplDX12_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();

        if (!SubmitImGuiRender(pSwapChain))
            return oPresentD3D12(pSwapChain, SyncInterval, Flags);

        return oPresentD3D12(pSwapChain, SyncInterval, Flags);
    }

    void __fastcall hookExecuteCommandListsD3D12(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists)
    {
        if (!g_CommandQueue)
        {
            g_CommandQueue = queue;
            loghandler::sdk->logger->Info(loghandler::handle, "CommandQueue captured");
        }

        oExecuteCommandListsD3D12(queue, NumCommandLists, ppCommandLists);
    }
}
