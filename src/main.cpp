#include <iostream>
#include <cmath>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "getBMP.h"
#include "world.h"
#include "body.h"
#include "abstractGlBody.h"
#include "sphereGlBody.h"
#include "boxGlBody.cpp"
#include "planetBody.cpp"
#include "astroidGlBody.h"

#define PI 3.14

// function definitions//////////////////////////////////////////
void loadTexture(imageFile*, int);
void loadTextures();
void setupLighting();
void setupTextures();
void setup();
void drawScene();
void resize(int, int);
void keyInput(unsigned char, int, int);
void animate(int);
// end function definitions//////////////////////////////////////

// global variables /////////////////////////////////////////////
World world(0);
static unsigned int texture[7]; // Array of texture indices.
bool isAnimating = true;
// end globals //////////////////////////////////////////////////

void loadTexture(imageFile* image, int textureId) {
	// Bind can label image to texture[textureId]. 
	glBindTexture(GL_TEXTURE_2D, texture[textureId]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
		         GL_RGBA, GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
// Load external textures.
void loadTextures() {
	// Local storage for bmp image data.
	imageFile *image[7];

	// Load the images.
	image[0] = getBMP("../res/textures/planet0.bmp");
	image[1] = getBMP("../res/textures/planet1.bmp");
	image[2] = getBMP("../res/textures/planet2.bmp");
	image[3] = getBMP("../res/textures/planet3.bmp");
	image[4] = getBMP("../res/textures/sun.bmp");
	image[5] = getBMP("../res/textures/sky.bmp");
	// image[6] = getBMP("../res/textures/crab_nebula.bmp");
	image[6] = getBMP("../res/textures/milky_way.bmp");
	loadTexture(image[0], 0);
	loadTexture(image[1], 1);
	loadTexture(image[2], 2);
	loadTexture(image[3], 3);
	loadTexture(image[4], 4);
	loadTexture(image[5], 5);
	loadTexture(image[6], 6);
}

void setupLighting() {
	// Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightPos[] = { 0.0, 0.0, 1.0, 0.0 };
	float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

	// Light properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR); // Enable separate specular light calculation.

	// Light properties.
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

	glEnable(GL_LIGHT1); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR); // Enable separate specular light calculation.

	// Material property vectors.
	float matAmbAndDif[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 100.0 };

	// Material properties.
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
}

void setupTextures() {
	glGenTextures(6, texture); // generate texture ids
	loadTextures(); // load external textures

	// Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

// Define bodies.
SphereGlBody sunGlBody(20);
SphereGlBody moonGlBody(2);
SphereGlBody planet0GlBody(2);
SphereGlBody planet1GlBody(3);
SphereGlBody planet2GlBody(6);
SphereGlBody planet3GlBody(4);
AstroidGlBody astroidGlBody(1);

SphereGlBody planet2AtmosphereGlBody(6.3);
SphereGlBody sunAtmosphereGlBody(21);
SphereGlBody backgroundGlBody(250);

btScalar mass = 1;
btScalar mass1 = 2000000;

Body sunBody(&sunGlBody, new btVector3(0, 0, 0), new btSphereShape(20), mass1, false);
PlanetBody moonBody(90, &moonGlBody, new btVector3(0, 0, 90), new btSphereShape(2), mass, false);
PlanetBody planet0Body(30, &planet0GlBody, new btVector3(0, 0, 30), new btSphereShape(2), mass, false);
PlanetBody planet1Body(60, &planet1GlBody, new btVector3(0, 0, 60), new btSphereShape(3), mass, false);
PlanetBody planet2Body(80, &planet2GlBody, new btVector3(0, 0, 80), new btSphereShape(6), mass, false);
PlanetBody planet3Body(100, &planet3GlBody, new btVector3(0, 0, 100), new btSphereShape(4), mass, false);
PlanetBody astroidBody(150, &astroidGlBody, new btVector3(150, 150, 150), new btSphereShape(1), mass, false);

// Initialization routine.
void setup(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// glClearColor(1.0, 1.0, 1.0, 0.0);
	// glClearColor(25.0/255.0, 25.0/255.0, 112.0/255.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setupLighting();
    setupTextures();

	// Set colors of glBodies
	planet0GlBody.setColor(0,0,1,1);
	planet1GlBody.setColor(0,1,1,1);
	planet2GlBody.setColor(1,0,1,1);
	planet3GlBody.setColor(1,1,0,1);
	sunGlBody.setColor(255/255,140/255,0,0.8);
	astroidGlBody.setColor(1,1,1,1);
	planet2AtmosphereGlBody.setColor(0,0,0,0.2);
	sunAtmosphereGlBody.setColor(0,0,0,0.3);
	backgroundGlBody.setColor(0,0,0,0.3);


	// Set textures of glBodies
	planet0GlBody.setTexture(texture[0]);
	planet1GlBody.setTexture(texture[1]);
	planet2GlBody.setTexture(texture[2]);
	planet3GlBody.setTexture(texture[3]);
	sunGlBody.setTexture(texture[4]);
	moonGlBody.setTexture(texture[0]);
	astroidGlBody.setTexture(texture[0]);
	planet2AtmosphereGlBody.setTexture(texture[5]);
	sunAtmosphereGlBody.setTexture(texture[4]);
	backgroundGlBody.setTexture(texture[6]);

	// Add bodies to world.
    world.addBody(sunBody);
    world.addBody(planet0Body);
    world.addBody(planet1Body);
    world.addBody(planet2Body);
    world.addBody(planet3Body);
	world.addBody(moonBody);
	world.addBody(astroidBody);

	astroidBody.getRigidBody()->setLinearVelocity(btVector3(0-20,0-20,-20));
	astroidBody.getRigidBody()->activate(true);

	animate(1);
}

float angleX = 0;
float angleY = 0;
float angleZ = 0;

float X=100, Y=100, Z=100;
// Drawing routine.
void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();

	// auto lookAt = btVector3(0, 0, 0);
	auto lookAt = planet2Body.getPosition();
	// auto lookAt = astroidBody.getPosition();
	auto eye = btVector3(X, Y, Z);

    auto velocityVector = btVector3(
			( planet2Body.getPosition().getX()-astroidBody.getPosition().getX() ),
			( planet2Body.getPosition().getY()-astroidBody.getPosition().getY() ),
			( planet2Body.getPosition().getZ()-astroidBody.getPosition().getZ() )).normalize();

    // Move the "camera".
	if (Z > -100) {
		// X -= 0.35;
		X = lookAt.getX() + 5;
		/* Y -= 0.35; */
		Y = lookAt.getY() + 15;
		Z -= 0.25;
		/* std::cout << Y << std::endl; */
        
        // Set velocity of the astroid.
        astroidBody.getRigidBody()->setLinearVelocity(velocityVector * 30);
    } else {
        // Increase the velocity of the astroid enough to collide with planet2
        // once the camera settles down.
        astroidBody.getRigidBody()->setLinearVelocity(velocityVector * 60);
    }

	// eye.setX(lookAt.getX() - 20);
	// eye.setY(lookAt.getY() - 20);
	// eye.setZ(lookAt.getZ() - 20);

	astroidBody.getRigidBody()->activate(true);

	// glRotatef(planet0Body.rotationY, 0, 1, 0);
	gluLookAt(
		eye.getX(), eye.getY(), eye.getZ(),
		lookAt.getX(), lookAt.getY(), lookAt.getZ(),
		0.0, 1.0, 0.0
		);

	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);

	// btTransform trans =  sunBody.getRigidBody()->getWorldTransform();
	// glLightfv(GL_LIGHT0, GL_POSITION, trans.getOrigin());
	float lightPos0[4] = {0,0,0,1};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	float lightPos1[4] = {0,0,0,1};
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	// update and draw bodies.
	// world.update();
	world.drawBodies();
    astroidGlBody.drawTail();

    // TODO: MAGIC NUMBERS!!!! OMG!!!
    if(astroidBody.getPosition().distance(planet2Body.getPosition()) < 6.5 ) {
       isAnimating = false; 
    }

	// planet2AtmosphereGlBody.draw(planet2Body.getPosition());
	glDisable(GL_LIGHTING);
	planet2AtmosphereGlBody.draw(planet2Body.getPosition());

	sunAtmosphereGlBody.draw(sunBody.getPosition());
	backgroundGlBody.draw();
	glEnable(GL_LIGHTING);

	// sunBody.draw();
	// planet0Body.draw();
	// planet1Body.draw();
	// moonBody.draw();
	// planet2Body.draw();
	// planet3Body.draw();


	glutSwapBuffers();
}

