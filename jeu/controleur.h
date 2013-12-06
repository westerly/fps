#ifndef CONTROLEUR_H_INCLUDED
#define CONTROLEUR_H_INCLUDED


#include "bullet.h"
#include "animator.h"
#include "carte.h"
#include "physicEngine.h"
#include "personnage.h"
#include <vector>
#include <algorithm>
#include <string>


class GameEventHandler;

class Controleur
{
	GameEventHandler * handlerEvent;
	animator * animationHandler;
	Carte * carte;
	PhysicEngine * physicEngine;
	Personnage * personnage;

	public:
		inline Controleur::Controleur(Carte * p_carte, PhysicEngine * p_physicEngine, Personnage * p_personnage) :carte(p_carte), physicEngine(p_physicEngine), personnage(p_personnage){}
		~Controleur(void);

		void shootBullet(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical);
		void setLinkWithHandlerEvent(GameEventHandler * handlerEvent){this->handlerEvent = handlerEvent;}
		void setLinkWithAnimatorHandler(animator * animationHandler){this->animationHandler = animationHandler;}
		void setLinkWithMap(Carte * carte){this->carte = carte;}

		void handleBullets();

};


#endif // CONTROLEUR_H_INCLUDED
