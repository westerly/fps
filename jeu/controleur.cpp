#include "controleur.h"
#include "gameEventHandler.h"

Controleur::~Controleur(void)
{

}


void Controleur::shootBullet(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical)
{	
	// Calcul du point 2 pour construire le vecteur direction (utilisé pour le ray casting)
	float16 positionCibleX2 = this->personnage->getX() - 200 * cos(-(this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES));
	float16 positionCibleY2 = this->personnage->getY() + 200 * sin(-(this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES));
	float16 positionCibleZ2 = HAUTEUR_OEIL_PERSONNAGE + tan(this->personnage->getAngleVertical() * RADIANS_PAR_DEGRES) * 200;
	
	// Construction du vecteur force de base à appliquer au point de contact entre la boite et le ray 
	btVector3 vecteurDirection((this->personnage->getX() - 2 * cos(-(this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES))) - positionX,
		(this->personnage->getY() + 2 * sin(-(this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES))) - positionY,
		(HAUTEUR_OEIL_PERSONNAGE + tan(this->personnage->getAngleVertical() * RADIANS_PAR_DEGRES) * 2) - positionZ);

	// Start est le point de départ du ray et End est le point d'arrivé (calculé à 200 unités du personnage)
	btVector3 Start(positionX, positionY, positionZ);
	btVector3 End(positionCibleX2, positionCibleY2, positionCibleZ2);
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start,End);

	// Perform raycast
	this->physicEngine->getWorld()->rayTest(Start, End, RayCallback);

	// Si le ray a touché quelque chose
	if (RayCallback.hasHit()) {
		End = RayCallback.m_hitPointWorld;
		btVector3 Normal = RayCallback.m_hitNormalWorld;

		const btCollisionObject * objectShooted = RayCallback.m_collisionObject;
		Objet3DDeformable* test = static_cast<Objet3DDeformable*>(objectShooted->getUserPointer());
		box * boxInCollision = dynamic_cast<box*>(test);
		// Si une boxe est touché par le ray 
		if (boxInCollision != NULL){
			boxInCollision->getBody()->activate();
			// On applique une force au point d'impact entre le ray et la boite 
			boxInCollision->getBody()->applyImpulse(vecteurDirection * 5, RayCallback.m_hitNormalWorld);
			boxInCollision->setRGBA(255, 0.1, 0.1, 0.1);
		}
		else{
			cylinder * cylinderInCollision = dynamic_cast<cylinder*>(test);
			if (cylinderInCollision != NULL){
				cylinderInCollision->getBody()->activate();
				// On applique une force au point d'impact entre le ray et la boite 
				cylinderInCollision->getBody()->applyImpulse(vecteurDirection * 5, RayCallback.m_hitNormalWorld);
				cylinderInCollision->setRGBA(255, 0.1, 0.1, 0.1);
			}
		}
	}
}


