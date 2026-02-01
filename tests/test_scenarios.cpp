#include <iostream>
#include <cassert>
#include <string>
#include "../src/block.cpp"

using namespace std;

// Helper to reset state for each test
void reset_state(UTXOManager& mgr, Mempool& mp) {
    mgr.utxo_set.clear();
    mp.clear_mempool();
    mgr.add_utxo("genesis", 0, 50.0, "Alice");
    mgr.add_utxo("genesis", 1, 30.0, "Bob");
    mgr.add_utxo("genesis", 2, 20.0, "Charlie");
    mgr.add_utxo("genesis", 3, 10.0, "David");
    mgr.add_utxo("genesis", 4, 5.0, "Eve");
}

void run_tests() {
    UTXOManager mgr;
    Mempool mp;

    // --- TEST 1: Basic Valid Transaction --- [cite: 271-274]
    reset_state(mgr, mp);
    Transaction tx1;
    tx1.tx_id = "tx1";
    tx1.inputs = {{"genesis", 0, "Alice"}};
    tx1.outputs = {{10.0, "Bob"}, {39.999, "Alice"}}; // 0.001 fee
    assert(mp.add_transaction(tx1, mgr) == true);
    cout << "Test 1 Passed: Basic Valid Transaction" << endl;

    // --- TEST 2: Multiple Inputs --- [cite: 275-278]
    reset_state(mgr, mp);
    Transaction tx2;
    tx2.tx_id = "tx2";
    tx2.inputs = {{"genesis", 0, "Alice"}, {"genesis", 2, "Charlie"}}; // 50 + 20 = 70
    tx2.outputs = {{60.0, "Bob"}, {9.999, "Alice"}};
    assert(mp.add_transaction(tx2, mgr) == true);
    cout << "Test 2 Passed: Multiple Inputs" << endl;

    // --- TEST 3: Double-Spend in Same Transaction --- [cite: 279-281]
    reset_state(mgr, mp);
    Transaction tx3;
    tx3.tx_id = "tx3";
    tx3.inputs = {{"genesis", 0, "Alice"}, {"genesis", 0, "Alice"}}; // Same input used twice
    tx3.outputs = {{10.0, "Bob"}};
    assert(mp.add_transaction(tx3, mgr) == false);
    cout << "Test 3 Passed: Double-Spend in Same Transaction (Rejected)" << endl;

    // --- TEST 4: Mempool Double-Spend --- [cite: 282-285]
    reset_state(mgr, mp);
    Transaction tx4a;
    tx4a.tx_id = "tx4a";
    tx4a.inputs = {{"genesis", 1, "Bob"}};
    tx4a.outputs = {{10.0, "Alice"}};
    mp.add_transaction(tx4a, mgr); // First one occupies the UTXO

    Transaction tx4b;
    tx4b.tx_id = "tx4b";
    tx4b.inputs = {{"genesis", 1, "Bob"}}; // Tries to spend the same UTXO
    tx4b.outputs = {{5.0, "Charlie"}};
    assert(mp.add_transaction(tx4b, mgr) == false);
    cout << "Test 4 Passed: Mempool Double-Spend (Rejected)" << endl;

    // --- TEST 5: Insufficient Funds --- [cite: 286-288]
    reset_state(mgr, mp);
    Transaction tx5;
    tx5.tx_id = "tx5";
    tx5.inputs = {{"genesis", 1, "Bob"}}; // Bob has 30
    tx5.outputs = {{35.0, "Alice"}};      // Tries to send 35
    assert(mp.add_transaction(tx5, mgr) == false);
    cout << "Test 5 Passed: Insufficient Funds (Rejected)" << endl;

    // --- TEST 6: Negative Amount --- [cite: 289-291]
    reset_state(mgr, mp);
    Transaction tx6;
    tx6.tx_id = "tx6";
    tx6.inputs = {{"genesis", 3, "David"}};
    tx6.outputs = {{-5.0, "Eve"}}; // Negative output
    assert(mp.add_transaction(tx6, mgr) == false);
    cout << "Test 6 Passed: Negative Amount (Rejected)" << endl;

    // --- TEST 7: Zero Fee Transaction --- [cite: 292-294]
    reset_state(mgr, mp);
    Transaction tx7;
    tx7.tx_id = "tx7";
    tx7.inputs = {{"genesis", 4, "Eve"}}; // 5.0
    tx7.outputs = {{5.0, "Alice"}};       // 5.0 (0 fee)
    assert(mp.add_transaction(tx7, mgr) == true);
    cout << "Test 7 Passed: Zero Fee Transaction (Accepted)" << endl;

    // --- TEST 8: Race Attack Simulation (First-Seen Rule) --- [cite: 295-301, 465-474]
    reset_state(mgr, mp);
    Transaction tx8_low_fee;
    tx8_low_fee.tx_id = "tx8_low";
    tx8_low_fee.inputs = {{"genesis", 0, "Alice"}};
    tx8_low_fee.outputs = {{10.0, "Bob"}, {39.999, "Alice"}}; // 0.001 fee
    mp.add_transaction(tx8_low_fee, mgr);

    Transaction tx8_high_fee;
    tx8_high_fee.tx_id = "tx8_high";
    tx8_high_fee.inputs = {{"genesis", 0, "Alice"}}; // Same UTXO
    tx8_high_fee.outputs = {{10.0, "Charlie"}, {39.0, "Alice"}}; // 1.0 fee
    assert(mp.add_transaction(tx8_high_fee, mgr) == false); // Should reject even with higher fee
    cout << "Test 8 Passed: Race Attack (First-seen rule enforced)" << endl;

    // --- TEST 9: Complete Mining Flow --- [cite: 302-305, 478-491]
    reset_state(mgr, mp);
    Transaction tx9;
    tx9.tx_id = "tx9";
    tx9.inputs = {{"genesis", 1, "Bob"}}; // 30
    tx9.outputs = {{10.0, "Alice"}, {19.0, "Bob"}}; // 1.0 fee
    mp.add_transaction(tx9, mgr);
    
    Block::mine_block("Miner1", mp, mgr);
    assert(mgr.exists("tx9", 0) == true);  // New UTXO for Alice exists
    assert(mgr.exists("genesis", 1) == false); // Old UTXO spent
    assert(mp.transactions.empty() == true);   // Mempool cleared
    cout << "Test 9 Passed: Complete Mining Flow" << endl;

    // --- TEST 10: Unconfirmed Chain --- [cite: 306-310, 421-424]
    reset_state(mgr, mp);
    Transaction tx10a; // Alice -> Bob
    tx10a.tx_id = "tx10a";
    tx10a.inputs = {{"genesis", 0, "Alice"}};
    tx10a.outputs = {{10.0, "Bob"}, {39.9, "Alice"}};
    mp.add_transaction(tx10a, mgr);

    Transaction tx10b; // Bob tries to spend the 10.0 he just got before it's mined
    tx10b.tx_id = "tx10b";
    tx10b.inputs = {{"tx10a", 0, "Bob"}};
    tx10b.outputs = {{5.0, "Charlie"}, {4.9, "Bob"}};
    
    // We choose the 'Reject' approach as recommended for simplicity [cite: 423]
    assert(mp.add_transaction(tx10b, mgr) == false); 
    cout << "Test 10 Passed: Unconfirmed Chain (Reject simple approach)" << endl;
}

int main() {
    run_tests();
    cout << "\nAll 10 Mandatory Tests Passed!" << endl;
    return 0;
}