#ifndef CONTROLEUR_H_INCLUDED
#define CONTROLEUR_H_INCLUDED


#include "bullet.h"
#include "animator.h"
#include "carte.h"
#include "physicEngine.h"
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
	PhysicEngine * physicEngine;

	public:
		Controleur(Carte * carte, PhysicEngine * physicEngine);
		~Controleur(void);

		void shootBullet(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical);
		bool thereAreBullets(){return !(this->Bullets.size()==0);}
		void setLinkWithHandlerEvent(GameEventHandler * handlerEvent){this->handlerEvent = handlerEvent;}
		void setLinkWithAnimatorHandler(animator * animationHandler){this->animationHandler = animationHandler;}
		void setLinkWithMap(Carte * carte){this->carte = carte;}
		void drawBullets();
		bool callbackFunc(btManifoldPoint& cp, const btCollisionObject * obj1, int id1, int index1, const btCollisionObject * obj2, int id2, int index2);
		void handleCollisions();
};


#endif // CONTROLEUR_H_INCLUDED
