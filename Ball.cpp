//Copyright 2015 Roger Bassons
#include "Ball.h"
#include <cmath>
#include <ctime>
#include <random>

Ball::Ball() {
	b_.x = b_.y = b_.w = b_.h = 0;
	speed_ = 7;
	dy_ = 0;
	dx_ = 7;
	srand(time(NULL)); 
}

void Ball::setSize(int size) {
	b_.w = b_.h = size;
}

void Ball::setPosition(int x, int y) {
	b_.x = x;
	b_.y = y;
}

bool Ball::move(int width, int height, const Paddle &left, const Paddle &right) {
	int nextX = b_.x + dx_;
	int nextY = b_.y + dy_;

	if (nextX > width or nextX+b_.w < 0) {
		nextX = width/2-b_.w/2;
		nextY = height/2-b_.h;
		
		int min = 0;
		int max = 65;
		int angle = rand()%(max-min + 1) + min;                                        

		min = 0;
	        max = 1;
		int direction = rand()%(max-min + 1) + min;
		if (direction == 0)
			direction = -1;
		
		dx_ = direction*speed_*cos(angle*M_PI/180.0);               
		dy_ = speed_*sin(angle*M_PI/180.0);
	} else if (nextY < 0) {
		dy_*=-1;
		nextY = 0;
	} else if (nextY+b_.h > height) {
		dy_*=-1;
		nextY = height-b_.h;
	}

	bool collided = false;
	if (dx_ < 0 and b_.x < width/2) {
		if (b_.x >= left.getX() and b_.x <= left.getX()+left.getWidth()) {
			if (b_.y >= left.getY() and b_.y <= left.getY()+left.getHeight())
				collided = true;
			else if (b_.y+b_.h >= left.getY() and b_.y+b_.h <= left.getY()+left.getHeight())
				collided = true;
		}
	} else if (dx_ > 0 and b_.x > width/2) {
		if (b_.x+b_.w >= right.getX() and b_.x+b_.w <= right.getX()+right.getWidth()) {
			if (b_.y >= right.getY() and b_.y <= right.getY()+right.getHeight())
				collided = true;
			else if (b_.y+b_.h >= right.getY() and b_.y+b_.h <= right.getY()+right.getHeight())
				collided = true;
		}
	}
	if (collided) {
		dx_ *= -1;
		if (dy_ == 0)
			dy_ = 1;
	}
	
	b_.x = nextX;
	b_.y = nextY;
	return collided;
}

void Ball::moreSpeed() {
	speed_++;
	dx_++;
}

void Ball::draw(SDL_Renderer *ren) const {
	SDL_SetRenderDrawColor(ren,255,255,255,255);
	SDL_RenderFillRect(ren,&b_);
}
	
