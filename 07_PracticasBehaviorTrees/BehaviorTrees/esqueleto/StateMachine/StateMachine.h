#pragma once
#include <vector>

class ICondition;
class TiXmlElement;
class CAction;
class Character;
class CState;
typedef std::vector<CState*> StateVector;

class CStateMachine
{
public:
    CStateMachine(Character* _owner);
    ~CStateMachine();

    void Load(const char* _filename);
    void Start();
    void Update(float _deltaTime);
    void DrawDebug();
    const Character* GetOwner() const { return m_owner; }
    Character* GetOwner() { return m_owner; }

protected:
    CAction* ReadActionFromXml(TiXmlElement* _actionElement);
    ICondition* ReadConditionFromXml(TiXmlElement* _conditionElement);

private:
    StateVector m_states;
    CState* m_currentState;
    Character* m_owner;
};
