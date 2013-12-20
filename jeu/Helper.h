#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <time.h>
#include <algorithm>
#include <string>
#include <SDL_ttf.h>
#include "conteneurTextures.h"
#include "configuration.h"

static class Helper
{
	public:

		// On suppose a<b, Permet d'obtenir un nombre entier random compris entre [a;b[ 
		static inline int rand_a_b(int a, int b){ return rand() % (b - a) + a; }
		// On suppose a<b, Permet d'obtenir un nombre floatant random compris entre [a;b[ 
		static inline double frand_a_b(double a, double b){ return (rand() / (double)RAND_MAX) * (b - a) + a; }

		// retourne la somme des n premiers nombres naturels
		static inline int sum_n(int n){
			if (n == 0){
				return 0;
			}
			else{
				return n + sum_n(n-1);
			}

		}

		// Dessine du texte à la position x,y pour une taille donnée, l'id de la texture générée sera enregistré dans le dernier paramètre
		static void dessinerTexte(Texture &texture, sint32 pX, sint32 pY);
		static void creerTextureText(std::string texte, int size, SDL_Color couleur, Texture &texture);
	};

#endif

