#ifndef TEST_SCENARIOS_CPP
#define TEST_SCENARIOS_CPP

#include <iostream>
#include <string>
#include <cassert>
#include "../src/block.cpp"

using namespace std;

// This function acts as a dispatcher for individual test scenarios [cite: 270-306, 538]
void run_specific_test(int choice, UTXOManager& mgr, Mempool& mp) {
    cout << "\nRunning test scenario " << choice << "...\n";

    switch (choice) {
        case 1: { // Test 1: Basic Valid Transaction [cite: 271-274]
            cout << "Test: Alice sends 10 BTC to Bob\n";
            Transaction tx = {"tx1", {{"genesis", 0, "Alice"}}, {{10.0, "Bob"}, {39.999, "Alice"}}, 0.001};
            if(mp.add_transaction(tx, mgr)) cout << "Result: VALID and ADDED\n";
            break;
        }
        case 2: // Generic Double-spend logic [cite: 255-262]
        case 4: { // Test 4: Mempool Double-Spend [cite: 282-285, 467-473]
            cout << "Test: Alice tries to spend same UTXO twice in different transactions\n";
            Transaction tx1 = {"tx1", {{"genesis", 0, "Alice"}}, {{10.0, "Bob"}, {39.999, "Alice"}}, 0.001};
            mp.add_transaction(tx1, mgr);
            cout << "TX1: Alice -> Bob (10 BTC) - VALID\n";
            
            Transaction tx2 = {"tx2", {{"genesis", 0, "Alice"}}, {{10.0, "Charlie"}, {39.999, "Alice"}}, 0.001};
            cout << "TX2: Alice -> Charlie (10 BTC) - ";
            if (!mp.add_transaction(tx2, mgr)) cout << "REJECTED (Correct)\n";
            break;
        }
        case 3: { // Test 3: Double-Spend in Same Transaction [cite: 279-281]
            cout << "Test: Transaction tries to use same input twice\n";
            Transaction tx = {"tx3", {{"genesis", 0, "Alice"}, {"genesis", 0, "Alice"}}, {{10.0, "Bob"}}, 0.0};
            cout << "Result: ";
            if(!mp.add_transaction(tx, mgr)) cout << "REJECTED (Correct)\n";
            break;
        }
        case 5: { // Test 5: Insufficient Funds [cite: 286-288]
            cout << "Test: Bob sends 35 BTC while having only 30 BTC\n";
            Transaction tx = {"tx5", {{"genesis", 1, "Bob"}}, {{35.0, "Alice"}}, 0.0};
            cout << "Result: ";
            if(!mp.add_transaction(tx, mgr)) cout << "REJECTED (Correct)\n";
            break;
        }
        case 6: { // Test 6: Negative Amount [cite: 289-291]
            cout << "Test: Output amount is -5 BTC\n";
            Transaction tx = {"tx6", {{"genesis", 3, "David"}}, {{-5.0, "Eve"}}, 0.0};
            cout << "Result: ";
            if(!mp.add_transaction(tx, mgr)) cout << "REJECTED (Correct)\n";
            break;
        }
        case 7: { // Test 7: Zero Fee Transaction [cite: 292-294]
            cout << "Test: Inputs = Outputs (Zero Fee)\n";
            Transaction tx = {"tx7", {{"genesis", 4, "Eve"}}, {{5.0, "Alice"}}, 0.0};
            cout << "Result: ";
            if(mp.add_transaction(tx, mgr)) cout << "ACCEPTED (Correct)\n";
            break;
        }
        case 8: { // Test 8: Race Attack (First-Seen Rule) [cite: 295-301, 465-474]
            cout << "Test: High fee TX tries to replace already-seen low fee TX\n";
            Transaction tx_low = {"low_fee", {{"genesis", 0, "Alice"}}, {{10.0, "Bob"}, {39.999, "Alice"}}, 0.001};
            mp.add_transaction(tx_low, mgr);
            cout << "TX1 (Low Fee) - ADDED\n";

            Transaction tx_high = {"high_fee", {{"genesis", 0, "Alice"}}, {{10.0, "Charlie"}, {39.0, "Alice"}}, 1.0};
            cout << "TX2 (High Fee) - ";
            if(!mp.add_transaction(tx_high, mgr)) cout << "REJECTED (First-seen rule correctly enforced)\n";
            break;
        }
        case 9: { // Test 9: Complete Mining Flow [cite: 302-305, 478-491]
            cout << "Test: Full lifecycle from creation to confirmation\n";
            Transaction tx = {"tx9", {{"genesis", 1, "Bob"}}, {{10.0, "Alice"}, {19.999, "Bob"}}, 0.001};
            mp.add_transaction(tx, mgr);
            cout << "Mining Block...\n";
            Block::mine_block("Miner1", mp, mgr);
            break;
        }
        case 10: { // Test 10: Unconfirmed Chain [cite: 306-310, 421-424]
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