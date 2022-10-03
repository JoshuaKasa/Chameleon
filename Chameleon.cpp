#include <iostream>
#include <random>
#include <string>
#include <algorithm>

using namespace std;

const int KEY_MIN_LENGTH = 1000; // Min key number
const int KEY_MAX_LENGTH = 99999999; // Max key number
const int CHAMELEON = 0xAF5C9; // First 6 digits of e do hexadecimal

// Modern random range function -> https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
int rndrange(int min_num, int max_num)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(min_num, max_num);

    return distr(gen);
}

int main()
{
    string str = "Hello world!"; // String to encrypt
    int length = str.length(); // Length of the string
    int key = rndrange(KEY_MIN_LENGTH, KEY_MAX_LENGTH); // Generating a key (random number between 1000 and 99999999)
    int key_length = to_string(key).length(); // Getting key length (4-8)

    clock_t begin = clock(); // Starting run time calculator

    for (int i = 0; i < length; i++) // Iterating through every character of the word
    {
        swap(str[length % key_length], str[(int)(CHAMELEON ^ key) % key_length]); // Swapping 2 string characters
        swap(str[length - key_length], str[(key > CHAMELEON) - 1 + i]); // Swapping 2 string characters
        // We swap characters and then get the character for not preventing problems
        char ch = str[i]; // Getting current character of the string

        for (int j = 0; j <= 4; j++) // Iterating 4 times through each character
        {
            // We can see characters as number, and so we can operate on them using bitwise and normal operations
            ch = (int)ch + (key * key);
            ch += ch << (i + j); // i + j makes it so that the same letters are not the same anymore after encryption

            for (int k = 1; k <= 2; k++) // Iterating 2 times every iteration
            {
                // Performing more operation
                if (ch ^ ch * j) ch += (key % ch) * 2;
                else ch += (key * k + i) << 1;
                // Using the key for operations creates unpredictable patterns
            }

            // Operations
            char temp = ch; // Creating a temporary character before overwriting ch

            ch += str[i] >> 2;
            temp += ch & str[i] + key;
            ch += temp << 4;

            if ((key + key_length) % 2 == 0) // Using key again for unpredictability
            {
                ch += (key / 4) >> 2;
                ch <<= 1;
            }

            ch -= (key / key_length) >> 1;
            ch >>= (1 & temp) + 1;

            ch += ch;
        }
        ch = abs(ch); // Turning ch into a positive number if it isn't, this make it easier to perform checks on it

        if (ch == 0 || ch == 10 || ch == 32 || ch == 8 || ch == 7 || ch == 9 || ch == 13) // Checking for blank character (\n, \t, ecc)
        {
            ch >>= 1;
            ch -= floor(ch / 4); // turning ch / 4 into an integer
        }

        str[i] = ch; // Overwriting string character with encrypted one
    }

    clock_t end = clock(); // Ending timer
    // Average MBS encrypted -> 45

    // Printing encryption time, encrypted string and key used
    cout << "Encrypted: " << str << "\nLength: " << str.length() << endl;
    cout << "Key: " << key << endl;
    cout << "Time: " << (double)(end - begin) / CLOCKS_PER_SEC;

    // Working on a decryption algorithm

    return 0;
}
