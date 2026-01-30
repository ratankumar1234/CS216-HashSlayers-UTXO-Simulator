# 🪙 Bitcoin UTXO Simulator

### CS 216: Introduction to Blockchain | Assignment 1

## 👥 Team Details

**Team Name: HashSlayers**

| Name                 | Roll Number | Role         |

| RAGHAV SHARMA        | 240001056   | Group Leader |

| ROHAN CHAUHAN        | 240001061   | Memeber      |

| ABHISHEK KUMAR VERMA | 240001005   | Member       |

| RATAN KUMAR          | 240001059   | Member       |

---

## 📖 Project Overview

This project is a functional simulation of Bitcoin's **Unspent Transaction Output (UTXO)** model. It demonstrates the lifecycle of a cryptocurrency transaction from creation to validation, mempool buffering, and final confirmation via mining.

Unlike a simple account-based system (like a bank), this simulator tracks individual "coins" (UTXOs) to prevent **double-spending** and enforce cryptographic rules logic.

### 🎯 Key Features
* **🏦 UTXO Management:** Efficiently tracks all spendable outputs using an in-memory database.
* **🛡️ Strict Validation:** Enforces the 5 core rules of Bitcoin transactions (Solvency, Existence, No Double-Spending, etc.).
* **⏳ Mempool Logic:** Manages unconfirmed transactions and prevents "race attacks" using a "first-seen" policy.
* **⛏️ Mining Simulation:** Simulates block creation, fee collection, and global state updates.
* **🧪 Test Suite:** Includes mandatory test scenarios for double-spending, negative outputs, and complex chains.

---

## 📂 Repository Structure

```text
CS216-TeamName-UTXO-Simulator/
├── 📄 README.md           # This documentation file
├── 📄 sample_output.txt   # Execution log / Screenshots
└── 📂 src/                # Source Code
    ├── 🐍 main.py         # 🚀 Entry point (CLI Menu)
    ├── 🐍 utxo_manager.py # 🏦 Manages the state of coins
    ├── 🐍 mempool.py      # ⏳ Handles pending transactions
    ├── 🐍 transaction.py  # 📝 Structuring & Validation logic
    └── 🐍 miner.py        # ⛏️ Mining & Consensus logic
