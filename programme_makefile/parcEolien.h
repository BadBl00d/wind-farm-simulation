// Import bibliothèques et fichier  ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE* fichierCSV; 
// Constantes  -------------------------------------------------------------------------------------------------

#define N 3  // Nombre d'états du vent
#define prix 100 // € : prix de l'élec moyen au MWh

//Structures ----------------------------------------------------------------------------------------------------
// Structure pour une éolienne
typedef struct 
{
    int niveauProduction;    // Niveau de production (0, 1, 2)
    int estOperationnelle;   // Indicateur d'état (0 pour maintenance, 1 pour fonctionnement)
    int tempsMaintenance;    // Temps restant avant la fin de la maintenance
} Eolienne;

// Structure du parc d'éoliennes
typedef struct 
{
    int nombreEoliennes;        // Nombre d'éoliennes dans le parc
    Eolienne* eoliennes;        // Tableau d'éoliennes
} ParcEolien;

typedef struct
{
    double Prod[3]; // Puissance produite selon vent
    int Cout; //Cout de maintenance 
    int Maintenance; // Durée de la maintenance
} Parametres;

// Fonctions------------------------------------------------------

// Initialisation d'une éolienne
void initialiserEolienne(Eolienne* eolienne); 
// Initialisation du parc 
void initialiserParcEolien(ParcEolien* parc, int nombreEoliennes) ; 
void initialiserMatriceTransitionVent(double matrice[N][N]); 
void initialiserParametres(Parametres* param); 
void enregistrerResultatsCSV(double benefice, int perte, int etatVent1, double prod) ;
int input();
void modifierParametres(Parametres* param, double faible, double moyen, double fort, int cost, int duree);
// Calcul de la nouvelle force du vent
int Vent(double matrice[N][N], int etatInitial) ;
// Fonction principale de simulation (pour une journée et une vitesse de vent donée )
int simulerJournee(ParcEolien* parc, Parametres* param , int nouvelleVitesse, int beneficesTotaux, int choice); 

