#include "target.h"


target::target(float16 positionX, float16 positionY, float16 positionZ, int largeur, int hauteur, int nbr_obj_base)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->positionZ = positionZ;
	this->largeur = largeur;
	this->hauteur = hauteur;
	this->number_objets_base = nbr_obj_base;

	int j = nbr_obj_base;
	while (j != 0){
		for (int i = 0; i < j; i++){
			this->objets.push_back(new box(positionX, positionY + i * largeur + i * (largeur / 3), (nbr_obj_base - j) * hauteur + positionZ, 0.0, 0.0, 0.5, 1, 1));
		}
		j--;
	}
}


target::~target()
{
	// Libération des objets
	for (std::vector<Objet3DDeformable*>::iterator obj = this->objets.begin(); obj != this->objets.end(); obj++)
	{
		delete *obj;
	}
}


void target::dessiner(){
	Objet3DDeformable* el;
	for (std::vector<Objet3DDeformable*>::iterator obj = this->objets.begin(); obj != this->objets.end(); obj++)
	{
		el = *obj;
		el->dessiner();
	}
}

std::vector<Objet3DDeformable*> & target::getObjets(){
	return this->objets;
}
