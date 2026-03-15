#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class PlayfairDecoder {
private:
    char matrix[5][5];

    string formatString(string s) {
        string res = "";
        for (char c : s) {
            if (isalpha(c)) {
                c = toupper(c);
                if (c == 'J') c = 'I';
                res += c;
            }
        }
        return res;
    }

    void findPosition(char c, int &r, int &col) {
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (matrix[i][j] == c) { r = i; col = j; return; }
    }

public:
    void createMatrix(string key) {
        string processedKey = formatString(key);
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

        cout << "\n--- Ma tran Playfair 5x5 ---" << endl;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) cout << matrix[i][j] << " ";
            cout << endl;
        }
        cout << "----------------------------" << endl;
    }

    string decrypt(string cipher) {
        string text = formatString(cipher);
        string plain = "";

        for (int i = 0; i < (int)text.length(); i += 2) {
            int r1, c1, r2, c2;
            findPosition(text[i], r1, c1);
            findPosition(text[i + 1], r2, c2);

            if (r1 == r2) { // Cùng hàng -> Dịch sang trái
                plain += matrix[r1][(c1 + 4) % 5];
                plain += matrix[r2][(c2 + 4) % 5];
            } else if (c1 == c2) { // Cùng cột -> Dịch lên trên
                plain += matrix[(r1 + 4) % 5][c1];
                plain += matrix[(r2 + 4) % 5][c2];
            } else { // Hình chữ nhật -> Đổi cột
                plain += matrix[r1][c2];
                plain += matrix[r2][c1];
            }
        }
        return plain;
    }
};

int main() {
    PlayfairDecoder pd;
    string key, ciphertext;

    cout << "=== CHUONG TRINH GIAI MA PLAYFAIR ===" << endl;
    cout << "Nhap Keyword: ";
    getline(cin, key);

    pd.createMatrix(key);

    cout << "Nhap Ban Ma (Ciphertext): ";
    getline(cin, ciphertext);

    if (ciphertext.length() % 2 != 0) {
        cout << "Loi: Ban ma phai co so ky tu chan!" << endl;
    } else {
        string result = pd.decrypt(ciphertext);
        cout << "\n=> Ket qua giai ma: " << result << endl;
        cout << "\n(Luu y: Ban hay tu ngat dau cach va xoa chu 'X' thua neu co)" << endl;
    }

    return 0;
}