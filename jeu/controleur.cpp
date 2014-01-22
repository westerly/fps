#include "controleur.h"
#include "gameEventHandler.h"


Controleur::Controleur(Carte * p_carte, PhysicEngine * p_physicEngine, Personnage * p_personnage) :carte(p_carte), physicEngine(p_physicEngine), personnage(p_personnage){
	// Définition du temps courant de vie d'une cible avec le paramètre correspondant au temps max de vie d'une cible au début du jeu
	this->current_max_living_time_target = MAX_LIVING_TIME_TARGET_BEGINNING;
	// Le joueur commence avec 0 point
	this->nbrPoints = 0;
	this->textureTextePoints.texture = NULL;
	// Le joueur a trois vies au début du jeu
	this->nbrVies = 3;

	this->conteneurTextures.ajouter("coeur.png");
}

Controleur::~Controleur(void)
{
	// Libération des textures
	for (Textures::iterator element = this->textures.begin(); element != textures.end(); element++)
	{
		this->conteneurTextures.supprimer(*element);
	}

	// Destruction des targets
	for (std::vector<target*>::iterator it = this->targets.begin(); it != this->targets.end(); it++){
		delete *it;
	}

	// Libération de la texture des textes
	glDeleteTextures(1, &this->textureTextePoints.texture);
}


void Controleur::shootBullet(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical)
{	
	// Calcul du point 2 pour construire le vecteur direction (utilisé pour le ray casting)
	float16 positionCibleX2 = this->personnage->getX() - 200 * cos(-(this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES));
	float16 positionCibleY2 = this->personnage->getY() + 200 * sin(-(this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES));
	float16 positionCibleZ2 = HAUTEUR_OEIL_PERSONNAGE + tan(this->personnage->getAngleVertical() * RADIANS_PAR_DEGRES) * 200;
	
	// Construction du vecteur force de base à appliquer au point de contact entre la boite et le ray 
	btVector3 vecteurDirection((this->personnage->getX() - 2 * cos(-(this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES))) - positionX,
		(this->personnage->getY() + 2 * sin(-(this->personnage->getAngleHorizontal() * RADIANS_PAR_DEGRES))) - positionY,
		(HAUTEUR_OEIL_PERSONNAGE + tan(this->personnage->getAngleVertical() * RADIANS_PAR_DEGRES) * 2) - positionZ);

	// Start est le point de départ du ray et End est le point d'arrivé (calculé à 200 unités du personnage)
	btVector3 Start(positionX, positionY, positionZ);
	btVector3 End(positionCibleX2, positionCibleY2, positionCibleZ2);
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start,End);

	// Perform raycast
	this->physicEngine->getWorld()->rayTest(Start, End, RayCallback);

	// Si le ray a touché quelque chose
	if (RayCallback.hasHit()) {
		End = RayCallback.m_hitPointWorld;
		btVector3 Normal = RayCallback.m_hitNormalWorld;

		const btCollisionObject * objectShooted = RayCallback.m_collisionObject;
		Objet3DDeformable* elTarget = static_cast<Objet3DDeformable*>(objectShooted->getUserPointer());
		box * boxInCollision = dynamic_cast<box*>(elTarget);
		bool elHighlightedShot = false;

		// Si une boxe est touché par le ray 
		if (boxInCollision != NULL){
			boxInCollision->getBody()->activate();
			// On applique une force au point d'impact entre le ray et la boite 
			boxInCollision->getBody()->applyImpulse(vecteurDirection * 5, RayCallback.m_hitNormalWorld);
			if (boxInCollision->isHighlighted()){
				elHighlightedShot = true;
			}
		}
		else{
			cylinder * cylinderInCollision = dynamic_cast<cylinder*>(elTarget);
			if (cylinderInCollision != NULL){
				cylinderInCollision->getBody()->activate();
				// On applique une force au point d'impact entre le ray et la boite 
				cylinderInCollision->getBody()->applyImpulse(vecteurDirection * 5, RayCallback.m_hitNormalWorld);
				//cylinderInCollision->setRGBA(255, 0.1, 0.1, 0.1);
				if (cylinderInCollision->isHighlighted()){
					elHighlightedShot = true;
				}
			}
		}

		if (elHighlightedShot){
			this->handleElHighlightedShot(elTarget);
		}
	}
}

