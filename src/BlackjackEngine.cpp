#include "BlackjackEngine.h"

BlackjackEngine::BlackjackEngine() 
    : deck(1), player("Agent", 1000), dealer(), currentHandIdx(0), roundOver(false){
        // one deck, 1000 chips
    };

std::vector<float> BlackjackEngine::reset(int betAmount) {
    if (deck.needsReshuffle()) {
        deck.shuffle();
    }

    currentBetAmount = betAmount;
    player.clearHand();
    dealer.clearHand();
    currentHandIdx = 0;
    roundOver = false;

    // player place bet
    if (player.getBalance() >= betAmount) {
        player.placeBet(betAmount);
    } else {
        // handle bankruptcy or reset balance logic if needed for RL training
        player.adjustBalance(1000); 
        player.placeBet(betAmount);
    }

    dealInitialCards();

    //check for natural blackjack
    bool playerBJ = player.getHand().isBlackjack();
    bool dealerBJ = dealer.getHand().isBlackjack();

    if (playerBJ || dealerBJ) {
        roundOver = true;
    }

    return getCurrentState
    ();
}

void BlackjackEngine::dealInitialCards() {
    player.hit(deck.draw());
    dealer.hit(deck.draw());
    player.hit(deck.draw());
    dealer.hit(deck.draw());
}

StepResult BlackjackEngine::step(EngineAction action) {
    StepResult result;
    result.reward = 0.0f;
    result.win = false;
    result.done = false;

    // initial blackjacks
    if (roundOver) {
        float finalReward = 0.0f;
        bool win = false;
        resolveRound(finalReward, win);
        result.reward = finalReward;
        result.win = win;
        result.done = true;
        result.state = getCurrentState();
        return result;
    }

    // play one hand
    player.setActiveHand(currentHandIdx);
    Hand& currentHand = player.getHand();
    bool turnEnded = false;


    switch (action) {
        case EngineAction::Hit: {
            player.hit(deck.draw());
            if (currentHand.isBust()) {
                turnEnded = true;
            } else if (currentHand.getValue() == 21) {
                // blackjack
                turnEnded = true; 
            }
            break;
        }
        case EngineAction::Stand: {
            turnEnded = true;
            break;
        }
        case EngineAction::Double: {
            if (player.doubleDown()) {
                player.hit(deck.draw());
                turnEnded = true; // Double down always ends turn after 1 card
            } else {
                // Illegal move! -> punishment? 
                // for now, treat as a Stand or invalid op
                turnEnded = true; 
            }
            break;
        }
        case EngineAction::Split: {
            if (player.splitHand()) {
                // Deal one card to the current hand and the new hand (at the back of the list)
                currentHand.addCard(deck.draw());
                player.getHand(player.getHandCount() - 1).addCard(deck.draw());

                // rule: Split Aces usually only get 1 card each
                if (currentHand.fromSplitAces) {
                    currentHandIdx++; 
                    if (currentHandIdx >= player.getHandCount()) {
                         playDealer();
                         roundOver = true;
                    }
                    result.state = getCurrentState
                    ();
                    result.done = roundOver;
                    if(roundOver) resolveRound(result.reward, result.win);
                    return result; 
                }
            } else {
                // Illegal split attempted
                result.reward = -1.0f; // Penalty?
            }
            break;
        }
    }
    // check if the current hand is done
    if (turnEnded) {
        currentHandIdx++;
        // if we have played all hands, finish the round
        if (currentHandIdx >= player.getHandCount()) {
            playDealer();
            roundOver = true;
            resolveRound(result.reward, result.win);
            result.done = true;
        }
    }

    result.state = getCurrentState
    ();
    return result;
}

void BlackjackEngine::playDealer() {
    while (dealer.shouldHit()) {
        dealer.hit(deck.draw());
    }
}

void BlackjackEngine::resolveRound(float& reward, bool& win) {
    reward = 0.0f;
    win = false;
    
    int dealerVal = dealer.getHand().getValue();
    bool dealerBust = dealer.getHand().isBust();
    bool dealerBJ = dealer.getHand().isBlackjack();

    for (int i = 0; i < player.getHandCount(); ++i) {
        Hand& pHand = player.getHand(i);
        int pVal = pHand.getValue();
        bool pBJ = pHand.isBlackjack();
        int bet = player.getBet(i);
        
        float handReward = 0.0f;

        if (pHand.isBust()) {
            // Player Busts: Lose bet, Reward -1
            handReward = -1.0f;
        } 
        else if (pBJ && dealerBJ) {
            // Push (Both BJ): Return bet, Reward 0
            handReward = 0.0f;
            player.adjustBalance(bet);
        } 
        else if (pBJ) {
            // Player BJ: Win 3:2, Reward 1.5
            handReward = 1.5f;
            win = true;
            player.adjustBalance(bet + static_cast<int>(bet * 1.5));
        } 
        else if (dealerBJ) {
            // Dealer BJ: Lose bet, Reward -1
            handReward = -1.0f;
        } 
        else if (dealerBust) {
            // Dealer Busts: Win 1:1, Reward 1
            handReward = 1.0f;
            win = true;
            player.adjustBalance(bet * 2);
        } 
        else if (pVal > dealerVal) {
            // Higher Value: Win 1:1, Reward 1
            handReward = 1.0f;
            win = true;
            player.adjustBalance(bet * 2);
        } 
        else if (pVal < dealerVal) {
            // Lower Value: Lose bet, Reward -1
            handReward = -1.0f;
        } 
        else {
            // Tie: Push, Reward 0
            handReward = 0.0f;
            player.adjustBalance(bet);
        }

        reward += handReward;
    }
}

std::vector<float> BlackjackEngine::getCurrentState() {
    // Note: If splitting, we return state for the current active hand.
    
    std::vector<float> state;
    
    // empty
    if(player.getHandCount() == 0) return {0, 0, 0};

    
    int idx = (currentHandIdx < player.getHandCount()) ? currentHandIdx : player.getHandCount() - 1;
    const Hand& hand = player.getHand(idx);
    
    // Player Sum
    state.push_back(static_cast<float>(hand.getValue()));

    // Dealer Up Card (Face value)
    if (!dealer.getHand().getCards().empty()) {
        state.push_back(static_cast<float>(dealer.getHand().getCards()[0].value));
    } else {
        state.push_back(0.0f);
    }

    // 3. Usable Ace (1.0 or 0.0)
    state.push_back(hand.isSoft() ? 1.0f : 0.0f);

    return state;
}