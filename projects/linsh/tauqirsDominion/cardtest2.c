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

#define TESTCARD "adventurer"

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

	choice1 = 1;

    int treasure_cards[4] = {copper, silver, gold, copper};
    char* treasure_names[4] = {"copper", "silver", "gold", "copper"};

    for ( i = 0; i < 4; i++){
        /* set hand  and deck*/
	    printf("\n*** find %s cards\n", treasure_names[i]);
        handpos = 0;
        G.hand[thisPlayer][handpos] = adventurer;

        for (j = 0; j < G.deckCount[thisPlayer]; j++){
            G.deck[thisPlayer][j] = treasure_cards[i];
        }

        // If you run out of cards after shuffling and still only have 
        // one Treasure, you get just that one Treasure.
        //only single treasure
        if (i == 3){
            for (j = 0; j < G.deckCount[thisPlayer]; j++){
                G.deck[thisPlayer][j] = curse;
            }
                G.deck[thisPlayer][G.deckCount[thisPlayer]-1] = copper;
        }

        newCards = 2;
        discarded = 0;
        xtraCoins = 0;
        if (i==3){discarded=G.deckCount[thisPlayer]-1; newCards=1;}

        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

        printf("hand count = %d, expected = %d", 
                testG.handCount[thisPlayer], 
                G.handCount[thisPlayer] + newCards );
        perror(testG.handCount[thisPlayer] 
                == G.handCount[thisPlayer] + newCards );

        int card1 = testG.hand[thisPlayer][testG.handCount[thisPlayer]-1];
        int card2 = testG.hand[thisPlayer][testG.handCount[thisPlayer]-2];

        printf("1st treasure card = %d, expected = %d ", 
                card1, treasure_cards[i]);
        perror(card1 == treasure_cards[i]);

        if(i != 3){
        printf("2nd treasure card = %d, expected = %d ", 
                card2, treasure_cards[i]);
        perror(card2 == treasure_cards[i]);
        }

        printf("deck count = %d, expected = %d", 
                testG.deckCount[thisPlayer], 
                G.deckCount[thisPlayer] - newCards - discarded);
        perror(testG.deckCount[thisPlayer] 
                == G.deckCount[thisPlayer] - newCards - discarded);

        printf("discard count = %d, expected = %d", 
                testG.discardCount[thisPlayer], 
                G.discardCount[thisPlayer] + discarded);
        perror(testG.discardCount[thisPlayer] 
                == G.discardCount[thisPlayer] + discarded);

        printf("adventurer played: card = %d, NOT = %d",
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


