//
// Created by lchenke on 10.08.23.
//

#ifndef NEURALPY_PYTHONCONTEXT_H
#define NEURALPY_PYTHONCONTEXT_H

#include "OpenFHEPrerequisites.h"

#include "PythonCiphertext.h"
#include "PythonKeys.h"
#include "../../NeuralOFHE/src/LinTools.h"


/***
 *  Class around the CKKS context object. This was written do to issues with 
 *  large OpenFHE templates.
*/
class PythonContext {
public:
    /***
     * Standard constructor that does nothing.
    */
    PythonContext () {}

    /***
     * Setter function to set the wrapped underlying OpenFHE context object
     * 
     * @param cont pointer to 
    */
    void SetContext(Context cont) {
        this->context = cont;
    }

    /***
     * Method to enable FHE features of the circuit
     * 
     * @param feature Corresponding enum to the feature
    */
    void Enable(PKESchemeFeature feature) {
        context->Enable(feature);
    }

    /***
     * Method that generates multiplication keys within the context object.
     *
     * @param privateKey Mult. keys are generated from the private key
     */
    void EvalMultKeyGen (PythonKey<PrivateKey<DCRTPoly>> privateKey) {
        context->EvalMultKeyGen(privateKey.getKey());
    }

    /***
     * Method to generate Bootstrapping keys
     *
     * @param privateKey private key of the circuit
     */
    void EvalBootstrapKeyGen (PythonKey<PrivateKey<DCRTPoly>> privateKey) {
        uint32_t slots = context->GetEncodingParams()->GetBatchSize();

        context->EvalBootstrapSetup(levelBudget, {0, 0}, slots);
        context->EvalBootstrapKeyGen(privateKey.getKey(), slots);
    }

