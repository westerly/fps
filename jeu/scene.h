#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED


#include "types.h"
#include <map>
#include <string>
#include "objet3DStatique.h"
#include "carte.h"

#include "types.h"
#include "configuration.h"
#include <map>

#include <string>
#include "personnage.h"
#include "carte.h"
#include "animator.h"

#include "gameEventHandler.h"
#include "controleur.h"

//#include "bullet.h"


class Scene
{

    private:
        //typedef std::map<std::string, GLuint> Textures;

        bool8 continuer;
        uint32 tempsDernierPas;

        Objet3DStatique* skybox;
        Carte* carte;

        void gererEvenements(void);
        void animer(void);
        void dessiner(void);
        void afficher(void);

        Personnage * personnage;

		//std::vector<Bullet*> Bullets;

		animator * animationHandler;

		GameEventHandler * eventHandler;

		Controleur * controleur;

		void initOpenGL(void);


    public:

        Scene();
        ~Scene();
        void executer();
};

#endif // SCENE_H_INCLUDED

