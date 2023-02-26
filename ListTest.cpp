/********************************************************************************* 
* Nghia Pham, ngmpham 
* 2023 Winter CSE101 PA5
* ListTest.cpp  
*********************************************************************************/ 
#include<iostream>
#include<string>
#include<stdexcept>
#include"List.h"

using namespace std;

int main(){
   List A, B, C;
   A.insertBefore(1);
   A.insertBefore(2);
   A.insertBefore(3);
   A.insertBefore(3);
   A.insertBefore(4);
   A.insertBefore(2);
   A.insertBefore(5);

   B.insertAfter(4);
   B.insertAfter(3);
   B.insertAfter(1);
   B.insertAfter(7);
   B.insertAfter(8);
   B.insertAfter(9);
   B.insertAfter(2);
   
   if(A.length() == 7){
      cout << "insertbefore worked passed "<< endl;
   }else{
      cout << "insertbefore failed "<< endl;
   }
   if(B.length() == 7){
      cout << "insertAfter worked passed "<< endl;
   }else{
      cout << "insertAfter failed "<< endl;
   }
   if(A.findPrev(2) == 5){
      cout << "findPrev worked passed "<< endl;
   }else{
      cout << "findPrev failed "<< endl;
   }
   A.moveFront();
   if(A.front() == 1){
      cout << "moveFront and front worked passed "<< endl;
   }else{
      cout << " failed "<< endl;
   }
   if(A.findNext(3) == 3){
      cout << "findNext worked passed "<< endl;
   }else{
      cout << " failed "<< endl;
   }
   A.moveBack();
   if(A.back() == 5){
      cout << "moveBack and back worked passed "<< endl;
   }else{
      cout << " failed "<< endl;
   }
   A.moveFront();
   A.peekNext();
   if(A.peekNext() == 1){
      cout << "A.peekNext()worked passed "<< endl;
   }else{
      cout << " failed "<< endl;
   }

   A.moveNext();
   if(A.position() == 1){
      cout << "moveNext worked passed "<< endl;
   }else{
      cout << " failed "<< endl;
   }
   A.movePrev();
   if(A.position() == 0){
      cout << "movePrev worked passed "<< endl;
   }else{
      cout << " failed "<< endl;
   }
   A.cleanup();
   if(A.length() == 5){
      cout << "cleanup() worked passed "<< endl;
   }else{
      cout << " failed "<< endl;
   }
   C = A.concat(B);
   if(C.length() == 12){
      cout << "concat() worked passed "<< endl;
   }else{
      cout << " failed "<< endl;
   }
   A.eraseAfter();
   cout << "Eraseafter()A = " << A << endl;
   cout << "EraseAfter() Pass! " << endl;
   B.moveBack();
   B.eraseBefore();
   cout << "Erasebefore()B = " << B << endl;
   cout << "Erasebefore() Pass! " << endl;
    
   
   return(EXIT_SUCCESS);
}
