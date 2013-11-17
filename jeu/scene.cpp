#include "scene.h"

Scene::Scene(SDL_Window *screen, SDL_Renderer *renderer, SDL_GLContext contexteOpenGL)
{
	this->screen = screen;
	this->renderer = renderer;
	this->contexteOpenGL = contexteOpenGL;

    // Maintien de la souris dans la fenetre
	SDL_SetRelativeMouseMode(SDL_TRUE);
    // Correctif SDL 2.0 SDL_WM_GrabInput(SDL_GRAB_ON);

    // La souris est au centre de l'ecran
    SDL_WarpMouseInWindow(this->screen, (LARGEUR_FENETRE/2), (HAUTEUR_FENETRE/2) );

    // La souris est invisible
    SDL_ShowCursor(FALSE);

    this->skybox = new Objet3DStatique("skybox.o3s.m3s");
    this->carte = new Carte("carte.bmp");
    this->personnage = new Personnage(8, 3, 0, 0, 0);

	//this->table = new Objet3DDeformableBlender("D:\Modeles 3D\Colt_anaconda\colt.obj",3,3,4,0,0);

	this->physicHandler = new physicEngine();

	this->animationHandler = new animator();
	this->animationHandler->armePersonnage = this->personnage->gun;


	this->controleur = new Controleur();
	this->eventHandler = new GameEventHandler();

	this->controleur->setLinkWithHandlerEvent(this->eventHandler);
	this->controleur->setLinkWithMap(this->carte);
	this->eventHandler->setLinkWithControleur(this->controleur);
	
	// Le gestionnaire d'évènements écoute l'objet personnage
	this->eventHandler->hookEvent(this->personnage);

	this->targetTest = new target(4, 4, 0, 1, 1, 3);

	// Ajouter les objets composants la cible au monde physique
	this->physicHandler->addRigidBody(this->targetTest);
	// Ajouter le sol au monde physique
	this->physicHandler->addRigidBody(this->carte->getBody_Sol());

}

Scene::~Scene()
{
    delete this->skybox;
	delete this->controleur;
	delete this->personnage;
	delete this->eventHandler;
	delete this->animationHandler;
	//delete this->table;
	delete this->targetTest;
	delete this->physicHandler;
}

void Scene::executer()
{	
	// Parametrage d'OpenGL
    this->initOpenGL();


    // Initialisation des variables de gestion du temps
    this->tempsDernierPas = 0;
    uint32 heureDernierPas = SDL_GetTicks();

    this->continuer = TRUE;

	// Variable pour compter le nombre de fps
	int framesPerSecond = 0;
	int lastFramePerSecond = 0;
	uint32 cptMilliSeconde = 0;

    while(this->continuer)
    {
        gererEvenements();
        animer();
        dessiner();
        afficher();

        // On calcule le temps de construction de la derniere image
        this->tempsDernierPas = SDL_GetTicks() - heureDernierPas;
        heureDernierPas += this->tempsDernierPas;

		// On compte le nombre de frames par seconde et on l'affiche
		
		cptMilliSeconde = cptMilliSeconde + this->tempsDernierPas;
		framesPerSecond++;
		if (cptMilliSeconde >= 1000){
			//printf("\nCurrent Frames Per Second: %d\n\n", (int)framesPerSecond);
			lastFramePerSecond = framesPerSecond;
			framesPerSecond = 0;
			cptMilliSeconde = 0;
		}
		dessinerFPS(lastFramePerSecond);

    }
}

void Scene::dessinerFPS(int fps){
	if (TTF_Init() == -1)
	{
	    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
	}
	else{
		/* Chargement de la police */
		this->police = TTF_OpenFont("polices/arial.ttf", 65);
		SDL_Color couleurNoire = { 0, 0, 0 };
		/* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
		SDL_Surface *surface = TTF_RenderText_Blended(police, "Salut les Zér0s !", couleurNoire);
		this->texte = SDL_CreateTextureFromSurface(this->renderer, surface);
		SDL_FreeSurface(surface);
		SDL_Rect position;
		position.x = 60;
		position.y = 370;
		// SDL 2.0 SDL_BlitSurface(texte, NULL, this->screen, &position); /* Blit du texte */
		// Flip(this->ecran);
		//SDL_RenderClear(this->renderer);
		SDL_RenderCopy(this->renderer, this->texte, NULL, &position);
		SDL_RenderPresent(this->renderer);
	}

}


