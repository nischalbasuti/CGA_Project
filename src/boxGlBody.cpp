#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "getBMP.h"
#include "world.h"
#include "body.h"
#include "abstractGlBody.h"

class BoxGlBody : public AbstractGlBody {
	public:
		BoxGlBody(float width, float height=1, float depth=1) {
			this->width = width;
			this->height = height;
			this->depth = depth;

			this->rgba[0] = 0;
			this->rgba[1] = 1;
			this->rgba[2] = 2;
			this->rgba[3] = 3;
		}
		void setColor(float r, float g, float b, float a=1) {
			this->rgba[0] = r;
			this->rgba[1] = g;
			this->rgba[2] = b;
			this->rgba[3] = a;
		}
		void draw() {
			glPushMatrix();
				glColor3fv(this->rgba);
				// glutSolidCube(width); // TODO make cuboid, duh
				// Below is code forked from glutWireCube to make a cuboid
				double sizeW = this->width * 0.5;
				double sizeH = this->height * 0.5;
				double sizeD = this->depth * 0.5;

				#   define V(a,b,c) glVertex3d( a sizeW, b sizeH, c sizeD );
				#   define N(a,b,c) glNormal3d( a, b, c );
					/* PWO: I dared to convert the code to use macros... */
					glBegin( GL_POLYGON );
						 N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+); 
					glEnd();
					glBegin( GL_POLYGON ); 
						N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+); 
					glEnd();
					glBegin( GL_POLYGON ); 
						N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+); 
					glEnd();
					glBegin( GL_POLYGON ); 
						N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-); 
					glEnd();
					glBegin( GL_POLYGON ); 
						N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+); 
					glEnd();
					glBegin( GL_POLYGON ); 
						N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-); 
					glEnd();
				#   undef V
				#   undef N
			glPopMatrix();
		}
	private:
		float width, height, depth;
		float rgba[4];
};
