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

            //vote(j, name, ranks);

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
    // TODO



    //printf("VOTE CALLED\n");
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    //for (int i = 0; i < candidate_count; i++)
    //{
    //    printf("Voter rank %i: %i\n", i, ranks[i]);
    //}


    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    int winner;
    int loser;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i < j)
            {
                winner = ranks[i];
                loser = ranks[j];
                preferences[winner][loser] += 1;

            }
        }
    }
    //printf("\n");

    return;
}
// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    //count pairs, excluding ties
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] != preferences [j][i])
            {
                pair_count += 1;

            }
        }
    }

    int k = 0;


    while (k < pair_count)
    {
        for (int i = 0; i < candidate_count; i++) // colums of candidates array
        {
            for (int j = i + 1; j < candidate_count; j++) // rows of candidate array.
            {
                if (i != j)
                {
                    if (preferences[i][j] > preferences[j][i])
                    {
                        pairs[k].winner = i;
                        pairs[k].loser = j;

                        //strength[k] = preferences[i][j];

                        k += 1;

                        //printf("The people prefer %s over %s with a strength of %i\n", candidates[j], candidates[i], strength[k-1]);
                    }
                    else if (preferences[i][j] < preferences [j][i])
                    {
                        pairs[k].winner = j;
                        pairs[k].loser = i;

                        //strength[k] = preferences[j][i];

                        k += 1;

                        //printf("The people prefer %s over %s with a strength of %i\n", candidates[i], candidates[j], strength[k-1]);
                    }
                    else
                    {
                        //j += 1;
                    }
                }
            }
        }
    }



    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int currint;
    int checkint;
    int high;
    int highLocation;
    int hiWin;
    int hiLose;
    bool hiLocFound;
    int strength[pair_count];

    //create strength array
    for (int i = 0; i < pair_count; i++)
    {
        strength[i] = preferences[pairs[i].winner][pairs[i].loser];
    }


    // Sort strength array in descending order
    for (int i = 0; i < pair_count; i++)
    {

        currint = strength[i];
        high = currint;
        hiWin = pairs[i].winner;
        hiLose = pairs[i].loser;
        hiLocFound = false;

        for (int j = 0; j < pair_count; j++)
        {
            //printf("j = %i\n", j);
            checkint = strength[j];

            if (i < j)
            {
                //printf("we got here\n");
                if (high < checkint)
                {
                    high = checkint;
                    hiWin = pairs[j].winner;
                    hiLose = pairs[j].loser;

                    highLocation = j;
                    hiLocFound = true;
                    //printf("hiLoc = %i\n", highLocation);
                }


            }
        }

        //MAKE SWAP HERE
        if (hiLocFound)
        {
            strength[i] = high;
            pairs[highLocation].winner = pairs[i].winner;
            pairs[highLocation].loser = pairs[i].loser;

            pairs[i].winner = hiWin;
            pairs[i].loser = hiLose;


            strength[highLocation] = currint;
        }



        //Swap candidates in structure


    }

    //for (int i = 0; i < pair_count; i++)
    //{
    //    printf("%i  ", strength[i]);
    //}

    printf("Order of strength:\n");

        for (int k = 0; k < pair_count; k ++)
        {

           printf("%s over %s with strength of %i\n", candidates[pairs[k].winner], candidates[pairs[k].loser], strength[k]);
        }

    printf("\n");

    return;

}



bool cycle(int winner, int loser, bool lockedArray[MAX][MAX], int index, int train[], int searchStart, bool weFoundACycle)
{
    //void secondCycle(int winner, int loser, bool lockedArray[candidate_count][candidate_count], int index, int windex);
    printf("cycle 1 called\n");
    bool incompleteCheck = true;
    bool caboose;



    //while (incompleteCheck)
    //{

        for (int i = searchStart; i < candidate_count; i++)
        {

            //printf("%i\n", i);
            //printf("loser is %i\n", loser);

            if (lockedArray[loser][i] == true)
            {

                //printf("found!\n");

                int j = 0;
                caboose = false;

                //find and update caboose
                while (!caboose)
                {
                    //if (j < pair_count - 1)
                    //{
                        if (train[j] >= 0)
                        {
                            j += 1;
                        }
                        else
                        {
                            caboose = true;
                        }
                    //}
                    //else
                    //{
                    //    caboose = true;
                    //    return false;
                    //}

                }


                winner = loser;
                loser = i;

                train[j] = loser;

                for (int q = 0; q < pair_count + 1; q++)
                {
                    printf("%i  ", train[q]);
                }
                printf("\n");

                //if loser ever equals indexed winner, return true for cycle
                printf("loser is %i, index is %i\n", loser, index);

                if (loser == index)
                {
                    printf("cycle condition activated\n");
                   //weFoundACycle = true;
                    //printf("okay, wefoundacycle is %d\n", weFoundACycle);
                    return true;
                    printf("executed right after true\n");
                }
                printf("it executed here\n");

                //printf("cyce 1 called with %i as loser\n", loser);
                weFoundACycle = cycle(winner, loser, lockedArray, index, train, 0, weFoundACycle);

            }


        }



        // at this point, move backwards in train, one car at a time, to check for branches
        //first, reset current caboose to -1

        if (!weFoundACycle)
        {
            printf("it executed wefoundcycle\n");
            int j = 0;
            caboose = false;

            while (!caboose)
            {
                if (train[j] >= 0)
                {
                    j += 1;
                }
                else
                {
                    caboose = true;
                }
            }



            if (j <= 3) //true if we have returned all the way to the front of the trian w/ no cycles
            {
                printf("return short train false\n");
                return false;
            }
            else
            {
                searchStart = train[j - 1] + 1; //start new search one column past last lock

                train[j - 1] = -1; //reset train caboose

                loser = train[j - 2];
                winner = train[j - 3];
                printf("cycle 2 called \n");
                weFoundACycle = cycle(winner, loser, lockedArray, index, train, searchStart, weFoundACycle);
            }

        }


    printf("wefoundacycle is %d\n", weFoundACycle);


    if (!weFoundACycle)
    {
        printf("it's returning daddy false\n");
        return false;
    }
    else
    {
        printf("it's returning daddy true\n");
        return true;
    }

    //}



}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    //create tempLocked array
    //bool tempLocked[candidate_count][candidate_count];

    int train[pair_count + 1];



    //bool cycle(int winner, int loser, bool lockedArray[MAX][MAX], int index, int train[], int searchStart);

    bool cycleCreated;
    for (int i = 0; i < pair_count; i++)
    {
        // fill out train array with -1
        for (int j = 0; j < pair_count + 1; j++)
        {
            train[j] = -1;
        }

        train[0] = pairs[i].winner;
        train[1] = pairs[i].loser;
        printf("%s over %s \n", candidates[pairs[i].winner], candidates[pairs[i].loser]);



        cycleCreated = cycle(pairs[i].winner, pairs[i].loser, locked, pairs[i].winner, train, 0, false);

        if (!cycleCreated)
        {
            printf("updated locked!\n");
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        else
        {
            printf("skipped for the cycle\n");
        }

        for (int s = 0; s < candidate_count; s++)
        {
            for (int t = 0; t < candidate_count; t++)
            {
                printf("%i  ", locked[s][t]);

            }
            printf("\n");
        }


        printf("-----------------------\n");
    }


    //print locked array


    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int colCheck[pair_count];

    for (int i = 0; i < pair_count; i++)
    {
        colCheck[i] = 0;
    }

    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[i][j] == true)
            {
                colCheck[j] = 1;
            }
        }
    }

    for (int i = 0; i < pair_count; i++)
    {
        if (colCheck[i] == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

