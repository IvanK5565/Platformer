#include "Collide.h"
#include "Entity.h"

namespace myGame {

	CollisionInfo RightTriangle::checkCollision(const Rectangle& box) {
		if (box.y + box.height < y) return { false };
		if (box.x > x + SIDE) return { false };

		int x1 = x - box.x - box.width;
		if (x1 > 0) return { false };

		int y2 = y + SIDE - box.y;
		if (y2 < 0) return{ false };

		int Xdif = std::max(x1+SIDE, 0);
		int topY = y + Xdif;
		int y1 = topY - box.y - box.height;
		if (y1 > 0) return { false };
		int x2 = box.x > x ? box.x + box.width : y1;

		return{ true, x1,x2,y1,y2 };
	}
	CollisionInfo RightTriangle::checkCollision(const int _x, const int _y)
	{
		if (_y < y) return { false };
		if (_x > x + SIDE) return { false };

		int x1 = x - _x;
		if (x1 > 0) return { false };

		int y2 = y + SIDE - _y;
		if (y2 < 0) return{ false };

		int Xdif = std::max(x1 + SIDE, 0);
		int topY = y + Xdif;
		int y1 = topY - _y;
		if (y1 > 0) return { false };
		int x2 = _x > x ? _x : y1;

		return{ true, x1,x2,y1,y2 };
	}
	CollisionInfo LeftTriangle::checkCollision(const Rectangle& box) {
		if (box.y + box.height < y) return { false };
		if (box.x > x + SIDE) return { false };

		int x1 = x - box.x - box.width;
		if (x1 > 0) return { false };

		int y2 = y + SIDE - box.y;
		if (y2 < 0) return{ false };
		int Xdif = std::max(box.x - x, 0);
		int topY = y + Xdif;
		int y1 = topY - box.y - box.height;
		if (y1 > 0) return { false };
		int x2 = box.x < x ? x + SIDE - box.x : -y1;

		return{ true, x1,x2,y1,y2 };
	}
	CollisionInfo LeftTriangle::checkCollision(const int _x, const int _y)
	{
		if (_y < y) return { false };
		if (_x > x + SIDE) return { false };

		int x1 = x - _x;
		if (x1 > 0) return { false };

		int y2 = y + SIDE - _y;
		if (y2 < 0) return{ false };
		int Xdif = std::max(_x - x, 0);
		int topY = y + Xdif;
		int y1 = topY - _y;
		if (y1 > 0) return { false };
		int x2 = _x < x ? x + SIDE - _x : -y1;

		return{ true, x1,x2,y1,y2 };
	}
	CollisionInfo Rectangle::checkCollision(const Rectangle& box) {
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

	CollisionInfo Rectangle::checkCollision(const int _x, const int _y)
	{
		int x1 = _x - x;
		int x2 = _x - x - width;
		if (x1 < 0 || x2 > 0) {
			return { false };
		}

		int y1 = _y - y;
		int y2 = _y - y - height;
		if (y1 < 0 || y2 > 0) {
			return { false };
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
	void Rectangle::drawBorder(SDL_Renderer* renderer, Camera& camera)
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
	CollideType Rectangle::getType()
	{
		return CollideType::Rectangle;
	}

	void Collide::setPos(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
}