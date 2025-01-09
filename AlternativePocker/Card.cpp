#include "Card.h"
#include "GlobalConstants.h"

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
			card._cardString = CardToString(card._card);
			cardsDeck[idx++] = card;
		}
	}
}

std::string CardToString(card_type card)
{
	std::string result = "";

	switch (card & Pip::PipMask)
	{
	case  Pip::A:
		result.append("A");
		break;

	case Pip::K:
		result.append("K");
		break;

	case Pip::Q:
		result.append("Q");
		break;

	case Pip::J:
		result.append("J");
		break;

	case Pip::N10:
		result.append("10");
		break;

	case Pip::N9:
		result.append("9");
		break;

	case Pip::N8:
		result.append("8");
		break;

	case Pip::N7:
		result.append("7");
		break;
	default:
		break;
	}

	switch (card & Suit::SuitMask)
	{
	case Suit::Hearts:
		result.append("H");
		break;

	case Suit::Diamonds:
		result.append("D");
		break;

	case Suit::Clubs:
		result.append("C");
		break;

	case Suit::Spades:
		result.append("S");
		break;
	default:
		break;
	}

	return result;
}
