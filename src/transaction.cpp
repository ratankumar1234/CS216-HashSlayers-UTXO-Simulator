#ifndef TRANSACTION_CPP
#define TRANSACTION_CPP
#include <string>
#include <vector>
using namespace std;

struct Input {
    string prev_tx; 
    int index;      
    string owner;   
};
struct Output {
    double amount;  
    string address; 
};
struct Transaction {
    string tx_id;   
    vector<Input> inputs;
    vector<Output> outputs;
    double fee;     
};

#endif