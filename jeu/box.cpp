#include "box.h"


box::box(float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical, btScalar mass, float plargeur, float phauteur, float r, float g, float b, float a)
{
	Objet3DDeformable(positionX, positionY, positionZ, angleHorizontal, angleVertical, mass);
	this->largeur = plargeur;
	this->hauteur = phauteur;
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

	// Chargement de la texture du mur
	this->conteneurTextures.ajouter("box.bmp");

	// On d�clare une forme et on l'initialise en tant que boite de la taille largeur,largeur,hauteur (x, y, z)
	shape = new btBoxShape(btVector3(largeur, largeur, hauteur));

	//On initialise notre btTransform et on lui dit une position (la position de la boite)
	transform.setIdentity();
	transform.setOrigin(btVector3(positionX, positionY, positionZ));


	this->localInertia = btVector3(0, 0, 0);
	shape->calculateLocalInertia(mass, this->localInertia);

	// Il est conseill� d'utiliser motionState car il fournit des capacit�s d'interpolation et synchronise seulement les objets "actifs".
	motionState = new btDefaultMotionState(transform);
	//On regroupe les informations de la boite � partir de la masse, l'inertie et cetera
	btRigidBody::btRigidBodyConstructionInfo myBoxRigidBodyConstructionInfo(mass, motionState, shape, localInertia);
	//On construis le corps de la boite � partir de l'information regroup�e
	body = new btRigidBody(myBoxRigidBodyConstructionInfo);

	this->body->setUserPointer(this);
}


box::~box()
{
	this->conteneurTextures.supprimer("box.bmp");

	delete motionState;
	delete shape;
	//delete body;
}


void box::dessiner()
{
	// On recup�re la matrice OpenGL transform�e par Bullet qu'on appliquera � notre boite
	motionState->m_graphicsWorldTrans.getOpenGLMatrix(matrix);
	
	// On affiche notre boite avec les transformations appliqu�es gr�ce � la matrice
	glPushMatrix();
	glMultMatrixf(matrix);
	dessinerBox(this->largeur, this->largeur, this->hauteur);
	glPopMatrix();
}


void box::dessinerBox(float x, float y, float z)
{


	// Activation des textures
	glEnable(GL_TEXTURE_2D);

	// Selection de la texture du mur
	glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture("box.bmp").texture);

	glPushMatrix();
	glScalef(x, y, z);
	glBegin(GL_QUADS);

	//face 1
	glColor4f(this->r, this->g, this->b, this->a);

	glNormal3i(-1, 1, -1);
	glTexCoord2i(1, 1); glVertex3i(-1, 1, -1); glTexCoord2i(1, 0); glVertex3i(1, 1, -1);
	glTexCoord2i(0, 0); glVertex3i(1, -1, -1); glTexCoord2i(0, 1); glVertex3i(-1, -1, -1);
	//face 2
	glNormal3i(-1, -1, -1);
	glTexCoord2i(1, 1); glVertex3i(-1, -1, -1); glTexCoord2i(1, 0); glVertex3i(1, -1, -1);
	glTexCoord2i(0, 0); glVertex3i(1, -1, 1); glTexCoord2i(0, 1); glVertex3i(-1, -1, 1);
	// face 3
	glNormal3i(1, -1, 1);
	glTexCoord2i(1, 1); glVertex3i(1, -1, 1); glTexCoord2i(1, 0); glVertex3i(1, -1, -1);
	glTexCoord2i(0, 0); glVertex3i(1, 1, -1); glTexCoord2i(0, 1); glVertex3i(1, 1, 1);
	//face 4
	glNormal3i(1, 1, -1);
	glTexCoord2i(1, 1); glVertex3i(1, 1, -1); glTexCoord2i(1, 0); glVertex3i(-1, 1, -1);
	glTexCoord2i(0, 0); glVertex3i(-1, 1, 1); glTexCoord2i(0, 1); glVertex3i(1, 1, 1);
	//face 5
	glNormal3i(-1, 1, 1);
	glTexCoord2i(1, 1); glVertex3i(-1, 1, 1); glTexCoord2i(1, 0); glVertex3i(-1, 1, -1);
	glTexCoord2i(0, 0); glVertex3i(-1, -1, -1); glTexCoord2i(0, 1); glVertex3i(-1, -1, 1);
	//face 6
	glNormal3i(1, -1, 1);
	glTexCoord2i(1, 1); glVertex3i(1, -1, 1); glTexCoord2i(1, 0); glVertex3i(1, 1, 1);
	glTexCoord2i(0, 0); glVertex3i(-1, 1, 1); glTexCoord2i(0, 1); glVertex3i(-1, -1, 1);
	glEnd();
	glPopMatrix();

	// On remet la couleur � la normale ( �vite de bleuter les textures )
	glColor4f(1, 1, 1, 1);

}