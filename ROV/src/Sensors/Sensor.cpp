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


bool Sensor::SensorInfo::hasDataForSensor(Sensor::SensorInfo::Sensor s) const {
    return intelligence.find(s) != intelligence.end();
}

const Sensor::SensorInfo::Measurement * Sensor::SensorInfo::getSensorData(Sensor::SensorInfo::Sensor s) const {
    auto i = intelligence.find(s);
    if (i == intelligence.end())
        return nullptr;
    return &i->second;
}

