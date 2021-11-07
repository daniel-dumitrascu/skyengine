#pragma once
#include "entities/GameObject.h"
#include "Rectangle.h"
#include <memory>

class GameRectangle : public GameObject
{
	public:
		GameRectangle(vec_2x& recLeftTop, vec_2x& recRightBottom, const vec_4x& color, const std::string& id);
		~GameRectangle();

		std::unique_ptr<GameObject> Clone() override;

		void SetColor(const vec_4x& color);
		const vec_4x& GetColor() { return m_color; }

		void Draw();
		void PreDraw();
		void Update();

	private:
		void Init();

	private:
		vec_4x	m_color;
};