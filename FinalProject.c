/*EEL2161 Final Project*/
/*Game of blackjack and slots, start with $500, print results to a file.*/
#include <stdio.h>
#include <stdlib.h> //for rand();
#include <time.h> //also for rand();
#include <windows.h> //for Beep()

#define RESULT "GameResult.txt"

FILE *resultp;
int moneyTotal = 500, continuePlaying = 1, gameChosen, numberOfDecks, cards[13], sum, dealerSum, hit, bet, num0, num1, num2, i;
int cardCount; int playedBlackJack = 0;

int DrawCards();
int DeclareWinner();
int SlotSelect(); //implement

int main(void){
	time_t t;
	/* Intializes random number generator */
    srand((unsigned) time(&t));
    
    //open file
	resultp = fopen(RESULT, "w");
	//ask player which game they would like to begin with.
	printf("Which game would you like to play? 0 for Black Jack and 1 for Slots: ");
	scanf("%d", &gameChosen);
	
	while (continuePlaying == 1){ //keep looping as long as player has not quit or money has not run out.
		int reBet = 1;
		while (gameChosen == 0 && moneyTotal > 0){ //Black Jack
			if (reBet == 1){ //ask player how much they would like to bet
				printf("How much money do you want to bet? Choose from $1 to $%d: ", moneyTotal);
				scanf("%d", &bet);
				reBet == 0;
			}
			if (playedBlackJack == 0){ //if Black Jack has not been played before, initialize decks.
				printf("How many decks do you want to play with? Choose from 1 to 8. ");
				scanf("%d", &numberOfDecks);
				for (i = 0; i<13; i++){
					cards[i] = 4*numberOfDecks;
				}
				cardCount = 52*numberOfDecks;
				playedBlackJack = 1;
			}
			sum = 0; dealerSum = 0; hit = 1;
			DrawCards();
			DrawCards();
			//tell player what both parties drew.
			printf("Your current total is %d.\n", sum);
			printf("Dealer drew %d.\n", dealerSum);
			while (hit == 1){
				if (sum == 21 || dealerSum == 21 || sum > 21 || dealerSum > 21){
					DeclareWinner();
					break;
				} //declare winner if any of the main conditions are hit.
				printf("Will you hold or hit? 0 to hold, 1 to hit: ");
				scanf("%d", &hit); //ask player to hit or hold
				if (hit == 1){
					DrawCards();
					printf("Your current total is %d.\n", sum);
				} //tell player what they drew if they decided to draw.
				else if (dealerSum >= 16 && hit == 0){
					DeclareWinner();
					break;
				} //if player has decided to hold but the dealer has less than 16 in hand, keep drawing
			}
			while (hit == 0 && dealerSum < 16){
				DrawCards();
				if (dealerSum >= 16){
					DeclareWinner();
					break;
				} //if dealer decides to stop drawing, declare a winner
			}
			if (continuePlaying == 0 || moneyTotal <= 0){
				break; //if player quits or they have run out of money, break out of loop.
			}
		}
		while (gameChosen == 1 && moneyTotal > 0){ //Slots
			printf("How much do you want to bet? Choose from $1 to $%d: ", moneyTotal);
			scanf("%d", &bet); //choose bet amount
			num0 = num1 = num2 = 0;
			SlotSelect();
			fprintf(resultp, "Slots: You started with $%d this round. You bet $%d. ", moneyTotal, bet);
			moneyTotal -= bet;
			printf("You rolled %d %d %d!\n", num0, num1, num2); //show player what was rolled
			if (num0 == num1 && num1 == num2){
				moneyTotal += (bet*2); //if all equal each other, give player twice as much as they put down.
				printf("You won! You win $%d and your total is now $%d!", bet, moneyTotal);
				fprintf(resultp, "You won, leaving you with $%d.\n" ,moneyTotal);
			}
			else if (num0 == num1){
				moneyTotal += bet; //if only first two numbers match, break even
				printf("You broke even. Your total is $%d!", moneyTotal);
				fprintf(resultp, "You broke even, leaving you with $%d.\n", moneyTotal);
			}
			else{ //if losing, do nothing
				printf("You lost! You lose $%d and your total is now $%d!", bet, moneyTotal);
				fprintf(resultp, "You lost, leaving you with $%d.\n", moneyTotal);
			}
			//ask player if they want to play again, change game, or quit only if they have enough money.
			if (moneyTotal > 0){
				printf("\nDo you want to keep playing slots, change game, or quit? Type 0 to keep playing, 1 to change game, 2 to quit. ");
				int choice;
				scanf("%d", &choice);
				switch (choice){
					case 0:	gameChosen = 1; break;
					case 1: gameChosen = 0; break;
					case 2: continuePlaying = 0; break;
					default: printf("Your input was not registered. Current game will continue.\n");
				}
				printf("\n");	
			}
		}
		if (moneyTotal <= 0){
			break;
		}
	}	
	fclose(resultp);
}


