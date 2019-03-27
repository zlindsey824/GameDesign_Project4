#include <SDL.h>
#include <iostream>
#include "imageFactory.h"

class RenderContext {
 public:
  // TASK 3: GoF to Meyer
  static RenderContext& getInstance();

  ~RenderContext();
  SDL_Window* getWindow() const { return window; }
  SDL_Renderer* getRenderer() const { return renderer; }
  RenderContext(const RenderContext&) = delete;
  RenderContext& operator=(const RenderContext&) = delete;

 private:
  // TASK 3: GoF to Meyer
  //static RenderContext& instance;
  SDL_Window* window;
  SDL_Renderer* renderer;

  ImageFactory& factory;

  SDL_Window* initWindow();
  SDL_Renderer* initRenderer();
  RenderContext();
};
