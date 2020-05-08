#include <algorithm>
#include <cmath>

#include "Bresenham.h"

Graphics::Bresenham::Bresenham(Vec2D firstPoint, Vec2D secondPoint)
: m_firstPoint(firstPoint)
, m_secondPoint(secondPoint)
, m_currentPosition({0, 0})
, m_delta({0, 0})
, m_absDelta({0, 0})
, m_p({0, 0})
{
    m_absDelta = {static_cast<int16_t>(std::abs(secondPoint.x - firstPoint.x)), static_cast<int16_t>(std::abs(secondPoint.y - firstPoint.y))};
    if (m_absDelta.x > m_absDelta.y)
    {
        if (m_firstPoint.x > m_secondPoint.x)
            std::swap(m_firstPoint, m_secondPoint);

        m_incrementX = true;
    }
    else
    {
        if (m_firstPoint.y > m_secondPoint.y)
            std::swap(m_firstPoint, m_secondPoint);

        m_incrementX = false;
    }

    m_currentPosition = m_firstPoint;

    m_delta = {static_cast<int16_t>(secondPoint.x - firstPoint.x), static_cast<int16_t>(secondPoint.y - firstPoint.y)};
    m_p = {static_cast<int16_t>(2 * m_absDelta.y - m_absDelta.x), static_cast<int16_t>(2 * m_absDelta.x - m_absDelta.y)};
}

void Graphics::Bresenham::simulateNextPoint(Vec2D &position, Vec2D &p)
{
    // Don't do anything if finished
    if (hasFinished())
    {
        position = m_currentPosition;
        p = m_p;

        return;
    }

    // Copy actual values
    Vec2D currentPosition = m_currentPosition;
    Vec2D pCpy = m_p;

    if (m_incrementX)
    {
        // Increment X and compute Y
        ++currentPosition.x;

        if (pCpy.x < 0)
        {
            pCpy.x = pCpy.x + 2 * m_absDelta.y;
        }
        else
        {
            if ((m_delta.x < 0 && m_delta.y < 0) || (m_delta.x > 0 && m_delta.y > 0))
                ++currentPosition.y;
            else
                --currentPosition.y;

            pCpy.x = pCpy.x + 2 * (m_absDelta.y - m_absDelta.x);
        }
    }
    else
    {
        // Increment Y and compute X
        ++currentPosition.y;

        if (pCpy.y <= 0)
        {
            pCpy.y = pCpy.y + 2 * m_absDelta.x;
        }
        else
        {
            if ((m_delta.x < 0 && m_delta.y < 0) || (m_delta.x > 0 && m_delta.y > 0))
                ++currentPosition.x;
            else
                --currentPosition.x;

            pCpy.y = pCpy.y + 2 * (m_absDelta.x - m_absDelta.y);
        }
    }

    position = currentPosition;
    p = pCpy;
}

Graphics::Vec2D Graphics::Bresenham::getNextPoint()
{
    simulateNextPoint(m_currentPosition, m_p);

    return m_currentPosition;
}

Graphics::Vec2D Graphics::Bresenham::getNextX(bool skipToLastOfAxis)
{
    // Copy the current position
    const int16_t targetX = m_currentPosition.x;

    // Get next point until the X coordinate changes (or the algorithm is finished)
    while (!hasFinished() && targetX == m_currentPosition.x)
        simulateNextPoint(m_currentPosition, m_p);

    // Go to the last point on this X coordinate if asked
    if (skipToLastOfAxis)
    {
        Vec2D nextPoint = {};
        Vec2D p = {};
        while (!hasFinished())
        {
            simulateNextPoint(nextPoint, p);
            if (nextPoint.x == m_currentPosition.x)
            {
                m_currentPosition = nextPoint;
                m_p = p;
            }
            else
            {
                break;
            }
        }
    }

    return m_currentPosition;
}

Graphics::Vec2D Graphics::Bresenham::getNextY(bool skipToLastOfAxis)
{
    // Copy the current position
    const int16_t targetY = m_currentPosition.y;

    // Get next point until the Y coordinate changes (or the algorithm is finished)
    while (!hasFinished() && targetY == m_currentPosition.y)
        simulateNextPoint(m_currentPosition, m_p);

    // Go to the last point on this Y coordinate if asked
    if (skipToLastOfAxis)
    {
        Vec2D nextPoint = {};
        Vec2D p = {};
        while (!hasFinished())
        {
            simulateNextPoint(nextPoint, p);
            if (nextPoint.y == m_currentPosition.y)
            {
                m_currentPosition = nextPoint;
                m_p = p;
            }
            else
            {
                break;
            }
        }
    }

    return m_currentPosition;
}

bool Graphics::Bresenham::hasFinished() const
{
    return (m_incrementX && m_currentPosition.x >= m_secondPoint.x) || (!m_incrementX && m_currentPosition.y >= m_secondPoint.y);
}