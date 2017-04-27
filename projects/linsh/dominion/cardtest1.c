/*
 * cardtest1.c
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "smithy"

int main() {
    int newCards = 0;
    int discarded = 0;
    int xtraCoins = 0;
    int shuffledCards = 0;

    int i, j, m;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int remove1, remove2;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
        sea_hag, tribute, smithy, council_room};

    int error_count = 0;
    void perror(int bval){
        if (bval) {
            printf("... OK\n");
        }else{
            printf("... Error\n");
            error_count++;
        }
    }

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

    /***************************************************
     * draw 3 cards 
     ***************************************************/
	printf("draw 3 cards\n");
	choice1 = 1;
    /* set hand */
    handpos = 0;
    G.hand[thisPlayer][handpos] = smithy;
	newCards = 3;
    discarded = 1;
	xtraCoins = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("hand count = %d, expected = %d", 
            testG.handCount[thisPlayer], 
            G.handCount[thisPlayer] + newCards - discarded);
	perror(testG.handCount[thisPlayer] 
            == G.handCount[thisPlayer] + newCards - discarded);

	printf("deck count = %d, expected = %d", 
            testG.deckCount[thisPlayer], 
            G.deckCount[thisPlayer] - newCards);
	perror(testG.deckCount[thisPlayer] 
            == G.deckCount[thisPlayer] - newCards);

    printf("smithy played: card = %d, NOT = %d",
            testG.hand[thisPlayer][handpos], 
            G.hand[thisPlayer][handpos]);
    perror( testG.hand[thisPlayer][handpos] != G.hand[thisPlayer][handpos]);

    if (error_count == 0)                                                      
        printf("All tests passed!\n");                                         
    else                                                                       
        printf("Failed %d tests\n", error_count);
    return 0;
}


