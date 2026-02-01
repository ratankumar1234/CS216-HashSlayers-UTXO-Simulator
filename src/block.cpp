#ifndef BLOCK_CPP
#define BLOCK_CPP

#include "mempool.cpp"
#include <algorithm>

using namespace std;

class Block {
public:
    static void mine_block(string miner_address, Mempool& mempool, UTXOManager& utxo_mgr) {
        if (mempool.transactions.empty()) {
            cout << "Mempool empty. Nothing to mine." << endl;
            return;
        }

        double total_fees = 0;
        for (auto& tx : mempool.transactions) {
            for (auto& in : tx.inputs) {
                utxo_mgr.remove_utxo(in.prev_tx, in.index);
            }
            for (size_t i = 0; i < tx.outputs.size(); ++i) {
                utxo_mgr.add_utxo(tx.tx_id, i, tx.outputs[i].amount, tx.outputs[i].address);
            }
        }

        utxo_mgr.add_utxo("coinbase_" + to_string(rand()), 0, 0.0, miner_address);
        cout << "Block mined successfully by " << miner_address << endl;
        mempool.clear_mempool();
    }
};

#endif