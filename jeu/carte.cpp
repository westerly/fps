
#include "carte.h"


Carte::Carte(const char* nomFichier)
{
    // Initialisation des attributs
    this->carte = NULL;
    this->largeurCarte = 0u;
    this->hauteurCarte = 0u;
    this->listeAffichage = 0u;

    // Tentative de chargement
    SDL_Surface* imageCarte = SDL_LoadBMP(nomFichier);

    // Si l'image est chargee en 24 bits/px
    if ( (NULL != imageCarte) && (24 == imageCarte->format->BitsPerPixel) )
    {
        // Dimension de la carte
        this->largeurCarte = imageCarte->w;
        this->hauteurCarte = imageCarte->h;

        // Calcule le nombre de cases
        uint32 nbCases = this->largeurCarte * this->hauteurCarte;

        // Instantiation des cases de la carte
        this->carte = new bool8[nbCases];

        // Structuration des pixels BGR pour une lecture simplifiee
        typedef struct {uint8 bleu; uint8 vert; uint8 rouge;} bgr;
        bgr* pixelsCarte = (bgr*)imageCarte->pixels;

        // Parcours de la carte
        for(uint32 i = 0; i < nbCases; i++)
        {
            // Si la moyenne des 3 couleurs est superieur à 50% de la luminosité maximale
            if( (pixelsCarte[i].rouge + pixelsCarte[i].vert + pixelsCarte[i].bleu) > 382 )
            {
                // Pas de mur
                this->carte[i] = 0;
            }
            else
            {
                this->carte[i] = 1;
            }

            /* Ecriture de la carte sur la sortie
            printf("%d", this->carte[i]);
            if ((i+1) % this->largeurCarte == 0) printf("\n");
            //*/
        }

        // Creation de la liste d'affichage de la carte
        this->creerListeAffichage();


        SDL_FreeSurface(imageCarte);
    }

    // Erreur de chargement
    else
    {
        printf("Erreur de chargement de la carte\n");
    }
}

Carte::~Carte()
{
    delete this->carte;
    this->conteneurTextures.supprimer("mur.bmp");
    this->conteneurTextures.supprimer("herbe.bmp");
}

void Carte::dessiner()
{
    glCallList(this->listeAffichage);
}

void Carte::creerListeAffichage()
{
    // Chargement de la texture du mur
    this->conteneurTextures.ajouter("mur.bmp");
    // Chargement de la texture de l'herbe
    this->conteneurTextures.ajouter("herbe.bmp");

    // Creation de la liste d'affichage
    this->listeAffichage = glGenLists(1);
    glNewList(this->listeAffichage, GL_COMPILE);

    // Activation des textures
    glEnable(GL_TEXTURE_2D);

    // Selection de la texture de l'herbe
    glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture("herbe.bmp").texture);

    for(uint32 i = 0u; i < this->hauteurCarte ; i++)
    {
        // Parcours d'un ligne
        for (uint32 j = 0u; j < this->largeurCarte ; j++)
        {
            // Dessin de l'herbe
            glBegin(GL_QUADS);
            glTexCoord2i(0, 0); glVertex3i(i, j, 0);
            glTexCoord2i(0, 1); glVertex3i(i+1, j, 0);
            glTexCoord2i(1, 1); glVertex3i(i+1, j+1, 0);
            glTexCoord2i(1, 0); glVertex3i(i, j+1, 0);
            glEnd();
        }
    }

    // Selection de la texture du mur
    glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture("mur.bmp").texture);

    bool8 caseGauche;
    bool8 caseDroite;

    bool8 caseHaut;
    bool8 caseBas;


    uint32 nbCases;
    nbCases = this->hauteurCarte * this->largeurCarte;

