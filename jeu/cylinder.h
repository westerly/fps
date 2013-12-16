#ifndef CYLINDER_H_INCLUDED
#define CYLINDER_H_INCLUDED

#include "Objet3DDeformable.h"



class cylinder : public Objet3DDeformable{

private:

	// Hauteur et largeur de la boite
	float largeur;
	float hauteur;
	// Les comosantes de la couleur de la boite;
	float r, g, b, a;


public:
	cylinder(float16 positionX = 0.0f, float16 positionY = 0.0f, float16 positionZ = 0.0f, float16 angleHorizontal = 0.0f, float16 angleVertical = 0.0f, btScalar mass = 0.0f, float plargeur = 1, float phauteur = 1, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0);
	virtual ~cylinder();

	void dessinerCylinder(float x, float y, float z);
	virtual void dessiner();
	inline void cylinder::setRGBA(float p_r, float p_g, float p_b, float p_a){ r = p_r; g = p_g; b = p_b; a = p_a; }

	// Renvoie true si l'élément est un élément à shooter
	inline bool isHighlighted(){ return (this->r == 255 && this->g == 0 && this->b == 0); }
};

#endif

