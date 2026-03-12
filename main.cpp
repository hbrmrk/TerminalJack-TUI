#include <vector>
#include <string>
#include <algorithm>
#include <random>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "Card.h"

using namespace std;
using namespace ftxui;

// --- 1. VALUE CALCULATOR ---
int calculateHandValue(const vector<Card>& hand) {
    if (hand.empty()) return 0;
    int sum = 0;
    int acesCount = 0;
    for (const Card& card : hand) {
        int cardValue = card.getValue();
        if (cardValue == 1) { sum += 11; acesCount++; }
        else if (cardValue >= 11 && cardValue <= 13) { sum += 10; }
        else { sum += cardValue; }
    }
    while (sum > 21 && acesCount > 0) { sum -= 10; acesCount--; }
    return sum;
}

// --- 2. DEALER FUNCTION ---
void dealCard(vector<Card>& deck, vector<Card>& hand) {
    if (!deck.empty()) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
}

// --- 3. HELPER FUNCTION TO COLORIZE CARDS ---
Element renderCard(const Card& card) {
    auto cardText = text(card.toString());
    // Ha káró vagy kör, pirosra színezi
    if (card.getSuit() == "Hearts" || card.getSuit() == "Diamonds") {
        cardText = cardText | color(Color::Red);
    }
    return border(cardText);
}

int main() {
    random_device rd;
    mt19937 g(rd());

    vector<Card> deck;
    vector<Card> playerHand;
    vector<Card> dealerHand;

    string gameStatus = "Welcome! Set your bet and press Deal!";
    bool playing = false;

    int balance = 1000;
    int currentBet = 100;
    string deal_label;

    auto screen = ScreenInteractive::TerminalOutput();

    // --- STAND LOGIC ---
    auto standLogic = [&]() {
        if (!playing) return;
        playing = false;

        int pScore = calculateHandValue(playerHand);
        int dScore = calculateHandValue(dealerHand);

        while (dScore < 17) {
            dealCard(deck, dealerHand);
            dScore = calculateHandValue(dealerHand);
        }

        if (dScore > 21 || pScore > dScore) {
            gameStatus = "YOU WIN! (+" + to_string(currentBet) + " chips)";
            balance += currentBet * 2;
        }
        else if (pScore < dScore) {
            gameStatus = "Dealer wins! You lost your bet.";
        }
        else {
            gameStatus = "Push! Your bet is returned.";
            balance += currentBet;
        }

        if (balance <= 0) gameStatus += " Out of chips! (Press Deal to restart)";
        if (currentBet > balance) currentBet = balance;
    };

    // --- DEAL LOGIC ---
    auto dealLogic = [&]() {
        if (balance <= 0) {
            balance = 1000;
            currentBet = 100;
        }

        balance -= currentBet;
        playing = true;

        deck.clear(); playerHand.clear(); dealerHand.clear();
        gameStatus = "Press 'Hit' to draw a card, or 'Stand' to hold!";

        // Angol színek!
        vector<string> suits = {"Spades", "Hearts", "Diamonds", "Clubs"};
        for (const string& suit : suits)
            for (int value = 1; value <= 13; ++value)
                deck.emplace_back(value, suit);

        shuffle(deck.begin(), deck.end(), g);

        dealCard(deck, playerHand); dealCard(deck, playerHand);
        dealCard(deck, dealerHand); dealCard(deck, dealerHand);

        if (calculateHandValue(playerHand) == 21) {
            playing = false;
            int winnings = currentBet * 2.5;
            gameStatus = "BLACKJACK! You win instantly! (+" + to_string(winnings - currentBet) + " chips)";
            balance += winnings;
        }
    };

    // --- HIT LOGIC ---
    auto hitLogic = [&]() {
        if (!playing) return;
        dealCard(deck, playerHand);

        int score = calculateHandValue(playerHand);
        if (score > 21) {
            gameStatus = "Bust! Dealer wins.";
            playing = false;
            if (balance <= 0) gameStatus += " Out of chips!";
            if (currentBet > balance) currentBet = balance;
        } else if (score == 21) {
            standLogic();
        }
    };

    // --- BUTTONS ---
    auto btn_deal = Button(&deal_label, dealLogic);
    auto maybe_deal = Maybe(btn_deal, [&] { return !playing; });

    auto btn_bet_up = Button("Bet +50", [&]{ if(currentBet + 50 <= balance) currentBet += 50; });
    auto maybe_bet_up = Maybe(btn_bet_up, [&] { return !playing; });

    auto btn_bet_down = Button("Bet -50", [&]{ if(currentBet - 50 > 0) currentBet -= 50; });
    auto maybe_bet_down = Maybe(btn_bet_down, [&] { return !playing; });

    auto btn_hit = Button("Hit", hitLogic);
    auto maybe_hit = Maybe(btn_hit, &playing);

    auto btn_stand = Button("Stand", standLogic);
    auto maybe_stand = Maybe(btn_stand, &playing);

    auto btn_quit = Button("Quit", screen.ExitLoopClosure());

    auto buttons = Container::Horizontal({
        maybe_deal, maybe_bet_down, maybe_bet_up,
        maybe_hit, maybe_stand,
        btn_quit
    });

    // --- RENDERER ---
    auto renderer = Renderer(buttons, [&] {
        deal_label = "Deal (Bet: " + to_string(currentBet) + ")";

        Elements playerCardElements, dealerCardElements;

        if (playerHand.empty()) playerCardElements.push_back(border(text(" ? ")));
        else for (const auto& card : playerHand) playerCardElements.push_back(renderCard(card));

        if (dealerHand.empty()) dealerCardElements.push_back(border(text(" ? ")));
        else if (playing) {
            dealerCardElements.push_back(renderCard(dealerHand.front()));
            dealerCardElements.push_back(border(text("[ Hidden ]")));
        } else {
            for (const auto& card : dealerHand) dealerCardElements.push_back(renderCard(card));
        }

        return vbox({
            text("=== TERMINALJACK ===") | bold | center,
            separator(),

            hbox({
                text("Balance: " + to_string(balance) + " chips ") | bold | color(Color::Green),
                text(" | Current bet: " + to_string(currentBet) + " chips") | color(Color::Cyan)
            }) | center,
            separator(),

            text("Dealer's hand: (Value: " + (!playing && !dealerHand.empty() ? to_string(calculateHandValue(dealerHand)) : "?") + ")"),
            hbox(dealerCardElements),
            separator(),

            text("Your hand: (Value: " + (playerHand.empty() ? "0" : to_string(calculateHandValue(playerHand))) + ")"),
            hbox(playerCardElements),
            separator(),

            text(gameStatus) | bold | color(Color::Yellow),
            separator(),

            buttons->Render() | center
        }) | border;
    });

    screen.Loop(renderer);
    return 0;
}