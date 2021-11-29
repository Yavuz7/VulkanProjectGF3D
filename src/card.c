#include <stdio.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"
#include "tile.h"


Card *cardData = { 0 }; /*Deck storage*/
Card *Field = { 0 };/* Cards on the field*/
Card *Hand = { 0 };
List *player1DeckList;
List *player1HandList;
List *fieldList;
List *player1GraveyardList;
Card *rewards = { 0 };
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
	 cardData = gfc_allocate_array(sizeof(Card), 100);
	 player1DeckList = gfc_list_new_size(50);
	 player1HandList = gfc_list_new_size(5);
	 fieldList = gfc_list_new_size(50);
	 loadDeck(player1DeckList, "cards/deck2.json");
	 do{
		 drawCard(player1DeckList);
	 } while (gfc_list_get_count(player1HandList) < 5);
 }


void loadDeck(List *deck, char *deckname)
{
	SJson *cardDataFromFile, *dataBuffer,*deckIdData,*cardIdFromFile;
	if (!deck)
	{
		slog("Didn't get deck from param wat wat");
		return;
	}
	cardDataFromFile = sj_load("cards/cardData.json");
	if (!cardDataFromFile)
	{
		slog("Card Data not loaded from cards/cardData.json");
		sj_free(cardDataFromFile);
		return;
	}
	deckIdData = sj_load(deckname);
	if (!deckIdData)
	{
		slog("Card Data not loaded from %s", deckname);
		sj_free(cardDataFromFile);
		sj_free(deckIdData);
		return;
	}
	
	for (int i = 0; i < 50; i++)
	{
		/*String conversion converted from : https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c */
		if (&cardData[49] == NULL)
		{
			i = i + 49;
		}
		char *stringBuffer = malloc(4);
		snprintf(stringBuffer,4, "%d", i);
		//slog("Test 2 : %s", stringBuffer);


		cardIdFromFile = sj_object_get_value_countdown(deckIdData,stringBuffer);
		dataBuffer = sj_object_get_value(cardDataFromFile, sj_get_string_value(cardIdFromFile));
		if (!dataBuffer)
		{
			slog("Bad card Read:<");
			free(stringBuffer);
			continue;
		}
		
		cardData[i].cardName = gfc_allocate_array(sizeof(TextLine), 1);
		cardData[i].cardText = gfc_allocate_array(sizeof(TextBlock), 1);

		memcpy(cardData[i].cardName, sj_get_string_value(sj_object_get_value(dataBuffer, "Name")), sizeof(TextLine));
		memcpy(cardData[i].cardText, sj_get_string_value(sj_object_get_value(dataBuffer, "Text")), sizeof(TextBlock));
		
		sj_get_integer_value(sj_object_get_value(dataBuffer, "cardAP"), &cardData[i].cardAP);
		sj_get_integer_value(sj_object_get_value(dataBuffer, "cardDP"), &cardData[i].cardDP);
		sj_get_integer_value(sj_object_get_value(dataBuffer, "cardHP"), &cardData[i].cardHP);
		cardData[i].cardHPcurrent = cardData[i].cardHP;
		cardData[i].listReference = i;

		void *p = i;
		deck = gfc_list_append(deck, p);
		/*
		slog("Card Name : %s", deck[i].cardName);
		slog("Card Text : %s", deck[i].cardText);
		
		slog("Card AP : %i", deck[i].cardAP);
		slog("Card AP : %i", deck[i].cardDP);
		slog("Card AP : %i", deck[i].cardHP);
			*/		
		free(stringBuffer);
		if (&cardData[50] == NULL)
		{
			i = i - 49;
		}
	}
	sj_free(cardDataFromFile);
	sj_free(deckIdData);
	return;
}

//Field Play
void drawCard(List *deck)
{
	int rando;
	// Srand modified from : https://stackoverflow.com/a/9459063	
	srand((unsigned int)SDL_GetTicks());
	
	rando = rand() % gfc_list_get_count(deck);
	void *p = gfc_list_get_nth(deck, rando);
	gfc_list_append(player1HandList,p);
	gfc_list_delete_nth(deck, rando);
	slog("Drew Card : %s", cardData[(int)p].cardName);
	for (int i = 0; i < 5; i++)
	{
		if (gfc_list_get_nth(player1HandList, i) == 0)continue;
		slog("Card ID in hand position %i : %i",i, gfc_list_get_nth(player1HandList, i));
	}
	return;
}


//owo

void endDuel()
{
	free(Hand);
	//free(playerDeck);
	return;
}

void playCard(int x, int y, int handIndex, List *hand)
{
	if (!hand)return;

	void *p = gfc_list_get_nth(hand, handIndex);
	gfc_list_append(fieldList, p);
	gfc_list_delete_nth(hand, handIndex);

	cardData[(int)p].cardXpos = x;
	cardData[(int)p].cardYpos = y;
	cardData[(int)p].eP = entity_new();
	cardData[(int)p].eP->model = gf3d_model_load_plus("cardDefault", "cardDefault");

	setCardModelLocation(x, y, cardData[(int)p].eP);
	setTileOccupation(x, y, &cardData[(int)p]);
	return;
}

//Field Play
void cardMove(int x, int y, Card *cardPointer)
{
	if (!cardPointer)return;
	setCardModelLocation(x, y, cardPointer->eP);
	if (cardPointer->eMP)
	{
		setCardModelLocation(x, y, cardPointer->eMP);
		cardPointer->eMP->position.z = 5.0f;
	}
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

//Field Play
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
		//Field[i]._cardType = leader;
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
//Field Plays
void destroyCard(Card *cardpointer)
{
	int g;
	for (int i = 0; i < 50; i++)
	{
		if (Field[i].cardXpos == cardpointer->cardXpos && Field[i].cardYpos == cardpointer->cardYpos)
		{
			entity_free(Field[i].eP);
			if (Field[i].eMP)
			{
				entity_free(Field[i].eMP);
			}
			slog("Card %s sent to grave", Field[i].cardName);
			free(Field[i].cardName);
			free(Field[i].cardText);

			free(Field[i].cardId);
			memset(&Field[i], 0, sizeof(Card));
			return;
		}
	}
}
