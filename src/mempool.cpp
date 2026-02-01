#ifndef MEMPOOL_CPP
#define MEMPOOL_CPP
#include "validator.cpp"
#include <vector>
using namespace std;

class Mempool {
public:
    vector<Transaction> transactions;
    set<pair<string, int>> spent_utxos;
    
    bool add_transaction(Transaction tx, UTXOManager& utxo_mgr) {
        string error;
        if (Validator::validate(tx, utxo_mgr, spent_utxos, error)) {
            transactions.push_back(tx);
            for (auto& in : tx.inputs) {
                spent_utxos.insert({in.prev_tx, in.index});
            }
            return true;
        }
        cout << "Rejected : " << error << endl;
        return false;
    }
    void clear_mempool() {
        transactions.clear();
        spent_utxos.clear();
    }
};

#endif
