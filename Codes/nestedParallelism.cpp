#include <bits/stdc++.h> 
#include <omp.h>
#include <fstream>
#include "matrices.h"
#include "numberConversions.h"
using namespace std;
string permutation(string str, int* arr, int n) 
{ 
	string res = ""; 
	for (int i = 0; i < n; i++) { 
		res += str[arr[i] - 1]; 
	} 
	return res; 
} 

string shift_left(string k, int shifts) 
{ 
	string s = ""; 
	for (int i = 0; i < shifts; i++) { 
		for (int j = 1; j < 28; j++) { 
			s += k[j]; 
		} 
		s += k[0]; 
		k = s; 
		s = ""; 
	} 
	return k; 
} 

string xorOperation(string str1, string str2) 
{ 
	string xored = ""; 
	for (int i = 0; i < str1.size(); i++) { 
		if (str1[i] == str2[i])
			xored += "0"; 
		else
			xored += "1";
	} 
	return xored; 
} 
string encryption(string pt,vector<string> roundKeys) 
{ 
	// String to binary 
	pt = str2bin(pt); 
	// Initial Permutation Process
	pt = permutation(pt, initialPermutation, 64); 
	// Splitting of 64bits plain text to LPT and RPT of 32 bits each
	string left = pt.substr(0, 32); 
	string right = pt.substr(32, 32);  
	for (int i = 0; i < 16; i++) { 
		// Exapansion Permutation 
		string expandedRPT = permutation(right, dBox, 48); 
		// XOR of RoundKey and expandedRPT
		string x = xorOperation(roundKeys[i],expandedRPT); 
		// S-boxes
		//result string array for storing the 4 bits outputs for 8*6 bits input
		string result[8]; 
		//res stores the final result from s box. i.e. concat all the result array elements
		string res = "";
		int row[8],col[8],val[8];
		//nested level parallelism
		#pragma omp parallel for num_threads(8) schedule(static)
			for (int i = 0; i < 8; i++) {
				//the value of '0' is 48, '1' is 49 and so on. but since we are referring the matrix index, we are interested in 0,1,..
				//So, the '0' should be subtracted . i.e. the 49 value of '1' will be 49-48=1.
					int tid=omp_get_thread_num();
					row[tid] = 2 * int(x[tid * 6] - '0') + int(x[tid * 6 + 5] - '0'); 
					col[tid] = 8 * int(x[tid * 6 + 1] - '0') + 4 * int(x[tid * 6 + 2] - '0') + 2 * int(x[tid * 6 + 3] - '0') + int(x[tid * 6 + 4] - '0');
					val[tid] = sbox[tid][row[tid]][col[tid]];
					result[tid]= decimalToBinary(val[tid]);
			}
		for(int i=0;i<8;i++) res+=result[i];
		// P-Box Permutation 
		res = permutation(res, pbox, 32); 
		// XOR of left and res 
		x = xorOperation(res, left); 
		left = x; 
		// Swap left and right in every rounds except the last round
		if (i != 15) { 
			swap(left, right); 
		} 
	} 
	// Left and Right combined
	string combined = left + right; 
	// Final Permutation to obtain 64bits cipher text
	string cipher = bin2str(permutation(combined, finalPermutation, 64)); 
	return cipher; 
} 
int main(){
	double t1=omp_get_wtime();
	//Reading from the file and storing the plaintext in pt vector
	vector<string> pt;
	string c,temp;
	ifstream MyReadFile("plaintext.txt");
	while(getline(MyReadFile,c)){
			temp+=c;
	}
	while(temp.length()%8!=0){
		temp+=" ";
	}
	for(int i=0;i<temp.length();i=i+8){
		pt.push_back(temp.substr(i,8));
	}
	MyReadFile.close();
	string key = "ABC12532110EDA56"; 
	key = convertToBinary(key);
	key = permutation(key, keyTransformation, 56); // key without parity 
	//Splitting 56 bit keys to left and right of 28 bits each
	string left = key.substr(0, 28); 
	string right = key.substr(28, 28); 
	vector<string> roundKeys; // Declaring vector for storing keys of 16 rounds
	for (int i = 0; i < 16; i++) { 
		//Left Shift and Right Shift done to the respective left and right keys in each round
		left = shift_left(left, shiftsMatrix[i]); 
		right = shift_left(right, shiftsMatrix[i]); 
		string combinedkey = left + right; 
		// Key Compression : Converting 56 bit key to 48 bit combined key
		string RoundKey = permutation(combinedkey, keyCompresssion, 48); 
		roundKeys.push_back(RoundKey); 
	}
	
	//Encryption
	vector<string> ciphertext;
	string cipher[pt.size()];
	cout << "Encrypting..."<<endl;
	omp_set_nested(1); //enable nested parallelism
	#pragma omp parallel for schedule(guided) num_threads(8)
	for(int i=0;i<pt.size();i++){
		cipher[i] = encryption(pt[i],roundKeys);
	}
	for(int i=0;i<pt.size();i++){
		ciphertext.push_back(cipher[i]);
	}
	//Writing Cipher Text to File
	ofstream writeObj;
	remove("encrypted.txt");
	writeObj.open("encrypted.txt",ios::app);
	int j=0;
	for(j=0;j<ciphertext.size()-1;j++){	
		writeObj << ciphertext[j];
	}
	writeObj<<ciphertext[j+1];
	writeObj.close(); 
	cout << "Encryption Process Completed."<<endl;
	
	//Decryption : Reversing the round keys and executing the encryption process to get Plain Text
	reverse(roundKeys.begin(),roundKeys.end()); 
	string decrypted;
	string text[ciphertext.size()];
	cout << "Decrypting..."<<endl;
	omp_set_nested(1);
	#pragma omp parallel for schedule(guided) num_threads(8)
	for(int i=0;i<ciphertext.size();i++){
		text[i] = encryption(ciphertext[i],roundKeys);
	}
	for(int i=0;i<ciphertext.size();i++){
		decrypted+=text[i];
	}
	double t2=omp_get_wtime();
	cout << "Decryption Process Completed"<<endl;
	cout << "\nDecrypted Plain Text:\n " << decrypted << endl; 
	cout <<endl<< "Time taken:"<<(t2-t1)<<"s"<<endl;
}

