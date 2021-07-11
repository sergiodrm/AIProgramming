#pragma once

class ICondition;
class CBehavior;
class TiXmlElement;
class CGroup;
class Character;

class CBehaviorTree
{
public:
    CBehaviorTree(Character* _owner);

    void Load(const char* _file);
    void Tick(float _deltaTime);
    Character* GetOwner();

protected:
    CGroup* ReadGroupFromXml(TiXmlElement* _groupElement);
    CBehavior* ReadBehaviorFromXml(TiXmlElement* _behaviorElement);
    ICondition* ReadConditionFromXml(TiXmlElement* _conditionElement);
private:
    Character* m_owner;
    CGroup* m_root;
};
