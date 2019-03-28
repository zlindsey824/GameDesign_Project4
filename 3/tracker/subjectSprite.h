#ifndef SUBJECTSPRITE__H
#define SUBJECTSPRITE__H
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include "drawable.h"

class SmartSprite;

class SubjectSprite : public Drawable {
public:
  SubjectSprite(const std::string&);
  SubjectSprite(const SubjectSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void attach( SmartSprite* o ) { observers.push_back(o); }
  void detach( SmartSprite* o );

  virtual const Image* getImage() const {
    return images[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

  void right();
  void left();
  void up();
  void down();
  void stop();

private:
  std::vector<Image *> images;
  std::vector<Image *> imagesLeft;
  std::vector<Image *> imagesRight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  Vector2f initialVelocity;

  void advanceFrame(Uint32 ticks);
  SubjectSprite& operator=(const SubjectSprite&);

protected:
  std::list<SmartSprite*> observers;
  // SubjectSprite& operator=(const SubjectSprite&);
};
#endif
