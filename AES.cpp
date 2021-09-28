#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

void SubBytes(vector<vector<unsigned char>> &state);
void ShiftRows(vector<vector<unsigned char>> &state);
void MixColumns(vector<vector<unsigned char>> &state);
void AddRoundKey(vector<vector<unsigned char>> &state);
void printSquareMatrix(vector<vector<unsigned char>> state);

const unsigned char SBox[16][16] = {
        {99, 124, 119, 123, 242, 107, 111, 197, 48, 1, 103, 43, 254, 215, 171, 118},
        {202, 130, 201, 125, 250, 89, 71, 240, 173, 212, 162, 175, 156, 164, 114, 192},
        {183, 253, 147, 38, 54, 63, 247, 204, 52, 165, 229, 241, 113, 216, 49, 21},
        {4, 199, 35, 195, 24, 150, 5, 154, 7, 18, 128, 226, 235, 39, 178, 117},
        {9, 131, 44, 26, 27, 110, 90, 160, 82, 59, 214, 179, 41, 227, 47, 132},
        {83, 209, 0, 237, 32, 252, 177, 91, 106, 203, 190, 57, 74, 76, 88, 207},
        {208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2, 127, 80, 60, 159, 168},
        {81, 163, 64, 143, 146, 157, 56, 245, 188, 182, 218, 33, 16, 255, 243, 210},
        {205, 12, 19, 236, 95, 151, 68, 23, 196, 167, 126, 61, 100, 93, 25, 115},
        {96, 129, 79, 220, 34, 42, 144, 136, 70, 238, 184, 20, 222, 94, 11, 219},
        {224, 50, 58, 10, 73, 6, 36, 92, 194, 211, 172, 98, 145, 149, 228, 121},
        {231, 200, 55, 109, 141, 213, 78, 169, 108, 86, 244, 234, 101, 122, 174, 8},
        {186, 120, 37, 46, 28, 166, 180, 198, 232, 221, 116, 31, 75, 189, 139, 138},
        {112, 62, 181, 102, 72, 3, 246, 14, 97, 53, 87, 185, 134, 193, 29, 158},
        {225, 248, 152, 17, 105, 217, 142, 148, 155, 30, 135, 233, 206, 85, 40, 223},
        {140, 161, 137, 13, 191, 230, 66, 104, 65, 153, 45, 15, 176, 84, 187, 22},
    };


int main(){

    const string message = "This will be the";
    const int bits = 128;
    const int cols = 4;
    const int rows = (bits/8)/4;
    vector<vector<unsigned char>> state(rows, vector<unsigned char>(cols));


    //populate 2d vector, state, with message data
    for(int i=0; i<message.length(); i++){
        for(int x=0; x<cols; x++){
            for(int y=0; y<rows; y++){
                state[x][y] = message[y*rows + x];              //populate by column
            }
        }
    }

    // state[0][0] = 135;
    // state[0][1] = 242;
    // state[0][2] = 77;
    // state[0][3] = 151;
    // state[1][0] = 236;
    // state[1][1] = 110;
    // state[1][2] = 76;
    // state[1][3] = 144;
    // state[2][0] = 74;
    // state[2][1] = 195;
    // state[2][2] = 70;
    // state[2][3] = 231;
    // state[3][0] = 140;
    // state[3][1] = 216;
    // state[3][2] = 149;
    // state[3][3] = 166;


    // for(int i=0; i<9; i++){                                 //9 rounds for 128 bit AES
    //     SubBytes(state);
    //     ShiftRows(state);
    //     MixColumns(state);
    //     AddRoundKey(state);
    // }
    // SubBytes(state);                                       //+1 round without MixColumns
    // ShiftRows(state);
    // AddRoundKey(state);

}

void SubBytes(vector<vector<unsigned char>> &state){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            int row = (state[i][j] >> (4 * 1)) & 0x000F;       //1st hex digit
            int col = (state[i][j] >> (4 * 0)) & 0x000F;       //2nd hex digit
            state[i][j] = SBox[row][col];
        }
    }
}

void ShiftRows(vector<vector<unsigned char>> &state){
    for(int i=1; i<4; i++){
        rotate(state[i].begin(), state[i].begin()+i, state[i].end());
    }
}

void MixColumns(vector<vector<unsigned char>> &state){

}

void AddRoundKey(vector<vector<unsigned char>> &state){

}

void printSquareMatrix(vector<vector<unsigned char>> state){

    int dim = state.size();

    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            cout << hex << static_cast<unsigned>(state[i][j]) << ' ';
        }
        cout << endl;
    }
}