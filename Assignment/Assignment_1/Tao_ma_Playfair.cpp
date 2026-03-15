#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class PlayfairCipher {
private:
    char matrix[5][5];

    string prepareText(string text, bool isMessage = true) {
        string res = "";
        for (char c : text) {
            if (isalpha(c)) {
                c = toupper(c);
                if (c == 'J') c = 'I';
                res += c;
            }
        }

        if (isMessage) {
            string processed = "";
            int i = 0;
            while (i < (int)res.length()) {
                processed += res[i];
                if (i + 1 < (int)res.length()) {
                    if (res[i] == res[i + 1]) {
                        processed += 'X'; // Chèn X nếu 2 chữ trùng nhau, chưa tiêu thụ res[i+1]
                    } else {
                        processed += res[i + 1];
                        i += 2;
                        continue;
                    }
                }
                i++;
            }
            if (processed.length() % 2 != 0) processed += 'X'; // Thêm X nếu lẻ ký tự
            return processed;
        }
        return res;
    }

    void findPosition(char c, int &row, int &col) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (matrix[i][j] == c) {
                    row = i; col = j;
                    return;
                }
            }
        }
    }

public:
    PlayfairCipher(string key) {
        string processedKey = prepareText(key, false);
        string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // Không có J
        string combined = processedKey + alphabet;
        string finalKey = "";

        for (char c : combined) {
            if (finalKey.find(c) == string::npos) finalKey += c;
        }

        int idx = 0;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                matrix[i][j] = finalKey[idx++];
    }

    void displayMatrix() {
        cout << "Ma tran Playfair 5x5:" << endl;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) cout << matrix[i][j] << " ";
            cout << endl;
        }
    }

    string encrypt(string text) {
        string pText = prepareText(text);
        string cipher = "";

        for (int i = 0; i < (int)pText.length(); i += 2) {
            int r1, c1, r2, c2;
            findPosition(pText[i], r1, c1);
            findPosition(pText[i + 1], r2, c2);

            if (r1 == r2) { // Cùng hàng
                cipher += matrix[r1][(c1 + 1) % 5];
                cipher += matrix[r2][(c2 + 1) % 5];
            } else if (c1 == c2) { // Cùng cột
                cipher += matrix[(r1 + 1) % 5][c1];
                cipher += matrix[(r2 + 1) % 5][c2];
            } else { // Hình chữ nhật
                cipher += matrix[r1][c2];
                cipher += matrix[r2][c1];
            }
        }
        return cipher;
    }
};

int main() {
    string key = "MONARCHY";
    string plaintext = "Do you like to study a cryptography course";

    PlayfairCipher cipher(key);

    cout << "Keyword goc: " << key << endl;
    cipher.displayMatrix();

    cout << "\nCau goc: " << plaintext << endl;
    string encrypted = cipher.encrypt(plaintext);
    cout << "Ban ma (Ciphertext): " << encrypted << endl;

    return 0;
}