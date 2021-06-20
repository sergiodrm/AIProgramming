#include "stdafx.h"
#include "Params.h"
#include "tinyxml.h"

void SParams::Load(const char* _filename)
{
    TiXmlDocument doc(_filename);
    if (!doc.LoadFile())
    {
        printf("Couldn't read %s\n", _filename);
        return;
    }

    TiXmlHandle handleDoc(&doc);
    TiXmlElement* element = handleDoc.FirstChildElement().Element();
    if (!element)
    {
        printf("Invalid format\n");
        return;
    }

    TiXmlHandle handleRoot(element);
    TiXmlHandle handleParams = handleRoot.FirstChildElement("params");

    MaxLinearVelocity = GetValueFromXML("max_linear_velocity", handleParams);
    MaxLinearAcceleration = GetValueFromXML("max_linear_acceleration", handleParams);
    LinearDestRadius = GetValueFromXML("linear_dest_radius", handleParams);
    LinearArriveRadius = GetValueFromXML("linear_arrive_radius", handleParams);

    MaxAngularVelocity = GetValueFromXML("max_angular_velocity", handleParams);
    MaxAngularAcceleration = GetValueFromXML("max_angular_acceleration", handleParams);
    AngularDestRadius = GetValueFromXML("angular_dest_radius", handleParams);
    AngularArriveRadius = GetValueFromXML("angular_arrive_radius", handleParams);

    LookAhead = GetValueFromXML("look_ahead", handleParams);
    TimeAhead = GetValueFromXML("time_ahead", handleParams);
}

float SParams::GetValueFromXML(const char* _field, const TiXmlHandle& _handle) const
{
    TiXmlElement* element = _handle.FirstChildElement(_field).Element();
    if (element)
    {
        float x;
        element->Attribute("value", &x);
        return x;
    }
    return -1.f;
}
