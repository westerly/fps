#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

#include "Objet3DDeformable.h"



class box : public Objet3DDeformable{

private:

	// Hauteur et largeur de la boite
	float largeur;
	float hauteur;
	// Les comosantes de la couleur de la boite;
	float r, g, b;


public:
	box(float16 positionX = 0.0f, float16 positionY = 0.0f, float16 positionZ = 0.0f, float16 angleHorizontal = 0.0f, float16 angleVertical = 0.0f, btScalar mass = 0.0f, float plargeur = 1, float phauteur = 1, float r = 255, float g = 255, float b = 255);
	virtual ~box();

	void dessinerBox(float x, float y, float z);
	virtual void dessiner();
};

#endif

