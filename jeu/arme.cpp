#include "arme.h"


arme::arme(std::string fichierTexture)
{
	this->fichierTextureCouranteDisplayed = fichierTexture;
	this->ajouterTexture(this->fichierTextureCouranteDisplayed);
}

// Constructeur qui charge à la fois le fichier  de texture représentant l'arme mais aussi les textures
// permettant d'afficher l'animation de cette arme
arme::arme(std::string fichierTexture, std::vector<std::string> & texturesAnimation)
{
	this->fichierTextureCouranteDisplayed = fichierTexture;
	this->ajouterTexture(this->fichierTextureCouranteDisplayed);
	 
	for(std::vector<std::string>::iterator texture = texturesAnimation.begin(); texture != texturesAnimation.end(); texture++)
	{
		this->ajouterTexture(*texture);
	}
}


arme::~arme(void)
{
	// Libération des textures
    for (Textures::iterator element = this->textures.begin(); element != textures.end(); element++)
    {
        this->conteneurTextures.supprimer(*element);
    }
}


// Ajoute et charge une texture à partir d'un fichier
// La texture est aussi ajouté aux textures de l'objet 
void arme::ajouterTexture(std::string nomFichierTexture)
{
	// Si la texture n'est pas deja creee
    if(this->textures.find(nomFichierTexture) == this->textures.end())
    {
        this->conteneurTextures.ajouter(nomFichierTexture);
        this->textures.insert(nomFichierTexture);
    }
}

void arme::dessiner(float16 positionX,float16 positionY, float16 positionZ, float16 angleHorizontal, float16 angleVertical)
{
	// on ajoute la texture de l'arme que l'on va dessiner
	// elle ne sera chargé que si elle ne l'est pas déjà
	this->ajouterTexture(this->fichierTextureCouranteDisplayed);

	// On mémorise le repère courant avant d'effectuer la RST
    glPushMatrix();

		glTranslatef(positionX,
        positionY,
        positionZ);


		glRotatef(angleHorizontal, 0.0, 0.0, 1.0);
		glRotatef(angleVertical, 0.0, 1.0, 0.0);

		sint32 hauteurTextureCourante;
		sint32 largeurTextureCourante;
		// Permet d'afficher le gun et le viseur dans tous les cas et même si une texture est dessiné devant
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		// Permet d'afficher les textures transparentes
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
        hauteurTextureCourante = this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).hauteur;
		largeurTextureCourante = this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).largeur;

		glBindTexture(GL_TEXTURE_2D, this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).texture);
		glBegin(GL_POLYGON);

		// 0.4f correspond au rayon du personnage...
		glTexCoord2f((float16)largeurTextureCourante / this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).largeur, (float16)0.0 / this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).hauteur); 
		glVertex3d(-(0.4f-0.1), 0.25, 0.21);

		glTexCoord2f((float16)largeurTextureCourante / this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).largeur, (float16)hauteurTextureCourante / this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).hauteur); 
		glVertex3d(-(0.4f-0.1), 0.25, -0.21);

		glTexCoord2f((float16)0.0 / this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).largeur, (float16)hauteurTextureCourante / this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).hauteur); 
		glVertex3d(-(0.4f-0.1), -0.25, -0.21);

		glTexCoord2f((float16)0.0 / this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).largeur, (float16)0.0 / this->conteneurTextures.texture(this->fichierTextureCouranteDisplayed).hauteur); 
		glVertex3d(-(0.4f-0.1), -0.25, 0.21);

		glEnd();
		glDisable(GL_BLEND);

    // Restoration du repère d'origine
    glPopMatrix();


	// On mémorise le repère courant avant d'effectuer la RST
	glPushMatrix();

		glTranslatef(positionX,
			positionY,
			positionZ);


		glRotatef(angleHorizontal, 0.0, 0.0, 1.0);
		glRotatef(angleVertical, 0.0, 1.0, 0.0);

		glEnable(GL_TEXTURE_2D);
		// Permet d'afficher les textures transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		hauteurTextureCourante = this->conteneurTextures.texture("personnage\\viseur.png").hauteur;
		largeurTextureCourante = this->conteneurTextures.texture("personnage\\viseur.png").largeur;
		GLuint textureViseur = this->conteneurTextures.texture("personnage\\viseur.png").texture;

		glBindTexture(GL_TEXTURE_2D, textureViseur);
		glBegin(GL_POLYGON);

		// 0.4f correspond au rayon du personnage...
		glTexCoord2f(1,0);
		glVertex3d(-2, 0.25, 0.21);

		glTexCoord2f(1, 1);
		glVertex3d(-2, 0.25, -0.21);

		glTexCoord2f(0, 1);
		glVertex3d(-2, -0.25, -0.21);

		glTexCoord2f(0, 0);
		glVertex3d(-2, -0.25, 0.21);

		glEnd();
		glDisable(GL_BLEND);

	// Restoration du repère d'origine
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
}