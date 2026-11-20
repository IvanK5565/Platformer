#pragma once
#include "Entity.h"
#include "Player.h"

namespace myGame {
	class Enemy : public Entity {
		Player* player;
		bool targetSpotted;
	public:
		Enemy(int x, int y, EntityData* data, vector<Collide*>& ground, Player* player, SDL_Renderer* r);
		virtual ~Enemy();


		virtual void move(Direction dir, Uint32 delay) override;
		virtual void attack() override;
		virtual void act(Uint32 delay) override;
	};
}