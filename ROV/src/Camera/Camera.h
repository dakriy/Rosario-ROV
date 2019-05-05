#pragma once

#include <opencv2/opencv.hpp>
#include <SFML/Graphics/Image.hpp>

namespace Camera {
	struct Properties {
		float framerate = 0.f;
		unsigned width = 0;
		unsigned height = 0;
	};

	class Camera {

		Properties props;

		std::thread cameraCapture;

		std::atomic_bool sendVideo, recordVideo, initialized, running;

		cv::VideoCapture capture;
		cv::VideoWriter video;

		void cam();
		bool getCameraProperties();

		bool init();
	public:
		Camera();
		void startVideoStream();
		void endVideoStream();
		void startVideoRecord();
		void endVideoRecord();
		Properties & getProps();
		~Camera();
	};
}
