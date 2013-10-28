#include "Objet3DDeformableBlender.h"


Objet3DDeformableBlender::Objet3DDeformableBlender(std::string filenameObj,float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical): Objet3DDeformable(positionX,positionY,positionZ,angleHorizontal,angleVertical)
{
	//objloader obj;  //create an instance of the objloader
	this->loader = new objloader();
	this->listeAffichage=this->loader->load(filenameObj.c_str());
}


void Objet3DDeformableBlender::dessiner(){

	//glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
	// On mémorise le repère courant avant d'effectuer la RST
    glPushMatrix();

		glTranslatef(this->positionX,this->positionY,this->positionZ);
		glRotatef(angleHorizontal, 0.0, 0.0, 1.0);
		glRotatef(angleVertical, 0.0, 1.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);

		glCallList(this->listeAffichage); 

	// Restoration du repère d'origine
    glPopMatrix();

	glDisable(GL_LIGHTING);
}