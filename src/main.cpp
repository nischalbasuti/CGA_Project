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
bool isWire = false;
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
bool isAstroidMoving = false;
int currentScene = 1;
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

SphereGlBody cameraGlBody(.0001);

SphereGlBody planet2AtmosphereGlBody(6.3);
SphereGlBody sunAtmosphereGlBody(21);
SphereGlBody backgroundGlBody(250);

btScalar mass = 1;
btScalar mass1 = 20;

Body sunBody(&sunGlBody, new btVector3(0, 0, 0), new btSphereShape(20), mass1, false);
PlanetBody moonBody(90, &moonGlBody, new btVector3(0, 0, 90), new btSphereShape(2), mass, false);
PlanetBody planet0Body(30, &planet0GlBody, new btVector3(0, 0, 30), new btSphereShape(2), mass, false);
PlanetBody planet1Body(60, &planet1GlBody, new btVector3(0, 0, 60), new btSphereShape(3), mass, false);
PlanetBody planet2Body(100, &planet2GlBody, new btVector3(0, 0, 80), new btSphereShape(6), mass, false);
PlanetBody planet3Body(200, &planet3GlBody, new btVector3(0, 0, 100), new btSphereShape(4), mass, false);
PlanetBody astroidBody(150, &astroidGlBody, new btVector3(-80, 80, 100), new btSphereShape(1), mass, false);

PlanetBody cameraBody(150, &cameraGlBody, new btVector3(-0, 0, 0), new btSphereShape(1), mass, false);
// Initialization routine.
void setup() {
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
	planet0GlBody.setColor(0, 0, 1, 1);
	planet1GlBody.setColor(0, 1, 1, 1);
	planet2GlBody.setColor(1, 0, 1, 1);
	planet3GlBody.setColor(1, 1, 0, 1);
	sunGlBody.setColor(255.f/255.f, 140.f/255.f, 0.f, 0.8f);
	astroidGlBody.setColor(1,1,1,1);
	planet2AtmosphereGlBody.setColor(0.f, 0.f, 0.f, 0.2);
	sunAtmosphereGlBody.setColor(0.f, 0.f, 0.f, 0.3f);
	backgroundGlBody.setColor(0.f, 0.f, 0.f, 0.3f);
	cameraGlBody.setColor(0.f, 0.f, 0.f, 0.0f);

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
	cameraGlBody.setTexture(texture[6]);

	// Add bodies to world.
    world.addBody(sunBody);
    world.addBody(planet0Body);
    world.addBody(planet1Body);
    world.addBody(planet2Body);
    world.addBody(planet3Body);
	world.addBody(moonBody);
	world.addBody(astroidBody);
    world.addBody(cameraBody);

	animate(1);
}

float angleX = 0;
float angleY = 0;
float angleZ = 0;

int globalTimeStep = 0;

