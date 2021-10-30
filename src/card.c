#include <stdio.h>
#include <stdlib.h>

#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"


Card *Hand;
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
		slog("Card Data for iD: [ %i ] not found", card->cardId);
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
	Card cardBuff;
	int rando,i;
	
	if (!deck)
	{
		slog("Nu deck found through drawCard()");
		return;
	}

	do
	{
		rando = (int)(gfc_crandom() * 50.0);

		if (rando < 0)
		{
			rando *= -1;
		}
		slog("Random number %i", rando);
		cardBuff._cardState = deck[rando]._cardState;
		slog("cardstate %i", cardBuff._cardState);
	} while (cardBuff._cardState != 0);

	cardBuff.cardId = &playerDeck[rando].cardId;

	cardBuff._cardState = 1;
	deck[rando]._cardState = 1;

	setCardData(&cardBuff);
	slog("Drew card %i from deck", rando);
	for (i = 0; i < 5; i++)
	{
		if (Hand[i]._cardState == 1)
		{
			slog("Hand slot %i full moving to next position in hand", i);
			continue;
		}
		else
		{
			slog("Card placed in hand slot %i", i);
			Hand[i] = cardBuff;
			break;
		}
	}

	cardsInDeck -= 1;

	return;
}

void setDeck(char *deckName)
{
	FILE *deck;
	char buff[256];
	int x;
	Card *cardBuff;
	playerDeck = gfc_allocate_array(sizeof(Card), 50);
	Hand = gfc_allocate_array(sizeof(Card), 5);
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
			playerDeck[x]._cardState = 0;
			slog("Deck index %i set to ID %i", x, playerDeck[x].cardId);
	}
	drawCard(&playerDeck);
	fclose(deck);
	return;
}