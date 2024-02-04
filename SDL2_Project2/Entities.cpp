#include "Entity.h"

namespace myGame {
	Entity::Entity(int x, int y, int health, int damage, EntityData* data, vector<Collide*>& collides, vector<Entity*>& enemies, SDL_Renderer* r) :
		x{ (float)x }, y{ (float)y }, health{ health }, damage{ damage }, collides{ collides }, enemies{ enemies }, currentState{ States::Idle },
		hitBox{x-data->hitW/2, y-data->hitH,data->hitW,data->hitH }, attackBox{ x, y - hitBox.height / 2 - attackBox.height / 2,data->attackW,data->attackH },
		lookDir{ Direction::Right }, speedX{ 0 }, speedY{ 0 }
	{
		int w = data->width;
		int h = data->height;

		states[States::Attack]	= new AttackState(this, w, h, &data->attack, data->type, r);
		states[States::Death]	= new DeathState(this, w, h, &data->death, data->type, r);
		states[States::Hurt]	= new HurtState(this, w, h, &data->hurt, data->type, r);
		states[States::Idle]	= new IdleState(this, w, h, &data->idle, data->type, r);
		states[States::Jump]	= new JumpState(this, w, h, &data->jump, data->type, r);
		states[States::Run]		= new RunState(this, w, h, &data->run, data->type, r);
	}
	Entity::~Entity()
	{
	}

	void Entity::setState(States state) {
		currentState = state;
		states[currentState]->reset();
	}
	void Entity::move(Direction dir)
	{
		setState(States::Run);
		if (dir == Direction::Right) {
			speedX = std::max(speedX + MAXSPEEDX, (float)MAXSPEEDX);
		}
		else {
			speedX = std::min(speedX - MAXSPEEDX, -(float)MAXSPEEDX);
		}
		lookDir = dir;
	}
	void Entity::stopMove(Direction dir)
	{
		if (dir == Direction::Right)
			speedX = std::min(0.f, speedX);
		else
			speedX = std::max(0.f, speedX);
		if (speedX == 0) {
			setState(States::Idle);
		}
	}
	void Entity::jump()
	{
		setState(States::Jump);
		speedY = -(float)MAXSPEEDY;
	}
	void Entity::idle()
	{
		setState(States::Idle);
	}
	void Entity::startAttack()
	{
		setState(States::Attack);
	}
	void Entity::takeDamage(int damage)
	{
		health -= damage;
	}
	void Entity::attack()
	{
		if (lookDir == Direction::Left)
			attackBox.setPos(x - attackBox.width, y - hitBox.height / 2 - attackBox.height / 2);
		else
			attackBox.setPos(x, y - hitBox.height / 2 - attackBox.height / 2);
		for (Entity* enemy : enemies) {
			if (attackBox.checkCollision(enemy->getHitBox())) {
				enemy->takeDamage(damage);
			}
		}
	}
	Square& Entity::getHitBox() {
		hitBox.setPos(x - hitBox.width / 2, y - hitBox.height);
		return hitBox;
	}
	SDL_Rect Entity::getRect()
	{
		return getHitBox().getRect();
	}
	SDL_Point Entity::getFocus()
	{
		return { (int)x,(int)y - hitBox.height / 2 };
	}
}