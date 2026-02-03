#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "block.cpp"
using namespace std;
 
void initialize_genesis(UTXOManager& mgr) {
    mgr.add_utxo("genesis", 0, 50.0, "Alice");
    mgr.add_utxo("genesis", 1, 30.0, "Bob");
    mgr.add_utxo("genesis", 2, 20.0, "Charlie");
    mgr.add_utxo("genesis", 3, 10.0, "David");
    mgr.add_utxo("genesis", 4, 5.0, "Eve");
}
 
string generate_tx_id(string sender, string receiver) {
    return "tx_" + sender + "_" + receiver + "_" + to_string(rand() % 10000);
}

int main() {
    srand(time(0));
    UTXOManager utxo_mgr;
    Mempool mempool;
    initialize_genesis(utxo_mgr);

    int choice;
        cout << "\n=== Bitcoin Transaction Simulator ===\n";cout<<endl;
        cout << "Initial UTXOs (Genesis Block):\n";
        cout << "Alice: 50.0 BTC, Bob: 30.0 BTC, Charlie: 20.0 BTC, David: 10.0 BTC, Eve: 5.0 BTC\n";

    while (true) { 
        cout<<endl;
        cout<<"========================================"<<endl;
        cout<<endl;
        cout << "Main Menu:\n";
        cout << "1. Create new transaction\n";
        cout << "2. View UTXO set\n";
        cout << "3. View mempool\n";
        cout << "4. Mine block\n";
        cout << "5. Run test scenarios\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cout<<endl;

        cin >> choice;
        if (choice == 6) break;
        switch (choice) {
            case 1: {
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
                    const auto& key = it.first;
                    const auto& utxo = it.second;
                    if (utxo.owner == sender) {
                        tx.inputs.push_back({key.first, key.second, sender});
                        gathered_amount += utxo.amount;
                        if (gathered_amount >= (amount + fee)) break;
                    }
                }

                tx.outputs.push_back({amount, recipient});
                if (gathered_amount > (amount + fee)) {
                    tx.outputs.push_back({gathered_amount - amount - fee, sender});
                }
                tx.fee = fee;
                cout << "Creating transaction...\n";
                if (mempool.add_transaction(tx, utxo_mgr)) {
                    cout << "Transaction valid! Fee: " << fee << " BTC\n";
                    cout << "Transaction ID: " << tx.tx_id << "\n";
                    cout << "Transaction added to mempool.\n";
                    cout << "Mempool now has " << mempool.transactions.size() << " transactions.\n";
                }
                break;
            }
            case 2:  
                for (auto const& it : utxo_mgr.utxo_set) {
                    cout << it.second.owner << ": " << it.second.amount << " BTC (" << it.first.first << ":" << it.first.second << ")\n";
                }
                break;
            case 3: 
                if (mempool.transactions.empty()) {
                    cout << "Mempool is empty.\n";
                } else {
                    for (const auto& tx : mempool.transactions) {
                        cout << "TX ID: " << tx.tx_id << " | Fee: " << tx.fee << " BTC\n";
                    }
                }
                break;
            case 4: { 
                string miner;
                cout << "Enter miner name: "; cin >> miner;
                cout << "Mining block...\n";
                Block::mine_block(miner, mempool, utxo_mgr);
                break;
            }
            case 5: {  
                cout<<endl;
                cout << "Please run 'test_scenarios.cpp' to verify all 10 mandatory cases.\n";
                break;
            }
            default:
                cout << "Invalid choice.\n";
        }
    }
    return 0;
}
