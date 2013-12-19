#ifndef BOUTON_H_INCLUDED
#define BOUTON_H_INCLUDED


#include <string>
#include "types.h"
#include "conteneurTextures.h"
#include "SDL.h"
#include <SDL_ttf.h>
#include "configuration.h"

#define BOUTON_CLIQUE SDL_USEREVENT

class Bouton
{

	private:
		enum Etat { SURVOLE, ENFONCE, PAS_SURVOLE };
		Bouton::Etat etat;

		GLuint textureTexte;
		sint32 positionX, positionY;
		uint32 largeurTexte, hauteurTexte;
		ConteneurTextures conteneurTextures;
		sint32 code;


		GLuint creerTextureTexte(std::string texte);
		Bouton::Etat lectureEtatBouton(void);
		void dessinerTexte(void);
		void dessinerFond(std::string image);
		

	public:
		Bouton(std::string texte, sint32 x, sint32 y, sint32 code);
		~Bouton();
		void dessiner(void);

		
};

#endif