// Drawing routine.
void drawSceneOne() {
    if(globalTimeStep >= 988) {
        isAstroidMoving = true;
    }
	auto lookAt = planet2Body.getPosition();
	auto eye = cameraBody.getPosition();

    auto velocityVector = btVector3(
            ( planet2Body.getFuturePosition(8).getX()-astroidBody.getPosition().getX() ),
			( planet2Body.getFuturePosition(8).getY()-astroidBody.getPosition().getY() ),
			( planet2Body.getFuturePosition(8).getZ()-astroidBody.getPosition().getZ() )).normalize();

    // Move the "camera".
	if (isAstroidMoving) {
        astroidBody.getRigidBody()->activate(true);
        astroidBody.getRigidBody()->setLinearVelocity(velocityVector * 60);
    }
    auto velocityVectorCamera = btVector3(
            ( astroidBody.getPosition().getX()-cameraBody.getPosition().getX() + 5),
            ( astroidBody.getPosition().getY()-cameraBody.getPosition().getY() - 0),
            ( astroidBody.getPosition().getZ()-cameraBody.getPosition().getZ() + 5)).normalize();
    eye = cameraBody.getPosition();
    cameraBody.getRigidBody()->activate(true);
    if (isAstroidMoving) {
        cameraBody.getRigidBody()->setLinearVelocity(velocityVectorCamera*60);
    }
    else {
        cameraBody.getRigidBody()->setLinearVelocity(velocityVectorCamera*10);
    }
    gluLookAt(
            eye.getX(), eye.getY(), eye.getZ(),
            lookAt.getX(), lookAt.getY(), lookAt.getZ(),
            0.0, 1.0, 0.0
            );


	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);

	// update and draw bodies.
	world.drawBodies();

    // astroidBody and planet2Body collide
    // TODO: MAGIC NUMBERS!!!! OMG!!!
    if(astroidBody.getPosition().distance(planet2Body.getPosition()) < 6.5 ) {
        // Zoom into earth 
        // TODO: cut to diffrent scene.
        currentScene = 2;
        cameraBody.getRigidBody()->activate(true);
        auto velocityVectorCamera = btVector3(
                ( astroidBody.getPosition().getX()-cameraBody.getPosition().getX() ),
                ( astroidBody.getPosition().getY()-cameraBody.getPosition().getY() ),
                ( astroidBody.getPosition().getZ()-cameraBody.getPosition().getZ() )).normalize();
        cameraBody.getRigidBody()->setLinearVelocity(velocityVectorCamera*100);
        eye = cameraBody.getPosition();
    } else if(isAstroidMoving){
        astroidGlBody.drawTail(1, 0.5);
    }

    // cameraBody and planet2Body collide
    if(cameraBody.getPosition().distance(planet2Body.getPosition()) < 6.25 ) {
       // isAnimating = false; 
       astroidBody.setPosition(0,0,0);
    }

	// planet2AtmosphereGlBody.draw(planet2Body.getPosition());
	glDisable(GL_LIGHTING);
	planet2AtmosphereGlBody.draw(planet2Body.getPosition());

	sunAtmosphereGlBody.draw(sunBody.getPosition());
	backgroundGlBody.draw();
	glEnable(GL_LIGHTING);
}

// Control points for the texture coordinates Bezier surface.
static float texturePoints[2][2][2] =
{
	{ { 0.0, 0.0 },{ 0.0, 1.0 } },
	{ { 1.0, 0.0 },{ 1.0, 1.0 } }
};
float height1 = 1.0f;
float height2 = 2.0f;
float pettleControlPoints[16][3] = {
    {0.0f,0.0f,0.0f}, { 1.0f,0.0f,height2},
    {2.0f,0.0f,0.0f},  {3.0f,0.0f,0.0f},     
    {0.0f,1.0f,height2},  {1.0f,1.0f,height1},
    {2.0f,1.0f,0.0f},  {3.0f,1.0f,0.0f},     
    {0.0f,2.0f,0.0f},  {1.0f,2.0f,0.0f},
    {2.0f-2,2.0f, 0.0f},  {3.0f,2.0f,0.0f},
    {0.0f,3.0f,0.0f},  {1.0f,3.0f,0.0f},
    {2.0f,3.0f,0.0f},   {3.0f,3.0f,0.0f}
};    
float pettle2ControlPoints[16][3] = {
    {0.0f,0.0f,0.0f},  {1.0f,0.0f,0.0f},
    {2.0f,0.0f,2.0f},  {3.0f,0.0f,0.0f},     
    {0.0f,1.0f,0.0f},  {1.0f,1.0f,1.5f},
    {2.0f+8,1.0f,0.0f},  {3.0f,1.0f+1,0.0f},     
    {0.0f,2.0f,2.0f},  {1.0f,2.0f+8,4.0f},
    {2.0f,2.0f,50.0f},  {3.0f,2.0f,2.0f},
    {0.0f,3.0f,0.0f},  {1.0f,3.0f+2,0.0f},
    {2.0f,3.0f,2.0f},  {3.0f,3.0f,200.0f}
};    
float leafControlPoints[16][3] = {
    {0.0f,0.0f,0.0f},  {1.0f,0.0f,0.0f},
    {2.0f,0.0f,2.0f},  {3.0f,0.0f,0.0f},     
    {0.0f,1.0f,0.0f},  {1.0f,1.0f,1.5f},
    {2.0f+8,1.0f,0.0f},  {3.0f,1.0f+1,0.0f},     
    {0.0f,2.0f,2.0f},  {1.0f,2.0f+8,4.0f},
    {2.0f,2.0f,20.0f},  {3.0f,2.0f,2.0f},
    {0.0f,3.0f,0.0f},  {1.0f,3.0f+2,0.0f},
    {2.0f,3.0f,2.0f},  {3.0f,3.0f,200.0f}
};    

