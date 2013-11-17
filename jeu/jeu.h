#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include "configuration.h"
#include "SDL.h"

#include < glut.h >

// Classe mère du jeu.
class Jeu
{

	private:
		SDL_Window *screen;
		SDL_Renderer *renderer;
		SDL_GLContext  contexteOpenGL;
    public:
        Jeu();
        ~Jeu();
		
        void executer(void);


	private:
        void initSDL(void);
};
 
#endif // JEU_H_INCLUDED