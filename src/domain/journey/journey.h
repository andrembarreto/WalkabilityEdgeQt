#ifndef JOURNEY_H
#define JOURNEY_H

struct Journey
{
public:
    void setActive(bool active) { m_isActive = active; }
    void setElapsedTime(int elapsedTime_s) { m_elapsedTime = elapsedTime_s; }

    bool isActive() const { return m_isActive; }
    int elapsedTime() const { return m_elapsedTime; }

private:
    bool m_isActive = false;
    int m_elapsedTime = 0;
};

#endif // JOURNEY_H
