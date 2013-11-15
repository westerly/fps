#include "box.h"


box::box(float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical, btScalar mass, int plargeur, int phauteur)
{
	Objet3DDeformable(positionX, positionY, positionZ, angleHorizontal, angleVertical, mass);
	this->largeur = plargeur;
	this->hauteur = phauteur;

	// On déclare une forme et on l'initialise en tant que boite de la taille largeur,largeur,hauteur (x, y, z)
	shape = new btBoxShape(btVector3(largeur, largeur, hauteur));

	//On initialise notre btTransform et on lui dit une position (la position de la boite)
	transform.setIdentity();
	transform.setOrigin(btVector3(positionX, positionY, positionZ));


	this->localInertia = btVector3(0, 0, 0);

	shape->calculateLocalInertia(mass, this->localInertia);

	// Il est conseillé d'utiliser motionState car il fournit des capacités d'interpolation et synchronise seulement les objets "actifs".
	motionState = new btDefaultMotionState(transform);
	//On regroupe les informations de la boite à partir de la masse, l'inertie et cetera
	btRigidBody::btRigidBodyConstructionInfo myBoxRigidBodyConstructionInfo(mass, motionState, shape, localInertia);
	//On construis le corps de la boite à partir de l'information regroupée
	body = new btRigidBody(myBoxRigidBodyConstructionInfo);
}


box::~box()
{
	// On ne libère pas l'attribut body du tas, c'est le physicEngine qui s'en charge lorsque il libère de la mémoire l'attibut world
	delete motionState;
	delete shape;
}


void box::dessiner()
{
	// On recupère la matrice OpenGL transformée par Bullet qu'on appliquera à notre boite
	motionState->m_graphicsWorldTrans.getOpenGLMatrix(matrix);
	
	// On affiche notre boite avec les transformations appliquées grâce à la matrice
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
	glColor3ub(255, 0, 0); //face rouge
	glNormal3i(-1, 1, -1);
	glVertex3i(-1, 1, -1); glVertex3i(1, 1, -1);
	glVertex3i(1, -1, -1); glVertex3i(-1, -1, -1);
	//face 2
	glColor3ub(0, 255, 0); //face verte
	glNormal3i(-1, -1, -1);
	glVertex3i(-1, -1, -1); glVertex3i(1, -1, -1);
	glVertex3i(1, -1, 1); glVertex3i(-1, -1, 1);
	// face 3
	glColor3ub(0, 0, 255); //face bleue
	glNormal3i(1, -1, 1);
	glVertex3i(1, -1, 1); glVertex3i(1, -1, -1);
	glVertex3i(1, 1, -1); glVertex3i(1, 1, 1);
	//face 4
	glColor3ub(45, 0, 0); //face rouge
	glNormal3i(1, 1, -1);
	glVertex3i(1, 1, -1); glVertex3i(-1, 1, -1);
	glVertex3i(-1, 1, 1); glVertex3i(1, 1, 1);
	//face 5
	glColor3ub(0, 784, 0); //face verte
	glNormal3i(-1, 1, 1);
	glVertex3i(-1, 1, 1); glVertex3i(-1, 1, -1);
	glVertex3i(-1, -1, -1); glVertex3i(-1, -1, 1);
	//face 6
	glColor3ub(0, 0, 255); //face bleue
	glNormal3i(1, -1, 1);
	glVertex3i(1, -1, 1); glVertex3i(1, 1, 1);
	glVertex3i(-1, 1, 1); glVertex3i(-1, -1, 1);
	glEnd();
	glPopMatrix();

}