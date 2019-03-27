#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "twoWayMultiSprite.h"
#include "player.h"

Engine::~Engine() {
  std::vector<Drawable*> :: const_iterator it = sprites.begin();
  while(it != sprites.end())
  {
    delete *it;
    it++;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  cloud("cloud", Gamedata::getInstance().getXmlInt("cloud/factor")),
  rainbow("rainbow", Gamedata::getInstance().getXmlInt("rainbow/factor")),
  viewport( Viewport::getInstance() ),
  player(new Player("Helicopter")),
  sprites(0),
  currentSprite(0),
  makeVideo( false )
{
 sprites.reserve(7);
 MultiSprite* balloon = new MultiSprite("Balloon");
 MultiSprite* balloon1 = new MultiSprite("Balloon");
 twoWayMultiSprite* bluebird = new twoWayMultiSprite("Bluebird");
 twoWayMultiSprite* flyinsect = new twoWayMultiSprite("Flyinsect");
 twoWayMultiSprite* flappybird = new twoWayMultiSprite("Flappybird");
 twoWayMultiSprite* grumpbird = new twoWayMultiSprite("Grumpbird");
 //twoWayMultiSprite* helicopter = new twoWayMultiSprite("Helicopter");

 Viewport::getInstance().setObjectToTrack(player);
 sprites.emplace_back(balloon);
 sprites.emplace_back(balloon1);
 sprites.emplace_back(bluebird);
 sprites.emplace_back(flyinsect);
 sprites.emplace_back(flappybird);
 sprites.emplace_back(grumpbird);
 //sprites.emplace_back(helicopter);
  //star->setScale(1.5);
  //Viewport::getInstance().setObjectToTrack(star);
  std::cout << "Loading complete" << std::endl;
}


void Engine::draw() const {
  //world.draw();
  cloud.draw();
  rainbow.draw();

  player->draw();
  std::vector<Drawable*> :: const_iterator it = sprites.begin();
  while(it != sprites.end())
  {
    (*it)-> draw();
    it++;
  }
  //star->draw();
  //bluebird->draw();
  //flyinsect->draw();
  //flappybird->draw();
  //ballon->draw();
  //grumpbird->draw();
  //helicopter->draw();
  //4. Task 4: add fps information on screen
  std::stringstream string_fps;
  SDL_Color fpsColor = {229, 54, 49,0};
  string_fps << "FPS: " << clock.getFps();
  io.writeText(string_fps.str(), fpsColor, 30, 60);

  //5. Task: add addtional colors
  std::stringstream string_name;
  SDL_Color nameColor = {247, 233, 77, 0};
  string_name << Gamedata::getInstance().getXmlStr("myname");
  int name_loc_x = 20;
  int name_loc_y = 420;
  io.writeText(string_name.str(), nameColor, name_loc_x, name_loc_y);
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  cloud.update();
  rainbow.update();

  player->update(ticks);
  std::vector<Drawable*> :: const_iterator it = sprites.begin();
  while(it != sprites.end())
  {
    (*it)-> update(ticks);
    it++;
  }
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % (int)sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
  }

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
