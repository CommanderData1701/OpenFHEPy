#ifndef NEURALPY_PYTHONCIPHERTEXT_H
#define NEURALPY_PYTHONCIPHERTEXT_H

#include "OpenFHEPrerequisites.h"

/***
 * Python-Ciphertext class which was written due to issues with long templates in
 * OpenFHE.
*/
class PythonCiphertext {
public:
    /***
     * Empty constructor that does nothing, since ciphertext objet should be set 
     * with setter method.
    */
    PythonCiphertext () {}

    /***
     * Setter method for the underlying OpenFHE CKKS ciphertext object
     * 
     * @param cipher OpenFHE ciphertext object.
    */
    void setCiphertext(Cipher cipher) {
        ciphertext = cipher;
    }

    /***
     * Getter method for the underlying OpenFHE CKKS ciphertext object
     * 
     * @return OpenFHE ciphertext
    */
    Cipher getCiphertext () {
        return ciphertext;
    }

    /***
     * Method that allows a ciphertexts to be serialized from a file.
     *
     * @param filePath Path to Ciphertext file
     */
    void load(std::string filePath) {
        if (!Serial::DeserializeFromFile(filePath, ciphertext, SerType::BINARY)) {
            std::cerr << "Could not deserialize " + filePath + " ciphertext" << std::endl;
            exit(1);
        } else if (Operator::getVerbosity())
            std::cout << "Ciphertext " + filePath << " deserialized." << std::endl;
    }

    /***
     * Method that allows serialization of a ciphertext.
     *
     * @param filePath
     */
    void save(std::string filePath) {
        if(!Serial::SerializeToFile(filePath, ciphertext, SerType::BINARY)) {
            std::cerr << "Error Serializing ciphertext." << std::endl;
            exit(1);
        } else if (Operator::getVerbosity())
            std::cout << "Ciphertext serialized." << std::endl;
    }

private:
    Cipher ciphertext;
};

#endif //NEURALPY_PYTHONCIPHERTEXT_H
