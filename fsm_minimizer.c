#include <stdio.h>
#include <stdlib.h>
#define MAX_INPUT 30

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
    if (scanf("%d", &num_states) != 1 || num_states <= 0) {
        printf("Invalid number of states.\n");
        return 1;
    }

    printf("Enter the number of inputs: ");
    if (scanf("%d", &num_inputs) != 1 || num_inputs <= 0) {
        printf("Invalid number of inputs.\n");
        return 1;
    }

    if (num_inputs > MAX_INPUT) {
        printf("Error: Number of inputs exceeds maximum allowed (%d).\n", MAX_INPUT);
        return 1;
    }

    Transition **table = (Transition **)malloc(num_states * sizeof(Transition *));
    if (!table) { printf("Memory allocation failed.\n");free(table); return 1; }

    for (int i = 0; i < num_states; i++) {
        table[i] = (Transition *)malloc(num_inputs * sizeof(Transition));
        if (!table[i]) { printf("Memory allocation failed.\n"); free(table); return 1; }
    }

    printf("---- Enter the transition table (next state and output for each state and input) ----\n");
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < num_inputs; j++) {
            printf("State S%d, Input %d: ", i, j);
            if (scanf("%d %d", &table[i][j].next_state, &table[i][j].output) != 2) {
                printf("Invalid input.\n");
                free(table);
                return 1;
            }
            if (table[i][j].next_state < 0 || table[i][j].next_state >= num_states) {
                printf("Error: Next state %d out of bounds (0-%d).\n", table[i][j].next_state, num_states - 1);
                free(table);
                return 1;
            }
        }
    }
    
    ImplicationTable **imp_table = (ImplicationTable **)malloc((num_states-1) * sizeof(ImplicationTable *));
    if (!imp_table) { printf("Memory allocation failed.\n"); return 1; }

    for(int i = 0; i < num_states-1; i++) {
        imp_table[i] = (ImplicationTable *)malloc((i+1) * sizeof(ImplicationTable));
        if (!imp_table[i]) { printf("Memory allocation failed.\n"); return 1; } 
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
                        if(!((next_i == i && next_j == j) || (next_i == j && next_j == i))) { // Avoiding infinite loops for self-references
                            
                            int index = imp_table[i-1][j].num_pairs;
                            if (index < MAX_INPUT) {
                                imp_table[i-1][j].pairs[index].stateA = next_i;
                                imp_table[i-1][j].pairs[index].stateB = next_j;
                                imp_table[i-1][j].num_pairs++;
                            }
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

                        if(stateA == stateB) {
                            continue;
                        }

                        if(stateA < stateB) {
                            int temp = stateA;
                            stateA = stateB;        // Making sure stateA is always the larger index
                            stateB = temp;
                        }

                        if(stateA > 0 && stateB >= 0 && stateA - 1 < num_states - 1 && stateB <= stateA - 1) {
                            if(imp_table[stateA-1][stateB].is_eliminated) {
                                imp_table[i-1][j].is_eliminated = 1;
                                changed = 1;
                                break;
                            }
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
                
                printf("S%d and S%d are equivalent.\n", j, i); 
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
    int *visited = (int *)calloc(num_states, sizeof(int)); //calloc to initialize to 0
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
