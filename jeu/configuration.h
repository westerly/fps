#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED


#define LARGEUR_FENETRE (320*2)
#define HAUTEUR_FENETRE (240*2)

#define TITRE_APPLICATION "Virtual Environment Shoot"

#define PRET 0.1
#define LOIN 1000.0
#define ANGLE_VISION 70.0

#define RADIANS_PAR_DEGRES 0.0174532925199
#define HAUTEUR_OEIL_PERSONNAGE 0.8


// Fréquence de l'affichage des images de l'animation de l'arme
#define FREQUENCE_IMAGE_ANIMATION_ARME 50


#define HAUTEUR_MURS 2

#define VITESSE_DEPLACEMENT_PERSONNAGE 2.0f

#define VITESSE_DEPLACEMENT_BULLET 2.0f

// Représente le nombre de cibles max affiché au maximum au début du jeu
#define NBR_CIBLES_MAX 3
// Représente le nombre max d'elements qu'une cible peut avoir à sa base 
#define NBR_EL_TARGET_BASE_MAX 8
// Représente le temps de vie maximum d'une cible au début du jeu (en millisecondes)
#define MAX_LIVING_TIME_TARGET_BEGINNING 5000
// Représente la fraction de la largeur d'un element d'une cible qui servira d'espacement entre les elements des cibles
#define ESPACEMENT_ENTRE_ELEMENTS 0.33333
// La taille min de la largeur d'un element d'une cible
#define MIN_LARGEUR_EL_TARGET_CYLINDRE 0.25
// La taille max de la largeur d'un element d'une cible
#define MAX_LARGEUR_EL_TARGET_CYLINDRE 1.0
// La taille min de la largeur d'un element d'une cible
#define MIN_LARGEUR_EL_TARGET_BOX 0.15
// La taille max de la largeur d'un element d'une cible
#define MAX_LARGEUR_EL_TARGET_BOX 0.35

// Le temps en millisecondes au bout duquel la cible correctement shooté disparait
#define TIMES_BEFORE_TARGET_RIGHT_SHOT_DISAPPEARS 5000

// Variable static pour le relief en mode 3D
#define VR 0.05

#endif // CONFIGURATION_H_INCLUDED