void drawPettle(
        float *cp,
        float zAngle = 0,
        float x=0,
        float y=0,
        float z=0,
        int textureIndex=0,
        float alpha = 1) {
	// Specify and enable the Bezier curve.
    // Control points for the texture coordinates Bezier surface.
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

	// Draw the Bezier curve by defining a sample grid and evaluating on it.
	// Enable Bezier surface with texture co-ordinates generation.
    glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3 , 4, 0.0f, 1.0f, 12, 4, cp);
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, texturePoints[0][0]);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	// Map the texture onto the blade Bezier surface.
	glBindTexture(GL_TEXTURE_2D, texture[textureIndex]);

    // Set the color, needed for transulcency
    glEnable(GL_COLOR_MATERIAL);
        glColor4f(1, 1, 1, alpha);
    glDisable(GL_COLOR_MATERIAL);

    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(zAngle, 0, 0, 1);
        if(isWire) {
            glEvalMesh2(GL_LINE, 0, 5, 0, 5);
        }
        else {
            // Enable texturing and filled polygon mode.
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glEvalMesh2(GL_FILL, 0, 5, 0, 5);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    glPopMatrix();
}

void drawFlower(float pistilRadius, float stemRadius, float stemHeight,
                GLUquadric* quad,
                float xPos = 0, float yPos = 0, float zPos = 0) {
    glTranslatef(xPos, yPos, zPos);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(isWire)
        gluQuadricDrawStyle(quad, GLU_LINE);
    else
        gluQuadricDrawStyle(quad, GLU_FILL);
    gluQuadricTexture(quad, GL_TRUE);
    /* // Draw pistil............................................................. */
    glEnable(GL_COLOR_MATERIAL);
        glColor4f(1 , 1, 1, 1.0);
    glDisable(GL_COLOR_MATERIAL);


        glBindTexture(GL_TEXTURE_2D, texture[3]);

        gluSphere(quad, pistilRadius, 10, 10);


    // Draw Stem...............................................................
    glEnable(GL_COLOR_MATERIAL);
        glColor4f(1 , 1, 1, 0.8);
    glDisable(GL_COLOR_MATERIAL);
        glPushMatrix();
            glTranslatef(0, 0, -stemHeight);

            glBindTexture(GL_TEXTURE_2D, texture[1]);
            
            gluCylinder(quad, stemRadius, stemRadius, stemHeight, 10, 10);
        glPopMatrix();
    gluDeleteQuadric(quad);

    // Draw pettles along the parameter for the pistil.........................
    float t = 0;
    int count = 16;
    count = 1;
    count = 8;
    float x[count];
    float y[count];
    float z[count];
    float angle[count];
    float R = pistilRadius;

    for (int i = 0; i < count; i++) {
        angle[i] = t * 57.2958; //convert to degrees
        x[i] = R * cos(t);
        y[i] = R * sin(t);
        z[i] = 0;
        t += 2 * PI / (float)count;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawPettle(pettle2ControlPoints[0], count/2 + angle[i], x[i], y[i], z[i], 1, 0.8);
        drawPettle(leafControlPoints[0], count/2 + angle[i] + 15, x[i], y[i], z[i], 2);
        drawPettle(pettleControlPoints[0], count/2 + angle[i], x[i], y[i], z[i], 0, 0.8);
    }
}

