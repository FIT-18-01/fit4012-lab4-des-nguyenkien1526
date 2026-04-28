#include <array>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {

constexpr array<int, 64> IP = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

constexpr array<int, 64> FP = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

constexpr array<int, 48> E = {
    32, 1, 2, 3, 4, 5, 4, 5,
    6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1
};

constexpr array<int, 32> P = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

constexpr array<int, 56> PC1 = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

constexpr array<int, 48> PC2 = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

constexpr array<int, 16> SHIFTS = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

constexpr int SBOX[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

string readBits(const string &raw) {
    string bits;
    bits.reserve(raw.size());
    for (char ch : raw) {
        if (ch == '0' || ch == '1') {
            bits.push_back(ch);
        }
    }
    return bits;
}

string normalizeBits(string bits, size_t minSize = 0) {
    bits = readBits(bits);
    if (bits.size() < minSize) {
        bits.append(minSize - bits.size(), '0');
    }
    return bits;
}

string zeroPadRight(const string &bits, size_t blockSize) {
    if (bits.size() % blockSize == 0) {
        return bits;
    }
    string padded = bits;
    padded.append(blockSize - (bits.size() % blockSize), '0');
    return padded;
}

string xorBits(const string &a, const string &b) {
    string out;
    out.reserve(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        out.push_back(a[i] == b[i] ? '0' : '1');
    }
    return out;
}

string permute(const string &input, const int *table, size_t n) {
    string out;
    out.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        out.push_back(input[static_cast<size_t>(table[i] - 1)]);
    }
    return out;
}

string leftShift(string bits, int count) {
    count %= static_cast<int>(bits.size());
    return bits.substr(static_cast<size_t>(count)) + bits.substr(0, static_cast<size_t>(count));
}

string sBoxSubstitute(const string &bits48) {
    string out;
    out.reserve(32);
    for (int i = 0; i < 8; ++i) {
        const string chunk = bits48.substr(static_cast<size_t>(i * 6), 6);
        const int row = (chunk[0] - '0') * 2 + (chunk[5] - '0');
        const int col = (chunk[1] - '0') * 8 + (chunk[2] - '0') * 4 + (chunk[3] - '0') * 2 + (chunk[4] - '0');
        const int val = SBOX[i][row][col];
        for (int bit = 3; bit >= 0; --bit) {
            out.push_back(((val >> bit) & 1) ? '1' : '0');
        }
    }
    return out;
}

class DesEngine {
public:
    explicit DesEngine(const string &key64) {
        string key = normalizeBits(key64, 64).substr(0, 64);
        const string permuted = permute(key, PC1.data(), PC1.size());
        string c = permuted.substr(0, 28);
        string d = permuted.substr(28, 28);
        roundKeys.reserve(16);
        for (int i = 0; i < 16; ++i) {
            c = leftShift(c, SHIFTS[i]);
            d = leftShift(d, SHIFTS[i]);
            roundKeys.push_back(permute(c + d, PC2.data(), PC2.size()));
        }
    }

    string encryptBlock(const string &block64) const {
        return processBlock(block64, roundKeys);
    }

    string decryptBlock(const string &block64) const {
        vector<string> reversed(roundKeys.rbegin(), roundKeys.rend());
        return processBlock(block64, reversed);
    }

private:
    vector<string> roundKeys;

    static string processBlock(const string &block64, const vector<string> &keys) {
        string block = normalizeBits(block64, 64).substr(0, 64);
        block = permute(block, IP.data(), IP.size());
        string left = block.substr(0, 32);
        string right = block.substr(32, 32);

        for (const string &key : keys) {
            const string expanded = permute(right, E.data(), E.size());
            const string mixed = xorBits(expanded, key);
            const string substituted = sBoxSubstitute(mixed);
            const string permuted = permute(substituted, P.data(), P.size());
            const string nextRight = xorBits(left, permuted);
            left = right;
            right = nextRight;
        }

        return permute(right + left, FP.data(), FP.size());
    }
};

string processEncrypt(const string &plaintext, const string &key) {
    const DesEngine engine(key);
    const string padded = zeroPadRight(normalizeBits(plaintext), 64);
    string cipher;
    cipher.reserve(padded.size());
    for (size_t i = 0; i < padded.size(); i += 64) {
        cipher += engine.encryptBlock(padded.substr(i, 64));
    }
    return cipher;
}

string processDecrypt(const string &ciphertext, const string &key) {
    const DesEngine engine(key);
    const string input = normalizeBits(ciphertext);
    string plain;
    plain.reserve(input.size());
    for (size_t i = 0; i < input.size(); i += 64) {
        plain += engine.decryptBlock(input.substr(i, 64));
    }
    return plain;
}

string tripleEncrypt(const string &plaintext, const string &k1, const string &k2, const string &k3) {
    const DesEngine e1(k1);
    const DesEngine e2(k2);
    const DesEngine e3(k3);
    const string block = normalizeBits(plaintext, 64).substr(0, 64);
    return e3.encryptBlock(e2.decryptBlock(e1.encryptBlock(block)));
}

string tripleDecrypt(const string &ciphertext, const string &k1, const string &k2, const string &k3) {
    const DesEngine e1(k1);
    const DesEngine e2(k2);
    const DesEngine e3(k3);
    const string block = normalizeBits(ciphertext, 64).substr(0, 64);
    return e1.decryptBlock(e2.encryptBlock(e3.decryptBlock(block)));
}

} // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Chọn mode:\n";
    cout << "1 = DES encrypt\n";
    cout << "2 = DES decrypt\n";
    cout << "3 = TripleDES encrypt\n";
    cout << "4 = TripleDES decrypt\n";
    cout << "Mode: ";

    int mode = 0;
    if (!(cin >> mode)) {
        return 0;
    }

    string input;
    string key1, key2, key3;

    switch (mode) {
        case 1: {
            cout << "Plaintext: ";
            cin >> input;
            cout << "Key: ";
            cin >> key1;
            cout << processEncrypt(input, key1) << '\n';
            break;
        }
        case 2: {
            cout << "Ciphertext: ";
            cin >> input;
            cout << "Key: ";
            cin >> key1;
            cout << processDecrypt(input, key1) << '\n';
            break;
        }
        case 3: {
            cout << "Plaintext: ";
            cin >> input;
            cout << "K1: ";
            cin >> key1;
            cout << "K2: ";
            cin >> key2;
            cout << "K3: ";
            cin >> key3;
            cout << tripleEncrypt(input, key1, key2, key3) << '\n';
            break;
        }
        case 4: {
            cout << "Ciphertext: ";
            cin >> input;
            cout << "K1: ";
            cin >> key1;
            cout << "K2: ";
            cin >> key2;
            cout << "K3: ";
            cin >> key3;
            cout << tripleDecrypt(input, key1, key2, key3) << '\n';
            break;
        }
        default:
            cerr << "Mode không hợp lệ.\n";
            return 1;
    }

    return 0;
}
