// TIDEMAN.C HAS ERRORS (SCORE: 71%)

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
    int gap;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void merge_sort(int i, int j, pair array[], pair sorted[]);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Take a candidate name (and their index)
        if (!strcmp(candidates[i], name))
            {
            // Put the index in the ranked order
            // EXAMPLE: Candidates are John [0], Mike [1], Peter [2] and Nick [3]
            // Written on the ballot: Mike, Nick, John, Peter
            // Ranks would be: [1, 3, 0, 2]
            ranks[rank] = i;
            return true;
            }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // See that this function will be called for every voter
    // Function call in line 88 is within the loop in line 71 to 91
    // This function loops over candidates, not voters
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // See the rank example in line 111
            // Tally the score for everyone ranked after Mike ([ranks[0], which is [1])
            // Put the score in the two dimensional array
            // EXAMPLE: [Mike][Nick]++, [Mike][John]++, [Mike][Peter]++, then [Nick][John]++, etc.
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < pair_count; i++) // Iterate over the pairs array of pair struct
    {
        for (int j = 0; j < candidate_count; j++) // Iterate over collumns in preferences (the more preferred)
        {
            for (int k = 0; k < candidate_count; k++) // Iterate over rows in preferences (the less preferred)
            {
                if (preferences[j][k] > preferences [k][j]) // Who is more frequently preferred: i or j?
                {
                    pair p; // Create a local variable to later populate the pairs array
                    p.winner = j; // Establish the winner of the pair
                    p.loser = k; // Establis the loser of the pair
                    // p.gap = preferences[j][k] - preferences[k][k];
                    pairs[i] = p; // Populate the pairs array
                    // Notice how the if statement would ignore tied pairs
                }
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair sorted[pair_count];
    merge_sort(0, pair_count - 1, pairs, sorted);
}

// Check for cycles
bool cycle_check(string s)
{
    bool cycle = true;
    for (int j = 0; j <= pair_count; j++)
    {
        if (strcmp(s, candidates[pairs[j].winner]))
        {
            cycle_check(candidates[pairs[j].loser]);
            cycle = true;
        }
        else
        {
            cycle = false;
        }
    }
    if (cycle == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}
// If true, a cycle has been detected
// Take the most recently locked pair (e.g. [Mike][John])
// Search if the loser [John] is a winner in another pair (e.g. [John][Peter])
// Recursively do this until either:
// a. The function has been called as many times as the (pair_count + 1)
// (i.e. there's obviously a loop); return true
// b. The function finds a dead end (i.e. candidate without edge, a.k.a. ultimate loser); return false

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        if (cycle_check(candidates[pairs[i].loser]) == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    int winner;
    int rank;

    for (int i = 0; i < candidate_count; i++)
    {
        rank = 0;
        for (int k = 0; k < candidate_count; k++)
        {
            if (locked[k][i] == false)
            {
                rank++;
            }
        }

        // Prints all the names that are the source of the graph
        if (rank == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }
}

// Merge Sort
void merge_sort(int first_i, int last_i, pair array[], pair sorted[])
{
    // If sub-array is only 0 or 1 instance/element long, stop recursion
    if (last_i <= first_i)
    {
        return;
    }

    int mid_i = (first_i + last_i) / 2;
    // Establish mid_i as the last instance/element of LEFT the side of the array
    // DEBUG: printf("%i is the middle point of the array\n", mid_i);

    merge_sort(first_i, mid_i, array, sorted);
    // DEBUG: printf("Merge sorting LEFT: %i to %i\n", first_i, mid_i);
    merge_sort(mid_i + 1, last_i, array, sorted);
    // DEBUG: printf("Merge sorting RIGHT: %i to %i\n\n", mid_i + 1, last_i);
    // Perform recursion on each side of the array

    int i_left = first_i;
    int i_right = mid_i + 1;
    int i_merged;
    // Pretend that array[] is actually two sub-arrays
    // LEFT sub-array is array[first_i ... mid_i]
    // RIGHT sub-array is array[mid_i + 1 ... last_i]
    // The merged list will be in sorted[i_merged]

    for (i_merged = first_i; i_merged <= last_i; i_merged++)
        // Loop from the first_i to the last_i
    {
        if (i_left == mid_i + 1)
            // If the LEFT side of the array has been completely looped over and merged,
        {
            sorted[i_merged] = array[i_right];
            // Just get the value from the RIGHT side of the array
            i_right++;
            // Move to the next leftmost/smallest value of the RIGHT side of the array
        }
        else if (i_right == last_i + 1)
            // If the RIGHT side of the array has been completely looped over and merged,
        {
            sorted[i_merged] = array[i_left];
            // Just get the value from the LEFT side of the array
            i_left++;
            // Move to the next leftmost/smallest value of the LEFT side of the array
        }
        else if (array[i_left].gap < array[i_right].gap)
            // If the leftmost value of LEFT side of the array is
            // LESS THAN the leftmost value of RIGHT side of the array
        {
            sorted[i_merged] = array[i_left];
            // Get the value from the LEFT side of the array
            i_left++;
            // Move to the next leftmost/smallest value of the LEFT side of the array
        }
        else // if (array[i_right].gap < array[i_left].gap)
            // If the leftmost value of the RIGHT side of the array is
            // LESS THAN the leftmost value of the LEFT side of the array
        {
            sorted[i_merged] = array[i_right];
            // Get the value from the RIGHT side of the array
            i_right++;
            // Move to the next leftmost/smallest value of the RIGHT side of the array
        }
    }

    for (i_merged = first_i; i_merged <= last_i; i_merged++)
        // Loop from the first_i to the last_i
    {
        array[i_merged] = sorted[i_merged];
        // Replace the original array with the merged array
    }
}