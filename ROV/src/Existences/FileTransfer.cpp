#include "FileTransfer.h"
#include <filesystem>
#include <fstream>
#include "../Core/GlobalContext.h"
#include "../Factories/PacketFactory.h"

std::vector<std::string> FileTransfer::getMissionDataFiles() {
	std::vector<std::string> files;
	if (std::filesystem::exists(missionDirectory)) {
		for (auto& f : std::filesystem::directory_iterator(missionDirectory)) {
			files.emplace_back(f.path().filename());
		}
		return files;
	}
	return std::vector<std::string>();
}

void FileTransfer::clearMissionDataFiles() {
	std::filesystem::remove_all(missionDirectory);
}

FileTransfer::FileTransfer(const std::string &missionDir) : missionDirectory(missionDir) {
	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) ->bool{
		auto list = getMissionDataFiles();
		GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_mission_data_file_list_packet(list));
		return true;
	}, Core::Event::MissionFileListRequested));

	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) ->bool{
		auto filePath = missionDirectory / std::get<std::string>(e->data);
		if (std::filesystem::exists(filePath)) {
			std::ifstream file(filePath, std::ios::binary);
			GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_mission_data_file_packet(file));
		} else {
			GlobalContext::get_engine()->log("Could not find the file to upload.");
		}
		return true;
	}, Core::Event::MissionFileRequested));

	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) ->bool{
		clearMissionDataFiles();
		return true;
	}, Core::Event::DeleteMissionFiles));
}
