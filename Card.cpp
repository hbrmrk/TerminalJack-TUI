//
// Created by hbrmrk on 2026. 03. 12..
//

#include "Card.h"

#include <iostream>
#include <ostream>

Card::Card(int cValue, std::string cSuit) : value(cValue), suit(cSuit)
{}

Card::~Card()
{}

void Card::printCard() const
{
    std::cout << suit << " " << value << std::endl;
}

int Card::getValue() const
{
    return value;
}

std::string Card::toString() const
{
    std::string valStr = std::to_string(value);
    if (value == 1) valStr = "A";      // Ace
    else if (value == 11) valStr = "J"; // Jack
    else if (value == 12) valStr = "Q"; // Queen
    else if (value == 13) valStr = "K"; // King

    // Kártyaszimbólumok hozzárendelése
    std::string suitSymbol = suit;
    if (suit == "Hearts") suitSymbol = "♥";
    else if (suit == "Diamonds") suitSymbol = "♦";
    else if (suit == "Clubs") suitSymbol = "♣";
    else if (suit == "Spades") suitSymbol = "♠";

    return suitSymbol + " " + valStr;
}

std::string Card::getSuit() const
{
    return suit;
}