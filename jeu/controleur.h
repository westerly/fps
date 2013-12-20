#ifndef CONTROLEUR_H_INCLUDED
#define CONTROLEUR_H_INCLUDED


#include "bullet.h"
#include "animator.h"
#include "carte.h"
#include "physicEngine.h"
#include "personnage.h"
#include "Helper.h"
#include <vector>
#include <string>
#include <SDL_ttf.h>



class GameEventHandler;

class Controleur
{
	GameEventHandler * handlerEvent;
	animator * animationHandler;
	Carte * carte;
	PhysicEngine * physicEngine;
	Personnage * personnage;
	typedef std::set<std::string> Textures;
	Textures textures; // Les textures de l'objet
	ConteneurTextures conteneurTextures;
	// Vecteur de targets contenant toute les cibles affichés (donc à gérer) dans le jeu
	std::vector<target*> targets;
	// Nombre de vies du joueur
	int nbrVies;
	// Le nombre de points courant du joueur
	int nbrPoints;
	// Texture du texte qui affche le nombre de points du personnage
	Texture textureTextePoints;

	// Temps maximum de courant de vie d'une cible (évolue avec la difficulté)
	uint32 current_max_living_time_target;

	public:
		Controleur::Controleur(Carte * p_carte, PhysicEngine * p_physicEngine, Personnage * p_personnage);
		~Controleur(void);

		void shootBullet(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical);
		void setLinkWithHandlerEvent(GameEventHandler * handlerEvent){this->handlerEvent = handlerEvent;}
		void setLinkWithAnimatorHandler(animator * animationHandler){this->animationHandler = animationHandler;}
		void setLinkWithMap(Carte * carte){this->carte = carte;}
		
		// Fonction a appeler au lancement du jeu 
		void startGame();
		// Dessine les cibles
		void drawTargets();

		// Renvoi true si une cible affichée et trop proche de la position x,y et false dans le cas contraire
		bool targetToCloseFromXY(float x, float y, float size);

		// MAJ les timers des targets comme (displayedsince et sinceIsShot)
		void updateTimersTargets();

		void handleTargets();
		// Créer une cible à une position random sur la carte
		void createTarget();
		// Permet de supprimer tous les rigidBody des elements de la cible du monde physique et de la mémoire
		void deleteRigidBodiesFromTarget(target *tr);

		// Fonction qui gère lorsque un element en surbrillance el a été shooté
		void handleElHighlightedShot(Objet3DDeformable * el);

		// Permet de MAJ le nombre de points du joueur qui vient de toucher correctement la cible t
		// Le nombre de points gagné est calculé en fonction de la position du joueur par rapport à la cible 
		// et par rapport à sa rapidité c'est à dire par rapport au paramètre displayedsince de la cible
		void majNbrPointsPlayer(target * t);

		inline int getNbrPoints(){ return this->nbrPoints; }

		
		void creerTextureTextPoints();
		// Dessine et créer la texture qui affiche le nombre de points du joueur
		void dessinerTextePoints();
		// Dessine les vies du joueur
		void dessinerVie();

		inline bool persoIsAlive(){ return this->nbrVies != 0; }
};


#endif // CONTROLEUR_H_INCLUDED
