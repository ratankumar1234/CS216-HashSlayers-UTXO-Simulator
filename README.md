# 🪙 Bitcoin UTXO Simulator  
### CS216 – Introduction to Blockchain | College Assignment

---

## 👥 Team Information

**Team Name:** HashSlayers  

| Name | Roll Number |
|------|------------|
| Raghav Sharma (Leader) | 240001056 | 
| Rohan Chauhan | 240001061 |
| Abhishek Kumar Verma | 240001005 |
| Ratan Kumar | 240001059 |

---

## 📌 Project Overview

This project is a **C++ based simulation of Bitcoin’s UTXO (Unspent Transaction Output) model**.  
It demonstrates how cryptocurrency transactions are:

- Created
- Validated
- Stored in a mempool
- Confirmed through block mining

The simulator focuses on **core blockchain logic** such as double-spending prevention, transaction validation, mempool handling, and mining state updates.

This is an **academic implementation** developed to satisfy CS216 assignment requirements.

---

## 🧠 Design Explanation (Brief)

The simulator follows Bitcoin’s transaction lifecycle:

### 🔹 Components
- **Transaction**  
  Stores inputs, outputs, transaction ID, and fee.

- **UTXO Manager**  
  Maintains the global set of unspent outputs and calculates balances.

- **Validator**  
  Enforces transaction rules:
  - Input UTXO must exist
  - No double-spending (same transaction & mempool)
  - Sufficient balance
  - No negative output amounts

- **Mempool**  
  Holds valid but unconfirmed transactions and applies a **first-seen rule** to prevent race attacks.

- **Block (Mining)**  
  Confirms all mempool transactions, updates the UTXO set, creates a coinbase transaction, and clears the mempool.

- **Test Suite**  
  Automatically verifies **10 mandatory scenarios**, including double-spending, insufficient funds, mining flow, and unconfirmed chains.

---

## ⚙️ Dependencies & Installation

### Requirements
- GCC / Clang compiler
- C++17 or later
- Standard Template Library (STL)

No external libraries are required.

---
### 🔹 Clone the Repository
```
git clone https://github.com/ratankumar1234/CS216-HashSlayers-UTXO-Simulator.git
```
---

## ▶️ Compile and Run the Simulator

### 🔹 Compile
```bash
g++ -std=c++17 main.cpp -o utxo_simulator
🔹 Run
./utxo_simulator
🔹 Simulator Features
The menu-driven interface allows you to:

Create new transactions

View UTXO set

View mempool

Mine a block

Exit

🧪 Compile and Run Test Suite
🔹 Compile Test File
g++ -std=c++17 test_scenarios.cpp -o tests
🔹 Run Tests
./tests
✅ Expected Output
Test 1 Passed: Basic Valid Transaction
Test 2 Passed: Multiple Inputs
Test 3 Passed: Double-Spend in Same Transaction (Rejected)
Test 4 Passed: Mempool Double-Spend (Rejected)
Test 5 Passed: Insufficient Funds (Rejected)
Test 6 Passed: Negative Amount (Rejected)
Test 7 Passed: Zero Fee Transaction (Accepted)
Test 8 Passed: Race Attack (First-seen rule enforced)
Test 9 Passed: Complete Mining Flow
Test 10 Passed: Unconfirmed Chain (Reject simple approach)

All 10 Mandatory Tests Passed!
📂 Project Structure
├── README.md
├── test/
    ├── test_scenarios.cpp
└── src/
    ├── main.cpp
    ├── block.cpp
    ├── mempool.cpp
    ├── transaction.cpp
    ├── utxo_manager.cpp
    └── validator.cpp
✅ Features Implemented
Genesis block initialization

UTXO-based balance management

Transaction fee handling

Double-spend prevention

Mempool conflict handling

Mining and UTXO state updates

Race attack prevention (first-seen rule)

Automated validation using test cases

