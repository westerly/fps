#ifndef CONTROLEUR_H_INCLUDED
#define CONTROLEUR_H_INCLUDED


#include "bullet.h"
#include "animator.h"
#include "carte.h"
#include <vector>
#include <algorithm>
#include <string>


class GameEventHandler;

class Controleur
{

	std::vector<Bullet*> Bullets;

	GameEventHandler * handlerEvent;

	animator * animationHandler;

	Carte * carte;

	public:
		Controleur(void);
		~Controleur(void);

		void shootBullet(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical);

		// Fonction qui gère l'ensemble des balles de la Scène
		// La distance reeprésente la distance à parcourir pour chaque balle
		void handleBullets(float16 & distance);

		void handleBulletCollision(Bullet * balle);

		bool thereAreBullets(){return !(this->Bullets.size()==0);}

		void setLinkWithHandlerEvent(GameEventHandler * handlerEvent){this->handlerEvent = handlerEvent;}
		void setLinkWithAnimatorHandler(animator * animationHandler){this->animationHandler = animationHandler;}
		void setLinkWithMap(Carte * carte){this->carte = carte;}

		void drawBullets();

};


#endif // CONTROLEUR_H_INCLUDED
