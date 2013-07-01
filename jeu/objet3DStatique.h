#ifndef OBJET3DSTATIQUE_H_INCLUDED
#define OBJET3DSTATIQUE_H_INCLUDED

#include <string>
#include "types.h"
#include <string>
#include <set>
#include "types.h"

#include "conteneurTextures.h"

class Objet3DStatique
{
    private:
       

    protected:

		typedef std::set<std::string> Textures;

        Textures textures; // Les textures de l'objet
        ConteneurTextures conteneurTextures;


        // Position de l'objet
        float16 positionX;
        float16 positionY;
        float16 positionZ;

		GLuint listeAffichage;
        GLuint listeRST;

        // Angle horizontal avec la verticale en z
        float16 angleHorizontal;

        std::string nom;

    public:
        Objet3DStatique(std::string nomFichier);
        ~Objet3DStatique();
        void dessiner();
};

#endif // OBJET3DSTATIQUE_H_INCLUDED
