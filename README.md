# Chameleon
A encryption algorithm written in C++ with a lot of bitwise operations

## How it works
Chameleon is based on the fact that characters can be seen as numbers. It also uses a lot of bitwise operators and simple mathematical operations.
Chameleon is also a very fast encryption algorithm, that can ecrypt around 40 megabytes per second.
The operations done in the encryption algorithm also work for the decryption one. The only difference between the 2 is that the encryption algorithm
starts from 0 gets to the string length divided by 2 and the decryption algorithm starts from the string length divided by 2 and gets to 0.
Chameleon gets to half of the string length both for more unpredictability and optimization. Since the 2 half of the string are encrypted in a different way
the algorithm will be more unpredictable and the time will be less, since it does half of the iterations.

### Key and operstions
The chameleon encryption is secure and unpredictable, using a 32 to 64 bit key, and a constant for key expansion and encryption.
The basic encryption conprehends around 4 operations, and uses a function called C, which is used for modifying singular numbers. 

### A and 0x706080AF
Chameleon also uses an array called A, for encrypting it's characters. The array is 2d and has a total of 32 entries (4 columns, 8 entries per column).
It is filled at the beginning of the encryption, with a total of 32 iterations, which is the size of the array.
A is filled using the talked about before constant 0x706080AF and talking about this constant, it is made from subtracting 0x1706080AF (the first 10 digits
of the golden ratio without the 1 in hexadecimal) and 0x100000000 (2^32 (the size of the array) in hexadecimal).
This constant is used because of it's unpredictability and oddness.

### Key expansion
The chameleon key, is initially a key between 16-32 bits key but gets then expanded to be a 32-64 bits key.
The key expansion is done before the encryption begins, and it's done by making 4 operations on K (the key).
The operations comprehend the 0x706080AF constant and the A array.
After 3 operations the key is 128 bits, but gets then turned into a 32-64 bits key by dividing it by 10e10.
