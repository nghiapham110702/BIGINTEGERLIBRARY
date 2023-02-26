#include<iostream>
#include<string>
#include"List.h"
#include <cstdlib>
#include <cstddef>  
#include<math.h>
#include "BigInteger.h"
using namespace std;
// negateList()
// Changes the sign of each integer in List L. Used by sub().
void negateList(List& L);
// scalarMultList()
// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List& L, ListElement m);
// sumList()
// Overwrites the state of S with A + sgn*B (considered as vectors).
// Used by both sum() and sub().
void sumList(List& S, List A, List B, int sgn);
// normalizeList()
// Performs carries from right to left (least to most significant
// digits), then returns the sign of the resulting integer. Used
// by add(), sub() and mult().
int normalizeList(List& L);
// shiftList()
// Prepends p zero digits to L, multiplying L by base^p. Used by mult().
void shiftList(List& L, int p);


const int power = 9;
const long base = 1000000000;
// BigInteger()
// Constructor that creates a new BigInteger in the zero state: 
// signum=0, digits=().
BigInteger::BigInteger(){
    signum = 0;
    digits = List();
}
// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// Pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s){
    std::size_t found = s.find_first_not_of("+-0123456789");
    if( found !=  string::npos ){
        throw std::invalid_argument("Error:Input is not valid!");
    }
    if (s.length() == 0 || (s.length() == 1 && (s[0] == '+' || s[0] == '-'))) {
        throw std::invalid_argument("Error: string is empty or contains only a sign!");
    }
    //Determine the starting index to parse the string based on the sign of the number:
    int start = 0;
    if (s[0] == '-'){
        signum = -1;
        start = 1;
    }
    else if (s[0] == '+'){
        signum = 1;
        start = 1;
    }
    else{
        signum = 1;
    }
    //Determine the number of "blocks" of digits that the input string should be divided into, based on the number of digits and the power (which is set to 4). If the number of digits is evenly divisible by the power, the number of blocks is simply num_digits / power. 
    //Otherwise, we need to add an extra block to capture the leftover digits: num_blocks = (num_digits + power - 1) / power;.
    int num_digits = s.length() - start;
    int num_blocks = (num_digits + power - 1) / power; // round up division
    int num_zeros = num_blocks * power - num_digits;
    //Add any necessary leading zeros to the digits list, so that we have the correct number of blocks:
    for (int i = 0; i < num_zeros; i++){
        digits.insertAfter(0);
    }
    //Starting from the end of the string, parse each block of digits into an integer and add it to the digits list:
    for (int i = s.length() - 1; i >= start; i -= power){
        int block = 0;
        //The inner loop parses a block of power digits into an integer by multiplying each digit by an appropriate power of 10, and then adding up the results. 
        //For example, if power is 4 and we're parsing the block "1234", then the resulting integer will be 1 * 1000 + 2 * 100 + 3 * 10 + 4 = 1234.
        for (int j = std::max(start, i - power + 1); j <= i; j++){
            int digit = s[j] - '0';
            block = 10 * block + digit;
        }
        digits.insertAfter(block);
    }
    //Remove any leading zeros that were added to the digits list
    digits.moveFront();
    while (digits.peekNext() == 0){
        digits.eraseAfter();
    }
    //Remove any trailing zeros from the digits list
    digits.moveBack();
    while (digits.peekPrev() == 0){
        digits.eraseBefore();
    }
}
// BigInteger()
// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger& N){
    this->signum = N.signum;
    this->digits = N.digits;
}
// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is negative, 
// positive or 0, respectively.
int BigInteger::sign() const{
    return signum;
}
// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(const BigInteger& N) const{
    //if the signs of the two BigInteger objects are different. then it returns -1 
    //if the sign of the first object is less than the sign of the second object, and 1 otherwise. 
    // because negative numbers are less than positive numbers in terms of absolute value.
    if (signum != N.signum) {
        return signum < N.signum ? -1 : 1;
    }
    //if the lengths of the two BigInteger objects' digit lists are different. 
    //then it returns -1 if the length of the first object's digit list is less than the length of the second object's digit list, and 1 otherwise.
    //BigInteger with fewer digits is less than one with more digits, assuming that they have the same sign. 
    if (digits.length() != N.digits.length()) {
        return signum * (digits.length() < N.digits.length() ? -1 : 1);
    }

    if (digits.length() == 0) {
        return 0;
    }

    List A = digits;
    List B = N.digits;
    A.moveFront();
    B.moveFront();
    // iterating over both digit lists simultaneously, starting from the front, and comparing each corresponding pair of digits. 
    //If a pair of digits is not equal, 
    //then returns -1 if the digit in the first object is less than the digit in the second object, and 1 otherwise.
    while (A.position() != A.length()) {
        if (A.peekNext() != B.peekNext()) {
            return signum * (A.peekNext() < B.peekNext() ? -1 : 1);
        }
        A.moveNext();
        B.moveNext();
    }
    //if all of the digits in both objects are equal, then the function returns 0 to indicate that the objects are equal.
    return 0;
}
// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero(){
    signum = 0;
    digits.clear();
}
// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate(){
    if(signum == 1){
        signum = -1;
    }
    if (signum == -1){
        signum = (1);
    }
}
// negateList()
// Changes the sign of each integer in List L. Used by sub().
void negateList(List& L){
    L.moveFront();
    while (L.position() < L.length()) {
        L.setAfter(-(L.peekNext()));
        L.moveNext();
    }
}
// scalarMultList()
// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List& L, ListElement m){
    L.moveFront();
    while (L.position() < L.length()) {
        L.setAfter(m*(L.peekNext()));
        L.moveNext();
    }
}
// normalizeList()
// Performs carries from right to left (least to most significant
// digits), then returns the sign of the resulting integer. Used
// by add(), sub() and mult().
int normalizeList(List& L){
    //normalizeList, is responsible for normalizing a list of digits after a mathematical operation has been performed on it.
    // In this context, normalization means removing leading zeros, ensuring that each digit in the list is within the range of 0 to (base - 1), 
    //and ensuring that the sign of the list is correct.Here is the line-by-line explanation of the function:
    //initializes two variables: carry to 0 and result to 0. It also moves the cursor to the back of the list.
    long carry = 0;
    long result = 0;
    L.moveBack();
    // checks if the list is negative by examining the sign of the first node in the list. If the first node is negative, then the list is negative.
    bool isNegative = false;
    if (L.front() < 0) {
        isNegative = true;
    }
     // loop through the list from the back to the front.
    while (L.position() > 0 || carry != 0){
       // if the cursor has reached the front of the list, insert a new node with the carry as its value and break out of the loop
        if (L.position() == 0){
            L.insertAfter(carry);
            carry = 0;
            break;
        }
        //sets the value of the current node to the sum of the current node and the carry value.
        L.setBefore(L.peekPrev() + carry);
        // check if the value of the current node exceeds the base
        if (L.peekPrev() >= base){
             // if the remainder of the value divided by the base is non-zero, 
             //subtract the remainder from the value, set the current node to the result, and calculate a new carry
            if ((L.peekPrev() % base) != 0){
                result = L.peekPrev() - (L.peekPrev() % base);
                L.setBefore(L.peekPrev() - result);
                carry = result/base;
            }
             // if the remainder is zero, calculate a new carry and set the current node to 0
            else{
                carry = L.peekPrev()/base;
                L.setBefore(0);
            }
        }
        // check if the value of the current node is negative and not the sign bit
        else if (L.peekPrev() < 0 && L.position() != 1){
            // if the remainder of the value divided by the base is non-zero, subtract the remainder plus the base from the value,
            // set the current node to the result, and calculate a new carry
            if ((L.peekPrev() % base) != 0){
                result = L.peekPrev() - (base + (L.peekPrev() % base));
                L.setBefore(L.peekPrev()-result);
                carry = (result/base);
            }
            // if the remainder is zero, calculate a new carry and set the current node to 0
            else{
                carry = L.peekPrev()/base;
                L.setBefore(0);
            }
        }
        // if the value of the current node is within the range of 0 to (base - 1), set the carry to 0
        else{
            carry = 0;
        }
        L.movePrev();
    }
    // remove leading zeros from the front of the list
    L.moveFront();
    while (L.length() > 1 && L.front() == 0){
        L.eraseAfter();
    }
    //if the list was negative, the function flips the sign of all nodes in the list, performs the same addition and carry logic as before, and returns -1. 
    L.moveFront();
    if (isNegative && L.peekNext() != 0) {
        //iterating through the list from back to front and adding the carry (which is initially set to -1) to each node in turn,
        // while adjusting the carry as necessary. Once the addition is complete, 
        //iterates through the list from front to back, removing any leading zeros that may have been created during the addition process. 
        //Finally, the function returns -1 to indicate that the result of the addition is negative.
        for (L.moveFront(); L.position() < L.length(); L.moveNext()){
            L.setAfter(-L.peekNext());
        }
        L.moveBack();
        while (L.position() > 0){
            L.setBefore(L.peekPrev() + carry);
            if (L.peekPrev() < 0){
                carry = -1;
                L.setBefore(L.peekPrev() + base);
            }
            else{
                carry = 0;
            }
            L.movePrev();
        }
        L.moveFront();
        while (L.length() > 1 && L.front() == 0){
            L.eraseAfter();
        }
        return -1;
    }
    //If the second node of the list is positive, returns 1 to indicate that the result of the addition is positive. 
    //If the second node of the list is 0, the function returns 0 to indicate that the result of the addition is 0.
    
    else if (L.peekNext() > 0){
        return 1;
    }
    else {
        return 0;
    }
}
// shiftList()
// Prepends p zero digits to L, multiplying L by base^p. Used by mult().
void shiftList(List& L, int p){
    for (int i = 0; i < p; i++){
        L.insertAfter(0);
    }
}
// sumList()
// Overwrites the state of S with A + sgn*B (considered as vectors).
// Used by both sum() and sub().
void sumList(List& S, List A, List B, int sgn){
    //purpose of this function is to compute the sum of two lists A and B, scaled by an integer sgn, and store the result in a third list S. 
    // create a temporary list T to store the sum of A and sgn*B. B is multiplied by sgn to ensure that it is added or subtracted as desired.
    List T; 
    scalarMultList(B, sgn); // multiply B by sgn
    A.moveBack(); // move to the end of A
    B.moveBack(); // move to the end of B
    long carry = 0;
    // iterating over the digits of A and B from right to left, adding the corresponding digits together along with any carry from the previous addition.
    while (A.position() != 0 || B.position() != 0 || carry != 0) {
        //In each iteration of the loop, we add the value of the digit at the current position of A to sum, 
        //and the value of the digit at the current position of B if it exists. We also add any previous carry that has not yet been added.
        long sum = (A.position() != 0 ? A.peekPrev() : 0);
        sum += (B.position() != 0 ? B.peekPrev() : 0);
        sum += carry;
        //calculate the carry by dividing sum by the base of the number system being used (which is stored in a global variable base). 
        //The remainder is the digit to be added to the sum.
        carry = sum / base; // calculate the carry
        sum %= base; // calculate the remainder
        // insert the digit sum into T after the current node.
        T.insertAfter(sum); // insert the remainder after the current node in T
        //move the iterators for A and B to the previous nodes, so that the next iteration will add the next digits.
        if (A.position() != 0) A.movePrev(); // move to the previous node of A
        if (B.position() != 0) B.movePrev(); // move to the previous node of B
    }
    normalizeList(T); // normalize T
    swap(S, T); // swap the contents of S and T
}

// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N) const{
    //creates two List objects x and y, which are initialized with the digits of
    // the current BigInteger object and the BigInteger object passed as an argument, respectively.
    List x = N.digits;
    List y = digits;
    //creates an empty BigInteger object z, which will hold the result of the addition operation.
    BigInteger z;
    //N is negative and this is positive. In this case, 
    //calls the sumList function with y, x, and -1 as arguments. The -1 is used as a flag to indicate that x should be subtracted from y.
    if (N.signum == -1 && signum ==1){
        sumList(z.digits, y, x, -1);
    }
    //N is positive and this is negative. In this case, calls the sumList function with x, y, and -1 as arguments.
    if (N.signum == 1 && signum == -1){
        sumList(z.digits, x, y, -1);
    }
    //Both N and this are positive. In this case, 
    // calls the sumList function with y, x, and 1 as arguments. The 1 is used as a flag to indicate that x should be added to y.
    if (N.signum == 1 && signum == 1){
        sumList(z.digits, y, x, 1);
    }
    //Both N and this are negative. In this case, 
    // multiplies the digits of this by -1 using the scalarMultList function, and then calls sumList with y, x, and -1 as arguments.
    if (N.signum == -1 && signum == -1){
        scalarMultList(y, -1);
        sumList(z.digits, y, x, -1);
    }
    //If N is zero, sets z.digits to x (the digits of this) and sets z.signum to N.signum.  then returns z.
    if (N.signum == 0){
        z.digits = x;
        z.signum = N.signum;
        return z;
    }
    //If this is zero,  sets z.digits to digits (the digits of N) and sets z.signum to signum. then returns z.
    if(signum == 0){
        z.digits = digits;
        z.signum = signum;
        return z;
    }

    //normalizes the digits of z using the normalizeList function and sets the sign of z based on the result of the normalization.
    // The normalized z is then returned.
    int norma_li = normalizeList(z.digits);
    z.signum = norma_li;
    return z;
}
// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N) const{
    //creates two List objects x and y, which are initialized with the digits of
    // the current BigInteger object and the BigInteger object passed as an argument, respectively.
    List x = N.digits;
    List y = digits;
    //creates an empty BigInteger object z, which will hold the result of the addition operation.
    BigInteger z;
    //N is negative and this is positive. In this case, 
    //calls the sumList function with y, x, and -1 as arguments. The 1 is used as a flag to indicate that x should be subtracted from y.
    if (N.signum == -1 && signum ==1){
        sumList(z.digits, y, x, 1);
    }
    //N is positive and this is negative. In this case, calls the sumList function with x, y, and -1 as arguments.
    if (N.signum == 1 && signum == -1){
        scalarMultList(y, -1);
        sumList(z.digits, x, y, -1);
    }
    //Both N and this are positive. In this case, 
    // calls the sumList function with y, x, and -1 as arguments. The -1 is used as a flag to indicate that x should be subtract to y.
    if (N.signum == 1 && signum == 1){
        sumList(z.digits, y, x, -1);
    }
    //Both N and this are negative. In this case, 
    // multiplies the digits of this by -1 using the scalarMultList function, and then calls sumList with y, x, and 1 as arguments.
    if (N.signum == -1 && signum == -1){
        scalarMultList(y, -1);
        sumList(z.digits, y, x, 1);
    }

    //normalizes the digits of z using the normalizeList function and sets the sign of z based on the result of the normalization.
    // The normalized z is then returned.
    int norma_li = normalizeList(z.digits);
    z.signum = norma_li;
    return z;
}
// mult()
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N) const{
    //create copies of the digits list of the two input BigIntegers
    List x = digits;
    List y = N.digits;
    List prod = List();
    //create a new list to store the product of the two BigIntegers
    BigInteger z = BigInteger();
    //set the sign of the result to positive if both input BigIntegers have the same sign, and negative otherwise
    if (signum == N.signum){
        z.signum = 1;
    }
    else if (signum != N.signum){
        z.signum = -1;
    }
    // If either input BigInteger is zero, the result is also zero, set the sign of the result to zero and return it
    if (signum == 0 || N.signum == 0){
        z.signum = 0;
        return z;
    }
    //loop through the digits of the second input BigInteger, starting from the least significant digit
    y.moveBack();
    while (y.position() > 0) {
         // For each digit in the second input BigInteger, we multiply it with the first input BigInteger and store the result in a new list called prod
        scalarMultList(x, y.peekPrev());
        //  add the product to the accumulated result so far, which is stored in the prod list
        sumList(prod, prod, x, 1);
        //normalize the prod list to ensure that each digit is between 0 and the base - 1, and carry over any overflow to the next digit
        normalizeList(prod);
        //compute the number of shifts required to align the prod list with the corresponding digit in the second input BigInteger, 
        //and shift the prod list accordingly
        int shifts = N.digits.length() - y.position();
        prod.moveBack();
        shiftList(prod, shifts);
        // We add the shifted prod list to the final result, which is stored in the BigInteger z
        sumList(z.digits, z.digits, prod, 1);
        //normalize the final result to ensure that each digit is between 0 and the base - 1, and carry over any overflow to the next digit
        normalizeList(z.digits);
        //We clear the prod list to prepare for the next iteration of the loop
        prod.clear();
        //reset the first input BigInteger to its original value, and move to the next digit of the second input BigInteger
        x = digits;
        y.movePrev();
    }
    
    return z;
}

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string(){
    //creates an empty string big_int. If the signum of the BigInteger object is 0, 
    //appends "0" to big_int and returns it. If the signum is -1, it appends "-" to the string big_int to indicate a negative number.
    std::string big_int = "";

    if (signum == 0){
        big_int = "0";
        return big_int;
    }

    if (signum == -1){
        big_int+="-";
    }
    //iterates over the doubly linked list of digits in the BigInteger object 
    digits.moveFront();
    while (digits.position() < digits.length()) {
        //If the position of the iterator is 0 and the next digit is not 0, converts the digit to a string using std::to_string() and appends it to big_int.
        if (digits.position() == 0 && digits.peekNext() != 0){
            big_int += std::to_string(digits.peekNext());
        }
        //If the next digit is negative, the absolute value of the digit is converted to a string and appended to big_int.
        else if (digits.peekNext() < 0){
            big_int += std::to_string(-digits.peekNext());
        }
        //If the next digit is positive, the absolute value of the digit is converted to a string and 
        //the length of the string is compared to the power of the number system. If the length of the string is less than the power of the number system,
        //  appends leading zeros to big_int to make up the difference. then appends the digit to big_int.
        else{
            std::string digits_str = std::to_string(digits.peekNext());
            int num_zeros = power - digits_str.length();
            int j = 0;
            while (j < num_zeros) {
                big_int += "0";
                j++;
            }
            big_int+=digits_str;
        }
        digits.moveNext();
    }
    return big_int;
}
// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N ){
    return stream << N.to_string();
}
// operator+()
// Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    BigInteger C = A.add(B);
    return C;
}
// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    BigInteger C = A.add(B);
    std::swap(A.signum, C.signum);
    A.digits = C.digits;
    return A;
}
BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    BigInteger C = A.sub(B);
    return C;
}
// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    BigInteger C = A.sub(B);
    std::swap(A.signum, C.signum);
    A.digits = C.digits;
    return A;
}
BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    BigInteger C;
    C = A.mult(B);
    return C;
}
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    BigInteger C = A.mult(B);
    std::swap(A.signum, C.signum);
    A.digits = C.digits;
    return A;
}
// operator==()
// Returns true if and only if A equals B. 
bool operator==( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 0){
        return true;
    }
    return false;
}
// operator<()
// Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == -1){
        return true;
    }
    return false;
}
// operator<=()
// Returns true if and only if A is less than or equal to B. 
bool operator<=( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == -1 || A.compare(B) == 0){
        return true;
    }
    return false;
}
// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 1){
        return true;
    }
    return false;
}
// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 1 || A.compare(B) == 0){
        return true;
    }
    return false;
}


