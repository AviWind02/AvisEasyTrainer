#include "pch.h"
#include "gamebase.h"

namespace gamebase {

    using namespace RED4ext;
    using namespace RED4ext::game;

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


    RED4ext::Handle<RED4ext::IScriptable> TryGetPlayerVehicleComponent()
    {
        using namespace RED4ext;

        Handle<IScriptable> playerHandle;
        if (!gamebase::TryGetPlayerHandle(playerHandle))
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[GetPlayerVehicleComponent] Failed to get player handle");
            return nullptr;
        }

        auto* rtti = CRTTISystem::Get();
        if (!rtti)
            return nullptr;

        auto* puppetObj = reinterpret_cast<game::Puppet*>(playerHandle.instance);
        if (!puppetObj)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[GetPlayerVehicleComponent] Player object is null");
            return nullptr;
        }

        // Try both Puppet and PlayerPuppet classes if available
        auto* puppetClass = rtti->GetClass("gamePuppet");
        if (!puppetClass)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[GetPlayerVehicleComponent] gamePuppet class not found");
            return nullptr;
        }

        auto* vehicleProp = puppetClass->GetProperty("vehicleComponent");
        if (!vehicleProp)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[GetPlayerVehicleComponent] vehicleComponent property not found");
            return nullptr;
        }

        Handle<IScriptable> vehicleComponent = vehicleProp->GetValue<Handle<IScriptable>>(puppetObj);
        return vehicleComponent;
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
