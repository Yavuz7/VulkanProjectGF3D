#include <stdio.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"
#include "tile.h"


Card *cardData = { 0 }; /*Holds all the Card Data of The Game*/

List *player1DeckList;
List *player1HandList;
List *player2DeckList;
List *player2HandList;

List *graveyardList;
List *fieldList;

 void setCardFileData()
 {

	 cardData = gfc_allocate_array(sizeof(Card), 102);

	 player1DeckList = gfc_list_new_size(50);
	 player1HandList = gfc_list_new_size(6);
	 player2DeckList = gfc_list_new_size(50);
	 player2HandList = gfc_list_new_size(6);
	 graveyardList = gfc_list_new_size(120);
	 fieldList = gfc_list_new_size(50);

	 loadDeck(player1DeckList, "cards/deck2.json");
	 loadDeck(player2DeckList, "cards/deck2.json");

	 startDuel();

	 do{
		 drawCard(player1DeckList,player1HandList);
	 } while (gfc_list_get_count(player1HandList) < 5);
	 do{
		 drawCard(player2DeckList, player2HandList);
	 } while (gfc_list_get_count(player2HandList) < 5);

	 //playCard(2, 2, 0, 1);
	 //playCard(1, 1, 2, 2);
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
	
	for (int i = 1; i <= 50; i++)
	{
		/*String conversion converted from : https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c */
		if (deck == player2DeckList)
		{
			i = i + 50;
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
		if (i <= 50)
		{
			cardData[i]._cardOwner = 1;
		}
		else
		{
			cardData[i]._cardOwner = 2;
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
		//slog("P data : %i", (int)p);
		free(stringBuffer);

		if (deck == player2DeckList)
		{
			i = i - 50;
		}
	}
	sj_free(cardDataFromFile);
	sj_free(deckIdData);
	return;
}

//Field Play
void drawCard(List *deck, List *Hand)
{
	int rando;
	// Srand modified from : https://stackoverflow.com/a/9459063

	srand((unsigned int)SDL_GetTicks());
	
	rando = rand() % gfc_list_get_count(deck);
	void *p = gfc_list_get_nth(deck, rando);
	Hand = gfc_list_append(Hand,p);
	gfc_list_delete_nth(deck, rando);
	slog("Drew Card : %s", cardData[(int)p].cardName);
	return;
}


//owo

void endDuel()
{
	return;
}

void resetCardMoves()
{
	for (int i = 0; i < gfc_list_get_count(fieldList); i++)
	{
		void *p = gfc_list_get_nth(fieldList, i);
		cardData[(int)p]._cardMoved = 0;
	}
	return;
}

void refillHands()
{
	if (gfc_list_get_count(player1HandList) < 5)
	{
		do{
			drawCard(player1DeckList, player1HandList);
		} while (gfc_list_get_count(player1HandList) < 5);
	}
	if (gfc_list_get_count(player2HandList) < 5)
	{
		do{
			drawCard(player2DeckList, player2HandList);
		} while (gfc_list_get_count(player2HandList) < 5);
	}
}


void playCard(int x, int y, int handIndex, Uint8 player)
{
	List * hand = NULL;
	if (player == 1)
	{
		hand = player1HandList;
	}
	else
	{
		hand = player2HandList;
	}
	if (!hand)return;
	
	void *p = gfc_list_get_nth(hand, handIndex);
	fieldList = gfc_list_append(fieldList, p);
	gfc_list_delete_nth(hand, handIndex);

	cardData[(int)p].fieldReference = gfc_list_get_item_index(fieldList, p);
	cardData[(int)p].cardXpos = x;
	cardData[(int)p].cardYpos = y;
	cardData[(int)p].eP = entity_new();
	cardData[(int)p].eP->model = gf3d_model_load("cardDefault");

		cardData[(int)p]._cardOwner = player;

	setCardModelLocation(x, y, cardData[(int)p].eP);
	setTileOccupation(x, y, p);
	return;
}

Card *getCardPointer(void *p)
{
	if (!p)
	{
		//slog("Card pointer not gotten");
		return NULL;
	}
	return &cardData[(int)p];
}

List *getPlayerHand(int player)
{
	if (player == 1)
	{
		return player1HandList;
	}
	if (player == 2)
	{
		return player2HandList;
	}
}
void setCardModelLocation(int x, int y, Entity *eCard)
{
	if (!eCard)return;
	eCard->scale.x = 2;
	eCard->scale.y = 2;
	eCard->scale.z = 2;
	eCard->position.z = -5.0f;
	eCard->position.x = 1 + x*XPOSITIONOFFSET;
	eCard->position.y = 47 + y*YPOSITIONOFFSET;
	
	return;
}

//Field Play
void startDuel()
{
	int x, y, index,player;
	cardData[1]._cardType = leader;
	cardData[51]._cardType = leader;
	List *deck;
	void *p;
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			x = 3;
			y = 0;
			index = 0;
			player = 1;
			deck = player1DeckList;			
		}
		else
		{
			x = 3;
			y = 6;
			index = 0;
			player = 2;
			deck = player2DeckList;
		}
		p = gfc_list_get_nth(deck, index);			
		fieldList = gfc_list_append(fieldList, p);
		gfc_list_delete_nth(deck, index);

		cardData[(int)p].fieldReference = gfc_list_get_item_index(fieldList, p);
		cardData[(int)p].cardXpos = x;
		cardData[(int)p].cardYpos = y;
		cardData[(int)p].eP = entity_new();
		cardData[(int)p].eP->model = gf3d_model_load("cardDefault");

		cardData[(int)p]._cardOwner = player;

		setCardModelLocation(x, y, cardData[(int)p].eP);
		setTileOccupation(x, y, p);

		
	}
}

void destroyCard(void *Cardp)
{
	slog("Card %s sent to grave", cardData[(int)Cardp].cardName);
	/*free(cardData[(int)Cardp].cardName);
	free(cardData[(int)Cardp].cardText);
	free(cardData[(int)Cardp].cardId);*/
	
	entity_free(cardData[(int)Cardp].eP);
	if (cardData[(int)Cardp].eMP)
	{
		entity_free(cardData[(int)Cardp].eMP);
	}
	
	void *p = gfc_list_get_nth(fieldList, cardData[(int)Cardp].fieldReference); //Using the set field Reference, gets and removes from field list
	graveyardList = gfc_list_append(graveyardList, p);
	gfc_list_delete_nth(fieldList, cardData[(int)Cardp].fieldReference);
	cardData[(int)Cardp].fieldReference = NULL;
	//memset(&cardData[(int)Cardp], 0, sizeof(Card));
	
	return;
}
