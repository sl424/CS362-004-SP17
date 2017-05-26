/* -----------------------------------------------------------------------
 * unit test for numHandCards() in dominion.c
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1
#define KNUM 10

int main() {
    int i;
    int seed = 1000;
    int numPlayer = 2;
    int maxBonus = 10;
    int p, r, handCount;
    int rs;
    int bonus;
    int k[10] = {adventurer, council_room, feast, gardens, mine, 
                 remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int maxHandCount = 5;

    // use these two for any initialization and tracking
    int cards_avail;
    int rc;
    int error_count;

    // clear the game state
    memset(&G, 23, sizeof(struct gameState));   
    // initialize a new game
    r = initializeGame(numPlayer, k, seed, &G); 
    // default card supply
    printf("TESTING numHandCount():\n");

    /* test each player */
    for (p = 0; p < numPlayer; p++) {
        cards_avail = G.handCount[p];
        G.whoseTurn = p;
        /* check function and remove a card each time */
        for (i = 0; i < cards_avail; i++){
            printf("player %d: returned = %d, expected = %d ", 
                    p,  numHandCards(&G), G.handCount[p]);
            //assert(numHandCards(&G) == G.handCount[p]);
            if (numHandCards(&G) == G.handCount[p]) {
                printf("... OK\n");
            }else{
                printf("... Error\n");
                error_count++;
            }

            // change the state and check again.
            G.handCount[p]--;
        }
        endTurn(&G);
    }

    if (error_count == 0)                                                      
        printf("All tests passed!\n");                                         
    else                                                                       
        printf("Failed %d tests\n", error_count);

    return 0;
}
