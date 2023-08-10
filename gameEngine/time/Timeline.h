#pragma once
#include <mutex>
#include <time.h>
#include <iostream>

class Timeline {
    private:
        std::mutex m;
        int startTime;
        float tic;
        bool paused;
        int lastPausedTime;
        int elapsedPausedTime;
        Timeline *anchor;
    
    public:
        Timeline(Timeline *anchor, float tic);
        int getTime();
        bool isPaused();
        float getTic();
        int pause();
        int unpause();
        void changeTic(float newTic);
};