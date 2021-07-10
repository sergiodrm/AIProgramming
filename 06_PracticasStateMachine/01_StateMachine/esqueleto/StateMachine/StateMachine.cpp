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


    ///**
    // * Conditions
    // */
    //CCheckTargetDistanceCondition* idleToChaseCondition = new CCheckTargetDistanceCondition(m_owner);
    //CCheckTargetDistanceCondition* chaseToIdleCondition = new CCheckTargetDistanceCondition(m_owner);
    //CCheckTargetDistanceCondition* chaseToAttackCondition = new CCheckTargetDistanceCondition(m_owner);
    //chaseToAttackCondition->SetTriggerDistance(100.f);
    //CCheckTargetDistanceCondition* attackToChaseCondition = new CCheckTargetDistanceCondition(m_owner);
    //attackToChaseCondition->SetTriggerDistance(100.f);

    ///**
    // * Actions
    // */
    //CSetImageAction* setImageToAlarmAction = new CSetImageAction(this);
    //setImageToAlarmAction->SetImage(EImageId::Alarm);
    //CSetImageAction* setImageToIdleAction = new CSetImageAction(this);
    //setImageToIdleAction->SetImage(EImageId::Idle);
    //CSetImageAction* setImageToAttackAction = new CSetImageAction(this);
    //setImageToAttackAction->SetImage(EImageId::Attack);
    //CChaseAction* chaseTargetAction = new CChaseAction(this);
    //CAttackAction* attackAction = new CAttackAction(this);

    ///** Idle state */
    //CState* idleState = new CState();
    //idleState->AddEnterAction(setImageToIdleAction);
    //m_states.push_back(idleState);

    ///** Chase state */
    //CState* chaseState = new CState();
    //chaseState->AddStateAction(chaseTargetAction);
    //chaseState->AddEnterAction(setImageToAlarmAction);
    //m_states.push_back(chaseState);

    ///** Attack state */
    //CState* attackState = new CState();
    //attackState->AddStateAction(attackAction);
    //attackState->AddEnterAction(setImageToAttackAction);

    ///** Transition: idle -> chase */
    //CTransition* idleToChase = new CTransition();
    //idleToChase->SetCondition(idleToChaseCondition);
    //idleToChase->SetTargetState(chaseState);
    //// Add transition to idle state
    //idleState->AddTransition(idleToChase);

    ///** Transition: chase -> idle */
    //CTransition* chaseToIdle = new CTransition();
    //chaseToIdle->SetCondition(new CNotCondition(chaseToIdleCondition));
    //chaseToIdle->SetTargetState(idleState);
    //// Add transition to chase state
    //chaseState->AddTransition(chaseToIdle);

    ///** Transition: chase -> attack */
    //CTransition* chaseToAttack = new CTransition();
    //chaseToAttack->SetCondition(chaseToAttackCondition);
    //chaseToAttack->SetTargetState(attackState);
    //chaseState->AddTransition(chaseToAttack);

    ///** Transition: chase -> attack */
    //CTransition* attackToChase = new CTransition();
    //attackToChase->SetCondition(new CNotCondition(attackToChaseCondition));
    //attackToChase->SetTargetState(chaseState);
    //attackState->AddTransition(attackToChase);
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
    const char* conditionStr = _conditionElement->Value();
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
    if (strcmp(conditionStr, "Alive") == 0)
    {
        CIsTargetAliveCondition* condition = new CIsTargetAliveCondition(m_owner->GetTarget());
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
