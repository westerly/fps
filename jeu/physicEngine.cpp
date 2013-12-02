#include "physicEngine.h"


PhysicEngine::PhysicEngine()
{
	// Initialisation

	//Contient la configuration par défaut pour la mémoire, la collision
	collisionConfiguration = new btDefaultCollisionConfiguration();

	// Le collision dispatcher par défaut. Pour du processus parallèle utilisez un dispatcher différent. Regardez Extras/BulletMultiThreaded.
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	// Initialisation du broadphase (détecteur des objets où leurs AABB se chevauchent)
	broadphase = new btDbvtBroadphase();

	// Constraint solver par défaut. Pour du processus parallèle utilisez un constrint solver différent. Regardez Extras/BulletMultiThreaded.
	sequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;
	

	// On initialise le monde physique Bullet.
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, sequentialImpulseConstraintSolver, collisionConfiguration);

	// On définit la gravité, de façon à ce que les objets tombent vers le bas (-Z).
	world->setGravity(btVector3(0, 0, -10));

}


PhysicEngine::~PhysicEngine()
{
	delete world;
	delete sequentialImpulseConstraintSolver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfiguration;
}

void PhysicEngine::addRigidBody(btRigidBody * body){

	this->world->addRigidBody(body);
}


void PhysicEngine::addRigidBody(target * t){

	Objet3DDeformable* el;
	for (std::vector<Objet3DDeformable*>::iterator obj = t->getObjets().begin(); obj != t->getObjets().end(); obj++)
	{
		el = *obj;
		this->world->addRigidBody(el->getBody());
	}
}


btDiscreteDynamicsWorld * PhysicEngine::getWorld(){
	return this->world;
}