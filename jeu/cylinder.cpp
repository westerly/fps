#include "cylinder.h"

cylinder::cylinder(float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical, btScalar mass, float plargeur, float phauteur, float r, float g, float b, float a)
{
	Objet3DDeformable(positionX, positionY, positionZ, angleHorizontal, angleVertical, mass);
	this->largeur = plargeur;
	this->hauteur = phauteur;
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	float radius = largeur / 2;

	// Chargement de la texture du mur
	this->conteneurTextures.ajouter("cylinder.bmp");

	// On déclare une forme et on l'initialise en tant que cylindre
	shape = new btCylinderShapeZ(btVector3(radius, this->hauteur, radius));

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

	this->body->setUserPointer(this);
}


cylinder::~cylinder()
{
	// On ne libère pas l'attribut body du tas, c'est le physicEngine qui s'en charge lorsque il libère de la mémoire l'attibut world
	delete motionState;
	delete shape;
}


void cylinder::dessiner()
{
	// On recupère la matrice OpenGL transformée par Bullet qu'on appliquera à notre boite
	motionState->m_graphicsWorldTrans.getOpenGLMatrix(matrix);

	// On affiche notre boite avec les transformations appliquées grâce à la matrice
	glPushMatrix();
		glMultMatrixf(matrix);
		dessinerCylinder(this->largeur / 2, this->largeur / 2, this->hauteur);
	glPopMatrix();
}


void cylinder::dessinerCylinder(float x, float y, float z)
{
	float radius = x;

	// Activation des textures
	//glEnable(GL_TEXTURE_2D);

	// Selection de la texture du mur
	//glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture("mur.bmp").texture);
		
		glTranslatef(this->positionX,
		this->positionY,
		// Fix pour décalage entre physique et rendu...
		this->positionZ - this->hauteur/2);
		glScalef(x, y, z);
		

		glColor4f(this->r, this->g, this->b, this->a);

		/*GLUquadricObj *quadratic;
		quadratic = gluNewQuadric();
		gluCylinder(quadratic, 1, 1, 1, 32, 32);*/

		
		/* number of side faces */
		int faces = 360;
		/* Choose neutral color (white)*/
		glColor3d(1, 1, 1);
		/* Enable 2D Texture*/
		glEnable(GL_TEXTURE_2D);
		/* set current working texture */
		glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture("cylinder.bmp").texture);

		/* Disabling these is not necessary in this example,
		* BUT if you have previously enabled GL_TEXTURE_GEN_
		* for other textures,then you need these lines
		*/
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);

		glBegin(GL_QUAD_STRIP);
		double x2, y2, z2;
		z = this->hauteur;
		for (int i = 0; i <= faces; i++) {
			double u = i / (double)faces;
			x2 = 1*cos(2 * M_PI*u);
			y2 = 1*sin(2 * M_PI*u);
			/* Bottom vertex*/
			glTexCoord2d(u, 1.0); glVertex3d(x2, y2, 0);
			/* Top vertex*/
			glTexCoord2d(u, 0.0); glVertex3d(x2, y2, 1);
		}
		glEnd();



		float angle, radian, xc, yc, xcos, ysin, tx, ty;       // values needed by drawCircleOutline

		// draw first circle
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture("cylinder.bmp").texture);

		glBegin(GL_POLYGON);

		for (angle = 0.0; angle<360.0; angle += 2.0)
		{
			radian = angle * (M_PI / 180.0f);

			xcos = (float)cos(radian);
			ysin = (float)sin(radian);
			xc = xcos * this->largeur * (1 / this->largeur);
			yc = ysin * this->largeur * (1 / this->largeur);
			tx = xcos * 0.5 + 0.5;
			ty = ysin * 0.5 + 0.5;

			glTexCoord2f(tx, ty);
			glVertex3f(xc, yc, 0);

		}

		glEnd();

		// Draw second circle
		glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture("cylinder.bmp").texture);
		glBegin(GL_POLYGON);

		for (angle = 0.0; angle<360.0; angle += 2.0)
		{
			radian = angle * (M_PI / 180.0f);

			xcos = (float)cos(radian);
			ysin = (float)sin(radian);
			xc = xcos * this->largeur * (1/this->largeur);
			yc = ysin * this->largeur * (1/this->largeur);
			tx = xcos * 0.5 + 0.5;
			ty = ysin * 0.5 + 0.5;

			glTexCoord2f(tx, ty);
			glVertex3f(xc, yc, 1.0);

		}

		glEnd();


		glDisable(GL_TEXTURE_2D);


	// On remet la couleur à la normale ( évite de bleuter les textures )
	glColor4f(1, 1, 1, 1);

}
