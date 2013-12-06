#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include "Objet3DDeformable.h"
#include "arme.h"
#include "configuration.h"


[event_source(native)]
class Personnage : public Objet3DDeformable
{
    private:

        float16 rayon;
		// Vitesse de déplacement du personnage
		float vitesse;
	

    public:

		// Pour l'instant qu'une arme pour le perso et c'est un gun
		arme * gun;


		Personnage(float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical, btScalar mass = 0.0f);

        // Déplacement
        void deplacer(float16 distance, float16 direction, bool8 entourage[8]);
        void tournerHorizontalement(float16 angle);
        void tournerVerticalement(float16 angle);

        void positionSurLaCarte(sint32* x, sint32* y);
        void regarder(void);

		void dessiner();
		void drawTextInFrontOfCharacter(const char *text, int length, int x, int y);
		__event void tirer(float16 positionX,float16 positionY,float16 positionZ,float16 angleHorizontal,float16 angleVertical);

		inline float getVitesse(){ return this->vitesse; }
		inline void setVitesse(float v){ this->vitesse = v; }

};

#endif // PERSONNAGE_H_INCLUDED
