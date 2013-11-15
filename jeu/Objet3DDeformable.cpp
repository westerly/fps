#include "Objet3DDeformable.h"


Objet3DDeformable::Objet3DDeformable(float16 positionX, float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical, btScalar mass)
{
	 // Initialisation des attributs
    this->positionX = positionX;
    this->positionY = positionY;
    this->positionZ = positionZ;
    this->angleHorizontal = angleHorizontal;
	this->angleVertical = angleVertical;
	//this->localInertia = localInertia;
	this->mass = mass;

}


Objet3DDeformable::~Objet3DDeformable(void)
{
	// Libération des textures
    for (Textures::iterator element = this->textures.begin(); element != textures.end(); element++)
    {
        this->conteneurTextures.supprimer(*element);
    }
}

          
// Ajoute et charge une texture à partir d'un fichier
// La texture est aussi ajouté aux textures de l'objet 
void Objet3DDeformable::ajouterTexture(std::string nomFichierTexture)
{
	// Si la texture n'est pas deja creee
    if(this->textures.find(nomFichierTexture) == this->textures.end())
    {
        this->conteneurTextures.ajouter(nomFichierTexture);
        this->textures.insert(nomFichierTexture);
    }
}


// Méthode virtuelle à redéfinir dans les classes filles
void Objet3DDeformable::dessiner()
{
   
}

btRigidBody * Objet3DDeformable::getBody(){
	return this->body;
}

