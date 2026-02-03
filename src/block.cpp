#ifndef BLOCK_CPP
#define BLOCK_CPP

#include "mempool.cpp"
#include <algorithm>
#include <iostream>

using namespace std;

class Block {
public:
    static void mine_block(string miner_address, Mempool& mempool, UTXOManager& utxo_mgr) {

        if (mempool.transactions.empty()) {
            cout << "Mempool empty. Nothing to mine.\n";
            return;
        }

        double total_fees = 0.0;

        // 🔹 Apply all transactions
        for (const auto& tx : mempool.transactions) {

            // 1️⃣ Remove spent UTXOs
            for (const auto& in : tx.inputs) {
                utxo_mgr.remove_utxo(in.prev_tx, in.index);
            }

            // 2️⃣ Add new UTXOs
            for (size_t i = 0; i < tx.outputs.size(); i++) {
                utxo_mgr.add_utxo(
                    tx.tx_id,
                    i,
                    tx.outputs[i].amount,
                    tx.outputs[i].address
                );
            }

            // 3️⃣ Accumulate fees
            total_fees += tx.fee;
        }

        // 🔹 Miner reward (block reward + fees)
        const double block_reward = 50.0;
        utxo_mgr.add_utxo(
            "coinbase",
            rand() % 100000,
            block_reward + total_fees,
            miner_address
        );

        // 🔹 Clear mempool
        mempool.clear_mempool();

        cout << "Block mined successfully by " << miner_address << endl;
        cout << "Miner reward: " << block_reward + total_fees << " BTC\n";
    }
};

#endif
