#include "physicEngine.h"


PhysicEngine::PhysicEngine()
{
	// Initialisation

	//Contient la configuration par d�faut pour la m�moire, la collision
	collisionConfiguration = new btDefaultCollisionConfiguration();

	// Le collision dispatcher par d�faut. Pour du processus parall�le utilisez un dispatcher diff�rent. Regardez Extras/BulletMultiThreaded.
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	// Initialisation du broadphase (d�tecteur des objets o� leurs AABB se chevauchent)
	broadphase = new btDbvtBroadphase();

	// Constraint solver par d�faut. Pour du processus parall�le utilisez un constrint solver diff�rent. Regardez Extras/BulletMultiThreaded.
	sequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;
	

	// On initialise le monde physique Bullet.
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, sequentialImpulseConstraintSolver, collisionConfiguration);

	// On d�finit la gravit�, de fa�on � ce que les objets tombent vers le bas (-Z).
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