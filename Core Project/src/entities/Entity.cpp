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
}