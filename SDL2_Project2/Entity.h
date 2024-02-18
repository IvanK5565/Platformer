#pragma once
#include "Drawable.h"
#include "Enums.h"

namespace myGame {
	class Entity : public Drawable {
	protected:
		float x, y;
		static const int GRAVITY = 100;
		static const int MAXSPEEDY = 100;
		static const int MAXSPEEDX = 100;
		float speedY;
		int health, damage;
		//bool immobilized;
		Direction lookDir;
		Direction hurtDir;
		//states
		map<States, State*> states;
		States currentState;
		//collides
		vector<Collide*>& walls;

		Square hitBox;
		Square attackBox;

	public:
		Entity(int, int, int, int, EntityData*, vector<Collide*>&, SDL_Renderer*);
		virtual ~Entity();

		float getX() const;
		float getY() const;
		float getSpeedY() const;
		void setSpeedY(float);
		Direction getLookDir();
		Direction getHurtDir();
		bool isDead();
		void setState(States);
		virtual void move(Direction, Uint32);
		void jump();
		void takeDamage(int, Direction);
		virtual void attack() = 0;
		Square& getHitBox();
		bool onGround();
		void collissionUpdate();

		virtual SDL_Rect getRect() override;
		virtual SDL_Point getFocus() override;
		virtual void render(Camera& camera, SDL_Renderer* renderer) override;
	};

	class Player : public Entity {
		vector<Entity*>& enemies;
	public:
		Player(int x, int y, EntityData* data, vector<Collide*>& ground, vector<Entity*>& enemies, SDL_Renderer* r);
		virtual ~Player();

		virtual void attack() override;
		virtual void act(Uint32 delay) override;
		//virtual void render(Camera& camera, SDL_Renderer* renderer) override;
	};

	class Enemy : public Entity {
		Player* player;
		bool targetSpotted;
	public:
		Enemy(int x, int y, EntityData* data, vector<Collide*>& ground, Player* player, SDL_Renderer* r);
		virtual ~Enemy();


		virtual void move(Direction dir, Uint32 delay) override;
		virtual void attack() override;
		virtual void act(Uint32 delay) override;

		//virtual void render(Camera& camera, SDL_Renderer* renderer) override;
	};
}