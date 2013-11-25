#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED


#include "types.h"
#include <map>
#include <string>
#include "objet3DStatique.h"
#include "Objet3DDeformableBlender.h"
#include "carte.h"
#include "objloader.h"
#include "types.h"
#include "configuration.h"
#include <map>
#include <string>
#include "personnage.h"
#include "carte.h"
#include "animator.h"
#include "target.h"
#include "gameEventHandler.h"
#include "controleur.h"
#include "physicEngine.h"
#include <SDL_ttf.h>

//#include "bullet.h"


class Scene
{

    private:
        //typedef std::map<std::string, GLuint> Textures;

        bool8 continuer;
        uint32 tempsDernierPas;

        Objet3DStatique* skybox;
		Objet3DDeformableBlender * table;
        Carte* carte;
		SDL_Window *screen;
		SDL_Renderer *renderer;
		SDL_GLContext contexteOpenGL;
		TTF_Font * police;
		SDL_Texture* texte;
		Personnage * personnage;
		//std::vector<Bullet*> Bullets;
		animator * animationHandler;
		GameEventHandler * eventHandler;
		Controleur * controleur;
		// test target
		target * targetTest;
		PhysicEngine * physicHandler;
		int currentFPS;


        void gererEvenements(void);
        void animer(void);
        void dessiner(void);
		void afficher(void);
		void initOpenGL(void);


    public:

		Scene(SDL_Window *screen, SDL_Renderer * renderer, SDL_GLContext contexteOpenGL);
        ~Scene();
        void executer();
};

#endif // SCENE_H_INCLUDED

