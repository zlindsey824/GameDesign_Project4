#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "smartSprite.h"
#include "subjectSprite.h"
#include "sprite.h"
#include "multisprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "twoWayMultiSprite.h"
#include "collisionStrategy.h"
//#include "player.h"

const SDL_Color yellow = {255, 255, 0, 255};

Engine::~Engine() {


  delete player;
  for ( Drawable* sprite : sprites ) {
    delete sprite;
  }
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
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
  player(new SubjectSprite("Helicopter")),
  sprites(),
  strategies(),
  currentStrategy(0),
  collision(false),
  makeVideo( false )

{
 int n = Gamedata::getInstance().getXmlInt("numberOfBalloons");
  sprites.reserve(n);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 0; i < n; ++i) {
    sprites.push_back( new SmartSprite("Balloon", pos, w, h) );
    player->attach( sprites[i] );
  }

  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  //star->setScale(1.5);
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}


void Engine::draw() const {
  //world.draw();
  cloud.draw();
  rainbow.draw();
	for ( const Drawable* sprite : sprites ) {
    sprite->draw();
  }
  //4. Task 4: add fps information on screen

 SDL_Rect rect;
 rect.x = 15;
 rect.y = 15;
 rect.w = 250;
 rect.h = 150;
 //this is small hud that display F1 HUD and FPS and Name
 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
 // Now set the color for the hud:
 SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );
 // Render rect
 SDL_RenderFillRect( renderer, &rect );
 // Now set the color for the outline of the hud:
 SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );
 SDL_RenderDrawRect( renderer, &rect );

  io.writeText("Press F1 for HUD", 30, 90);



  std::stringstream string_fps;
  SDL_Color fpsColor = yellow;
  string_fps << "FPS: " << clock.getFps();
  io.writeText(string_fps.str(), fpsColor, 30, 60);

  //5. Task: add addtional colors
  std::stringstream string_name;
  SDL_Color nameColor = {247, 233, 77, 0};
  string_name << Gamedata::getInstance().getXmlStr("myname");
  int name_loc_x = 20;
  int name_loc_y = 420;
  io.writeText(string_name.str(), nameColor, name_loc_x, name_loc_y);

  io.writeText("Press m to change strategy", 500, 60);
  for ( const Drawable* sprite : sprites ) {
    sprite->draw();
  }
  std::stringstream strm;
  strm << sprites.size() << " Smart Sprites Left";
  io.writeText(strm.str(), yellow, 30, 120);
  strategies[currentStrategy]->draw();
  if ( collision ) {
    io.writeText("Oops: Collision", 500, 90);
  }
  player->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = sprites.begin();
  while ( it != sprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = sprites.erase(it);
    }
    else ++it;
  }
}

void Engine::update(Uint32 ticks) {
  cloud.update();
  rainbow.update();

	checkForCollisions();
  player->update(ticks);
  //subjectPlayer->update(ticks);
  for ( Drawable* sprite : sprites ) {
    sprite->update( ticks );
  }
  viewport.update(); // always update viewport last
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
        if (keystate[SDL_SCANCODE_F1]) {
          if (clock.isPaused())
            clock.unpause();
          else {
            clock.pause();
            Hud::getInstance().display(renderer);
          }
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
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

    // In this section of the ev  player->draw();ent loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<SubjectSprite*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<SubjectSprite*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<SubjectSprite*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<SubjectSprite*>(player)->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
