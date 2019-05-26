#include "Servo.h"
#include "../Core/GlobalContext.h"

Camera::Servo::Servo() : controller(&Servo::thread, this), done(false) {
}

void Camera::Servo::thread() {
	while (!done) {
		control();
	}
}

Camera::Servo::~Servo() {
	done = true;
	if (controller.joinable()) controller.join();
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(updateHook);
}
