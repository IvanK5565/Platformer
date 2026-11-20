#include "Enemy.h"

namespace myGame {
  Enemy::Enemy(int x, int y, EntityData* data, vector<Collide*>& ground, Player* player, SDL_Renderer* r)
		: Entity(x, y, 100, 25, data, ground, r), player{ player }, targetSpotted{ false }
	{
		int w = data->width;
		int h = data->height;
		states[States::Attack] = new EnemyAttackState(this, w, h, &data->attack, data->type, r);
		states[States::Death] = new EnemyDeathState(this, w, h, &data->death, data->type, r);
		states[States::Hurt] = new EnemyHurtState(this, w, h, &data->hurt, data->type, r);
		states[States::Idle] = new EnemyIdleState(this, w, h, &data->idle, data->type, r);
		states[States::Jump] = new EnemyJumpState(this, w, h, &data->jump, data->type, r);
		states[States::Run] = new EnemyRunState(this, w, h, &data->run, data->type, r);
	}

	Enemy::~Enemy() {
		delete states[States::Attack];
		delete states[States::Death];
		delete states[States::Hurt];
		delete states[States::Idle];
		delete states[States::Jump];
		delete states[States::Run];
	}

	void Enemy::move(Direction dir, Uint32 delay)
	{
		float dop = (float)MAXSPEEDX * (float)delay * 0.0005;
		switch (dir)
		{
		case myGame::Direction::Right:
			x += dop;
			break;
		case myGame::Direction::Left:
			x -= dop;
			break;
		default:
			break;
		}
	}

	void Enemy::attack()
	{
		if (lookDir == Direction::Left)
			attackBox.setPos(x - attackBox.width, y - hitBox.height / 2 - attackBox.height / 2);
		else
			attackBox.setPos(x, y - hitBox.height / 2 - attackBox.height / 2);
		if (attackBox.checkCollision(player->getHitBox())) {
			player->takeDamage(damage, lookDir);
		}

	}
	void Enemy::act(Uint32 delay) {
		if (isDead()) { 
			setState(States::Death);
		}
		else {
			int distanceToPlayerX = player->getX() - getX();
			if (abs(distanceToPlayerX) < 100) {
				targetSpotted = true;
				if (distanceToPlayerX < 0) lookDir = Direction::Left;
				else if (distanceToPlayerX > 0) lookDir = Direction::Right;
			}
			else {
				targetSpotted = false;
			}

			if (currentState == States::Hurt || currentState == States::Death || currentState == States::Attack) {
				targetSpotted = false;
			}
			else {
				bool onGroundTemp = onGround();
				if (onGroundTemp) {
					//to idle:
					if (!targetSpotted || player->isDead()) {
						setState(States::Idle);
					}
					else {
						//to attack:
						if (abs(distanceToPlayerX) < attackBox.getRect().w) {
							setState(States::Attack);
						}
						//to run:
						else setState(States::Run);
					}
				}
				//to jump:
				else {
					setState(States::Jump);
					if (targetSpotted) {
						move(lookDir, delay);
					}
				}
			}
		}
		states[currentState]->act(delay);

		if (!onGround()) {
			speedY += (float)(delay * GRAVITY) / 1000;

			speedY = std::min((float)MAXSPEEDY, speedY);
		}

		y += speedY * delay / 1000;

		collissionUpdate();
	}
}