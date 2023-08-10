#pragma once

class Event {
    public:
        static const int TYPE_COLLISION = 1;
        static const int TYPE_DEATH = 2;
        static const int TYPE_SPAWN = 3;
        static const int TYPE_INPUT = 4;
        int type;
        int timestamp;
        void setTimestamp(int ts) {this->timestamp = ts; };
};