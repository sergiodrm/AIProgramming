#include "stdafx.h"
#include "SetImageBehavior.h"

#include "character.h"
#include "BehaviorTree/BehaviorTree.h"


CSetImageBehavior::CSetImageBehavior(CBehaviorTree* _owner)
    : CBehavior(_owner), m_imageId(EImageId::Idle) {}

EStatus CSetImageBehavior::OnUpdate(float _deltaTime)
{
    if (m_owner && m_owner->GetOwner())
    {
        m_owner->GetOwner()->SetImage(static_cast<int>(m_imageId));
        return EStatus::Success;
    }
    return EStatus::Fail;
}
