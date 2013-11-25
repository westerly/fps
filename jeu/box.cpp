#include "box.h"


box::box(float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical, btScalar mass, float plargeur, float phauteur, float r, float g, float b)
{
	Objet3DDeformable(positionX, positionY, positionZ, angleHorizontal, angleVertical, mass);
	this->largeur = plargeur;
	this->hauteur = phauteur;
	this->r + r;
	this->g = g;
	this->b = b;

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
	// On ne lib�re pas l'attribut body du tas, c'est le physicEngine qui s'en charge lorsque il lib�re de la m�moire l'attibut world
	delete motionState;
	delete shape;
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

	
	glPushMatrix();
	glScalef(x, y, z);
	glBegin(GL_QUADS);

	//face 1
	glColor3ub(this->r, this->g, this->b);
	glNormal3i(-1, 1, -1);
	glVertex3i(-1, 1, -1); glVertex3i(1, 1, -1);
	glVertex3i(1, -1, -1); glVertex3i(-1, -1, -1);
	//face 2
	glColor3ub(this->r, this->g, this->b);
	glNormal3i(-1, -1, -1);
	glVertex3i(-1, -1, -1); glVertex3i(1, -1, -1);
	glVertex3i(1, -1, 1); glVertex3i(-1, -1, 1);
	// face 3
	glColor3ub(this->r, this->g, this->b);
	glNormal3i(1, -1, 1);
	glVertex3i(1, -1, 1); glVertex3i(1, -1, -1);
	glVertex3i(1, 1, -1); glVertex3i(1, 1, 1);
	//face 4
	glColor3ub(this->r, this->g, this->b);
	glNormal3i(1, 1, -1);
	glVertex3i(1, 1, -1); glVertex3i(-1, 1, -1);
	glVertex3i(-1, 1, 1); glVertex3i(1, 1, 1);
	//face 5
	glColor3ub(this->r, this->g, this->b);
	glNormal3i(-1, 1, 1);
	glVertex3i(-1, 1, 1); glVertex3i(-1, 1, -1);
	glVertex3i(-1, -1, -1); glVertex3i(-1, -1, 1);
	//face 6
	glColor3ub(this->r, this->g, this->b);
	glNormal3i(1, -1, 1);
	glVertex3i(1, -1, 1); glVertex3i(1, 1, 1);
	glVertex3i(-1, 1, 1); glVertex3i(-1, -1, 1);
	glEnd();
	glPopMatrix();

	// On remet la couleur � la normale ( �vite de bleuter les textures )
	glColor4f(1, 1, 1, 1);

}