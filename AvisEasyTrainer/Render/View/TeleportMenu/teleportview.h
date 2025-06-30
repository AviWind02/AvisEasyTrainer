#pragma once
namespace view::teleport {
	void TeleportMainView();
	inline UI::SubMenu teleportMenu{ "Teleport Menu", &TeleportMainView };

}