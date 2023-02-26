/********************************************************************************* 
* Nghia Pham, ngmpham 
* 2023 Winter CSE101 PA5
* List.cpp 
* List ADT functions 
*********************************************************************************/ 
#include<iostream>
#include "List.h"
#include<string>
#include<stdexcept>

// Private Constructor --------------------------------------------------------

// Node constructor
List::Node::Node(ListElement x){
   data = x;
   next = nullptr;
   prev = nullptr;
}

 List::List(){
    frontDummy = new Node(-1);
    frontDummy->next = backDummy;
    backDummy = new Node(-1);
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
 }

 // Copy constructor.
List::List(const List& L){
    // base on class quene.ccp class example
    frontDummy = new Node(-1);
    frontDummy->next = backDummy;
    backDummy = new Node(-1);
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
    if (L.length() != 0){
        Node* N = L.frontDummy;
        while (N->next != L.backDummy){
            this->insertBefore(N->next->data);
            N = N->next;
        }
    }
}

List::~List(){
    // remove all elements from the list. It then deletes the frontDummy and backDummy nodes, which were used to simplify list operations.
    //sets the beforeCursor and afterCursor pointers to nullptr, sets pos_cursor to 0, and deallocates any other resources held by the List object. 
    clear();
    delete frontDummy;
    delete backDummy;
    beforeCursor = nullptr;
    afterCursor = nullptr;
    pos_cursor = 0;
}
 // length()
   // Returns the length of this List.
int List::length() const{
    return (num_elements);
}
// front()
// Returns the front element in this List.
// pre: length()>0
ListElement List::front() const{
    if(num_elements <= 0){
        throw std::length_error("List: front(): empty list");
    }
    return (frontDummy->next->data);
}
ListElement List::back() const{
    if(num_elements <= 0){
        throw std::length_error("List: back(): empty list");
    }
    return (backDummy->prev->data);
}
// position()
// Returns the position of cursor in this List: 0 <= position() <= length().
int List:: position() const{
    return (pos_cursor);
}

// peekNext()
// Returns the element after the cursor.
// pre: position()<length()
ListElement List::peekNext() const{
    if (pos_cursor >= num_elements) {
        throw std::range_error("List: peekNext(): cursor at back");
    }
    return afterCursor->data;
}

// peekPrev()
// Returns the element before the cursor.
// pre: position()>0
ListElement List::peekPrev() const{
    if (pos_cursor <= 0){
        throw std::range_error("List: peekPrev(): cursor at front");
    }
    return (beforeCursor->data);
}
// clear()
// Deletes all elements in this List, setting it to the empty state.
void List::clear(){
    //  a loop that continues until there are no more elements in the list (num_elements is 0). In each iteration of the loop, 
    //it calls the eraseAfter() function, which removes the element immediately after the cursor. 
    //This effectively removes all elements from the list, as it repeatedly erases the element after the cursor until the list is empty.
    moveFront();
    while (num_elements != 0){
        eraseAfter();
    }   

}

//error here

// moveFront()
// Moves cursor to position 0 in this List.
void List::moveFront(){
    pos_cursor = 0;
    afterCursor = frontDummy->next;
    beforeCursor = frontDummy;
}

// moveBack()
// Moves cursor to position length() in this List.
void List::moveBack(){
    pos_cursor = num_elements;
    beforeCursor = backDummy->prev;
    afterCursor = backDummy;
}

// error here

// moveNext()
// Advances cursor to next higher position. Returns the List element that
// was passed over. 
// pre: position()<length() 
ListElement List::moveNext(){
    if (pos_cursor >= num_elements) {
        throw std::range_error("List: moveNext(): cursor at back");
    }
    //increments the cursor position by 1, moving it one position to the right.
    //sets the node before the cursor to be the node currently pointed to by the cursor.
    // sets the cursor to point to the next node in the list.
    //returns the List element that was just passed over.
    pos_cursor += 1;
    beforeCursor = afterCursor;
    afterCursor = afterCursor->next;
    return (beforeCursor->data);
}

// movePrev()
// Advances cursor to next lower position. Returns the List element that
// was passed over. 
// pre: position()>0
ListElement List::movePrev(){
    if (pos_cursor <= 0) {
        throw std::range_error("List: movePrev(): cursor at front");
    }
    pos_cursor-= 1;
    afterCursor = beforeCursor;
    beforeCursor = beforeCursor->prev;
    return(afterCursor->data);
}


// insertAfter()
// Inserts x after cursor.
void List::insertAfter(ListElement x){
    Node* new_node = new Node(x);
    //The prev pointer of the new node is set to the node before the cursor.
    //The next pointer of the node before the cursor is set to the new node.
    //The next pointer of the new node is set to the node after the cursor.
    //The prev pointer of the node after the cursor is set to the new node.
    //The cursor is moved to the new node (which is now after the cursor).
    new_node->prev = beforeCursor;
    beforeCursor->next = new_node;
    new_node->next = afterCursor;
    afterCursor->prev = new_node;
    afterCursor = new_node;
    num_elements++;
}

