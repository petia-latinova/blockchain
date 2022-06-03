#include <iostream>
#include <vector>
#include <memory.h>
#include <cassert>
#include <secp256k1.h>
#include "random.h"
using namespace std;

#ifndef CRYPTOGRAPY_HPP
#define CRYPTOGRAPY_HPP

class Cryptography {
private:
  secp256k1_context* ctx;
  unsigned char seckey[32];
  unsigned char compressed_pubkey[33];
  secp256k1_pubkey pubkey;
  
public:
  Cryptography(unsigned char* _seckey, unsigned char* _compressed_pubkey) {
    memcpy(seckey, _seckey, sizeof(seckey));
    memcpy(compressed_pubkey, _compressed_pubkey, sizeof(compressed_pubkey));
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

    /* Deserialize the public key. This will return 0 if the public key can't be parsed correctly. */
    if (!secp256k1_ec_pubkey_parse(ctx, &pubkey, compressed_pubkey, sizeof(compressed_pubkey))) {
      printf("Failed parsing the public key\n");
      return;
    }
  }
  
  Cryptography() {
    unsigned char randomize[32];
    size_t len;
    int return_val;

    /* The specification in secp256k1.h states that `secp256k1_ec_pubkey_create` needs
    * a context object initialized for signing and `secp256k1_ecdsa_verify` needs
    * a context initialized for verification, which is why we create a context
    * for both signing and verification with the SECP256K1_CONTEXT_SIGN and
    * SECP256K1_CONTEXT_VERIFY flags. */
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    if (!fill_random(randomize, sizeof(randomize))) {
      printf("Failed to generate randomness\n");
      return;
    }
    /* Randomizing the context is recommended to protect against side-channel
    * leakage See `secp256k1_context_randomize` in secp256k1.h for more
    * information about it. This should never fail. */
    return_val = secp256k1_context_randomize(ctx, randomize);
    assert(return_val);

    /*** Key Generation ***/

    /* If the secret key is zero or out of range (bigger than secp256k1's
    * order), we try to sample a new key. Note that the probability of this
    * happening is negligible. */
    while (1) {
      if (!fill_random(seckey, sizeof(seckey))) {
        printf("Failed to generate randomness\n");
        return;
      }
      if (secp256k1_ec_seckey_verify(ctx, seckey)) {
        break;
      }
    }

    /* Public key creation using a valid context with a verified secret key should never fail */
    return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, seckey);
    assert(return_val);

    /* Serialize the pubkey in a compressed form(33 bytes). Should always return 1. */
    len = sizeof(compressed_pubkey);
    return_val = secp256k1_ec_pubkey_serialize(ctx, compressed_pubkey, &len, &pubkey, SECP256K1_EC_COMPRESSED);
    assert(return_val);
    /* Should be the same size as the size of the output, because we passed a 33 byte array. */
    assert(len == sizeof(compressed_pubkey));
  }

  ~Cryptography() {
    /* This will clear everything from the context and free the memory */
    secp256k1_context_destroy(ctx);
  }

  string signature(string data) {
    int return_val;
    unsigned char serialized_signature[64];
    secp256k1_ecdsa_signature sig;

    /* Generate an ECDSA signature `noncefp` and `ndata` allows you to pass a
    * custom nonce function, passing `NULL` will use the RFC-6979 safe default.
    * Signing with a valid context, verified secret key
    * and the default nonce function should never fail. */
    return_val = secp256k1_ecdsa_sign(ctx, &sig, (unsigned char*)data.c_str(), seckey, NULL, NULL);
    assert(return_val);

    /* Serialize the signature in a compact form. Should always return 1
    * according to the documentation in secp256k1.h. */
    return_val = secp256k1_ecdsa_signature_serialize_compact(ctx, serialized_signature, &sig);
    assert(return_val);

    return string((char*)serialized_signature);
  }

  bool verify(string data, string hash) {
    int is_signature_valid;
    unsigned char serialized_signature[64];
    secp256k1_ecdsa_signature sig;

    memcpy(serialized_signature, hash.c_str(), sizeof(serialized_signature));

    /* Deserialize the signature. This will return 0 if the signature can't be parsed correctly. */
    if (!secp256k1_ecdsa_signature_parse_compact(ctx, &sig, serialized_signature)) {
        printf("Failed parsing the signature\n");
        return 1;
    }

    /* Deserialize the public key. This will return 0 if the public key can't be parsed correctly. */
    if (!secp256k1_ec_pubkey_parse(ctx, &pubkey, compressed_pubkey, sizeof(compressed_pubkey))) {
      printf("Failed parsing the public key\n");
      return 1;
  }

    /* Verify a signature. This will return 1 if it's valid and 0 if it's not. */
    is_signature_valid = secp256k1_ecdsa_verify(ctx, &sig, (unsigned char*)data.c_str(), &pubkey);

    return !(bool)is_signature_valid;
  }
};

#endif