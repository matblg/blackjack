#ifndef TYPES_H
#define TYPES_H

enum class Action { Hit, Stand, DoubleDown, Split };

struct GameState {
    int playerHandValue;
    bool isSoftHand;
    int dealerUpCardValue;
    bool canSplit;
    bool canDouble;
};

#endif