# Blockchain

**Task 1 Block producer**
- append only linked list of data;  
- fn: add new data;  
- fn: preview by index;  
- fn: preview whole chain of data;  

**Task 2 Keypair generator and signer**
- generate a keypair (ECDSA);  
- sign a message with that keypair;  
- verify if a signed message is not tempered;  

Clone the repo with the submodule
```bash
$ git clone https://github.com/petia-latinova/blockchain.git --recursive
```
  
To run the project open console in the main directory of the project

setup the project
```bash
./setup.sh
```

compile the project
```bash
./build.sh
```

run
```bash
./build/node
```


The example used for cryptography.hpp:  
https://github.com/bitcoin-core/secp256k1/blob/master/examples/ecdsa.c

The library used for signing:  
https://github.com/bitcoin-core/secp256k1