// insertBefore()
// Inserts x before cursor.
void List::insertBefore(ListElement x){
    Node* new_node = new Node(x);
    //Sets the previous pointer of the new node to point to the node before the cursor 
    //Sets the next pointer of the node before the cursor (beforeCursor) to point to the new node.
    //Sets the next pointer of the new node to point to the node after the cursor (afterCursor).
    //Sets the previous pointer of the node after the cursor (afterCursor) to point to the new node.
    //Sets the cursor to point to the new node.
    new_node->prev = beforeCursor;
    beforeCursor->next = new_node;
    new_node->next = afterCursor;
    afterCursor->prev = new_node;
    beforeCursor = new_node;
    num_elements++;
    pos_cursor++;
}
// setAfter()
// Overwrites the List element after the cursor with x.
// pre: position()<length()
void List::setAfter(ListElement x){
    if (pos_cursor >= num_elements) {
        throw std::range_error("List: setAfter(): cursor at back");
    }
    afterCursor->data = x;
}

// setBefore()
// Overwrites the List element before the cursor with x.
// pre: position()>0
void List::setBefore(ListElement x){
    if (pos_cursor <= 0) {
        throw std::range_error("List: setBefore(): cursor at front");
    }
    beforeCursor->data = x;
}

// eraseAfter()
// Deletes element after cursor.
// pre: position()<length()
void List::eraseAfter(){
    if (pos_cursor >= num_elements) {
        throw std::range_error("List: eraseAfter(): cursor at back");
    }
    //Updates afterCursor to point to the node that follows the one that will be deleted (i.e. afterCursor = node_delete->next;).
    //Updates the next pointer of the node before the one that will be deleted to point to the node that follows the one that will be deleted.
    //Updates the prev pointer of the node that follows the one that will be deleted to point to the node before the one that will be deleted
    Node* node_delete = afterCursor;
    afterCursor = node_delete->next;
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;
    delete node_delete;
    num_elements--;
}

// eraseBefore()
// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore(){
    if (pos_cursor <= 0) {
        throw std::range_error("List: eraseBefore(): cursor at front");
    }
    //updates the beforeCursor pointer to point to the previous node of the Node_f node, effectively removing Node_f from the list.
    // updates the prev pointer of the node after the cursor (afterCursor) to point to the previous node of Node_f.
    //deletes the Node_f node, freeing up the memory it occupied.
    //decrements the number of elements in the list (num_elements) and decrements the position of the cursor (pos_cursor) to reflect the removal of the node.
    Node* Node_f = beforeCursor;
    beforeCursor = Node_f->prev;
    Node_f->prev->next = afterCursor;
    afterCursor->prev = Node_f->prev;
    delete Node_f;
    num_elements--;
    pos_cursor--;
}

// findNext()
// Starting from the current cursor position, performs a linear search (in 
// the direction front-to-back) for the first occurrence of element x. If x
// is found, places the cursor immediately after the found element, then 
// returns the final cursor position. If x is not found, places the cursor 
// at position length(), and returns -1. 
int List::findNext(ListElement x){
    while(pos_cursor != num_elements){
        if(afterCursor->data == x){
            moveNext();
            return pos_cursor;
        }
        moveNext();
    }
    return -1;
}

// findPrev()
// Starting from the current cursor position, performs a linear search (in 
// the direction back-to-front) for the first occurrence of element x. If x
// is found, places the cursor immediately before the found element, then
// returns the final cursor position. If x is not found, places the cursor 
// at position 0, and returns -1. 
int List::findPrev(ListElement x){
    while(pos_cursor != 0){
        if(beforeCursor->data == x){
            movePrev();
            return pos_cursor;
        }
        movePrev();
    }
    return -1;
}

