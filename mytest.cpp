/*****************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 3, Fall 2023
** Author:  Jervon Drakes
** Date:    11/14/2023
** E-mail:  jdrakes1@umbc.edu
** 
**
** This file contains the mytest.cpp file for Project 3.
** This program implements functions for the tester class to 
** implement numerous test cases for the functions in pqueue.cpp
**
**
**
***********************************************/



#include "pqueue.h"

int priorityFn1(const Patient& patient){

  return patient.getTemperature() + patient.getRR() + patient.getBP();


}

int priorityFn2(const Patient& patient){


  return patient.getOpinion() + patient.getOxygen();

    
}

class Tester{

public:
  
  // Function: PQueue::insertPatient(const Patient& input)
  // Case: Tests insertion for a normal case for a minheap
  // Expected Result: The function should return true indicating that the data structure 
  // correctl maintains the min-heap property 
  bool minHeapInsertion(){

    PQueue minHeap(priorityFn2, MINHEAP, LEFTIST);

    int numNodes = 300;
    // insert a number of patients
    for(int i = 1; i <= numNodes; i++){

      Patient patient("Patient", i, i, i, i, i);
      minHeap.insertPatient(patient);

    }

    // determine if the tree satisfies the min heap property
    bool minHeapValid = isMinHeapProperty(minHeap.m_heap);

    return minHeapValid;

  }

  
  // Function: PQueue::insertPatient(const Patient& input)
  // Case: Tests insertion for a normal case for a maxheap
  // Expected Result: The function should return true as the data structure correctly 
  //maintains the max-heap property 
  bool maxHeapInsertion() {

    PQueue maxHeap(priorityFn1, MAXHEAP, LEFTIST);

    int numNodes = 300;
    // insert a number of patients
    for(int i = numNodes; i >= 1; i--){

      Patient patient("Patient", i, i, i, i, i);
      maxHeap.insertPatient(patient);

    }

    // determine if the tree satisfies the min heap property
    bool maxHeapValid = isMaxHeapProperty(maxHeap.m_heap);

    

    return maxHeapValid;

  }


  // Function: Helper function to tests insertion for a normal case of min-heap
  // Case: Tests insertion for a normal case for a minheap
  // Expected Result: The function should return true indicating that the data structure 
  // correctly maintains the min-heap property 
  bool isMinHeapProperty(Node* node){


    bool result = true;
    // an empty tree is a min heap by default
    if(node == nullptr){

      return result;
      
    }

    
    // determine the heap property of the current node
    if(node->m_left != nullptr && priorityFn2(node->getPatient()) > priorityFn2(node->m_left->getPatient())) {

      result = false;
    }

    if(node->m_right != nullptr && priorityFn2(node->getPatient()) > priorityFn2(node->m_right->getPatient())){

      result = false;
    }

    

    // recursively check the left and right subtrees
    return result && isMinHeapProperty(node->m_left) && isMinHeapProperty(node->m_right);

  }

  // Function: Helper function to test the insertion for a normal case of max-heap
  // Case: Tests insertion for a normal case for a maxheap
  // Expected Result: The function should return true as the data structure correctly 
  //maintains the max-heap property 
  bool isMaxHeapProperty(Node* node){

    bool result = true;
    
    // an empty tree is a max heap by default
    if(node == nullptr){

      return result;

    }


    
    // determine the heap property of the current node
    if(node->m_left != nullptr && priorityFn1(node->getPatient()) < priorityFn2(node->m_left->getPatient())) {

      
      result = false;
    }

    if(node->m_right != nullptr && priorityFn1(node->getPatient()) < priorityFn2(node->m_right->getPatient())){

      result = false;
    }

    
    // recursively check the left and right subtrees
    return result && isMaxHeapProperty(node->m_left) && isMaxHeapProperty(node->m_right);


  }