float pistilRadius = 0.25;
float stemRadius = 0.10;
float stemHeight = 3;
void drawFlowerAndReflection(float x, float y, float z) {
    glPushMatrix();
        glTranslatef(x, y, z);
        drawFlower(pistilRadius, stemRadius, stemHeight, gluNewQuadric());

        glPushMatrix();
            glTranslatef(0, 0, -stemHeight*2);
            glRotatef(180, 1, 0, 0);
            drawFlower(pistilRadius, stemRadius, stemHeight, gluNewQuadric());
        glPopMatrix();
    glPopMatrix();
}

float astroidPosition = 150;
float explotionRadius = 1.0f;
bool toDraw = true;
float z = 10;
void drawSceneTwo() {
	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE); // Enable local viewpoint.

	glEnable(GL_LIGHT1); // Enable particular light source.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE); // Enable local viewpoint.

    // setupSceneTwoLighting(); // find a more effecient way to do this.
    // sunGlBody.draw(); //use as placeholder explosion.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, z, astroidPosition, astroidPosition, -astroidPosition, 0.0, 1.0, 0.0);

    if(z < 27) {
        z += 0.1;
    }

    if(astroidPosition > 0) {
        astroidBody.setPosition(astroidPosition,astroidPosition,-astroidPosition);
        astroidGlBody.draw();
        astroidGlBody.drawTail(1, 0.5);
        astroidPosition-=1;
    } else{
        sunGlBody.draw(explotionRadius);
        explotionRadius += 0.5;
    }

    angleY = 180;
    angleX = 100;
    // std::cout << angleX << " ";
    // std::cout << angleY << " ";
    // std::cout << angleZ << std::endl;

    backgroundGlBody.draw();
    backgroundGlBody.setTexture(6);

    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    glRotatef(angleZ, 0.0, 0.0, 1.0);

    std::cout << explotionRadius << std::endl;
    // Draw some flowers and their "reflections".
    if(explotionRadius < 17){
        drawFlowerAndReflection(0,0,0);
        drawFlowerAndReflection(12,0,0);
        drawFlowerAndReflection(-12,0,0);
        drawFlowerAndReflection(0,10,0);
        drawFlowerAndReflection(0,-10,0);

        drawFlowerAndReflection(10,14,0);
        drawFlowerAndReflection(-10,14,0);
        drawFlowerAndReflection(16,12,0);
        drawFlowerAndReflection(-16,12,0);

        drawFlowerAndReflection(10,-14,0);
        drawFlowerAndReflection(-10,-14,0);
        drawFlowerAndReflection(16,-12,0);
        drawFlowerAndReflection(-16,-12,0);
    }

    // Draw the "reflective" surface.
    glEnable(GL_COLOR_MATERIAL);
        glColor4f(1.0 , 1.0, 1.0, 0.8); // Make transparent to display "reflections".
    glDisable(GL_COLOR_MATERIAL);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTranslatef(0, -50, 0);
    glBegin(GL_POLYGON);
        glNormal3f(0,0,1); glTexCoord2f(0.0, 0.0); glVertex3f(-200.0, 0.0, -stemHeight);
        glNormal3f(0,1,0); glTexCoord2f(1.0, 0.0); glVertex3f(200.0, 0.0, -stemHeight);
        glNormal3f(1,0,0); glTexCoord2f(1.0, 1.0); glVertex3f(200.0, 240.0, -stemHeight);
        glNormal3f(0,0,1); glTexCoord2f(0.0, 1.0); glVertex3f(-200.0, 240.0, -stemHeight);
    glEnd();

}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();
    switch (currentScene) {
        case 1:
            drawSceneOne();
            break;
        case 2:
            drawSceneTwo();
            break;
        default:
            std::cout << "scene not defined." << std::endl;
    }
	glutSwapBuffers();
}

// TODO: Fixed time step, check if it's framerate independent.
static int animationPeriod = 20; // Time interval between frames.

// Timer function.
void animate(int value) {
	if (!isAnimating) {
		return;
	}
    globalTimeStep += 1;
    std::cout << globalTimeStep << std::endl;

	glutTimerFunc(animationPeriod, animate, 1);
	world.getDynamicsWorld()->stepSimulation(1 / 60.f, 10);

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
        case 'p':
            isAstroidMoving = true;
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
