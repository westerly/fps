#ifndef ARME_H_INCLUDED
#define ARME_H_INCLUDED


#include <string>
#include <vector>
#include <set>
#include "types.h"

#include "conteneurTextures.h"


class arme
{
	private:

		typedef std::set<std::string> Textures;

        Textures textures; // Les textures de l'objet
        ConteneurTextures conteneurTextures;


	public:

		std::string fichierTextureCouranteDisplayed;

		arme(std::string fichierTexture);
		arme::arme(std::string fichierTexture, std::vector<std::string> & texturesAnimation);

		virtual ~arme(void);
		void ajouterTexture(std::string nomFichierTexture);
		void dessiner(float16 positionX,float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical);
};


#endif 

