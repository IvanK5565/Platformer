#pragma once
#include "Camera.h"
#include "Collide.h"
#include "Data.h"
#include "State.h"

namespace myGame {
	class Drawable {
	public:
		virtual void handleEvent(SDL_Event& e) = 0;
		virtual void render(Camera&, SDL_Renderer*) = 0;
		virtual void act(Uint32) = 0;
		virtual SDL_Rect getRect() = 0;
		virtual SDL_Point getFocus() = 0;
		virtual ~Drawable() {}
	};
}