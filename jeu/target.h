#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

#include "btBulletDynamicsCommon.h"
#include "Objet3DDeformable.h"
#include "box.h"
#include "cylinder.h"
#include "types.h"
#include <vector>

class target
{
	// Vector de l'ensemble des objets de base déformable qui composent la cible
	std::vector<Objet3DDeformable*> objets;

	// Le nombre d'objets à la base de la cible
	int number_objets_base;

	// Largeur et hauteur des objets qui composent la cible
	float largeur;
	float hauteur;

	// Position de la cible (position du premier élément de la base)
	float16 positionX;
	float16 positionY;
	float16 positionZ;


public:
	target(float16 positionX = 0.0f, float16 positionY = 0.0f, float16 positionZ = 0.0f, float largeur = 1, float hauteur = 1, int nbr_obj_base = 3);
	virtual ~target();

	void dessiner();

	std::vector<Objet3DDeformable*> &getObjets();

};


#endif

