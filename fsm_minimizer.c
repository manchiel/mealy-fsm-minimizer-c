#include <stdio.h>
#include <stdlib.h>
#define MAX_INPUT 20

typedef struct{
    int next_state;
    int output;
}Transition;

typedef struct{
    int stateA;
    int stateB;
}StatePair;

typedef struct{
    int is_eliminated;
    StatePair pairs[MAX_INPUT];
    int num_pairs;
}ImplicationTable;

int main() {

    int num_states;
    int num_inputs;

    printf("\nEnter the number of states: ");
    scanf("%d", &num_states);
    printf("Enter the number of inputs: ");
    scanf("%d", &num_inputs);

    Transition **table = (Transition **)malloc(num_states * sizeof(Transition *));
    for (int i = 0; i < num_states; i++) {
        table[i] = (Transition *)malloc(num_inputs * sizeof(Transition));
    }

    printf("---- Enter the transition table (next state and output for each state and input) ----\n");
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < num_inputs; j++) {
            printf("State S%d, Input %d: ", i, j);
            scanf("%d %d", &table[i][j].next_state, &table[i][j].output);
        }
    }
    
    ImplicationTable **imp_table = (ImplicationTable **)malloc((num_states-1) * sizeof(ImplicationTable *));
    for(int i = 0; i < num_states-1; i++) {
        imp_table[i] = (ImplicationTable *)malloc((i+1) * sizeof(ImplicationTable));
    }

    for (int i = 0; i < num_states - 1; i++) {
        for (int j = 0; j <= i; j++) {
            imp_table[i][j].is_eliminated = 0;
            imp_table[i][j].num_pairs = 0; 
        }
    }

    // Step 1: Mark states with different outputs as non-equivalent (X)

    for (int i = 1; i < num_states; i++) {
        for (int j = 0; j < i; j++) {

            for (int k = 0; k < num_inputs; k++) {
                if(table[i][k].output != table[j][k].output) {
                    imp_table[i-1][j].is_eliminated = 1; 
                    break;
                }
            }
        }
    }

    // Step 2: Record next-state dependencies for pairs with same outputs

    for(int i = 1; i < num_states; i++) {
        for(int j = 0; j < i; j++) {

            if(!imp_table[i-1][j].is_eliminated){
               
                for(int k = 0; k < num_inputs; k++) {
                    int next_i = table[i][k].next_state;
                    int next_j = table[j][k].next_state;

                    if(next_i != next_j) {
                        if(!((next_i == i && next_j == j) || (next_i == j && next_j == i))) {
                            
                            int index = imp_table[i-1][j].num_pairs;
                            imp_table[i-1][j].pairs[index].stateA = next_i;
                            imp_table[i-1][j].pairs[index].stateB = next_j;
                            imp_table[i-1][j].num_pairs++;
                        }
                    }
                }
            }
        }
    }

    // Step 3: Iteratively eliminate pairs based on dependencies

    int changed = 1;
    while(changed){
        changed = 0;
        for(int i = 1; i < num_states;i++){
            for(int j = 0; j< i; j++){

                if(!imp_table[i-1][j].is_eliminated){

                    for(int p = 0; p < imp_table[i-1][j].num_pairs; p++) {
                        int stateA = imp_table[i-1][j].pairs[p].stateA;
                        int stateB = imp_table[i-1][j].pairs[p].stateB;

                        int row = (stateA > stateB) ? stateA : stateB;
                        int col = (stateA > stateB) ? stateB : stateA;

                        if(imp_table[row-1][col].is_eliminated == 1) {
                            imp_table[i-1][j].is_eliminated = 1;
                            changed = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    // Step 4: Collect and print equivalent states 


printf("\n---- Equivalent Pairs Found ----\n");
    int found_any = 0;
    for (int i = 1; i < num_states; i++) {
        for (int j = 0; j < i; j++) {
            
            if (imp_table[i-1][j].is_eliminated == 0) {
                if(i-1 < j){
                    printf("S%d and S%d are equivalent.\n", i, j);
                }else{
                    printf("S%d and S%d are equivalent.\n", j, i);
                } 
                found_any = 1;
            }
        }
    }

    if (!found_any) {
        printf("No equivalent states found. The machine is already minimal.\n");
    }

    // Step 5: Create new state classes and print the minimized machine

    int *boss = (int *)malloc(num_states * sizeof(int));
    for (int i = 0; i < num_states; i++) {
        boss[i] = i;
    }
    for(int i = 1; i < num_states; i++){
        for(int j = 0; j < i; j++){
            if(!imp_table[i-1][j].is_eliminated){
                int old_boss = boss[i];
                int new_boss = boss[j];
                for(int k = 0; k < num_states; k++){
                    if(boss[k] == old_boss){
                        boss[k] = new_boss;
                    }
                }
            }
        }
    }

    printf("\n---- Final Equivalence Classes ----\n");
    int *visited = (int *)calloc(num_states, sizeof(int));
    int class_counter = 0;

    for (int i = 0; i < num_states; i++) {
        if (!visited[i]) {
            printf("A%d = { %d", class_counter++, i);
            visited[i] = 1;
            for (int j = i + 1; j < num_states; j++) {
                if (boss[j] == boss[i]) {
                    printf(", %d", j);
                    visited[j] = 1;
                }
            }
            printf(" }\n");
        }
    }

    free(boss);
    free(visited);

    for(int i = 0; i < num_states - 1; i++) {
        free(imp_table[i]);
    }
    free(imp_table);

    for (int i = 0; i < num_states; i++) { 
        free(table[i]);
    }
    free(table);

    return 0;
}