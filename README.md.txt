# Mealy FSM Minimizer

## Motivation
This project was developed as a practical tool for the **Logical Design of Digital Systems** course. Its primary purpose is to provide a quick and reliable way to verify manual calculations of Mealy FSM minimization (implication tables and equivalence classes) while practicing for exams.

A C implementation of minimizing Mealy Finite State Machines.

## Logic & Implementation
Equivalent states must satisfy two conditions:
1. **Identical outputs** for all inputs.
2. **Equivalent next-state transitions** (states they transition to must also be equivalent).

### The Minimization Algorithm:
The program uses a systematic elimination system (Implication Table):
1. **Initial Marking**: Eliminate all pairs that have different outputs for the same input.
2. **Recording Dependencies**: For non-eliminated pairs, record the next-state transition pairs as conditions.
3. **Iterative Elimination**: Repeatedly remove state pairs if their dependency pair has already been eliminated.
4. **Final Grouping**: Use transitivity to group remaining states into final equivalence classes (e.g., if 0=3 and 3=7, then A0 = {0, 3, 7}).

## Terminal Execution Example

```text
Enter the number of states: 8
Enter the number of inputs: 3
---- Enter the transition table (next state and output for each state and input) ----
State S0, Input 0: 0 0
State S0, Input 1: 1 1
State S0, Input 2: 7 1
State S1, Input 0: 2 1
State S1, Input 1: 0 0
State S1, Input 2: 6 1
State S2, Input 0: 0 1
State S2, Input 1: 4 0
State S2, Input 2: 1 0
State S3, Input 0: 0 0
State S3, Input 1: 6 1
State S3, Input 2: 0 1
State S4, Input 0: 3 1
State S4, Input 1: 4 0
State S4, Input 2: 1 1
State S5, Input 0: 3 1
State S5, Input 1: 4 0
State S5, Input 2: 6 0
State S6, Input 0: 5 1
State S6, Input 1: 7 0
State S6, Input 2: 1 1
State S7, Input 0: 0 0
State S7, Input 1: 1 1
State S7, Input 2: 0 1

---- Equivalent Pairs Found ----
S0 and S3 are equivalent.
S2 and S5 are equivalent.
S1 and S6 are equivalent.
S0 and S7 are equivalent.
S3 and S7 are equivalent.

---- Final Equivalence Classes ----
A0 = { 0, 3, 7 }
A1 = { 1, 6 }
A2 = { 2, 5 }
A3 = { 4 }
```