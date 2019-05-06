#include "Mission.h"
#include "../Core/GlobalContext.h"


Mission::Mission() {
	GlobalContext::get_engine()->addUpdateableEntitiy(this);
}

Mission::~Mission() {
	GlobalContext::get_engine()->removeUpdateableEntity(this);
}

void Mission::update(const sf::Time &) {
	if (showMissionWindow) {
		ImGui::Begin("Mission");

		ImGui::Button("lol hi");

		ImGui::End();
	}
}
