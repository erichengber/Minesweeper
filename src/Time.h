//
// Created by Eric Hengber on 4/18/2023.
//

#pragma once
#include "SFML/Graphics.hpp"

class Time {
    sf::Clock mainTime;
    float runTime;
    bool paused = false;
public:
    void reset() {
        mainTime.restart();
        runTime = 0;
    }

    void pause() {
        if (!paused) {
            runTime += mainTime.getElapsedTime().asSeconds();
        }
        paused = true;
    }

    void unpause() {
        if (paused) {
            mainTime.restart();
        }
        paused = false;
    }

    void winLose() {
        if (!paused) {
            runTime += mainTime.getElapsedTime().asSeconds();
        }
        paused = true;
    }

    float getTotalTime() {
        if (!paused) {
            return runTime + mainTime.getElapsedTime().asSeconds();
        }
        return runTime;
    }
};
