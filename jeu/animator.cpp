#include "animator.h"
#include "configuration.h"
#include <sstream>


animator::animator(void)
{
	this->animationArmeEnCours = false;
	this->cptImagesArmeDisplayed = 1;
}


animator::~animator(void)
{
	// Lib�ration des textures
    for (Textures::iterator element = this->textures.begin(); element != textures.end(); element++)
    {
        this->conteneurTextures.supprimer(*element);
    }
}

// Ajoute et charge une texture � partir d'un fichier
// La texture est aussi ajout� aux textures de l'objet 
void animator::ajouterTexture(std::string nomFichierTexture)
{
	// Si la texture n'est pas deja creee
    if(this->textures.find(nomFichierTexture) == this->textures.end())
    {
        this->conteneurTextures.ajouter(nomFichierTexture);
        this->textures.insert(nomFichierTexture);
    }
}



void animator::animer(void)
{

	// Animation non statiques
	if(this->animationArmeEnCours && this->cptImagesArmeDisplayed == 7)
	{
		this->animationArmeEnCours = false;
		this->cptImagesArmeDisplayed = 1;
		this->armePersonnage->fichierTextureCouranteDisplayed = "personnage\\gun.png";

	}else{

		if (this->animationArmeEnCours && ((SDL_GetTicks() - this->lastTimeImageArmeDisplayed) >= FREQUENCE_IMAGE_ANIMATION_ARME))
		{
			// cr�er un flux de sortie
			std::ostringstream oss;
			// �crire un nombre dans le flux
			oss << this->cptImagesArmeDisplayed;
			// r�cup�rer une cha�ne de caract�res
			std::string cptString = oss.str();

			this->lastTimeImageArmeDisplayed = SDL_GetTicks();
			this->armePersonnage->fichierTextureCouranteDisplayed = "personnage\\gun_shoot" + cptString + ".png";
			this->cptImagesArmeDisplayed ++;
		}
	}

	// Animations Statiques
}


void animator::beginAnimationArme(void)
{
	this->animationArmeEnCours = true;
	this->lastTimeImageArmeDisplayed = SDL_GetTicks();
}