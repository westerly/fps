#include <string>
#include <set>
#include "types.h"

#include "conteneurTextures.h"


class Objet3DDeformable
{
	protected:

		typedef std::set<std::string> Textures;

        Textures textures; // Les textures de l'objet
        ConteneurTextures conteneurTextures;


        // Position de l'objet
        float16 positionX;
        float16 positionY;
        float16 positionZ;


        // Angle horizontal avec l'axe des x (pour tourner sur sois même)
        float16 angleHorizontal;
		

		// Angle vertical avec l'axe des z (pour regarder en haut en bas)
		float16 angleVertical;

        std::string nom;

	public:
		Objet3DDeformable(float16 positionX = 0.0f, float16 positionY = 0.0f, float16 positionZ = 0.0f, float16 angleHorizontal = 0.0f, float16 angleVertical = 0.0f);
		// Le mot clé virtual devant le destructeur permet d'être sure qu'il sera appelé 
		// lors de la destruction d'un objet d'une classe fille et même si on ne l'appel pas explicitement
		virtual ~Objet3DDeformable(void);
        virtual void dessiner();
		void ajouterTexture(std::string nomFichierTexture);

		float16 getX(){return this->positionX;}
		float16 getY(){return this->positionY;}
		float16 getZ(){return this->positionZ;}
		float16 getAngleHorizontal(){return this->angleHorizontal;}
		float16 getAngleVertical(){return this->angleVertical;}

};

