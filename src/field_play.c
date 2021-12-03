#include "field_play.h"

Uint8 player1Resource, player2Resource;

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

void cardMove(int x, int y, Card *cardP)
{
	if (!cardP)return;
	setCardModelLocation(x, y, cardP->eP);
	if (cardP->eMP)
	{
		setCardModelLocation(x, y, cardP->eMP);
		cardP->eMP->position.z = 5.0f;
	}
	removeTileOccupation(cardP->cardXpos, cardP->cardYpos);
	setTileOccupation(x, y, cardP->listReference);
	cardP->cardXpos = x;
	cardP->cardYpos = y;
	//cardPointer->_cardMoved = 1;
}

int cardFight(Card *attacker, Card *defender)
{
	if (!attacker)return;
	slog("%s (AP%i ,DP %i,HP %i)", defender->cardName, defender->cardAP, defender->cardDP, defender->cardHPcurrent);
	//Attacking into a attack monster
	if (defender->_cardPosition == Fight)
	{
		defender->cardHPcurrent = defender->cardHPcurrent - (attacker->cardAP - (defender->cardDP / 2));
		if (defender->cardHPcurrent <= 0)
		{
			slog("Defender Dead");
			return 0; // Victor attacker
		}
		attacker->cardHPcurrent = attacker->cardHPcurrent - (defender->cardAP - (attacker->cardDP / 2));
		if (attacker->cardHPcurrent <= 0)
		{
			slog("Attacker Dead, Defender HP %i", defender->cardHPcurrent);
			return 1; // Victor defender
		}
		slog("Attacker Hp: %i, Defender HP: %i", attacker->cardHPcurrent, defender->cardHPcurrent);
		return 2; //no victor
	}
	if (defender->_cardPosition == Defense)
	{
		defender->cardHPcurrent = defender->cardHPcurrent - (attacker->cardAP - defender->cardDP);
		if (defender->cardHPcurrent <= 0)
		{
			slog("Defender Dead");
			return 0; // Victor attacker
		}
		slog("Defender HP %i", defender->cardHPcurrent);
		return 2;
	}
}

void generateResource(int player)
{
	if (player == 1)
	{
		player1Resource += 3;
	}
	if (player == 2)
	{
		player2Resource += 3;
	}
}