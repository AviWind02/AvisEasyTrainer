#pragma once
#pragma once
#include <Base/gamebase.h>


using namespace RED4ext;

namespace gamebase {
    namespace natives {
        namespace prevention {

            inline bool SetPlayerWantedLevel(int level)
            {
                using namespace RED4ext;

                auto* rtti = CRTTISystem::Get();
                if (!rtti)
                    return false;

                auto* cls = rtti->GetClass("SetWantedLevel");
                if (!cls)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWantedLevel] Class 'SetWantedLevel' not found");
                    return false;
                }

                ScriptInstance raw = cls->CreateInstance(true);
                if (!raw)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWantedLevel] CreateInstance returned null");
                    return false;
                }

                cls->InitializeProperties(raw);

                cls->InitializeProperties(raw);

                RED4ext::Handle<IScriptable> request;
                request.instance = reinterpret_cast<IScriptable*>(raw);
                if (!request)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWantedLevel] Handle wrap failed");
                }

                uint32_t clamped = std::clamp(level, 0, 5);
                auto* prop = cls->GetProperty("wantedLevel");
                if (!prop)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWantedLevel] Property 'wantedLevel' not found");
                    return false;
                }
                prop->SetValue(request, &clamped);


                ScriptGameInstance gi;
                if (!gamebase::TryGetGameInstance(gi))
                    return false;

                auto* fn = rtti->GetFunction("PreventionSystem::QueueRequest;GameInstanceScriptableSystemRequestFloat");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWantedLevel] QueueRequest function not found");
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
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWantedLevel] QueueRequest execution failed");
                    return false;
                }

                return result;
            }

            inline bool DisablePoliceSystem(bool disabled)
            {
                using namespace RED4ext;

                auto* rtti = CRTTISystem::Get();
                if (!rtti)
                    return false;

                auto* cls = rtti->GetClass("TogglePreventionSystem");
                if (!cls)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] Class 'TogglePreventionSystem' not found");
                    return false;
                }

                ScriptInstance raw = cls->CreateInstance(true);
                if (!raw)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] CreateInstance returned null");
                    return false;
                }

                cls->InitializeProperties(raw);

                RED4ext::Handle<IScriptable> request;
                request.instance = reinterpret_cast<IScriptable*>(raw);
                if (!request)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] Handle wrap failed");
                    return false;
                }

                auto* sourceProp = cls->GetProperty("sourceName");
                if (!sourceProp)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] Property 'sourceName' not found");
                    return false;
                }

                RED4ext::CName sourceName("SMDisablePolice");
                sourceProp->SetValue(request, &sourceName);

                auto* activeProp = cls->GetProperty("isActive");
                if (!activeProp)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] Property 'isActive' not found");
                    return false;
                }

                bool isActive = !disabled;
                activeProp->SetValue(request, &isActive);

                ScriptGameInstance gi;
                if (!gamebase::TryGetGameInstance(gi))
                    return false;

                auto* fn = rtti->GetFunction("PreventionSystem::QueueRequest;GameInstanceScriptableSystemRequestFloat");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] QueueRequest function not found");
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
                    loghandler::sdk->logger->Error(loghandler::handle, "[DisablePoliceSystem] QueueRequest execution failed");
                    return false;
                }

                return result;
               
            }



        }
    }
}


