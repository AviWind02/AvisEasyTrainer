#include "pch.h"
#include "Controls/controls.h"

#include "d3d12globals.h"

#include "Vendor/minhook/include/MinHook.h"
#include <RED4ext/GpuApi/DeviceData.hpp>
namespace render::hooks
{
    static bool hooksInitialized = false;

    void Init(RED4ext::PluginHandle aHandle, const RED4ext::v0::Sdk* aSdk, uint32_t swapID)
    {
        render::hooks::d3d12::g_WorkingSwapChainID = swapID;

        loghandler::handle = aHandle;
        loghandler::sdk = aSdk;

        aSdk->logger->InfoF(aHandle, "Initializing custom D3D12 hook using swapID: %u...", swapID);

        auto* deviceData = RED4ext::GpuApi::GetDeviceData();
        if (!deviceData)
        {
            aSdk->logger->Error(aHandle, "DeviceData is null.");
            return;
        }

        render::hooks::d3d12::g_CachedDeviceData = RED4ext::GpuApi::GetDeviceData();

        auto& swapContainer = deviceData->swapChains;
        if (!swapContainer.IsValidID(swapID) || !swapContainer.IsUsedID(swapID))
        {
            aSdk->logger->ErrorF(aHandle, "swapChain ID %u is invalid or unused.", swapID);
            return;
        }

        aSdk->logger->InfoF(aHandle, "Accessing swapContainer.GetData(%u)...", swapID);

        const auto& swapData = swapContainer.GetData(swapID);

        __try
        {
            aSdk->logger->InfoF(aHandle, "windowHandle: 0x%p", swapData.windowHandle);
            aSdk->logger->InfoF(aHandle, "backBufferIndex: %u", swapData.backBufferIndex);
            aSdk->logger->InfoF(aHandle, "backBufferTextureId: %u", swapData.backBufferTextureId);
            aSdk->logger->InfoF(aHandle, "startupHdrMode: %u", static_cast<uint8_t>(swapData.startupHdrMode));
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            aSdk->logger->Error(aHandle, "Access violation while logging swapData.");
            return;
        }

        IDXGISwapChain4* swapChain4 = swapData.swapChain.Get();
        if (!swapChain4)
        {
            aSdk->logger->Error(aHandle, "swapChain is null.");
            return;
        }

        IDXGISwapChain3* swapChain3 = nullptr;
        if (FAILED(swapChain4->QueryInterface(IID_PPV_ARGS(&swapChain3))) || !swapChain3)
        {
            aSdk->logger->Error(aHandle, "Failed to cast IDXGISwapChain4 to IDXGISwapChain3.");
            return;
        }

        aSdk->logger->Info(aHandle, "swapChain3 acquired successfully.");

        void** vtable = *reinterpret_cast<void***>(swapChain3);
        if (!vtable)
        {
            aSdk->logger->Error(aHandle, "swapChain3 vtable is null.");
            return;
        }

        auto presentFn = vtable[8];
        if (!presentFn)
        {
            aSdk->logger->Error(aHandle, "vtable[8] (Present) is null.");
            return;
        }

        aSdk->logger->InfoF(aHandle, "Present vfunc address: 0x%p", presentFn);

        render::hooks::d3d12::oPresentD3D12 = reinterpret_cast<decltype(render::hooks::d3d12::oPresentD3D12)>(presentFn);

        DWORD oldProtect;
        if (!VirtualProtect(&vtable[8], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            aSdk->logger->Error(aHandle, "Failed to change memory protection on vtable.");
            return;
        }

        vtable[8] = reinterpret_cast<void*>(&render::hooks::d3d12::hookPresentD3D12);
        VirtualProtect(&vtable[8], sizeof(void*), oldProtect, &oldProtect);

        aSdk->logger->Info(aHandle, "Hooked Present successfully.");
    }

    void WaitForDX12AndInit(RED4ext::PluginHandle aHandle, const RED4ext::v0::Sdk* aSdk)
    {
        aSdk->logger->Info(aHandle, "Waiting for REDengine DX12 system...");

        try
        {
            while (true)
            {
                auto* deviceData = RED4ext::GpuApi::GetDeviceData();
                if (deviceData)
                {
                    auto& swapChains = deviceData->swapChains;
                    for (uint32_t id = 1; id < 64; ++id)
                    {
                        if (!swapChains.IsValidID(id) || !swapChains.IsUsedID(id))
                            continue;

                        const auto& swapData = swapChains.GetData(id);
                        IDXGISwapChain4* sc4 = swapData.swapChain.Get();

                        if (!sc4)
                            continue;

                        IDXGISwapChain3* sc3 = nullptr;
                        if (FAILED(sc4->QueryInterface(IID_PPV_ARGS(&sc3))) || !sc3)
                            continue;

                        // Validate by checking GetBuffer(0)
                        ID3D12Resource* testBuffer = nullptr;
                        HRESULT hr = sc3->GetBuffer(0, IID_PPV_ARGS(&testBuffer));
                        if (SUCCEEDED(hr) && testBuffer)
                        {
                            testBuffer->Release(); // Only checking that it works
                            aSdk->logger->InfoF(aHandle, "Valid swapChain found at ID %u", id);
                            Init(aHandle, aSdk, id);
                            sc3->Release();
                            return;
                        }

                        sc3->Release();
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }
        catch (const std::exception& ex)
        {
            MessageBoxA(nullptr, ex.what(), "WaitForDX12AndInit Exception", MB_OK | MB_ICONERROR);
        }
        catch (...)
        {
            MessageBoxA(nullptr, "Unknown crash inside WaitForDX12AndInit", "Fatal Error", MB_OK | MB_ICONERROR);
        }
    }



    void Shutdown() // Prevents the game from staying open
    {

		using namespace render::hooks::d3d12;   

        loghandler::sdk->logger->Info(loghandler::handle, "[Shutdown] Entry");

        ImGui_ImplDX12_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        loghandler::sdk->logger->Info(loghandler::handle, "[Shutdown] ImGui shutdown");

        if (g_FenceEvent) CloseHandle(g_FenceEvent);
        if (g_Fence) g_Fence->Release();
        if (g_CommandQueue) g_CommandQueue->Release();
        if (g_RtvHeap) g_RtvHeap->Release();
        if (g_ImGuiHeap) g_ImGuiHeap->Release();
        if (g_Device) g_Device->Release();

        for (int i = 0; i < 3; ++i)
        {
            if (g_BackBuffers[i]) g_BackBuffers[i]->Release();
            if (g_Allocators[i]) g_Allocators[i]->Release();
        }

        MH_Uninitialize();
        loghandler::sdk->logger->Info(loghandler::handle, "[Shutdown] Resources cleaned up");
    }

}