int DrawCards(){
	int repeat = 1, drawnCard;
	
	while (repeat == 1 && hit == 1){
		drawnCard = rand() % 13; //select card
		if (cards[drawnCard] > 0){
			cards[drawnCard]--;
			if ((drawnCard > 0) && (drawnCard < 10)){ //drawing 2 to 10.
				printf("You drew a %d.\n", drawnCard+1);
			}
			else if(drawnCard >= 10){ //drawing royal card
				switch (drawnCard){
					case 10: printf("You drew a Jack.\n"); break;
					case 11: printf("You drew a Queen.\n"); break;
					case 12: printf("You drew a King.\n"); break;
				}
				drawnCard = 9;
			}
			else if (drawnCard == 0){ //drawing ace
				printf("You drew an Ace. Do you want it to count as a 1 or 11? ");
				scanf("%d", &drawnCard);
				if (drawnCard != 1){
					drawnCard == 11;
				}
				drawnCard--;
			}
			sum += (drawnCard + 1);
			repeat = 0;
			cardCount--;
		}
	}
	repeat = 1;
	while ((repeat == 1) && (dealerSum < 16)){ //dealer will only draw if his sum is less than 16.
		drawnCard = rand() % 13;
		if (cards[drawnCard] > 0){
			cards[drawnCard]--;
			if(drawnCard >= 10){
				drawnCard = 9;
			}
			else if (drawnCard == 0){ //make ace worth 11 for dealer.
				drawnCard = 10;
			}
			dealerSum += (drawnCard + 1);
			repeat = 0;
			cardCount--;
		}
	}
	if (cardCount < 10){ //shuffle if there are less than 10 cards in deck.
		for (i = 0; i<13; i++){
			cards[i] = 4*numberOfDecks;
		}
		cardCount = 52*numberOfDecks;
		printf("Cards have been shuffled.\n");
	}
}

DeclareWinner(){
	int choice; int shuffleChoice = 0;
	fprintf(resultp, "Black Jack: You started with $%d this round. ", moneyTotal, bet);
	if (sum == dealerSum || (sum > 21 && dealerSum > 21)){ //decide if tied
		printf("You tied! Your total was %d and the dealer's total was %d. Your new balance is $%d\n", sum, dealerSum, moneyTotal);
		fprintf(resultp, "You tied, leaving you with $%d.\n", moneyTotal);
	}
	else if (dealerSum == 21 || (dealerSum > sum && dealerSum <= 21) || sum > 21){ //decide if dealer won
		moneyTotal -= bet;
		printf("Dealer wins! Your total was %d and the dealer's total was %d. You lose $%d. Your new balance is $%d.\n", sum, dealerSum, bet, moneyTotal);
		fprintf(resultp, "You lost $%d, leaving you with $%d.\n", bet, moneyTotal);
		//play sad tune
		Beep(784, 250);
		Beep(784, 250);
		Beep(784, 250);
		Beep(622, 999);
	}
	else if (sum == 21 || (sum > dealerSum && sum <= 21) || dealerSum > 21){ //decide if player won
		moneyTotal += bet;
		printf("You win! Your total was %d and the dealer's total was %d. You win $%d. Your new balance is $%d.\n", sum, dealerSum, bet, moneyTotal);
		fprintf(resultp, "You won $%d, leaving you with $%d.\n", bet, moneyTotal);
		//play happy tune
		Beep(494, 250); //B
		Beep(494, 250); //B
		Beep(523, 250); //C
		Beep(587, 250); //D
		Beep(587, 250); //D
		Beep(523, 250); //C
		Beep(494, 250); //B
		Beep(440, 250); //A	
	}
	printf("Do you want to keep playing black jack, change game, or quit? Type 0 to keep playing, 1 to change game, 2 to quit. ");
	scanf("%d", &choice);
	//ask player what their next game choice will be.
	switch (choice){
		case 0: 
			gameChosen = 0; 
			printf("Do you want to shuffle the cards? There are currently %d cards in deck.\n", cardCount);
			printf("Type 0 to play as is or 1 to shuffle. ");
			scanf("%d", &shuffleChoice);
			if (shuffleChoice != 0){
				for (i = 0; i<13; i++){
					cards[i] = 4*numberOfDecks;
				}
				cardCount = 52*numberOfDecks;
				printf("Cards have been shuffled.\n");
			}
			break;
		case 1: gameChosen = 1; break;
		case 2: continuePlaying = 0; break;
		default: printf("Your input was not registered. Current game will continue.\n");
	}
	printf("\n");
}

SlotSelect(){ //assign random numbers to slots.
	num0 = rand() % 3;
	num1 = rand() % 3;
	num2 = rand()%3;
}
