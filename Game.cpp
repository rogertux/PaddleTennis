//Copyright 2015 Roger Bassons
#include "Game.h"

Game::Game() {
	bounces_ = 0;
	end_ = false;
	
	win_ = NULL;
	ren_ = NULL;

	screenWidth_ = 1024;
	screenHeight_ = 768;
	ballSize_ = 15;

	ball_.setSize(ballSize_); 
	ball_.setPosition(screenWidth_/2-ballSize_/2,screenHeight_/2-ballSize_/2);

	int padWidth = 20;
	int padHeight = 100;
	p1_.setSize(padWidth,padHeight);
	p2_.setSize(padWidth,padHeight);

	int x,y;
	y = screenHeight_/2-padHeight/2;
	x = screenWidth_-padWidth-5;
	p1_.setPosition(x,y);

	x = 5;
	p2_.setPosition(x,y);	
}

Game::~Game() {
	if (ren_ != NULL) 
		SDL_DestroyRenderer(ren_);
	if (win_ != NULL) 
		SDL_DestroyWindow(win_);
}

void Game::initVideo() {
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		throw SDL_GetError();

	win_ = SDL_CreateWindow("Game",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screenWidth_,screenHeight_,SDL_WINDOW_SHOWN);
	if (win_ == NULL) {
		SDL_Quit();
		throw SDL_GetError();
	}

	ren_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren_ == NULL) {
		SDL_DestroyWindow(win_);
		SDL_Quit();
		throw SDL_GetError();
	}

}

bool Game::isEnded() const {
	return end_;
}

void Game::play() {
	getUserInput();
	updateGame();
}

void Game::getUserInput() {
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0 ) {
		if (e.type == SDL_QUIT)
			end_ = true;
		else if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_ESCAPE) 
				end_ = true;
			else if (e.key.keysym.sym == SDLK_UP)
				p1_.accelerateUp();
			else if (e.key.keysym.sym == SDLK_DOWN)
				p1_.accelerateDown();
			else if (e.key.keysym.sym == SDLK_w)
				p2_.accelerateUp();
			else if (e.key.keysym.sym == SDLK_s)
				p2_.accelerateDown();
		} else if (e.type == SDL_KEYUP) {
			if (e.key.keysym.sym == SDLK_UP)
				p1_.stop();
			else if (e.key.keysym.sym == SDLK_DOWN)
				p1_.stop();
			else if (e.key.keysym.sym == SDLK_w)
				p2_.stop();
			else if (e.key.keysym.sym == SDLK_s)
				p2_.stop();
		}
	}
}

void Game::updateGame() {
	p1_.move(screenHeight_);
	p2_.move(screenHeight_);
	if (ball_.move(screenWidth_,screenHeight_,p2_,p1_))
		bounces_++;

	if (bounces_ == 10) {
		bounces_ = 0;
		ball_.moreSpeed();
	}
}

void Game::show() {
	SDL_SetRenderDrawColor(ren_,0,0,0,255);
	SDL_RenderClear(ren_);
	
	SDL_SetRenderDrawColor(ren_,255,255,255,255);
	SDL_RenderDrawLine(ren_,screenWidth_/2,0,screenWidth_/2,screenHeight_);
	
	ball_.draw(ren_);
	p1_.draw(ren_);
	p2_.draw(ren_);
	
	SDL_RenderPresent(ren_);
}
