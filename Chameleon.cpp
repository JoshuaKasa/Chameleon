#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <cmath>
#define ui unsigned int
#define si short int
#define ul unsigned long
#define ull unsigned long long
#define l long
#define ll long long
#define TS(WORD) to_string(WORD)
#define TI(STRING) stoi(STRING)
#define LOOPIE(i, n) for (int i = 0; i <= n; i++)
#define LOOPI(i, n) for (int i = 0; i < n; i++)
#define LOOPDE(i, n) for (int i = n; i >= 0; i--)
#define LOOPD(i, n) for (int i = n; i > 0; i--)
#define PRINTA(ARR) for (int i = 0; i < sizeof(ARR)/sizeof(*ARR); i++) cout << ARR[i] << " "

using namespace std;

// Modern random range function for key initialization
int rndrange(int min_num, int max_num)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(min_num, max_num);

    return distr(gen);
}

// Array of number generated using 0x706080AF (0x1706080AF - 0x100000000), first 10 digits of golden ration (without 1) minus 2^32 (total entries of the array)
int A[4][8]; // 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
ui KA[32];

/*
    Simple XOR int encryption function that takes in an integer X and outputs a scrambled version of X.
    The function works by first adding the values in the first and third rows of the A array together and XORing them with
    the value in the second column of the A array.
    This value is then added to the value in the fourth column of the A array.
    Finally, the value in the first column of the A array is XORed with the value in the third column of the A array
    and this value is returned as the scrambled output.
*/
int C(int X)
{
    int B = A[0][X >> 32 & 8] + A[3][X >> 16 & 4];
    return ((B ^ A[X % 3][X >> 8 & 2] + (X & 32)) ^ 16) >> (X % 12);
}

// Encryption function
string chameleon_encrypt(string *SS, const int *K) // S = message to encrypt; K = key
{
    string S = *SS;
    int SL = S.length(); // Message length
    int KL = TS(*K).length(); // Key length

    // Looping through half of the message, faster and more unpredictable, since the 2 half use a different encryption method
    LOOPIE(i, SL/2)
    {
        int u = SL/2 + i; // Right half counter
        char L = S[i]; // Left half
        char R = S[u]; // Right half
        char LB = S[i - 1]; // Left half character before
        char RB = S[u - 1]; // Right half character before
        // The 2 before characters are used so that there are not repeating pattern during the encryption

        // Looping 4 times over each character
        LOOPI (k, 4) // Looping 4 times because the size of A is 4
        {
            si T = A[k][k + 1] + i; // Temporary variable used to store the operations of A[k][k + 1] + i

            // Simple xoring encryption for left and right characters
            L ^= T + *K; // Key usage
            R ^= T + C(T);
        }
        // Using the 2 before characters for non repeating patterns
        // By mixing LB (Left character Before) and RB (Right character Before) we get unpredictable patterns
        L ^= C(A[3][KL - 1]) + (LB ^ RB);
        R ^= A[0][1] * *K + (RB ^ LB); // Key usage

        // Assigning encrypted characters to the string
        S[i] = L;
        S[u] = R;
    }

    return S; // Returning the string
}

// Decryption algorithm
string chameleon_decrypt(string *SS, const int *K)
{
    string S = *SS;
    int SL = S.length();
    int KL = TS(*K).length();

    LOOPDE(i, SL/2)
    {
        int u = SL/2 + i;
        char L = S[i];
        char R = S[u];
        char LB = S[i - 1];
        char RB = S[u - 1];

        LOOPI (k, 4)
        {
            si T = A[k][k + 1] + i;

            L ^= T + *K;
            R ^= T + C(T);
        }
        L ^= C(A[3][KL - 1]) + (LB ^ RB);
        R ^= A[0][1] * *K + (RB ^ LB);

        S[i] = L;
        S[u] = R;
    }

    return S;
}

int main()
{
    string SS = "Hello!"; // Plain text
    int KK = rndrange(10000000, 99999999); // Key initial creation
    int KL = TS(KK).length(); // Key initial length

    /*
        Filling the A array with values based on the (explained before) 0x706080AF number.
        This number is used since it is a very odd number (see: https://en.wikipedia.org/wiki/Nothing-up-my-sleeve_number).
    */
    LOOPI (i, 4)
    {
        LOOPI (k, 8)
        {
            A[i][k] += 0x706080AF << (16 + (k + i));
            A[i][k] -= 0x706080AF ^ C(0x706080AF);
            A[i][k] = abs(A[i][k]);
        }
    }

    // Key expansion
    /*
        With this 256 iterations we can create a number that has around 200 digits (1024-bit number).
        This number will then be split into 32 parts and each part will be assigned to the KA array.
        Each of this numbers will later be used inside the encryption and decryption functions (32-bit number).
    */
    string KKK;
    string MK = TS(KK);
    int KEK = 0;

    // Expansion loop
    LOOPI (i, 64) // 64 iterations
    {
        for (si j = 0; j < 8; j += 2) // 4 iterations
        {
            KEK ^= 0x706080AF >> TI(TS(MK[j])); // XOR operations and use of 0x706080AF
        }
        KKK += TS(KEK ^ i);
    }
    // Splitting loop
    int j = 0;
    for (int i = 0; i < KKK.length(); i += ceil(KKK.length() / 31)) // 32 iterations
    {
        KA[j] = TI(KKK.substr(i, ceil(KKK.length() / 31))); // Assigning each 32-bit number to the KA array
        j++; // j is used because i inside the array goes from 0 to the length of the key, so even if there are 8 iteration i will be much higher
    }

    // Getting encryption time
    clock_t begin = clock();

    SS = chameleon_encrypt(&SS, &KK);
    SS = chameleon_decrypt(&SS, &KK);

    clock_t end = clock();

    cout << "Encrypted: " << SS << "\nLength: " << SS.length() << endl;
    cout << "Key: " << KKK << "\nKey length: " << KKK.length() << endl;
    cout << "Time: " << (double)(end - begin) / CLOCKS_PER_SEC;

    return 0;
}