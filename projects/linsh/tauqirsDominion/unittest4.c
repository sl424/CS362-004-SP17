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
    int error_count;
    srand(time(NULL));   // should only be called once
    int buys;
    int rv, ev, hc;

    printf ("TESTING buyCard():\n");
    /* test the number of buys */
    memset(&G, 23, sizeof(struct gameState));
    r = initializeGame(numPlayer, k, seed, &G);
    hc = G.discardCount[p];
    G.numBuys = 2;
    G.coins = 100;
    for (buys = 2; buys >= 0; buys--){
        rv = buyCard(gold, &G);
        if (rv == 0)
            hc++;
        /* test the return value */
        ev = (buys > 0) ? 0 : -1;
        printf("buys = %d, returned = %d, expected = %d", buys, rv, ev);
        if (rv == ev){
            printf(" ... OK\n");
        }else{
            printf(" ... Error\n");
            error_count++;
        }

        /* test the number of cards in discard pile */
        printf("card # in discard: returned = %d, expected = %d", G.discardCount[p], hc);
        if (G.discardCount[p] == hc){
            printf(" ... OK\n");
        }else{
            printf(" ... Error\n");
            error_count++;
        }
    }

    /* test the number of gold supply */
    memset(&G, 23, sizeof(struct gameState));
    r = initializeGame(numPlayer, k, seed, &G);
    G.coins = 100;
    G.numBuys = 8;
    G.supplyCount[gold] = 8;
    for (i = G.supplyCount[gold]; i >= 0; i--){
        rv = buyCard(gold, &G);
        ev = (i > 0) ? 0 : -1;
        printf("gold = %d, returned = %d, expected = %d", i, rv, ev);
        if (rv == ev){
            printf(" ... OK\n");
        }else{
            printf(" ... Error\n");
            error_count++;
        }
    }

    /* test the wallet */
    memset(&G, 23, sizeof(struct gameState));
    r = initializeGame(numPlayer, k, seed, &G);
    G.numBuys = 8;
    for ( i = 10; i >= 0; i--) {
        G.coins = i;
        rv  = buyCard(gold, &G);
        ev = (i >= getCost(gold)) ? 0 : -1;
        printf("coins = %d, returned = %d, expected = %d", i, rv, ev);
        if (rv == ev){
            printf(" ... OK\n");
        }else{
            printf(" ... Error\n");
            error_count++;
        }
    }


    if (error_count == 0)
        printf("All tests passed!\n");
    else
        printf("Failed %d tests\n", error_count);

    return 0;
}
