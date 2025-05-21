#include "pch.h"
#include "Controls/controls.h"

#include "d3d12globals.h"

#include "Vendor/minhook/include/MinHook.h"
namespace render::hooks
{
    static bool hooksInitialized = false;

    void Init(RED4ext::PluginHandle aHandle, const RED4ext::v0::Sdk* aSdk)
    {
        loghandler::handle = aHandle;
        loghandler::sdk = aSdk;

        aSdk->logger->Info(aHandle, "Initializing Kiero for D3D12...");

        auto status = kiero::init(kiero::RenderType::D3D12);
        if (status != kiero::Status::Success)
        {
            aSdk->logger->ErrorF(aHandle, "Kiero failed to initialize (code %d)", status);
            return;
        }

        aSdk->logger->Info(aHandle, "Kiero initialized successfully.");

        using namespace render::hooks::d3d12;

        auto presentStatus = kiero::bind(140, reinterpret_cast<void**>(&oPresentD3D12), hookPresentD3D12);
        auto executeStatus = kiero::bind(54, reinterpret_cast<void**>(&oExecuteCommandListsD3D12), hookExecuteCommandListsD3D12);

        if (presentStatus == kiero::Status::Success)
            aSdk->logger->Info(aHandle, "Hooked PresentD3D12 successfully.");
        else
            aSdk->logger->Error(aHandle, "Failed to hook PresentD3D12.");

        if (executeStatus == kiero::Status::Success)
            aSdk->logger->Info(aHandle, "Hooked ExecuteCommandListsD3D12 successfully.");
        else
            aSdk->logger->Error(aHandle, "Failed to hook ExecuteCommandListsD3D12.");

        if (presentStatus == kiero::Status::Success && executeStatus == kiero::Status::Success)
        {
            hooksInitialized = true;
            aSdk->logger->Info(aHandle, "All hooks applied successfully.");
        }
        else
        {
            aSdk->logger->Error(aHandle, "One or more DX12 hooks failed.");
        }
    }

    void WaitForDX12AndInit(RED4ext::PluginHandle aHandle, const RED4ext::v0::Sdk* aSdk)
    {
        aSdk->logger->Info(aHandle, "Waiting for Cyberpunk 2077 window to initialize...");

        while (true)
        {
            HWND hwnd = FindWindowA(nullptr, "Cyberpunk 2077 (C) 2020 by CD Projekt RED");
            if (hwnd)
            {
                aSdk->logger->Info(aHandle, "Game window found");
                std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // Give DX12 time to settle
                Init(aHandle, aSdk);
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void Shutdown()
    {
        if (hooksInitialized)
        {
            kiero::shutdown();
            hooksInitialized = false;
            loghandler::sdk->logger->Info(loghandler::handle, "Hooks shutdown complete.");
        }
    }
}