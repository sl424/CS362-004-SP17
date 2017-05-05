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
#include<time.h>

#define REPS 10

#define TESTCARD "adventurer"

int main() {
    srand(time(NULL));
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
    int randomTreasureCard(void){
        int min =  copper;
        int max = gold;
        int r = (rand() % (max + 1 - min)) + min;
        return r;
    }

    int randomRange(int min, int max){
        int r = (rand() % (max + 1 - min)) + min;
        return r;
    }
    int nTreasures;

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	//choice1 = 1;
    //int treasure_cards[4] = {copper, silver, gold, copper};
    //char* treasure_names[4] = {"copper", "silver", "gold", "copper"};

    //for ( i = 0; i < 4; i++){
    for ( i = 0; i < REPS; i++){
        /* set hand  and deck*/
	    printf("\n*** find treasure cards\n");
        for (handpos = 0; handpos < G.handCount[thisPlayer]; handpos++){
            if (handpos == 0) 
                G.hand[thisPlayer][handpos] = adventurer;
            else
                G.hand[thisPlayer][handpos] = curse;
        }

        /*
        for (j = 0; j < G.deckCount[thisPlayer]; j++){
            G.deck[thisPlayer][j] = treasure_cards[i];
        }
        */

        /* create random n treasure card of any kind in deck*/
        nTreasures = randomRange(0, G.deckCount[thisPlayer]);
        for (j = 0; j < G.deckCount[thisPlayer]; j++){
            if (j < nTreasures)
                G.deck[thisPlayer][j] = randomTreasureCard();
            else
                G.deck[thisPlayer][j] = curse;
        }
        shuffle(thisPlayer, &G);

        newCards = nTreasures < 2 ? nTreasures : 2;
        discarded = 0;
        xtraCoins = 0;
        //if (i==3){discarded=G.deckCount[thisPlayer]-1; newCards=1;}

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

        /*
        printf("1st treasure card = %d, expected = %d ", 
                card1, treasure_cards[i]);
        perror(card1 == treasure_cards[i]);
        if(i != 3){
        printf("2nd treasure card = %d, expected = %d ", 
                card2, treasure_cards[i]);
        perror(card2 == treasure_cards[i]);
        }
        */
        for (m = 0; m < newCards; m++){
            printf("treasure card %d = %d, expected = %d,%d,%d ", m,
                    card1, copper, silver, gold);
            perror(card1 == copper || card1 == silver || card1 == gold);
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


