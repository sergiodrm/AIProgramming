#include "stdafx.h"
#include "StateMachine.h"

#include <tinyxml.h>

#include "character.h"
#include "State.h"
#include "Transition.h"
#include "Actions/AttackAction.h"
#include "Actions/ChaseAction.h"
#include "Actions/SetImageAction.h"
#include "Conditions/AndCondition.h"
#include "Conditions/CheckTargetDistanceCondition.h"
#include "Conditions/HitCondition.h"
#include "Conditions/IsTargetAliveCondition.h"
#include "Conditions/NotCondition.h"
#include "Conditions/OrCondition.h"

CStateMachine::CStateMachine(Character* _owner)
    : m_currentState(nullptr), m_owner(_owner)
{}

CStateMachine::~CStateMachine()
{
    for (CState* it : m_states)
    {
        delete it;
    }
}

void CStateMachine::Load(const char* _filename)
{
    if (!_filename)
    {
        return;
    }

    TiXmlDocument doc(_filename);
    if (!doc.LoadFile())
    {
        printf("Couldn't read state machine file\n");
        return;
    }

    TiXmlHandle docHandle(&doc);
    TiXmlElement* rootElement = docHandle.FirstChildElement().Element();
    if (!rootElement)
    {
        printf("Invalid format\n");
        return;
    }

    TiXmlHandle handle(rootElement);

    TiXmlElement* states = handle.FirstChildElement("states").Element();
    for (TiXmlElement* stateIterator = states->FirstChildElement(); stateIterator != nullptr; stateIterator =
         stateIterator->NextSiblingElement())
    {
        CState* state = new CState();
        m_states.push_back(state);

        for (TiXmlElement* actionIterator = stateIterator->FirstChildElement(); actionIterator != nullptr;
             actionIterator = actionIterator->NextSiblingElement())
        {
            CAction* action = ReadActionFromXml(actionIterator);

            if (action)
            {
                if (strcmp(actionIterator->Value(), "enter") == 0)
                {
                    state->AddEnterAction(action);
                }
                else if (strcmp(actionIterator->Value(), "loop") == 0)
                {
                    state->AddStateAction(action);
                }
                else if (strcmp(actionIterator->Value(), "exit") == 0)
                {
                    state->AddExitAction(action);
                }
            }
        }
    }

    TiXmlElement* transitions = handle.FirstChildElement("transitions").Element();
    for (TiXmlElement* transitionIterator = transitions->FirstChildElement(); transitionIterator != nullptr;
         transitionIterator = transitionIterator->NextSiblingElement())
    {
        ICondition* condition = ReadConditionFromXml(transitionIterator->FirstChildElement("condition"));
        if (condition)
        {
            CTransition* transition = new CTransition();
            const int from = atoi(transitionIterator->Attribute("origin"));
            const int to = atoi(transitionIterator->Attribute("dest"));
            CState* stateFrom = m_states.at(from);
            CState* stateTo = m_states.at(to);
            stateFrom->AddTransition(transition);
            transition->SetCondition(condition);
            transition->SetTargetState(stateTo);
        }
    }
}

void CStateMachine::Start()
{
    if (!m_states.empty())
    {
        m_currentState = m_states.at(0);
        m_currentState->OnEnter();
    }
}

void CStateMachine::Update(float _deltaTime)
{
    if (m_currentState)
    {
        // Update current state
        m_currentState->OnUpdate(_deltaTime);

        // Check transitions
        for (CTransition* it : m_currentState->GetTransition())
        {
            if (it->CanTrigger())
            {
                m_currentState->OnExit();
                m_currentState = it->Trigger();
                m_currentState->OnEnter();
                return;
            }
        }
    }
}

void CStateMachine::DrawDebug()
{
    m_currentState->DrawDebug();
    for (CTransition* it : m_currentState->GetTransition())
    {
        it->DrawDebug();
    }
}

CAction* CStateMachine::ReadActionFromXml(TiXmlElement* _actionElement)
{
    const char* actionStr = _actionElement->Attribute("action");
    if (strcmp(actionStr, "SetImage") == 0)
    {
        CSetImageAction* action = new CSetImageAction(this);
        const char* imageStr = _actionElement->Attribute("image");
        EImageId imageId = ImageIdFromStr(imageStr);
        action->SetImage(imageId);
        return action;
    }
    if (strcmp(actionStr, "Attack") == 0)
    {
        CAttackAction* action = new CAttackAction(this);
        action->SetTimeBetweenAttacks(atof(_actionElement->Attribute("time")));
        action->SetDamage(atof(_actionElement->Attribute("damage")));
        return action;
    }
    if (strcmp(actionStr, "Chase") == 0)
    {
        CChaseAction* action = new CChaseAction(this);
        return action;
    }
    return nullptr;
}

ICondition* CStateMachine::ReadConditionFromXml(TiXmlElement* _conditionElement)
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
