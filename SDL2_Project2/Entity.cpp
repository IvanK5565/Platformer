#include "Entity.h"

namespace myGame {
	Entity::Entity(int x, int y, int health, int damage, EntityData* data, vector<Collide*>& walls, SDL_Renderer* r) :
		x{ (float)x }, y{ (float)y }, health{ health }, damage{ damage }, walls{ walls }, currentState{ States::Idle },
		hitBox{ x - data->hitW / 2, y - data->hitH,data->hitW,data->hitH }, attackBox{ x, y - hitBox.height / 2 - attackBox.height / 2,data->attackW,data->attackH },
		lookDir{ Direction::Right }, hurtDir{ Direction::Left }, speedY{ 0 }
	{
		int w = data->width;
		int h = data->height;
	}
	Entity::~Entity()
	{

	}

	float Entity::getX() const {
		return x;
	}
	float Entity::getY() const {
		return y;
	}
	float Entity::getSpeedY() const {
		return speedY;
	}
	void Entity::setSpeedY(float speed)
	{
		speedY = speed;
	}
	Direction Entity::getHurtDir()
	{
		return hurtDir;
	}
	bool Entity::isDead()
	{
		return health <= 0 && states[currentState];
	}
	Direction Entity::getLookDir() {
		return lookDir;
	}

	void Entity::setState(States state) {
		if (currentState == state) return;
		currentState = state;
		auto result = states.find(state);
		if (result != states.end()) {
			result->second->reset();
		}
	}
	void Entity::move(Direction dir, Uint32 delay)
	{
		int dop = MAXSPEEDX * delay / 1000;
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
	void Entity::jump()
	{
		setState(States::Jump);
		speedY = -(float)MAXSPEEDY;
	}
	void Entity::takeDamage(int damage, Direction dir)
	{
		if (health > 0) {
			health -= damage;
		}
		if (health > 0) {
			setState(States::Hurt);
			hurtDir = dir;
		}
		else {
			setState(States::Death);
		}
	}
	Rectangle& Entity::getHitBox() {
		hitBox.setPos(x - hitBox.width / 2, y - hitBox.height);
		return hitBox;
	}
	bool Entity::onGround()
	{
		for (Collide* wall : walls) {
			if (wall->checkCollision(x - hitBox.width / 2 + 1, y + 1) || wall->checkCollision(x + hitBox.width / 2 - 1, y + 1))
				return true;
		}
		return false;
	}
	void Entity::collissionUpdate()
	{
		for (Collide* wall : walls) {
			if (CollisionInfo info = wall->checkCollision(getHitBox())) {
				int _x = std::abs(info.x1) < std::abs(info.x2) ? info.x1 : info.x2;
				int _y = std::abs(info.y1) < std::abs(info.y2) ? info.y1 : info.y2;

				if (std::abs(_x) < std::abs(_y)) {
					x += _x;
				}
				else {
					y += _y;
					if (_y > 0) y++;
					speedY = 0;
				}
			}
		}
	}
	SDL_Rect Entity::getRect()
	{
		return getHitBox().getRect();
	}
	SDL_Point Entity::getFocus()
	{
		return { (int)x,(int)y - hitBox.height / 2 };
	}

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

	void Entity::render(Camera& camera, SDL_Renderer* renderer) {
		states[currentState]->render(camera, renderer);

#ifdef DEBUG
		SDL_Rect pos = { x,y,1,1 };
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