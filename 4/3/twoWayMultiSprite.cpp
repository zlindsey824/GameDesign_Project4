#include "multisprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "twoWayMultiSprite.h"

void twoWayMultiSprite::advanceFrame(Uint32 ticks) {
	 timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

twoWayMultiSprite::twoWayMultiSprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	flip(false)
{ }

twoWayMultiSprite::twoWayMultiSprite(const twoWayMultiSprite& s) :
  Drawable(s),
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
	flip(s.flip)
  { }

twoWayMultiSprite& twoWayMultiSprite::operator=(const twoWayMultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void twoWayMultiSprite::draw() const {
	if(flip){
  images[currentFrame]->draw(getX(), getY(), getScale(), true, 1);
}else{
	images[currentFrame]->draw(getX(), getY(), getScale(), false, 0);
}
}

void twoWayMultiSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
		flip = false;
  }
  if ( getX() + getScaledWidth() > worldWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
		flip = true;
  }

}
