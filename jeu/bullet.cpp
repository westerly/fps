#define _USE_MATH_DEFINES
#include <math.h>

#include "bullet.h"

#include "configuration.h"

Bullet::Bullet(float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical, btScalar mass, float16 rayon):Objet3DDeformable(positionX, positionY, positionZ, angleHorizontal, angleVertical, mass)
{
	this->rayon = rayon;
	this->inColision = false;

	// On déclare une forme et on l'initialise en tant que sphere
	shape = new btSphereShape(this->rayon);

	//On initialise notre btTransform et on lui dit une position (la position de la sphere)
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

	// Permet de pouvoir travailler sur l'objet bullet lors de la détection des collisions
	body->setUserPointer(this);

	this->body->setCollisionFlags(this->body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}


Bullet::~Bullet(void)
{
	// Libération des textures
    for (Textures::iterator element = this->textures.begin(); element != textures.end(); element++)
    {
        this->conteneurTextures.supprimer(*element);
    }

	// On ne libère pas l'attribut body du tas, c'est le physicEngine qui s'en charge lorsque il libère de la mémoire l'attibut world
	delete motionState;
	delete shape;

}


// Ajoute et charge une texture à partir d'un fichier
// La texture est aussi ajouté aux textures de l'objet 
void Bullet::ajouterTexture(std::string nomFichierTexture)
{
	// Si la texture n'est pas deja creee
    if(this->textures.find(nomFichierTexture) == this->textures.end())
    {
        this->conteneurTextures.ajouter(nomFichierTexture);
        this->textures.insert(nomFichierTexture);
    }
}


void Bullet::positionSurLaCarte(sint32* x, sint32* y, sint32* z)
{
	// Recupere la position de la balle sur la carte
    *x = (sint32)this->positionX;
    *y = (sint32)this->positionY;
	*z = (sint32)this->positionZ;
}


void Bullet::deplacer(const btVector3 &force){
	this->body->applyCentralForce(force);
}

void Bullet::deplacer(float16 & distance,bool8 entourage[8])
{

    // Calcule de la position cible de la balle
	//float16 positionCibleY = this->positionY - distance * sin(this->angleHorizontal * M_PI / 180.0);
	//float16 positionCibleX = this->positionX - distance * cos(this->angleHorizontal * M_PI / 180.0);
	//float16 positionCibleZ = this->positionZ + distance * sin(this->angleVertical * M_PI / 180.0);


	//// Colision avec le sol
	//if((positionCibleZ - this->rayon) <=0)
	//{
	//	this->inColision = true;
	//}


	//// Si la balle n'est pas au dessus des murs
	//if(((positionCibleZ - this->rayon) < HAUTEUR_MURS))
	//{
	//	// Gestion des colisions avec les murs
	//	this->handleColisionWithWall(positionCibleX,positionCibleY, entourage);
	//}

	//if(!this->inColision)
	//{
	//	this->positionY = positionCibleY;
	//	this->positionX = positionCibleX;
	//	this->positionZ = positionCibleZ;
	//}

}

void Bullet::handleColisionWithWall(float16 & positionCibleX, float16 & positionCibleY, bool8 * entourage)
{
	// GESTION DES COLLISIONS AVEC MUR EST



    // S'il y a un mur à l'Est
    if(  (1 == entourage[4])
         && ((sint32)(positionCibleY + this->rayon) != (sint32)this->positionY)  )
    {
        this->inColision = true;
    }

    // GESTION DES COLLISIONS AVEC MUR SUD

    // S'il y a un mur au Sud et que notre Sud arrive dans le mur
    if (1 == entourage[6]
        && (sint32)(positionCibleX + this->rayon) != (sint32)this->positionX
        )
    {
        this->inColision = true;
    }

    // GESTION DES COLLISIONS AVEC MUR OUEST

    // S'il y a un mur a l'Ouest et que notre Ouest arrive dans le mur
    if (1 == entourage[3]
        && (sint32)(positionCibleY - this->rayon) != (sint32)this->positionY
        )
    {
        this->inColision = true;
    }

    // GESTION DES COLLISIONS AVEC MUR AU NORD

    // S'il y a un mur au Nord et que notre Nord arrive dans le mur
    if (1 == entourage[1]
        && (sint32)(positionCibleX - this->rayon) != (sint32)this->positionX
        )
    {
        this->inColision = true;
    }


    // GESTION DES COLLISIONS AVEC MUR AU SUD-EST
    if (1 == entourage[7]
        && (sint32)(positionCibleX + this->rayon) != (sint32)this->positionX
        && (sint32)(positionCibleY + this->rayon) != (sint32)this->positionY
        )
    {
        if (positionCibleX > this->positionX && positionCibleY > this->positionY) // Approche
        {
            // Evite la division par zero
            if (((positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon)) == 0.0 || (positionCibleY - this->positionY) == 0.0)
            {
                this->inColision = true;
            }
            else
            {
                // Si ce sont les bord en Y qui ont été en contact lors de la collision
                if ( ((positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon)) / ((positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon)) > (positionCibleX - this->positionX) / (positionCibleY - this->positionY))
                {
                    this->inColision = true;
                }
                else
                {
                    this->inColision = true;
                }
            }
        }
        else if (positionCibleX < this->positionX) // Eloigne en X
        {
            this->inColision = true;
        }
        else if (positionCibleY < this->positionY) // Eloigne en Y
        {
            this->inColision = true;
        }
    }

    // GESTION DES COLLISIONS AVEC MUR AU SUD-OUEST
    if (1 == entourage[5]
        && (sint32)(positionCibleX + this->rayon) != (sint32)this->positionX
        && (sint32)(positionCibleY - this->rayon) != (sint32)this->positionY
        )
    {
        if (positionCibleX < this->positionX) // Eloigne en X
        {
            this->inColision = true;
        }
        else if (positionCibleY > this->positionY) // Eloigne en Y
        {
            this->inColision = true;
        }
        else if (positionCibleX > this->positionX && positionCibleY < this->positionY) // Approche
        {
            if(((sint32)(positionCibleY - this->rayon + 1) - (positionCibleY - this->rayon)) == 0.0 || (this->positionY - positionCibleY) == 0.0)
            {
                this->inColision = true;
            }
            else
            {
                // Si ce sont les bords en Y qui ont été en contact lors de la collision
                if ( ((positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon)) / ((sint32)(positionCibleY - this->rayon + 1) - (positionCibleY - this->rayon)) > (positionCibleX - this->positionX) / (this->positionY - positionCibleY))
                {
                    this->inColision = true;
                }
                else
                {
                   this->inColision = true;
                }
            }
        }
    }

    // GESTION DES COLLISIONS AVEC MUR AU NORD-OUEST
    if (1 == entourage[0]
        && (sint32)(positionCibleX - this->rayon) != (sint32)this->positionX
        && (sint32)(positionCibleY - this->rayon) != (sint32)this->positionY
        )
    {
        if (positionCibleX > this->positionX) // Eloigne en X
        {
           this->inColision = true;
        }
        else if (positionCibleY > this->positionY) // Eloigne en Y
        {
           this->inColision = true;
        }
        else if (positionCibleX < this->positionX && positionCibleY < this->positionY) // Approche
        {
            // Evite la division par zero
            if (((sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon)) == 0.0 || (this->positionY - positionCibleY) == 0.0)
            {
                this->inColision = true;
            }
            else
            {
                // Si ce sont les bords en Y qui ont été en contact lors de la collision
                if ( ((sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon)) / ((sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon)) > (this->positionX - positionCibleX) / (this->positionY - positionCibleY))
                {
                    this->inColision = true;
                }
                else
                {
                    this->inColision = true;
                }
            }
        }
    }

    // GESTION DES COLLISIONS AVEC MUR AU NORD-EST
    if (1 == entourage[2]
        && (sint32)(positionCibleX - this->rayon) != (sint32)this->positionX
        && (sint32)(positionCibleY + this->rayon) != (sint32)this->positionY
        )
    {
        if (positionCibleX > this->positionX) // Eloigne en X
        {
            this->inColision = true;
        }
        else if (positionCibleY < this->positionY) // Eloigne en Y
        {
            this->inColision = true;
        }
        else if (positionCibleX < this->positionX && positionCibleY > this->positionY) // Approche
        {
            if ((((positionCibleY + this->rayon)) - (sint32)(positionCibleY + this->rayon)) == 0.0 || (this->positionY - positionCibleY) == 0.0)
            {
                this->inColision = true;
            }
            else
            {
                // Si ce sont les bords en Y qui ont été en contact lors de la collision
                if ( (((positionCibleX - this->rayon)) - (sint32)((positionCibleX - this->rayon) + 1)) / (((positionCibleY + this->rayon)) - (sint32)(positionCibleY + this->rayon)) < (this->positionX - positionCibleX) / (this->positionY - positionCibleY))
                {
                    this->inColision = true;
                }
                else
                {
                    this->inColision = true;
                }
            }
        }
    }
}


void Bullet::dessiner()
{
	glDisable(GL_TEXTURE_2D);
	//New quadric object
	GLUquadric * quadric = gluNewQuadric();

	// On recupère la matrice OpenGL transformée par Bullet qu'on appliquera à notre boite
	motionState->m_graphicsWorldTrans.getOpenGLMatrix(matrix);

	// On affiche notre boite avec les transformations appliquées grâce à la matrice
	glPushMatrix();
		glMultMatrixf(matrix);
		glColor3f(0.0, 1.0, 1.0);
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluSphere(quadric, this->rayon, 20, 20);
	glPopMatrix();
	// On remet la couleur standard
	glColor3f(255, 255, 255);
}