//    for(uint32 i = 0u; i < this->hauteurCarte ; i++)
//    {
//        for(uint32 j = 0u; j < this->largeurCarte ; j++)
//        {
//            printf("%d ",this->carte[i*this->largeurCarte+j]);
//        }
//        printf ("\n");
//    }

    for(uint32 j = 0u; j < this->hauteurCarte ; j++)
    {
        // Parcours d'un ligne
        for (uint32 i = 0u; i < this->largeurCarte - 1; i++)
        {
            uint32 pas = j*this->largeurCarte;
            caseGauche = this->carte[i+ pas];
            caseDroite = this->carte[i+1+pas];

            // Si le mur est visible de l'ouest vers l'est
            if ((0 == caseGauche) && (1 == caseDroite))
            {
                // Dessin du mur
                glBegin(GL_QUADS);
                glTexCoord2i(0, 0); glVertex3i(j, i+1, HAUTEUR_MURS);
                glTexCoord2i(0, 1); glVertex3i(j, i+1, 0);
                glTexCoord2i(1, 1); glVertex3i(j+1, i+1, 0);
                glTexCoord2i(1, 0); glVertex3i(j+1, i+1, HAUTEUR_MURS);
                glEnd();
            }

              // Si le mur est visible de l'est vers l'ouest
            if ((1 == caseGauche) && (0 == caseDroite))
            {
                // Dessin du mur
                glBegin(GL_QUADS);
                glTexCoord2i(1, 0); glVertex3i(j, i+1, HAUTEUR_MURS);
                glTexCoord2i(0, 0); glVertex3i(j+1, i+1, HAUTEUR_MURS);
                glTexCoord2i(0, 1); glVertex3i(j+1, i+1, 0);
                glTexCoord2i(1, 1); glVertex3i(j, i+1, 0);

                glEnd();
            }
        }
    }

    for(uint32 j = 0u; j < this->largeurCarte ; j++)
    {
        // Parcours d'une colonne
        for (uint32 i = 0u; i < this->hauteurCarte - 1; i++)
        {
            uint32 pas = i*this->largeurCarte;
            caseHaut = this->carte[j+pas];
            caseBas = this->carte[j+pas+this->largeurCarte];

            // Si le mur est visible du Nord vers le Sud
            if ((0 == caseHaut) && (1 == caseBas))
            {
                // Dessin du mur
                glBegin(GL_QUADS);

                glTexCoord2i(1, 0); glVertex3i(i+1, j, HAUTEUR_MURS);
                glTexCoord2i(0, 0); glVertex3i(i+1, j+1, HAUTEUR_MURS);
                glTexCoord2i(0, 1); glVertex3i(i+1, j+1, 0);
                glTexCoord2i(1, 1); glVertex3i(i+1, j, 0);

                glEnd();
            }

              // Si le mur est visible du Sud vers le Nord
            if ((0 == caseBas) && (1 == caseHaut))
            {
                // Dessin du mur
                glBegin(GL_QUADS);

                glTexCoord2i(0, 0); glVertex3i(i+1, j, HAUTEUR_MURS);
                glTexCoord2i(0, 1); glVertex3i(i+1, j, 0);
                glTexCoord2i(1, 1); glVertex3i(i+1, j+1, 0);
                glTexCoord2i(1, 0); glVertex3i(i+1, j+1, HAUTEUR_MURS);

                glEnd();
            }
        }
    }

    // Fin de la liste d'affichage
    glEndList();

}

void Carte::entourage(sint32 x, sint32 y, bool8 entourage[8])
{
    sint32 largeurCarte = this->largeurCarte;

    // Par defaut, pas de murs
    entourage[0] = 0;
    entourage[1] = 0;
    entourage[2] = 0;
    entourage[3] = 0;
    entourage[4] = 0;
    entourage[5] = 0;
    entourage[6] = 0;
    entourage[7] = 0;

    // Construction de la vue de l'entourage dans le milieu de la carte
    if (y > 0 && y < (sint32)hauteurCarte - 1 && x > 0 && x < largeurCarte - 1)
    {
        entourage[0] = this->carte[ (x-1) + ((y-1) * largeurCarte) ];
        entourage[1] = this->carte[ (x)   + ((y-1) * largeurCarte) ];
        entourage[2] = this->carte[ (x+1) + ((y-1) * largeurCarte) ];
        entourage[3] = this->carte[ (x-1) + (  (y) * largeurCarte) ];
        entourage[4] = this->carte[ (x+1) + (  (y) * largeurCarte) ];
        entourage[5] = this->carte[ (x-1) + ((y+1) * largeurCarte) ];
        entourage[6] = this->carte[ (x)   + ((y+1) * largeurCarte) ];
        entourage[7] = this->carte[ (x+1) + ((y+1) * largeurCarte) ];
    }

    // ATTENTION : Pour une gestion correcte des collisions,
    // la carte doit etre entouree de murs.
}


// Detecte les murs autour du plan horizontal ( parallèle au plan x,y ) de l'objet en position x,y,z

void Carte::entourage(sint32 x, sint32 y, sint32 z, bool8 entourage[8])
{

	// Par defaut, pas de murs
    entourage[0] = 0;
    entourage[1] = 0;
    entourage[2] = 0;
    entourage[3] = 0;
    entourage[4] = 0;
    entourage[5] = 0;
    entourage[6] = 0;
    entourage[7] = 0;

	if (y > 0 && y < (sint32)hauteurCarte - 1 && x > 0 && x < largeurCarte - 1)
	{
		entourage[0] = this->carte[ (x-1) + ((y-1) * largeurCarte) ];
        entourage[1] = this->carte[ (x)   + ((y-1) * largeurCarte) ];
        entourage[2] = this->carte[ (x+1) + ((y-1) * largeurCarte) ];
        entourage[3] = this->carte[ (x-1) + (  (y) * largeurCarte) ];
        entourage[4] = this->carte[ (x+1) + (  (y) * largeurCarte) ];
        entourage[5] = this->carte[ (x-1) + ((y+1) * largeurCarte) ];
        entourage[6] = this->carte[ (x)   + ((y+1) * largeurCarte) ];
        entourage[7] = this->carte[ (x+1) + ((y+1) * largeurCarte) ];

	}

}