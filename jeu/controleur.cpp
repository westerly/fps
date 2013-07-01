#include "controleur.h"
#include "gameEventHandler.h"

Controleur::Controleur(void)
{
}


Controleur::~Controleur(void)
{
	// Destruction des balles
	for (std::vector<Bullet*>::iterator element = this->Bullets.begin(); element != this->Bullets.end(); element++)
    {
        delete *element;
    }
}


void Controleur::shootBullet(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical)
{
	std::vector<std::string> v;

	v.push_back("bullet\\animation_destruction_bullet_1.png");
	v.push_back("bullet\\animation_destruction_bullet_2.png");
	
	Bullet * balle = new Bullet(v,positionX,positionY,positionZ,angleHorizontal,angleVertical);
	//this->handlerEvent->hookEvent(balle);

	this->Bullets.push_back(balle);

}

void Controleur::drawBullets()
{
	for (std::vector<Bullet*>::iterator element = this->Bullets.begin(); element != this->Bullets.end(); element++)
    {
        static_cast<Bullet*>(*element)->dessiner();
    }
}


void Controleur::handleBullets(float16 & distance)
{

	int i = 0;
	while(i<this->Bullets.size())
    {
		Bullet * balle = this->Bullets.at(i);

	   // Recuperation de l'environnement
       sint32 positionCarteX = 0, positionCarteY = 0, positionCarteZ = 0;
	   bool8 entouragePersonnage[8];
	   balle->positionSurLaCarte(&positionCarteX,&positionCarteY,&positionCarteZ);
	   this->carte->entourage(positionCarteY, positionCarteX,positionCarteZ, entouragePersonnage);

	   balle->deplacer(distance,entouragePersonnage);

	   if(balle->isInColision())
		{
			// Suppression de l'objet balle
			delete balle;

			std::vector<Bullet*>::iterator it = find(this->Bullets.begin(),this->Bullets.end(),balle);
			// Suppression de la balle dans le vector de balles
			this->Bullets.erase(it);
			i++;
		}
	   i++;
    }

}
//
//// Gérer la collision de la balle
//void Controleur::handleBulletCollision(Bullet * balle)
//{
//	//this->animationHandler->beginAnimationCollisionBullet(balle->getX(), balle->getY(), balle->getZ());
//
//	std::vector<Bullet*>::iterator it = find(this->Bullets.begin(),this->Bullets.end(),balle);
//
//	// Suppression de l'objet balle
//	delete *it;
//
//	// Suppression de la balle dans le vector de balles
//	this->Bullets.erase(it);
//	
//
//}