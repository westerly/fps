#define _USE_MATH_DEFINES
#include <math.h>

#include "personnage.h"

Personnage::Personnage(float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical, btScalar mass) : Objet3DDeformable(positionX, positionY, positionZ, angleHorizontal, angleVertical, mass)
{
    this->rayon = 0.4f;
	this->vitesse = VITESSE_DEPLACEMENT_PERSONNAGE;

	std::vector<std::string> v;
	v.push_back("personnage\\gun_shoot1.png");
	v.push_back("personnage\\gun_shoot2.png");
	v.push_back("personnage\\gun_shoot3.png");
	v.push_back("personnage\\gun_shoot4.png");
	v.push_back("personnage\\gun_shoot5.png");
	v.push_back("personnage\\gun_shoot6.png");
	v.push_back("personnage\\viseur.png");

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
    //if(  (1 == entourage[4])
    //     && ((sint32)(positionCibleY + this->rayon) != (sint32)this->positionY)  )
    //{
    //    // On rectifie la translation en Y (3D)
    //    positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
    //}

    //// GESTION DES COLLISIONS AVEC MUR SUD

    //// S'il y a un mur au Sud et que notre Sud arrive dans le mur
    //if (1 == entourage[6]
    //    && (sint32)(positionCibleX + this->rayon) != (sint32)this->positionX
    //    )
    //{
    //    // On rectifie la translation en X (3D)
    //    positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
    //}

    //// GESTION DES COLLISIONS AVEC MUR OUEST

    //// S'il y a un mur a l'Ouest et que notre Ouest arrive dans le mur
    //if (1 == entourage[3]
    //    && (sint32)(positionCibleY - this->rayon) != (sint32)this->positionY
    //    )
    //{
    //    // On rectifie la translation en Y (3D)
    //    positionCibleY -= (positionCibleY - this->rayon) - (sint32)(positionCibleY + this->rayon);
    //}

    //// GESTION DES COLLISIONS AVEC MUR AU NORD

    //// S'il y a un mur au Nord et que notre Nord arrive dans le mur
    //if (1 == entourage[1]
    //    && (sint32)(positionCibleX - this->rayon) != (sint32)this->positionX
    //    )
    //{
    //    // On rectifie la translation en X (3D)
    //    positionCibleX -= (positionCibleX - this->rayon) - (sint32)(positionCibleX + this->rayon);
    //}


    //// GESTION DES COLLISIONS AVEC MUR AU SUD-EST
    //if (1 == entourage[7]
    //    && (sint32)(positionCibleX + this->rayon) != (sint32)this->positionX
    //    && (sint32)(positionCibleY + this->rayon) != (sint32)this->positionY
    //    )
    //{
    //    if (positionCibleX > this->positionX && positionCibleY > this->positionY) // Approche
    //    {
    //        // Evite la division par zero
    //        if (((positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon)) == 0.0 || (positionCibleY - this->positionY) == 0.0)
    //        {
    //            positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
    //        }
    //        else
    //        {
    //            // Si ce sont les bord en Y qui ont été en contact lors de la collision
    //            if ( ((positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon)) / ((positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon)) > (positionCibleX - this->positionX) / (positionCibleY - this->positionY))
    //            {
    //                positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
    //            }
    //            else
    //            {
    //                positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
    //            }
    //        }
    //    }
    //    else if (positionCibleX < this->positionX) // Eloigne en X
    //    {
    //        positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
    //    }
    //    else if (positionCibleY < this->positionY) // Eloigne en Y
    //    {
    //        positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
    //    }
    //}

    //// GESTION DES COLLISIONS AVEC MUR AU SUD-OUEST
    //if (1 == entourage[5]
    //    && (sint32)(positionCibleX + this->rayon) != (sint32)this->positionX
    //    && (sint32)(positionCibleY - this->rayon) != (sint32)this->positionY
    //    )
    //{
    //    if (positionCibleX < this->positionX) // Eloigne en X
    //    {
    //        positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
    //    }
    //    else if (positionCibleY > this->positionY) // Eloigne en Y
    //    {
    //        positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
    //    }
    //    else if (positionCibleX > this->positionX && positionCibleY < this->positionY) // Approche
    //    {
    //        if(((sint32)(positionCibleY - this->rayon + 1) - (positionCibleY - this->rayon)) == 0.0 || (this->positionY - positionCibleY) == 0.0)
    //        {
    //            positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
    //        }
    //        else
    //        {
    //            // Si ce sont les bords en Y qui ont été en contact lors de la collision
    //            if ( ((positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon)) / ((sint32)(positionCibleY - this->rayon + 1) - (positionCibleY - this->rayon)) > (positionCibleX - this->positionX) / (this->positionY - positionCibleY))
    //            {
    //                positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
    //            }
    //            else
    //            {
    //                positionCibleX -= (positionCibleX + this->rayon) - (sint32)(positionCibleX + this->rayon);
    //            }
    //        }
    //    }
    //}

    //// GESTION DES COLLISIONS AVEC MUR AU NORD-OUEST
    //if (1 == entourage[0]
    //    && (sint32)(positionCibleX - this->rayon) != (sint32)this->positionX
    //    && (sint32)(positionCibleY - this->rayon) != (sint32)this->positionY
    //    )
    //{
    //    if (positionCibleX > this->positionX) // Eloigne en X
    //    {
    //        positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
    //    }
    //    else if (positionCibleY > this->positionY) // Eloigne en Y
    //    {
    //        positionCibleX += (sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon);
    //    }
    //    else if (positionCibleX < this->positionX && positionCibleY < this->positionY) // Approche
    //    {
    //        // Evite la division par zero
    //        if (((sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon)) == 0.0 || (this->positionY - positionCibleY) == 0.0)
    //        {
    //            positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
    //        }
    //        else
    //        {
    //            // Si ce sont les bords en Y qui ont été en contact lors de la collision
    //            if ( ((sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon)) / ((sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon)) > (this->positionX - positionCibleX) / (this->positionY - positionCibleY))
    //            {
    //                positionCibleY += (sint32)((positionCibleY - this->rayon) + 1) - (positionCibleY - this->rayon);
    //            }
    //            else
    //            {
    //                positionCibleX += (sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon);
    //            }
    //        }
    //    }
    //}

    //// GESTION DES COLLISIONS AVEC MUR AU NORD-EST
    //if (1 == entourage[2]
    //    && (sint32)(positionCibleX - this->rayon) != (sint32)this->positionX
    //    && (sint32)(positionCibleY + this->rayon) != (sint32)this->positionY
    //    )
    //{
    //    if (positionCibleX > this->positionX) // Eloigne en X
    //    {
    //        positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
    //    }
    //    else if (positionCibleY < this->positionY) // Eloigne en Y
    //    {
    //        positionCibleX += (sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon);
    //    }
    //    else if (positionCibleX < this->positionX && positionCibleY > this->positionY) // Approche
    //    {
    //        if ((((positionCibleY + this->rayon)) - (sint32)(positionCibleY + this->rayon)) == 0.0 || (this->positionY - positionCibleY) == 0.0)
    //        {
    //            positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
    //        }
    //        else
    //        {
    //            // Si ce sont les bords en Y qui ont été en contact lors de la collision
    //            if ( (((positionCibleX - this->rayon)) - (sint32)((positionCibleX - this->rayon) + 1)) / (((positionCibleY + this->rayon)) - (sint32)(positionCibleY + this->rayon)) < (this->positionX - positionCibleX) / (this->positionY - positionCibleY))
    //            {
    //                positionCibleY -= (positionCibleY + this->rayon) - (sint32)(positionCibleY + this->rayon);
    //            }
    //            else
    //            {
    //                positionCibleX += (sint32)((positionCibleX - this->rayon) + 1) - (positionCibleX - this->rayon);
    //            }
    //        }
    //    }
    //}

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
	this->gun->dessiner(this->positionX,this->positionY,HAUTEUR_OEIL_PERSONNAGE,this->angleHorizontal,this->angleVertical);
}


void Personnage::drawTextInFrontOfCharacter(const char *text, int length, int x, int y){

	glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
	double matrix[16]; // 16 doubles in stack memory
	glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
	glLoadIdentity(); // reset PROJECTION matrix to identity matrix
	glOrtho(0, LARGEUR_FENETRE, 0, HAUTEUR_FENETRE, -5, 5); // orthographic perspective
	glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIEW matrix again
	glLoadIdentity(); // reset it to identity matrix
	glPushMatrix(); // push current state of MODELVIEW matrix to stack
	glTranslatef(this->positionX,
		this->positionY,
		this->positionZ);
	glRotatef(this->angleHorizontal, 0.0, 0.0, 1.0);
	glRotatef(this->angleVertical, 0.0, 1.0, 0.0);
	glLoadIdentity(); // reset it again. (may not be required, but it my convention)
	glRasterPos2i(x, y); // raster position in 2D
	for (int i = 0; i<length; i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]); // generation of characters in our text with 9 by 15 GLU font
	}
	glPopMatrix(); // get MODELVIEW matrix value from stack
	glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
	glLoadMatrixd(matrix); // reset
	glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW

	// On remet la couleur à la normal
	glColor3f(255, 255, 255);
}