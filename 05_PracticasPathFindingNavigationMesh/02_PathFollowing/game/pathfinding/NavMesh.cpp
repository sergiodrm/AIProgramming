#include <stdafx.h>

#include "NavMesh.h"
#include <tinyxml.h>

/**
 * ****************************************************************
 * Polygon
 * ****************************************************************
 */

USVec2D SPolygon::GetCenter() const
{
    USVec2D center(0.f, 0.f);

    float area = 0.f;
    for (size_t i = 0; i < Vertex.size(); ++i)
    {
        USVec2D vertex1(Vertex.at(i).mX, Vertex.at(i).mY);
        USVec2D vertex2(Vertex.at((i + 1) % Vertex.size()).mX, Vertex.at((i + 1) % Vertex.size()).mY);

        float a = (vertex1.mX * vertex2.mY) - (vertex2.mX * vertex1.mY);
        area += a;

        center.mX += (vertex1.mX + vertex2.mX) * a;
        center.mY += (vertex1.mY + vertex2.mY) * a;
    }
    return center;
}

bool SPolygon::IsInside(const USVec2D& _point) const
{
    bool inside = false;
    for (int32_t i = 0, j = Vertex.size() - 1; i < Vertex.size(); j = i++)
    {
        USVec2D vertexi = Vertex.at(i);
        USVec2D vertexj = Vertex.at(j);
        if ((vertexi.mY > _point.mY != vertexj.mY > _point.mY) &&
            (_point.mX < (vertexj.mX - vertexi.mX) * (_point.mY - vertexi.mY) / (vertexj.mY - vertexi.mY) + vertexi.mX))
        {
            inside = !inside;
        }
    }
    return inside;
}

USVec2D SPolygon::GetMiddleEdgePoint(const SLink& _link) const
{
    const USVec2D start = Vertex.at(_link.EdgeStart);
    const USVec2D end = Vertex.at(_link.EdgeEnd);
    return start + (end - start) * 0.5f;
}

/**
 * ****************************************************************
 * CNavMesh
 * ****************************************************************
 */

CNavMesh::CNavMesh()
{}

CNavMesh::~CNavMesh()
{}

CNavMesh* CNavMesh::Load(const char* _filename)
{
    TiXmlDocument doc(_filename);
    if (!doc.LoadFile())
    {
        printf("Couldn't read %s\n", _filename);
        return nullptr;
    }
    TiXmlHandle handleDoc(&doc);
    TiXmlElement* element = handleDoc.FirstChildElement().Element();
    if (!element)
    {
        printf("Invalid format\n");
        return nullptr;
    }

    // Create the navigation mesh
    CNavMesh* navMesh = new CNavMesh();

    // Get the root of polygons
    TiXmlHandle handleRoot(element);
    TiXmlHandle handlePolygons = handleRoot.FirstChildElement("polygons");

    // Read polygons
    TiXmlElement* polygonElement = handlePolygons.FirstChildElement("polygon").Element();
    for (polygonElement; polygonElement; polygonElement = polygonElement->NextSiblingElement())
    {
        SPolygon* polygon = new SPolygon();
        TiXmlElement* pointElement = polygonElement->FirstChildElement("point");
        for (pointElement; pointElement; pointElement = pointElement->NextSiblingElement())
        {
            float x = 0.f;
            float y = 0.f;
            pointElement->Attribute("x", &x);
            pointElement->Attribute("y", &y);
            polygon->Vertex.push_back(USVec2D(x, y));
        }
        navMesh->m_polygons.push_back(polygon);
    }

    // Read links
    TiXmlHandle handleLink = handleRoot.FirstChildElement("links");
    TiXmlElement* linkElement = handleLink.FirstChildElement("link").Element();
    for (linkElement; linkElement; linkElement = linkElement->NextSiblingElement())
    {
        TiXmlElement* startElement = linkElement->FirstChildElement("start");
        int32_t polygonIndex = 0;
        int32_t edgeStart = 0;
        int32_t edgeEnd = 0;
        startElement->Attribute("polygon", &polygonIndex);
        startElement->Attribute("edgestart", &edgeStart);
        startElement->Attribute("edgeend", &edgeEnd);

        TiXmlElement* endElement = linkElement->FirstChildElement("end");
        int32_t neighbourIndex = 0;
        int32_t neighbourEdgeStart = 0;
        int32_t neighbourEdgeEnd = 0;
        endElement->Attribute("polygon", &neighbourIndex);
        endElement->Attribute("edgestart", &neighbourEdgeStart);
        endElement->Attribute("edgeend", &neighbourEdgeEnd);

        SPolygon* firstPoly = navMesh->m_polygons.at(polygonIndex);
        SPolygon* secondPoly = navMesh->m_polygons.at(neighbourIndex);

        SLink firstLink;
        firstLink.Neighbour = secondPoly;
        firstLink.EdgeStart = edgeStart;
        firstLink.EdgeEnd = edgeEnd;
        firstPoly->Links.push_back(firstLink);

        SLink secondLink;
        secondLink.Neighbour = firstPoly;
        secondLink.EdgeStart = neighbourEdgeStart;
        secondLink.EdgeEnd = neighbourEdgeEnd;
        secondPoly->Links.push_back(secondLink);
    }
    return navMesh;
}

