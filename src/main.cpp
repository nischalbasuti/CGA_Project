#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "getBMP.h"
#include "world.h"
#include "body.h"
#include "abstractGlBody.h"
#include "sphereGlBody.h"
#include "boxGlBody.cpp"

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
World world(10);
static unsigned int texture[4]; // Array of texture indices.
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
	imageFile *image[4];

	// Load the images.
	image[0] = getBMP("./pettle.bmp");
	image[1] = getBMP("./green_leaf.bmp");
	image[2] = getBMP("./leaf.bmp");
	image[3] = getBMP("./pistil.bmp");
    loadTexture(image[0], 0);
    loadTexture(image[1], 1);
    loadTexture(image[2], 2);
    loadTexture(image[3], 3);
}

void setupLighting() {
	// Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightPos[] = { 5.0, 5.0, -1.0, 0.0 };
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

	// Material property vectors.
	float matAmbAndDif[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };

	// Material properties.
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
}

void setupTextures() {
	glGenTextures(4, texture); // generate texture ids
	loadTextures(); // load external textures

	// Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


// Define bodies.
SphereGlBody abstractGlBody(1);
BoxGlBody boxGlBody(20,2,2);
btScalar mass = 1;
btScalar mass1 = 2;

Body dynamicBody(&abstractGlBody, new btVector3(0, 10, 0), new btSphereShape(1), mass, false);
Body dynamicBody1(&abstractGlBody, new btVector3(0.5, 5, 0), new btSphereShape(1), mass1, false);
Body staticBody(&boxGlBody, new btVector3(0.5, 0, 0), new btBoxShape(btVector3( 10,1,1 )), 0, true);
// Initialization routine.
void setup(void) {
	/* glClearColor(0.0, 0.0, 0.0, 0.0); */
	/* glClearColor(1.0, 1.0, 1.0, 0.0); */
	glClearColor(25.0/255.0, 25.0/255.0, 112.0/255.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);

    setupLighting();
    /* setupTextures(); */

	abstractGlBody.setColor(1,0,0,1);

	// Set coeffs of restitution.
	dynamicBody.getRigidBody()->setRestitution(0.8);
	dynamicBody1.getRigidBody()->setRestitution(0.5);
	staticBody.getRigidBody()->setRestitution(1);

	dynamicBody.getRigidBody()->setFriction(1);
	dynamicBody1.getRigidBody()->setFriction(1);
	staticBody.getRigidBody()->setFriction(1);

	dynamicBody.getRigidBody()->setRollingFriction(0.2);
	dynamicBody1.getRigidBody()->setRollingFriction(0.2);

	// Add bodies to world.
    world.addBody(dynamicBody);
    world.addBody(dynamicBody1);
    world.addBody(staticBody);
	// dynamicBody1.getRigidBody()->applyCentralForce(btVector3(0,20,0));
	animate(1);
}

// Drawing routine.
void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();

	gluLookAt(0.0, 10.0, 16.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	btTransform trans =  dynamicBody1.getRigidBody()->getWorldTransform();
	// glLightfv(GL_LIGHT0, GL_POSITION, trans.getOrigin());
	// update and draw bodies.
	world.drawBodies();
	// std::cout << dynamicBody1.getRigidBody()->getLinearVelocity().getX() << std::endl;

	glutSwapBuffers();
}

// TODO: properly do time step.
bool isAnimating = true;
static int animationPeriod = 10; // Time interval between frames.
// Timer function.
void animate(int value) {
	if (!isAnimating) {
		return;
	}
	glutTimerFunc(animationPeriod, animate, 1);
	world.getDynamicsWorld()->stepSimulation(1 / 60.f, 10);
	glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// glViewport(0, 0, w, h);
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
    x = 0;
    y = 0;
	switch (key) {
        case 27:
            exit(0);
            break;
		case 'l':
			dynamicBody1.getRigidBody()->activate(true);
			dynamicBody.getRigidBody()->activate(true);
			dynamicBody1.getRigidBody()->applyCentralImpulse(btVector3( 2,0,0 ));
			break;	
		case 'j':
			dynamicBody1.getRigidBody()->activate(true);
			dynamicBody.getRigidBody()->activate(true);
			dynamicBody1.getRigidBody()->applyCentralImpulse(btVector3( -2,0,0 ));
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
	glutInitWindowSize(500, 500);
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
