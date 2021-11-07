#include <stdio.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"
#include "tile.h"



typedef struct{
	TextWord  *cardId; /* Id to identify card*/
	enum states _cardState; /*Identifies card position, for this struct it only keeps track of in deck or in hand*/
}deckData;

Card *Hand = { 0 }; /*Player Hand*/
Card *Field = { 0 };/* Cards on the field*/
deckData *playerDeck = { 0 }; /*Deck of ids to pull from*/
deckData *graveyard = { 0 }; /*Graveyard of ids to pull from*/
Uint8 cardsInDeck;
 Uint8 cardsInHand;
 Uint8 cardsInField;
 Uint8 cardsInGrave;

 void setCardFileData()
 {
	 cardsInDeck = 50;
	 cardsInHand = 0;
	 cardsInField = 0;
	 cardsInGrave = 0;
	 playerDeck = gfc_allocate_array(sizeof(deckData), 50);
	 graveyard = gfc_allocate_array(sizeof(deckData), 50);
	 Hand = gfc_allocate_array(sizeof(Card), 5);
	 Field = gfc_allocate_array(sizeof(Card), 50);
 }

void setDeck(char *deckName)
{
	FILE *deck;
	char *buff = { 0 };
	int x;
	
	if (!deckName)
	{
		slog("Didn't get Deck file parameter ,w,");
		free(playerDeck);
		free(Hand);
		return;
	}
	deck = fopen(deckName, "r");
	if (!deck)
	{
		slog("Didn't load the deck %s using fopen, ,w,", deck);
		fclose(deck);
		free(playerDeck);
		free(Hand);
		return;
	}
	slog("Deck Opened ^w^");
	slog_sync();

	for (x = 0; x < 50; x++)
	{
		buff = gfc_allocate_array(sizeof(char), 15);
		fscanf(deck, "%s", buff);
		playerDeck[x].cardId = buff;
		playerDeck[x]._cardState = inDeck;
		slog("Deck index %i set to ID %s", x, playerDeck[x].cardId);
	}
	slog("Deck index 2 set to ID %s", playerDeck[2].cardId);
	slog("Deck index 3 set to ID %s", playerDeck[3].cardId);
	for (int i = 0; i < 5; i++)
	{
		drawCard();
	}

	fclose(deck);
	free(buff);
	return;
}


void drawCard()
{

	if (cardsInHand >= 5)
	{
		slog("Hand full");
		return;
	}
	
	int rando,i;
	
	for (i = 0; i < 5; i++)
	{
		if (Hand[i]._cardState == inHand)
		{
			slog("Hand slot %i full moving to next position in hand", i);
			continue;
		}
		else
		{
			slog("Card placed in hand slot %i", i);
			break;
		}
	}
	if (cardsInHand > 4)
	{
		slog("Hand is full");
		return;
	}
	cardsInHand += 1;
	// Srand gotten from : https://stackoverflow.com/a/9459063	
	srand((unsigned int)SDL_GetTicks());
	
	do
	{	
		rando = rand() % 50;
		if (rando < 0)
		{
			rando *= -1;
		}
	} while (playerDeck[rando]._cardState != inDeck);

	Hand[i].cardId = playerDeck[rando].cardId;
	
	Hand[i]._cardState = inHand;
	playerDeck[rando]._cardState = inHand;

	setCardData(&Hand[i]);
	slog("Drew card %i from deck", rando);

	cardsInDeck -= 1;
	
	slog("CardName of card 0: %s", Hand[0].cardName);
	slog("CardName of card 1: %s", Hand[1].cardName);
	slog("CardName of card 2: %s", Hand[2].cardName);
	slog("CardName of card 3: %s", Hand[3].cardName);
	slog("CardName of card 4: %s", Hand[4].cardName);

	return;
}

void setCardData(Card *card)
{
	SJson *cardData, *dataBuffer;

	cardData = sj_load("cards/cardData.json");
	if (!cardData)
	{
		slog("Card Data not loaded from cards/cardData.json");
		sj_free(cardData);
		return;
	}
	dataBuffer = sj_object_get_value(cardData, card->cardId);
	if (!dataBuffer)
	{
		slog("Card Data for iD: [ %i ] not found", card->cardId);
		sj_free(cardData);
		return;
	}
	card->cardName = gfc_allocate_array(sizeof(TextLine), 1);
	card->cardText = gfc_allocate_array(sizeof(TextBlock), 1);
	
	memcpy(card->cardName, sj_get_string_value(sj_object_get_value(dataBuffer, "Name")), sizeof(TextLine));
	memcpy(card->cardText, sj_get_string_value(sj_object_get_value(dataBuffer, "Text")), sizeof(TextBlock));

	sj_get_integer_value(sj_object_get_value(dataBuffer, "cardAP"),&card->cardAP);
	sj_get_integer_value(sj_object_get_value(dataBuffer, "cardDP"), &card->cardDP);
	sj_get_integer_value(sj_object_get_value(dataBuffer, "cardHP"), &card->cardHP);
	card->cardHPcurrent = card->cardHP;

	slog("Card Name : %s", card->cardName);
	slog("Card Text : %s", card->cardText);
	slog("Card AP : %i", card->cardAP);
	slog("Card AP : %i", card->cardDP);
	slog("Card AP : %i", card->cardHP);
	sj_free(cardData);
	return;

}
void endDuel()
{
	free(Hand);
	free(playerDeck);
	return;
}

