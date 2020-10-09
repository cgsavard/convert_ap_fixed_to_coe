/*
 * convert.cpp
 * Author: Claire Savard (claire.savard@colorado.edu)
 *
 * This file converts decimal values to ap_fixed and 
 * prints them in the proper format required here:
 * https://www.xilinx.com/support/documentation/sw_manuals/xilinx11/cgn_r_coe_file_syntax.htm
 *
 * To run:
 *   $ make run
 *   $ ./convert
 *
 * Things to change before running script:
 * 
 * in convert.cpp, input and output file names
 *
 * in convert.h, N (num of rows you want to convert), nbits
 * (num tot bits in ap_fixed), ndec (num of bits assigned
 * to decimal portion), nfeat (num of feats in each row)
 *
 * in Makefile: change path of vivado inputs dir to path 
 * found on your machine
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <bitset>

#include "convert.h"

using namespace std;

// from https://www.geeksforgeeks.org/efficient-method-2s-complement-binary-string/
// Function to find two's complement 
string findTwoscomplement(string str) 
{ 
  int n = str.length(); 
  
  // Traverse the string to get first '1' from 
  // the last of string 
  int i; 
  for (i = n-1 ; i >= 0 ; i--) 
    if (str[i] == '1') 
      break; 
  
  // If there exists no '1' concatenate 1 at the 
  // starting of string 
  if (i == -1) 
    return '1' + str; 
  
  // Continue traversal after the position of 
  // first '1' 
  for (int k = i-1 ; k >= 0; k--) 
    { 
      //Just flip the values 
      if (str[k] == '1') 
	str[k] = '0'; 
      else
	str[k] = '1'; 
    } 
  
  // return the modified string 
  return str;
} 

// adapted from https://www.geeksforgeeks.org/convert-decimal-fraction-binary-number/
string decimalToBinary(float num){
 
  bool sign = 0;
  if (num<0) sign = 1;
  num = abs(num);

  string binary = ""; 
  
  // Fetch the integral part of decimal number 
  int Integral = num; 
  
  // Fetch the fractional part decimal number 
  float fractional = num - Integral; 
  
  string int_bin = bitset<nint-1>(Integral).to_string();

  binary = binary+int_bin;
  
  // Conversion of fractional part to 
  // binary equivalent
  int k_prec = nbits-nint; 
  while (k_prec--) { 
    // Find next bit in fraction 
    fractional *= 2; 
    int fract_bit = fractional; 
  
    if (fract_bit == 1){ 
      fractional -= fract_bit; 
      binary.push_back(1 + '0'); 
    } else binary.push_back(0 + '0'); 
  } 
  
  if (sign){
    binary = findTwoscomplement(binary);
    binary = "1"+binary;
  }else binary = "0"+binary;

  return binary; 
} 

int main() {

  struct tracks* trks[N];
  ifstream input;
  input.open("data/y_true.dat");
  if (!input.is_open()) {
    cout << "file not opened" << endl;
    return 0;
  }
  ofstream output;
  output.open("converted/y.coe");
  output << "memory_initialization_radix=2;\n";
  output << "memory_initialization_vector=";

  input_t feat;
  for (int i=0; i<N; i++){
    string row = "";
    for (int j=0; j<nfeat; j++){
      input >> feat;
      row = row + decimalToBinary(feat);
    }
    output << row;
    if (i != N-1) output << ',' << endl;
  }
  output << ';';

  output.close();
  
  return 0;
}
