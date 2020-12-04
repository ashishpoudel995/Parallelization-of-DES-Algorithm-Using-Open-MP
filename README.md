# Parallelization-of-DES-Algorithm-Using-Open-MP

## 1. Parallel Implementation of DES Algorithm
For the parallel Implementation of DES Algorithm, plain text is read from the plaintext.txt file and stored in a string. The string is divided into strings with length=8 each and stored in string array.
Then, the divided strings stored in arrays are parallely encrypted and decrypted. Vectors are also used for making the process easy.

### Why length=8 is taken for splitting the string?
The ASCII value is taken into consideration and an alphabet corresponds to 8 bit binary when ASCII is taken into consideration.

## 2. S Box Parallelism
S box is separately parallelized and the implementation code is available in Codes folder. S Box parallelism consumed more time than Technique 1. This might be due to thread overhead.

## 3. Nested Parallelism
For Nested Parallelism, Technique 1 and 2 are combined. Nested Parallelism also consumed more time than technique 1. This also might be due to thread overhead.

## 4. Serial Implementation of DES Algorithm
The serial implementation of DES Algorithm is also done and the code is available in Codes folder of the repository.

## Note: Don't forget to create plaintext.txt file and the text to be encrypted and decrypted should be there inside the file. Also, don't forget to download matrices.h and numberConversions.h header files. 