    /***
     * Method to add two chiphertexts
     * 
     * @param a first ciphertext
     * @param b second ciphertext
     * @return a + b
     */
    PythonCiphertext EvalAdd (PythonCiphertext a, PythonCiphertext b) {
        PythonCiphertext result;
        Cipher ciph_result = context->EvalAdd(a.getCiphertext(), b.getCiphertext());
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Method to add a plaintext to a ciphertext
     * 
     * @param a Plaintext
     * @param b Ciphertext
     * @return a + b
     */
    PythonCiphertext EvalAdd (std::vector<double> a, PythonCiphertext b) {
        PythonCiphertext result;
        Plaintext pl = context->MakeCKKSPackedPlaintext(a);
        Cipher ciph_result = context->EvalAdd(pl, b.getCiphertext());
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Method for adding a constant to a ciphertext
     * 
     * @param a Constant 
     * @param b Ciphertext
     * @return a + b
    */
    PythonCiphertext EvalAdd (double a, PythonCiphertext b) {
        PythonCiphertext result;
        Cipher ciph_result = context->EvalAdd(a, b.getCiphertext());
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Method to subtract one ciphertext from another
     * 
     * @param a first ciphertext
     * @param b second ciphertext
     * @return a - b
    */
    PythonCiphertext EvalSub (PythonCiphertext a, PythonCiphertext b) {
        PythonCiphertext result;
        Cipher ciph_result = context->EvalSub(a.getCiphertext(), b.getCiphertext());
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Method to subtract a ciphertext from a plaintext a. Introduced a third 
     * parameter in order to do one less overload
     * 
     * @param a Plaintext
     * @param b Ciphertext
     * @param reverse If set to true, operation will compute the negative
     * @return a - b if reverse is false, b - a otherwise
    */
    PythonCiphertext EvalSub (std::vector<double> a, PythonCiphertext b, bool reverse=false) {
        PythonCiphertext result;
        Plaintext pl = context->MakeCKKSPackedPlaintext(a);
        Cipher ciph_result;
        if (!reverse) {
            ciph_result = context->EvalSub(pl, b.getCiphertext());
        }else {
            ciph_result = context->EvalSub(b.getCiphertext(), pl);
        }
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Overload for subtraction of a ciphertext from a constant value.
     * 
     * @param a Constant
     * @param b Ciphertext
     * @param reverse If set to true it will compute negative of the operation
     * @return a - b if reverse is false, b - a otherwise
    */
    PythonCiphertext EvalSub (double a, PythonCiphertext b, bool reverse=false) {
        PythonCiphertext result;
        Cipher ciph_result;
        if (!reverse) {
            ciph_result = context->EvalSub(a, b.getCiphertext());
        }else {
            ciph_result = context->EvalSub(b.getCiphertext(), a);
        }
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Overload of ciphertext ciphertext multiplication
     *
     * @param a Ciphertext
     * @param b Ciphertext
     * @return a * b
     */
    PythonCiphertext EvalMult (PythonCiphertext a, PythonCiphertext b) {
        PythonCiphertext result;
        Cipher ciph_result = context->EvalMult(a.getCiphertext(), b.getCiphertext());
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Overload of plaintext ciphertext multiplication
     *
     * @param a Plaintext
     * @param b Ciphertext
     * @return a * b
     */
    PythonCiphertext EvalMult (std::vector<double> a, PythonCiphertext b) {
        PythonCiphertext result;
        Plaintext pl = context->MakeCKKSPackedPlaintext(a);
        Cipher ciph_result = context->EvalMult(pl, b.getCiphertext());
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Overload of double, ciphertext multiplication
     *
     * @param a
     * @param b
     * @return a * b
     */
    PythonCiphertext EvalMult (double a, PythonCiphertext b) {
        PythonCiphertext result;
        Cipher ciph_result = context->EvalMult(a, b.getCiphertext());
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Method for doing plaintext matrix, ciphertext vector multiplication.
     * 
     * @param matrix Plain matrix
     * @param vec Cipher vector
     * @return vec . matrix
    */
    PythonCiphertext EvalMatMul (matVec matrix, PythonCiphertext vec, bool parallel = true) {
        PythonCiphertext result;
        Cipher ciph_result = matrix_multiplication(matrix, vec.getCiphertext(), context, parallel);
        result.setCiphertext(ciph_result);

        return result;
    }

    /***
     * Generate rotation keys required to do matrix multiplication with the contexts batch size.
     *
     * @param key Private key of the circuit
     */
    void GenRotations (PythonKey<PrivateKey<DCRTPoly>> key) {
        std::vector<int> rotations = GetRotations(context->GetEncodingParams()->GetBatchSize());
        context->EvalRotateKeyGen(key.getKey(), rotations);
    }

    /***
     * Get dimension of the polynomial ring within the context.
     *
     * @return Ring dimension
     */
    uint32_t GetRingDim() {
        return context->GetRingDimension();
    }

    /***
     * Encrypt a plaintext using the public key.
     *
     * @param plaintext Plaintext that should be encrypted.
     * @param publicKey Public key of the application.
     * @return Encrypted ciphertext.
     */
    PythonCiphertext Encrypt(std::vector<double> plaintext, PythonKey<PublicKey<DCRTPoly>> publicKey) {
        Plaintext pl = context->MakeCKKSPackedPlaintext(plaintext);
        uint32_t batchSize = context->GetEncodingParams()->GetBatchSize();
        PythonCiphertext result;
        result.setCiphertext(context->Encrypt(publicKey.getKey(), pl));
        result.getCiphertext()->SetSlots(batchSize);

        auto len = plaintext.size();
        auto size = std::make_shared<MetadataTest>();
        size->SetMetadata(std::to_string(len));

        MetadataTest::StoreMetadata<DCRTPoly>(result.getCiphertext(), size);

        return result;
    }

    /***
     * Decrypt a ciphertext using the private key.
     *
     * @param cipher Ciphertext that should be decrypted.
     * @param privateKey Private key of the application.
     * @return Plaintext object resulting from the encryption.
     */
    PythonPlaintext Decrypt(PythonCiphertext cipher, PythonKey<PrivateKey<DCRTPoly>> privateKey) {
        PythonPlaintext result;
        Plaintext pl;

        context->Decrypt(privateKey.getKey(), cipher.getCiphertext(), &pl);
        std::string meta = MetadataTest::GetMetadata<DCRTPoly>(cipher.getCiphertext())->GetMetadata();
        int size = std::stoi(meta);

        pl->SetLength((unsigned) size);
        result.setPlaintext(pl);

        return result;
    }

    /***
     * Packing a C++ iterator containing doubles into a plaintext object.
     *
     * @param plaintext Plaintext in form of a C++ iterator
     * @return Plaintext object
     */
    PythonPlaintext PackPlaintext(std::vector<double> plaintext) {
        PythonPlaintext result;
        result.setPlaintext(context->MakeCKKSPackedPlaintext(plaintext));
        return result;
    }

    /***
     * Generate keypair for public key encryption.
     *
     * @return Keypair object
     */
    PythonKeypair KeyGen () {
        PythonKeypair keys;
        auto keyPair = context->KeyGen();

        keys.privateKey.setKey(keyPair.secretKey);
        keys.publicKey.setKey(keyPair.publicKey);

        return keys;
    }

    Context getContext() {
        return context;
    }

    /***
     * Load context object from file.
     *
     * @param filePath
     */
    void load(std::string filePath) {
        if (!Serial::DeserializeFromFile(filePath, context, SerType::BINARY)) {
            std::cerr << "Error loading context" << std::endl;
            exit(1);
        }

        if (Operator::getVerbosity())
            std::cout << "Context was loaded." << std::endl;
    }

    /***
     * Load multiplication keys from file into the context object.
     *
     * @param filePath
     */
    void loadMultKeys(std::string filePath) {
        context->ClearEvalMultKeys();

        std::ifstream multKeyIStream(filePath, std::ios::in | std::ios::binary);
        if (!multKeyIStream.is_open()) {
            std::cerr << "Error opening mult. key file. at " << filePath << std::endl;
            exit(1);
        }
        if (!context->DeserializeEvalMultKey(multKeyIStream, SerType::BINARY)) {
            std::cerr << "Error loading mult. key." << std::endl;
            exit(1);
        }

        if (Operator::getVerbosity())
            std::cout << "Deserialized mult. key" << std::endl;
        multKeyIStream.close();
    }

    /***
     * Load rotation keys from file into the context object.
     *
     * @param filePath
     */
    void loadRotKeys (std::string filePath) {
        context->ClearEvalAutomorphismKeys();

        std::ifstream rotKeyIStream(filePath, std::ios::in | std::ios::binary);
        if (!rotKeyIStream.is_open()) {
            std::cerr << "Error opening rot. key file at " << filePath << std::endl;
            exit(1);
        }
        if (!context->DeserializeEvalAutomorphismKey(rotKeyIStream, SerType::BINARY)) {
            std::cerr << "Error loading rot. key." << std::endl;
            exit(1);
        }

        if (Operator::getVerbosity())
            std::cout << "Deserialized rot. key" << std::endl;
        rotKeyIStream.close();
    }

    /***
     * Serialize context object without keys to file.
     *
     * @param filePath
     */
    void save(std::string filePath) {
        if (!Serial::SerializeToFile(filePath, context, SerType::BINARY)) {
            std::cerr << "Error serializing context." << std::endl;
            exit(1);
        }

        if (Operator::getVerbosity())
            std::cout << "Cryptocontext serialized!" << std::endl;
    }

    /***
     * Serialize multiplication keys to file.
     *
     * @param filePath
     */
    void saveMultKeys(std::string filePath) {
        std::ofstream multKeyFile(filePath, std::ios::out | std::ios::binary);
        if (multKeyFile.is_open()) {
            if (!context->SerializeEvalMultKey(multKeyFile, SerType::BINARY)) {
                std::cerr << "Error serializing multiplication key." << std::endl;
                std::exit(1);
            }

            if (Operator::getVerbosity())
                std::cout << "Multiplication key serialized!" << std::endl;
            multKeyFile.close();

        } else {
            std::cerr << "Error opening Mult Key file at " << filePath << std::endl;
        }
    }

    /***
     * Serialize rotation keys to file.
     *
     * @param filePath
     */
    void saveRotKeys(std::string filePath) {
        std::ofstream rotKeyFile(filePath, std::ios::out | std::ios::binary);
        if (rotKeyFile.is_open()) {
            if (!context->SerializeEvalAutomorphismKey(rotKeyFile, SerType::BINARY)) {
                std::cerr << "Error serializing rotation key." << std::endl;
                std::exit(1);
            }

            if (Operator::getVerbosity()) {
                std::cout << "Rotation key serialized!" << std::endl;
            }
            rotKeyFile.close();
        } else {
            std::cerr << "Error opening Rot. Key file at " << filePath << std::endl;
        }
    }

    bool hasRelinKeys() {
        auto KeyMap = this->context->GetAllEvalMultKeys();

        if (Operator::getVerbosity()) {
            std::cout << "Context has " << KeyMap.size() << " relin keys." << std::endl;
            std::cout << "Names:" << std::endl;
            for (const auto& pair : KeyMap)
                std::cout << "\t" << pair.first << std::endl;
        }

        return KeyMap.size() != 0;
    }

    bool hasGaloisKeys() {
        auto KeyMap = this->context->GetAllEvalAutomorphismKeys();

        if (Operator::getVerbosity()) {
            std::cout << "Context has " << KeyMap.size() << " automorphism keys." << std::endl;
            std::cout << "Names:" << std::endl;
            for (const auto& pair : KeyMap)
                std::cout << "\t" << pair.first << std::endl;
        }

        return KeyMap.size() != 0;
    }

    static void setLevelBudget(std::vector<uint32_t> budget) {
        levelBudget = budget;
    }

private:
    Context context;

    static std::vector<uint32_t> levelBudget;
};

std::vector<uint32_t> PythonContext::levelBudget = {};

#endif //NEURALPY_PYTHONCONTEXT_H
