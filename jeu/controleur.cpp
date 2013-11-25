#include "controleur.h"
#include "gameEventHandler.h"

Controleur::Controleur(Carte * carte, PhysicEngine * physicEngine)
{
	this->carte = carte;
	this->physicEngine = physicEngine;
	//gContactAddedCallback = this->callbackFunc;	
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
	Bullet * balle = new Bullet(positionX,positionY,positionZ,angleHorizontal,angleVertical,0.5,0.2);
	//this->handlerEvent->hookEvent(balle);
	// Ajout d'une balle dans le vecteur de balles 
	this->Bullets.push_back(balle);
	// Ajout de la balle dans le monde physique
	this->physicEngine->addRigidBody(balle->getBody());

	float16 angleH = balle->getAngleHorizontal();
	float16 angleV = balle->getAngleVertical();

	float16 positionCibleX = balle->getX() - 1 * cos(-(balle->getAngleHorizontal() * RADIANS_PAR_DEGRES));
	float16 positionCibleY = balle->getY() + 1 * sin(-(balle->getAngleHorizontal() * RADIANS_PAR_DEGRES));
	float16 positionCibleZ = HAUTEUR_OEIL_PERSONNAGE + tan(balle->getAngleVertical() * RADIANS_PAR_DEGRES) * 1;

	float16 positionCibleX2 = balle->getX() - 300 * cos(-(balle->getAngleHorizontal() * RADIANS_PAR_DEGRES));
	float16 positionCibleY2 = balle->getY() + 300 * sin(-(balle->getAngleHorizontal() * RADIANS_PAR_DEGRES));
	float16 positionCibleZ2 = HAUTEUR_OEIL_PERSONNAGE + tan(balle->getAngleVertical() * RADIANS_PAR_DEGRES) * 300;

	btVector3 fx(positionCibleX2 - positionCibleX, positionCibleY2 - positionCibleY, positionCibleZ2 - positionCibleZ);

	// On change la gravité de la balle en lui appliquant le vecteur force calculé (permet de tirer une balle droit devant le personnage)
	balle->getBody()->setGravity(fx);

}

void Controleur::drawBullets()
{
	for (std::vector<Bullet*>::iterator element = this->Bullets.begin(); element != this->Bullets.end(); element++)
    {
        static_cast<Bullet*>(*element)->dessiner();
    }
}

bool Controleur::callbackFunc(btManifoldPoint& cp, const btCollisionObject * obj1, int id1, int index1, const btCollisionObject * obj2, int id2, int index2){
	printf("Collision avec une bullet");

	return false;
}

void Controleur::handleCollisions(){

	btDiscreteDynamicsWorld * world = this->physicEngine->getWorld();

	int numManifolds = world->getDispatcher()->getNumManifolds();
	std::vector<Bullet *> tmp;
	for (int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j<numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;

				Objet3DDeformable* test = static_cast<Objet3DDeformable*>(obA->getUserPointer());
				Bullet * bullet = dynamic_cast<Bullet*>(test);
				if (bullet != NULL){
					
					std::vector<Bullet*>::iterator itTmp = find(tmp.begin(), tmp.end(), bullet);
					if (itTmp == tmp.end()){
						std::vector<Bullet*>::iterator it = find(this->Bullets.begin(), this->Bullets.end(), bullet);
						tmp.push_back(*it);
					}
				}
				else{
					test = static_cast<Objet3DDeformable*>(obB->getUserPointer());
					bullet = dynamic_cast<Bullet*>(test);
					if (bullet != NULL){
						std::vector<Bullet*>::iterator itTmp = find(tmp.begin(),tmp.end(), bullet);
						if (itTmp == tmp.end()){
							std::vector<Bullet*>::iterator it = find(this->Bullets.begin(), this->Bullets.end(), bullet);
							tmp.push_back(*it);
						}
					}
				}
			}
		}
	}

	// Libération des objets bullet en collision
	for (std::vector<Bullet*>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		Bullet * currentBullet = *it;
		// Suppression de la balle dans le vector de balles
		std::vector<Bullet*>::iterator it2 = find(this->Bullets.begin(), this->Bullets.end(), currentBullet);
		this->Bullets.erase(it2);
		// Suppression du btRigidBody de la balle dans le monde physique
		this->physicEngine->getWorld()->removeRigidBody(currentBullet->getBody());
		// Suppression de l'objet balle
		delete currentBullet;
	}
}
