#define _USE_MATH_DEFINES
#include <math.h>

#include "personnage.h"

Personnage::Personnage(float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical) : Objet3DDeformable(positionX,positionY,positionZ,angleHorizontal,angleVertical)
{
    this->rayon = 0.4f;

	std::vector<std::string> v;
	v.push_back("personnage\\gun_shoot1.png");
	v.push_back("personnage\\gun_shoot2.png");
	v.push_back("personnage\\gun_shoot3.png");
	v.push_back("personnage\\gun_shoot4.png");
	v.push_back("personnage\\gun_shoot5.png");
	v.push_back("personnage\\gun_shoot6.png");

	// Création de l'arme à partir du fichier de texture et du vector des fichiers pour son animation
	this->gun = new arme("personnage\\gun.png",v);

}

void Personnage::deplacer(float16 distance, float16 direction, bool8 entourage[8])
{
    direction += this->angleHorizontal;

    // On evite que l'azimut du personnage approche un multiple de 90
    // Cela evite des soucis lors de la gestion des collisions
    const float EPSILON = 0.001f;
    const float MODULO = 90.0f;
    float reste = fmod(direction, MODULO);

    if (this->angleHorizontal >= 0.0f) // Angle positif ou nul
    {
        if (reste < EPSILON) // 0+
        {
            direction += EPSILON;
        }
        else if (reste > MODULO - EPSILON) // 90-
        {
            direction -= EPSILON;
        }
    }
    else // Angle negatif
    {
        if (reste > -EPSILON) // 0-
        {
            direction -= EPSILON;
        }
        else if (reste > -MODULO + EPSILON) // (-90)+
        {
            direction += EPSILON;
        }
    }


    // Calcule de la position cible du personnage
    float16 positionCibleY = this->positionY - distance * sin(direction * M_PI / 180.0);
    float16 positionCibleX = this->positionX - distance * cos(direction * M_PI / 180.0);

    // GESTION DES COLLISIONS AVEC MUR EST

    // S'il y a un mur à l'Est
    // et que la droite du personnage arrive dans le mur
    if(  (1 == entourage[4])
         && ((sint32)(positionCibleY + this->rayon) != (sint32)this->positionY)  )
    {
        // On rectifie la translation en Y (3D)
        positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
    }

    // GESTION DES COLLISIONS AVEC MUR SUD

    // S'il y a un mur au Sud et que notre Sud arrive dans le mur
    if (1 == entourage[6]
        && (sint32)(positionCibleX + this->rayon) != (sint32)this->positionX
        )
    {
        // On rectifie la translation en X (3D)
        positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
    }

    // GESTION DES COLLISIONS AVEC MUR OUEST

    // S'il y a un mur a l'Ouest et que notre Ouest arrive dans le mur
    if (1 == entourage[3]
        && (sint32)(positionCibleY - this->rayon) != (sint32)this->positionY
        )
    {
        // On rectifie la translation en Y (3D)
        positionCibleY -= (positionCibleY - this->rayon) - (sint32)(positionCibleY + this->rayon);
    }

    // GESTION DES COLLISIONS AVEC MUR AU NORD

    // S'il y a un mur au Nord et que notre Nord arrive dans le mur
    if (1 == entourage[1]
        && (sint32)(positionCibleX - this->rayon) != (sint32)this->positionX
        )
    {
        // On rectifie la translation en X (3D)
        positionCibleX -= (positionCibleX - this->rayon) - (sint32)(positionCibleX + this->rayon);
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
                positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
            }
            else
            {
                // Si ce sont les bord en Y qui ont été en contact lors de la collision
                if ( ((positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon)) / ((positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon)) > (positionCibleX - this->positionX) / (positionCibleY - this->positionY))
                {
                    positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
                }
                else
                {
                    positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
                }
            }
        }
        else if (positionCibleX < this->positionX) // Eloigne en X
        {
            positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
        }
        else if (positionCibleY < this->positionY) // Eloigne en Y
        {
            positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
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
            positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
        }
        else if (positionCibleY > this->positionY) // Eloigne en Y
        {
            positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
        }
        else if (positionCibleX > this->positionX && positionCibleY < this->positionY) // Approche
        {
            if(((sint32)(positionCibleY - this->rayon + 1) - (positionCibleY - this->rayon)) == 0.0 || (this->positionY - positionCibleY) == 0.0)
            {
                positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
            }
            else
            {
                // Si ce sont les bords en Y qui ont été en contact lors de la collision
                if ( ((positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon)) / ((sint32)(positionCibleY - this->rayon + 1) - (positionCibleY - this->rayon)) > (positionCibleX - this->positionX) / (this->positionY - positionCibleY))
                {
                    positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
                }
                else
                {
                    positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
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
            positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
        }
        else if (positionCibleY > this->positionY) // Eloigne en Y
        {
            positionCibleX += (sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon);
        }
        else if (positionCibleX < this->positionX && positionCibleY < this->positionY) // Approche
        {
            // Evite la division par zero
            if (((sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon)) == 0.0 || (this->positionY - positionCibleY) == 0.0)
            {
                positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
            }
            else
            {
                // Si ce sont les bords en Y qui ont été en contact lors de la collision
                if ( ((sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon)) / ((sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon)) > (this->positionX - positionCibleX) / (this->positionY - positionCibleY))
                {
                    positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
                }
                else
                {
                    positionCibleX += (sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon);
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
            positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
        }
        else if (positionCibleY < this->positionY) // Eloigne en Y
        {
            positionCibleX += (sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon);
        }
        else if (positionCibleX < this->positionX && positionCibleY > this->positionY) // Approche
        {
            if ((((positionCibleY + this->rayon)) - (sint32)(positionCibleY + this->rayon)) == 0.0 || (this->positionY - positionCibleY) == 0.0)
            {
                positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
            }
            else
            {
                // Si ce sont les bords en Y qui ont été en contact lors de la collision
                if ( (((positionCibleX - this->rayon)) - (sint32)((positionCibleX - this->rayon) + 1)) / (((positionCibleY + this->rayon)) - (sint32)(positionCibleY + this->rayon)) < (this->positionX - positionCibleX) / (this->positionY - positionCibleY))
                {
                    positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
                }
                else
                {
                    positionCibleX += (sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon);
                }
            }
        }
    }

    this->positionY = positionCibleY;
    this->positionX = positionCibleX;
}


void Personnage::tournerHorizontalement(float16 angle)
{
    this->angleHorizontal += angle;

    while (this->angleHorizontal >= 180.0f) // Lorsqu'on dépasse la limite (1/2 tour)
    {
        this->angleHorizontal -= 360.0f;
    }
    while (this->angleHorizontal < -180.0f) // Idem après 1/2 tours vers la droite
    {
        this->angleHorizontal += 360.0f;
    }
}

void Personnage::tournerVerticalement(float16 angle)
{
    this->angleVertical += angle;

    if (45.0f < this->angleVertical)
    {
        this->angleVertical = 45.0f;
    }
    else if (-45.0f > this->angleVertical)
    {
        this->angleVertical = -45.0f;
    }
}

void Personnage::positionSurLaCarte(sint32* x, sint32* y)
{
    // Recupere la position du personnage sur la carte
    *x = (sint32)this->positionX;
    *y = (sint32)this->positionY;
}

void Personnage::regarder(void)
{

    gluLookAt(
        // Position de l'oeil
        this->positionX, this->positionY, HAUTEUR_OEIL_PERSONNAGE,

        // Point vise
        this->positionX - cos(-angleHorizontal * RADIANS_PAR_DEGRES),
        this->positionY + sin(-this->angleHorizontal * RADIANS_PAR_DEGRES),
        HAUTEUR_OEIL_PERSONNAGE + tan(this->angleVertical * RADIANS_PAR_DEGRES),

        // La verticale est en Z
        0,0,1);
}



void Personnage::dessiner()
{
	// On mémorise le repère courant avant d'effectuer la RST
    glPushMatrix();
		
        // Positionne l'objet en lieu de dessin
		//glTranslatef(this->positionX  - this->rayon * cos(-this->angleHorizontal * RADIANS_PAR_DEGRES),
		//this->positionY + this->rayon * sin(-this->angleHorizontal * RADIANS_PAR_DEGRES),
		//HAUTEUR_OEIL_PERSONNAGE + this->rayon * tan(this->angleVertical * RADIANS_PAR_DEGRES));

		this->gun->dessiner(this->positionX,this->positionY,HAUTEUR_OEIL_PERSONNAGE,this->angleHorizontal,this->angleVertical);
		
		
		glTranslatef(this->positionX,
        this->positionY,
        HAUTEUR_OEIL_PERSONNAGE);


		glRotatef(this->angleHorizontal, 0.0, 0.0, 1.0);
		glRotatef(this->angleVertical, 0.0, 1.0, 0.0);

		sint32 hauteurTextureCourante;
		sint32 largeurTextureCourante;

		glEnable(GL_TEXTURE_2D);
		// Permet d'afficher les textures transparentes
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
        hauteurTextureCourante = this->conteneurTextures.texture("personnage\\gun.png").hauteur;
		largeurTextureCourante = this->conteneurTextures.texture("personnage\\gun.png").largeur;

		glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture("personnage\\gun2.png").texture);
		glBegin(GL_POLYGON);

		glTexCoord2f((float16)largeurTextureCourante / this->conteneurTextures.texture("personnage\\gun.png").largeur, (float16)0.0 / this->conteneurTextures.texture("personnage\\gun.png").hauteur); 
		glVertex3d(-(this->rayon-0.1), 0.25, 0.21);

		glTexCoord2f((float16)largeurTextureCourante / this->conteneurTextures.texture("personnage\\gun.png").largeur, (float16)hauteurTextureCourante / this->conteneurTextures.texture("personnage\\gun.png").hauteur); 
		glVertex3d(-(this->rayon-0.1), 0.25, -0.21);

		glTexCoord2f((float16)0.0 / this->conteneurTextures.texture("personnage\\gun.png").largeur, (float16)hauteurTextureCourante / this->conteneurTextures.texture("personnage\\gun.png").hauteur); 
		glVertex3d(-(this->rayon-0.1), -0.25, -0.21);

		glTexCoord2f((float16)0.0 / this->conteneurTextures.texture("personnage\\gun.png").largeur, (float16)0.0 / this->conteneurTextures.texture("personnage\\gun.png").hauteur); 
		glVertex3d(-(this->rayon-0.1), -0.25, 0.21);

		glEnd();


    // Restoration du repère d'origine
    glPopMatrix();
}