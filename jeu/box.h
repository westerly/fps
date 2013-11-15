#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

#include "btBulletDynamicsCommon.h"
#include "Objet3DDeformable.h"



class box : public Objet3DDeformable{

private:

	// Hauteur et largeur de la boite
	int largeur;
	int hauteur;


public:
	box(float16 positionX = 0.0f, float16 positionY = 0.0f, float16 positionZ = 0.0f, float16 angleHorizontal = 0.0f, float16 angleVertical = 0.0f, btScalar mass = 0.0f, int plargeur = 1, int phauteur = 1);
	virtual ~box();

	void dessinerBox(float x, float y, float z);
	virtual void dessiner();
};

#endif

