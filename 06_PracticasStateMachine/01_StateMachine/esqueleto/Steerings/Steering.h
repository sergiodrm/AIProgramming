#pragma once


class Character;

struct SSteeringResult
{
    USVec2D Linear;
    float Angular;

    SSteeringResult(const USVec2D& linear = USVec2D(0.f, 0.f), float angular = 0.f)
        : Linear(linear), Angular(angular) {}

    SSteeringResult operator+(const SSteeringResult& other)
    {
        SSteeringResult result;
        result.Linear = Linear + other.Linear;
        result.Angular = Angular + other.Angular;
        return result;
    }

    void operator+=(const SSteeringResult& other)
    {
        Linear += other.Linear;
        Angular += other.Angular;
    }
};

class CSteering
{
public:
    CSteering(Character* character)
        : m_character(character) {}

    virtual ~CSteering() {}

    virtual void OnDestroy() {}
    virtual const SSteeringResult& GetSteering(const USVec2D& _target) { return m_steering; }
    virtual const SSteeringResult& GetSteering(float _target) { return m_steering; }
    virtual void DrawDebug() const = 0;

protected:
    SSteeringResult m_steering;
    Character* m_character {nullptr};
};
