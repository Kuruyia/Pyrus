#ifndef PYRUS_BRESENHAM_H
#define PYRUS_BRESENHAM_H

#include "Vec2D.h"

namespace Graphics {

class Bresenham
{
public:
    Bresenham(Vec2D firstPoint, Vec2D secondPoint, bool progressGuarantee = false);

    bool getProgressGuarantee() const;
    void setProgressGuarantee(bool progressGuarantee);

    void simulateNextPoint(Vec2D &position, Vec2D &p);

    Vec2D getNextPoint();
    Vec2D getNextX(bool skipToLastOfAxis = false);
    Vec2D getNextY(bool skipToLastOfAxis = false);

    Vec2D getOffset() const;
    Vec2D getCurrentPosition() const;
    const Vec2D &getRawCurrentPosition() const;

    bool hasFinished() const;

private:
    bool m_progressGuarantee;
    bool m_pointsSwapped;

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
