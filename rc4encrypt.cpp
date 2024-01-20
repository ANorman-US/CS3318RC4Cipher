//Alexander Norman
//CS3318 - Professor Yuan
//encrypt
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

string readFile(const string&);//read file as string
string toBinary(const string&);//convert chars in string to binary
void swapArray(int*, const int&, const int&);//swap two elements in an array by index
void KSA(int*, const string&);//KSA 
string PRGA(int*, const int&);//PRGA, returns keyStream
void encrypt(const string&, const string&, const string&);//takes XOR and outputs in hex

int main()
{
    string inputTextFileName = "rc4input.txt";
    string outputTextFileName = "rc4cipher.txt";
    string inputText = readFile(inputTextFileName);

    string key;
    cout << "Input Key (5-32 Bytes): ";
    cin >> key;

    int s[256];
    KSA(s, key);
    int keyStreamLength = inputText.length();
    string keyStream = PRGA(s, keyStreamLength);//holds keystream as binary
    string inputStream = toBinary(inputText);//holds input text as binary

    encrypt(inputStream, keyStream, outputTextFileName);

    return 0;
}

void encrypt(const string& inputStream, const string& keyStream, const string& outputFN)
{
    ofstream outFile(outputFN);
    string cipherText = "";
    for(int i=0;i<inputStream.length();i++)
    {
        if(inputStream[i] == keyStream[i])
            cipherText+='0';
        else
            cipherText+='1';
    }
    for(int i=0;i<cipherText.length();i+=4)
        outFile << hex << bitset<4>(cipherText.substr(i, i+4)).to_ulong() << dec;
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
        bin+= bitset<8>(s[i]).to_string();
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