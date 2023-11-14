#ifndef NEURALPY_OPENFHEPREREQUISITES_H
#define NEURALPY_OPENFHEPREREQUISITES_H

#include "openfhe.h"
#include "key/key-ser.h"
#include "scheme/ckksrns/ckksrns-ser.h"
#include "cryptocontext-ser.h"

using namespace lbcrypto;

typedef CCParams<CryptoContextCKKSRNS> Parameters;
typedef CryptoContext<DCRTPoly> Context;
typedef Ciphertext<DCRTPoly> Cipher;

#endif //NEURALPY_OPENFHEPREREQUISITES_H
