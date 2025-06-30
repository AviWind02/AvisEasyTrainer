#pragma once
namespace view::world {
    namespace timeview {

        void TimeView();
        inline UI::SubMenu timeMenu{ "Game Time Menu", &TimeView };
    }

    namespace weatherview {
        void WeatherView();
        inline UI::SubMenu weatherControlMenu{ "Weather Control", &WeatherView };

    }
}