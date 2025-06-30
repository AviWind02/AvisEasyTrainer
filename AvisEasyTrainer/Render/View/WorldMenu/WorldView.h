#pragma once
namespace View::World {
    namespace TimeView {

        void TimeView();
        inline UI::SubMenu timeMenu{ "Game Time Menu", &TimeView };
    }

    namespace WeatherView {
        void WeatherView();
        inline UI::SubMenu weatherControlMenu{ "Weather Control", &WeatherView };

    }
}