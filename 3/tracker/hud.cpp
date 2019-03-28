#include "hud.h"

#include <SDL.h>

#include "gameData.h"

#include "ioMod.h"



const SDL_Color color = {255, 255, 0, 255};



Hud::Hud()

    : x(Gamedata::getInstance().getXmlInt("hud/x")),

      y(Gamedata::getInstance().getXmlInt("hud/y")),

      width(Gamedata::getInstance().getXmlInt("hud/w")),

      height(Gamedata::getInstance().getXmlInt("hud/h")),

      c(color),

      io(IoMod::getInstance()) {}



Hud& Hud::getInstance() {

  static Hud instance;

  return instance;

}



void Hud::display(SDL_Renderer* renderer) {

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  //setting the color for hud
  SDL_SetRenderDrawColor(renderer, 106, 106, 106, 200);
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = width;
  rect.h = height;

  //render for rectangle
  SDL_RenderFillRect(renderer, &rect);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255/ 2);
  SDL_RenderDrawRect(renderer, &rect);



  io.writeText("W: Up", c, 350, 110);
  io.writeText("S: Down", c, 350, 150);
  io.writeText("A: Left", c, 350, 190);
  io.writeText("D: Right", c, 350, 230);
  io.writeText("P: Pause", c, 350, 270);
  io.writeText("M: Change Collision Strategy", c, 350, 310);
  io.writeText("T: Change Track Object", c, 350, 350);

  SDL_RenderPresent(renderer);

}
