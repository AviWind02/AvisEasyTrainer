#include "pch.h"
#include "GameBase.h"

namespace GameBase {


    bool TryGetGameInstance(ScriptGameInstance& outInstance)
    {
        return ExecuteGlobalFunction("GetGameInstance", &outInstance);
    }

    bool TryGetPlayerHandle(Handle<IScriptable>& outHandle)
    {
        ScriptGameInstance gameInstance;
        if (!TryGetGameInstance(gameInstance))
            return false;

        return ExecuteGlobalFunction("GetPlayer;GameInstance", &outHandle, gameInstance) && outHandle;
    }

    bool TryGetPlayerID(ent::EntityID& outID)
    {
        Handle<IScriptable> handle;
        if (!TryGetPlayerHandle(handle))
            return false;

        auto* rtti = CRTTISystem::Get();
        auto* getEntityID = rtti->GetClass("entEntity")->GetFunction("GetEntityID");
        StackArgs_t args;

        return ExecuteFunction(handle, getEntityID, &outID, args) && outID.hash != 0;
    }

    bool TryGetPlayerHandleAndID(Handle<IScriptable>& outHandle, ent::EntityID& outID)
    {
        if (!TryGetPlayerHandle(outHandle))
            return false;

        auto* rtti = CRTTISystem::Get();
        auto* getEntityID = rtti->GetClass("entEntity")->GetFunction("GetEntityID");
        StackArgs_t args;

        return ExecuteFunction(outHandle, getEntityID, &outID, args) && outID.hash != 0;
    }

    ent::Entity* GetPlayerEntity()
    {
        Handle<IScriptable> handle;
        if (!TryGetPlayerHandle(handle))
            return nullptr;

        return reinterpret_cast<ent::Entity*>(handle.instance);
    }

    bool TryGetVehicleHandle(Handle<IScriptable>& outHandle)
    {
        Handle<IScriptable> player;
        if (!TryGetPlayerHandle(player))
            return false;

        return RED4ext::ExecuteGlobalFunction("GetMountedVehicle;GameObject", &outHandle, player) && outHandle;
    }

    bool TryGetVehicleHandleAndID(Handle<IScriptable>& outHandle, ent::EntityID& outID)
    {
        if (!TryGetVehicleHandle(outHandle))
            return false;

        auto* rtti = RED4ext::CRTTISystem::Get();
        auto* getEntityID = rtti->GetClass("entEntity")->GetFunction("GetEntityID");
        StackArgs_t args;

        return RED4ext::ExecuteFunction(outHandle, getEntityID, &outID, args) && outID.hash != 0;
    }




    static bool hasRun = false;

    void ListAllGlobalFunctionsOnce()
    {
        if (hasRun)
            return;
        hasRun = true;

        RED4ext::CRTTISystem* rtti = RED4ext::CRTTISystem::Get();
        if (!rtti)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[ListAllGlobalFunctionsOnce] Failed to get CRTTISystem.");
            return;
        }

        loghandler::sdk->logger->Info(loghandler::handle, "==== GLOBAL FUNCTION LIST START ====");

        rtti->funcs.ForEach([](const RED4ext::CName& name, RED4ext::CGlobalFunction*& func)
            {
                if (func)
                {
                    auto shortName = func->shortName.ToString();
                    auto fullName = func->fullName.ToString();

                    loghandler::sdk->logger->InfoF(
                        loghandler::handle,
                        "ShortName: %s | FullName: %s",
                        shortName,
                        fullName
                    );
                }
            });

        loghandler::sdk->logger->Info(loghandler::handle, "==== GLOBAL FUNCTION LIST END ====");
    }

}
