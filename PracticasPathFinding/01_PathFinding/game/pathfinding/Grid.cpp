#include <stdafx.h>

#include "Grid.h"


CGrid::CGrid() {}

CGrid* CGrid::Load(const char* _mapFilename, const char* _codeFilename)
{
    if (!_mapFilename || !_codeFilename)
    {
        printf("Invalid filename");
        return nullptr;
    }

    CGrid* grid = new CGrid();
    std::map<char, float> codeMap;
    {
        std::ifstream codeStream;
        codeStream.open(_codeFilename);
        if (!codeStream.is_open())
        {
            printf("Error loading code file");
            codeStream.close();
            return nullptr;
        }

        std::string line;
        while (std::getline(codeStream, line))
        {
            const size_t index = line.find(':');
            if (index == std::string::npos)
            {
                printf("Invalid format");
                codeStream.close();
                return nullptr;
            }

            codeMap.insert(std::make_pair(line.at(0), std::stof(line.substr(index + 1))));
        }
        codeStream.close();
    }

    {
        std::ifstream mapStream;
        mapStream.open(_mapFilename);
        if (!mapStream.is_open())
        {
            printf("Error loading map file");
            mapStream.close();
            return nullptr;
        }

        std::string line;
        int sizeX = 0;
        int sizeY = 0;
        while (std::getline(mapStream, line))
        {
            ++sizeY;
            if (sizeX == 0)
            {
                sizeX = line.size();
            }

            for (char it : line)
            {
                float cost = codeMap.at(it);
                grid->m_map.push_back(cost);
            }
        }
        mapStream.close();

        grid->m_size = USVec2D(static_cast<float>(sizeX), static_cast<float>(sizeY));
    }

    return grid;
}


float CGrid::GetCost(uint32_t _index) const
{
    return m_map.at(_index);
}

float CGrid::GetCost(uint32_t _row, uint32_t _col) const
{
    const uint32_t index = _row * static_cast<uint32_t>(m_size.mX) + _col;
    if (index > m_map.size())
    {
        return -1.f;
    }
    return GetCost(index);
}

float CGrid::GetCost(const USVec2D& _gridLocation) const
{
    return GetCost(static_cast<uint32_t>(_gridLocation.mY), static_cast<uint32_t>(_gridLocation.mX));
}

USVec2D CGrid::GetSize() const
{
    return m_size;
}

USVec2D CGrid::GetRectSize() const
{
    return USVec2D(static_cast<float>(MOAIGfxDevice::Get().GetWidth()) / m_size.mX,
        static_cast<float>(MOAIGfxDevice::Get().GetHeight()) / m_size.mY);
}

bool CGrid::IsValidPosition(const USVec2D& _position) const
{
    return _position.mX >= 0.f && _position.mX < m_size.mX && _position.mY >= 0.f && _position.mY < m_size.mY;
}

USVec2D CGrid::GridToWorldLocation(const USVec2D& _gridLocation) const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
    const float halfWidth = static_cast<float>(gfxDevice.GetWidth()) / 2.f;
    const float halfHeight = static_cast<float>(gfxDevice.GetHeight()) / 2.f;
    USVec2D rectSize = GetRectSize();
    USVec2D screenLocation(rectSize.mX * _gridLocation.mX, rectSize.mY * _gridLocation.mY);
    USVec2D worldLocation = screenLocation - USVec2D(halfWidth, halfHeight);
    return worldLocation;
}

USVec2D CGrid::WorldToGridLocation(const USVec2D& _worldLocation) const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
    const float halfWidth = static_cast<float>(gfxDevice.GetWidth()) / 2.f;
    const float halfHeight = static_cast<float>(gfxDevice.GetHeight()) / 2.f;
    const USVec2D worldLocationFromScreenCorner = _worldLocation + USVec2D(halfWidth, halfHeight);
    USVec2D rectSize = GetRectSize();
    USVec2D gridLocation(floorf(worldLocationFromScreenCorner.mX / rectSize.mX),
                        floorf(worldLocationFromScreenCorner.mY / rectSize.mY));
    return gridLocation;
}

void CGrid::DrawDebug() const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    const float halfWidth = static_cast<float>(gfxDevice.GetWidth()) / 2.f;
    const float halfHeight = static_cast<float>(gfxDevice.GetHeight()) / 2.f;
    USVec2D rectSize(static_cast<float>(gfxDevice.GetWidth()) / m_size.mX,
                     static_cast<float>(gfxDevice.GetHeight()) / m_size.mY);
    for (int32_t row = 0; row < static_cast<int32_t>(m_size.mY); ++row)
    {
        for (int32_t col = 0; col < static_cast<int32_t>(m_size.mX); ++col)
        {
            USRect rect;
            rect.mXMin = static_cast<float>(col) * rectSize.mX - halfWidth;
            rect.mXMax = rect.mXMin + rectSize.mX;
            rect.mYMin = static_cast<float>(row) * rectSize.mY - halfHeight;
            rect.mYMax = rect.mYMin + rectSize.mY;

            const int32_t cost = static_cast<int32_t>(GetCost(row, col));

            gfxDevice.SetPenColor(0.2f, 0.2f, 0.4f, 1.f);
            if (cost == -1)
            {
                MOAIDraw::DrawRectFill(rect);
            }
            else
            {
                gfxDevice.SetPenColor(0.01f + 0.2f * cost, 0.4f, 0.1f, 0.1f);
                MOAIDraw::DrawRectFill(rect);
                gfxDevice.SetPenColor(0.1f, 0.1f, 0.1f, 1.f);
                MOAIDraw::DrawRectOutline(rect);
            }
        }
    }
}

void CGrid::DrawRectangle(const USVec2D& _gridPosition, const USVec4D& _color) const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
}
