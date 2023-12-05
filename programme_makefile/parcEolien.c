
// Import bibliothèques et fichier  ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parcEolien.h"

FILE* fichierCSV; 

// Constantes  -------------------------------------------------------------------------------------------------

#define N 3  // Nombre d'états du vent
#define prix 100 // € : prix de l'élec moyen au MWh

// Initialisation ------------------------------------------------------------------------------------
// Initialisation d'une éolienne
void initialiserEolienne(Eolienne* eolienne) 
{
    eolienne->niveauProduction = 0;
    eolienne->estOperationnelle = 1;
    eolienne->tempsMaintenance = 0;
}

// Initialisation du parc 
void initialiserParcEolien(ParcEolien* parc, int nombreEoliennes) 
{
    parc->nombreEoliennes = nombreEoliennes;
    parc->eoliennes = malloc(nombreEoliennes * sizeof(Eolienne)); // Allocation dynamique du tableau de structures
    for (int i = 0; i < nombreEoliennes; ++i) 
    {
        initialiserEolienne(&(parc->eoliennes[i])); // On initialise toutes les eoliennes
    }
}

void initialiserMatriceTransitionVent(double matrice[N][N]) 
{
    matrice[0][0] = 0.2;
    matrice[0][1] = 0.73;
    matrice[0][2] = 0.07;
    matrice[1][0] = 0.11;
    matrice[1][1] = 0.74;
    matrice[1][2] = 0.15;
    matrice[2][0] = 0.04;
    matrice[2][1] = 0.61;
    matrice[2][2] = 0.35;
}

void initialiserParametres(Parametres* param)
{
    param->Prod[0] = 0; 
    param->Prod[1] = 0.4;
    param->Prod[2] = 0.65; 
    param->Cout = 150000; 
    param->Maintenance = 3; 
}

// Fonctions annexes------------------------------------------------------------------------------------------------

void enregistrerResultatsCSV(double benefice, int perte, int etatVent1, double prod) 
{
    fprintf(fichierCSV, "%f,%d,%d, %f \n", benefice, perte, etatVent1, prod);
}

int input()
{
    int number;
    scanf("%d", &number);
    return (number);
}

void modifierParametres(Parametres* param, double faible, double moyen, double fort, int cost, int duree)
{
    param->Prod[0] = faible; 
    param->Prod[1] = moyen;
    param->Prod[2] = fort; 
    param->Cout = cost; 
    param->Maintenance = duree; 
}
//-------------------------------------------------------------------------------------------------------------------

// Calcul de la nouvelle force du vent
int Vent(double matrice[N][N], int etatInitial) 
{
    int etatActuel = etatInitial;

        // Les pb de transition pour l'état actuel 
        double probasTransition[N];
        for (int j = 0; j < N; ++j) 
        {
            probasTransition[j] = matrice[etatActuel - 1][j];
        }

        // On génére un nombre aléatoire entre 0 et 1
        double randNum = (double)rand() / RAND_MAX;

        // On trouve l'état suivant en fonction des probabilités de transition
        double sum = 0.0;
        for (int j = 0; j < N; ++j) 
        {
            sum += probasTransition[j];
            if (randNum <= sum) 
            {
                etatActuel = j + 1;
                break;
            }
        }
    return etatActuel; // Retourne la nouvelle vitesse de vent 
}

// Fonction principale de simulation (pour une journée et une vitesse de vent donée )
int simulerJournee(ParcEolien* parc, Parametres* param , int nouvelleVitesse, int beneficesTotaux, int choice)
{   

    double benef = 0 ;
    double perte = 0; // on initialise les bénéfices et pertes de la journée 
    double productionJournaliere = 0; 
    // Vent fort --- probabilité de panne : si vent fort seulement
    if (nouvelleVitesse == 3)
    {   
        for (int i = 0; i < parc->nombreEoliennes; ++i)
        {
            // Génération d'un nombre aléatoire entre 0 et 1
            double randNum = (double)rand() / RAND_MAX;

            // Probabilité de panne associée à une vitesse de vent forte (0.15)
            double probabilitePanne = 0.15;

            // Si le nombre aléatoire est inférieur ou égal à la probabilité de panne,
            // et l'éolienne est opérationnelle, elle tombe en panne
            if (randNum <= probabilitePanne && parc->eoliennes[i].estOperationnelle == 1)
            {
                parc->eoliennes[i].estOperationnelle = 0; // éolienne en panne
                parc->eoliennes[i].tempsMaintenance = param->Maintenance; // temps de maintenance de 3 jours
                perte += param->Cout; // coût de l'intervention
            }

            // Bénéfices et pertes
            else if (parc->eoliennes[i].estOperationnelle == 1)
            {
                benef += 24 * prix * (param->Prod[2]);
                productionJournaliere += param->Prod[2];
            }
        }
    }
    // Vent force est 2
    else if (nouvelleVitesse == 2)
    {
        for (int i = 0; i < parc->nombreEoliennes; ++i)
        {
            
            // Bénéfices et pertes
            if (parc->eoliennes[i].estOperationnelle == 1)
            {
                benef += 24 * prix * (param->Prod[1]);
                productionJournaliere += param->Prod[1];
            }
            // Si la turbine est en maintenance, décrémenter le temps de maintenance
            if (parc->eoliennes[i].tempsMaintenance > 0)
            {
                parc->eoliennes[i].tempsMaintenance -= 1;
            }

            // Si la maintenance est terminée, marquer la turbine comme opérationnelle
            if (parc->eoliennes[i].tempsMaintenance == 0)
            {
                parc->eoliennes[i].estOperationnelle = 1;
            }

            
        }
    }
    // Vent force est 1
    else
    {
        for (int i = 0; i < parc->nombreEoliennes; ++i)
        {
            // Bénéfices et pertes
            if (parc->eoliennes[i].estOperationnelle == 1)
            {
                benef += 24 * prix * (param->Prod[0]);
                productionJournaliere += param->Prod[0];
            }
            
            // Si la turbine est en maintenance, décrémenter le temps de maintenance
            if (parc->eoliennes[i].tempsMaintenance > 0)
            {
                parc->eoliennes[i].tempsMaintenance -= 1;
            }

            // Si la maintenance est terminée, marquer la turbine comme opérationnelle
            if (parc->eoliennes[i].tempsMaintenance == 0)
            {
                parc->eoliennes[i].estOperationnelle = 1;
            }
        }
    }
        
        // On enregistre les résultats dans le fichier 
        if (choice==1)
        {
            enregistrerResultatsCSV(benef, perte, nouvelleVitesse, productionJournaliere);
        }

    // Mise à jour des bénéfices totaux
    beneficesTotaux += (benef - perte);
    return beneficesTotaux;
}