#ifndef CONTENEURTEXTURES_H_INCLUDED
#define CONTENEURTEXTURES_H_INCLUDED

#include "types.h"
#include <map>
#include <sstream>

#include <string>

#include "SDL.h"
#include < glut.h >
#include "SDL_image.h"

struct Texture
{
	GLuint texture;
	uint32 compteur;
	sint32 largeur;
	sint32 hauteur;
};

class ConteneurTextures 
{
    private:

        typedef std::map<std::string, Texture> Textures;

        static Textures textures;
        void chargerTexture(const char* nomFichier, GLuint* texture, sint32* largeur, sint32* hauteur);

    public:
        Texture ajouter(std::string nomFichier);
        void supprimer(std::string nomFichier);
        Texture texture(std::string nomFichier);
};

#endif // CONTENEURTEXTURES_H_INCLUDED
