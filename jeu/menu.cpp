#include "menu.h"
#include "conteneurTextures.h"
 
Menu::Menu(std::string imageFond, SDL_Window *scr)
{
    this->imageFond = imageFond;
    this->conteneurTextures.ajouter(this->imageFond);
	this->screen = scr;
}
 
Menu::~Menu()
{
    this->conteneurTextures.supprimer(this->imageFond);

	// Liberation des boutons
	for (Boutons::iterator element = this->listeBoutons.begin(); element != listeBoutons.end(); element++)
	{
		delete *element;
	}
}
 
void Menu::dessiner(void)
{
    // En 2D
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
 
    // Definition de la fenetre
    gluOrtho2D(0.0, (GLdouble)LARGEUR_FENETRE, 0.0, (GLdouble)HAUTEUR_FENETRE);
 
    // Desactivation du test de prophondeur
    glDisable(GL_DEPTH_TEST);
 
    // Vidage de l'image
    glClear(GL_COLOR_BUFFER_BIT);

	// Activation des textures
    glEnable(GL_TEXTURE_2D);
 
    // Dessin de fond du menu
    this->dessinerFond();
 
    // Dessin des boutons
    this->dessinerBoutons();
 
    // Affichage du dessin
    glFlush();
	// appeler SDL_GL_SwapWindow(this->screen); faire passer screen en attribut de menu
    // Changement SDL 2.0 SDL_GL_SwapBuffers();
	SDL_GL_SwapWindow(this->screen);
 
}
 
void Menu::dessinerFond(void)
{


    // Selection de l'image de fond
    glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture(this->imageFond).texture);

	GLuint test = this->conteneurTextures.texture(this->imageFond).texture;
 
    // Application de l'image de fond
    glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex2f(0, HAUTEUR_FENETRE);
		glTexCoord2d(0, 1); glVertex2f(0, 0);
		glTexCoord2d(1, 1); glVertex2f(LARGEUR_FENETRE, 0);
		glTexCoord2d(1, 0); glVertex2f(LARGEUR_FENETRE, HAUTEUR_FENETRE);
    glEnd();
}
 
void Menu::dessinerBoutons(void)
{
	// Dessin des boutons
	for (Boutons::iterator element = this->listeBoutons.begin(); element != listeBoutons.end(); element++)
	{
		(*element)->dessiner();
	}
}

void Menu::ajouterBouton(std::string texte, sint32 x, sint32 y, sint32 code)
{
	this->listeBoutons.insert(new Bouton(texte, x, y, code));
}