#ifndef NEURALPY_PYTHONKEYS_H
#define NEURALPY_PYTHONKEYS_H

#include "OpenFHEPrerequisites.h"


/***
 * Class used to represent Private and Public keys in Python.
 *
 * @tparam T Should be either set to PrivateKey<DCRTPoly> or PublicKey<DCRTPoly>
 */
template<class T>
class PythonKey {
public:
    /***
     * Empty constructor that does nothing
    */
    PythonKey() {}

    /***
     * Setter method for the underlying OpenFHE key object
     * 
     * @param k OpenFHE key object
    */
    void setKey(T k) {
        this->key = k;
    }

    /***
     * Getter method for the underlying OpenFHE key object
     * 
     * @return key object
    */
    T getKey() {
        return key;
    }

    /***
     * Method to load key from storage
     * 
     * @param filePath Path to file
    */
    void load(std::string filePath) {
        if (!Serial::DeserializeFromFile(filePath, key, SerType::BINARY)) {
            std::cerr << "Error deserializing key from " << filePath << "." << std::endl;
            exit(1);
        }

        if (Operator::getVerbosity())
            std::cout << "Key deserialized from " << filePath << "." << std::endl;
    }

    /***
     * Method to save key to storage
     * 
     * @param filePath Path to file
    */
    void save(std::string filePath) {
        if (!Serial::SerializeToFile(filePath, key, SerType::BINARY)) {
            std::cerr << "Error serializing key to " << filePath << "." << std::endl;
            std::exit(1);
        }

        if (Operator::getVerbosity())
            std::cout << "Key serialized to " << filePath << "." << std::endl;
    }

private:
    T key;
};


/***
 * Class to represent a keypair in Python. Similar to OpenFHE the publicKey and privateKey can be read and written to
 * from outside the class.
 */
class PythonKeypair {
public:
    PythonKeypair () {}

    PythonKey<PublicKey<DCRTPoly>> publicKey;

    PythonKey<PrivateKey<DCRTPoly>> privateKey;

private:
    KeyPair<DCRTPoly> keys;
};


class PythonPlaintext {
public:
    PythonPlaintext () {}

    void setPlaintext(Plaintext plain) {
        this->pl = plain;
    }

    Plaintext getPlaintext() {
        return pl;
    }

    /***
     * Setting the size of the plaintext vector.
     *
     * @param length
     */
    void SetLength (uint32_t length) {
        pl->SetLength(length);
    }

    /***
     * Getting the plaintext value as a C++ iterator. Conversion between C++ iterators and Python iterators is seamless
     * with Pybind11.
     *
     * @return Plain vector
     */
    std::vector<double> GetPackedValue () {
        return pl->GetRealPackedValue();
    }

private:
    Plaintext pl;
};

#endif //NEURALPY_PYTHONKEYS_H
