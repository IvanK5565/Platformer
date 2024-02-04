#pragma once
#include "Data.h"
#include "Enums.h"
#include "Tools.h"
#include <map>
#include <SDL.h>
#include <vector>
using std::map, std::vector;

namespace myGame {

	class Entity;

	class State {
	protected:
		int w, h;
		Entity* entity;
		vector<SDL_Texture*> frames;
		int currentFrame;
		Uint32 timer;
		SDL_RendererFlip flip;
	public:
		State(Entity* e, int _w, int _h, StateData* data, string type, SDL_Renderer* r);
		virtual void act(Uint32 delay);
		virtual void reset();
		void render(Camera& camera, SDL_Renderer* renderer);

		virtual void handleEvent(SDL_Event&) = 0;
		virtual ~State();
	};

	class IdleState : public State {
	public:
		IdleState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void handleEvent(SDL_Event&) override;
		virtual ~IdleState() {}
	};
	class RunState : public State {
	public:
		RunState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void handleEvent(SDL_Event&) override;
		virtual ~RunState() {}
	};
	class JumpState : public State {
	public:
		JumpState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void act(Uint32 delay) override;
		virtual void handleEvent(SDL_Event&) override;
		virtual ~JumpState() {}
	};
	class AttackState : public State {
		bool ended;
	public:
		AttackState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer), ended{ false } {}
		virtual void reset() override;
		virtual void act(Uint32 delay) override;
		virtual void handleEvent(SDL_Event&) override;
		virtual ~AttackState() {}
	};
	class HurtState : public State {
	public:
		HurtState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void handleEvent(SDL_Event&) override;
		virtual ~HurtState() {}
	};
	class DeathState : public State {
	public:
		DeathState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void handleEvent(SDL_Event&) override;
		virtual ~DeathState() {}
	};
}