void Controleur::handleElHighlightedShot(Objet3DDeformable * el){
	bool targetFound = false;
	target * t;
	std::vector<target *>::iterator it = this->targets.begin();
	while (!targetFound){
		t = *it;
		if (t->elIsPartOfTarget(el)){
			targetFound = true;
		}
		else{
			it++;
		}
	}

	t->setIsShot(true);

	// MAJ le nombre de points du joueur
	// Si la cible vient juste d'être shooté par le joueur alors l'attribut shotAt vaut toujours 0 et on peut MAJ le nombre de points du joueur
	// Ce test évite de MAJ plusieurs fois les points du joueur qui tirerait plusieurs fois sur l'élément en surbrillance de la cible
	if (t->getShotAt() == 0){
		this->majNbrPointsPlayer(t);
	}
	
}

void Controleur::startGame(){

	// Faire apparaitre entre 1 et un nombre paramétrable de cible à des positions random sur la carte
	int random_nbr_cibles = Helper::rand_a_b(NBR_CIBLES_MAX, NBR_CIBLES_MAX + 1);
	for (int i = 0; i < random_nbr_cibles; i++){
		createTarget();
	}
}

void Controleur::createTarget(){
	
	bool toClose = true;

	while (toClose){

		float random_obj_base = Helper::rand_a_b(3, NBR_EL_TARGET_BASE_MAX);

		int random_type_target = Helper::rand_a_b(0, 2);

		float random_hauteur_largeur = 0;
		// Target de type cylindre
		if (random_type_target == 0){
			random_hauteur_largeur = Helper::frand_a_b(MIN_LARGEUR_EL_TARGET_CYLINDRE, MAX_LARGEUR_EL_TARGET_CYLINDRE);
		}
		else{
			// Target de type box
			random_hauteur_largeur = Helper::frand_a_b(MIN_LARGEUR_EL_TARGET_BOX, MAX_LARGEUR_EL_TARGET_BOX);
		}

		float size_target = random_obj_base * random_hauteur_largeur + ((random_obj_base - 1) * ESPACEMENT_ENTRE_ELEMENTS * random_hauteur_largeur);

		float random_pos_x = Helper::frand_a_b(1 + size_target, this->carte->getHauteur() - (size_target + 2));
		float random_pos_y = Helper::frand_a_b(1 + size_target, this->carte->getLargeur() - (size_target + 2));

		toClose = targetToCloseFromXY(random_pos_x, random_pos_y, size_target);

		if (!toClose){
			target * tr = new target(random_pos_x, random_pos_y, 0, random_hauteur_largeur, random_hauteur_largeur, random_obj_base,random_type_target);
			this->targets.push_back(tr);
			// Ajouter les objets composants la cible au monde physique
			this->physicEngine->addRigidBody(tr);

			// initialisation de l'attribut timeStart de la cible
			tr->setTimeStart(SDL_GetTicks());
		}
	}

}


void Controleur::drawTargets(){
	for (std::vector<target*>::iterator it = this->targets.begin(); it != this->targets.end(); it++){
		target * tr = *it;
		tr->dessiner();
	}
}

void Controleur::updateTimersTargets(){
	// Update des temps de vie de toutes les cibles
	for (std::vector<target*>::iterator it = this->targets.begin(); it != this->targets.end(); it++){
		target * tr = *it;
		tr->setDisplayedSince(SDL_GetTicks() - tr->getTimeStart());

		// Si le bon élément en surbrillance de la cible a été touché
		if (tr->isShot() && tr->getShotAt() == 0){
			// On MAJ le timer servant à savoir depuis combien de temps la cible a été touchée
			tr->setShotAt(SDL_GetTicks());
		}
	}
}



void Controleur::handleTargets(){

	int nbr_targets_remove = 0;

	for (std::vector<target*>::iterator it = this->targets.begin(); it != this->targets.end();){
		target * tr = *it;
		// Si la cible a été affiché depuis un certain temps et que celle ci n'a pas encore été bien shooté on la fait disparaitre
		// Si la cible a été shooté correctement depuis un certain temps paramétrable on l'a fait disparaitre aussi
		if ( (tr->getDisplayedSince() >= this->current_max_living_time_target && !tr->isShot())
			|| (tr->getSinceIsShot() >= TIMES_BEFORE_TARGET_RIGHT_SHOT_DISAPPEARS && tr->isShot()) ){
			target * tr = *it;

			// si la cible n'a pas été correctement shooté, le joueur perd une vie
			if (!tr->isShot()){
				//this->nbrVies--;
			}

			// supression de la cible dans le vecteur de cibles
			it = this->targets.erase(it);
			// Suppression des rigidBody dans le monde physique et dans la mémoire
			deleteRigidBodiesFromTarget(tr);
			// suppression de la cible
			delete tr;
			nbr_targets_remove++;

		}
		else{
			++it;
		}
	}

	for (int i = 0; i < nbr_targets_remove; i++){
		createTarget();
	}
}



