/*
 * convert.cpp
 * Author: Claire Savard (claire.savard@colorado.edu)
 *
 * This file converts decimal values to ap_fixed and 
 * prints them in the proper format required here:
 * https://www.xilinx.com/support/documentation/sw_manuals/xilinx11/cgn_r_coe_file_syntax.htm
 *
 * To run:
 *   $ make convert
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

#include "convert.h"

using namespace std;

string get_str_num(input_t num){

  string temp_str_num = num.to_string();

  string sign = "";
  //start with the sign  
  if (temp_str_num.at(0) == '-'){
    sign = "1";
  }else{
    sign = "0";
  }

  //grab integer and decimal parts then zero-pad
  int b = temp_str_num.find('b');
  int per = temp_str_num.find('.');
  string dec_part = "";
  if (per == -1){
    per = temp_str_num.length();
  }else{ 
    dec_part = temp_str_num.substr(per+1);
  }  
  string int_part = string(&temp_str_num[b+2], &temp_str_num[per]);

  dec_part = dec_part.append(ndec-dec_part.length(),'0');
  int_part = int_part.insert(0, nbits-ndec-1-int_part.length(),'0');

  //put full string together
  string str_num = sign+int_part+dec_part;

  return str_num;
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
  output << "memory_initialization_radix=2\n";
  output << "memory_initialization_vector=";

  input_t feat;
  for (int i=0; i<N; i++){
    string row = "";
    for (int j=0; j<nfeat; j++){
      input >> feat;
      row = row + get_str_num(feat);
    }
    output << row;
    if (i != N-1) output << ',' << endl;
  }
  output << ';';

  output.close();
  
  return 0;
}
