#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED


#include "types.h"
#include <string>
#include <vector>
#include <set>

#include "conteneurTextures.h"

class Bullet
{

	private:

		typedef std::set<std::string> Textures;

        Textures textures; // Les textures de l'objet
        ConteneurTextures conteneurTextures;

		float16 positionX;
		float16 positionY;
		float16 positionZ;
		float16 rayon;
		float16 angleHorizontal;
		float16 angleVertical;

		bool inColision;

	public:
		Bullet(std::vector<std::string> & fichiersTextureAnimationDestructionBullet, float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical);
		~Bullet(void);
		void ajouterTexture(std::string nomFichierTexture);
		void positionSurLaCarte(sint32* x, sint32* y, sint32* z);
		void deplacer(float16 & distance,bool8 entourage[8]);

		__event void collision(Bullet * balle);

		float16 getX(){return this->positionX;}
		float16 getY(){return this->positionY;}
		float16 getZ(){return this->positionZ;}

		bool isInColision(){return this->inColision;}

		void handleColisionWithWall(float16 & positionCibleX, float16 & positionCibleY,bool8 * entourage);

		void dessiner();
};


#endif // BULLET_H_INCLUDED

