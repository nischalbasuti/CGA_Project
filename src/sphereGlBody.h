#pragma once

#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "getBMP.h"
#include "world.h"
#include "body.h"
#include "abstractGlBody.h"

class SphereGlBody : public AbstractGlBody {
    public:
		SphereGlBody(float); 
		void setColor(float, float, float, float);
		void setTexture(int);
        void draw();
        void draw(float);
		void draw(btVector3);
    private:
		float radius;
		float rgba[4];
		float textureId;
		bool isTextureBound = false;
};
