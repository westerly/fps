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
    Menu menu("fond_menu.bmp", this->screen);

	// Generation des codes de bouton
	enum { BOUTON_JOUER, BOUTON_QUITTER };

	menu.ajouterBouton("Jouer", LARGEUR_FENETRE / 2, 2.5 * HAUTEUR_FENETRE / 6, BOUTON_JOUER);
	menu.ajouterBouton("Quitter", LARGEUR_FENETRE / 2, 3.5 * HAUTEUR_FENETRE / 6, BOUTON_QUITTER);

	int score = NULL;
	
    // Boucle d'execution du jeu
    bool8 continuer = TRUE;
    while(continuer)
    {
        // Affichage du menu
		menu.dessiner(score);
        // Gestion des evenements
        SDL_Event evenement;
        SDL_WaitEvent(&evenement);
        switch(evenement.type)
        {
            // Quitter
			case SDL_QUIT:{
				continuer = FALSE;
                break;
			}
                
			// Un bouton a ete clique
			case BOUTON_CLIQUE:
			{
				switch (evenement.user.code)
				{
					case BOUTON_JOUER:
					{
						// Execution de la scene
						Scene * scene = new Scene(this->screen, this->renderer, this->contexteOpenGL,this->playWithCamera);
						scene->executer();
						// On récupère le dernier score
						score = scene->getScore();
						// On supprime la scene
						delete scene;
						break;
					}
				
					case BOUTON_QUITTER:
					{
						continuer = FALSE;
						break;
					}
				}
				break;
			}
        }
    }
 

	// Arret de SDL_ttf
	TTF_Quit();
 
    // Arret de la SDL
	SDL_GL_DeleteContext(this->contexteOpenGL);
	SDL_DestroyWindow(this->screen);
	SDL_DestroyRenderer(this->renderer);
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
    // Correctif SDL 2.0 this->ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_OPENGL);

	// Initialisation de SDL TTF
	TTF_Init();

	this->screen = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		LARGEUR_FENETRE, HAUTEUR_FENETRE, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (this->screen == 0)
	{
		std::cout << "Erreur lors de la recuperation de la vue : " << SDL_GetError() << std::endl;
		SDL_Quit();

		exit(EXIT_FAILURE);
	}

	// Création du contexte OpenGL

	this->contexteOpenGL = SDL_GL_CreateContext(this->screen);

	if (this->contexteOpenGL == 0)
	{
		std::cout << SDL_GetError() << std::endl;
		SDL_DestroyWindow(this->screen);
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	//Sow windows YES NO
	this->playWithCamera = true;

	this->renderer = SDL_CreateRenderer(this->screen, -1, 0);
 
    // Titre de l'application
    //Correctif SDL 2.0 SDL_WM_SetCaption(TITRE_APPLICATION, NULL);
	SDL_SetWindowTitle(screen, TITRE_APPLICATION);

}