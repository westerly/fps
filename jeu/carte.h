#ifndef CARTE_H_INCLUDED
#define CARTE_H_INCLUDED

#include "types.h"

#include "conteneurTextures.h"
#include "SDL.h"

#include "configuration.h"
#include "btBulletDynamicsCommon.h"


class Carte
{
    private:
        ConteneurTextures conteneurTextures;

        bool8* carte;
        uint32 largeurCarte;
        uint32 hauteurCarte;
        GLuint listeAffichage;

        void creerListeAffichage();

		// Position, orientation.
		btTransform transform;
		btCollisionShape *shape;
		//btDefaultMotionState fournit une implémentation pour synchroniser les transformations.
		btDefaultMotionState *motionState;
		//Une matrice OpenGL, pour récupérer la position, rotation d'un objet.
		btScalar matrix[16];
		//btRigidBody est la classe principale des objets rigides
		btRigidBody *body_sol;

    public:
        Carte(const char* nomFichier);
        ~Carte();
        void dessiner();

        void entourage(sint32 x, sint32 y, bool8 entourage[8]);

		void entourage(sint32 x, sint32 y, sint32 z, bool8 entourage[8]);

		btRigidBody * getBody_Sol();
};

#endif // CARTE_H_INCLUDED
