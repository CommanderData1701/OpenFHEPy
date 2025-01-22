/**
 * @file WrapperFunctions.h
 *
 * @brief This file includes definitions that allow usage of all C++ functions needed to operate the python library.
 *
 * @author Anonymous
 *
 */

#ifndef NEURALPY_WRAPPERFUNCTIONS_H
#define NEURALPY_WRAPPERFUNCTIONS_H

#include "WrapperClasses.h"
#include "NeuralOFHE/NeuralOFHE.h"


/***
 * Python function to set the context variable.
 *
 * @param context
 */
void SetPythonContext (PythonContext context) {
    SetContext(context.getContext());
}


/***
 * Python factory to create Context object out of a parameter object.
 *
 * @param params Parameter object.
 * @return Context object.
 */
PythonContext MakeContext(Parameters params) {
    auto context = GenCryptoContext(params);

    PythonContext result;
    result.SetContext(context);

    return result;
}

/***
 * Get Context of a Ciphertext
 *
 * @param cipher
 * @return Context with which the ciphertext was created.
 */
PythonContext GetContext(PythonCiphertext cipher) {
    Context context = cipher.getCiphertext()->GetCryptoContext();
    PythonContext pyContext;
    pyContext.SetContext(context);

    return pyContext;
}

/***
 * Setting verbosity of the application
 *
 * @param verbose
 */
void SetVerbosity(bool verbose) {
    Operator::setVerbosity(verbose);
}

/***
 * Calculating mulitplication Depth required for bootstrapping
 *
 * @param 
 */
uint32_t GetBootStrapDepth(uint32_t approxDepth,std::vector<uint32_t> levelBudget, SecretKeyDist secretKeyDist) {
    PythonContext::setLevelBudget(levelBudget);
    return FHECKKSRNS::GetBootstrapDepth(approxDepth, levelBudget, secretKeyDist);
}


#endif //NEURALPY_WRAPPERFUNCTIONS_H
