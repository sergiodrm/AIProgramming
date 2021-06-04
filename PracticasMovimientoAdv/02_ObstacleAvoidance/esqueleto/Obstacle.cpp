#include <stdafx.h>
#include <tinyxml.h>
#include "Obstacle.h"

void CObstacle::LoadFromFile(std::vector<std::unique_ptr<CObstacle>>& obstacles_, const char* _filename)
{
    TiXmlDocument doc(_filename);
    if (!doc.LoadFile())
    {
        fprintf(stderr, "Couldn't read params from %s", _filename);
        return;
    }

    TiXmlHandle hDoc(&doc);

    TiXmlElement* pElem;
    pElem = hDoc.FirstChildElement().Element();
    if (!pElem)
    {
        fprintf(stderr, "Invalid format for %s", _filename);
        return;
    }

    TiXmlHandle hRoot(pElem);
    TiXmlHandle hParams = hRoot.FirstChildElement("obstacles");

    TiXmlElement* paramElem = hParams.FirstChild().Element();
    for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement())
    {
        const char* paramName = paramElem->Value();
        if (!strcmp(paramName, "obstacle"))
        {
            USVec2D pos;
            paramElem->Attribute("x", &pos.mX);
            paramElem->Attribute("y", &pos.mY);
            float radius;
            paramElem->Attribute("r", &radius);
            obstacles_.push_back(std::make_unique<CObstacle>(pos, radius));
        }
    }
}

CObstacle::CObstacle(const USVec2D& _position, float _radius)
    : m_position(_position), m_radius(_radius)
{ }

const USVec2D& CObstacle::GetPosition() const { return m_position; }
float CObstacle::GetRadius() const { return m_radius; }

void CObstacle::DrawDebug() const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    gfxDevice.SetPenColor(1.f, 1.f, 1.f, 1.f);
    MOAIDraw::DrawEllipseFill(m_position.mX, m_position.mY, m_radius, m_radius, 10);
}
