#include <iostream>
#include <numeric>
#include <tuple>

using namespace std;

bool isValidE(uint32_t e, uint32_t Carmichael){
    if(e<Carmichael && gcd(e, Carmichael) == 1){
        return true;
    }
    else{
        return false;
    }
}

// returns mod inverse of a with respect to m
// using Extended Euclid Algorithm
// assumption: a and m are coprimes: gcd(a, m) = 1
int modInverse(int a, int m){
    int m0 = m;
    int y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        // q is quotient
        int q = a / m;
        int t = m;

        // m is remainder now, process same as
        // Euclid's algo
        m = a % m, a = t;
        t = y;

        // Update y and x
        y = x - q * y;
        x = t;
    }

    // make x positive
    if (x < 0)
        x += m0;

    return x;
}

int main(){

    // randomly generate large primes p and q with a CSRNG, p != q
    uint32_t p = 7919;
    uint32_t q = 5693;

    // RSA modulus n
    uint32_t n = p * q;
    uint32_t Carmichael = lcm(p-1, q-1);

    // encryption exponent e
    // choose an integer e such that 1 < e < λ(n) and gcd(e, λ(n)) = 1; that is, e and λ(n) are coprime.
    // 65537 is chosen for efficiency as a Fermat prime, 2^2^m + 1 with m = 4
    uint32_t e = 65537;

    // decryption exponent d, must satisfy e*d = 1(mod λ(n))
    uint32_t d = modInverse(e, Carmichael);

    tuple<uint32_t, uint32_t> privateKey (e, n);
    tuple<uint32_t, uint32_t> publicKey (d, n);

    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    cout << "e = " << e << endl;
    if(isValidE(e, Carmichael)){
        cout << "VALID choice for e for chosen p and q" << endl;
    }
    else{
        cout << "INVALID choice for e for chosen p and q" << endl;
        // if invalid, rechoose p and/or q, or choose new e. 257, 17, 5 and 3 are the other Fermat primes
    }
    cout << "n = pq = " << n << endl;
    cout << "Carmichael = lcm(p-1, q-1) = " << Carmichael << endl;
    cout << "d = " << d << endl;

}