const SPolygon* CNavMesh::GetPolygon(const USVec2D& _screenPosition) const
{
    for (const SPolygon* polygon : m_polygons)
    {
        if (polygon->IsInside(_screenPosition))
        {
            return polygon;
        }
    }
    return nullptr;
}

SPolygon* CNavMesh::GetPolygon(const USVec2D& _screenPosition)
{
    for (SPolygon* polygon : m_polygons)
    {
        if (polygon->IsInside(_screenPosition))
        {
            return polygon;
        }
    }
    return nullptr;
}

bool CNavMesh::SelectStartPolygon(const USVec2D& _screenPosition)
{
    m_startPolygon = GetPolygon(_screenPosition);
    return m_startPolygon != nullptr;
}

bool CNavMesh::SelectEndPolygon(const USVec2D& _screenPosition)
{
    m_endPolygon = GetPolygon(_screenPosition);
    return m_endPolygon != nullptr;
}

void CNavMesh::DrawDebug() const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    for (const SPolygon* polygon : m_polygons)
    {
        if (polygon == m_startPolygon)
        {
            gfxDevice.SetPenColor(0.8f, 0.8f, 0.4f, 0.5f);
            MOAIDraw::DrawPolygonFilled(polygon->Vertex);
            gfxDevice.SetPenColor(0.7f, 0.1f, 0.2f, 0.5f);
            MOAIDraw::DrawPolygon(polygon->Vertex);
        }
        else if (polygon == m_endPolygon)
        {
            gfxDevice.SetPenColor(0.1f, 0.9f, 0.9f, 0.5f);
            MOAIDraw::DrawPolygonFilled(polygon->Vertex);
            gfxDevice.SetPenColor(0.7f, 0.1f, 0.2f, 0.5f);
            MOAIDraw::DrawPolygon(polygon->Vertex);
        }
        else
        {
            gfxDevice.SetPenColor(0.1f, 0.1f, 0.4f, 0.5f);
            MOAIDraw::DrawPolygonFilled(polygon->Vertex);
            gfxDevice.SetPenColor(0.7f, 0.1f, 0.2f, 0.5f);
            MOAIDraw::DrawPolygon(polygon->Vertex);
        }

        gfxDevice.SetPenColor(0.4f, 0.1f, 0.6f, 0.4f);
        for (const SLink& link : polygon->Links)
        {
            USVec2D edgeStart = polygon->Vertex.at(link.EdgeStart);
            USVec2D edgeEnd = polygon->Vertex.at(link.EdgeEnd);

            USVec2D edgeCenter = edgeEnd - edgeStart;
            edgeCenter *= 0.5f;
            edgeCenter += edgeStart;
            MOAIDraw::DrawEllipseFill(edgeCenter.mX, edgeCenter.mY, 5.f, 5.f, 10);
        }
    }
}
