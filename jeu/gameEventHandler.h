#ifndef GAMEEVENTHANDLER_H_INCLUDED
#define GAMEEVENTHANDLER_H_INCLUDED

#include "personnage.h"
#include "types.h"
#include "controleur.h"


[event_receiver(native)]
class GameEventHandler
{

		Controleur  * controleur;

	public:

		void shootBullet(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical);

		void hookEvent(Personnage* pSource) {
		  __hook(&Personnage::tirer, pSource, &GameEventHandler::shootBullet);
		}

		void unhookEvent(Personnage* pSource) {
		__unhook(&Personnage::tirer, pSource, &GameEventHandler::shootBullet);
		}

		void setLinkWithControleur(Controleur * link){this->controleur = link;}
};




#endif // GAMEEVENTHANDLER_H_INCLUDED