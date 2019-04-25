#include "Event.h"

bool Core::SensorInfo::hasDataForSensor(Core::SensorInfo::Sensor s) const {
    return intelligence.find(s) != intelligence.end();
}

const Core::SensorInfo::Measurement * Core::SensorInfo::getSensorData(Core::SensorInfo::Sensor s) const {
    auto i = intelligence.find(s);
    if (i == intelligence.end())
        return nullptr;
    return &i->second;
}
