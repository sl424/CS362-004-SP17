/*
 * cardtest2.c
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "remodel"

int main() {
    int newCards = 0;
    int played = 0;
    int xtraCoins = 0;
    int shuffledCards = 0;

    int i, j, m;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int remove1, remove2;
    int trashed;
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
    int rv, ev;

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

    /***************************************************
     * trade a card from the hand and from the supply
     ***************************************************/
    for ( j = 0; j < 10; j++){
        /* set hand  and deck*/
        handpos = 0;
        G.hand[thisPlayer][handpos] = remodel;
        choice1 = 1;
        choice2 = k[j];

        newCards = 1;
        played = 1;
        trashed = 1;
        xtraCoins = 0;

	    printf("\n*** swap cards %d to %d\n", G.hand[thisPlayer][choice1], choice2);

        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        rv = cardEffect(remodel, choice1, choice2, choice3, &testG, handpos, &bonus);

        printf("return status = %d, expected = %d", rv, 0);
        perror(rv == 0);

        printf("hand count = %d, expected = %d", 
                testG.handCount[thisPlayer], 
                G.handCount[thisPlayer] - played - trashed);
        perror(testG.handCount[thisPlayer] 
                == G.handCount[thisPlayer] - played - trashed);

        /*If you do gain a card, it comes from the Supply and 
         * is put into your discard pile.*/

        //int card1 = testG.hand[thisPlayer][handpos];
        int card1 = testG.discard[thisPlayer][testG.discardCount[thisPlayer] -1];
        printf("new card = %d, expected = %d ", card1, choice2);
        perror(card1 == choice2);

        printf("supply count = %d, expected = %d", 
                testG.supplyCount[choice2],
                G.supplyCount[choice2]-1);
        perror(testG.supplyCount[choice2] == G.supplyCount[choice2]-1);

        printf("discard count = %d, expected = %d", 
                testG.discardCount[thisPlayer], 
                G.discardCount[thisPlayer] + newCards);
        perror(testG.discardCount[thisPlayer] 
                == G.discardCount[thisPlayer] + newCards);

        printf("adventurer discarded: card = %d, NOT = %d",
                testG.hand[thisPlayer][handpos],
                G.hand[thisPlayer][handpos]);
        perror( testG.hand[thisPlayer][handpos] != G.hand[thisPlayer][handpos]);
    }


    /* tally error count */
    if (error_count == 0)                                                      
        printf("All tests passed!\n");                                         
    else                                                                       
        printf("Failed %d tests\n", error_count);
    return 0;
}


