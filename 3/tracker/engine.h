#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"

class Player;

class CollisionStrategy;
class SmartSprite;
class SubjectSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  //void switchSprite();
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

  Player* player;
  //int currentSprite;

  SubjectSprite* subjectPlayer;
  std::vector<SmartSprite*> sprites;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;

  bool makeVideo;


  void draw() const;
  void update(Uint32);
  void printScales() const;
  void checkForCollisions();
};
