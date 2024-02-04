#include "Collide.h"
#include "Entity.h"

namespace myGame {

	CollisionInfo RightTriangle::checkCollision(const Square& box) {
		if (box.y + box.height < y) return { false };
		if (box.x > x + SIDE) return { false };

		int x1 = x - box.x - box.width;
		if (x1 > 0) return { false };

		int y2 = y + SIDE - box.y;
		if (y2 < 0) return{ false };

		int Xdif = std::max(x+SIDE - box.x - box.width, 0);
		int topY = y + Xdif;
		int y1 = topY - box.y - box.height;
		if (y1 > 0) return { false };
		int x2 = box.x > x ? box.x + box.width : y1;

		return{ true, x1,x2,y1,y2 };
	}
	CollisionInfo LeftTriangle::checkCollision(const Square& box) {
		if (box.y + box.height < y) return { false };
		if (box.x > x + SIDE) return { false };

		int x1 = x - box.x - box.width;
		if (x1 > 0) return { false };

		int y2 = y + SIDE - box.y;
		if (y2 < 0) return{ false };
		int buf = box.x - x;
		int Xdif = std::max(buf, 0);
		int topY = y + Xdif;
		int y1 = topY - box.y - box.height;
		if (y1 > 0) return { false };
		int x2 = box.x < x ? x + SIDE - box.x : -y1;

		return{ true, x1,x2,y1,y2 };
	}
	CollisionInfo Square::checkCollision(const Square& box) {
		int x1 = box.x + box.width - x;
		int x2 = box.x - x - width;
		if (x1 < 0 || x2 > 0) {
			return { false };
		}

		int y1 = box.y + box.height - y;
		int y2 = box.y - y - height;
		if (y1 < 0 || y2 > 0) {
			return { false };
		}

		if (y1 != 0) {
			y1 = y1;
		}

		return { true, -x1,-x2,-y1,-y2 };
	}

	void RightTriangle::drawBorder(SDL_Renderer* renderer, Camera& camera)
	{
		SDL_Rect pos{ x,y,SIDE,SIDE };
		if (SDL_HasIntersection(&pos, &camera.rect) == SDL_TRUE) {
			pos.x -= camera.rect.x;
			pos.y -= camera.rect.y;
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderDrawRect(renderer, &pos);
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderDrawLine(renderer, pos.x, pos.y + SIDE, pos.x + SIDE, pos.y + SIDE);
			SDL_RenderDrawLine(renderer, pos.x + SIDE, pos.y, pos.x + SIDE, pos.y + SIDE);
			SDL_RenderDrawLine(renderer, pos.x + SIDE, pos.y, pos.x, pos.y + SIDE);
		}
	}
	void LeftTriangle::drawBorder(SDL_Renderer* renderer, Camera& camera)
	{
		SDL_Rect pos{ x,y,SIDE,SIDE };
		if (SDL_HasIntersection(&pos, &camera.rect) == SDL_TRUE) {
			pos.x -= camera.rect.x;
			pos.y -= camera.rect.y;
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderDrawRect(renderer, &pos);
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderDrawLine(renderer, pos.x, pos.y + SIDE, pos.x + SIDE, pos.y + SIDE);
			SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x, pos.y + SIDE);
			SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x + SIDE, pos.y + SIDE);
		}
	}
	void Square::drawBorder(SDL_Renderer* renderer, Camera& camera)
	{
		SDL_Rect pos{ x,y,width,height };
		if (SDL_HasIntersection(&pos, &camera.rect) == SDL_TRUE) {
			pos.x -= camera.rect.x;
			pos.y -= camera.rect.y;
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(renderer, &pos);
		}
	}

	CollideType RightTriangle::getType()
	{
		return CollideType::RightTriangle;
	}
	CollideType LeftTriangle::getType()
	{
		return CollideType::LeftTriangle;
	}
	CollideType Square::getType()
	{
		return CollideType::Square;
	}

	void Collide::setPos(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
}