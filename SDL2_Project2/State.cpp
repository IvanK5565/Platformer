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
				currentFrame -= frames.size();
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
		SDL_Rect pos = { entity->getX() - w / 2, entity->getY() - h, w, h };
		if (SDL_HasIntersection(&pos, &camera.rect) == SDL_TRUE) {
			pos.x -= camera.rect.x;
			pos.y -= camera.rect.y;
			if (entity->getLook() == Direction::Left) flip = SDL_FLIP_HORIZONTAL;
			else if (entity->getLook() == Direction::Right) flip = SDL_FLIP_NONE;
			SDL_RenderCopyEx(renderer, frames[currentFrame], NULL, &pos, 0.f, NULL, flip);
			
#ifdef DEBUG
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(renderer, &pos);
#endif // DEBUG
		}

	}
	State::~State()
	{
		for (SDL_Texture* t : frames) {
			SDL_DestroyTexture(t);
		}
	}
	//IdleState
	void IdleState::handleEvent(SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
			switch (e.key.keysym.sym)
			{
			case SDLK_SPACE:
				entity->jump();
				break;
			case SDLK_LEFT:
			case SDLK_a:
				entity->move(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->move(Direction::Right);
				break;
			default:
				break;
			}
		}
		else if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				entity->stopMove(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->stopMove(Direction::Right);
				break;
			default:
				break;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0) {
			entity->startAttack();
		}
	}
	//RunState
	void RunState::handleEvent(SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
			switch (e.key.keysym.sym)
			{
			case SDLK_SPACE:
				std::cout << "SPACE\n";
				entity->jump();
				break;
			case SDLK_LEFT:
			case SDLK_a:
				entity->move(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->move(Direction::Right);
				break;
			default:
				break;
			}
		}
		 else if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				entity->stopMove(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->stopMove(Direction::Right);
				break;
			default:
				break;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0) {
			entity->startAttack();
		}
		else if (entity->getSpeedX() == 0 && entity->onGround()) {
			entity->idle();
		}
	}
	//JumpState
	void JumpState::act(Uint32 delay)
	{
		int speedY = entity->getSpeedY();
		if (speedY < 0)			currentFrame = 0;
		else if (speedY > 0)	currentFrame = 2;
		else					currentFrame = 1;
		if (entity->onGround()) {
			if (entity->getSpeedX() != 0)
				entity->setState(States::Run);
			else {
				entity->idle();
			}
		}
	}
	void JumpState::handleEvent(SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				entity->move(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->move(Direction::Right);
				break;
			default:
				break;
			}
		}
		else if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				entity->stopMove(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->stopMove(Direction::Right);
				break;
			default:
				break;
			}
		}
		else if (entity->onGround()) {
			if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0) {
				entity->startAttack();
			}
			else {
				if (entity->getSpeedX() != 0)
					entity->setState(States::Run);
				else {
					entity->idle();
				}
			}
		}
	}
	//AttackState
	void AttackState::reset()
	{
		State::reset();
		ended = false;
	}
	void AttackState::act(Uint32 delay)
	{
		if (ended) {
			entity->idle();
		}
		else{
			timer += delay;
			if (timer >= 100) {
				currentFrame++;
				if (currentFrame == frames.size())
				{
					currentFrame -= frames.size();
					ended = true;
				}
				timer -= 100;
			}
			if (currentFrame == 1) entity->attack();
		}
	}
	void AttackState::handleEvent(SDL_Event& e)
	{
		if (!ended) {
			return;
		}
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
			switch (e.key.keysym.sym)
			{
			case SDLK_SPACE:
				entity->jump();
				break;
			case SDLK_LEFT:
			case SDLK_a:
				entity->move(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->move(Direction::Right);
				break;
			default:
				break;
			}
		}
		else if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				entity->stopMove(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->stopMove(Direction::Right);
				break;
			default:
				break;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0) {
			entity->startAttack();
		}
		else {
			if (entity->getSpeedX() != 0)
				entity->setState(States::Run);
			else {
				entity->idle();
			}
		}
	}
	//HurtState
	void HurtState::handleEvent(SDL_Event& e)
	{
		if (currentFrame != frames.size()) {
			return;
		}
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
			switch (e.key.keysym.sym)
			{
			case SDLK_SPACE:
				entity->jump();
				break;
			case SDLK_LEFT:
			case SDLK_a:
				entity->move(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->move(Direction::Right);
				break;
			default:
				break;
			}
		}
		else if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				entity->stopMove(Direction::Left);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				entity->stopMove(Direction::Right);
				break;
			default:
				break;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0) {
			entity->startAttack();
		}
		else {
			entity->idle();
		}
	}
	//DeathState
	void DeathState::handleEvent(SDL_Event& e)
	{
		return;
	}
}