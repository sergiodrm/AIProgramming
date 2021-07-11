#pragma once

#include "Action.h"

enum class EImageId : int8_t
{
    None = -1,
    Idle,
    Alarm,
    Attack,
    Dead,
    Hit,
    WindUp,
    Max
};

EImageId ImageIdFromStr(const char* _str);

class CSetImageAction : public CAction
{
public:
    CSetImageAction(CStateMachine* _ownerStateMachine);

    virtual void OnStart() override;
    void SetImage(EImageId _id);
private:
    EImageId m_imageId;
};
