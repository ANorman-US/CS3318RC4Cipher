//Alexander Norman
//CS3318 - Professor Yuan
//decrypt
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

string readFile(const string&);//read file as string
string toBinary(const string&);//convert hex string to binary
void swapArray(int*, const int&, const int&);//swap two elements in an array by index
void KSA(int*, const string&);//KSA 
string PRGA(int*, const int&);//PRGA, returns keyStream
void decrypt(const string&, const string&, const string&);//takes XOR and outputs as plaintext

int main()
{
    string inputTextFileName = "rc4cipher.txt";
    string outputTextFileName = "rc4output.txt";
    string cipherText = readFile(inputTextFileName);

    string key;
    cout << "Input Key (5-32 Bytes): ";
    cin >> key;

    int s[256];
    KSA(s, key);
    int keyStreamLength = cipherText.length()/2;//since cipher is in hex binaryStream will be half length
    string keyStream = PRGA(s, keyStreamLength);//holds keystream as binary
    string cipherStream = toBinary(cipherText);//holds input ciphertext as binary
    decrypt(cipherStream, keyStream, outputTextFileName);
}

void decrypt(const string& cipherStream, const string& keyStream, const string& outputFN)
{
    ofstream outFile(outputFN);
    string cipherText = "";
    for(int i=0;i<cipherStream.length();i++)
    {
        if(cipherStream[i] == keyStream[i])
            cipherText+='0';
        else
            cipherText+='1';
    }
    for(int i=0;i<cipherText.length();i+=8)
        outFile << char(bitset<8>(cipherText.substr(i, i+8)).to_ulong());
    outFile.close();
}

string PRGA(int* s, const int &keyStreamLength)
{
    int i = 0;
    int j = 0;
    for(int count=0;count<3072;count++)
    {
        i = (i+1)%256;
        j = (j+s[i])%256;
        swapArray(s, i, j);
    }
    string keyStream = "";
    int k;
    for(int count=0;count<keyStreamLength;count++)
    {
        i = (i+1)%256;
        j = (j+s[i])%256;
        swapArray(s, i, j);
        k = s[(s[i]+s[j])%256];
        keyStream+= bitset<8>(k).to_string();
    }
    return keyStream;
}

void KSA(int* s, const string& key)
{
    for(int i=0;i<256;i++)
        s[i] = i;
    int j = 0;
    for(int i=0;i<256;i++)
    {
        j = (j+s[i]+(int)key[i%key.length()])%256;
        swapArray(s, i, j);
    }
}

void swapArray(int *array, const int &n1, const int &n2)
{
    int temp = array[n1];
    array[n1] = array[n2];
    array[n2] = temp;
}

string toBinary(const string&s)
{
    string bin = "";
    for(int i=0;i<s.length();i++)
    {
        switch(s[i])
        {
            case '0': bin+="0000"; break;
            case '1': bin+="0001"; break;
            case '2': bin+="0010"; break;
            case '3': bin+="0011"; break;
            case '4': bin+="0100"; break;
            case '5': bin+="0101"; break;
            case '6': bin+="0110"; break;
            case '7': bin+="0111"; break;
            case '8': bin+="1000"; break;
            case '9': bin+="1001"; break;
            case 'a': bin+="1010"; break;
            case 'b': bin+="1011"; break;
            case 'c': bin+="1100"; break;
            case 'd': bin+="1101"; break;
            case 'e': bin+="1110"; break;
            case 'f': bin+="1111"; break;
        }
    }
    return bin;
}

string readFile(const string& s)
{
    ifstream inFile(s);
    string scanner;
    string p = "";
    while(getline(inFile, scanner))
        p+=scanner;
    inFile.close();
    return p;
}