#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "block.cpp"
#include "../tests/test_scenarios.cpp"

using namespace std;

// Function to populate the initial spendable coins [cite: 213-218, 265-269]
void initialize_genesis(UTXOManager& mgr) {
    mgr.add_utxo("genesis", 0, 50.0, "Alice");
    mgr.add_utxo("genesis", 1, 30.0, "Bob");
    mgr.add_utxo("genesis", 2, 20.0, "Charlie");
    mgr.add_utxo("genesis", 3, 10.0, "David");
    mgr.add_utxo("genesis", 4, 5.0, "Eve");
}

// Generates a unique ID for transactions [cite: 343-346]
string generate_tx_id(string sender, string receiver) {
    return "tx_" + sender + "_" + receiver + "_" + to_string(rand() % 10000);
}

int main() {
    srand(time(0));
    UTXOManager utxo_mgr;
    Mempool mempool;
    initialize_genesis(utxo_mgr);

    int choice;
    while (true) { 
        // Required Program Interface 
        cout << "\n=== Bitcoin Transaction Simulator ===\n";
        cout << "Initial UTXOs (Genesis Block):\n";
        cout << "Alice: 50.0 BTC, Bob: 30.0 BTC, Charlie: 20.0 BTC, David: 10.0 BTC, Eve: 5.0 BTC\n\n";
        cout << "Main Menu:\n";
        cout << "1. Create new transaction\n";
        cout << "2. View UTXO set\n";
        cout << "3. View mempool\n";
        cout << "4. Mine block\n";
        cout << "5. Run test scenarios\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 6) break;

        switch (choice) {
            case 1: { // Manual Transaction Creation Workflow [cite: 232-244]
                string sender, recipient;
                double amount;
                cout << "Enter sender: "; cin >> sender;
                cout << "Available balance: " << utxo_mgr.get_balance(sender) << " BTC\n";
                cout << "Enter recipient: "; cin >> recipient;
                cout << "Enter amount: "; cin >> amount;

                Transaction tx;
                tx.tx_id = generate_tx_id(sender, recipient);
                double gathered_amount = 0;
                const double fee = 0.001; 

                for (auto const& it : utxo_mgr.utxo_set) {
                    if (it.second.owner == sender) {
                        tx.inputs.push_back({it.first.first, it.first.second, sender});
                        gathered_amount += it.second.amount;
                        if (gathered_amount >= (amount + fee)) break;
                    }
                }

                tx.outputs.push_back({amount, recipient});
                if (gathered_amount > (amount + fee)) {
                    tx.outputs.push_back({gathered_amount - amount - fee, sender}); // Change output [cite: 63-66]
                }
                tx.fee = fee;

                cout << "Creating transaction...\n";
                if (mempool.add_transaction(tx, utxo_mgr)) {
                    cout << "Transaction valid! Fee: " << fee << " BTC\n";
                    cout << "Transaction ID: " << tx.tx_id << "\n";
                    cout << "Transaction added to mempool.\n";
                }
                break;
            }
            case 2: // View Current UTXO State [cite: 221]
                for (auto const& it : utxo_mgr.utxo_set) {
                    cout << it.second.owner << ": " << it.second.amount << " BTC (" << it.first.first << ":" << it.first.second << ")\n";
                }
                break;
            case 3: // View Pending Transactions [cite: 222]
                if (mempool.transactions.empty()) {
                    cout << "Mempool is empty.\n";
                } else {
                    for (const auto& tx : mempool.transactions) {
                        cout << "TX ID: " << tx.tx_id << " | Fee: " << tx.fee << " BTC\n";
                    }
                }
                break;
            case 4: { // Mine Block Workflow [cite: 246-253]
                string miner;
                cout << "Enter miner name: "; cin >> miner;
                Block::mine_block(miner, mempool, utxo_mgr);
                break;
            }
            case 5: { // Sub-menu for individual test selection [cite: 256-257, 270-306]
                int test_choice;
                cout << "\nSelect test scenario (1-10):\n";
                cout << "1. Basic Valid Transaction\n2. Double-spend\n3. Double-spend in Same TX\n";
                cout << "4. Mempool Double-spend\n5. Insufficient Funds\n6. Negative Amount\n";
                cout << "7. Zero Fee\n8. Race Attack\n9. Mining Flow\n10. Unconfirmed Chain\n";
                cout << "Enter test choice: ";
                cin >> test_choice;

                // Reset state to Genesis for consistent testing [cite: 258, 264-269]
                utxo_mgr.utxo_set.clear();
                mempool.clear_mempool();
                initialize_genesis(utxo_mgr);

                run_specific_test(test_choice, utxo_mgr, mempool);
                break;
            }
            default:
                cout << "Invalid choice.\n";
        }
    }
    return 0;
}