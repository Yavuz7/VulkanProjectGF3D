#ifndef CARD_H_
#define CARD_H_

#include "gfc_types.h"
#include "gfc_text.h"
#include "gf3d_model.h"
#include "simple_json.h"
#include "gfc_list.h"
#include "entity.h"

typedef struct
{
	TextBlock *cardText; /* Text of a card*/
	TextLine *cardName; /* Card Name*/
	TextLine *imageFileName; /*String of file name*/
	TextWord *cardId; /* Id to identify card*/
	Entity *eP; /*Pointer to associated entity*/
	Entity *eMP;/*Pointer to monster entity*/

	Uint16 cardAttribute; /*Attribute of card*/
	Uint16 cardAP; /*Attack value of a card*/
	Uint16 cardDP; /*Defense value of a card*/
	Uint16 cardHP; /*Health value of a card*/
	int cardHPcurrent; /*Current HP of a card*/
	Uint16 cardCost; /*Summoning cost of a card*/
	
	Uint8 listReference; /*Number that correlates to its list reference*/
	Uint8 fieldReference; /*Number that correlates to it's position on the field*/
	Uint8 cardXpos;/*x position of card*/
	Uint8 cardYpos;/*y position of card*/
	Uint8 _cardMoved;/*Checks if card moved*/
	Uint8 _cardOwner;/*Owner of card 1 = player1, 2 = player2*/
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

Entity newCard();
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
*@Param Deck to draw cards from
*@Param Hand to add cards to
*/
void drawCard(List *deck, List *Hand);

/*
*@Brief Cleans up everything
*/

void endDuel();

/*
*@Brief Starts duel by summoning 2 players Deck Leaders
*/

void startDuel();
/*
*@Brief Plays a card at index x y 
*@Param X position on grid
*@Param Y position on grid
*@Param Active player
*/
void playCard(int x, int y, int handIndex, int player);

/*
*@Brief setCard model location stuff
*@Param X coordinate the model is moving to
*@Param Y coordinate the model is moving to
*@Param Entity that is moving
*@Param Player to decide card rotation
*/
void setCardModelLocation(int x, int y, Entity *eCard, int player);

/*
*@Brief Destroys card and entity
*@Param Card to be destroyed
*/
void destroyCard(void *Cardp);

/*
*@Brief Loads deck data 
*@Param Deck list for card data to be loaded into
*@Param Deck file for data to be loaded from, json
*/

void loadDeck(List *deck, char *deckname);

/*
*@Brief Returns card pointer from int data 
*@Param Int that represents a card in data
*/

Card *getCardPointer(void *p);

/*
*@Brief getter function of player hand
*@Param Int player
*@Return Playyer list
*/
List *getPlayerHand(int player);
/*
*@Brief Resets movement of all the cards on the field
*/

void resetCardMoves();

/*
*@Brief Draws cards until hands have 5 cards
*/

void refillHands();

/*
*@Brief Gets the image file location
*@Param Player Hand to get card from
*@Param Index of card
*@Returns string with image location
*/
char *getImageFromData(int player, int index);
/*
*@Brief Gets the Name
*@Param Player Hand to get card from
*@Param Index of card
*@Returns string with image hand
*/
char *getNameFromData(int player, int index);

#endif