  // Function: Function to test the removal of nodes for a normal case in min-heap
  // Case: Tests removal for a normal case for a min-heap
  // Expected Result: The function should return true if all the patients are 
  // removed in the ascending order of their opinion values 
  bool testRemovalMinHeap(){

    // create a priority queue implemented as a min-heap using priorityFn1
    PQueue pq(priorityFn1, MINHEAP, LEFTIST);

    // insert 300 patients 
    for(int i = 0; i <= 300; ++i){

      Patient patient("Patient", i, 100, 20, 100, 10);
      pq.insertPatient(patient);
    }

    bool result = true;

    Patient prevPatient; // keep track of previous removed patient
    
    // extract and check patients from the min heap
    for(int i = 0; i <= 300; ++i){

      // get the next patient from the min-heap
      Patient patient = pq.getNextPatient();

      // determine the correctness of the removal order based on priority
      if( i > 0 && pq.getPriorityFn()(patient) > pq.getPriorityFn()(prevPatient)){
	result = false;
      }

      prevPatient = patient;

    }

    bool result1 = pq.numPatients() == 0;
    

    // after all removals heap should be empty
    return result && result1;
  }

  // Function: Function to test the removal of nodes for a normal case in max-heap
  // Case: Tests removal for a normal case for a max-heap
  // Expected Result: The function should return true if all the patients are 
  // removed in the descending order of their opinion values
  bool testRemovalMaxHeap(){

    //create a priority queue implemented as a max-heap using priorityFn2
    PQueue pq(priorityFn2, MAXHEAP, SKEW);

    // insert 300 patients
    for(int i = 0; i <= 300; ++i){

      Patient patient("Patient", i, 100, 20, 100,10);
      pq.insertPatient(patient);
    }

    bool result = true;

    // extract and check pateints from the max-heap
    for(int i = 0; i <= 300; ++i){
      
      // get the next patient from the max-heap
      Patient patient = pq.getNextPatient();

      // determine if the patient's opinion is not in the expected oreder
      if(pq.getPriorityFn()(patient) != i) {

	 result = false;

      }
      

    }

    
    bool result1 = pq.numPatients() == 0; // after all removals, heap should be empty
      
    return result && result1;
	
  }

  // Function: Function to test all nodes in a leftist heap have the correct NPL value 
  // Case: Tests all nodes in a leftist heap have the correct NPL value
  // Expected Result: The function returns the result of the 'NPLValueCheck' function which checks
  // the NPL property within the leftist heap and ensures that the NPL values are correctly maintained for
  // each node in the heap
  
  bool testNPL(const PQueue& pqueue){

    return testNPLRecursive(pqueue.m_heap);
  }

  // Function: helper functions for the testNPLLeftistHeap
  // Case: Tests all nodes in a leftist heap have the correct NPL value
  // Expected Result: The function should true if the NPL property is maintained throughout the leftist heap
  // indicating that the NPL values are correctly calculated

  bool testNPLRecursive(const Node* node){

    if(node == nullptr){

      return true; // empty subtree is a valid leftist heap

    }
  

    // calculate the expected NPL for the current node
    int nplExpected = (node->m_right == nullptr) ? 0 : node->m_right->getNPL() + 1;
  
    // determine if the calculated NPL matches the expected NPL
    if(node->getNPL() != nplExpected){

      return false;
    }

    // recursively check the left and right subtrees
    return testNPLRecursive(node->m_left) && testNPLRecursive(node->m_right);
  }


  // Function: Function to test that the leftist heap preserves the property of such a heap 
  // Case: Test a leftist heap preserves the property of such a heap, i.e. at every node the NPL value of the left child is greater 
  //than or equal to the NPL value of the right child
  // Expected Result: The function should true if the leftist property is maintained
  // for the entire leftist heap, indicating that the leftist property is satisfied 
  bool LeftistProperty(const PQueue& pqueue){

    return leftistPropertyHelper(pqueue.m_heap);

  }

  // Function: helper function to test the leftistProperty function 
  // Case: Test a leftist heap preserves the property of such a heap, i.e. at every node the NPL value of the left child is greater 
  // than or equal to the NPL value of the right child
  // Expected Result: The function should true if the leftist property is maintained
  // for the entire leftist heap, indicating that the leftist property is satisfied 
  bool leftistPropertyHelper(const Node* node){

    bool result = true;
    
    if(node == nullptr){

      return result;

    }

    // calculate NPL for left and right children
    int nplLeft = (node->m_left == nullptr) ? 0 : node->m_left->getNPL();
    int nplRight = (node->m_right == nullptr) ? 0 : node->m_right->getNPL();

    // determine if the leftist property is maintained(left NPL >= right NPL)
    if(nplLeft < nplRight){

      result = false;

    }

    bool result1;

    // recursively check the leftist property for left and right subtree
    result1 = leftistPropertyHelper(node->m_left) && leftistPropertyHelper(node->m_right);

    return result && result1;

  }

