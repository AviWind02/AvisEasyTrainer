#include "pch.h"
#include "gamebase.h"

namespace gamebase {

    using namespace RED4ext;
    using namespace RED4ext::game;

    bool TryGetGameInstance(ScriptGameInstance& outInstance) {
        return ExecuteGlobalFunction("GetGameInstance", &outInstance);
    }

    ent::Entity* GetPlayerEntity() {
        ScriptGameInstance gi;
        if (!TryGetGameInstance(gi))
            return nullptr;

        Handle<IScriptable> handle;
        if (!ExecuteFunction("ScriptGameInstance", "GetPlayer", &handle, gi) || !handle)
            return nullptr;

        return reinterpret_cast<ent::Entity*>(handle.instance);
    }

    bool TryGetPlayerHandle(Handle<IScriptable>& outHandle) {
        ScriptGameInstance gi;
        if (!TryGetGameInstance(gi))
            return false;

        return ExecuteFunction("ScriptGameInstance", "GetPlayer", &outHandle, gi) && outHandle;
    }

    bool TryGetPlayerID(ent::EntityID& outID) {
        Handle<IScriptable> handle;
        if (!TryGetPlayerHandle(handle))
            return false;

        auto* cls = CRTTISystem::Get()->GetClass("entEntity");
        if (!cls)
            return false;

        auto* fn = cls->GetFunction("GetEntityID");
        if (!fn)
            return false;

        StackArgs_t args;
        return ExecuteFunction(handle, fn, &outID, args) && outID.hash != 0;
    }

    bool TryGetPlayerHandleAndID(Handle<IScriptable>& outHandle, ent::EntityID& outID) {
        if (!TryGetPlayerHandle(outHandle))
            return false;
        return TryGetPlayerID(outID);
    }

    // Template implementation
    template<typename T>
    Handle<T> GetGameSystem(const char* functionName) {
        ScriptGameInstance gi;
        if (!TryGetGameInstance(gi))
            return {};

        Handle<T> sys;
        if (!ExecuteFunction("ScriptGameInstance", functionName, &sys, gi) || !sys)
            return {};

        return sys;
    }

}
