#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

struct ChangeResult {
    std::vector<int> coins;
    bool valid;
};


bool canFulfillCombination(const std::vector<int>& combination, const std::vector<int>& availableCoins) {
    std::vector<int> tempAvailableCoins = availableCoins;
    
    for (size_t i = 0; i < combination.size(); ++i) {
        if (combination[i] > tempAvailableCoins[i]) {
            return false; 
        }
        tempAvailableCoins[i] -= combination[i];
    }
    return true;
}

void printChange(const std::vector<int>& change, const std::vector<int>& denominations) {
    for (int i = 0; i < denominations.size(); i++) {
        std::cout << change[i] << std::endl;
    }
}

void printRemainingCoins(const std::vector<int>& availableCoins, const std::vector<int>& denominations) {
    std::cout << "\nMonedas:\n";
    for (int i = 0; i < denominations.size(); i++) {
        std::cout << denominations[i] << ": " << availableCoins[i] << std::endl;
    }
}

ChangeResult dynamicChange(int amount, const std::vector<int>& denominations, std::vector<int>& availableCoins) {
    std::vector<int> dp(amount + 1, amount + 1);
    std::vector<std::vector<int>> coinsUsed(amount + 1, std::vector<int>(denominations.size(), 0));
    dp[0] = 0;
    
    for (int i = 0; i < denominations.size(); ++i) {
        for (int j = denominations[i]; j <= amount; ++j) {
            if (dp[j - denominations[i]] + 1 < dp[j] && availableCoins[i] > 0) {
                dp[j] = dp[j - denominations[i]] + 1;
                coinsUsed[j] = coinsUsed[j - denominations[i]];
                coinsUsed[j][i]++;
            }
        }
    }
    
    if (dp[amount] > amount) {
        return {{}, false}; 
    }
    
  
    if (!canFulfillCombination(coinsUsed[amount], availableCoins)) {
        return {{}, false}; 
    }
    
  
    for (size_t i = 0; i < coinsUsed[amount].size(); ++i) {
        availableCoins[i] -= coinsUsed[amount][i];
    }

    return {coinsUsed[amount], true};
}


ChangeResult greedyChange(int amount, const std::vector<int>& denominations, std::vector<int>& availableCoins) {
    std::vector<int> change(denominations.size(), 0);

    for (int i = denominations.size() - 1; i >= 0; --i) {
        while (amount >= denominations[i] && availableCoins[i] > 0) {
            amount -= denominations[i];
            change[i]++;
            availableCoins[i]--;
        }
    }

    if (amount > 0) {
        return {{}, false};
    }

    return {change, true};
}

int main() {
    std::vector<int> denominations = {1, 2, 5, 10, 20, 50, 100, 200, 500};
    std::unordered_map<int, ChangeResult> changeMap;

    std::vector<int> availableCoins(denominations.size());
    for (int i = 0; i < denominations.size(); ++i) {
        std::cout << "Monedas de " << denominations[i] << "$ : ";
        std::cin >> availableCoins[i];
    }

    while (true) {
        int P, Q;
        std::cout << "P";
        std::cin >> P ;
        std::cout << "Q";
        std::cin >> Q;
        if (P == -1 && Q == -1) break;  

        int changeAmount = Q - P;
        ChangeResult dynamicResult;
        bool foundInHash = false;


        if (changeMap.find(changeAmount) != changeMap.end()) {
            dynamicResult = changeMap[changeAmount];
            if (dynamicResult.valid && canFulfillCombination(dynamicResult.coins, availableCoins)) {
                foundInHash = true;
                // std::cout << "Programación dinámica guardada:\n";
            
                for (size_t i = 0; i < dynamicResult.coins.size(); ++i) {
                    availableCoins[i] -= dynamicResult.coins[i];
                }
            } else {
            
                dynamicResult = dynamicChange(changeAmount, denominations, availableCoins);
                if (dynamicResult.valid) {
                    changeMap[changeAmount] = dynamicResult; 
                    // std::cout << "Programación dinámica cálculo:\n";
                } else {
                    // std::cout << "No es posible dar el cambio con la combinación actual.\n";
                }
            }
        } else {
           
            dynamicResult = dynamicChange(changeAmount, denominations, availableCoins);
            if (dynamicResult.valid) {
                changeMap[changeAmount] = dynamicResult; 
                // std::cout << "Programación dinámica cálculo:\n";
            } else {
                // std::cout << "No es posible dar el cambio.\n";
            }
        }

        printChange(dynamicResult.coins, denominations);

        // // Greedy approach
        std::cout << "\nGreedy:\n";
        ChangeResult greedyResult = greedyChange(changeAmount, denominations, availableCoins);
        printChange(greedyResult.coins, denominations);

      
        // std::cout << "\nCambio: " << changeAmount << "$\n";
        // std::cout << "\nCombinación existente: " << (foundInHash ? 1 : 0) << "\n";



        // printRemainingCoins(availableCoins, denominations);
    }

    return 0;
}
