#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
 
#include <string>
#include "conteneurTextures.h"
#include "configuration.h"
#include "SDL.h"
 
class Menu
{
    public:
        Menu(std::string imageFond);
        ~Menu();
        void dessiner(void);
 
    private:
        std::string imageFond;
        ConteneurTextures conteneurTextures;

		void dessinerFond(void);
        void dessinerBoutons(void);
};
 
#endif // MENU_H_INCLUDED