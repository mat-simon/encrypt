#include <iostream>
#include <vector>
#include <algorithm>
#include "constants.h"

using namespace std;

void Encrypt(vector<vector<unsigned char>> &state, unsigned char roundKey[11][4][4]);
void SubBytes(vector<vector<unsigned char>> &state);
void ShiftRows(vector<vector<unsigned char>> &state);
void MixColumns(vector<vector<unsigned char>> &state);
void AddRoundKey(vector<vector<unsigned char>> &state, unsigned char roundKey[4][4]);
void Decrypt(vector<vector<unsigned char>> &state, unsigned char roundKey[11][4][4]);
void InvShiftRows(vector<vector<unsigned char>> &state);
void InvSubBytes(vector<vector<unsigned char>> &state);
void InvMixColumns(vector<vector<unsigned char>> &state);
void KeyExpand(unsigned char key[16], unsigned char w[11][4][4]);
void RotWord(vector<unsigned char> &temp);
void SubWord(vector<unsigned char> &temp);
void printSquareMatrix(vector<vector<unsigned char>> matrix);


int main(){
    // unsigned char key[16] = {0x0F, 0x15, 0x71, 0xC9, 0x47, 0xD9, 0xE8, 0x59,     // book's key example
    //                          0x0C, 0xB7, 0xAD, 0xD6, 0xAF, 0x7F, 0x67, 0x98};
    unsigned char key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                             0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    const unsigned char rows = 4;
    const unsigned char cols = 4;
    unsigned char roundKey[11][4][4];
    vector<vector<unsigned char>> state(rows, vector<unsigned char>(cols));

    // define 16 char string, to be used for state
    const string message = "CSE178 project1!";

    // populate 2d vector, state, with message data
    for(int i = 0; i < message.length(); i++){
        for(int x = 0; x < cols; x++){
            for(int y = 0; y < rows; y++){
                state[x][y] = message[y*rows + x];              //populate by column
            }
        }
    }

    // // for defining state manually
    // state = {
    //     {0x01, 0x23, 0x45, 0x67},
    //     {0x89, 0xab, 0xcd, 0xef},
    //     {0xfe, 0xdc, 0xba, 0x98},
    //     {0x76, 0x54, 0x32, 0x10}
    // };

    cout << "expanding key into Round Key..." << endl;
    KeyExpand(key, roundKey);

    // cout << "printing Round Key:" << endl;
    // for(int b=0; b<11; b++){                                             //print round key
    //     for(int i=0; i<4; i++){
    //         for(int j=0; j<4; j++){
    //             cout << (hex) << (unsigned)roundKey[b][i][j] << ' ' << ' ';
    //         }
    //     cout << endl;
    //     }
    //     cout << endl;
    // }

    cout << "initial state:" << endl;
    printSquareMatrix(state);

    Encrypt(state, roundKey);
    cout << "encrypted state:" << endl;
    printSquareMatrix(state);

    Decrypt(state, roundKey);
    cout << "decrypted state:" << endl;
    printSquareMatrix(state);
}


void Encrypt(vector<vector<unsigned char>> &state, unsigned char roundKey[11][4][4]){
    // cout << "Start of Round:" << endl;
    // printSquareMatrix(state);
    AddRoundKey(state, roundKey[0]);
    for(int i = 0; i < 9; i++){                                 //9 full rounds for 128 bit AES
        // cout << "Start of Round:" << endl;
        // printSquareMatrix(state);
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, roundKey[i+1]);
    }
    // cout << "Start of Round:" << endl;
    // printSquareMatrix(state);
    SubBytes(state);                                            //+1 round without MixColumns
    ShiftRows(state);
    AddRoundKey(state, roundKey[10]);
}

void SubBytes(vector<vector<unsigned char>> &state){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = SBox[state[i][j]];
        }
    }
    // cout << "After SubBytes:" << endl;
    // printSquareMatrix(state);
}

void ShiftRows(vector<vector<unsigned char>> &state){
    for(int i=1; i<4; i++){
        rotate(state[i].begin(), state[i].begin()+i, state[i].end());
    }
    // cout << "After ShiftRows:" << endl;
    // printSquareMatrix(state);
}

