#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include "configuration.h"
#include "SDL.h"

#include < GL/glut.h >

// Classe mère du jeu.
class Jeu
{
    public:
        Jeu();
        ~Jeu();
		
        void executer(void);


	private:
        void initSDL(void);
};
 
#endif // JEU_H_INCLUDED