#ifndef CARD_H_
#define CARD_H_

#include "gfc_types.h"
#include "simple_json.h"

typedef struct
{
	Uint8 _cardState; /*Keeps track of state of card, in hand or in play*/
	SJString cardName; /* Card Name*/
	SJString cardText; /* Text of a card*/
	Uint32 cardId; /* Id to identify card*/
	Uint8 cardType; /* Card Type , Monster or Magic*/
	Uint16 cardAttribute; /*Attribute of card*/
	Uint16 cardAP; /*Attack value of a card*/
	Uint16 cardDP; /*Defense value of a card*/
	Uint16 cardHP; /*Health value of a card*/
	Uint16 cardCost; /*Summoning cost of a card*/
	

}Card;


#endif