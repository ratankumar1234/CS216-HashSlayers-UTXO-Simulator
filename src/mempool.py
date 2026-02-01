# src/mempool.py

class Mempool:
    def __init__(self):
        self.transactions = []  # List of pending transactions
        self.spent_utxos = set()  # Track UTXOs referenced by pending txs

    def add_transaction(self, tx):
        """Add valid transaction and mark inputs as spent."""
        self.transactions.append(tx)
        # Mark inputs as spent in mempool to prevent double-spending
        for inp in tx["inputs"]:
            utxo_key = (inp["prev_tx"], inp["index"])
            self.spent_utxos.add(utxo_key)

    def remove_transaction(self, tx):
        """Remove transaction (e.g., when mined)."""
        if tx in self.transactions:
            self.transactions.remove(tx)
            # We don't remove from spent_utxos here because they are now
            # permanently spent in the blockchain (UTXOManager).
            
    def clear_mined_utxos(self, tx_list):
        """
        Clean up spent_utxos for transactions that have been mined.
        In a real node, we'd sync this with the UTXO set update.
        """
        for tx in tx_list:
            for inp in tx["inputs"]:
                utxo_key = (inp["prev_tx"], inp["index"])
                if utxo_key in self.spent_utxos:
                    self.spent_utxos.remove(utxo_key)

    def is_input_spent(self, prev_tx, index):
        """Check if a UTXO is already being spent in the mempool."""
        return (prev_tx, index) in self.spent_utxos
