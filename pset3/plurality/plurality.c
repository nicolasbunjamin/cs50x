#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);
void merge_sort(int i, int j, candidate array[], candidate temp[]);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {

        if (!strcmp(candidates[i].name, name))
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    candidate sorted[candidate_count];
    merge_sort(0, candidate_count - 1, candidates, sorted);
    int most_voted = candidates[candidate_count - 1].votes;

    for (int i = 0; i <= candidate_count; i++)
    {
        if (candidates[i].votes == most_voted)
        {
            printf("%s\n", candidates[i].name);
        }
    }

    return;
}

void merge_sort(int first_i, int last_i, candidate array[], candidate sorted[])
{
    if (last_i <= first_i)
        // if sub-array is only 0 or 1 instance/element long, stop recursion
    {
        return;
    }

    int mid_i = (first_i + last_i) / 2;
    // establish mid_i as the last instance/element of LEFT the side of the array
    // DEBUG: printf("%i is the middle point of the array\n", mid_i);

    merge_sort(first_i, mid_i, array, sorted);
    // DEBUG: printf("Merge sorting LEFT: %i to %i\n", first_i, mid_i);
    merge_sort(mid_i + 1, last_i, array, sorted);
    // DEBUG: printf("Merge sorting RIGHT: %i to %i\n\n", mid_i + 1, last_i);
    // perform recursion on each side of the array

    int i_left = first_i;
    int i_right = mid_i + 1;
    int i_merged;
    // pretend that array[] is actually two sub-arrays
    // LEFT sub-array is array[first_i ... mid_i]
    // RIGHT sub-array is array[mid_i + 1 ... last_i]
    // the merged list will be in sorted[i_merged]

    for (i_merged = first_i; i_merged <= last_i; i_merged++)
        // loop from the first_i to the last_i
    {
        if (i_left == mid_i + 1)
            // if the LEFT side of the array has been completely looped over and merged,
        {
            sorted[i_merged] = array[i_right];
            // just get the value from the RIGHT side of the array
            i_right++;
            // move to the next leftmost/smallest value of the RIGHT side of the array
        }
        else if (i_right == last_i + 1)
            // if the RIGHT side of the array has been completely looped over and merged,
        {
            sorted[i_merged] = array[i_left];
            // just get the value from the LEFT side of the array
            i_left++;
            // move to the next leftmost/smallest value of the LEFT side of the array
        }
        else if (array[i_left].votes < array[i_right].votes)
            // if the leftmost value of LEFT side of the array
            // is LESS THAN the leftmost value of RIGHT side of the array
        {
            sorted[i_merged] = array[i_left];
            // get the value from the LEFT side of the array
            i_left++;
            // move to the next leftmost/smallest value of the LEFT side of the array
        }
        else // if (array[i_right].votes < array[i_left].votes)
            // if the leftmost value of the RIGHT side of the array
            // is LESS THAN the leftmost value of the LEFT side of the array
        {
            sorted[i_merged] = array[i_right];
            // get the value from the RIGHT side of the array
            i_right++;
            // move to the next leftmost/smallest value of the RIGHT side of the array
        }
    }
    for (i_merged = first_i; i_merged <= last_i; i_merged++)
        // loop from the first_i to the last_i
    {
        array[i_merged] = sorted[i_merged];
        // replace the original array with the merged array
    }
}