  //Function: Function to tests whether after changing the priority function
  //a correct heap is rebuilt with the same data and different priority function
  //Case: Test whether after changing the priority function a correct heap is 
  //rebuilt with the same data and the different priority function.
  //Expected Result: The fucntion should return true if the priority function can be changed
  // and the priority queue maintains its heap property and size  
  bool changePriorityFunction(PQueue& originalPqueue, prifn_t newPriFn){

    
    PQueue copiedQueue = originalPqueue; // create a copy of the original priority queue

    int originalSize = originalPqueue.numPatients(); // get the original size of the priority queue

    // set the new priority function for the original queue
    originalPqueue.setPriorityFn(newPriFn, originalPqueue.getHeapType());

    bool result = true;
    
    //determine if the priority is updated
    if(originalPqueue.getPriorityFn() != newPriFn){

      result = false;
      
    } 

    // test of the heap is still valid with the new priority function
    bool validHeap = LeftistProperty(originalPqueue) && originalPqueue.numPatients() == originalSize;
    
    // restore original heap
    originalPqueue = copiedQueue;

    return result && validHeap;
  }

  //Function: PQueue::mergeWithQueue(PQueue& rhs)
  //Case: Test merge of an empty queue (an edge case) with a normal queue. This is a call to the function PQueue::mergeWithQueue(PQueue& rhs) 
  //where rhs is a normally populated queue.
  //Expected Result: The function should return true since merging an empty queue into a non empty queue should
  // throw an exception
  bool mergeEmptyQueue(PQueue& nonEmptyQueue){
    
    // create an empty and non-empty queue
    PQueue emptyQueue(priorityFn1, MINHEAP, LEFTIST);
    

    bool result = false;
    try{
      
      // attempt to merge the empty queue into the non-empty queue
      nonEmptyQueue.mergeWithQueue(emptyQueue);

    } catch (domain_error& e){

      result = true; // expected domain_error exception was thrown
    }

    return result;
  }

  //Function: PQueue(const PQueue& rhs)
  //Case: Test the PQueue class copy constructor for a normal case.
  //Expected Result: The function should return true if the copy constructor successfully
  // creates a copy of the source queue, maintaining all properties and the number of patients 
  bool NormalCopyConstructor(){

    // create a source priority queue with speicified properties and patients
    PQueue pq1(priorityFn1, MINHEAP, SKEW);
    Patient patient1("John", 36, 98, 15, 120, 7);
    Patient patient2("Bob", 37, 96, 22, 130, 5);
    pq1.insertPatient(patient1);
    pq1.insertPatient(patient2);

    // create a new priority queue using the copy constructor
    PQueue pq2(pq1);

    bool result = true;
    
    // determine if the properties of the second PQueue match the first
    if(pq2.getPriorityFn() != pq1.getPriorityFn() || pq2.getHeapType() != pq1.getHeapType() || pq2.getStructure() != pq1.getStructure()){

      result = false;
    }

    bool result1 = true;
    
    // determine if the number of patients in the second PQueue matches that of the first
    if(pq2.numPatients() != pq1.numPatients()){

      result1 = false;
    }
    
    

    return result && result1;
  }

  // Function: PQueue(const PQueue& rhs)
  // Case: Test the PQueue class copy constructor for an edge case.
  // Expected Result: The function should return true if the cpy constructor
  // successfully creates a copy of the source priority queue with all properties
  // and same number of patients
  bool EdgeCaseCopyConstructor(){
    
    // create a source priority queue qith specified properties 
    PQueue pq1(priorityFn2, MAXHEAP, LEFTIST);
    
    // create a new priority queue using the copy constructor
    PQueue pq2(pq1);
    
    // determine if the number of patients in pq2 matches that of pq1 and if their properties are the same
    return pq1.numPatients() == pq2.numPatients() && pq1.getHeapType() == pq2.getHeapType() && pq1.getStructure() == pq2.getStructure() && pq1.getPriorityFn() == pq2.getPriorityFn();
  }

