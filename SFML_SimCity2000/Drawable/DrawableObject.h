#pragma once
enum class DataType
{
	Sprite,
	Animation,
	Text,
	Shape,
	None
};

enum class OriginType
{
	TL = 0, TC = 1, TR = 2,
	ML = 3, MC = 4, MR = 5,
	BL = 6, BC = 7, BR = 8,
	Custom
};

enum class DrawPriorityType
{
	X,
	Y,
	Custom,
};

class DebugInfo;
class DrawableObject : public Transform
{
protected:
	DrawableObject(DataType datatype, sf::Drawable* drawable, sf::Transformable* transform, bool debug = true);
	DrawableObject(const DrawableObject& other, sf::Drawable* drawable, sf::Transformable* transform);
	DrawableObject(DrawableObject&& other, sf::Drawable* drawable, sf::Transformable* transform);

public:
	virtual ~DrawableObject();

	void SetPriorityType(DrawPriorityType type, float value = 0);
	float GetPriorityValue() const;
	static bool PriorityCompare(DrawableObject*& lhs, DrawableObject*& rhs)
	{
		return  lhs->GetPriorityValue() > rhs->GetPriorityValue();
	}

	bool GetIsValid()const { return m_IsValid; }

	virtual void Update(float dt) {};
	virtual void ImGuiUpdate() {};

	sf::Drawable* GetDrawable()const { return m_Drawable; }
	DebugInfo* GetDebugDraw();

	void SetDebugDraw(bool debug);

	std::string GetName()const { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }

	void SetOriginCenter();
	virtual void SetOrigin(OriginType type, const sf::Vector2f& detail = sf::Vector2f(0, 0));
	sf::Vector2f GetBoundPointPosition(int index) const;

	virtual sf::Vector2u GetTextureSize()const;
	virtual sf::FloatRect GetGlobalBounds()const = 0;
	virtual sf::FloatRect GetLocalBounds()const = 0;

	virtual sf::Color GetColor() const = 0;
	virtual void SetColor(const sf::Color& color) = 0;
	virtual void SetColor(int r, int g, int b, int a = 255) = 0;
	virtual sf::Color GetFillColor() const = 0;
	virtual sf::Color GetOutlineColor() const = 0;
	virtual void SetFillColor(const sf::Color& color) = 0;
	virtual void SetFillColor(int r, int g, int b, int a = 255) = 0;
	virtual void SetOutlineColor(const sf::Color& color) = 0;
	virtual void SetOutlineColor(int r, int g, int b, int a = 255) = 0;

	const DataType m_DataType;
protected:
	bool					m_IsValid;
	std::string				m_Name;
	sf::Drawable* m_Drawable;
	sf::Transformable* m_Transform;

	DrawPriorityType	m_PriorityType = DrawPriorityType::Y;
	float				m_PriorityValue;
private:


	DebugInfo* m_DebugInfo;
};
