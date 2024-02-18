#pragma once
#include "Enums.h"
#include "Camera.h"
#include <SDL.h>

namespace myGame {

	class Entity;
	class Square;

	struct CollisionInfo {
		bool collision;
		int x1; //distance between right side A and left side B
		//### ###
		//#A* *B#
		//### ###
		     
		int x2; //distance between left side A and right side B
		//### ###
		//*A# #B *
		//### ###
		     
		int y1; //distance between bottom side A and top side B
		//### #*#
		//#A# #B#
		//#*# ###
		     
		int y2; //distance between top side A and bottom side B
		//#*# ###
		//#A# #B#
		//### #*#
		explicit operator bool() const {
			return collision;
		}
	};


	class Collide {
	public:
		int x, y;
		Collide(int x, int y) : x(x), y(y) {}
		void setPos(int, int);
		virtual ~Collide() {}
		virtual CollisionInfo checkCollision(const Square&) = 0;
		virtual CollisionInfo checkCollision(const int _x, const int _y) = 0;
		virtual void drawBorder(SDL_Renderer*, Camera&) = 0;
		virtual CollideType getType() = 0;
	};
		//    ..#
		//    .##
		//    ###
	class RightTriangle : public Collide {
	public:
		static const int SIDE = 32;
		RightTriangle(int _x, int _y) : Collide(_x, _y) {}
		virtual ~RightTriangle() {}
		virtual CollisionInfo checkCollision(const Square&) override;
		virtual CollisionInfo checkCollision(const int _x, const int _y) override;
		virtual void drawBorder(SDL_Renderer*, Camera&) override;
		virtual CollideType getType() override;
	};
		//    #..
		//    ##.
		//    ###
	class LeftTriangle : public Collide {
	public:
		static const int SIDE = 32;
		LeftTriangle(int _x, int _y) : Collide(_x, _y) {}
		virtual ~LeftTriangle() {}
		virtual CollisionInfo checkCollision(const Square&) override;
		virtual CollisionInfo checkCollision(const int _x, const int _y) override;
		virtual void drawBorder(SDL_Renderer*, Camera&) override;
		virtual CollideType getType() override;
	};

	class Square : public Collide {
	public:
		int width, height;
		Square(int x, int y, int w, int h) : Collide(x, y), width{ w }, height{ h } {}
		Square(SDL_Rect rect) : Collide(rect.x, rect.y), width(rect.w), height(rect.h) {}
		virtual ~Square() {}
		virtual CollisionInfo checkCollision(const Square&) override;
		virtual CollisionInfo checkCollision(const int _x, const int _y) override;
		virtual void drawBorder(SDL_Renderer*, Camera&) override;
		virtual CollideType getType() override;
		SDL_Rect getRect() {
			return { x,y,width,height };
		}
		friend class RightTriangle;
		friend class LeftTriangle;
	};
}