# src/main.py
from utxo_manager import UTXOManager
from mempool import Mempool
import transaction
import miner

def init_genesis(utxo_manager):
    """Initialize the mandatory genesis UTXOs."""
    utxo_manager.add_utxo("genesis", 0, 50.0, "Alice")
    utxo_manager.add_utxo("genesis", 1, 30.0, "Bob")
    utxo_manager.add_utxo("genesis", 2, 20.0, "Charlie")
    utxo_manager.add_utxo("genesis", 3, 10.0, "David")
    utxo_manager.add_utxo("genesis", 4, 5.0, "Eve")

def print_menu():
    print("\n=== Bitcoin Transaction Simulator ===")
    print("1. Create new transaction")
    print("2. View UTXO set")
    print("3. View mempool")
    print("4. Mine block")
    print("5. Run test scenarios")
    print("6. Exit")

def create_tx_ui(utxo_mgr, mp):
    sender = input("Enter sender: ")
    # Find UTXOs for sender (Simple auto-selection logic)
    sender_utxos = []
    balance = 0
    for key, data in utxo_mgr.utxo_set.items():
        if data["owner"] == sender:
            sender_utxos.append({"prev_tx": key[0], "index": key[1], "amount": data["amount"]})
            balance += data["amount"]
    
    print(f"Available balance: {balance} BTC")
    recipient = input("Enter recipient: ")
    try:
        amount = float(input("Enter amount: "))
    except ValueError:
        print("Invalid amount.")
        return

    if amount > balance:
        print("Insufficient funds (Wallet logic check).")
        return

    # Construct Inputs (Naïve coin selection: take what is needed)
    inputs = []
    input_sum = 0.0
    for u in sender_utxos:
        inputs.append({"prev_tx": u["prev_tx"], "index": u["index"]})
        input_sum += u["amount"]
        if input_sum >= amount:
            break
            
    # Construct Outputs
    outputs = [{"amount": amount, "address": recipient}]
    
    # Calculate Change (Simple fee: assume 0.001 fee, rest is change)
    fee = 0.001
    change = input_sum - amount - fee
    
    if change > 0:
        outputs.append({"amount": change, "address": sender})
    
    # Create TX Object
    tx = transaction.create_transaction(inputs, outputs)
    
    # Validate
    valid, msg = transaction.validate_transaction(tx, utxo_mgr, mp)
    if valid:
        mp.add_transaction(tx)
        print(f"Transaction valid! Fee: {msg:.3f} BTC")
        print(f"Transaction ID: {tx['tx_id']}")
        print("Transaction added to mempool.")
    else:
        print(f"Transaction Rejected: {msg}")

def run_tests(utxo_mgr, mp):
    print("Running Test 3: Double-Spend in Same Transaction...")
    # Setup: Alice has 50. Alice tries to spend (genesis,0) twice in same TX
    inputs = [
        {"prev_tx": "genesis", "index": 0},
        {"prev_tx": "genesis", "index": 0} # Double Reference!
    ]
    outputs = [{"amount": 10.0, "address": "Bob"}]
    bad_tx = transaction.create_transaction(inputs, outputs)
    valid, msg = transaction.validate_transaction(bad_tx, utxo_mgr, mp)
    print(f"Result: {msg} (Expected: Rejected)")

    print("\nRunning Test 4: Mempool Double-Spend...")
    # Alice sends 10 to Bob (Valid)
    inputs_valid = [{"prev_tx": "genesis", "index": 0}]
    # Note: In a real run we need to calculate exact change, but for this test input logic matters
    outputs_valid = [{"amount": 10.0, "address": "Bob"}] 
    tx1 = transaction.create_transaction(inputs_valid, outputs_valid)
    
    # Mock adding UTXO manually for test context if it doesn't exist
    if not utxo_mgr.exists("genesis", 0):
        print("Skipping (UTXO spent). Reset system to run full tests.")
        return

    mp.add_transaction(tx1)
    print("TX1 added to mempool.")
    
    # Alice tries to send SAME UTXO to Charlie
    tx2 = transaction.create_transaction(inputs_valid, [{"amount": 10.0, "address": "Charlie"}])
    valid, msg = transaction.validate_transaction(tx2, utxo_mgr, mp)
    print(f"TX2 Result: {msg} (Expected: Rejected due to mempool conflict)")
    
    # Cleanup for simulation continuity
    mp.transactions = []
    mp.spent_utxos = set()
    print("Tests finished. Mempool cleared.")

def main():
    utxo_manager = UTXOManager()
    mempool = Mempool()
    init_genesis(utxo_manager)

    while True:
        print_menu()
        choice = input("Enter choice: ")

        if choice == '1':
            create_tx_ui(utxo_manager, mempool)
        elif choice == '2':
            print("\n--- Current UTXOs ---")
            for key, val in utxo_manager.utxo_set.items():
                print(f"{val['owner']}: {val['amount']} BTC (tx: {key[0]}, idx: {key[1]})")
        elif choice == '3':
            print(f"\nMempool: {len(mempool.transactions)} transactions.")
            for tx in mempool.transactions:
                print(f"- {tx['tx_id']}: {len(tx['inputs'])} inputs -> {len(tx['outputs'])} outputs")
        elif choice == '4':
            miner_name = input("Enter miner name: ")
            miner.mine_block(miner_name, mempool, utxo_manager)
        elif choice == '5':
            run_tests(utxo_manager, mempool)
        elif choice == '6':
            print("Exiting...")
            break
        else:
            print("Invalid choice.")

if __name__ == "__main__":
    main()
