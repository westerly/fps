#include "init.h"
#include "configuration.h"
#include "SDL.h"

#include < GL/glut.h >


void initSDL(void)
{
    // D�marrage de la SDL avec le module vid�o
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Cr�ation de la fenetre initialis�e pour fonctionner avec OpenGL
    SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_OPENGL);

    // Titre de l'application
    SDL_WM_SetCaption(TITRE_APPLICATION, NULL);
}



void initOpenGL(void)
{
    //glEnable( GL_CULL_FACE ); // Activer le masquage des faces cach�es
    //glCullFace(GL_BACK); // Face cach�es = faces arri�res
    //glFrontFace(GL_CCW); // Face avant = sens trigo

    // Couleur rgba de vidage de l'�cran
    glClearColor(0, 0, 0, 0);

    // D�finition de la fenetre
    glViewport(0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE);

    // D�finition de la zone visible
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(ANGLE_VISION, (GLdouble) LARGEUR_FENETRE / (GLdouble)HAUTEUR_FENETRE, PRET, LOIN);

    // Activation du tampon de profondeur
    glEnable(GL_DEPTH_TEST);
}

