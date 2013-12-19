#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

#include "btBulletDynamicsCommon.h"
#include "Objet3DDeformable.h"
#include "box.h"
#include "cylinder.h"
#include "types.h"
#include "configuration.h"
#include "Helper.h"
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

	// nombre de millisecondes depuis l'initialisation de la SDL et la création de la cible
	uint32 timeStart;
	// Nombre de millisecondes de "vie" de la cible (Depuis combien de temps l'element en surbrillance dans la cible n'a pas été shooté)
	uint32 displayedsince;

	// Boolean permettant de savoir si le bon élément en surbrillance a été touché
	bool shot;

	// Temps en millisecondes entre l'initialisation de la SDL et le moment ou le bon élément de la cible a été shooté
	uint32 shotAt;
	

public:

	// type : 0 pour target cylindre, 1 pour target box
	target(float16 positionX = 0.0f, float16 positionY = 0.0f, float16 positionZ = 0.0f, float largeur = 1, float hauteur = 1, int nbr_obj_base = 3, int type = 0);
	virtual ~target();

	void dessiner();

	std::vector<Objet3DDeformable*> &getObjets();

	inline void setTimeStart(uint32 t){ this->timeStart = t; }
	inline void setDisplayedSince(uint32 t){ this->displayedsince = t; }
	inline uint32 getTimeStart(){ return this->timeStart; }
	inline uint32 getDisplayedSince(){ return this->displayedsince; }
	inline std::vector<Objet3DDeformable*> * getTargetElements(){ return &this->objets; }

	inline float16 getPositionX(){ return this->positionX; }
	inline float16 getPositionY(){ return this->positionY; }
	inline float16 getPositionZ(){ return this->positionZ; }
	inline float getLargeur(){ return this->largeur; }
	inline float getHauteur(){ return this->hauteur; }
	// Retourne la taille de la cible
	inline float getSize(){ return this->largeur * this->number_objets_base + (ESPACEMENT_ENTRE_ELEMENTS * this->largeur) * (this->number_objets_base - 1); }

	// Renvoie true si le paramètre pointe sur un élément de la cible
	bool elIsPartOfTarget(Objet3DDeformable * obj);

	inline void setIsShot(bool b){ this->shot = b;}
	inline bool isShot(){ return shot; }
	inline void setShotAt(uint32 t){ this->shotAt = t; }
	inline uint32 getShotAt(){ return this->shotAt; }
	// Retourne le nombre de millisecondes depuis lequel le bon élément de la cible a été shooté
	inline uint32 getSinceIsShot() { return SDL_GetTicks() - this->shotAt; }

};


#endif

