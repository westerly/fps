#ifndef PHYSICENGINE_H_INCLUDED
#define PHYSICENGINE_H_INCLUDED

#include "target.h"
#include "MyDebug.h"

class PhysicEngine
{
private:
	// Le nom du monde physique
	btDiscreteDynamicsWorld * world;
	// La classe btBroadphaseInterface fournit une interface pour détecter les objets où leurs AABB se chevauchent.
	btBroadphaseInterface * broadphase;
	// btCollisionDispatcher supporte des algorithmes qui peuvent gérer des pairs de collisions ConvexConvex et ConvexConcave. Temps de l'impact, le point le plus proche et pénétration de profondeur.
	btCollisionDispatcher * dispatcher;
	// btCollisionConfiguration permet de configurer les allocataires de mémoire.
	btDefaultCollisionConfiguration* collisionConfiguration;
	// btSequentialImpulseConstraintSolver est une implémentation SIMD rapide de la méthode Projected Gauss Seidel (iterative LCP).
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
