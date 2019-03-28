#include "subjectSprite.h"
#include "smartSprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "renderContext.h"

void SubjectSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

SubjectSprite::SubjectSprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  imagesLeft( ImageFactory::getInstance().getImages(name+"Left") ),
  imagesRight( images ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  initialVelocity(getVelocity()),
  observers()
{ }

SubjectSprite::SubjectSprite(const SubjectSprite& s) :
  Drawable(s),
  images(s.images),
  imagesLeft(s.imagesLeft),
  imagesRight(s.imagesRight),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  initialVelocity( s.initialVelocity ),
  observers( s.observers )
  { }

SubjectSprite& SubjectSprite::operator=(const SubjectSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  imagesLeft = (s.imagesLeft);
  imagesRight = (s.imagesRight);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  initialVelocity = ( s.initialVelocity );
  return *this;
}

void SubjectSprite::draw() const {
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void SubjectSprite::stop() {
//  setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
}

void SubjectSprite::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    images = imagesRight;
  }
}
void SubjectSprite::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    images = imagesLeft;
  }
}
void SubjectSprite::up()    {
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
}
void SubjectSprite::down()  {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void SubjectSprite::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void SubjectSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    int offset = rand()%5;
    offset = offset*(rand()%2?-1:1);
    const Vector2f off(offset, offset);
    (*ptr)->setPlayerPos( getPosition()+off );
    ++ptr;
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }

  stop();
}
