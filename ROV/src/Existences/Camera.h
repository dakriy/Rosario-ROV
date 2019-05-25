#pragma once

#include <opencv2/opencv.hpp>
#include <SFML/Graphics/Image.hpp>
#include "../Core/Event.h"
#include "../Core/EventHandler.h"
#include "ServoHalf.h"
#include "../Utilities/Existence.h"

namespace Camera {
	struct Properties {
		float framerate = 0.f;
		unsigned width = 0;
		unsigned height = 0;
	};

	class Camera : public Existence {

		Properties props;

		std::thread cameraCapture;

		std::atomic_bool sendVideo, recordVideo, initialized, running;

		cv::VideoCapture capture;
		cv::VideoWriter video;

		EVENT_FUNC_INDEX_CORE camStartStopRequest;

		void cam();
		bool getCameraProperties();

		bool init();

		/**
		 * All this really needs to do is exist
		 * I'm putting it here in camera because it's related to the camera in that it moves it around
		 */
	public:
		ServoHalf s1;
		Camera();
		void startVideoStream();
		void endVideoStream();
		void startVideoRecord();
		void endVideoRecord();
		Properties & getProps();
		~Camera() override;
	};
}
