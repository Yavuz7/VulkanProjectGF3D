#include "simple_json.h"
#include "simple_logger.h"
#include "card.h"

void setCardData(char *id,Card *deck)
{
	SJson *cardData;
	
	cardData = sj_load("cards/cardData.json");
	if (!cardData)
	{
		slog("Card Data not loaded from cards/cardData.json");
		return;
	}	


}
