#include <stdafx.h>

#include "Path.h"
#include <tinyxml.h>

void CPath::AddPoint(const USVec2D& _point)
{
    m_path.push_back(_point);
}

void CPath::Clear()
{
    m_path.clear();
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
