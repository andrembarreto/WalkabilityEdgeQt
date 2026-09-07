#ifndef POSITION_H
#define POSITION_H

using timestamp_t = long long;

struct Position
{
    float latitude;
    float longitude;
    timestamp_t timestamp;

    bool operator==(const Position& other) const
    {
        return latitude == other.latitude &&
               longitude == other.longitude &&
               timestamp == other.timestamp;
    }
};

#endif // POSITION_H
