#ifndef BIRD_H
#define BIRD_H

#include "game_object/GameObjectLabel.h"
#include "game_object/GameObject.h"
#include "level/LevelData.h"
#include "texture/Texture.h"
#include "animation/player/AnimPlayer.h"
#include <memory>

class BirdObject : public GameObject
{
	public:

		BirdObject(Texture* tex, 
				   int shader, 
				   const float posX,
				   const float posY,
				   int scale,
				   GameObjectLabel& gameLabel);
		~BirdObject();

		BirdObject(const BirdObject& copy) = delete;
		BirdObject& operator = (const BirdObject& copy) = delete;
		
		std::unique_ptr<GameObject> Clone() override;

		void Draw();
		void PreDraw();
		void Update();

	private:

		void Init();

	private:

		AnimPlayer* m_anim_player;
};



#endif