#pragma once
#include <bullet/btBulletDynamicsCommon.h>
class AbstractGlBody {
    public:
        AbstractGlBody();
        // draw the object about origin with no rotations. Transformations of 
        // the body will be done in either World.cpp or in Body.cpp (TBD).
        virtual void draw() = 0; 
        void setPosition(float, float, float);
        void setPosition(btVector3 position);
        const btVector3 getPosition() const;

    private:
        btVector3 position;
};
