#pragma once
#pragma warning(disable:4996)
#include "header.h"

#define BUFF_SIZE 1024
#define DEFAULT_BLOCK_SIZE 3

class CMyStack
{

public:

	CMyStack( int nBSize = DEFAULT_BLOCK_SIZE );
	~CMyStack();
	CMyStack(const CMyStack&);
	
	void pop();			//delete the content pointed by top
	void push( char * );//pushes string to the stack
	void push( int );	//pushes int to the stack
	bool isEmpty();		//returns true if stack is empty
	int getElementCount();	//returns number of elements stored in the stack
	int getSize();		//shows size of elements stored in the stack + size of container to hold the elements address
	int getContainerSize(); //returns actual size of container (including empty spaces)
	char * getErrorMsg();	//returns recent error message
	void setErrorMsg(const char *); //updates error message
	void operator = (const CMyStack&); //overloaded '=' operator
	int getInt(unsigned long);	//converts address and returns the Integer number stored at that address
	char* getStr(unsigned long);	//converts address and returns the pointer to null terminated string

	//<Warning : These functions can allow modification of stack values from outside function
	//For Stack Content Iteration
	unsigned long * pBegin();	//address of the beginning of the container
	unsigned long * pEnd();		//address of the location next to the end of the container
	unsigned long * pTop();		//address of the location pointed by top
	bool * pTypeBegin();	//address of the beginning of the type mapping
	bool * pTypeEnd();		//address of the location next to the end of the type mapping
	bool * pTypeTop();		//address of the location pointed by top
private:

	bool *m_pStr1to1Map; // 0 - int, 1 - string : A 1 to 1 mapping to know the type of data stored in the container
	unsigned long * m_pContainer; // used to store data
	int m_nTop;			// points to the most recent data entered in the stack
	int m_nBlockSize;	// block size 
	char * m_pszErrorMsgBuffer; // Error message buffer
	int m_nActualSize;	// size of container including empty spaces
	int m_nSizeInBytes;	// size of container + size of data entered in the stack
	
	void incContainerSize();	// increases stack size if overflow condition is encountered
	void decContainerSize();	// decreases stack size if 2*(block size) is empty ; reduced single block size
	void push( unsigned long, bool );	//helper function to achieve push operation

};

