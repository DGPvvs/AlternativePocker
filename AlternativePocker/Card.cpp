#include "Card.h"
#include "GlobalConstants.h"

void CardToString(Card&);

void SetUpCardDesk(Card* cardsDeck)
{
	int idx = 0;
	for (size_t suit = 0; suit < SUITS; suit++)
	{
		for (size_t pips = 0; pips < CARDS_COL; pips++)
		{
			Card card = Card();
			card._card = (pipsMatrix[0][pips] | pipsMatrix[1][pips] | suits[suit]);

			bool isSevenClubs = (card._card & (card_type)Suit::SuitMask) == Suit::Clubs;
			isSevenClubs = isSevenClubs && (card._card & Pip::PipMask) == Pip::N7;

			card._isSevenClubs = isSevenClubs;
			CardToString(card);
			cardsDeck[idx++] = card;
		}
	}
}

void CardToString(Card& card)
{
	for (int i = 0; i < 4; i++)
	{
		card._cardString[i] = '\0';
	}

	char first = '\0';
	char second = '\0';

	switch (card._card & Suit::SuitMask)
	{
	case Suit::Hearts:
		second = 'H';
		break;

	case Suit::Diamonds:
		second = 'D';
		break;

	case Suit::Clubs:
		second = 'C';
		break;

	case Suit::Spades:
		second = 'S';
		break;
	default:
		break;
	}

	switch (card._card & Pip::PipMask)
	{
	case  Pip::A:
		first = 'A';
		break;

	case Pip::K:
		first = 'K';
		break;

	case Pip::Q:
		first = 'Q';
		break;

	case Pip::J:
		first = 'J';
		break;

	case Pip::N9:
		first = '9';
		break;

	case Pip::N8:
		first = '8';
		break;

	case Pip::N7:
		first = '7';
		break;
	default:
		card._cardString[0] = '1';
		card._cardString[1] = '0';
		card._cardString[2] = second;
		break;
	}

	if (first != '\0')
	{
		card._cardString[0] = first;
		card._cardString[1] = second;
	}
}