// cleanup()
// Removes any repeated elements in this List, leaving only unique elements.
// The order of the remaining elements is obtained by retaining the frontmost 
// occurrance of each element, and removing all other occurances. The cursor 
// is not moved with respect to the retained elements, i.e. it lies between 
// the same two retained elements that it did before cleanup() was called.
void List::cleanup(){

    if (num_elements < 2) {
        return;  // nothing to clean up in lists of length 0 or 1
    }

    Node* prev = frontDummy->next;  // element before the cursor
    Node* curr = prev->next;        // current element
    Node* last_unique = curr;       // last unique element found so far
    //loop runs until "curr" points to the last element in the list.
    //Inside the while loop, another while loop runs from the first element of the list up to the current element, "curr", 
    //and checks if there are any elements that match "curr".
    while (curr != backDummy) {
        bool found_dup = false;
        Node* runner = frontDummy->next;
        while (runner != curr) {
            if (runner->data == curr->data) {
                found_dup = true;
                break;
            }
            runner = runner->next;
        }
    //If a duplicate is found, it is removed from the list, and the beforeCursor, afterCursor, and pos_cursor pointers are updated to reflect the deletion.
        if (found_dup) {
            //If the current element (curr) is the same as the element before the cursor (beforeCursor), 
            //the beforeCursor is updated to point to the previous element (prev). 
            if (curr == beforeCursor) {
                beforeCursor = prev;
            //If curr is the same as the element after the cursor (afterCursor), afterCursor is updated to point to the next element (curr->next).
            } else if (curr == afterCursor) {
                afterCursor = curr->next;
            }
            //removes the duplicate element from the list by updating the pointers of the previous and next elements of the duplicate element to point to each other 
            //decreases the num_elements counter and deletes the memory allocated for the duplicate element using delete temp.
            Node* temp = curr;
            curr = curr->next;
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            delete temp;
            num_elements--;
            if (pos_cursor > num_elements) {
                pos_cursor = num_elements;
            }
        //If a unique element is found, "last_unique" is set to "curr", "prev" is set to "curr", and "curr" is incremented to the next element in the list.
        } else {
            last_unique = curr;
            prev = curr;
            curr = curr->next;
        }
    }

     //if the cursor position is greater than or equal to the number of elements in the list. 
     //If so, it sets the cursor position to the last element's index in the list. 
     //This handles the case where the last element has been removed from the list and the cursor is positioned beyond it.
    if (pos_cursor >= num_elements) {
        pos_cursor = num_elements - 1;
    }
    //if the cursor is at the beginning of the list (i.e., its index is 0). 
    //If so, it sets the beforeCursor pointer to the frontDummy node and the afterCursor pointer to the first node in the list.
    if (pos_cursor == 0) {
        beforeCursor = frontDummy;
        afterCursor = frontDummy->next;
    //If the cursor is at the end of the list (i.e., its index is num_elements - 1), 
    //it sets the beforeCursor pointer to the last unique node found in the list (stored in the last_unique variable) 
    //and the afterCursor pointer to the backDummy node.
    } else if (pos_cursor == num_elements - 1) {
        beforeCursor = last_unique->prev;
        afterCursor = backDummy;
    } else {
        //If the cursor is not at the beginning or end of the list, it sets the beforeCursor pointer to the node immediately preceding the cursor
        // (by traversing the list from the frontDummy node) and the afterCursor pointer to the node immediately following the cursor.
        beforeCursor = frontDummy->next;
        for (int i = 1; i < pos_cursor; i++) {
            beforeCursor = beforeCursor->next;
        }
        afterCursor = beforeCursor->next;
    }
}
// concat()
// Returns a new List consisting of the elements of this List, followed by
// the elements of L. The cursor in the returned List will be at postion 0.
List List::concat(const List& L) const{
    // create a new List object result with the same elements as the current list.
    List result = *this;
    //iterates through the elements in L, starting from the first element after the frontDummy node and ending with the node right before the backDummy node. 
    // For each node in L, it inserts the corresponding ListElement into result using the insertBefore 
    for (Node* node = L.frontDummy->next; node != L.backDummy; node = node->next) {
        result.insertBefore(node->data); // insert each element of L at the end of result
    }
    result.moveFront(); // move the cursor to position 0
    return result;
}


// to_string()
// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string() const{
    Node* N = nullptr;
    std::string s = "";

    for(N=frontDummy; N->next!=backDummy; N=N->next){
        s += std::to_string(N->next->data)+" ";
    }
   
   return s;
}
// operator<<()
// Inserts string representation of L into stream.
std::ostream& operator<<( std::ostream& stream, const List& L ){
    return stream << L.List::to_string();
}
// equals()
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R) const{
    if(num_elements != R.length()){
        return false;
    }
    Node* x = frontDummy;
    Node* y = R.frontDummy;
    while (x != backDummy && y != R.backDummy){
        if (x->next->data != y->next->data){
            return false;
        }
        x = x->next;
        y = y->next;
    }
    return true;
}

bool operator==( const List& A, const List& B ){
    return A.List::equals(B);
}

// operator=()
// Overwrites the state of this List with state of L.
List& List::operator=( const List& L ){
    if( this != &L ){ // not self assignment
      // make a copy of Q
      List temp = L;

      // then swap the copy's fields with fields of this
      std::swap(pos_cursor, temp.pos_cursor);
      std::swap(num_elements, temp.num_elements);
      std::swap(afterCursor, temp.afterCursor);
      std::swap(beforeCursor, temp.beforeCursor);
      std::swap(frontDummy, temp.frontDummy);
      std::swap(backDummy, temp.backDummy);

   }

   // return this with the new data installed
   return *this;

   // the copy, if there is one, is deleted upon return
}






