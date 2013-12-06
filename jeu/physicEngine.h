#ifndef PHYSICENGINE_H_INCLUDED
#define PHYSICENGINE_H_INCLUDED

#include "target.h"
#include "MyDebug.h"

class PhysicEngine
{
private:
	// Le nom du monde physique
	btDiscreteDynamicsWorld * world;
	// La classe btBroadphaseInterface fournit une interface pour d�tecter les objets o� leurs AABB se chevauchent.
	btBroadphaseInterface * broadphase;
	// btCollisionDispatcher supporte des algorithmes qui peuvent g�rer des pairs de collisions ConvexConvex et ConvexConcave. Temps de l'impact, le point le plus proche et p�n�tration de profondeur.
	btCollisionDispatcher * dispatcher;
	// btCollisionConfiguration permet de configurer les allocataires de m�moire.
	btDefaultCollisionConfiguration* collisionConfiguration;
	// btSequentialImpulseConstraintSolver est une impl�mentation SIMD rapide de la m�thode Projected Gauss Seidel (iterative LCP).
	btSequentialImpulseConstraintSolver *sequentialImpulseConstraintSolver;

	MyDebug * debugger;

public:
	PhysicEngine();
	virtual ~PhysicEngine();

	// Permet d'ajouter un objet rigide au monde physique Bullet
	void addRigidBody(btRigidBody * body);
	// Ajouter tous les objets rigides d'une cible au monde physique
	void addRigidBody(target * t);
	void addRigidBodies(std::vector<btRigidBody*> bodies);

	btDiscreteDynamicsWorld * getWorld();
};


#endif
