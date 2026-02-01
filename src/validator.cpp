#ifndef VALIDATOR_CPP
#define VALIDATOR_CPP
#include "utxo_manager.cpp"
#include <set>
using namespace std;

class Validator {
public:
    static bool validate(const Transaction& tx, UTXOManager& utxo_mgr, const set<pair<string, int>>& mempool_spent, string& error_msg) {
        double input_sum = 0;
        set<pair<string, int>> current_tx_inputs;

        for (const auto& in : tx.inputs) {
            if (!utxo_mgr.exists(in.prev_tx, in.index)) {
                error_msg = "Input UTXO does not exist.";
                return false;
            }
            if (current_tx_inputs.count({in.prev_tx, in.index})) {
                error_msg = "Double-spend detected in same transaction.";
                return false;
            }
            current_tx_inputs.insert({in.prev_tx, in.index});
            if (mempool_spent.count({in.prev_tx, in.index})) {
                error_msg = "UTXO already spent in mempool.";
                return false;
            }
            input_sum += utxo_mgr.utxo_set[{in.prev_tx, in.index}].amount;
        }

        double output_sum = 0;
        for (const auto& out : tx.outputs) {
            if (out.amount < 0) {
                error_msg = "Negative output amount.";
                return false;
            }
            output_sum += out.amount;
        }

        if (input_sum < output_sum) {
            error_msg = "Insufficient funds.";
            return false;
        }
        return true;
    }
};

#endif