/*****************************************
** File:    pqueue.cpp
** Project: CMSC 341 Project 3, Fall 2023
** Author:  Jervon Drakes
** Date:    11/14/2023
** E-mail:  jdrakes1@umbc.edu
** 
**
** This file contains the pqueue.cpp file for Project 30.
** This program reads the pqueue.h file and its functions and
** executes those functions for implementation
**
**
**
***********************************************/





// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"

// PQueue(constructor)
// This is the constructor. It must be provided with a pointer to the prioritization function, 
// the type of heap, and the desired data structure.
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
{

  m_heap = nullptr; // initialize the root of the heap to nullptr
  m_size = 0; // size of the heap is 0
  m_priorFunc = priFn; // assign the priority function to the provided function
  m_heapType = heapType; // assign the heap type
  m_structure = structure; // assign the data structure type
  
}

//~PQueue(destructor)
// The destructor deallocates the memory and re-initializes the member variables.
PQueue::~PQueue()
{
  clear(); //call the clear function to deallocate memory and reset member variables
  
}

// clear
// This function clears the queue. It must delete all the nodes in the heap, leaving the heap empty. 
// Moreover, it re-initializes the member variables.
void PQueue::clear() {

  // call the recursive function to deallocate memory for the enitre heap
  helperClear(m_heap);
  m_heap = nullptr;
  m_size = 0;
  
}

// helperClear
// helper function for the clear function
void PQueue::helperClear(Node* newNode){

  if(newNode == nullptr){

    return;

  }

  // recursively clear the left and right subtrees
  helperClear(newNode->m_left);
  helperClear(newNode->m_right);

  // deallocate the node
  delete newNode;

}

// PQueue(copy constructor)
// The copy constructor must make a deep copy of the rhs object. It must function correctly if rhs is empty. 
// This function creates an exact same copy of rhs.
PQueue::PQueue(const PQueue& rhs)
{

  // initialize member variables
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;

  // perform deep copy
  if(rhs.m_heap) {

    m_heap = deepCopy(rhs.m_heap); // make a deep copy of the heap by calling the helper function

    m_size = rhs.m_size;
  
    
  }
}

// deepCopy
// helper function to deep copy the heap recursively
Node* PQueue::deepCopy(Node* nodeRhs){

  if(nodeRhs == nullptr){

    return nullptr; // return nullptr for an empty node

  }

  // create a new node with the patient data from nodeRhs
  Node* node = new Node(nodeRhs->getPatient());
  node->setNPL(nodeRhs->getNPL());


  // recursively copy the left and right subtrees
  node->m_left = deepCopy(nodeRhs->m_left);
  node->m_right = deepCopy(nodeRhs->m_right);

  return node;

}

// Assignment operator
// The assignment operator creates an exact same copy of rhs. You should not call the copy constructor in 
// the implementation of the assignment operator.
PQueue& PQueue::operator=(const PQueue& rhs) {

  // determine self assignment
  if(this == &rhs){

    return *this;

  }

  // clear the object
  clear();

  // initialize member variables
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;

  // make a deep copy of the heap by calling the helper function
  m_heap = deepCopy(rhs.m_heap);
  m_size = rhs.m_size;

  return *this;

}
  
// mergeWithQueue(PQueue& rhs)
// This function merges the host queue with the rhs; it leaves rhs empty. Two heaps can only be merged if they have the same priority function and they are of the same data structure. 
// If the user attempts to merge queues with different priority functions, or two different data structures a domain_error exception should be thrown. This 
// function requires protection against self-merging. Merging a heap with itself is not allowed.
void PQueue::mergeWithQueue(PQueue& rhs) {
  
  // determine if the object is marging with itself
  if(this == &rhs){

    throw domain_error("Merging a queue with itself is not allowed");

  }

  // determine if erging queue with different priority functions is allowed
  if(m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure){

    throw domain_error("Merging queues with different priority functions or data structures is not allowed");

  }

  //merge the two heaps based on ther structure(skew or leftist)
  if(m_structure == SKEW){

    m_heap = skewHeapMerge(m_heap, rhs.m_heap);

  } else {

    m_heap = leftistHeapMerge(m_heap, rhs.m_heap);

  }
  
  m_size += rhs.m_size; // update the size

  // clear rhs heap
  rhs.clear();
}


