#include "scene.h"


Scene::Scene(SDL_Window *screen, SDL_Renderer *renderer, SDL_GLContext contexteOpenGL, bool playWithCamera)
{
	this->playWithCamera = playWithCamera;
	this->screen = screen;
	this->renderer = renderer;
	this->contexteOpenGL = contexteOpenGL;
	this->currentFPS = 0;

    this->skybox = new Objet3DStatique("skybox.o3s.m3s");
    this->carte = new Carte("carte.bmp",0.0,0.0,-1.0);
    this->personnage = new Personnage(8, 3, 0, 0, 0, 1);

	//this->table = new Objet3DDeformableBlender("D:\Modeles 3D\Colt_anaconda\colt.obj",3,3,4,0,0);

	this->physicHandler = new PhysicEngine();

	this->animationHandler = new animator();
	this->animationHandler->armePersonnage = this->personnage->gun;


	this->controleur = new Controleur(this->carte, this->physicHandler,this->personnage);
	this->eventHandler = new GameEventHandler();

	this->controleur->setLinkWithHandlerEvent(this->eventHandler);
	this->eventHandler->setLinkWithControleur(this->controleur);
	
	// Le gestionnaire d'évènements écoute l'objet personnage
	this->eventHandler->hookEvent(this->personnage);
	
	// Ajouter le sol au monde physique
	this->physicHandler->addRigidBody(this->carte->getBody_Sol());

	// Ajout des rigides body pour les murs dans le monde physique
	this->physicHandler->addRigidBodies(this->carte->getRigidBodiesWalls());

	if (this->playWithCamera){
		this->fireElementInside = false;
		this->capture = cvCaptureFromCAM(0);
		this->lastRedBallX = HAUTEUR_FENETRE / 2;
		this->lastRedBallY = HAUTEUR_FENETRE / 2;
	}

}

Scene::~Scene()
{
    delete this->skybox;
	delete this->controleur;
	delete this->personnage;
	delete this->eventHandler;
	delete this->animationHandler;
	delete this->physicHandler;
	cvReleaseCapture(&capture);
}

void Scene::executer()
{	
	this->verrouillerSouris();

	// Parametrage d'OpenGL
    this->initOpenGL();

	// Permet de pouvoir générer des nombres aléatoires par la suite
	srand(time(NULL));


    // Initialisation des variables de gestion du temps
    this->tempsDernierPas = 0;
    uint32 heureDernierPas = SDL_GetTicks();

    this->continuer = TRUE;

	// Variable pour compter le nombre de fps
	int framesPerSecond = 0;
	int lastFramePerSecond = 0;
	uint32 cptMilliSeconde = 0;

	// Initialisation du jeu par le controleur, création des premières cibles...
	this->controleur->startGame();

    while(this->continuer && this->controleur->persoIsAlive())
    {
		if (this->playWithCamera){
			IplImage*frame = cvQueryFrame(capture);
			if (!frame) break;
			frame = cvCloneImage(frame);

			cvSmooth(frame, frame, CV_GAUSSIAN, 3, 3); //smooth the original image using Gaussian kernel

			IplImage* imgHSV1 = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
			cvCvtColor(frame, imgHSV1, CV_BGR2HSV); //Change the color format from BGR to HSV

			IplImage* imgHSV2 = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
			cvCvtColor(frame, imgHSV2, CV_BGR2HSV); //Change the color format from BGR to HSV

			// Image pour la forme rouge
			IplImage* imgThreshRedForm = GetThresholdedImage(imgHSV1, cvScalar(152, 120, 46), cvScalar(180, 168, 256));
			IplImage* imgThreshGreenForm = GetThresholdedImage(imgHSV2, cvScalar(80, 105, 92), cvScalar(100, 209, 200));

			cvSmooth(imgThreshRedForm, imgThreshRedForm, CV_GAUSSIAN, 3, 3); //smooth the binary image using Gaussian kernel
			cvSmooth(imgThreshGreenForm, imgThreshGreenForm, CV_GAUSSIAN, 3, 3); //smooth the binary image using Gaussian kernel

			//track the possition of the ball
			this->handleGunWithCamera(imgThreshRedForm);
			this->handleFireWithCamera(imgThreshGreenForm);

			//Clean up used images
			cvReleaseImage(&imgHSV1);
			cvReleaseImage(&imgHSV2);
			cvReleaseImage(&imgThreshRedForm);
			cvReleaseImage(&imgThreshGreenForm);
			cvReleaseImage(&frame);
		}

        gererEvenements();
        animer();
		

        // On calcule le temps de construction de la derniere image (en millisecondes)
        this->tempsDernierPas = SDL_GetTicks() - heureDernierPas;
        heureDernierPas += this->tempsDernierPas;

		// On compte le nombre de frames par seconde
		cptMilliSeconde = cptMilliSeconde + this->tempsDernierPas;
		framesPerSecond++;
		if (cptMilliSeconde >= 1000){
			this->currentFPS = framesPerSecond;
			framesPerSecond = 0;
			cptMilliSeconde = 0;
		}

		dessiner();
		afficher();

    }

	this->deverrouillerSouris();
}

//This function threshold the HSV image and create a binary image
IplImage* Scene::GetThresholdedImage(IplImage* imgHSV, CvScalar lowValues, CvScalar highValue ){
	IplImage* imgThresh = cvCreateImage(cvGetSize(imgHSV), IPL_DEPTH_8U, 1);
	cvInRangeS(imgHSV, lowValues, highValue, imgThresh);
	return imgThresh;
}

