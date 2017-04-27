/* -----------------------------------------------------------------------
 * unit test for supplyCount() in dominion.c
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
    int cards_avail;
    int rc;
    int error_count;

    // clear the game state
    memset(&G, 23, sizeof(struct gameState));   
    // initialize a new game
    r = initializeGame(numPlayer, k, seed, &G); 
    // default card supply
    printf("TESTING supplyCount():\n");

    do {
        cards_avail = 0;
        for (i = adventurer; i <= treasure_map; i++){
            rc = supplyCount(i, &G);
            printf("%d: returned = %d, expected = %d", i, rc, G.supplyCount[i]);
            //assert(rc == G.supplyCount[i]);
            if (rc == G.supplyCount[i]) {
                printf("... OK\n");
            } else {
                printf("... Error\n");
            }
            /* decrement card supply*/
            if (G.supplyCount[i] > 0){
                cards_avail++;
                G.supplyCount[i] -= 2;
            }
        }
    } while (
        /* test until all card supply is exhausted 
         * check if the function works as expected */
        cards_avail > 0
    );

    if (error_count == 0)                                                      
        printf("All tests passed!\n");                                         
    else                                                                       
        printf("Failed %d tests\n", error_count);

    return 0;
}
