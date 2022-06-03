#include <iostream>
#include <vector>
#include "cryptography.hpp"
using namespace std;

#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

class Blockchain {
  private:
    vector<Block> _blockchain;
    Cryptography signer;

  public:
    Blockchain(Cryptography _signer) {
      signer = _signer;
    }

    void addBlock(Block b) {
      if(signer.verify(b.getData(), b.getHash())) {
        _blockchain.push_back(b);
      }
      else {
        cout << "ERROR: Wrong signature" << endl;
      }
    }

    void previewByIndex(int i) {
      _blockchain[i].print();
    }

    void previewAllData() {
      for (Block x : _blockchain)
        x.print();
    }

    Block getBlockByIndex(int i) {
      return _blockchain[i];
    }

    vector<Block>::iterator begin() {
      return _blockchain.begin();
    }
    vector<Block>::iterator end() {
      return _blockchain.end();
    }
};

#endif