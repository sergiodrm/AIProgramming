#pragma once


class ICondition
{
public:
    virtual bool Check() const = 0;
    virtual void DrawDebug() const {}
};
