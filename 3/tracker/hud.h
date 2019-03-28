#ifndef _HUD__H
#define _HUD__H

#include <SDL.h>
#include <iostream>
#include <string>

class IoMod;

class Hud{
	public:
	static Hud& getInstance();
	void display(SDL_Renderer*);

	private:
	int x;
	int y;
	int width;
	int height;
	SDL_Color c;
	IoMod& io;

	Hud();
	//Hud(Hud& h);
	Hud& operator=(Hud& h);
};

#endif