  //Function: PQueue::operator=(const PQueue& rhs)
  // Case: Test the PQueue class assignment operator for a normal case.
  // Expected Result: The function shoudl return true if the assignment operator
  // successfully copies the content and properties of the source priority queue
  // into the target queue
  bool NormalAssignmentOperator(){

    // create a source priority queue with the specified properties and a patient
    PQueue pq1(priorityFn1, MINHEAP, SKEW);
    Patient patient1("John", 36, 98, 15, 120, 7);
    pq1.insertPatient(patient1);

    // create a target priority queue
    PQueue pq2(priorityFn2, MAXHEAP, LEFTIST);
    
    // use the assignment operator to copy the content and properties of pq1 into pq2 
    pq2 = pq1;

    // determine if the two queues are equal
    return pq1.numPatients() == pq2.numPatients() && pq1.getHeapType() == pq2.getHeapType() && pq1.getStructure() == pq2.getStructure() && pq1.getPriorityFn() == pq2.getPriorityFn();
  }

  //Function: PQueue::operator=(const PQueue& rhs)
  // Case: Test the PQueue class assignment operator for an Edge case.
  // Expected Result: The function should return true if the assignment operator
  // successfully copies the content and properties of the source priority queue
  // into the target queue even when the configurations are different
  bool AssignmentOperatorEdgeCase(){
    
    // create a source priority queue with specified properties 
    PQueue pq1(priorityFn2, MAXHEAP, LEFTIST);

    // create a target queue with different properties
    PQueue pq2(priorityFn1, MINHEAP, SKEW);

    // use the assignment operator to copy tbe content and properties of pq1 into pq2
    pq2 = pq1;

    // determine if the two queues are equal
    return pq1.numPatients() == pq2.numPatients() && pq1.getHeapType() == pq2.getHeapType() && pq1.getStructure() == pq2.getStructure() && pq1.getPriorityFn() == pq2.getPriorityFn();
  }

  // Function: PQueue::getNextPatient()
  // Case: Test that attempting to dequeue an empty queue throws an out_of_range exception
  // Expected Result: The function returns true if the expected exceptions is thrown
  // indicating that dequeuing from an empty queue is handled correctly
  bool dequeueEmptyQueue(){
    
    bool result = true;

    try {

      PQueue pq(priorityFn1, MINHEAP, SKEW);
      pq.getNextPatient();

      result = false; // dequeue should throw an exception

    } catch(const out_of_range& e){

      result = true; // dequeue threw the expected out_of_range exception

    }

    return result;

  }

  //Function: PQueue::mergeWithQueue(PQueue& rhs)
  //Case: Test that attempting to merge queues with different priority 
  //functions throws a domain_error exception.
  //Expected Result: The fucntion should return true which indicated that the exception was thrown as expected
  bool mergeWithDifferentPriorityFns(){

    bool result = true;
    try{

      PQueue pq(priorityFn1, MINHEAP, SKEW);
      PQueue pq1(priorityFn2, MINHEAP, SKEW);
      
      // attempt to merge pq1 into pq
      pq.mergeWithQueue(pq1);

      result = false; // merging with different priority functions should throw an exception
    
    } catch (const domain_error& e){

      result = true; // merging threw the exected domain_error exception

    }

    return result;

  }
  
};




