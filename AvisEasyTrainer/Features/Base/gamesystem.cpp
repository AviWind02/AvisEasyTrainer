#include "pch.h"
#include "GameBase.h"

namespace gamebase {

    using namespace RED4ext;
    using namespace RED4ext::game;

    Handle<StatsSystem> GetStatsSystem() {
        return GetGameSystem<StatsSystem>("GetStatsSystem");
    }

    Handle<StatPoolsSystem> GetStatPoolsSystem() {
        return GetGameSystem<StatPoolsSystem>("GetStatPoolsSystem");
    }

}
