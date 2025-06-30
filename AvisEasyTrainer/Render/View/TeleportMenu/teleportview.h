#pragma once
namespace View::Teleport {
	void TeleportMainView();
	inline UI::SubMenu teleportMenu{ "Teleport Menu", &TeleportMainView };

}