#ifndef TEST_SCENARIOS_CPP
#define TEST_SCENARIOS_CPP

#include <iostream>
#include <string>
#include <vector>
#include "../src/block.cpp"

using namespace std;

void run_specific_test(int choice, UTXOManager& mgr, Mempool& mp) {
    cout << "\nRunning test scenario " << choice << "...\n";

    switch (choice) {
        case 1: { // Test 1: Basic Valid Transaction
            cout << "Test: Alice sends 10 BTC to Bob\n";
            Transaction tx = {"tx1", {{"genesis", 0, "Alice"}}, {{10.0, "Bob"}, {39.999, "Alice"}}, 0.001};
            if(mp.add_transaction(tx, mgr)) cout << "Result: VALID and ADDED\n";
            break;
        }

        // --- FIXED TEST 2 ---
        case 2: { // Test 2: Multiple Inputs 
            cout << "Test: Alice spends two UTXOs (50 + 20) to send 60 BTC\n";
            mgr.add_utxo("dummy_funding", 0, 20.0, "Alice"); 

            Transaction tx;
            tx.tx_id = "tx_multi_input"; 
            tx.inputs = { {"genesis", 0, "Alice"}, {"dummy_funding", 0, "Alice"} };
             
            tx.outputs = { {60.0, "Bob"}, {9.999, "Alice"} };
            tx.fee = 0.001;

            if(mp.add_transaction(tx, mgr)) {
                cout << "Result: VALID. Inputs aggregated (50+20=70) to cover output (60)\n";
            } else {
                cout << "Result: FAILED\n";
            }
            break;
        }

        case 3: {  
            cout << "Test: Transaction tries to use same input (genesis:0) twice\n"; 
            Transaction tx = {
                "tx3", 
                {{"genesis", 0, "Alice"}, {"genesis", 0, "Alice"}},  
                {{10.0, "Bob"}}, 
                0.0
            };
            if(!mp.add_transaction(tx, mgr)) {
                cout << "Result: REJECTED (Correct) - Double-spend in same TX detected.\n";
            } else {
                cout << "Result: FAILED - System incorrectly accepted the transaction.\n";
            }
            break;
        }

        case 4: { // Mempool Double-Spend
            cout << "Test: Alice tries to spend same UTXO twice in different transactions\n";
            Transaction tx1 = {"tx1", {{"genesis", 0, "Alice"}}, {{10.0, "Bob"}, {39.999, "Alice"}}, 0.001};
            mp.add_transaction(tx1, mgr);
            cout << "TX1: Alice -> Bob (10 BTC) - VALID\n";
            
            Transaction tx2 = {"tx2", {{"genesis", 0, "Alice"}}, {{10.0, "Charlie"}, {39.999, "Alice"}}, 0.001};
            cout << "TX2: Alice -> Charlie (10 BTC) - ";
            if (!mp.add_transaction(tx2, mgr)) cout << "REJECTED (Correct)\n";
            break;
        }

        case 5: { // Insufficient Funds
            cout << "Test: Bob sends 35 BTC while having only 30 BTC\n";
            Transaction tx = {"tx5", {{"genesis", 1, "Bob"}}, {{35.0, "Alice"}}, 0.0};
            cout << "Result: ";
            if(!mp.add_transaction(tx, mgr)) cout << "REJECTED (Correct)\n";
            break;
        }

        case 6: { // Negative Amount
            cout << "Test: Output amount is -5 BTC\n";
            Transaction tx = {"tx6", {{"genesis", 3, "David"}}, {{-5.0, "Eve"}}, 0.0};
            cout << "Result: ";
            if(!mp.add_transaction(tx, mgr)) cout << "REJECTED (Correct)\n";
            break;
        }

        case 7: { // Zero Fee
            cout << "Test: Inputs = Outputs (Zero Fee)\n";
            Transaction tx = {"tx7", {{"genesis", 4, "Eve"}}, {{5.0, "Alice"}}, 0.0};
            cout << "Result: ";
            if(mp.add_transaction(tx, mgr)) cout << "ACCEPTED (Correct)\n";
            break;
        }

        case 8: { // Race Attack
            cout << "Test: High fee TX tries to replace already-seen low fee TX\n";
            Transaction tx_low = {"low_fee", {{"genesis", 0, "Alice"}}, {{10.0, "Bob"}, {39.999, "Alice"}}, 0.001};
            mp.add_transaction(tx_low, mgr);
            cout << "TX1 (Low Fee) - ADDED\n";

            Transaction tx_high = {"high_fee", {{"genesis", 0, "Alice"}}, {{10.0, "Charlie"}, {39.0, "Alice"}}, 1.0};
            cout << "TX2 (High Fee) - ";
            if(!mp.add_transaction(tx_high, mgr)) cout << "REJECTED (First-seen rule correctly enforced)\n";
            break;
        }

        case 9: { // Mining Flow
            cout << "Test: Full lifecycle from creation to confirmation\n";
            Transaction tx = {"tx9", {{"genesis", 1, "Bob"}}, {{10.0, "Alice"}, {19.999, "Bob"}}, 0.001};
            mp.add_transaction(tx, mgr);
            cout << "Mining Block...\n";
            Block::mine_block("Miner1", mp, mgr);
            break;
        }

        case 10: { // Unconfirmed Chain
            cout << "Test: Spending a UTXO that is in mempool but not yet mined\n";
            Transaction tx_p = {"parent", {{"genesis", 0, "Alice"}}, {{10.0, "Bob"}, {39.9, "Alice"}}, 0.1};
            mp.add_transaction(tx_p, mgr);
            
            Transaction tx_c = {"child", {{"parent", 0, "Bob"}}, {{5.0, "Charlie"}, {4.9, "Bob"}}, 0.1};
            cout << "Result: ";
            if(!mp.add_transaction(tx_c, mgr)) cout << "REJECTED (Simulator chooses simple reject approach)\n";
            break;
        }

        default:
            cout << "Invalid Test Selection.\n";
    }
}
#endif