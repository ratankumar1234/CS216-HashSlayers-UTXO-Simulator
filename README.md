🪙 Bitcoin UTXO Transaction SimulatorCS 216: Introduction to Blockchain | IIT Indore👥 Team DetailsTeam Name: HashSlayersNameRoll NumberRoleRAGHAV SHARMA240001056Group LeaderROHAN CHAUHAN240001061MemberABHISHEK KUMAR VERMA240001005MemberRATAN KUMAR240001059Member📖 Project OverviewThis project is a functional C++ simulation of Bitcoin's Unspent Transaction Output (UTXO) model. It demonstrates the lifecycle of a cryptocurrency transaction—from creation and validation to mempool buffering and final confirmation via mining.The simulator strictly enforces Bitcoin's core logic: preventing double-spending, ensuring solvency (sufficient funds), and maintaining a "first-seen" policy for unconfirmed transactions in the mempool.🎯 Key Features🏦 UTXO Management: Tracks spendable outputs using a map-based in-memory database.🛡️ Robust Validation: Enforces five core rules: UTXO existence, solvency, non-negative amounts, and double-spend protection.⏳ Mempool Logic: Buffers valid transactions and tracks "spent" status to prevent race attacks.⛏️ Block Mining: Simulates processing the mempool, removing spent UTXOs, creating new ones, and awarding coinbase rewards to miners.🧪 Comprehensive Testing: Automated test suite covering 10 mandatory edge cases.🏗️ Design ExplanationThe system is built using a modular object-oriented approach in C++:Transaction Struct: Defines the structure of inputs (referencing previous TX IDs) and outputs (amount and recipient).UTXOManager: The "Global State." It maintains the utxo_set and provides methods to add/remove coins and check balances.Validator: The logic engine. It verifies that inputs are valid, not already spent, and that the total input sum covers the outputs.Mempool: A temporary storage area. It uses a spent_utxos set to ensure that once a UTXO is used in a pending transaction, it cannot be used again until the mempool is cleared or mined.Block: Handles the "Mining" phase where all transactions in the mempool are committed to the global state.🚀 Getting StartedPrerequisitesA C++ compiler supporting C++11 or higher (e.g., g++ or clang).Compilation & ExecutionTo run the interactive simulator:Bash# Compile the main program
g++ -o simulator main.cpp

# Run the simulator
./simulator
To run the automated test suite:Bash# Compile the test scenarios
g++ -o tests test_scenarios.cpp

# Run the tests
./tests
📂 Repository StructurePlaintext.
├── main.cpp             # 🚀 Entry point with Interactive CLI Menu
├── block.cpp            # ⛏️ Mining logic and block creation
├── mempool.cpp          # ⏳ Transaction buffering and first-seen rule
├── transaction.cpp      # 📝 Data structures for Inputs/Outputs
├── utxo_manager.cpp     # 🏦 Global state management (UTXO Set)
├── validator.cpp        # 🛡️ Core validation logic
└── test_scenarios.cpp   # 🧪 Automated test suite (10 mandatory cases)
🧪 Verified Test ScenariosThe test_scenarios.cpp file validates the following:Basic Valid Transaction: Simple transfer from Alice to Bob.Multiple Inputs: Combining two UTXOs to fund one large transaction.Intra-TX Double Spend: Rejecting a TX that tries to use the same input twice.Mempool Double Spend: Rejecting a second TX that tries to use an input already sitting in the mempool.Insufficient Funds: Rejecting transactions where output > input.Negative Amounts: Enforcing that all output values are positive.Zero Fee: Ensuring transactions with 0 fees are still structurally valid.Race Attack: Confirming that the "first-seen" transaction in the mempool wins.Mining Flow: Verifying that mining correctly updates the UTXO set and clears the mempool.Unconfirmed Chain: Correctly rejecting/handling chains of unmined transactions for simplicity.
