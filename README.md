# AES
[Sue Anne Davis](https://github.com/tiramisueanne), [Akash Kwatra](https://github.com/akashkw), [Benny Singer](https://github.com/bzsinger)

This project is an implementation of the [Advanced Encryption Standard](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard) (AES), built according to NIST standards as described in [FIPS Publication 197](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf).

To learn more about AES, see:
* [Bill Young's AES assignment specifications ](https://www.cs.utexas.edu/~byoung/cs361/assignment-aes.html)
* [A Stick Figure Guide to the Advanced Encryption Standard (AES)](http://www.moserware.com/2009/09/stick-figure-guide-to-advanced.html)
* [AES Simplified](https://www.ime.usp.br/~rt/cranalysis/AESSimplified.pdf)

## Algorithm
The AES algorithm encrypts a message in fixed-size blocks using shared key of length 128, 192, or 256 bits. Based on the key size used, AES runs a number of "rounds," which each includes substitution, transposition, and mixing steps. Our implementation only employs keys of length 128 and 256 bits.

We implement the AES algorithm in ```aes.cc```. Plaintext for encrypting and encrypted text for decrypting are passed into the ```AES``` class's ```encrypt_this``` and ```decrypt_this``` methods via an ```encrypt.cc```-defined text-based user interface.

We implement three classes, ```KeyMaster```, which handles everything related to the given key, including rotation, addition, and constants that rely on key length; ```EasyWord```, which performs operations on words, such as getting and setting byte values; and ```AES```, which performs encryption and decryption.

### KeyMaster

### EasyWord

### AES

## Running AES

Our implementation runs using a text-based user interface. To run the project, first compile the code using ```make``` and then run the executable using ```./aes``` with proper command line arguments.
```
$ git clone git@github.com:tiramisueanne/AES.git
$ cd AES
$ make
$ ./aes --keyfile <keyfile> --inputfile <inputfile> --outputfile <out> --mode <encrypt/decrypt>
```

This project was built according to the specifications detailed in the CS 361: Introduction to Computer Security [AES rubric](./misc/aes.pdf) and submitted for credit.

## Running tests

Our test suite uses Google's [Google Test](https://github.com/google/googletest) C++ testing framework. To install Google Test on a Mac computer, follow the instructions in the [test installation README](./misc/test-install.md).

To run the tests, simply run:
```
$ make test
```
