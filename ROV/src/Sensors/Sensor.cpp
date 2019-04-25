#include "Sensor.h"

float Sensor::Sensor::getFrequency() {
    return 1.f / period;
}

float Sensor::Sensor::getPeriod() {
    return period;
}

void Sensor::Sensor::setFrequency(float F) {
    period = 1.f / F;
}

void Sensor::Sensor::setPeriod(float T) {
    period = T;
}

bool Sensor::Sensor::needsPolled() {
    return timer.getElapsedTime().asSeconds() > period;
}
