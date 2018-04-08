#pragma once
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include "body.h"

class World {
    public:
        World(float gravity);
        ~World();
        void addBody(Body &body);
        void removeBody(Body &body);
        void update();
        void drawBodies();  // loop through all bodies and update the GlBody's transformations and draw them.
        btDiscreteDynamicsWorld* getDynamicsWorld() const;
    private:
        std::vector<Body*> bodies;
        btDiscreteDynamicsWorld* dynamicsWorld;
};
