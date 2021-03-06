#ifndef CARTE_H_INCLUDED
#define CARTE_H_INCLUDED

#include "types.h"

#include "conteneurTextures.h"
#include "SDL.h"

#include "configuration.h"
#include "btBulletDynamicsCommon.h"
#include "Objet3DDeformable.h"

#include <vector>


class Carte : public Objet3DDeformable{

    private:
        //ConteneurTextures conteneurTextures;

        bool8* carte;
        uint32 largeurCarte;
        uint32 hauteurCarte;
        GLuint listeAffichage;

        void creerListeAffichage();


		// Attributs pour la physique du sol
		// Position, orientation.
		btTransform transform;
		btCollisionShape *shape;
		//btDefaultMotionState fournit une implémentation pour synchroniser les transformations.
		btDefaultMotionState *motionState;
		//Une matrice OpenGL, pour récupérer la position, rotation d'un objet.
		btScalar matrix[16];
		//btRigidBody est la classe principale des objets rigides
		btRigidBody *body_sol;


		// Attributs pour la physique des murs
		std::vector<btTransform> transformWalls;
		std::vector<btCollisionShape*> shapeWalls;
		std::vector<btDefaultMotionState*> motionStateWalls;
		std::vector<btScalar[16]> matrixWalls;
		std::vector<btRigidBody*> bodyWalls;
		

    public:
		Carte(const char* nomFichier, float16 positionX = 0.0f, float16 positionY = 0.0f, float16 positionZ = 0.0f, float16 angleHorizontal = 0.0f, float16 angleVertical = 0.0f, btScalar mass = 0.0f);
        ~Carte();
        void dessiner();
        void entourage(sint32 x, sint32 y, bool8 entourage[8]);
		void entourage(sint32 x, sint32 y, sint32 z, bool8 entourage[8]);
		btRigidBody * getBody_Sol();
		inline std::vector<btRigidBody*> getRigidBodiesWalls(){ return this->bodyWalls; };
		inline uint32 getLargeur(){ return this->largeurCarte; }
		inline uint32 getHauteur(){ return this->hauteurCarte; }
};

#endif // CARTE_H_INCLUDED
