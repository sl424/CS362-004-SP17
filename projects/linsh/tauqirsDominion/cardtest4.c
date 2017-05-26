/*
 * cardtest4.c
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "mine"

int main() {
    int newCards = 0;
    int trashed = 0;
    int xtraCoins = 0;
    int shuffledCards = 0;

    int i, j, m;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int remove1, remove2;
    int played;
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
     * mine higher value treasure using copper, silver, gold
     ***************************************************/
    int treasure_cards[3] = {copper, silver, gold};
    char* treasure_names[3] = {"copper", "silver", "gold"};

    for ( i = 0; i < 3; i++){
    for ( j = 0; j < 3; j++){
        /* set hand  and deck*/
	    printf("\n*** trade treasure card %d for %d\n", treasure_cards[i], treasure_cards[j]);

        handpos = 0;
        G.hand[thisPlayer][handpos] = mine;
        choice1 = 1;
        G.hand[thisPlayer][choice1] = treasure_cards[i];
        choice2 = treasure_cards[j];

        newCards = 1;
        trashed = 1;
        played = 1;
        xtraCoins = 0;

        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        rv = cardEffect(mine, choice1, choice2, choice3, &testG, handpos, &bonus);

        printf("return status = %d, expected = %d", rv, 0);
        perror(rv == 0);

        /*The Treasure you gain comes from the Supply and is put into your 
         * hand; you can play it for the same turn.*/
        printf("hand count = %d, expected = %d", 
                testG.handCount[thisPlayer], 
                G.handCount[thisPlayer] + newCards - trashed - played);
        perror(testG.handCount[thisPlayer] 
                == G.handCount[thisPlayer] + newCards - trashed - played);

        int card1 = testG.hand[thisPlayer][handpos];
        printf("new card = %d, expected = %d ", card1,choice2);
        perror(card1 == choice2);

        printf("supply count = %d, expected = %d", 
                testG.supplyCount[choice2],
                G.supplyCount[choice2]-1);
        perror(testG.supplyCount[choice2] == G.supplyCount[choice2]-1);

        printf("mine discarded: card = %d, NOT = %d",
                testG.hand[thisPlayer][handpos],
                G.hand[thisPlayer][handpos]);
        perror( testG.hand[thisPlayer][handpos] != G.hand[thisPlayer][handpos]);
    }
    }


    /* tally error count */
    if (error_count == 0)                                                      
        printf("All tests passed!\n");                                         
    else                                                                       
        printf("Failed %d tests\n", error_count);
    return 0;
}


