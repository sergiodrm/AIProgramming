#include "stdafx.h"
#include "BehaviorTree.h"

#include <tinyxml.h>

#include "Behavior.h"
#include "character.h"
#include "Group.h"
#include "Selector.h"
#include "Sequence.h"
#include "Behaviors/AttackBehavior.h"
#include "Behaviors/ChaseBehavior.h"
#include "Behaviors/SetImageBehavior.h"
#include "Behaviors/WaitBehavior.h"
#include "Conditions/AndCondition.h"
#include "Conditions/CheckTargetDistanceCondition.h"
#include "Conditions/HitCondition.h"
#include "Conditions/OrCondition.h"
#include "Conditions/IsTargetAliveCondition.h"
#include "Conditions/NotCondition.h"

CBehaviorTree::CBehaviorTree(Character* _owner)
    : m_owner(_owner), m_root(nullptr) {}

void CBehaviorTree::Load(const char* _file)
{
    TiXmlDocument doc(_file);
    if (!doc.LoadFile())
    {
        fprintf(stderr, "Couldn't read values from %s", _file);
        return;
    }

    TiXmlHandle hDoc(&doc);

    TiXmlElement* pElem;
    pElem = hDoc.FirstChildElement().Element();
    if (!pElem)
    {
        fprintf(stderr, "Invalid format for %s", _file);
        return;
    }

    TiXmlHandle hBehaviorTree(pElem);
    TiXmlHandle hRootNode = hBehaviorTree.FirstChildElement();

    m_root = ReadGroupFromXml(hRootNode.Element());
}

void CBehaviorTree::Tick(float _deltaTime)
{
    if (m_root)
    {
        m_root->Tick(_deltaTime);
    }
}

void CBehaviorTree::DrawDebug() const
{
    if (m_root)
    {
        m_root->DrawDebug();
    }
}

Character* CBehaviorTree::GetOwner() { return m_owner; }

CGroup* CBehaviorTree::ReadGroupFromXml(TiXmlElement* _groupElement)
{
    const char* typeStr = _groupElement->Attribute("type");
    CGroup* group = nullptr;
    if (strcmp(typeStr, "sel") == 0)
    {
        group = new CSelector(this);
    }
    else if (strcmp(typeStr, "seq") == 0)
    {
        group = new CSequence(this);
    }

    for (TiXmlElement* child = _groupElement->FirstChildElement(); child != nullptr; child = child->
         NextSiblingElement())
    {
        const char* nodeStr = child->Value();

        if (strcmp(nodeStr, "group") == 0)
        {
            CGroup* childGroup = ReadGroupFromXml(child);
            group->AddBehavior(childGroup);
        }
        else if (strcmp(nodeStr, "condition") == 0)
        {
            ICondition* condition = ReadConditionFromXml(child);
            group->SetCondition(condition);
        }
        else if (strcmp(nodeStr, "behavior") == 0)
        {
            CBehavior* behavior = ReadBehaviorFromXml(child);
            group->AddBehavior(behavior);
        }
    }
    return group;
}

CBehavior* CBehaviorTree::ReadBehaviorFromXml(TiXmlElement* _behaviorElement)
{
    const char* typeStr = _behaviorElement->Attribute("type");

    if (strcmp(typeStr, "Attack") == 0)
    {
        CAttackBehavior* behavior = new CAttackBehavior(this);
        const float damage = atof(_behaviorElement->Attribute("damage"));
        behavior->SetDamage(damage);
        return behavior;
    }
    if (strcmp(typeStr, "Chase") == 0)
    {
        CChaseBehavior* behavior = new CChaseBehavior(this);
        const float dist = atof(_behaviorElement->Attribute("dist"));
        behavior->SetDistance(dist);
        return behavior;
    }
    if (strcmp(typeStr, "SetImage") == 0)
    {
        CSetImageBehavior* behavior = new CSetImageBehavior(this);
        const EImageId id = ImageIdFromStr(_behaviorElement->Attribute("id"));
        behavior->SetImage(id);
        return behavior;
    }
    if (strcmp(typeStr, "Wait") == 0)
    {
        CWaitBehavior* behavior = new CWaitBehavior(this);
        const float time = atof(_behaviorElement->Attribute("time"));
        behavior->SetTime(time);
        return behavior;
    }

    return nullptr;
}

ICondition* CBehaviorTree::ReadConditionFromXml(TiXmlElement* _conditionElement)
{
    const char* conditionStr = _conditionElement->Attribute("cond");
    if (strcmp(conditionStr, "And") == 0)
    {
        CAndCondition* condition = new CAndCondition();

        for (TiXmlElement* conditionIt = _conditionElement->FirstChildElement(); conditionIt != nullptr; conditionIt =
             conditionIt->NextSiblingElement())
        {
            ICondition* childCondition = ReadConditionFromXml(conditionIt);
            condition->AddCondition(childCondition);
        }
        return condition;
    }
    if (strcmp(conditionStr, "Or") == 0)
    {
        COrCondition* condition = new COrCondition();

        for (TiXmlElement* conditionIt = _conditionElement->FirstChildElement(); conditionIt != nullptr; conditionIt =
             conditionIt->NextSiblingElement())
        {
            ICondition* childCondition = ReadConditionFromXml(conditionIt);
            condition->AddCondition(childCondition);
        }
        return condition;
    }
    if (strcmp(conditionStr, "Not") == 0)
    {
        CNotCondition* condition = new CNotCondition(ReadConditionFromXml(_conditionElement->FirstChildElement()));
        return condition;
    }
    if (strcmp(conditionStr, "TargetAlive") == 0)
    {
        CIsAliveCondition* condition = new CIsAliveCondition(m_owner->GetTarget());
        return condition;
    }
    if (strcmp(conditionStr, "Alive") == 0)
    {
        CIsAliveCondition* condition = new CIsAliveCondition(m_owner);
        return condition;
    }
    if (strcmp(conditionStr, "Hit") == 0)
    {
        CHitCondition* condition = new CHitCondition(m_owner);
        return condition;
    }
    if (strcmp(conditionStr, "CheckDist") == 0)
    {
        CCheckTargetDistanceCondition* condition = new CCheckTargetDistanceCondition(m_owner);
        condition->SetTriggerDistance(atof(_conditionElement->Attribute("dist")));
        return condition;
    }
    return nullptr;
}