int main() {    
 
    
  Tester tester;

  PQueue pqueue(priorityFn2, MINHEAP, LEFTIST);
  int nodes = 300;

  for(int i = 1; i<= nodes; i++){
    Patient patient("Patient" , i, 100, 20, 100, 10 );
    pqueue.insertPatient(patient);

  }


  cout << "Testing Min-Heap Insertion normal case: " << endl;
  if(tester.minHeapInsertion()){

    cout << "Min heap insertion normal case passed" << endl;
      
  } else {

    cout << "Min heap insertion normal case failed" << endl;

  }

  cout << "                                        " << endl;
  
  cout << "Testing Max-Heap Insertion normal case: " << endl;
  
  if(tester.maxHeapInsertion()){

    cout << "Max heap insertion normal case passed" << endl;

  } else {

    cout << "Max heap insertion normal case failed" << endl;

  }

  cout << "                                        " << endl;

  cout << "Testing Min-Heap Removal normal case: " << endl;
  if(tester.testRemovalMinHeap()){

    cout << "Min heap removal normal case failed" << endl;

  } else {

    cout << "Min heap removal normal case passed" << endl;

  }

  cout << "                                        " << endl;


  cout <<"Testing Max-Heap Removal normal case: " << endl;
  if(tester.testRemovalMaxHeap()){

    cout << "Max Heap removal normal case failed" << endl;

  } else {

    cout << "Max heap removal normal case passed" << endl;

  }

  cout << "                                        " << endl;


  cout << "Testing all nodes in a leftist heap have the correct NPL value: " << endl;
  
  if(tester.testNPL(pqueue)){

    cout << "All nodes in a leftist heap have the correct NPL value" << endl;
    
  } else {

    cout << "Nodes in a leftist heap do not have the correct NPL value" << endl;

  }

  cout << "                                        " << endl;


  cout <<"Testing a leftist heap preserves the property of such heap: " << endl;
  if(tester.LeftistProperty(pqueue)){

    cout << "Leftist property is preserved" << endl;

  } else {

    cout << "Leftist property is not preserved" << endl;

  }

  cout << "                                        " << endl;


  cout <<"Testing whether after changing the priority function a correct heap is rebuilt with the same data and the different  priority function: " << endl;
  if(tester.changePriorityFunction(pqueue, priorityFn1)){

    cout << "Priority function changed and maintained data integrity." << endl;

  } else {

    cout << "Priority function change failed or data integrity not maintained" << endl;

  }

  cout << "                                        " << endl;


  cout <<"Testing the merge of an empty queue with a normal queue: " << endl;
  if(tester.mergeEmptyQueue(pqueue)){

      cout << "Merging an empty queue with a non-empty queue is successful" << endl;

  } else {

    cout << "Merging an empty queue with a non-empty queue failed" << endl;

  }

  cout << "                                        " << endl;


  cout <<"Testing the PQueue class copy constructor for a normal case: " << endl;
  if(tester.NormalCopyConstructor()){
    

    cout << "Copy Constructor Normal Case passed" << endl;

  } else {

    cout << "Copy Constructor Normal Case failed" << endl;

  }

  cout << "                                        " << endl;


  cout <<"Testing the PQueue class copy constructor for an edge case: " << endl;
  if(tester.EdgeCaseCopyConstructor()){

    cout << "Copy Constructor Edge Case passed" << endl;

  } else {

    cout << "Copy Construcyor Edge Case failed" << endl;

  }

  cout << "                                        " << endl;


  cout << "Testing the PQueue class assignment operator for a normal case: " << endl;
  if(tester.NormalAssignmentOperator()){

    cout << "Assignment Operator Normal Case passed" << endl;

  } else {

    cout << "Assignment Operator Normal Case failed" << endl;

  }

  cout << "                                        " << endl;


  cout << "Testing the PQueue class assignment operator for an edge case: " << endl;
  if(tester.AssignmentOperatorEdgeCase()){

    cout << "Assignment Operator Edge Case passed" << endl;

  } else {

    cout << "Assignment Operator Edge Case failed" << endl;

  }

  cout << "                                        " << endl;


  cout << "Testing that attempting to dequeue an empty queue throws an out_of_range exception: " << endl;
  if(tester.dequeueEmptyQueue()){

    cout << "Dequeue empty queue test passed" << endl;

  } else {

    cout <<"Dequeue empty queue test failed" << endl;

  }

  cout << "                                        " << endl;


  cout << "Testing that attempting to merge queues with different priority functions throws a domain_error exception: " << endl;
  if(tester.mergeWithDifferentPriorityFns()){

    cout << "Merge with different priority functions test passed" << endl;

  } else {

    cout << "Merge with different priority functions test failed" << endl;

  }
    
  return 0;

}


