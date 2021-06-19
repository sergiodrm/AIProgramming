#include <stdafx.h>

#include "NavMesh.h"
#include <tinyxml.h>

CNavMesh::CNavMesh()
{
}

CNavMesh::~CNavMesh()
{
}

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

void CNavMesh::DrawDebug() const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    for (const SPolygon* polygon : m_polygons)
    {
        gfxDevice.SetPenColor(0.1f, 0.1f, 0.4f, 0.5f);
        MOAIDraw::DrawPolygonFilled(polygon->Vertex);
        gfxDevice.SetPenColor(0.7f, 0.1f, 0.2f, 0.5f);
        MOAIDraw::DrawPolygon(polygon->Vertex);

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
