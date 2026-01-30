# src/miner.py
import transaction

def mine_block(miner_address, mempool, utxo_manager):
    """
    Mines a block:
    1. Selects txs from mempool
    2. Updates UTXO set (removes inputs, adds outputs)
    3. Awards fees to miner
    4. Clears mined txs from mempool
    """
    if not mempool.transactions:
        print("Mempool is empty. Nothing to mine.")
        return

    # 1. Select transactions (Simple: take all valid ones)
    # In a real sim, you might sort by fee here.
    txs_to_mine = mempool.transactions[:] 
    
    total_fees = 0.0
    
    # 2. Update UTXO Manager
    for tx in txs_to_mine:
        # Calculate fee (Input Sum - Output Sum)
        tx_in_amount = 0
        for inp in tx["inputs"]:
            # Remove spent inputs from UTXO set
            utxo = utxo_manager.get_utxo(inp["prev_tx"], inp["index"])
            tx_in_amount += utxo["amount"]
            utxo_manager.remove_utxo(inp["prev_tx"], inp["index"])
            
        tx_out_amount = 0
        for i, out in enumerate(tx["outputs"]):
            # Add new outputs to UTXO set
            utxo_manager.add_utxo(tx["tx_id"], i, out["amount"], out["address"])
            tx_out_amount += out["amount"]
            
        total_fees += (tx_in_amount - tx_out_amount)

    # 3. Miner Reward (Coinbase)
    # Give the fees to the miner as a new UTXO (tx_id="coinbase_...")
    coinbase_id = f"coinbase_{transaction.generate_tx_id()}"
    if total_fees > 0:
        utxo_manager.add_utxo(coinbase_id, 0, total_fees, miner_address)
        print(f"Miner {miner_address} collected {total_fees} BTC in fees.")

    # 4. Clean Mempool
    mempool.clear_mined_utxos(txs_to_mine) # Remove spent flags
    mempool.transactions = [] # Clear the list
    print(f"Block mined! {len(txs_to_mine)} transactions confirmed.")
