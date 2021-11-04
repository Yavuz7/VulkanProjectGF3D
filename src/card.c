#include <stdio.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"



typedef struct{
	TextWord  *cardId; /* Id to identify card*/
	enum states _cardState; /*Identifies card position, for this struct it only keeps track of in deck or in hand*/
}deckData;

 Card *Hand = { 0 };
 Uint16 cardsInDeck = 50;
 deckData *playerDeck = { 0 };
 Uint8 cardsInHand = 0;



void setDeck(char *deckName)
{
	FILE *deck;
	char *buff = { 0 };
	int x;

	playerDeck = gfc_allocate_array(sizeof(deckData), 50);
	Hand = gfc_allocate_array(sizeof(Card), 5);
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
			cardsInHand += 1;
			break;
		}
	}

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
		return;
	}
	dataBuffer = sj_object_get_value(cardData, card->cardId);
	if (!dataBuffer)
	{
		slog("Card Data for iD: [ %i ] not found", card->cardId);
		//sj_free(cardData);
		sj_free(dataBuffer);
		return;
	}
	card->cardName = gfc_allocate_array(sizeof(TextLine), 1);
	card->cardText = gfc_allocate_array(sizeof(TextBlock), 1);

	//card->cardName = sj_get_string_value(sj_object_get_value(dataBuffer, "Name"));	
	memcpy(card->cardName, sj_get_string_value(sj_object_get_value(dataBuffer, "Name")), sizeof(TextLine));
	//card->cardText = sj_get_string_value(sj_object_get_value(dataBuffer, "Text"));
	memcpy(card->cardText, sj_get_string_value(sj_object_get_value(dataBuffer, "Text")), sizeof(TextBlock));
	slog("Card Name : %s", card->cardName);
	slog("Card Text : %s", card->cardText);
	sj_free(cardData);
	return;

}
