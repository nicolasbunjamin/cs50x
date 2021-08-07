#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);
void merge_sort(int first_i, int last_i, candidate array[], candidate sorted[]);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(candidates[i].name, name))
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Loop over the rows of preferences (voter/ballot)
    for (int i = 0; i < voter_count; i++)
    {
        // Loop over the cells (collumns) in each row (ranks within ballots)
        for (int j = 0; j < candidate_count; j++)
        {
            if (candidates[preferences[i][j]].eliminated == false)
            {
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int minimum_vote = voter_count / 2 + 1;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes >= minimum_vote)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    candidate sorted[candidate_count];
    merge_sort(0, candidate_count - 1, candidates, sorted);
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            return candidates[i].votes;
        }
    }
    return candidates[0].votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int running_candidate = 0;
    int count_of_loser = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            running_candidate++;
        }
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            count_of_loser++;
        }
    }
    if (running_candidate == count_of_loser)
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
}

// Merge sort algorithm
void merge_sort(int first_i, int last_i, candidate array[], candidate sorted[])
{
    // if sub-array is only 0 or 1 instance/element long, stop recursion
    if (last_i <= first_i)
    {
        return;
    }

    // establish mid_i as the last instance/element of LEFT the side of the array
    // DEBUG: printf("%i is the middle point of the array\n", mid_i);
    int mid_i = (first_i + last_i) / 2;

    // DEBUG: printf("Merge sorting LEFT: %i to %i\n", first_i, mid_i);
    merge_sort(first_i, mid_i, array, sorted);

    // DEBUG: printf("Merge sorting RIGHT: %i to %i\n\n", mid_i + 1, last_i);
    // perform recursion on each side of the array
    merge_sort(mid_i + 1, last_i, array, sorted);


    // pretend that array[] is actually two sub-arrays
    // LEFT sub-array is array[first_i ... mid_i]
    int i_left = first_i;
    // RIGHT sub-array is array[mid_i + 1 ... last_i]
    int i_right = mid_i + 1;
    int i_merged;
    // the merged list will be in sorted[i_merged]

    for (i_merged = first_i; i_merged <= last_i; i_merged++)
    {
        if (i_left == mid_i + 1)
        {
            sorted[i_merged] = array[i_right];
            i_right++;
        }
        else if (i_right == last_i + 1)
        {
            sorted[i_merged] = array[i_left];
            i_left++;
        }
        else if (array[i_left].votes < array[i_right].votes)
        {
            sorted[i_merged] = array[i_left];
            i_left++;
        }
        else
        {
            sorted[i_merged] = array[i_right];
            i_right++;
        }
    }
    for (i_merged = first_i; i_merged <= last_i; i_merged++)
    {
        array[i_merged] = sorted[i_merged];
    }
}