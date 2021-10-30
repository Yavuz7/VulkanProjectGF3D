#include <stdio.h>
#include <stdlib.h>

#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"


Card *Hand[5];
Uint16 cardsInDeck = 50;
Card *playerDeck;

void *setCardData(Card *card)
{
	SJson *cardData,*dataBuffer;
	
	cardData = sj_load("cards/cardData.json");
	if (!cardData)
	{
		slog("Card Data not loaded from cards/cardData.json");
		return;
	}	
	dataBuffer = sj_object_get_value(cardData, card->cardId);
	if (!dataBuffer)
	{
		slog("Card Data for iD: [ %s ] not found", card->cardId);
		sj_free(cardData);
		return;
	}

	card->cardName = sj_get_string_value(sj_object_get_value(dataBuffer, "Name"));
	card->cardText = sj_get_string_value(sj_object_get_value(dataBuffer, "Text"));
	slog("Card Name : %s", card->cardName);
	slog("Card Text : %s", card->cardText);
	sj_free(cardData);
	return card;

}

void drawCard(Card *deck)
{
	float random;
	Card *cardBuff;
	int rando;
	do
	{
		random = gfc_crandom() * 50;
		if (random < 0)
		{
			random *= -1;
		}
		rando = (float)random;
		cardBuff = &deck[rando];
	} while (cardBuff->_cardState != 0);
	cardBuff->_cardState = 1;
	setCardData(cardBuff);
	cardsInDeck -= 1;
	slog("Random number %i", rando);
	return;
}

void setDeck(char *deckName)
{
	FILE *deck;
	char buff[256];
	int x;
	Card *cardBuff;
	playerDeck = gfc_allocate_array(sizeof(Card), 50);
	if (!deckName)
	{
		slog("Didn't get Deck file parameter ,w,");
		free(playerDeck);
		return;
	}
	deck = fopen(deckName, "r");
	if (!deck)
	{
		slog("Didn't load the deck %s using fopen, ,w,", deck);
		fclose(deck);
		free(playerDeck);
		return;
	}
	slog("Deck Opened ^w^");
	slog_sync();

	for (x = 0; x < 50; x++)
	{
			fscanf(deck, "%i", buff);
			playerDeck[x].cardId = buff[0];
			slog("Deck index %i set to ID %i", x, playerDeck[x].cardId);
	}
	fclose(deck);
	return;
}