void Scene::handleGunWithCamera(IplImage* imgThreshRedForm){
	// Calculate the moments of 'imgThresh'
	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
	cvMoments(imgThreshRedForm, moments, 1);
	double moment10 = cvGetSpatialMoment(moments, 1, 0);
	double moment01 = cvGetSpatialMoment(moments, 0, 1);
	double area = cvGetCentralMoment(moments, 0, 0);

	// if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
	if (area>1000){
		// calculate the position of the ball
		float16 posX = moment10 / area;
		float16 posY = moment01 / area;

		if (this->lastRedBallX >= 0 && this->lastRedBallY >= 0 && posX >= 0 && posY >= 0)
		{
			// Draw a yellow line from the previous point to the current point
			int ecartX = posX - this->lastRedBallX;
			int ecartY = posY - this->lastRedBallY;
			
			this->personnage->angleHorizontal += ecartX * 0.5;
			this->personnage->angleVertical += -ecartY * 0.5;
		}

		this->lastRedBallX = posX;
		this->lastRedBallY = posY;
	}

	free(moments);
}

void Scene::handleFireWithCamera(IplImage* imgThreshGreenForm){

	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
	cvMoments(imgThreshGreenForm, moments, 1);
	double moment10 = cvGetSpatialMoment(moments, 1, 0);
	double moment01 = cvGetSpatialMoment(moments, 0, 1);
	double area = cvGetCentralMoment(moments, 0, 0);

	// if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
	if (area < 1000 && this->fireElementInside == true){
		this->animationHandler->beginAnimationArme();
		this->controleur->shootBullet(this->personnage->getX() - cos(-this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES),
			this->personnage->getY() + sin(-this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES),
			HAUTEUR_OEIL_PERSONNAGE + tan(this->personnage->getAngleVertical() * RADIANS_PAR_DEGRES), this->personnage->getAngleHorizontal(), this->personnage->getAngleVertical());
		this->fireElementInside = false;
	}

	if (area < 1000){
		this->fireElementInside = false;
	}
	else{
		this->fireElementInside = true;
	}

	free(moments);
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

	if (touches[SDL_SCANCODE_Q]) // Touche A (Déplacement rapide)
	{
		// On double la vitesse du personnage si la touche est enfoncée
		this->personnage->setVitesse(VITESSE_DEPLACEMENT_PERSONNAGE * 2);
	}
	else{
		// On remet la vitesse du personnage à la vitesse normal si la touche n'est pas enfoncée
		this->personnage->setVitesse(VITESSE_DEPLACEMENT_PERSONNAGE);
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
        float16 distance = (float)tempsDernierPas * this->personnage->getVitesse() / 1000.0f;
        // Recuperation de l'environnement
        sint32 positionCarteX = 0, positionCarteY = 0;
        bool8 entouragePersonnage[8];
        this->personnage->positionSurLaCarte(&positionCarteX, &positionCarteY);
        this->carte->entourage(positionCarteY, positionCarteX, entouragePersonnage);
        // Deplacement du personnage dans la direction demande
        this->personnage->deplacer(distance, direction, entouragePersonnage);
    }
	
	this->animationHandler->animer();
}

void Scene::dessiner(void)
{
    // Vidage de l'écran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place la camera
    glMatrixMode( GL_MODELVIEW );
	
	//gluLookAt(0, 0, -10, 0, 0, 3, 0, 1, 0);
    this->personnage->regarder();

    // Dessin de la skybox
    this->skybox->dessiner();
	
    // Dessin de la carte
    this->carte->dessiner();

	this->controleur->drawTargets();

	this->physicHandler->getWorld()->debugDrawWorld();

	//Dessin du personnage
	this->personnage->dessiner();

	this->drawFPS();

	this->controleur->dessinerTextePoints();
	this->controleur->dessinerVie();
	
}

void Scene::afficher(void)
{
	// glFlush — force execution of GL commands in finite time
    glFlush();
	// Use this function to update a window with OpenGL rendering.
	SDL_GL_SwapWindow(this->screen);
    // SDL 2.0 SDL_GL_SwapBuffers();
}

void Scene::gererEvenements(void)
{

	// Update des différents timers des cibles
	this->controleur->updateTimersTargets();
	// Gérer les cibles
	this->controleur->handleTargets();

    SDL_Event evenement;
    while (SDL_PollEvent(&evenement))
    {
        switch(evenement.type)
        {
            case SDL_QUIT:
                this->continuer = FALSE;
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

void Scene::drawFPS(){
	std::string text;
	// créer un flux de sortie
	std::ostringstream oss;
	// écrire un nombre dans le flux
	oss << this->currentFPS;
	// récupérer une chaîne de caractères
	std::string nbrFPS = oss.str();
	text = "FPS: " + nbrFPS;
	glColor3f(0, 0, 255);
	this->personnage->drawTextInFrontOfCharacter(text.data(), text.size(), 0, HAUTEUR_FENETRE - 10);
}


void Scene::verrouillerSouris(void)
{
	// Maintien de la souris dans la fenetre
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// La souris est au centre de l'ecran
	SDL_WarpMouseInWindow(this->screen, (LARGEUR_FENETRE / 2), (HAUTEUR_FENETRE / 2));

	// La souris est invisible
	SDL_ShowCursor(FALSE);
}

void Scene::deverrouillerSouris(void)
{
	// Deverrouillage de la souris
	SDL_SetRelativeMouseMode(SDL_FALSE);

	// La souris est visible
	SDL_ShowCursor(TRUE);
}

