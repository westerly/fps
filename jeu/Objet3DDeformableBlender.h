#ifndef OBJET3DDEFORMABLEBLENDER_H_INCLUDED
#define OBJET3DDEFORMABLEBLENDER_H_INCLUDED

#include "Objet3DDeformable.h"
#include "objloader.h"

class Objet3DDeformableBlender : public Objet3DDeformable
{
	private:
		GLuint listeAffichage;

		objloader * loader;

	public:
		Objet3DDeformableBlender(std::string filenameObj,float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical);
		//~Objet3DDeformableBlender(void);

		void dessiner();
};

#endif 