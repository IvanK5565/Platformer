#pragma once
#include "Drawable.h"
#include "Enums.h"

namespace myGame {
	class Entity : public Drawable {
	protected:
		float x, y;
		static const int GRAVITY   = 80;
		static const int MAXSPEEDX = 80;
		static const int MAXSPEEDY = 80;
		float speedX, speedY;
		int health, damage;
		//bool immobilized;
		Direction lookDir;
		//states
		map<States, State*> states;
		States currentState;
		//collides
		vector<Collide*>& collides;
		vector<Entity*>& enemies;

		Square hitBox;
		Square attackBox;

	public:
		Entity(int, int, int, int, EntityData*, vector<Collide*>&, vector<Entity*>&, SDL_Renderer*);
		virtual ~Entity();

		float getX() {
			return x;
		}
		float getY() {
			return y;
		}
		float getSpeedY() {
			return speedY;
		}
		float getSpeedX() {
			return speedX;
		}
		Direction getLook() {
			return lookDir;
		}
		void setPos(int x, int y) {
			this->x = x;
			this->y = y;
		}

		virtual void act(Uint32) = 0;

		void setState(States);
		void move(Direction);
		void stopMove(Direction);
		void jump();
		void idle();
		void startAttack();
		void takeDamage(int);
		void attack();
		Square& getHitBox();
		bool onGround() {
			if (speedY == 0) return true;
			return false;
		}
		Direction getXDir() {
			if (speedX > 0) {
				return Direction::Left;
			}
			else if (speedX < 0) {
				return Direction::Right;
			}
			else {
				return Direction::None;
			}
		}

		virtual SDL_Rect getRect() override;
		virtual SDL_Point getFocus() override;
	};

	class Player : public Entity {
		
	public:
		Player(int x, int y, EntityData* data, vector<Collide*>& ground, vector<Entity*>& enemies, SDL_Renderer* r)
			: Entity(x, y, 100, 10, data, ground, enemies, r) {}
		virtual ~Player() {}

		virtual void act(Uint32 delay) override {
			float oldX = x, oldY = y;

			float dy = (float)(delay * GRAVITY) / 1000;

			speedY += dy;
			speedY = std::min((float)MAXSPEEDY, speedY);
			speedY = std::max(-(float)MAXSPEEDY, speedY);

			x += speedX * delay / 1000;
			y += speedY * delay / 1000;
			std::cout << y << "  " << speedY << "  " << delay << std::endl;

			for (Collide* collide : collides) {
				if (CollisionInfo info = collide->checkCollision(getHitBox())) {
					int _x = std::abs(info.x1) < std::abs(info.x2) ? info.x1 : info.x2;
					int _y = std::abs(info.y1) < std::abs(info.y2) ? info.y1 : info.y2;

					if (std::abs(_x) < std::abs(_y)) {
						x += _x;
					}
					else {
						y += _y;
						speedY = 0;
					}
				}
			}


			if (!onGround() && currentState != States::Jump) {
				setState(States::Jump);
			}
			states[currentState]->act(delay);
		}
		virtual void render(Camera& camera, SDL_Renderer* renderer) override {
			states[currentState]->render(camera, renderer);

#ifdef DEBUG
			SDL_Rect pos = {x,y,1,1};
			if (SDL_HasIntersection(&pos, &camera.rect) == SDL_TRUE) {
				pos.x -= camera.rect.x;
				pos.y -= camera.rect.y;
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
				SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x + 10, pos.y);
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x, pos.y + 10);
			}
			hitBox.drawBorder(renderer, camera);
			attackBox.drawBorder(renderer, camera);
#endif // DEBUG
		}
		void handleEvent(SDL_Event& e) {
			states[currentState]->handleEvent(e);
		}
	};

	class Enemy : public Entity {
	public:
		Enemy(int x, int y, EntityData* data, vector<Collide*>& ground, vector<Entity*>& enemies, SDL_Renderer* r)
			: Entity(x, y, 100, 10, data, ground, enemies, r) {}
		virtual ~Enemy() {}

		virtual void act(Uint32) override {}

		virtual void render(Camera&, SDL_Renderer*) override {}
	};
}