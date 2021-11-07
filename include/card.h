#ifndef CARD_H_
#define CARD_H_

#include "gfc_types.h"
#include "gfc_text.h"
#include "gf3d_model.h"
#include "simple_json.h"
#include "entity.h"

typedef struct
{
	TextBlock *cardText; /* Text of a card*/
	TextLine *cardName; /* Card Name*/
	TextWord *cardId; /* Id to identify card*/
	Entity *eP; /*Pointer to associated entity*/
	Entity *eMP;/*Pointer to monster entity*/

	Uint16 cardAttribute; /*Attribute of card*/
	Uint16 cardAP; /*Attack value of a card*/
	Uint16 cardDP; /*Defense value of a card*/
	Uint16 cardHP; /*Health value of a card*/
	int cardHPcurrent; /*Current HP of a card*/
	Uint16 cardCost; /*Summoning cost of a card*/
	

	Uint8 cardXpos;/*x position of card*/
	Uint8 cardYpos;/*y position of card*/
	Uint8 _cardMoved;/*Checks if card moved*/
	Uint8 _cardOwner;/*Owner of card 0 = you, 1 = not you*/
	enum states _cardState;/*Keeps track of state of card*/
	enum cardPosition _cardPosition;/*Keeps track of card position,*/
	enum cardType _cardType; /* Stores card type*/

}Card;

/*States for _cardState*/
enum states{ inDeck, inHand, inField,inGrave };
/*BattlePositions for Cards*/
enum cardPosition{Fight,Defense};
/*Card types of cards*/
enum cardType{monster,spell,trap,leader};
/*
*@Brief Loads character data of card from card ID, called when card is drawn
*@Param ID number of card
*/
void setCardData(Card *card);

/*
*@Brief Allocates memory for global variables in 
*/
void setCardFileData();
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
*@Brief Starts duel by summoning 2 players
*/


void startDuel();
/*
*@Brief Plays a card at index x y 
*@Param X position on grid
*@Param Y position on grid
*/
void playCard(int x, int y, int handIndex);

/*
*@Brief setCard model location stuff
*@Param X coordinate the model is moving to
*@Param Y coordinate the model is moving to
*@Param Entity that is moving
*/
void setCardModelLocation(int x, int y, Entity *eCard);
/*
*@Brief Move cards position and entity and sets new coordinates
*@Param X coordinate of new position
*@Param Y coordinate of new position
*@Param Pointer to card being moved
*/
void cardMove(int x, int y, Card *cardPointer);

/*
*@Brief Sets Card to defense position
*@Param Card being set to defense postion 
*/
void setCardDefense(Card *cardpointer);
/*
*@Brief Sets Card to Fight position
*@Param Card being set to Fight position
*/
void setCardFight(Card *cardpointer);

/*
*@BriefSets card hp from pointer
*@Param Card hp is being set from
*/
void setCardHP(Card *cardpointer);

/*
*@Brief Destroys card and entity
*@Param Card to be destroyed
*/
void destroyCard(Card *cardpointer);
#endif