// insertPatient
// Inserts a patient into the queue. Must maintain the min-heap or the max-heap property depending on the settings. Moreover, if the selected data structure is leftist heap, it needs to maintain 
// a correct value of Null Path Length (NPL) in the node.
void PQueue::insertPatient(const Patient& input) {
    
  // create a new node with the given patient
  Node* nodeNew = new Node(input);

  // compute the priority of the patient using the current priority function
  int patientPriority = m_priorFunc(input);

  // if it's a max-heap, negate the priority value to simulate a min-heap
  if (m_heapType == MAXHEAP) {

    patientPriority = -patientPriority;
  }


  // merge the new node with the existing heap
  if (m_heap == nullptr) {

    // if the heap is empty, simply set the new node as the root
    m_heap = nodeNew;

  } else {

    // Merge the new node with the existing heap
    if (m_structure == SKEW) {

      // if it's a skew heap, use skew heap merge
      m_heap = skewHeapMerge(m_heap, nodeNew);

    } else {

      // if it's a leftist heap, use leftist heap merge
      m_heap = leftistHeapMerge(m_heap, nodeNew);

    }
  }

  // increase the size of the heap
  m_size++;

}

// skewHeapMerge
// helper function for the insertPatient function
Node* PQueue::skewHeapMerge(Node* node1, Node* node2) {
    
  // if node1 is null return node2
  if(node1 == nullptr){

    return node2;

  }

  // if node2 is null return node1
  if (node2 == nullptr) {

    return node1;

  }

  // Ensure min or max heap property based on m_heapType
  if ((m_priorFunc(node1->getPatient()) > m_priorFunc(node2->getPatient())  && m_heapType == MINHEAP) || (m_priorFunc(node1->getPatient()) < m_priorFunc(node2->getPatient()) && m_heapTyp\
e == MAXHEAP)) {
    swap(node1, node2);
  }

      
  // Swap the left and right subtrees of node1
  swap(node1->m_left, node1->m_right);


  // Recursively merge h2 with the left subtree of the new h1
  node1->m_left = skewHeapMerge(node1->m_left, node2);

  return node1;
}

// leftistHeapMerge
// helper function for the insertPatient function
Node* PQueue::leftistHeapMerge(Node* node1, Node* node2) {
    
  // if node1 is null return node2
  if (node1 == nullptr) {
        
    return node2;

  }

  // if node2 is null return node1
  if (node2 == nullptr) {
    
    return node1;
    
  }

  // Ensure min or max heap property based on m_heapType
  if ((m_priorFunc(node1->getPatient()) >= m_priorFunc(node2->getPatient())  && m_heapType == MINHEAP) || (m_priorFunc(node1->getPatient()) <= m_priorFunc(node2->getPatient()) && m_heapType == MAXHEAP)) {
    swap(node1, node2);
  }


  // Merge h2 with the right subtree of node1
  node1->m_right = leftistHeapMerge(node1->m_right, node2);

  // Update the Null Path Length (NPL) of node1
  int nplLeft = (node1->m_left == nullptr) ? -1 : node1->m_left->getNPL();
  int nplRight = (node1->m_right == nullptr) ? -1 : node1->m_right->getNPL();

  // If the right subtree's NPL is greater than the left subtree's NPL, swap them
  if (nplLeft <= nplRight) {
    swap(node1->m_left, node1->m_right);
  }


  

  // Update the NPL of node1
  node1->setNPL((node1->m_right == nullptr) ? 0 : node1->m_right->getNPL() + 1);

  return node1;
}

// numPatients
// It returns the current number of patients in the queue.
int PQueue::numPatients() const
{
  return m_size;
}

// getPriorityFn
// This function returns the current priority function
prifn_t PQueue::getPriorityFn() const {

  return m_priorFunc;
}

