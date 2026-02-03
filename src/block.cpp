#ifndef BLOCK_CPP
#define BLOCK_CPP

#include "mempool.cpp"
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

class Block {
public:
    static void mine_block(string miner_address, Mempool& mempool, UTXOManager& utxo_mgr) { 
        if (mempool.transactions.empty()) {
            cout << "Mempool empty. Nothing to mine." << endl;
            return;
        } 
        sort(mempool.transactions.begin(), mempool.transactions.end(), 
            [](const Transaction& a, const Transaction& b) { return a.fee > b.fee; });

        double total_block_fees = 0;
        int tx_count = 0;
        vector<Transaction> mined_transactions;

        cout << "Mining block..." << endl; 
        for (auto it = mempool.transactions.begin(); it != mempool.transactions.end() && tx_count < 5; ++it) {
            Transaction& tx = *it; 
            double input_sum = 0;
            for (auto& in : tx.inputs) { 
                input_sum += utxo_mgr.utxo_set[{in.prev_tx, in.index}].amount;
            }
            double output_sum = 0;
            for (auto& out : tx.outputs) {
                output_sum += out.amount;
            }
            double tx_fee = input_sum - output_sum;
            total_block_fees += tx_fee;
            for (auto& in : tx.inputs) {
                utxo_mgr.remove_utxo(in.prev_tx, in.index);
            }
            for (size_t i = 0; i < tx.outputs.size(); ++i) {
                utxo_mgr.add_utxo(tx.tx_id, (int)i, tx.outputs[i].amount, tx.outputs[i].address);
            }
            mined_transactions.push_back(tx);
            tx_count++;
        }
        string coinbase_id = "coinbase_" + to_string(rand() % 10000);
        utxo_mgr.add_utxo(coinbase_id, 0, total_block_fees, miner_address);
        cout << "Selected " << tx_count << " transactions from mempool." << endl;
        cout << "Total fees: " << total_block_fees << " BTC" << endl;
        cout << "Miner " << miner_address << " receives " << total_block_fees << " BTC" << endl;
        cout << "Block mined successfully!" << endl;
        mempool.clear_mempool(); 
        cout << "Removed " << tx_count << " transactions from mempool." << endl;
    }
};

#endif