void MixColumns(vector<vector<unsigned char>> &state){
    unsigned char temp[4][4] = {
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
    };

    for(int j=0; j<4; j++){
        temp[0][j] = mul2[state[0][j]] ^ mul3[state[1][j]] ^ state[2][j] ^ state[3][j];
        temp[1][j] = state[0][j] ^ mul2[state[1][j]] ^ mul3[state[2][j]] ^ state[3][j];
        temp[2][j] = state[0][j] ^ state[1][j] ^ mul2[state[2][j]] ^ mul3[state[3][j]];
        temp[3][j] = mul3[state[0][j]] ^ state[1][j] ^ state[2][j] ^ mul2[state[3][j]];
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            state[i][j] = temp[i][j];
        }
    }
    // cout << "After MixColumns:" << endl;
    // printSquareMatrix(state);
}

void AddRoundKey(vector<vector<unsigned char>> &state, unsigned char roundKey[4][4]){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            state[i][j] = state[i][j] ^ roundKey[i][j];
        }
    }
}

void Decrypt(vector<vector<unsigned char>> &state, unsigned char roundKey[11][4][4]){
    AddRoundKey(state, roundKey[10]);
    for(int i = 0; i < 9; i++){
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, roundKey[10-i-1]);
        InvMixColumns(state);
    }
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, roundKey[0]);
}

void InvShiftRows(vector<vector<unsigned char>> &state){
    for(int i=1; i<4; i++){
        rotate(state[i].begin(), state[i].begin()+state.size()-i, state[i].end());
    }
}

void InvSubBytes(vector<vector<unsigned char>> &state){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = InvSBox[state[i][j]];
        }
    }
}

void InvMixColumns(vector<vector<unsigned char>> &state){
    unsigned char temp[4][4] = {
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
    };

    for(int j=0; j<4; j++){
        temp[0][j] = mul14[state[0][j]] ^ mul11[state[1][j]] ^ mul13[state[2][j]] ^ mul9[state[3][j]];
        temp[1][j] = mul9[state[0][j]] ^ mul14[state[1][j]] ^ mul11[state[2][j]] ^ mul13[state[3][j]];
        temp[2][j] = mul13[state[0][j]] ^ mul9[state[1][j]] ^ mul14[state[2][j]] ^ mul11[state[3][j]];
        temp[3][j] = mul11[state[0][j]] ^ mul13[state[1][j]] ^ mul9[state[2][j]] ^ mul14[state[3][j]];
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            state[i][j] = temp[i][j];
        }
    }
}

void KeyExpand(unsigned char key[16], unsigned char w[11][4][4]){
    vector<unsigned char> temp = {0, 0, 0, 0};

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            w[0][i][j] = key[4*j+i];
        }
    }

    for(int b = 1; b < 11; b++){
        for(int i = 0; i < 4; i++){
            if(i == 0){
                for(int j = 0; j < 4; j++){
                    temp[j] = w[b-1][j][3];
                }
            }
            else{
                for(int j = 0; j < 4; j++){
                    temp[j] = w[b][j][i-1];
                }
            }
            if(i == 0){
                RotWord(temp);
                SubWord(temp);
                temp[0] = temp[0] ^ RCon[b-1];
            }
            for(int j = 0; j < 4; j++){
                w[b][j][i] = w[b-1][j][i] ^ temp[j];
            }
        }
    }
}

void RotWord(vector<unsigned char> &temp){
    rotate(temp.begin(), temp.begin() + 1, temp.end());
}

void SubWord(vector<unsigned char> &temp){
    for(int i = 0; i < 4; i++){
        temp[i] = SBox[temp[i]];
    }
}

void printSquareMatrix(vector<vector<unsigned char>> matrix){
    int dim = matrix.size();
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            cout << hex << (unsigned int)(matrix[i][j]) << ' ' << ' ';
            //cout << matrix[i][j];                                     // cout chars instead of ints
        }
        cout << endl;
    }
}