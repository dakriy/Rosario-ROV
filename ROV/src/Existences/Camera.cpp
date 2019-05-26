#include "Camera.h"
#include "../Factories/PacketFactory.h"
#include "../Core/GlobalContext.h"
#include <SFML/System.hpp>
#include <wiringPi.h>
#include <chrono>
#include <ctime>

bool Camera::Camera::init() {
	auto capInit = capture.open(0);
	if (!capInit || !capture.isOpened())
		return false;

	// TODO: Make these dynamic
	capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
	capture.set(cv::CAP_PROP_FRAME_WIDTH, 800);
	capture.set(cv::CAP_PROP_FRAME_HEIGHT, 600);
	capture.set(cv::CAP_PROP_FPS, 30);

	// Default resolution of the frame is obtained.The default resolution is system dependent.
//	int frame_width = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
//	int frame_height = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
//	auto vidInit = video.open("file.avi", cv::VideoWriter::fourcc('M','J','P','G'),30, cv::Size(frame_width,frame_height));
	return getCameraProperties();
}

void Camera::Camera::cam() {
	if (!initialized) {
		initialized = init();
	}
	sf::Clock c;
	while (running) {
		if (!recordVideo && !sendVideo) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		} else {
			// TODO: Figure out a way to get errors from the capture device if it doesn't want to initialize
			if (!initialized || !capture.isOpened()) {
				// Try to reopen if it doesn't open
				initialized = init();
			} else {
				// Get the next frame
				cv::Mat frame;
				// End of stream... uhhh
				if (!capture.read(frame)) {
					// TODO: Handle this case
					recordVideo = false;
					sendVideo = false;
					continue;
				}

				// For now just send it to the boat
				if (sendVideo) {
					if (GlobalContext::get_network()->isConnected()) {
						// Too much data for network stream, so we have to compress to jpg
						std::vector<uint8_t> jpg;

						if (cv::imencode(".jpg", frame, jpg, std::vector<int> {
									cv::IMWRITE_JPEG_QUALITY,
									80
							})) {
							// Compression successful
							GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_video_packet(jpg));
						}
					} else {
						endVideoStream();
						if (recordVideo) {
							endVideoRecord();
						}
					}
				}

				if (recordVideo && video.isOpened()) {
					video.write(frame);
				} else {
					video.release();
				}
			}
			auto t = c.getElapsedTime().asMilliseconds();
			if (t < 1.f / props.framerate * 1000) {
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<unsigned>(1.f / props.framerate * 1000 - t)));
			}
			c.restart();
		}
	}
}

Camera::Camera::Camera() : sendVideo(false), recordVideo(false), initialized(false), cameraCapture(&Camera::cam, this), running(true) {
	camStartStopRequest = GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) -> bool {
		if (e->type == Core::Event::StartCamera) {
			startVideoStream();
			return true;
		} else if (e->type == Core::Event::StopCamera) {
			endVideoStream();
			return true;
		}
		return false;
	}, Core::Event::StartCamera, Core::Event::StopCamera);

	camRecordRequest = GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e)->bool{
		if (std::get<bool>(e->data)) {
			startVideoRecord();
		} else {
			endVideoRecord();
		}
		return true;
	}, Core::Event::VideoRecord);
}

void Camera::Camera::startVideoStream() {
	sendVideo = true;
}

void Camera::Camera::endVideoStream() {
	sendVideo = false;
}

void Camera::Camera::startVideoRecord() {
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto ltm = localtime(&t);
	std::string name;
	name += std::to_string(ltm->tm_hour) + '.' + std::to_string(ltm->tm_min) + '.' + std::to_string(ltm->tm_sec);
	name += ".avi";
	recordVideo = video.open(name, cv::VideoWriter::fourcc('H', '2', '6', '4'), props.framerate, cv::Size(props.width, props.height));
	if (recordVideo) {
		GlobalContext::get_engine()->log("Starting Video Record");
	}
}

void Camera::Camera::endVideoRecord() {
	recordVideo = false;
	GlobalContext::get_engine()->log("Stopping Video Record");
	video.release();
}

Camera::Properties &Camera::Camera::getProps() {
	return props;
}

bool Camera::Camera::getCameraProperties() {
	if (!capture.isOpened())
		return false;

	// Compute the framerate
	// With webcams get(CV_CAP_PROP_FPS) does not work.
	// We did set it up above, but now we need to actually see how fast we can get.
	// So we have to test the limit ourselves
	constexpr unsigned testFramerateWindow = 120;
	sf::Clock c;
	// Grab the number of frames.
	for(int i = 0; i < testFramerateWindow; i++)
	{
		cv::Mat frame;
		if (!capture.read(frame))
			return false;
	}
	auto time = c.getElapsedTime().asSeconds();
	props.framerate = static_cast<float>(testFramerateWindow) / time;
	props.width = static_cast<unsigned>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
	props.height = static_cast<unsigned>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
	return true;
}

Camera::Camera::~Camera() {
	running = false;
	recordVideo = false;
	sendVideo = false;
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(camStartStopRequest);
	if (cameraCapture.joinable()) cameraCapture.join();
	capture.release();
	video.release();
}
