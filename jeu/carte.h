#ifndef CARTE_H_INCLUDED
#define CARTE_H_INCLUDED

#include "types.h"

#include "conteneurTextures.h"
#include "SDL.h"

#include "configuration.h"


class Carte
{
    private:
        ConteneurTextures conteneurTextures;

        bool8* carte;
        uint32 largeurCarte;
        uint32 hauteurCarte;
        GLuint listeAffichage;

        void creerListeAffichage();

    public:
        Carte(const char* nomFichier);
        ~Carte();
        void dessiner();

        void entourage(sint32 x, sint32 y, bool8 entourage[8]);

		void entourage(sint32 x, sint32 y, sint32 z, bool8 entourage[8]);
};

#endif // CARTE_H_INCLUDED
