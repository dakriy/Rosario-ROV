#pragma once

#include <csv/writer.hpp>
#include "../Interfaces/IUpdateable.h"

class Mission : public Interfaces::IUpdateable {
protected:
	bool showMissionWindow = false;
public:
	Mission();
	void update(const sf::Time&) override;
	void showWindow();
	void hideWindow();
	~Mission() override;
};

