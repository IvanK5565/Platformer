#include "Player.h"

namespace myGame {
  Player::Player(int x, int y, EntityData* data, vector<Collide*>& ground, vector<Entity*>& enemies, SDL_Renderer* r)
		: Entity(x, y, 100, 50, data, ground, r), enemies{ enemies } {
		int w = data->width;
		int h = data->height;
		states[States::Attack] = new AttackState(this, w, h, &data->attack, data->type, r);
		states[States::Death] = new DeathState(this, w, h, &data->death, data->type, r);
		states[States::Hurt] = new HurtState(this, w, h, &data->hurt, data->type, r);
		states[States::Idle] = new IdleState(this, w, h, &data->idle, data->type, r);
		states[States::Jump] = new JumpState(this, w, h, &data->jump, data->type, r);
		states[States::Run] = new RunState(this, w, h, &data->run, data->type, r);
	}

	Player::~Player() {
		delete states[States::Attack];
		delete states[States::Death];
		delete states[States::Hurt];
		delete states[States::Idle];
		delete states[States::Jump];
		delete states[States::Run];
	}

	void Player::attack()
	{
		if (lookDir == Direction::Left)
			attackBox.setPos(x - attackBox.width, y - hitBox.height / 2 - attackBox.height / 2);
		else
			attackBox.setPos(x, y - hitBox.height / 2 - attackBox.height / 2);
		for (Entity* enemy : enemies) {
			if (attackBox.checkCollision(enemy->getHitBox())) {
				enemy->takeDamage(damage, lookDir);
			}
		}
	}

	void Player::act(Uint32 delay) {
		float oldX = x;
		states[currentState]->act(delay);
		if (oldX < x) {
			lookDir = Direction::Right;
		}
		else if (oldX > x) {
			lookDir = Direction::Left;
		}

		y += speedY * delay / 1000;

		collissionUpdate();

		if (!onGround()) {
			speedY += (float)(delay * GRAVITY) / 1000;

			speedY = std::min((float)MAXSPEEDY, speedY);

			if(currentState != States::Hurt)
				setState(States::Jump);
		}
		else {
			speedY = 0;
		}
		if (isDead()) setState(States::Death);
	}
}