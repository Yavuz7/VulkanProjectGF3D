#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"

void setCardData(char *id,Card *deck)
{
	SJson *cardData,*dataBuffer;
	
	cardData = sj_load("cards/cardData.json");
	if (!cardData)
	{
		slog("Card Data not loaded from cards/cardData.json");
		return;
	}	
	dataBuffer = sj_object_get_value(cardData, id);



}
