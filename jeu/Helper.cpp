#include "Helper.h"


void Helper::creerTextureText(std::string texte, int size, SDL_Color couleur, Texture &texture){

	// Chargement de la police du bouton
	TTF_Font* police = TTF_OpenFont("polices\\Cartonsix NC.ttf", size);
	if (NULL == police)
	{
		printf("Impossible de charger le fichier Cartonsix NC.ttf");
	}

	// Creation de l'image du texte avec la police associee
	SDL_Surface* texteSurface = TTF_RenderText_Blended(police, texte.c_str(), couleur);
	if (NULL == texteSurface)
	{
		printf("Impossible de créer le texte");
	}

	// Liberation de la police
	TTF_CloseFont(police);

	// Activation de la transparence
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generation de la texture pour le texte
	glGenTextures(1, &texture.texture);

	// Selection de la texture generee
	glBindTexture(GL_TEXTURE_2D, texture.texture);

	// Parametrage
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Detection du codage des pixels
	GLenum codagePixel;
	if (texteSurface->format->Rmask == 0x000000ff)
	{
		codagePixel = GL_RGBA;
	}
	else
	{
		#ifndef GL_BGRA
		#define GL_BGRA 0x80E1
		#endif
		codagePixel = GL_BGRA;
	}

	// Chargement de la texture du texte precedament creee
	glTexImage2D(GL_TEXTURE_2D, 0, 4, texteSurface->w, texteSurface->h, 0,
		codagePixel, GL_UNSIGNED_BYTE, texteSurface->pixels);

	// Recuperation des dimensions du texte
	texture.largeur = texteSurface->w;
	texture.hauteur = texteSurface->h;

	// Liberation de l'image du texte du bouton
	SDL_FreeSurface(texteSurface);

}


void  Helper::dessinerTexte(Texture &texture, sint32 pX, sint32 pY)
{
	// Activation de la transparence
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
	double matrix[16]; // 16 doubles in stack memory
	glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
	glLoadIdentity(); // reset PROJECTION matrix to identity matrix

	// Definition de la fenetre
	gluOrtho2D(0.0, (GLdouble)LARGEUR_FENETRE, 0.0, (GLdouble)HAUTEUR_FENETRE);

	glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIEW matrix again
	glLoadIdentity(); // reset it to identity matrix

	glPushMatrix();


	// Desactivation du test de prophondeur
	glDisable(GL_DEPTH_TEST);

	// Activation des textures
	glEnable(GL_TEXTURE_2D);

	// Selection de la texture du texte
	glBindTexture(GL_TEXTURE_2D, texture.texture);

	// Application du texte
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(pX - (texture.largeur / 2),
		HAUTEUR_FENETRE - pY + (texture.hauteur / 2));
	glTexCoord2i(0, 1); glVertex2i(pX - (texture.largeur / 2),
		HAUTEUR_FENETRE - pY - (texture.hauteur / 2));
	glTexCoord2i(1, 1); glVertex2i(pX + (texture.largeur / 2),
		HAUTEUR_FENETRE - pY - (texture.hauteur / 2));
	glTexCoord2i(1, 0); glVertex2i(pX + (texture.largeur / 2),
		HAUTEUR_FENETRE - pY + (texture.hauteur / 2));
	glEnd();

	glPopMatrix(); // get MODELVIEW matrix value from stack
	glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
	glLoadMatrixd(matrix); // reset
	glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW

	// Activation du tampon de profondeur
	glEnable(GL_DEPTH_TEST);

}