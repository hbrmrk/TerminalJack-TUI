![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

# 🃏 TerminalJack-TUI

A sleek, cross-platform **Blackjack** game built with **C++20** and the **FTXUI** library. Experience a modern, graphical-style card game directly in your terminal!

## ✨ Features
- ♠️ **Full 52-card deck** with hardware-seeded shuffling.
- ❤️ **Dynamic Coloring:** Hearts and Diamonds are rendered in red for a classic look.
- 💰 **Chip & Betting System:** Start with 1000 chips and manage your bankroll.
- 🃏 **Smart Ace Handling:** Automatically counts as 1 or 11 based on your hand.
- 🤖 **Dealer AI:** Follows casino standards (stands on all 17s).
- 🚀 **Automatic Payouts:** Natural Blackjacks (first two cards = 21) pay **3:2**.
- 💻 **Cross-Platform:** Native support for Arch Linux and Windows.

## 🚀 Gameplay
1. **Bet:** Adjust your wager using the **Bet +50** / **Bet -50** buttons.
2. **Deal:** Start the round.
3. **Hit:** Draw another card to get closer to 21.
4. **Stand:** Hold your current value and let the Dealer play.
5. **Win:** If your total is higher than the Dealer's (or the Dealer busts), you win!

## 🛠️ Build & Installation

### Prerequisites
- **CMake** (3.14 or higher)
- **C++20** compatible compiler (GCC, Clang, or MSVC)
- A terminal with **Unicode/UTF-8** support (for card symbols)

### Building from Source
```bash
# Clone the repository
git clone [https://github.com/hbrmrk/TerminalJack-TUI.git](https://github.com/hbrmrk/TerminalJack-TUI.git)
cd TerminalJack-TUI

# Configure and build
mkdir build && cd build
cmake ..
cmake --build .