// getNextPatient
// This function extracts (removes the node) and returns the highest priority patient from the queue. It must maintain the min-heap or max-heap property. The function throws an 
// out_of_range exception if the queue is empty when the function is called.
Patient PQueue::getNextPatient() {

  if (m_size == 0) {

    throw out_of_range("Queue is empty");
  
  }

  // save the highest priority patient (at the root) to be returned
  Patient patientHighestPriority = m_heap->getPatient();

  // create temporary pointers to the left and right subtrees
  Node *leftSubtree = m_heap->m_left;
  Node *rightSubtree = m_heap->m_right;

  // delete the root node
  delete m_heap;
  m_heap = nullptr;
  
  // decrement the size of the queue
  m_size--;

  // merge the left and right subtrees into a new heap
  if (m_structure == SKEW) {
        
    // for skew heap, you can simply merge the left and right subtrees
    m_heap = skewHeapMerge(leftSubtree, rightSubtree);
    
  } else if (m_structure == LEFTIST) {
        
    // for leftist heap, you need to merge and update the null path length
    m_heap = leftistHeapMerge(leftSubtree, rightSubtree);
    
  }

  return patientHighestPriority;

} 

// setPriorityFn
// This function sets a new priority function and its corresponding heap type (min-heap or max-heap). 
// It must rebuild the heap!
void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {

  //store the new priority function heap type.
  m_priorFunc = priFn;
  m_heapType = heapType;

  // create a temporary array to hold patients
  Patient* arrayPatients = new Patient[m_size];
  int indexCurr = 0;

  // traverse the current heap and store patients in the array
  while (m_size > 0) {

    arrayPatients[indexCurr++] = getNextPatient();
        
  }

  // Clear the old heap.
  clear();

  // reinsert the patients into the heap with the new priority function
  for (int i = 0; i < indexCurr; i++) {
    
    insertPatient(arrayPatients[i]);

  }

  // clean up the temprorary Array
  delete[] arrayPatients;

    
}


// getHeapType
// This function returns the heap type, i.e. it is either MINHEAP or MAXHEAP.
HEAPTYPE PQueue::getHeapType() const{

  return m_heapType;

}

// setStructure
// This function sets the data structure, i.e. it is either SKEW or LEFTIST. It must rebuild a heap with the new structure 
// using the nodes in the current data structure.
void PQueue::setStructure(STRUCTURE structure){

  if (m_structure == structure) {

    // no need to change the structure if it's the same.
    return;
  }

  // update the heap structure to the new one.
  m_structure = structure;

  // create a temporary array to hold the patients.
  Patient* tempArray = new Patient[m_size];
  int indexCurrent = 0;

  // traverse the current heap and store patients in the array.
  // done using a traversal function like preorder or inorder.
  while (m_size > 0) {

    tempArray[indexCurrent++] = getNextPatient();

  }

  // clear the old heap.
  clear();

  // reinsert the patients into the heap with the new structure.
  for (int i = 0; i < indexCurrent; i++) {

    insertPatient(tempArray[i]);

  }

  // clean up the temporary array.
  delete[] tempArray;

}


// getStructure
// This function returns the data structure of the type STRUCTURE, i.e. it is either SKEW or LEFTIST.
STRUCTURE PQueue::getStructure() const {

  return m_structure;
}

// printPatientQueue
// It prints the contents of the queue using preorder traversal. Although the first patient printed should have the highest priority, the remaining patients will 
// not necessarily be in priority order
void PQueue::printPatientQueue() const {

  cout << "Contents of the queue:" << endl;

  // call the helper function for pre-order traversal starting from the root 
  traversalPreOrder(m_heap);
}

// traversalPreOrder
// helper function for the printPatientQueue
void PQueue::traversalPreOrder(Node* node) const{

  // determine if the current node is not nukk
  if(node != nullptr){

    // output the priority value and patient information of the current node
    cout << "[" << m_priorFunc(node->m_patient) << "]" << node->m_patient << endl;
    
    // recursively traverse the left subtree
    traversalPreOrder(node->m_left);

    // recursively traverse the rught subtree
    traversalPreOrder(node->m_right);

  }

}

void PQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}

void PQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
    else
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
  sout  << patient.getPatient() 
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}
