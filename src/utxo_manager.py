# src/utxo_manager.py

class UTXOManager:
    def __init__(self):
        # Dictionary mapping (tx_id, index) -> {"amount": float, "owner": str}
        self.utxo_set = {}

    def add_utxo(self, tx_id, index, amount, owner):
        """Add a new UTXO to the set."""
        key = (tx_id, index)
        self.utxo_set[key] = {"amount": amount, "owner": owner}

    def remove_utxo(self, tx_id, index):
        """Remove a UTXO (when spent)."""
        key = (tx_id, index)
        if key in self.utxo_set:
            del self.utxo_set[key]

    def get_balance(self, owner):
        """Calculate total balance for an address."""
        balance = 0.0
        for utxo_data in self.utxo_set.values():
            if utxo_data["owner"] == owner:
                balance += utxo_data["amount"]
        return balance

    def exists(self, tx_id, index):
        """Check if UTXO exists and is unspent."""
        return (tx_id, index) in self.utxo_set

    def get_utxo(self, tx_id, index):
        """Return the UTXO data if it exists."""
        return self.utxo_set.get((tx_id, index))
