
#include "pch.h"


//Called every frame when the game is running
bool Running_OnUpdate(RED4ext::CGameApplication* aApp)
{
	render::ui::NativeTick();
    return false;
}

bool Running_OnEnter(RED4ext::CGameApplication* aApp)
{
    std::thread([=]() { render::hooks::WaitForDX12AndInit(loghandler::handle, loghandler::sdk); }).detach();

    return true;
}

bool Running_OnExit(RED4ext::CGameApplication* aApp)
{
    //clean up here
    return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL RegisterTypes()
{
    // You can leave this empty or register CET-callable functions here if needed later
}

RED4EXT_C_EXPORT void RED4EXT_CALL PostRegisterTypes()
{
    // Not used in this version
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
