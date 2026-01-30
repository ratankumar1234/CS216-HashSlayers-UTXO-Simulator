# src/transaction.py
import time
import random

def generate_tx_id():
    """Generate a unique transaction ID."""
    return f"tx_{int(time.time())}_{random.randint(1000, 9999)}"

def create_transaction(inputs, outputs):
    """Factory to create a transaction dictionary."""
    return {
        "tx_id": generate_tx_id(),
        "inputs": inputs,   # List of {"prev_tx": str, "index": int}
        "outputs": outputs  # List of {"amount": float, "address": str}
    }

def validate_transaction(tx, utxo_manager, mempool):
    """
    Validates transaction against 5 rules:
    1. Inputs exist
    2. No double-spend in inputs
    3. Solvency (Input Sum >= Output Sum)
    4. Positive outputs
    5. No mempool conflict
    """
    total_in = 0.0
    total_out = 0.0
    seen_inputs = set()

    # Rule 2 & 5: Check inputs
    for inp in tx["inputs"]:
        prev_tx = inp["prev_tx"]
        idx = inp["index"]
        
        # Rule 1: Existence
        if not utxo_manager.exists(prev_tx, idx):
            return False, f"Input {prev_tx}:{idx} does not exist."
        
        # Rule 5: Mempool Conflict
        if mempool.is_input_spent(prev_tx, idx):
            return False, f"Input {prev_tx}:{idx} is already pending in mempool."
            
        # Rule 2: Double spend in same tx
        input_key = (prev_tx, idx)
        if input_key in seen_inputs:
            return False, f"Double spend of {prev_tx}:{idx} in same transaction."
        seen_inputs.add(input_key)

        # Sum inputs
        utxo_data = utxo_manager.get_utxo(prev_tx, idx)
        total_in += utxo_data["amount"]

    # Rule 4: Check outputs
    for out in tx["outputs"]:
        if out["amount"] < 0:
            return False, "Output amount cannot be negative."
        total_out += out["amount"]

    # Rule 3: Solvency
    if total_in < total_out:
        return False, f"Insufficient funds. In: {total_in}, Out: {total_out}"

    fee = total_in - total_out
    return True, fee