void playCard(int x, int y, int handIndex)
{
	//Entity *eCard = entity_new();
	int fieldIndex; //Keeps track of positions of cards
	for (fieldIndex = 0; fieldIndex < 50; fieldIndex++)
	{
		if (Field[fieldIndex]._cardState == inField)
		{
			continue;
		}
		else
		{
			slog("Card summoned into field index %i", fieldIndex);
			break;
		}
	}
	
	memcpy(&Field[fieldIndex], &Hand[handIndex], sizeof(Card));
	memset(&Hand[handIndex], 0, sizeof(Card));

	//Set data of card on field
	Field[fieldIndex]._cardState = inField;
	Field[fieldIndex]._cardPosition = Fight;
	setCardFight(&Field[fieldIndex]);
	Field[fieldIndex].eP = entity_new();
	Field[fieldIndex].eP->model = gf3d_model_load_plus("cardDefault","cardDefault");
	Field[fieldIndex].eP->cfieldIndex = fieldIndex;
	Field[fieldIndex].cardXpos = x;
	Field[fieldIndex].cardYpos = y;
	setCardModelLocation(x, y, Field[fieldIndex].eP);
	setTileOccupation(x, y, &Field[fieldIndex]);
	
	cardsInHand -= 1;
	cardsInField += 1;
	
	return;
}

void cardMove(int x, int y, Card *cardPointer)
{
	if (!cardPointer)return;
	setCardModelLocation(x, y, cardPointer->eP);
	removeTileOccupation(cardPointer->cardXpos, cardPointer->cardYpos);
	setTileOccupation(x, y, cardPointer);
	cardPointer->cardXpos = x;
	cardPointer->cardYpos = y;
	//cardPointer->_cardMoved = 1;
}
void setCardModelLocation(int x, int y, Entity *eCard)
{
	if (!eCard)return;
	eCard->scale.x = 2;
	eCard->scale.y = 2;
	eCard->scale.z = 2;
	eCard->position.z = -5.0f;
	eCard->position.x = 1 + x*23.0f;
	eCard->position.y = 1 + y*23.0f;
	return;
}
void setCardDefense(Card *cardpointer)
{
	if (!cardpointer)return;
	if (cardpointer->_cardPosition == Defense)return;
	slog("Set Defense");
	cardpointer->_cardPosition = Defense;
	cardpointer->eP->rotation.z = 1.57f;
	return;
}

void setCardFight(Card *cardpointer)
{
	if (!cardpointer)return;
	if (cardpointer->_cardPosition == Fight)return;
	slog("Set Fight");
	cardpointer->_cardPosition = Fight;
	cardpointer->eP->rotation.z = 0.0f;
	return;
}
void destroyCard(Entity *eCard)
{
	if (eCard)
	{
		memset(&Field[eCard->cfieldIndex],0,sizeof(Card));
		entity_free(eCard);
	}
	return;
}

void startDuel()
{
	int x, y;
	for (int i = 0; i < 2; i++)
	{
		Field[i]._cardState = inField;
		Field[i].eP = entity_new();
		Field[i].eP->model = gf3d_model_load_plus("cardDefault", "cardFace");
		Field[i].eMP = entity_new();
		Field[i].eMP->model = gf3d_model_load_plus("dino", "cardFace");
		Field[i].eP->cfieldIndex = i;
		Field[i]._cardType = leader;
		Field[i].eMP->rotation.z = 3.14f;
		x = 3;
		y = 0;
		if (checkTileOccupation(x, y) == 1)
		{
			x = 3;
			y = 6;
			Field[i].eMP->model = gf3d_model_load_plus("dino", "cardDefault");
			Field[i].eMP->rotation.z = 0.0f;
		}
		Field[i].cardXpos = x;
		Field[i].cardYpos = y;
		setCardModelLocation(x, y, Field[i].eP);
		setCardModelLocation(x, y, Field[i].eMP);
		Field[i].eMP->position.z = 5.0f;
		setTileOccupation(x, y, &Field[i]);
	}
}
void setCardHP(Card *cardpointer)
{
	for (int i = 0; i < 50; i++)
	{
		if (Field[i].cardXpos == cardpointer->cardXpos && Field[i].cardYpos == cardpointer->cardYpos)
		{
			Field[i].cardHPcurrent = cardpointer->cardHPcurrent;
			return;
		}
	}
}