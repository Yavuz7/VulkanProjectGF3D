#include <stdio.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"



typedef struct{
	char  *cardId; /* Id to identify card*/
	Uint8 _cardState;
}deckData;

Card *Hand = { 0 };
Uint16 cardsInDeck = 50;
deckData *playerDeck = { 0 };


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
		buff = gfc_allocate_array(sizeof(char), 30);
		fscanf(deck, "%s", buff);
		playerDeck[x].cardId = buff;
		playerDeck[x]._cardState = 0;
		slog("Deck index %i set to ID %s", x, playerDeck[x].cardId);
		slog("Deck index 2 set to ID %s", playerDeck[2].cardId);
		slog("Deck index 3 set to ID %s", playerDeck[3].cardId);
	}
	drawCard();
	fclose(deck);
	return;
}


void drawCard()
{
	Card cardBuff;
	int rando,i;
	
	// Srand gotten from : https://stackoverflow.com/a/9459063	
	srand((unsigned int)SDL_GetTicks());
	do
	{	
		rando = rand() % 50;
		if (rando < 0)
		{
			rando *= -1;
		}
		slog("Random number %i", rando);
		cardBuff._cardState = playerDeck[rando]._cardState;
		slog("cardstate %i", cardBuff._cardState);

	} while (cardBuff._cardState != 0);

	cardBuff.cardId = playerDeck[rando].cardId;

	cardBuff._cardState = 1;
	playerDeck[rando]._cardState = 1;

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
	if (Hand[4]._cardState != 1)
	{
		drawCard();
	}


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
		sj_free(cardData);
		//sj_free(dataBuffer);
		return;
	}

	card->cardName = sj_get_string_value(sj_object_get_value(dataBuffer, "Name"));
	card->cardText = sj_get_string_value(sj_object_get_value(dataBuffer, "Text"));
	slog("Card Name : %s", card->cardName);
	slog("Card Text : %s", card->cardText);
	//sj_free(dataBuffer);
	sj_free(cardData);
	return;

}
