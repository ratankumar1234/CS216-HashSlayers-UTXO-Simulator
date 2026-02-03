#ifndef UTXO_MANAGER_CPP
#define UTXO_MANAGER_CPP
#include <map>
#include <iostream>
#include <string>
#include "transaction.cpp"
using namespace std;

struct UTXO {
    double amount;
    string owner;
};

class UTXOManager {
public:
    map<pair<string, int>, UTXO> utxo_set;

    void add_utxo(string tx_id, int index, double amount, string owner) {
        utxo_set[{tx_id, index}] = {amount, owner};
    }

    void remove_utxo(string tx_id, int index) {
        utxo_set.erase({tx_id, index});
    }

    double get_balance(string owner) {
        double balance = 0;
        for (auto const& it : utxo_set) {
            auto key = it.first;
            auto utxo = it.second;
            if (utxo.owner == owner) balance += utxo.amount;
        }
        return balance;
    }

    bool exists(string tx_id, int index) {
        return utxo_set.count({tx_id, index}) > 0;
    }
};

#endif
