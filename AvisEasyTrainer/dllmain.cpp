
#include "pch.h"
#include "Features/VehicleFeat/vehiclefeature.h"
#include "Vendor/minhook/include/MinHook.h"
#include <Base/Natives/vehicle.h>



//Temp loc - mirrored CET for LoadOptimized and thanks to WSSDude for the tip
void (*originalTweakDBLoad)(RED4ext::TweakDB*) = nullptr;
constexpr uint32_t LoadOptimizedHash = 3602585178UL;

std::once_flag injectVehiclesOnce;

void __stdcall HookTweakDBLoad(RED4ext::TweakDB* tdb)
{
    loghandler::sdk->logger->Info(loghandler::handle, "[HookTweakDBLoad] Hook called");

    if (originalTweakDBLoad)
        originalTweakDBLoad(tdb);

    std::call_once(injectVehiclesOnce, []()
    {
        std::thread([]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            for (const auto& vehicle : Feature::VehicleOptions::allVehicles)
            {
                if (vehicle.affiliation.find("player") == std::string::npos)
                {
                    GameBase::Natives::Vehicle::InjectSingleVehicleIntoTweakDB(vehicle.recordID);
                }
            }
            Feature::VehicleOptions::UpdateVehicleNames();// all the game labels into the vector
            loghandler::sdk->logger->Info(loghandler::handle, "[HookTweakDBLoad] Vehicle injection complete.");
        }).detach();
    });
}

void InstallTweakDBLoadHook()
{
    MH_Initialize();

    const RED4ext::UniversalRelocPtr<uint8_t> funcPtr(LoadOptimizedHash);
    uint8_t* address = funcPtr.GetAddr();

    if (!address)
    {
        loghandler::sdk->logger->Error(loghandler::handle, "[InstallTweakDBLoadHook] Failed to resolve LoadOptimized address");
        return;
    }

    if (MH_CreateHook(address, &HookTweakDBLoad, reinterpret_cast<void**>(&originalTweakDBLoad)) != MH_OK ||
        MH_EnableHook(address) != MH_OK)
    {
        loghandler::sdk->logger->Error(loghandler::handle, "[InstallTweakDBLoadHook] Failed to install hook");
    }
    else
    {
        loghandler::sdk->logger->Info(loghandler::handle, "[InstallTweakDBLoadHook] Hook successfully installed");
    }
}

//Called every frame when the game is running
bool Running_OnUpdate(RED4ext::CGameApplication* aApp)
{
	UI::NativeTick();
    return false;
}

bool Running_OnEnter(RED4ext::CGameApplication* aApp)
{
    

    std::thread([=]() { 
        Render::Hooks::WaitForDX12AndInit(loghandler::handle, loghandler::sdk);

        }).detach();
    
    return true;
}

bool Running_OnExit(RED4ext::CGameApplication* aApp)
{
    //clean up here
    return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL RegisterTypes()
{

}

RED4EXT_C_EXPORT void RED4EXT_CALL PostRegisterTypes()
{

}

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::PluginHandle aHandle, RED4ext::EMainReason aReason, const RED4ext::Sdk* aSdk)
{
    RED4EXT_UNUSED_PARAMETER(aSdk);

    switch (aReason)
    {
    case RED4ext::EMainReason::Load:
    {

        loghandler::handle = aHandle;
        loghandler::sdk = aSdk;

        loghandler::sdk->logger->Info(loghandler::handle, "On Load");

        InstallTweakDBLoadHook();

        auto* rtti = RED4ext::CRTTISystem::Get();
        rtti->AddRegisterCallback(RegisterTypes);

        HANDLE mutex = CreateMutexA(nullptr, TRUE, "Global\\EasyTrainer_Mutex");
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            aSdk->logger->Error(aHandle, "EasyTrainer already running. Only one instance can be loaded.");
            return false;
        }


        RED4ext::GameState runningState;
        runningState.OnEnter = &Running_OnEnter;
        runningState.OnUpdate = &Running_OnUpdate;
        runningState.OnExit = &Running_OnExit;
        aSdk->gameStates->Add(aHandle, RED4ext::EGameStateType::Running, &runningState);


        break;
    }

    case RED4ext::EMainReason::Unload:
        break;
    }

    return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::PluginInfo* aInfo)
{
    aInfo->name = L"EasyTrainer";
    aInfo->author = L"Avi";
    aInfo->version = RED4EXT_SEMVER(1, 0, 0);
    aInfo->runtime = RED4EXT_RUNTIME_LATEST;
    aInfo->sdk = RED4EXT_SDK_LATEST;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports()
{
    return RED4EXT_API_VERSION_LATEST;
}
