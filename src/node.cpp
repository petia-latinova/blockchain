#include <iostream>
#include <vector>

#include "cryptography.hpp"
#include "block.hpp"
#include "blockchain.hpp"

using namespace std;

int main() {
  Cryptography signer;
  Blockchain chain(signer);

  chain.addBlock(Block("data1", signer));
  chain.addBlock(Block("data2", signer));
  chain.addBlock(Block("data3", signer));

  chain.previewAllData();

  cout << endl;

  return 0;
}
