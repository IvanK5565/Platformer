#include "Drawable.h"
#include "Entity.h"
#include "Game.h"
#include "State.h"

namespace myGame {
	//State

	State::State(Entity* e, int _w, int _h, StateData* data, string type, SDL_Renderer* r) :
		entity{ e }, frames{}, currentFrame{ 0 }, timer{ 0 }, flip{ SDL_FLIP_NONE }, w{ _w }, h{ _h }
	{
		for (int i = 0; i < data->count; i++) {
			string name = i < 10 ? "0" : "";
			name += std::to_string(i) + "." + type;
			SDL_Texture* buf = loadTexture(r, data->place + name);
			if (buf == NULL) {
				printf("Error state loading!\n");
				getError();
			}
			else frames.push_back(buf);
		}
	}
	void State::act(Uint32 delay)
	{
		timer += delay;
		if (timer >= 100) {
			currentFrame++;
			if (currentFrame == frames.size())
				currentFrame = 0;
			timer -= 100;
		}
	}
	void State::reset()
	{
		currentFrame = 0;
		timer = 0;
	}
	void State::render(Camera& camera, SDL_Renderer* renderer)
	{
		currentFrame = min(currentFrame, frames.size() - 1);
		SDL_Rect pos = { entity->getX() - w / 2, entity->getY() - h, w, h };
		if (SDL_HasIntersection(&pos, &camera.rect) == SDL_TRUE) {
			pos.x -= camera.rect.x;
			pos.y -= camera.rect.y;
			if (entity->getLookDir() == Direction::Left) flip = SDL_FLIP_HORIZONTAL;
			else if (entity->getLookDir() == Direction::Right) flip = SDL_FLIP_NONE;
			SDL_RenderCopyEx(renderer, frames[currentFrame], NULL, &pos, 0.f, NULL, flip);

#ifdef DEBUG
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(renderer, &pos);
#endif // DEBUG
		}

	}
	bool State::isLastFrame()
	{
		return currentFrame == frames.size()-1;
	}
	State::~State()
	{
		for (SDL_Texture* t : frames) {
			SDL_DestroyTexture(t);
		}
	}
	//IdleState
	void IdleState::act(Uint32 delay)
	{
		State::act(delay);
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if (currentKeyStates[SDL_SCANCODE_A])
		{
			entity->move(Direction::Left, delay);
			entity->setState(States::Run);
		}
		else if (currentKeyStates[SDL_SCANCODE_D])
		{
			entity->move(Direction::Right, delay);
			entity->setState(States::Run);
		}
		if (!entity->onGround()) {
			entity->setState(States::Jump);
		}
		else if (currentKeyStates[SDL_SCANCODE_SPACE])
		{
			entity->jump();
		}
		else if (SDL_GetMouseState(NULL, NULL) & 1) {
			entity->setState(States::Attack);
		}

	}
	//RunState
	void RunState::act(Uint32 delay)
	{
		State::act(delay);
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if (!entity->onGround()) {
			entity->setState(States::Jump);
		}
		if (currentKeyStates[SDL_SCANCODE_SPACE])
		{
			entity->jump();
		}
		if (SDL_GetMouseState(NULL, NULL) & 1) {
			entity->setState(States::Attack);
		}
		else if (currentKeyStates[SDL_SCANCODE_A])
		{
			entity->move(Direction::Left, delay);
		}
		else if (currentKeyStates[SDL_SCANCODE_D])
		{
			entity->move(Direction::Right, delay);
		}
		else {
			entity->setState(States::Idle);
		}
	}
	//JumpState
	void JumpState::act(Uint32 delay)
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_A])
		{
			entity->move(Direction::Left, delay);
		}
		if (currentKeyStates[SDL_SCANCODE_D])
		{
			entity->move(Direction::Right, delay);
		}
		int speedY = entity->getSpeedY();
		if (speedY < 0)			currentFrame = 0;
		else if (speedY > 0)	currentFrame = 2;
		else					currentFrame = 1;
		if (entity->onGround()) {
			entity->setState(States::Idle);
		}
	}
	//AttackState
	void AttackState::reset()
	{
		State::reset();
		ended = false;
		cooldown = false;
	}
	void AttackState::act(Uint32 delay)
	{
		if (ended) {

			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

			if (currentKeyStates[SDL_SCANCODE_A])
			{
				entity->move(Direction::Left, delay);
				entity->setState(States::Run);
			}
			else if (currentKeyStates[SDL_SCANCODE_D])
			{
				entity->move(Direction::Right, delay);
				entity->setState(States::Run);
			}
			else if (currentKeyStates[SDL_SCANCODE_SPACE])
			{
				entity->jump();
			}
			else if (SDL_GetMouseState(NULL, NULL) & 1) {
				reset();
			}
			else
			{
				entity->setState(States::Idle);
			}
		}
		else {
			timer += delay;
			if (timer >= 100) {
				currentFrame++;
				if (currentFrame == frames.size())
				{
					currentFrame = 0;
					ended = true;
				}
				timer -= 100;
			}
			if (currentFrame == 1 && !cooldown) {
				entity->attack();
				cooldown = true;
			}
		}
	}

	void HurtState::reset()
	{
		hurtTimer = 0;
		State::reset();
	}

	void HurtState::act(Uint32 delay)
	{
		if (hurtTimer == 0) {
			entity->setSpeedY(-25);
		}
		hurtTimer += delay;
		if (hurtTimer > 500) {
			entity->setState(States::Idle);
		}
		State::act(delay);
		entity->move(entity->getHurtDir(), delay);
	}

	void DeathState::act(Uint32 delay)
	{
		if (currentFrame != frames.size() - 1)
			State::act(delay);
	}

	//Enemy States
	void EnemyIdleState::act(Uint32 delay)
	{
		State::act(delay);

	}
	void EnemyRunState::act(Uint32 delay)
	{
		State::act(delay);
		entity->move(entity->getLookDir(), delay);
	}
	void EnemyJumpState::act(Uint32 delay)
	{
		//if (entity->onGround()) {
		//	entity->setState(States::Idle);
		//}
	}
	void EnemyAttackState::reset()
	{
		State::reset();
		ended = false;
		cooldown = false;
	}
	void EnemyAttackState::act(Uint32 delay)
	{
		if (ended) {
			entity->setState(States::Idle);
		}
		else {
			timer += delay;
			if (timer >= 100) {
				currentFrame++;
				if (currentFrame == frames.size())
				{
					currentFrame = 0;
					ended = true;
				}
				timer -= 100;
			}
			if (currentFrame == 4 && !cooldown) {
				entity->attack();
				cooldown = true;
			}
		}
	}
	void EnemyDeathState::act(Uint32 delay)
	{
		if (currentFrame != frames.size() - 1)
			State::act(delay);
	}
	void EnemyHurtState::reset()
	{
		hurtTimer = 0;
		State::reset();
	}
	void EnemyHurtState::act(Uint32 delay)
	{
		hurtTimer += delay;
		if (hurtTimer > 1250 + rand()%250) {
			entity->setState(States::Idle);
		}
		State::act(delay);
	}
}