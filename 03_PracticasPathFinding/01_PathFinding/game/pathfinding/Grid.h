#pragma once

struct SNode
{
    SNode* Parent;
    USVec2D Position;
    float CostH;
    float CostG;
    float GetCost() const { return CostH + CostG; }

    SNode(SNode* _parent, const USVec2D& _gridPosition, float _costG, float _costH)
        : Parent(_parent), Position (_gridPosition), CostG(_costG), CostH(_costH) {}
    SNode(const SNode*& _other)
        : Parent(_other->Parent), Position(_other->Position), CostG(_other->CostG), CostH(_other->CostH) {}
};

class CGrid
{
public:
    static CGrid* Load(const char* _mapFilename, const char* _codeFilename);

    CGrid();
    float GetCost(uint32_t _index) const;
    float GetCost(uint32_t _row, uint32_t _col) const;
    float GetCost(const USVec2D& _gridLocation) const;
    USVec2D GetSize() const;
    USVec2D GetRectSize() const;
    bool IsValidPosition(const USVec2D& _position) const;

    USVec2D GridToWorldLocation(const USVec2D& _gridLocation) const;
    USVec2D WorldToGridLocation(const USVec2D& _worldLocation) const;

    void DrawDebug() const;
    void DrawRectangle(const USVec2D& _gridPosition, const USVec4D& _color) const;
private:
    std::vector<float> m_map;
    USVec2D m_size;
};
