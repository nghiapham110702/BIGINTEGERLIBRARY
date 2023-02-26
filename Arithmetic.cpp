#include<iostream>
#include<fstream>
#include<string>
#include"List.h"
#include "BigInteger.h"
using namespace std;

#define MAX_LEN 300

int main(int argc, char * argv[]){

    int token_count;
    int line_count = 0;
    size_t begin, end, len;
    ifstream in;
    ofstream out;
    string line;
    string tokenBuffer;
    string token;
    BigInteger A;
    BigInteger B;

    // check command line for correct number of arguments
    if( argc != 3 ){
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    // open files for reading and writing 
    in.open(argv[1]);
    if( !in.is_open() ){
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if( !out.is_open() ){
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }
    while( getline(in, line) ){
        line_count++;

        len = line.length();

        token_count = 0;
        tokenBuffer = "";
        
        // get first token
        begin = min(line.find_first_not_of(" ", 0), len);
        end = min(line.find_first_of(" ", begin), len);
        token = line.substr(begin, end-begin);

        while( token!="" ){  // we have a token
            // update token buffer
            tokenBuffer += token;
            token_count++;

            // get next token
            begin = min(line.find_first_not_of(" ", end+1), len);
            end = min(line.find_first_of(" ", begin), len);
            token = line.substr(begin, end-begin);
        }
        // print tokens in this line
        if (line_count == 1){
            A = BigInteger(tokenBuffer);
        }
        if (line_count == 3){
            B = BigInteger(tokenBuffer);
        }
    }
    // close files
    BigInteger two = BigInteger("2");
    BigInteger three = BigInteger("3");
    BigInteger nine = BigInteger("9");
    BigInteger sixteen = BigInteger("16");

    BigInteger C = A+B;
    BigInteger D = A-B;
    BigInteger E = A-A;
    BigInteger F = three*A - two*B; 
    BigInteger G = A*B;
    BigInteger H = A*A;
    BigInteger I = B*B;
    BigInteger J = nine*H*H + sixteen*I*I*B; 

    out << A << "\n" << endl;
    
    out << B << "\n" << endl; 

    out << C << "\n" << endl;

    out << D << "\n" << endl;

    out << E << "\n" << endl;

    out << F << "\n" << endl;

    out << G << "\n" << endl;

    out << H << "\n" << endl;

    out << I << "\n" << endl;

    out << J << "\n" << endl;
    in.close();
    out.close();

    return(EXIT_SUCCESS);

   
}