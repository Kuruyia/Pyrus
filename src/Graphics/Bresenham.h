#ifndef PYRUS_BRESENHAM_H
#define PYRUS_BRESENHAM_H

#include "Vec2D.h"

namespace Graphics {

class Bresenham {
public:
    Bresenham(Vec2D firstPoint, Vec2D secondPoint);

    void simulateNextPoint(Vec2D &position, Vec2D &p);

    Vec2D getNextPoint();
    Vec2D getNextX(bool skipToLastOfAxis = false);
    Vec2D getNextY(bool skipToLastOfAxis = false);

    bool hasFinished() const;

private:
    Vec2D m_firstPoint;
    Vec2D m_secondPoint;

    Vec2D m_currentPosition;
    bool m_incrementX;

    Vec2D m_delta;
    Vec2D m_absDelta;
    Vec2D m_p;
}; // class Bresenham

} // namespace Graphics

#endif //PYRUS_BRESENHAM_H
