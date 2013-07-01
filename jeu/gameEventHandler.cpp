#include "gameEventHandler.h"


void GameEventHandler::shootBullet(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical)
{
	this->controleur->shootBullet(positionX,positionY,positionZ,angleHorizontal,angleVertical);
}

