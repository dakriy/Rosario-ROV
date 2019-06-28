#pragma once

#include <string>
#include <vector>
#include "../Utilities/Existence.h"
#include "../Core/Event.h"
#include "../Core/EventHandler.h"
#include <filesystem>


class FileTransfer : public Existence {
protected:

	std::vector<EVENT_FUNC_INDEX_CORE> hooks;

	std::filesystem::path missionDirectory;
	std::vector<std::string> getMissionDataFiles();
	void clearMissionDataFiles();
public:
	explicit FileTransfer(const std::string& missionDir);
};
