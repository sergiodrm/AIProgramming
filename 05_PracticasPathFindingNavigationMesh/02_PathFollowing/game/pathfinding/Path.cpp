#include <stdafx.h>

#include "Path.h"
#include <tinyxml.h>

void CPath::AddPoint(const USVec2D& _point)
{
    m_path.insert(m_path.begin(), _point);
}

void CPath::Clear()
{
    m_path.clear();
}

CPath::SPointInSegment CPath::GetClosestPoint(const USVec2D& _point) const
{
    SPointInSegment closestPoint;
    float minSquareDistance = -1.f;
    for (size_t index = 0; index < m_path.size() - 1; ++index)
    {
        USVec2D pointInPath;
        const USVec2D origin = m_path.at(index);
        const USVec2D end = m_path.at(index + 1);
        const float denominator =
            (end.mX - origin.mX) * (end.mX - origin.mX) + (end.mY - origin.mY) * (end.mY - origin.mY);
        if (denominator != 0.f)
        {
            const float u =
                ((_point.mX - origin.mX) * (end.mX - origin.mX) + (_point.mY - origin.mY) * (end.mY - origin.mY)) /
                denominator;

            // Calculate square distance of current segment
            if (u < 0.f)
            {
                pointInPath = origin;
            }
            else if (u > 1.f)
            {
                pointInPath = end;
            }
            else
            {
                pointInPath = {origin.mX + u * (end.mX - origin.mX), origin.mY + u * (end.mY - origin.mY)};
            }
            const float squareDistance = pointInPath.DistSqrd(_point);

            // Check last square distance
            if (minSquareDistance < 0.f || squareDistance < minSquareDistance)
            {
                minSquareDistance = squareDistance;
                closestPoint.SegmentPoint = pointInPath;
                closestPoint.SegmentIndex = index;
            }
        }
    }
    return closestPoint;
}

USVec2D CPath::GetPointAhead(const SPointInSegment& _point, float _distance) const
{
    std::vector<USVec2D>::const_iterator it = m_path.begin() + _point.SegmentIndex;

    USVec2D segmentDirection = *(it + 1) - *it;
    segmentDirection.NormSafe();
    USVec2D nextPoint = _point.SegmentPoint + segmentDirection * _distance;

    if ((nextPoint - *it).LengthSquared() > (*(it + 1) - *it).LengthSquared() && it + 2 != m_path.end())
    {
        const float restOfDistance = _distance - (*(it + 1) - _point.SegmentPoint).Length();
        segmentDirection = *(it + 2) - *(it + 1);
        segmentDirection.NormSafe();
        nextPoint = *(it + 1) + segmentDirection * restOfDistance;
    }
    else if (it + 2 == m_path.end())
    {
        nextPoint = *(it + 1);
    }

    return nextPoint;
}

void CPath::DrawDebug() const
{
    if (!m_path.empty())
    {
        MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

        gfxDevice.SetPenColor(0.9f, 0.1f, 0.1f, 1.f);
        gfxDevice.SetPenWidth(3.f);
        for (int32_t index = 0; index < m_path.size() - 1; ++index)
        {
            MOAIDraw::DrawLine(m_path.at(index), m_path.at(index + 1));
        }
        gfxDevice.SetPenWidth(1.f);
    }
}
