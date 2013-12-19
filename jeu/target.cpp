#include "target.h"


target::target(float16 positionX, float16 positionY, float16 positionZ, float largeur, float hauteur, int nbr_obj_base, int type)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->positionZ = positionZ;
	this->largeur = largeur;
	this->hauteur = hauteur;
	this->number_objets_base = nbr_obj_base;
	// L'élément en surbrillance n'est pas touché à la création de la cible
	this->shotAt = 0;
	this->shot = false;


	// Tirage aléatoire du numéro de l'élément à mettre en surbrillance
	int highlited_el_num = Helper::rand_a_b(0,Helper::sum_n(nbr_obj_base));
	int cpt = 0;

	switch (type){
		
		// Création d'une cible de type cylindre
		case 0:
		{

			int j = nbr_obj_base;
			while (j != 0){
				/*for (int i = 0; i < j; i++){
					this->objets.push_back(new box(positionX, positionY + i * largeur + i * (largeur / 3) + (nbr_obj_base - j) * ((2 * largeur) / 3),
					(nbr_obj_base - j) * (hauteur + 0.2) + positionZ, 0.0, 0.0, 5.0, this->largeur, this->hauteur));
					}*/

				for (int i = 0; i < j; i++){

					float zPlus = 0.05;
					if (hauteur>0.5){
						zPlus = 0.25;
					}

					if (cpt == highlited_el_num){
						this->objets.push_back(new cylinder(positionX, positionY + i * largeur + i * (ESPACEMENT_ENTRE_ELEMENTS * largeur) + (nbr_obj_base - j) * (largeur - (ESPACEMENT_ENTRE_ELEMENTS * largeur)),
							(nbr_obj_base - j) * (hauteur)+positionZ + zPlus, 0.0, 0.0, 5.0, this->largeur, this->hauteur, 255, 0, 0, 1));
					}
					else{
						this->objets.push_back(new cylinder(positionX, positionY + i * largeur + i * (ESPACEMENT_ENTRE_ELEMENTS * largeur) + (nbr_obj_base - j) * (largeur - (ESPACEMENT_ENTRE_ELEMENTS * largeur)),
							(nbr_obj_base - j) * (hauteur)+positionZ + zPlus, 0.0, 0.0, 5.0, this->largeur, this->hauteur));
					}

					cpt++;

				}
				j--;	
			}
			
			break;
		}
		
		// Création d'une cible de type box
		case 1:
		{
			int j = nbr_obj_base;
			while (j != 0){
				/*for (int i = 0; i < j; i++){
				this->objets.push_back(new box(positionX, positionY + i * largeur + i * (largeur / 3) + (nbr_obj_base - j) * ((2 * largeur) / 3),
				(nbr_obj_base - j) * (hauteur + 0.2) + positionZ, 0.0, 0.0, 5.0, this->largeur, this->hauteur));
				}*/

				for (int i = 0; i < j; i++){

					float zPlus = 0.05;
					if (hauteur>0.5){
						zPlus = 0.25;
					}

					if (cpt == highlited_el_num){
						this->objets.push_back(new box(positionX, positionY + i * largeur + i * (ESPACEMENT_ENTRE_ELEMENTS * largeur) + (nbr_obj_base - j) * (largeur - (ESPACEMENT_ENTRE_ELEMENTS * largeur)),
							(nbr_obj_base - j) * (hauteur)+positionZ + zPlus, 0.0, 0.0, 5.0, this->largeur, this->hauteur, 255, 0, 0, 1));
					}
					else{
						this->objets.push_back(new box(positionX, positionY + i * largeur + i * (ESPACEMENT_ENTRE_ELEMENTS * largeur) + (nbr_obj_base - j) * (largeur - (ESPACEMENT_ENTRE_ELEMENTS * largeur)),
							(nbr_obj_base - j) * (hauteur)+positionZ + zPlus, 0.0, 0.0, 5.0, this->largeur, this->hauteur));
					}
					cpt++;
				}
				j--;
			}

			break;
		}
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

bool target::elIsPartOfTarget(Objet3DDeformable * obj){
	
	return (std::find(this->objets.begin(), this->objets.end(),obj) != this->objets.end());
}
