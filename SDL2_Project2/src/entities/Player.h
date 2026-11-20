#pragma once
#include "Entity.h"

namespace myGame {
	class Player : public Entity {
		vector<Entity*>& enemies;
	public:
		Player(int x, int y, EntityData* data, vector<Collide*>& ground, vector<Entity*>& enemies, SDL_Renderer* r);
		virtual ~Player();

		virtual void attack() override;
		virtual void act(Uint32 delay) override;
	};
}