// TODO: Fixed time step, check if it's framerate independent.
static int animationPeriod = 20; // Time interval between frames.

// Timer function.
void animate(int value) {
	if (!isAnimating) {
		return;
	}
	glutTimerFunc(animationPeriod, animate, 1);
	world.getDynamicsWorld()->stepSimulation(1 / 60.f, 10);

	/* planet3Body.rotationY += 0.1; */
	/* planet2Body.rotationY += 0.2; */
	/* planet1Body.rotationY += 0.3; */
	/* planet0Body.rotationY += 0.4; */
	/* moonBody.rotationY += 1; */


	moonBody.getRigidBody()->activate(true);
	planet0Body.getRigidBody()->activate(true);
	planet1Body.getRigidBody()->activate(true);
	planet2Body.getRigidBody()->activate(true);
	planet3Body.getRigidBody()->activate(true);
	sunBody.getRigidBody()->activate(true);
	
    // Rotate bodies.
	moonBody.getRigidBody()->setAngularVelocity(btVector3(1, 0, 0));
	planet0Body.getRigidBody()->setAngularVelocity(btVector3(1, 0, 0));
	planet1Body.getRigidBody()->setAngularVelocity(btVector3(1, 1, 0));
	planet2Body.getRigidBody()->setAngularVelocity(btVector3(0, -1, 0));
	planet3Body.getRigidBody()->setAngularVelocity(btVector3(1, 1, 1));
	sunBody.getRigidBody()->setAngularVelocity(btVector3(0, 1, 0));

    // Make objects revolve.
	planet0Body.revolve(1000/4);
	planet1Body.revolve(2000/4);
	planet2Body.revolve(3000/4);
	planet3Body.revolve(4000/4);

    // Revolve moon around planet2.
	float x = planet2Body.getPosition().getX();
	float y = planet2Body.getPosition().getY();
	float z = planet2Body.getPosition().getZ();
	moonBody.revolve(200, 10, x, y, z);

	glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, (float)w / (float)h, 1.0, 400.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// glViewport(0, 0, w, h);
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
    x = 0;
    y = 0;

	// float Z = ( ( Y-y1 ) / m ) - z1;
	switch (key) {
        case 27:
            exit(0);
            break;
		case ' ':
			isAnimating = !isAnimating;
			if (isAnimating){
				animate(1);
			}
			break;
		case 'w':
			X-=1;
			Y-=1;
			Z-=1;
			glutPostRedisplay();
			break;
		case 's':
			Y+=1;
			X+=1;
			Z+=1;
			glutPostRedisplay();
			break;
		case 'l':
			// planetBody.getRigidBody()->applyCentralImpulse(btVector3( 2,0,0 ));
			break;	
		case 'j':
			// planet0Body.getRigidBody()->applyTorqueImpulse(btVector3(-2,0,0));
			// planetBody.getRigidBody()->applyCentralImpulse(btVector3( -2,0,0 ));
			break;	
		case 'x':
			angleX +=5;
			glutPostRedisplay();
			break;
		case 'y':
			angleY +=5;
			glutPostRedisplay();
			break;
		case 'z':
			angleZ +=5;
			glutPostRedisplay();
			break;
		case 'X':
			angleX -=5;
			glutPostRedisplay();
			break;
		case 'Y':
			angleY -=5;
			glutPostRedisplay();
			break;
		case 'Z':
			angleZ -=5;
			glutPostRedisplay();
			break;
        default:
			break;
	}
}

// Main routine.
int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(720, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("main1.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
