#define _USE_MATH_DEFINES
#include <math.h>

#include "bullet.h"

#include "configuration.h"

Bullet::Bullet(std::vector<std::string> & fichiersTextureAnimationDestructionBullet,float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->positionZ = positionZ;
	this->rayon = 0.1f;
	this->angleHorizontal = angleHorizontal;
	this->angleVertical = angleVertical;

	this->inColision = false;

	for(std::vector<std::string>::iterator texture = fichiersTextureAnimationDestructionBullet.begin(); texture != fichiersTextureAnimationDestructionBullet.end(); texture++)
	{
		this->ajouterTexture(*texture);
	}
}


Bullet::~Bullet(void)
{
	// Libération des textures
    for (Textures::iterator element = this->textures.begin(); element != textures.end(); element++)
    {
        this->conteneurTextures.supprimer(*element);
    }

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


void Bullet::deplacer(float16 & distance,bool8 entourage[8])
{

    // Calcule de la position cible de la balle
	float16 positionCibleY = this->positionY - distance * sin(this->angleHorizontal * M_PI / 180.0);
	float16 positionCibleX = this->positionX - distance * cos(this->angleHorizontal * M_PI / 180.0);
	float16 positionCibleZ = this->positionZ + distance * sin(this->angleVertical * M_PI / 180.0);


	// Colision avec le sol
	if((positionCibleZ - this->rayon) <=0)
	{
		this->inColision = true;
	}


	// Si la balle n'est pas au dessus des murs
	if(((positionCibleZ - this->rayon) < HAUTEUR_MURS))
	{
		// Gestion des colisions avec les murs
		this->handleColisionWithWall(positionCibleX,positionCibleY, entourage);
	}

	if(!this->inColision)
	{
		this->positionY = positionCibleY;
		this->positionX = positionCibleX;
		this->positionZ = positionCibleZ;
	}

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
	//glDisable (GL_LIGHTING);
	GLUquadric * quadric = gluNewQuadric();
	//Turn on the texture mode for quadrics
	//gluQuadricTexture(quadric, false);

	// On mémorise le repère courant avant d'effectuer la RST
    glPushMatrix();
		
		glTranslatef(this->positionX,
        this->positionY,
        this->positionZ);

		glRotatef(this->angleHorizontal, 0.0, 0.0, 1.0);
		glRotatef(this->angleVertical, 0.0, 1.0, 0.0);
		//glColor3f(0.0, 1.0, 1.0);
		gluQuadricDrawStyle(quadric, GLU_FILL );

		gluSphere(quadric, this->rayon, 20, 20);

    // Restoration du repère d'origine
    glPopMatrix();
	
}