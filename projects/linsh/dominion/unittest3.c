/* -----------------------------------------------------------------------
 * unit test for isGameOver()
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <time.h>
#include <stdlib.h>

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1


int main() {
    int i, j;
    int seed = 1000;
    int numPlayer = 2;
    int maxBonus = 10;
    int p, r, handCount;
    int bonus;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int maxHandCount = 5;
    // arrays of all coppers, silvers, and golds
    int coppers[MAX_HAND];
    int silvers[MAX_HAND];
    int golds[MAX_HAND];
    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
        silvers[i] = silver;
        golds[i] = gold;
    }

    int pc;
    int cc;
    //int error_count;
    srand(time(NULL));   // should only be called once
    int error_count;
    void perror(int bval){
            if ((G.supplyCount[province] == 0) == isGameOver(&G)) {
                printf("... OK\n");
            }else{
                printf("... Error\n");
                error_count++;
            }
    }

    // clear the game state
    memset(&G, 23, sizeof(struct gameState));
    // initialize a new game
    r = initializeGame(numPlayer, k, seed, &G);
    // default card supply
    printf ("TESTING isGameOver():\n");

    for ( pc = G.supplyCount[province]; pc > -1; pc--) {
        printf("province = %d, returned = %d, expected = %d",
                G.supplyCount[province], 
                isGameOver(&G),
                (G.supplyCount[province] == 0));
        //assert((G.supplyCount[province] == 0) == isGameOver(&G));
        perror((G.supplyCount[province] == 0) == isGameOver(&G));
        G.supplyCount[province]--;
    }

    for (cc=0; cc < treasure_map; cc++){
        // clear the game state
        memset(&G, 23, sizeof(struct gameState));
        // initialize a new game
        r = initializeGame(numPlayer, k, seed, &G);
        for (j = cc; j < cc+3 || j <= treasure_map; j++){
            G.supplyCount[j] = 0;
        }
        printf("3 cards(%d, %d, %d) = 0, returned = %d, expected = %d", 
                cc, cc+1, cc+2, isGameOver(&G), 1);
        if (1 == isGameOver(&G)) {
            printf("... OK\n");
        }else{
            printf("... Error\n");
            error_count++;
        }
    }
    //int r = rand();      // returns a pseudo-random integer between 0 and RAND_MAX

    if (error_count == 0)
        printf("All tests passed!\n");
    else
        printf("Failed %d tests\n", error_count);

    return 0;
}
