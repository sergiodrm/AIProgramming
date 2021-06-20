#pragma once

class TiXmlHandle;

struct SParams
{
    float MaxLinearVelocity {0.f};
    float MaxLinearAcceleration {0.f};
    float LinearDestRadius {0.f};
    float LinearArriveRadius {0.f};

    float MaxAngularVelocity {0.f};
    float MaxAngularAcceleration {0.f};
    float AngularDestRadius {0.f};
    float AngularArriveRadius {0.f};

    float LookAhead {0.f};
    float TimeAhead {0.f};

    void Load(const char* _filename);
private:
    float GetValueFromXML(const char* _field, const TiXmlHandle& _handle) const;
};
