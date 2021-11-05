#ifndef CARD_H_
#define CARD_H_

#include "gfc_types.h"
#include "gfc_text.h"
#include "gf3d_model.h"
#include "simple_json.h"
#include "entity.h"

typedef struct
{
	
	/*Keeps track of state of card, in hand or in play*/
	enum states _cardState; 

	TextWord *cardId; /* Id to identify card*/
	TextLine *cardName; /* Card Name*/
	TextBlock *cardText; /* Text of a card*/

	Uint8 cardType; /* Card Type , Monster or Magic*/
	Uint16 cardAttribute; /*Attribute of card*/
	Uint16 cardAP; /*Attack value of a card*/
	Uint16 cardDP; /*Defense value of a card*/
	Uint16 cardHP; /*Health value of a card*/
	Uint16 cardCost; /*Summoning cost of a card*/
	
	Uint8 cardXpos;/*x position of card*/
	Uint8 cardYpos;/*y position of card*/

}Card;

/*States for _cardState*/
enum states{ inDeck, inHand, inField,inGrave };
/*
*@Brief Loads character data of card from card ID, called when card is drawn
*@Param ID number of card
*/
void setCardData(Card *card);

/*
*@Brief Loads card ids from txt file and puts it into a array of cards using setCardData
*@Return Array of cards
*/
void setDeck(char *deckName);

/*
*@Brief Adds card to hand array
*/
void drawCard();

/*
*@Brief Cleans up everything
*/

void endDuel();

/*
*@Brief Plays a card at index x y 
*@Param X position on grid
*@Param Y position on grid
*/
void playCard(int x, int y, int handIndex);

/*
*@Brief Destroys card and entity
*@Param Card to be destroyed
*/
void destroyCard(Entity *eCard);

#endif