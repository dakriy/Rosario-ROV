#pragma once

#include <csv/writer.hpp>
#include "../Interfaces/IUpdateable.h"

class Mission : public Interfaces::IUpdateable {
protected:

public:
	bool showMissionWindow = false;

	Mission();
	void update(const sf::Time&) override;
	~Mission() override;
};

