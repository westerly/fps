#ifndef ANIMATOR_H_INCLUDED
#define ANIMATOR_H_INCLUDED

#include "conteneurTextures.h"
#include "arme.h"

class animator
{

private:

	typedef std::set<std::string> Textures;

    Textures textures; // Les textures de l'objet
    ConteneurTextures conteneurTextures;
	
	bool animationArmeEnCours;
	int cptImagesArmeDisplayed;
	uint32 lastTimeImageArmeDisplayed;


	void ajouterTexture(std::string nomFichierTexture);

public:

	arme * armePersonnage;

	animator(void);
	~animator(void);

	void beginAnimationArme(void);

	void beginAnimationCollisionBullet(void);

	void animer(void);


};

#endif