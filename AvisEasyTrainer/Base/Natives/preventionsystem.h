#pragma once
#include "Base/GameBase.h"

namespace GameBase {
    namespace Natives {
        namespace Prevention {

            inline bool SetPlayerWantedLevel(int level)
            {
                auto* rtti = CRTTISystem::Get();
                if (!rtti)
                    return false;

                auto* cls = rtti->GetClass("SetWantedLevel");
                if (!cls)
                    return false;

                ScriptInstance raw = cls->CreateInstance(true);
                if (!raw)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWantedLevel] Failed to create instance of SetWantedLevel.");
                    return false;
                }

                cls->InitializeProperties(raw);

                RED4ext::Handle<IScriptable> request;
                request.instance = reinterpret_cast<IScriptable*>(raw);

                uint32_t clamped = std::clamp(level, 0, 5);
                auto* prop = cls->GetProperty("wantedLevel");
                if (prop)
                    prop->SetValue(request, &clamped);

                ScriptGameInstance gi;
                if (!TryGetGameInstance(gi))
                    return false;

                auto* fn = rtti->GetFunction("PreventionSystem::QueueRequest;GameInstanceScriptableSystemRequestFloat");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWantedLevel] QueueRequest function not found.");
                    return false;
                }

                float delay = 0.0f;
                StackArgs_t args{
                    { nullptr, &gi },
                    { nullptr, &request },
                    { nullptr, &delay }
                };

                bool result = false;
                if (!ExecuteFunction((ScriptInstance)nullptr, fn, &result, args))
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWantedLevel] ExecuteFunction failed.");
                    return false;
                }

                return result;
            }


            inline bool DisablePoliceSystem(bool disabled)
            {
                auto* rtti = CRTTISystem::Get();
                if (!rtti)
                    return false;

                auto* cls = rtti->GetClass("TogglePreventionSystem");
                if (!cls)
                    return false;

                ScriptInstance raw = cls->CreateInstance(true);
                if (!raw) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] Failed to create instance.");
                    return false;
                }

                cls->InitializeProperties(raw);

                RED4ext::Handle<IScriptable> request;
                request.instance = reinterpret_cast<IScriptable*>(raw);

                RED4ext::CName sourceName("SMDisablePolice");
                if (auto* sourceProp = cls->GetProperty("sourceName"))
                    sourceProp->SetValue(request, &sourceName);
                else {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] Property 'sourceName' not found");
                    return false;
                }

                bool isActive = !disabled;
                if (auto* activeProp = cls->GetProperty("isActive"))
                    activeProp->SetValue(request, &isActive);
                else {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] Property 'isActive' not found");
                    return false;
                }

                ScriptGameInstance gi;
                if (!TryGetGameInstance(gi))
                    return false;

                auto* fn = rtti->GetFunction("PreventionSystem::QueueRequest;GameInstanceScriptableSystemRequestFloat");
                if (!fn) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] Function not found.");
                    return false;
                }

                float delay = 0.0f;
                StackArgs_t args{
                    { nullptr, &gi },
                    { nullptr, &request },
                    { nullptr, &delay }
                };

                bool result = false;
                if (!ExecuteFunction((ScriptInstance)nullptr, fn, &result, args)) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] ExecuteFunction failed.");
                    return false;
                }

                return result;
            }


        }
    }
}


