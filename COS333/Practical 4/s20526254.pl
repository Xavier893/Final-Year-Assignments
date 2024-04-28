isPositive(X) :- X > 0.
isNegative(X) :- X =< 0.

stripNegativesAndZerosDuplicatePositives([], []).
stripNegativesAndZerosDuplicatePositives([H|T], [H,H|R]) :- isPositive(H), stripNegativesAndZerosDuplicatePositives(T, R).
stripNegativesAndZerosDuplicatePositives([H|T], R) :- isNegative(H), stripNegativesAndZerosDuplicatePositives(T, R).