#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World cloud;
  World rainbow;
  Viewport& viewport;

  std::vector <Drawable*> sprites;
  //Drawable* star;
  //Drawable* bluebird;
  //Drawable* flyinsect;
  //Drawable* flappybird;
  //Drawable* ballon;
  //Drawable* grumpbird;
  //Drawable* helicopter;
  int currentSprite;

  bool makeVideo;

  void draw() const;
  void update(Uint32);
  void printScales() const;
  void checkForCollisions();
};
