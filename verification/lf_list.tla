------------------------------ MODULE lf_list ------------------------------
EXTENDS Naturals, Sequences, FiniteSets

CONSTANT Address,
    Keys,
    Process,
    HEAD,
    TAIL

ASSUME \A k \in Keys: HEAD < k /\ k < TAIL

VARIABLES mem,
    proc,
    setup

(* invariants *)

TypeInvariant ==
    /\ {mem[a].key: a \in Address} \subseteq Keys \cup {0, 1, 100}
    /\ {mem[a].next: a \in Address} \subseteq Address \cup {0}
    /\ {mem[a].mark: a \in Address} \subseteq {0, 1}
    /\ setup \in {0, 1}
    /\ {proc[p].ninfo.CNkey: p \in Process} \subseteq Keys \cup {0}
    /\ {proc[p].ninfo.CNnext: p \in Process} \subseteq Address \cup {0}
    /\ {proc[p].ninfo.AdrLeft: p \in Process} \subseteq Address \cup {0}
    /\ {proc[p].ninfo.AdrRight: p \in Process} \subseteq Address \cup {0}
    /\ {proc[p].ninfo.RigNext: p \in Process} \subseteq Address \cup {0}
    /\ {proc[p].procIns: p \in Process} \subseteq {"readyI", "createdI", "locatedI", "unique", "swapedI1"}
    /\ {proc[p].procDel: p \in Process} \subseteq {"readyD", "identifiedD", "locatedD", "assignedD", "swapedD1"}
    /\ {proc[p].choice: p \in Process} \subseteq {"undecided", "toinsert", "todelete"}

Coherence ==
    LET nodp == {j \in Address: (mem[j].key /= 0 /\ mem[j].next /= 0)}
    IN \A i \in nodp: mem[i].key < mem[mem[i].next].key

(* actions *)

Init ==
    /\ mem = [a \in Address |-> [key |-> 0, next |-> 0, mark |-> 0]]
    /\ setup = 0
    /\ proc = [p \in Process |-> [
        ninfo |-> [CNkey |-> 0, CNnext |-> 0, AdrLeft |-> 0, AdrRight |-> 0, RigNext |-> 0],
        procIns |-> "readyI",
        procDel |-> "readyD",
        choice |-> "undecided"]]

SetInitNodes ==
    /\ setup = 0
    /\ LET
            fst == CHOOSE a \in Address: \A i \in Address: a <= i
            scd == CHOOSE a \in Address \ {fst}: \A i \in Address \ {fst}: a <= i
       IN mem' = [mem EXCEPT ![fst].key = HEAD, ![fst].next = scd, ![scd].key = TAIL]
    /\ setup' = 1
    /\ UNCHANGED proc

(* insert actions *)

CreateI(p, key) ==
    /\ setup = 1
    /\ proc[p].procIns = "readyI"
    /\ proc[p].choice = "undecided"
    /\ (Cardinality({a \in Address: mem[a].key = 0}) - Cardinality({i \in Process: proc[i].choice = "toinsert"})) > 0
    /\ proc' = [proc EXCEPT ![p].ninfo.CNkey = key,
                            ![p].procIns = "createdI",
                            ![p].choice = "toinsert"]
    /\ UNCHANGED <<mem, setup>>

LocateI(p) ==
    /\ proc[p].procIns = "createdI"
    /\ LET
            elemr == {j \in Address: (mem[j].key /= 0
                                    /\ mem[j].mark = 0
                                    /\ mem[j].key >= proc[p].ninfo.CNkey)}
            eleml == {j \in Address: (mem[j].key /= 0
                                    /\ mem[j].mark = 0
                                    /\ mem[j].key < proc[p].ninfo.CNkey)}
            right == CHOOSE a \in elemr: \A i \in elemr: mem[a].key <= mem[i].key
            left == CHOOSE a \in elemr: \A i \in eleml: mem[a].key >= mem[i].key
            adj == mem[left].next = right
        IN
            IF adj = TRUE THEN
                /\ proc' = [proc EXCEPT ![p].ninfo.AdrLeft = left,
                                        ![p].ninfo.AdrRight = right,
                                        ![p].procIns = "locatedI"]
                /\ UNCHANGED <<mem, setup>>
            ELSE
                /\ proc' = [proc EXCEPT ![p].procIns = "createdI"]
                /\ UNCHANGED <<mem, setup>>

VerUniq(p) ==
    /\ proc[p].procIns = "locatedI"
    /\ IF mem[proc[p].ninfo.AdrRight].key /= proc[p].ninfo.CNkey THEN
            proc' = [proc EXCEPT ![p].procIns = "unique"]
       ELSE
            proc' = [proc EXCEPT ![p].procIns = "readyI",
                                ![p].choice = "undecided"]
    /\ UNCHANGED <<mem, setup>>

CasI1(p) ==
    /\ proc[p].procIns = "unique"
    /\ proc' = [proc EXCEPT ![p].ninfo.CNnext = proc[p].ninfo.AdrRight,
                            ![p].procIns = "swapedI1"]
    /\ UNCHANGED <<mem, setup>>

