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
#include "opencv\cv.h"
#include "opencv2\highgui\highgui.hpp"

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
		SDL_Texture* texte;
		Personnage * personnage;
		animator * animationHandler;
		GameEventHandler * eventHandler;
		Controleur * controleur;
		PhysicEngine * physicHandler;
		int currentFPS;
		bool playWithCamera;
		CvCapture* capture;

		// Les dernières positions connues de la balle rouge trakée par la camera
		float16 lastRedBallX;
		float16 lastRedBallY;
		// Permet de savoir si l'element que l'on traque pour faire feu est dans le champ de la camera
		bool fireElementInside;

        void gererEvenements(void);
        void animer(void);
        void dessiner(void);
		void afficher(void);
		void initOpenGL(void);
		void drawFPS(void);
		void verrouillerSouris(void);
		void deverrouillerSouris(void);
		void handleGunWithCamera(IplImage* imgThreshRedForm);
		void handleFireWithCamera(IplImage* imgThreshGreenForm);
		IplImage* GetThresholdedImage(IplImage* imgHSV, CvScalar lowValues, CvScalar highValue);

    public:

		Scene(SDL_Window *screen, SDL_Renderer * renderer, SDL_GLContext contexteOpenGL, bool playWithCamera = false);
        ~Scene();
        void executer();
		inline int getScore(){ return this->controleur->getNbrPoints(); }
		
};

#endif // SCENE_H_INCLUDED