void Scene::animer(void)
{
    // Lecture de l'état des touches
    int nombreDeTouches;
	const Uint8* pTouches = SDL_GetKeyboardState(&nombreDeTouches);
    Uint8* touches = new Uint8[nombreDeTouches];
    memcpy(touches, pTouches, nombreDeTouches);

    // Evite les deplacements absurdes
	if (touches[SDL_SCANCODE_W] == touches[SDL_SCANCODE_S]) // Avancer et reculer
    {
		touches[SDL_SCANCODE_W] = 0u; // Ici, on travaille sur
		touches[SDL_SCANCODE_S] = 0u; // une copie des touches
    }
	if (touches[SDL_SCANCODE_A] == touches[SDL_SCANCODE_D]) // Gauche et droite
    {
		touches[SDL_SCANCODE_A] = 0u;
		touches[SDL_SCANCODE_D] = 0u;
    }

    float16 direction;
    bool8 deplacement = FALSE;

	if (touches[SDL_SCANCODE_W]) // Touche Z
    {
        // Avancer

        deplacement = TRUE;

        // En diagonale
		if (touches[SDL_SCANCODE_A]) direction = 45.0;
		else if (touches[SDL_SCANCODE_D]) direction = -45.0;

        // Droit
        else direction = 0.0;
    }
	else if (touches[SDL_SCANCODE_S]) // Touche S
    {
        // Reculer

        deplacement = TRUE;

        // En diagonale
		if (touches[SDL_SCANCODE_A]) direction = 135.0;
		else if (touches[SDL_SCANCODE_D]) direction = -135.0;

        // Droit
        else direction = 180.0;
    }

    if(FALSE == deplacement)
    {
		if (touches[SDL_SCANCODE_A]) // Touche Q
        {
            // Gauche
            direction = 90.0;
            deplacement = TRUE;
        }
		else if (touches[SDL_SCANCODE_D]) // Touche D
        {
            // Droite
            direction = -90.0;
            deplacement = TRUE;
        }
    }

    delete[] touches;

	// Update dynamics
	if (this->physicHandler->getWorld())
	{
		this->physicHandler->getWorld()->stepSimulation(this->tempsDernierPas);
	}

    // Si un deplacement est demande
    if (deplacement)
    {
        // Calcule de la distance à parcourir
        float16 distance = (float)tempsDernierPas * VITESSE_DEPLACEMENT_PERSONNAGE / 1000.0f;

        // Recuperation de l'environnement
        sint32 positionCarteX = 0, positionCarteY = 0;
        bool8 entouragePersonnage[8];
        this->personnage->positionSurLaCarte(&positionCarteX, &positionCarteY);
        this->carte->entourage(positionCarteY, positionCarteX, entouragePersonnage);

        // Deplacement du personnage dans la direction demande
        this->personnage->deplacer(distance, direction, entouragePersonnage);
    }

	// Si il y a des balles de tirées 
	if(this->controleur->thereAreBullets())
	{
		// Calcule de la distance à parcourir
        float16 distance = (float)tempsDernierPas * VITESSE_DEPLACEMENT_BULLET / 1000.0f;
		this->controleur->handleBullets(distance);
	}
	
	this->animationHandler->animer();
}

void Scene::dessiner(void)
{
    // Vidage de l'écran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place la camera
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	
	//gluLookAt(0,0,3,0,0,1,-1,0,0);
    this->personnage->regarder();

	//this->table->dessiner();

    // Dessin de la skybox
    this->skybox->dessiner();

	// Dessin des balles
	this->controleur->drawBullets();
	
    // Dessin de la carte
    this->carte->dessiner();

	// Dessin de la cible test
	this->targetTest->dessiner();

	//Dessin du personnage
    this->personnage->dessiner();


	// Dessin des animations
	
}

void Scene::afficher(void)
{
    glFlush();
	SDL_GL_SwapWindow(this->screen);
    // SDL 2.0 SDL_GL_SwapBuffers();
}

void Scene::gererEvenements(void)
{
    SDL_Event evenement;
    while (SDL_PollEvent(&evenement))
    {
        switch(evenement.type)
        {
            case SDL_QUIT:
                this->continuer = FALSE;

				// Fermeture de la police
				TTF_CloseFont(this->police);
				// Fermeture de la librairie ttf
				TTF_Quit();
                break;

            case SDL_MOUSEMOTION:
                // Si le mouvement de la souris est physique (pas par SDL_WarpMouse)
                if ((LARGEUR_FENETRE/2) != evenement.motion.x || (HAUTEUR_FENETRE/2) != evenement.motion.y)
                {
                    // Fait tourner le personnage
                    this->personnage->tournerHorizontalement(-evenement.motion.xrel * 0.06);
                    this->personnage->tournerVerticalement(-evenement.motion.yrel * 0.06);

                    // Replace la souris au milieu de l'ecran
                    SDL_WarpMouseInWindow(this->screen, (LARGEUR_FENETRE/2), (HAUTEUR_FENETRE/2) );
                }
                break;

			/* Enfoncement bouton souris */
			case SDL_MOUSEBUTTONDOWN:

				if (evenement.button.button == SDL_BUTTON_LEFT)
                {
					
					this->animationHandler->beginAnimationArme();
					// L'objet animator se chargera de l'animation des objets
					// Il devra agréger des poiteurs sur les objets qu'il peut animer et leur attribuera une texture courante à dessiner suivant 
					// le temps entre chaque image de l'animation

					__raise this->personnage->tirer(this->personnage->getX() - cos(-this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES),
						this->personnage->getY() + sin(-this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES),
						HAUTEUR_OEIL_PERSONNAGE + tan(this->personnage->getAngleVertical() * RADIANS_PAR_DEGRES),this->personnage->getAngleHorizontal(),this->personnage->getAngleVertical());
                }

			break;

            case SDL_KEYDOWN:
                // Permet de quitter grace a la touche Echap
                if (evenement.key.keysym.sym == SDLK_ESCAPE)
                {
                    this->continuer = FALSE;
                }
                break;
        }
    }
}

void Scene::initOpenGL(void)
{
    //glEnable( GL_CULL_FACE ); // Activer le masquage des faces cachées
    glCullFace(GL_BACK); // Face cachées = faces arrières
    glFrontFace(GL_CCW); // Face avant = sens trigo

    // Couleur rgba de vidage de l'écran
    glClearColor(0, 0, 0, 0);

    // Définition de la fenetre
    glViewport(0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE);

    // Définition de la zone visible
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(ANGLE_VISION, (GLdouble) LARGEUR_FENETRE / (GLdouble)HAUTEUR_FENETRE, PRET, LOIN);

    // Activation du tampon de profondeur
    glEnable(GL_DEPTH_TEST);
}

