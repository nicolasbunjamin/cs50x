A Tideman election, per the below.

./tideman Alice Bob Charlie
Number of voters: 5
Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob

Charlie

Tideman election tries to find “Condorcet winner” of the election:
the person who would have won any head-to-head matchup against another candidate.

Also known as “ranked pairs," it is a ranked-choice voting method
that’s guaranteed to produce the Condorcet winner of the election if one exists.

Generally speaking, the Tideman method works by constructing a “graph” of candidates,
where an arrow (i.e. edge) from candidate A to candidate B indicates that candidate A wins against candidate B in a head-to-head matchup.