# C++ Blackjack CLI

A simple single-player Blackjack game in C++.

Designed with the goal to train standard strategy and card counting.

Might run some Monte Carlo on it in the future.
## Rules

- The goal is to have a hand value closer to 21 than the dealer without going over.
- Number cards are worth their face value, face cards (J, Q, K) are worth 10, and Aces can be 1 or 11.
- Both player and dealer start with two cards. One of the dealer's cards is hidden.
- Player can choose to **Hit** (draw a card) or **Stand** (keep current hand).
- Dealer must hit until reaching at least 17. Dealer hits a soft 17.
- **Bust**: If a hand exceeds 21, it loses immediately.

## Payouts

- **Natural Blackjack:** Two-card 21 pays 3:2
- **Normal win:** 1:1
- **Push (tie):** bet is returned
- **Loss:** bet is lost

## Build & Run

```bash
make
./blackjack
```
