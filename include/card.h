#ifndef CARD_H_
#define CARD_H_

#include "gfc_types.h"
#include "simple_json.h"

typedef struct
{
	//Uint8 _cardState; /*Keeps track of state of card, in hand or in play*/
	char  *cardName; /* Card Name*/
	char *cardText; /* Text of a card*/
	Uint32 cardId; /* Id to identify card*/
	Uint8 cardType; /* Card Type , Monster or Magic*/
	Uint16 cardAttribute; /*Attribute of card*/
	Uint16 cardAP; /*Attack value of a card*/
	Uint16 cardDP; /*Defense value of a card*/
	Uint16 cardHP; /*Health value of a card*/
	Uint16 cardCost; /*Summoning cost of a card*/
	

}Card;

/*
*@Brief Loads character data of card from card ID, called when card is drawn.
*@Param ID number of card
*@Param Set data of card
*/
void setCardData(char *id, Card *card);

/*
*@Brief Loads card ids from txt file and puts it into a array of cards using setCardData
*@Return Array of cards
*/

Card *getDeck(char *deckName);


#endif