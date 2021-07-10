#pragma once

class CAction
{
public:
    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnEnd() {}
};