void Controleur::deleteRigidBodiesFromTarget(target *tr){
	std::vector<Objet3DDeformable*> * targetElements = tr->getTargetElements();
	for (std::vector<Objet3DDeformable*>::iterator itel = targetElements->begin(); itel != targetElements->end();itel++){
		Objet3DDeformable * el = *itel;
		this->physicEngine->getWorld()->removeRigidBody(el->getBody());
	}
}

bool Controleur::targetToCloseFromXY(float x, float y, float size){

	// Parcours du vecteur de cibles
	for (std::vector<target*>::iterator it = this->targets.begin(); it != this->targets.end(); it++){
		target * tr = *it;
		
		double dist = sqrt(pow((x + (size/2)) - tr->getPositionX(), 2) + pow((y + (size/2)) - tr->getPositionY(), 2));
		if (dist <= size){
			return true;
		}
	}

	return false;

}


void Controleur::majNbrPointsPlayer(target * t){
	
	float16 vX = t->getPositionX() - this->personnage->getX();
	float16 vY = t->getPositionY() - this->personnage->getY();
	float16 vZ = t->getPositionZ() - this->personnage->getZ();
	float16 distance_perso_target = sqrt(vX * vX + vY * vY + vZ * vZ);

	// On calcul le nombre de points en fonction de la distance par rapport à la cible et de la rapidité du personnage
	int points = distance_perso_target + (this->current_max_living_time_target - t->getDisplayedSince());

	// On affecte un coefficient au nombre de points calculé auparavant en fonction de la largeur des éléments de la cible
	this->nbrPoints = this->nbrPoints + (points * (MAX_LARGEUR_EL_TARGET_BOX + 1 - t->getLargeur()));

	// Suppression de la texture du texte pour afficher les points et création d'une nouvelle
	glDeleteTextures(1, &this->textureTextePoints.texture);
	this->creerTextureTextPoints();
}



void Controleur::creerTextureTextPoints(){
	std::string text;
	// créer un flux de sortie
	std::ostringstream oss;
	oss << nbrPoints;
	std::string nbrPoints = oss.str();
	text = nbrPoints + " points";
	SDL_Color couleurTexte = { 0, 0, 255 };
	Helper::creerTextureText(text, 35, couleurTexte, this->textureTextePoints);
}

void Controleur::dessinerTextePoints(){
	if (this->textureTextePoints.texture == NULL){
		// Si pas de texture encore créés pour le texte on en génère une avec la méthode creerTextureTextPoint
		this->creerTextureTextPoints();
	}

	Helper::dessinerTexte(this->textureTextePoints, LARGEUR_FENETRE - 80, 40);
}


void Controleur::dessinerVie(){

	// Activation de la transparence
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
	double matrix[16]; // 16 doubles in stack memory
	glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
	glLoadIdentity(); // reset PROJECTION matrix to identity matrix

	// Definition de la fenetre
	gluOrtho2D(0.0, (GLdouble)LARGEUR_FENETRE, 0.0, (GLdouble)HAUTEUR_FENETRE);

	glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIEW matrix again
	glLoadIdentity(); // reset it to identity matrix

	glPushMatrix();


	// Desactivation du test de prophondeur
	glDisable(GL_DEPTH_TEST);

	// Vidage de l'image
	//glClear(GL_COLOR_BUFFER_BIT);

	// Activation des textures
	glEnable(GL_TEXTURE_2D);

	// Selection de la texture du texte
	glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture("coeur.png").texture);

	sint32 hauteurTexture = this->conteneurTextures.texture("coeur.png").hauteur;
	sint32 largeurTexture = this->conteneurTextures.texture("coeur.png").largeur;


	for (int i = 0; i < this->nbrVies; i++){

		sint32 pX = 40 + i * (largeurTexture + largeurTexture / 4);
		sint32 pY = 40;

		// Application du texte
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(pX - (largeurTexture / 2),
			HAUTEUR_FENETRE - pY + (hauteurTexture / 2));
		glTexCoord2i(0, 1); glVertex2i(pX - (largeurTexture / 2),
			HAUTEUR_FENETRE - pY - (hauteurTexture / 2));
		glTexCoord2i(1, 1); glVertex2i(pX + (largeurTexture / 2),
			HAUTEUR_FENETRE - pY - (hauteurTexture / 2));
		glTexCoord2i(1, 0); glVertex2i(pX + (largeurTexture / 2),
			HAUTEUR_FENETRE - pY + (hauteurTexture / 2));
		glEnd();
	}


	

	glPopMatrix(); // get MODELVIEW matrix value from stack
	glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
	glLoadMatrixd(matrix); // reset
	glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW

	// Activation du tampon de profondeur
	glEnable(GL_DEPTH_TEST);
}