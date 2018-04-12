#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <bullet/btBulletDynamicsCommon.h>
#include "getBMP.h"
#include "abstractGlBody.h"

AbstractGlBody :: AbstractGlBody() = default;

void AbstractGlBody :: setPosition(float x, float y, float z) {
    this->position.setX( x );
    this->position.setY( y );
    this->position.setZ( z );
}
void AbstractGlBody :: setPosition(btVector3 position) {
    this->position.setX( position.getX() );
    this->position.setY( position.getY() );
    this->position.setZ( position.getZ() );
}

const btVector3 AbstractGlBody :: getPosition() const {
    return this->position;
}
