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

		virtual ~State();
	};
	//Player States
	class IdleState : public State {
	public:
		IdleState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void act(Uint32 delay) override;
		virtual ~IdleState() {}
	};
	class RunState : public State {
	public:
		RunState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void act(Uint32 delay) override;
		virtual ~RunState() {}
	};
	class JumpState : public State {
	public:
		JumpState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void act(Uint32 delay) override;
		virtual ~JumpState() {}
	};
	class AttackState : public State {
		bool ended;
		bool cooldown;
	public:
		AttackState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer), ended{ false }, cooldown{ false } {}
		virtual void reset() override;
		virtual void act(Uint32 delay) override;
		virtual ~AttackState() {}
	};
	class HurtState : public State {
		unsigned int hurtTimer;
	public:
		HurtState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer), hurtTimer{ 0 } {}
		virtual void reset() override;
		virtual void act(Uint32 delay) override;
		virtual ~HurtState() {}
	};
	class DeathState : public State {
	public:
		DeathState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void act(Uint32 delay) override;
		virtual ~DeathState() {}
	};
	//Enemy States
	class EnemyIdleState : public State {
	public:
		EnemyIdleState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void act(Uint32 delay) override;
		virtual ~EnemyIdleState() {}
	};
	class EnemyRunState : public State {
	public:
		EnemyRunState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void act(Uint32 delay) override;
		virtual ~EnemyRunState() {}
	};
	class EnemyJumpState : public State {
	public:
		EnemyJumpState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void act(Uint32 delay) override;
		virtual ~EnemyJumpState() {}
	};
	class EnemyAttackState : public State {
		bool ended;
		bool cooldown;
	public:
		EnemyAttackState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer), ended{ false }, cooldown{ false } {}
		virtual void reset() override;
		virtual void act(Uint32 delay) override;
		virtual ~EnemyAttackState() {}
	};
	class EnemyHurtState : public State {
	public:
		EnemyHurtState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual ~EnemyHurtState() {}
	};
	class EnemyDeathState : public State {
	public:
		EnemyDeathState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
			: State(entity, _w, _h, data, type, renderer) {}
		virtual void act(Uint32 delay) override;
		virtual ~EnemyDeathState() {}
	};
}