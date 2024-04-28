married(peter, mary).
married(lilly, joseph).
ownsPet(peter, rover).
ownsPet(mary, fluffy).
ownsPet(joseph, tweety).
ownsPet(lilly, fluffy).

household(X, Y) :- married(X, Y).
household(X, Y) :- married(Y, X).

householdPet(PER1, PER2, P) :- ownsPet(PER1, P), household(PER1, PER2).
householdPet(PER1, PER2, P) :- ownsPet(PER2, P), household(PER1, PER2).

wanderingPet(P) :-  householdPet(X, Y, P), householdPet(Z, W, P), X \= Z, Y \= W, X \= W, Y \= Z.

addPositives([], 0).

addPositives([H|T], Sum) :- H > 0, addPositives(T, RestSum), Sum is H + RestSum.

addPositives([H|T], Sum) :- H =< 0, addPositives(T, Sum).

getEverySecondValue([], []).
geteverySecondValue([_], []).
getEverySecondValue([_, Second | Tail], [Second | Result]) :-
    getEverySecondValue(Tail, Result).
