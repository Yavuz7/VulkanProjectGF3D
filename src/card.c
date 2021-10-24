#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"

void setCardData(char *id,Card *card)
{
	SJson *cardData,*dataBuffer;
	
	cardData = sj_load("cards/cardData.json");
	if (!cardData)
	{
		slog("Card Data not loaded from cards/cardData.json");
		return;
	}	
	dataBuffer = sj_object_get_value(cardData, id);
	if (!dataBuffer)
	{
		slog("Card Data for iD: [ %s ] not found", id);
		sj_free(cardData);
		return;
	}

	card->cardName = sj_get_string_value(sj_object_get_value(dataBuffer, "Name"));
	card->cardText = sj_get_string_value(sj_object_get_value(dataBuffer, "Text"));
	slog("Card Name : %s", card->cardName);
	slog("Card Text : %s", card->cardText);
	sj_free(cardData);
	return;

}
