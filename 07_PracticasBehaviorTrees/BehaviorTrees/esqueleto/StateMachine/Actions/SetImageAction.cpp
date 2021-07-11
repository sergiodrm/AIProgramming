#include "stdafx.h"
#include "SetImageAction.h"
#include "character.h"
#include "StateMachine/StateMachine.h"

EImageId ImageIdFromStr(const char* _str)
{
    if (_str)
    {
        if (strcmp(_str, "Idle") == 0)
        {
            return EImageId::Idle;
        }
        if (strcmp(_str, "Attack") == 0)
        {
            return EImageId::Attack;
        }
        if (strcmp(_str, "Alarm") == 0)
        {
            return EImageId::Alarm;
        }
        if (strcmp(_str, "Dead") == 0)
        {
            return EImageId::Dead;
        }
        if (strcmp(_str, "Hit") == 0)
        {
            return EImageId::Hit;
        }
    }
    return EImageId::None;
}

CSetImageAction::CSetImageAction(CStateMachine* _ownerStateMachine)
    : CAction(_ownerStateMachine), m_imageId(EImageId::Idle) {}

void CSetImageAction::OnStart()
{
    if (m_ownerStateMachine)
    {
        m_ownerStateMachine->GetOwner()->SetImage(static_cast<int>(m_imageId));
    }
}

void CSetImageAction::SetImage(EImageId _id) { m_imageId = _id; }
