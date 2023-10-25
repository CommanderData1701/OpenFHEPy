import neuralpy
import numpy as np
from os import listdir
from random import choice 
import matplotlib.pyplot as plt
from time import time
import json


def plain_relu(x: np.array) -> np.array:
    return np.maximum(0, x)


def main() -> None:
    # Choosing random image from dataset
    dirlist = listdir("images")
    filename = choice(dirlist)

    image = np.load("images/" + filename)[0][0]

    copy = image.copy()
    image = list(image.flat)
    initial_size = len(image)

    # Generate context and keypair object in order to load them from file
    context = neuralpy.Context()
    keypair = neuralpy.KeyPair()

    context.load("keys/context")
    context.loadMultKeys("keys/multKeys")
    context.loadRotKeys("keys/rotKeys")

    keypair.publicKey.load("keys/publicKey")
    keypair.privateKey.load("keys/privateKey")

    # Setting context object for the crypto environment
    neuralpy.SetContext(context)

    # Encode image into CKKS plaintext
    plain = context.PackPlaintext(image)

    # Encrypt image
    x = context.Encrypt(plain, keypair.publicKey)
    x.setSlots(initial_size)
    print("Encrypted ciphertext with {} slots".format(x.getSlots()))

    conv_weights, conv_biases = np.load("model/_Conv_0_weights.npy"), np.load("model/_Conv_0_bias.npy")
    gemm0_weights, gemm0_biases = np.load("model/_Gemm_3_w.npy"), np.load("model/_Gemm_3_bias.npy")
    gemm1_weights, gemm1_biases = np.load("model/_Gemm_5_w.npy"), np.load("model/_Gemm_5_bias.npy")

    # Define operations
    operations = [
        neuralpy.Conv2D(conv_weights, conv_biases),
        neuralpy.ReLU(-6.5318193435668945, 8.548895835876465, 3),
        neuralpy.Gemm(gemm0_weights, gemm0_biases),
        neuralpy.ReLU(-14.685586750507355, 12.968225657939911, 3),
        neuralpy.Gemm(gemm1_weights, gemm1_biases),
    ]

    total_time = 0

    # Carrying out operations
    for operation in operations:
        print("Beginning calculation of {}".format(operation.GetName()))
        start = time()
        x = operation(x)
        end = time()

        elapsed = end - start
        total_time += elapsed

        print("Took {}s".format(elapsed))


    output_size = x.getSlots()

    # Decrypt image
    result = context.Decrypt(x, keypair.privateKey)
    result.SetLength(output_size)
    result = result.GetPackedValue()

    plain_output = np.array(image) @ conv_weights + conv_biases
    plain_output = plain_relu(plain_output)
    plain_output = plain_output @ gemm0_weights + gemm0_biases
    plain_output = plain_relu(plain_output)
    plain_output = plain_output @ gemm1_weights + gemm1_biases

    print(f"Cipher output: {result}")
    print(f"Plain output: {plain_output}")
    print("Model predicted integer to be a {} and took {}s".format(result.index(max(result)), total_time))

    plt.imshow(copy, cmap="gray")
    plt.show()


if __name__ == "__main__":
    main()
