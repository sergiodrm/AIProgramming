#include "stdafx.h"
#include "Steering.h"

CSteering::~CSteering()
{
    delete m_delegate;
}

SSteeringResult CSteering::CallDelegate() const
{
    if (m_delegate)
    {
        return m_delegate->GetSteering();
    }
    return SSteeringResult({0.f, 0.f}, 0.f);
}
