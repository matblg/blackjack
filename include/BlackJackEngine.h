#pragma once
#include <vector>
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"

enum class EngineAction {
    Hit,
    Stand,
    Double,
    Split
};

struct StepResult {
    // game state for machine learning [PlayerSum, DealerUpCard, UsableAce]
    std::vector<float> state;   
    // reward gained in this step
    float reward;           
    // is the game over?    
    bool done;  
    // did the agent win? used to calculate winrate                
    bool win;                   
};

class BlackjackEngine {
    private:
        Deck deck;
        Player player;
        Dealer dealer;
        int currentHandIdx;
        bool roundOver;
        void playDealer();
        float calculateSettlement();
    
    public:
        BlackjackEngine();

        // starts a new round, places bet, deals cards
        // returns the initial state
        std::vector<float> reset(int betAmount = 10);

        // advance the round by one action
        StepResult step(EngineAction action);

        // returns current state [PlayerSum, DealerUpCard, UsableAce]
        std::vector<float> getState() const;

        //setter-getter
        const Player& getPlayer() const { return player; }
        const Dealer& getDealer() const { return dealer; }
        int getCurrentHandIndex() const { return currentHandIdx; }
};