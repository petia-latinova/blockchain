#include <iostream>
#include <vector>
#include "cryptography.hpp"
using namespace std;

#ifndef BLOCK_HPP
#define BLOCK_HPP

class Block {
  private:
    string data;
    string hash;

  public:
    Block(string _data, Cryptography _signer) {
      data = _data;
      hash = _signer.signature(data);
    }

    string getData() {
      return data;
    }

   string getHash() {
      return hash;
    }

    // TODO: refactor
    void print() {
      cout << data << ", ";
      print_hex((unsigned char*)hash.c_str(), hash.size());
    }
};

#endif