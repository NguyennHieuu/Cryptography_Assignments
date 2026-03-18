#include <iostream>
#include <vector>
#include <string>
using namespace std;

class RC4 {
private:
    vector<int> S;
    int N;
public:
    RC4(int size = 256) : N(size) { S.resize(N); }

    // KSA: hoán vị S dựa trên key
    void ksa(const vector<int>& key) {
        for (int i = 0; i < N; i++) S[i] = i;
        int j = 0, L = key.size();
        for (int i = 0; i < N; i++) {
            j = (j + S[i] + key[i % L]) % N;
            swap(S[i], S[j]);
        }
    }

    void printS(const string& label) {
        cout << label << ": [";
        for (int k = 0; k < N; k++) cout << S[k] << (k < N-1 ? ", " : "");
        cout << "]" << endl;
    }

    // PRGA: sinh keystream rồi XOR với plaintext
    vector<int> encrypt(const string& plaintext) {
        vector<int> tempS = S, ciphertext;
        int i = 0, j = 0;
        for (char c : plaintext) {
            i = (i + 1) % N;
            j = (j + tempS[i]) % N;
            swap(tempS[i], tempS[j]);
            int ks = tempS[(tempS[i] + tempS[j]) % N];
            ciphertext.push_back((int)c ^ ks);
        }
        return ciphertext;
    }

    // Decrypt: XOR ciphertext với cùng keystream -> ra plaintext
    string decrypt(const vector<int>& ciphertext) {
        vector<int> tempS = S;
        string plaintext;
        int i = 0, j = 0;
        for (int c : ciphertext) {
            i = (i + 1) % N;
            j = (j + tempS[i]) % N;
            swap(tempS[i], tempS[j]);
            int ks = tempS[(tempS[i] + tempS[j]) % N];
            plaintext += (char)(c ^ ks);
        }
        return plaintext;
    }
};

int main() {
    cout << "--- RC4 Stream Cipher ---" << endl;

    int N = 10;
    vector<int> key = {2, 4, 1, 7};
    string message  = "cybersecurity";

    cout << "Plaintext : " << message << endl;
    cout << "Key K     : [2, 4, 1, 7]" << endl;
    cout << "N         : " << N << endl;

    RC4 cipher(N);

    cout << "\n--- KSA ---" << endl;
    cout << "S ban dau : [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]" << endl;
    cipher.ksa(key);
    cipher.printS("S sau KSA");

    vector<int> ciphertext = cipher.encrypt(message);

    cout << "\n--- Ket qua ---" << endl;
    cout << "Plaintext : " << message << endl;
    cout << "Ciphertext: ";
    for (int v : ciphertext) cout << (char)v;
    cout << endl;

    // Decrypt: tạo object mới, reset S qua KSA với cùng key
    RC4 cipher2(N);
    cipher2.ksa(key);
    string decrypted = cipher2.decrypt(ciphertext);
    cout << "Decrypted : " << decrypted << endl;

    return 0;
}