CasI2(p) ==
    /\ proc[p].procIns = "swapedI1"
    /\ IF mem[proc[p].ninfo.AdrLeft].next = proc[p].ninfo.AdrRight THEN
        LET
            pos == CHOOSE a \in {b \in Address: mem[b].key = 0}:
                \A i \in {j \in Address: mem[j].key = 0}: a <= i
        IN
            /\ mem' = [mem EXCEPT ![pos].key = proc[p].ninfo.CNkey,
                                ![pos].next = proc[p].ninfo.CNnext,
                                ![proc[p].ninfo.AdrLeft].next = pos]
            /\ proc' = [proc EXCEPT ![p].ninfo.CNkey = 0,
                                    ![p].ninfo.CNnext = 0,
                                    ![p].ninfo.AdrLeft = 0,
                                    ![p].ninfo.AdrRight = 0,
                                    ![p].ninfo.RigNext = 0,
                                    ![p].procIns = "readyI",
                                    ![p].choice = "undecided"]
            /\ UNCHANGED setup
       ELSE
        /\ proc' = [proc EXCEPT ![p].procIns = "createdI"]
        /\ UNCHANGED <<mem, setup>>

(* delete actions *)

Identify(p, key) ==
    /\ setup = 1
    /\ proc[p].procDel = "readyD"
    /\ proc[p].choice = "undecided"
    /\ proc' = [proc EXCEPT ![p].ninfo.CNkey = key,
                            ![p].procDel = "identifiedD",
                            ![p].choice = "todelete"]
    /\ UNCHANGED <<mem, setup>>

LocateD(p) ==
    /\ proc[p].procDel = "identifiedD"
    /\ LET
            posr == {j \in Address: (mem[j].key /= 0
                                    /\ mem[j].mark = 0
                                    /\ mem[j].key >= proc[p].ninfo.CNkey)}
            posl == {j \in Address: (mem[j].key /= 0
                                    /\ mem[j].mark = 0
                                    /\ mem[j].key < proc[p].ninfo.CNkey)}
            right == CHOOSE a \in posr: \A i \in posr: mem[a].key <= mem[i].key
            left == CHOOSE a \in posl: \A i \in posl: mem[a].key >= mem[i].key
        IN
            IF mem[right].key = proc[p].ninfo.CNkey THEN
                /\ proc' = [proc EXCEPT ![p].ninfo.AdrLeft = left,
                                        ![p].ninfo.AdrRight = right,
                                        ![p].procDel = "locatedD"]
                /\ UNCHANGED <<mem, setup>>
            ELSE
                /\ proc' = [proc EXCEPT ![p].ninfo.CNkey = 0,
                                        ![p].procDel = "readyD",
                                        ![p].choice = "undecided"]
                /\ UNCHANGED <<mem, setup>>

AssignD(p) ==
    /\ proc[p].procDel = "locatedD"
    /\ proc' = [proc EXCEPT ![p].ninfo.RigNext = mem[proc[p].ninfo.AdrRight].next,
                            ![p].procDel = "assignedD"]
    /\ UNCHANGED <<mem, setup>>

CasD1(p) ==
    /\ proc[p].procDel = "assignedD"
    /\ IF mem[proc[p].ninfo.RigNext].mark = 0 THEN
            IF mem[proc[p].ninfo.AdrRight].next = proc[p].ninfo.RigNext THEN
                /\ mem' = [mem EXCEPT ![proc[p].ninfo.AdrRight].mark = 1]
                /\ proc' = [proc EXCEPT ![p].procDel = "swapedD1"]
                /\ UNCHANGED setup
            ELSE
                /\ proc' = [proc EXCEPT ![p].procDel = "identifiedD"]
                /\ UNCHANGED <<mem, setup>>
        ELSE
            /\ proc' = [proc EXCEPT ![p].procDel = "identifiedD"]
            /\ UNCHANGED <<mem, setup>>

CasD2(p) ==
    /\ proc[p].procDel = "swapedD1"
    /\ IF mem[proc[p].ninfo.AdrLeft].next = proc[p].ninfo.AdrRight THEN
            /\ mem' = [mem EXCEPT ![proc[p].ninfo.AdrLeft].next = proc[p].ninfo.RigNext]
            /\ proc' = [proc EXCEPT ![p].ninfo.CNkey = 0,
                                    ![p].ninfo.CNnext = 0,
                                    ![p].ninfo.AdrLeft = 0,
                                    ![p].ninfo.AdrRight = 0,
                                    ![p].ninfo.RigNext = 0,
                                    ![p].procDel = "readyD",
                                    ![p].choice = "undecided"]
            /\ UNCHANGED setup
       ELSE
            /\ proc' = [proc EXCEPT ![p].ninfo.AdrLeft = 0,
                                    ![p].ninfo.AdrRight = 0,
                                    ![p].ninfo.RigNext = 0,
                                    ![p].procDel = "identifiedD"]
            /\ UNCHANGED <<mem, setup>>

(* LFList methods description *)

Insert(i, k) == CreateI(i, k) \/ LocateI(i) \/ VerUniq(i) \/ CasI1(i) \/ CasI2(i)
Delete(i, k) == Identify(i, k) \/ LocateD(i) \/ AssignD(i) \/ CasD1(i) \/ CasD2(i)

Next ==
    \/ SetInitNodes
    \/ \E i \in Process: \E k \in Keys: Insert(i, k) \/ Delete(i, k)

Spec == Init /\ [][Next]_<<mem, proc, setup>>

THEOREM Spec => [](TypeInvariant /\ Coherence)

============================================================================
