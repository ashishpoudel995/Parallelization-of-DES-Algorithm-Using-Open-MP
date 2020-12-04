#include <bits/stdc++.h>
#include <string.h>
#define numberConversions
std::string str2bin(std::string s) 
{ 
	std::string binary;
	for(int i=0;i<s.length();i++){
		binary+=std::bitset<8>(s[i]).to_string();
	}
	return binary; 
} 
std::string bin2str(std::string str) 
{
	std::string s;
	for(int i=0;i<str.length();i=i+8){ 
		std::string bin=str.substr(i,8);
		char c = static_cast<char>(std::bitset<8>(bin.substr(0,8)).to_ulong());;
		s+=c;
	}
	return s; 
}
std::string decimalToBinary(int val){
	std::string res;
	//When ASCII encoding is used, the integer value of '0' is 48. So, adding '0' i.e. 48 to get the integer value of 1 as '1'.
	//i.e. Converting digit to its corresponding character.
	res += char(val/ 8 + '0'); 
	val= val% 8; 
	res += char(val/ 4 + '0'); 
	val= val% 4; 
	res += char(val/ 2 + '0'); 
	val= val% 2; 
	res += char(val+ '0');  
return res;
}
std::string convertToBinary(std::string s) 
{ 
	std::string res="";
	for(int i=0;i<s.length();i++){
		switch(s[i]){
			case '0': 
				res+="0000"; 
            break; 
			case '1': 
				res+="0001"; 
				break; 
			case '2': 
				res+="0010"; 
				break; 
			case '3': 
				res+="0011"; 
				break; 
			case '4': 
				res+="0100"; 
				break; 
			case '5': 
				res+="0101"; 
				break; 
			case '6': 
				res+="0110"; 
				break; 
			case '7': 
				res+="0111"; 
				break; 
			case '8': 
				res+="1000"; 
				break; 
			case '9': 
				res+="1001"; 
				break; 
			case 'A': 
			case 'a': 
				res+="1010"; 
				break; 
			case 'B': 
			case 'b': 
				res+="1011"; 
				break; 
			case 'C': 
			case 'c': 
				res+="1100"; 
				break; 
			case 'D': 
			case 'd': 
				res+="1101"; 
				break; 
			case 'E': 
			case 'e': 
				res+="1110"; 
				break; 
			case 'F': 
			case 'f': 
				res+="1111"; 
				break; 
			}
	}
return res;
}

