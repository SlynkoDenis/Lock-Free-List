------------------------------ MODULE lf_list ------------------------------
EXTENDS Integers, TLC, Naturals, Sequences, FiniteSets
CONSTANT N
ASSUME Nasm == (N \in Nat) /\ (N > 0)
