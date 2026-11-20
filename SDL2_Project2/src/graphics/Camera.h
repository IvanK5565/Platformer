#pragma once
#include "Enums.h"
#include <SDL.h>
#include <cmath>

namespace myGame {
	class Camera {
	public:
		SDL_Rect rect;
		const int maxSpeedX, maxSpeedY;

		Camera();
		Camera(int, int, int, int);
		Camera(SDL_Rect);

		void move(SDL_Point, SDL_Rect, Uint32);
		void move(Direction, SDL_Rect);
	};
}
