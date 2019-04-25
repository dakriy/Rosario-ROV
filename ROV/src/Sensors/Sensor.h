#pragma once

#include <SFML/System/Clock.hpp>

namespace Sensor {
    class Sensor {
    protected:
        float period = 1;
        sf::Clock timer;
    public:
        float getPeriod();
        void setPeriod(float T);
        float getFrequency();
        void setFrequency(float F);
        bool needsPolled();
    };
}