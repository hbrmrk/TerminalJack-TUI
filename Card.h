//
// Created by hbrmrk on 2026. 03. 12..
//

#ifndef BLACKJACK_CARD_H
#define BLACKJACK_CARD_H

#include <string>


class Card
{
    int value;
    std::string suit;

    public:
    Card(int cValue, std::string cSuit);
    ~Card();

    void printCard() const;

    int getValue() const;
    std::string getSuit() const;

    std::string toString() const;
};


#endif //BLACKJACK_CARD_H