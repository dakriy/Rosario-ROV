#include "Event.h"

Core::Event::~Event() {
	if (type == SensorInfoReceived) {
		if (sInfo.type == SensorInfo::Video) {
			delete [] sInfo.f.data;
		}
	}
}
