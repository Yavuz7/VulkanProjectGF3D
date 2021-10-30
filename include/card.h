#ifndef CARD_H_
#define CARD_H_

#include "gfc_types.h"
#include "gf3d_model.h"
#include "simple_json.h"

typedef struct
{
	/*Card States: 0 = In deck, 1 = In hand*/
	/*Keeps track of state of card, in hand or in play*/
	Uint8 _cardState; 

	char  *cardName; /* Card Name*/
	char *cardText; /* Text of a card*/
	Model *cardModel; /*Model of card(taken from file)*/
	char  *cardId; /* Id to identify card*/
	Uint8 cardType; /* Card Type , Monster or Magic*/
	Uint16 cardAttribute; /*Attribute of card*/
	Uint16 cardAP; /*Attack value of a card*/
	Uint16 cardDP; /*Defense value of a card*/
	Uint16 cardHP; /*Health value of a card*/
	Uint16 cardCost; /*Summoning cost of a card*/
	

}Card;

/**
 *@Brief Loads character data of card from card ID, called when card is drawn
 *@Param ID number of card
*/
void *setCardData(Card *card);

/*
*@Brief Loads card ids from txt file and puts it into a array of cards using setCardData
*@Return Array of cards
*/
void setDeck(char *deckName);

/*
*@Brief Adds card to hand array
*@Deck card is drawn from
*/
void drawCard(Card *deck);

#endif