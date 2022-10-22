#include <iostream>
#include <string>
#include <ctime>
#include <random>

using namespace std;

int rndrange(int min_num, int max_num)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(min_num, max_num);

    return distr(gen);
}

int A[4][8];

int C(int X)
{
    int B = A[0][X >> 32 & 8] + A[3][X >> 16 & 4];
    return ((B ^ A[X % 4][X >> 8 & 2] + (X & 32)) ^ 16) >> (X % 12);
}

string chameleon_encrypt(string S, int K)
{
    int SL = S.length();
    int KL = to_string(K).length();

    for (int i = 0; i <= SL/2; i++)
    {
        int u = SL/2 + i;
        char L = S[i];
        char R = S[u];
        char LB = S[i - 1];
        char RB = S[u - 1];

        for (short int k = 0; k < 4; k++)
        {
            short int T = A[k][k + 1] + i;

            L ^= T + K;
            R ^= T + C(T);
        }
        L ^= C(A[4][KL - 1]) + (LB ^ A[2][2]);
        R ^= A[0][1] * K + (RB ^ A[3][3]);

        S[i] = L;
        S[u] = R;
    }

    return S;
}

string chameleon_decrypt(string S, int K)
{
    int SL = S.length();
    int KL = to_string(K).length();

    for (int i = SL/2; i >= 0; i--)
    {
        int u = SL/2 + i;
        char L = S[i];
        char R = S[u];
        char LB = S[i - 1];
        char RB = S[u - 1];

        for (short int k = 0; k < 4; k++)
        {
            short int T = A[k][k + 1] + i;

            L ^= T + K;
            R ^= T + C(T);
        }
        L ^= C(A[4][KL - 1]) + (LB ^ A[2][2]);
        R ^= A[0][1] * K + (RB ^ A[3][3]);

        S[i] = L;
        S[u] = R;
    }

    return S;
}

int main()
{
    string SS = "Hello world!";
    long long int KK = rndrange(100000,10000000);
    int KL = to_string(KK).length();

    for (short int i = 0; i < 4; i++)
    {
        for (short int k = 0; k < 8; k++)
        {
            A[i][k] += 0x706080AF << 16 + (k + i);
            A[i][k] -= 0x706080AF ^ C(0x706080AF);
            A[i][k] = abs(A[i][k]);
        }
    }

    clock_t begin = clock();

    SS = chameleon_encrypt(SS, KK);

    clock_t end = clock();

    cout << "Encrypted: " << SS << "\nLength: " << SS.length() << endl;
    cout << "Key: " << KK << "\nKey bits: " << KL << endl;
    cout << "Time: " << (double)(end - begin) / CLOCKS_PER_SEC;

    return 0;
}