#include "Camera.h"

namespace myGame {
	Camera::Camera() : maxSpeedX(10), maxSpeedY(10) {
		rect = { 0,0,0,0 };
	}
	Camera::Camera(int x, int y, int w, int h) : maxSpeedX(10), maxSpeedY(10) {
		rect = { x,y,w,h };
	}
	Camera::Camera(SDL_Rect rect) : maxSpeedX(10), maxSpeedY(10), rect(rect) {

	}
	void Camera::move(SDL_Point focus, SDL_Rect zone, Uint32 delay) {
		float cameraCenterX = (rect.x + rect.w / 2), cameraCenterY = (rect.y + rect.h / 2);
		float xDist = (float)focus.x - cameraCenterX;
		float yDist = (float)focus.y - cameraCenterY;
		rect.x += xDist * delay / 1000.f;
		rect.y += yDist * delay / 1000.f;

		rect.y = std::max(0, rect.y);
		rect.y = std::min(zone.h, rect.y);
		rect.x = std::min(zone.w, rect.x);
		rect.x = std::max(0, rect.x);
	}

	void Camera::move(Direction dir, SDL_Rect zone)
	{
		switch (dir)
		{
		case Direction::Up:
			rect.y = std::max(0, rect.y - 10);
			break;
		case Direction::Down:
			rect.y = std::min(zone.h, rect.y + rect.h + 10) - rect.h;
			break;
		case Direction::Right:
			rect.x = std::min(zone.w, rect.x + rect.w + 10) - rect.w;
			break;
		case Direction::Left:
			rect.x = std::max(0, rect.x - 10);
			break;
		default:
			break;
		}
	}
}