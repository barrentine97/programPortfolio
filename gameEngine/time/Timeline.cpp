#include "Timeline.h"

using namespace std::chrono;

/**
 * @brief Construct a new Timeline:: Timeline object
 * 
 * @param anchor the timeline this timeline anchors to
 * @param tic the tic size
 */
Timeline::Timeline(Timeline *anchor, float tic) {
    this->anchor = anchor;
    this->tic = tic;
    this->startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    this->elapsedPausedTime = 0;
    this->paused = false;
}

/**
 * @brief returns the time, with respect to this timeline
 * 
 * @return int the current time, with respect to this timeline
 */
int Timeline::getTime() {
    if (paused) {
        return lastPausedTime;
    }

    int currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - elapsedPausedTime;
    return currentTime - this->startTime;
}

/**
 * @brief returns the pause status
 * 
 * @return true if paused
 * @return false if not paused
 */
bool Timeline::isPaused() {
    return paused;
}

/**
 * @brief returns the tic size
 * 
 * @return float the tic size
 */
float Timeline::getTic() {
    return tic;
}

/**
 * @brief pauses the timeline
 * 
 * @return int if pausing was successful
 */
int Timeline::pause() {
    if (paused) {
        return false;
    }

    this->lastPausedTime = getTime();
    this->elapsedPausedTime = 0;
    paused = true;
    return true;
}

/**
 * @brief unpauses the timeline
 * 
 * @return int if unpausing was successful
 */
int Timeline::unpause() {
    if (!paused) {
        return false;
    }

    int currentTime = anchor->getTime();
    this->elapsedPausedTime += (currentTime - this->lastPausedTime);
    this->paused = false;
    return true;
}

/**
 * @brief changes the tic size
 * 
 * @param newTic the new tic size
 */
void Timeline::changeTic(float newTic) {
    tic = newTic;
}