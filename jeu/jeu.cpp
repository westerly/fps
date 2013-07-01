#include "jeu.h"
#include "scene.h"
#include "menu.h"

Jeu::Jeu()
{
    
}
 
Jeu::~Jeu()
{
    
}
 

void Jeu::executer(void)
{
    // Creation de la fenetre avec SDL
    this->initSDL();
 
    // Création du menu
    Menu menu("fond_menu.bmp");
	
    // Boucle d'execution du jeu
  //  bool8 continuer = TRUE;
  //  while(continuer)
  //  {
  //      // Affichage du menu
  //      
		//menu.dessiner();
  //      // Gestion des evenements
  //      SDL_Event evenement;
  //      SDL_WaitEvent(&evenement);
  //      switch(evenement.type)
  //      {
  //          // Quitter
  //          case SDL_QUIT:
  //              continuer = FALSE;
  //              break;
  //      }
  //  }
 
    // Execution de la scene
    Scene scene;
    scene.executer();
 
    // Arret de la SDL
    SDL_Quit();
}

 
void Jeu::initSDL(void)
{
    // Demarrage de la SDL avec le module video
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
 
    // Creation de la fenetre initialisee pour fonctionner avec OpenGL
    SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_OPENGL);
 
    // Titre de l'application
    SDL_WM_SetCaption(TITRE_APPLICATION, NULL);
}