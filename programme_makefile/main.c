// Import bibliothèques et fichier  ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parcEolien.h"

FILE* fichierCSV; 

// Fonction principale
int main() 
{   
    // On initialise la graine du random pour plusieurs tests
    srand((unsigned int)time(NULL)); 

    //Menu 
    printf("Pour obtenir un fichier csv taper 1.\n");
    printf("Pour modifier les parametres du parc puis obtenir un fichier csv taper 2.\n");
    printf("Pour obtenir un l'évolution du bénéfice en fonction du nb eoliennes taper 3.\n");
    int choice = input();

    //initialisation matrice de transition
    double matriceTransition[N][N];
    initialiserMatriceTransitionVent(matriceTransition);
    
    switch(choice ){
    //--------------------------------------------------------------
    case 1:
    {   
        //Paramètres modifiables ------------------------------
        printf("Nombre Eolienne: \n");
        int nombreEoliennes; 
        scanf("%d", &nombreEoliennes); // Nombre eolienne du parc
        printf("Nombre jours: \n");
        int nombreJours; 
        scanf("%d", &nombreJours); // Nombre jours de la simulation 

        int nouvelleVitesse = 1;
        int beneficesTotaux = 0;
        //------------------------------------------------------

        //initialisation Parc
        ParcEolien monParcEolien;
        initialiserParcEolien(&monParcEolien, nombreEoliennes);

        //initialisation paramètres
        Parametres Para;
        initialiserParametres(&Para); 

        // FIchier CSV -----------------------------------------------------
        // Ouverture du fichier CSV en mode écriture
        fichierCSV = fopen("resultats_simulation.csv", "w");
        if (fichierCSV == NULL) 
        {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier CSV.\n");
            return 1; // Quitter le programme en cas d'erreur
        }
        // En-tête fichier CSV
        fprintf(fichierCSV, "Gain journalier, Coût maintenance journalier, Etat Vent, Production journalière\n");
        
        // Simulation -----------------------------------------------------
        for (int i = 0; i < nombreJours; i++)
        {
            // simulation pour une journée 
            nouvelleVitesse = Vent(matriceTransition, nouvelleVitesse);
            beneficesTotaux+=simulerJournee(&monParcEolien, &Para, nouvelleVitesse, beneficesTotaux, choice);
            
        }
        fprintf(fichierCSV, "\nBénéfice total simulation: \n%d %s", beneficesTotaux, "€");

        // on libère la mémoire allouée pour le tableau d'éoliennes
        free(monParcEolien.eoliennes);

    }
    break; 

    //--------------------------------------------------------------
    case 2:
    { 
        //Paramètres modifiables ------------------------------
        printf("Nombre Eolienne: \n");
        int nombreEoliennes; 
        scanf("%d", &nombreEoliennes); // Nombre eolienne du parc
        printf("Nombre jours: \n");
        int nombreJours; 
        scanf("%d", &nombreJours); // Nombre jours de la simulation 

        int nouvelleVitesse = 1;
        int beneficesTotaux = 0;
        //------------------------------------------------------

        //initialisation Parc
        ParcEolien monParcEolien;
        initialiserParcEolien(&monParcEolien, nombreEoliennes);

        //initialisation paramètres
        Parametres Para;
        double faible;
        printf("puissance produite sous vent faible: \n");
        scanf("%lf", &faible);
        double moyen;
        printf("puissance produite sous vent moyen: \n:");
        scanf("%lf", &moyen);
        double fort;
        printf("puissance produite sous vent fort: \n:");
        scanf("%lf", &fort);
        int cost;
        printf("coût de la maintenance: \n");
        scanf("%d",&cost);
        int duree;
        printf("durée de la maintenance: \n");
        scanf("%d",&duree);
        modifierParametres(&Para, faible, moyen, fort, cost, duree); 

        //fichier CSV -----------------------------------------------------
        // Ouverture du fichier CSV en mode écriture
        fichierCSV = fopen("resultats_simulation.csv", "w");
        if (fichierCSV == NULL) 
        {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier CSV.\n");
            return 1; // Quitter le programme en cas d'erreur
        }
        // En-tête fichier CSV
        fprintf(fichierCSV, "Gain journalier, Coût maintenance journalier, Etat Vent, Production journalière\n");
        
        //Simulation -----------------------------------------------------
        for (int i = 0; i < nombreJours; i++)
        {
            // simulation pour une journée 
            nouvelleVitesse = Vent(matriceTransition, nouvelleVitesse);
            beneficesTotaux+=simulerJournee(&monParcEolien, &Para, nouvelleVitesse, beneficesTotaux, 1);
            
        }
        fprintf(fichierCSV, "\nBénéfice total simulation: \n%d %s", beneficesTotaux, "€");

        // on libère la mémoire allouée pour le tableau d'éoliennes
        free(monParcEolien.eoliennes);

    }
    break; 
    
    //--------------------------------------------------------------

    case 3:
    {   
        //fichier CSV ----------------------------------------------
        // Ouverture du fichier CSV en mode écriture
        fichierCSV = fopen("resultats_simulation.csv", "w");
        if (fichierCSV == NULL) 
        {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier CSV.\n");
            return 1; // Quitter le programme en cas d'erreur
        }
        // En-tête fichier CSV
        fprintf(fichierCSV, "Nombre d'éoliennes, Bénéfice\n");

        //Input-----------------------------------------------------
        // Nombre de jours de la simulation 
        int nombreJours; 
        printf("Nombre de jours simulation:\n");
        scanf("%d", &nombreJours);

        //Nombre maximum éolienne à tester 
        int eolienneMax; 
        printf("Nombre'éoliennes max du parc"); 
        scanf("%d", &eolienneMax); 
        
        // Boucle pour tester différentes tailles de parc d'éoliennes (on va de 5 en 5)
        for (int nombreEoliennes = 1; nombreEoliennes <= eolienneMax; nombreEoliennes += 5)
        {
            int nouvelleVitesse = 1;
            int beneficesTotaux = 0;
            //------------------------------------------------------

            //initialisation Parc
            ParcEolien monParcEolien;
            initialiserParcEolien(&monParcEolien, nombreEoliennes);

            //initialisation paramètres
            Parametres Para;
            initialiserParametres(&Para); 

            for (int i = 0; i < nombreJours; i++)
            {
                // simulation pour une journée 
                nouvelleVitesse = Vent(matriceTransition, nouvelleVitesse);
                beneficesTotaux+=simulerJournee(&monParcEolien, &Para, nouvelleVitesse, beneficesTotaux, choice);  
            }
            // Enregistrement des résultats pour Gnuplot
            fprintf(fichierCSV, "%d %d\n", nombreEoliennes, beneficesTotaux);

            // Réinitialisation des bénéfices pour le prochain test
            beneficesTotaux = 0;
            // on libère la mémoire allouée pour le tableau d'éoliennes
            free(monParcEolien.eoliennes);
        }
    // Fermeture du fichier CSV 
    fclose(fichierCSV);
    }
    break; 
    return 0;
    }
}