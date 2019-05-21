#include "production.hpp"

#include <iostream>
#include <limits>

// Cette fonction prend en entree une instance du probleme de planification de la production et retourne
// dans le vecteur "solution" la quantite qui doit etre produite chaque jour afin de minimiser les couts
// de production et d'entreposage.
void ProductionProgrammationDynamique(const InstanceProduction& instance, std::vector<unsigned int>& solution) {
  // Ecrivez votre code ici. Votre algorithme devrait proceder en deux phases:
  //   1) il devrait construire un tableau.
  //   2) il devrait remonter le tableau et ecrire la solution dans vecteur "solution".
  //
  // Vous pouvez declarer de nouvelles fonctions dans le fichier production.cpp, mais vous ne devez pas modifier le fichier production.hpp

    std::vector<int> vPrecedent(instance.entreposage_maximal() + 1);

    int table[instance.nb_jours()][instance.entreposage_maximal() + 1];

    for (int i = 0; i < instance.nb_jours(); i++){
        for (int j = 0; j <= instance.entreposage_maximal(); j++){
            if (i == 0 && j >= 0 && instance.demande(i)+j <= instance.production_maximale()){
                    table[i][j] = instance.cout_production(instance.demande(i)+j) + instance.cout_entreposage(j);
            }
            else if (i != 0) {
                for (int k = 0; k <= instance.entreposage_maximal(); k++){
                    if (instance.demande(i)-k+j <= instance.production_maximale()){
                        vPrecedent[k] = table [i-1][k] + instance.cout_production(instance.demande(i)-k+j) + instance.cout_entreposage(j);
                    } else {
                        //cas max
                        vPrecedent[k] = instance.cout_production(instance.production_maximale()) + instance.cout_entreposage(instance.entreposage_maximal());
                    }
                }
                table [i][j] = *std::min_element(vPrecedent.begin(), vPrecedent.end());
            }
        }
    }



    // trouver le v_min de la ligne derniere ligne
    int entrepot_actuel = 0;// index de val v_min
    int v_min = instance.cout_production(instance.production_maximale()) + instance.cout_entreposage(instance.entreposage_maximal()); //val minimum
    for (int j = 0; j <= instance.entreposage_maximal(); j++){
        if (table[instance.nb_jours()-1][j] < v_min){ v_min = table[instance.nb_jours()-1][j]; entrepot_actuel = j; }
    }

    int i = 0;
    for (i = instance.nb_jours()-1; i > 0; i--){
        //construire le vecteur des possibilités pou v_min
        for (int k = 0; k <= instance.entreposage_maximal(); k++){
            vPrecedent[k] = table [i-1][k] + instance.cout_production(instance.demande(i)-k+entrepot_actuel) + instance.cout_entreposage(entrepot_actuel);
        }
        //trouver le v_precedent qui permet de trouver v_min
        int index_entrepot_precdent = 0;
        int v_entrepot_precdent = 0;
        for (index_entrepot_precdent = 0; index_entrepot_precdent <= instance.entreposage_maximal(); index_entrepot_precdent++){
            if (vPrecedent[index_entrepot_precdent] == v_min){
                break;
            }

        }
        solution.push_back(instance.demande(i)-index_entrepot_precdent+entrepot_actuel);
        v_min = table[i-1][index_entrepot_precdent];
        entrepot_actuel = index_entrepot_precdent;
    }

    solution.push_back(instance.demande(i)+entrepot_actuel);
    std::reverse(solution.begin(),solution.end());
}
