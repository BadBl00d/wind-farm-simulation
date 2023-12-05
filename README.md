# wind-farm-simulation 💡
![](https://media2.giphy.com/media/WSqsdbIH6mLrHe78tJ/giphy.gif)

## What's that ? 💻
Embarking on my first substantial C project, I found myself delving into the intricate realm of simulating a wind farm. The task at hand was to analyze the energy production of a wind turbine park, considering variables such as wind conditions, the robustness of the turbines, and the overall size of the park. The operational framework was elegantly simplified for the exercise. 

## Instructions
Each wind turbine generated energy based on wind speed, categorized into three levels: no wind (yielding no energy production), moderate wind (production level 1), and strong wind (production level 2). The wind speed remained constant throughout the day, with random changes in speed at the onset of each day, governed by a transition matrix, aptly named V. Adding a layer of complexity, when subjected to strong winds, a turbine had a probability 'p' of malfunctioning, leading to its cessation for maintenance. Thus, this endeavor not only delves into the dynamics of renewable energy production but also introduces the challenge of managing turbine reliability within the dynamic interplay of wind conditions.

## Data 📊
- On the first day:
Wind speed is zero.
All wind turbines are in a new state.

- Probability for a wind turbine to malfunction (only if the wind is strong): p = 0.15
- A wind transition matrix
- Wind turbine production in MW based on wind speed (zero, medium, maximum):
  prod = (0, 0.4, 0.65)
- Maintenance conditions:
  - Maintenance following a malfunction requires a 3-day intervention.
  - Cost of an intervention: 150 k€.
  - The intervention is suspended as long as the wind is strong.
After the intervention, the wind turbine is considered to be in a new state.

## Results
![My Image](images/menu1.png)
![My Image](images/menu3.png)

## Explanation of the code

The parameters for the number of simulation days or the number of wind turbines in the park can be directly modified in the terminal (menu #1).

The output is provided in a CSV file 'resultats_simulation.csv' for data analysis purposes. One can also easily change the initialization values for the park, turbines, and parameters using menu #2.

Furthermore, menu #3 allows for displaying the profit evolution based on the number of turbines 'nombreEoliennes' in the park.

In addition, a Makefile facilitates the program's usage in the "programme_makefile" folder.

Note: The "programmes_brut" folder contains the 'final.c' program identical to that of the Makefile, as well as a version allowing plotting in menu 3. However, it requires the installation of 'gnuplot', without which only the 'gnuplot_data.data' file will be visible.

Justification for the Modeling Choices:

Structures:
A structure is used to represent a wind turbine, with fields such as the current wind speed, production level, probability of malfunction, current state (operational or under maintenance), time remaining until the end of maintenance, etc.

This modeling choice can be relevant, for example, if one wants to treat turbines individually (e.g., different production, non-uniform winds).

Another structure can represent global parameters, such as the transition matrix V, the malfunction probability 'p,' wind turbine production based on wind speed, intervention duration, and cost.

Wind Turbine Park:
An array of structures is used to represent the wind turbine park, where each element is an instance of the Wind Turbine structure. Dynamic memory allocation with malloc() is used to manage the park size flexibly.

Transition Matrix:
'etatActuel' can take one of the three following values:

1: Weak Wind
2: Moderate Wind
3: Strong Wind

The function 'int Vent(double matrice[N][N], int etatInitial) -> int' returns the new wind state following the given transition matrix. The function 'void simulerJournee(ParcEolien* parc, int nouvelleVitesse);' performs a case disjunction based on wind strength and traverses all turbines.

'void initialiserEolienne(Eolienne* eolienne)', 'void initialiserParcEolien(ParcEolien* parc, int nombreEoliennes)', and 'void initialiserParametres(Parametres* param)' initialize turbines, the park, and given parameters, respectively.

'void modifierParametres(Parametres* param, double faible, double moyen, double fort, int cost, int duree)' allows for modifying park parameters.

'int simulerJournee(ParcEolien* parc, Parametres* param, int nouvelleVitesse, int beneficesTotaux) --> int' simulates a day of park operation, returns the cumulative profit after 24 hours, and displays data in the created CSV table.

Miscellaneous/Remarks:

The random seed is initialized for multiple tests: srand((unsigned int)time(NULL)); otherwise, randomness is 'identical' with each call.
