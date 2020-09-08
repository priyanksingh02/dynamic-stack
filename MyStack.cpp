#include "MyStack.h"

CMyStack::CMyStack( int nBSize )
{
	m_nBlockSize = nBSize;
	m_pContainer = new unsigned long [nBSize];
	//initialize container with NULL
	memset( m_pContainer,0,sizeof(unsigned long)*nBSize );
	m_pStr1to1Map = new bool [nBSize];
	memset( m_pStr1to1Map, false, nBSize );
	m_nTop = -1;
	m_nActualSize = nBSize;
	m_nSizeInBytes = sizeof(unsigned long)*nBSize;
	
	m_pszErrorMsgBuffer = new char [BUFF_SIZE];
	strcpy(m_pszErrorMsgBuffer,"\0");
}

CMyStack::CMyStack(const CMyStack& obj) :m_nBlockSize(obj.m_nBlockSize)
{
	m_nActualSize = obj.m_nActualSize;
	m_pContainer = new unsigned long[m_nActualSize];
	m_pStr1to1Map = new bool[m_nActualSize];
	m_nTop = obj.m_nTop;
	m_nSizeInBytes = obj.m_nSizeInBytes;
	m_pszErrorMsgBuffer = new char [BUFF_SIZE];
	strcpy(m_pszErrorMsgBuffer,obj.m_pszErrorMsgBuffer);
	//Copying Contents from source to destination
	//assigning new memory and copying the content to get duplicates
	for (int i = 0; i <= m_nTop; i++){
		if (obj.m_pStr1to1Map[i]){
			m_pStr1to1Map[i] = true;
			char * pszSource = reinterpret_cast<char *>(obj.m_pContainer[i]);
			int nLength = strlen(pszSource);
			char * pszDest = new char[nLength + 1];
			strcpy(pszDest, pszSource);
			unsigned long lAddr = reinterpret_cast<unsigned long>(pszDest);
			m_pContainer[i] = lAddr;
		}
		else{
			m_pStr1to1Map[i] = false;
			int * pnSource = reinterpret_cast<int*>(obj.m_pContainer[i]);
			int * pnDest = new int(*pnSource);
			unsigned long lAddr = reinterpret_cast<unsigned long>(pnDest);
			m_pContainer[i] = lAddr;
		}
	}
}

void CMyStack::operator=(const CMyStack& obj)
{
	//delete current contents
	for (int i = 0; i <= m_nTop; i++){
		if (m_pStr1to1Map[i]){
			delete[] reinterpret_cast<char *>(m_pContainer[i]);
		}
		else{
			delete reinterpret_cast<int *>(m_pContainer[i]);
		}
	}
	delete[] m_pStr1to1Map;
	delete[] m_pContainer;

	//reinitialise memory and copy content
	//Copying Contents from source to destination
	//assigning new memory and copying the content to get duplicates
	m_nActualSize = obj.m_nActualSize;
	m_pContainer = new unsigned long[m_nActualSize];
	m_pStr1to1Map = new bool[m_nActualSize];
	m_nTop = obj.m_nTop;
	m_nSizeInBytes = obj.m_nSizeInBytes;
	m_pszErrorMsgBuffer = obj.m_pszErrorMsgBuffer;
	for (int i = 0; i <= m_nTop; i++){
		if (obj.m_pStr1to1Map[i]){
			m_pStr1to1Map[i] = true;
			char * pszSource = reinterpret_cast<char *>(obj.m_pContainer[i]);
			int nLength = strlen(pszSource);
			char * pszDest = new char[nLength + 1];
			strcpy(pszDest, pszSource);
			unsigned long lAddr = reinterpret_cast<unsigned long>(pszDest);
			m_pContainer[i] = lAddr;
		}
		else{
			m_pStr1to1Map[i] = false;
			int * pnSource = reinterpret_cast<int*>(obj.m_pContainer[i]);
			int * pnDest = new int(*pnSource);
			unsigned long lAddr = reinterpret_cast<unsigned long>(pnDest);
			m_pContainer[i] = lAddr;
		}
	}
}

CMyStack::~CMyStack()
{
	//eliminate memory leak
	for (int i = 0; i <=m_nTop; i++){
		if (m_pStr1to1Map[i]){
			delete[] reinterpret_cast<char *>(m_pContainer[i]);
		}
		else{
			delete reinterpret_cast<int *>(m_pContainer[i]);
		}
	}
	delete[] m_pStr1to1Map;
	delete[] m_pContainer;
	delete[] m_pszErrorMsgBuffer;
}
//Push for String Data Input
void CMyStack::push( char * pszStr )
{
	int nLength = strlen(pszStr);
	char * szNewStr = new char [nLength+1];
	strcpy(szNewStr,pszStr);

	//modify stack size counter
	m_nSizeInBytes += sizeof(char)*(nLength + 1);

	//convert address to a unsigned long number
	unsigned long lAddr = reinterpret_cast<unsigned long>(szNewStr);
	push(lAddr,true);
}

//Push for Interger Data Input
void CMyStack::push( int nValue )
{
	int *p = new int(nValue);
	unsigned long lAddr = reinterpret_cast<unsigned long>(p);

	//modify stack size counter
	m_nSizeInBytes += sizeof(int);

	push(lAddr,false);
}

//Push Helper Function
void CMyStack::push( unsigned long lValue, bool bType )
{
	if(m_nTop+1==m_nActualSize){
		incContainerSize();
	}
	m_pContainer[++m_nTop] = lValue;
	m_pStr1to1Map[m_nTop] = bType;
}



void CMyStack::pop()
{
	if(isEmpty()){
		strcpy(m_pszErrorMsgBuffer,"Pop Failed! Stack is empty.");
		throw underflow_error(m_pszErrorMsgBuffer);
	}
	//pop and delete data from heap
	if (m_pStr1to1Map[m_nTop]){
		char * str = reinterpret_cast<char*>(m_pContainer[m_nTop]);
		//update size counter
		m_nSizeInBytes -= sizeof(char)*(strlen(str) + 1);
		delete[] str;
	}
	else{
		int * p = reinterpret_cast<int*>(m_pContainer[m_nTop]);
		//update size counter
		m_nSizeInBytes -= sizeof(int);
		delete p;
	}
	m_nTop --;

	//reduce the size if free space is twice the size of blocksize
	//2*blocksize is selected for better performance
	if((m_nActualSize-1-m_nTop)>=2*m_nBlockSize){
		decContainerSize();
	}
}

void CMyStack::incContainerSize()
{
	//reallocate to a bigger size buffer
	unsigned long * pNewContainer = new unsigned long [m_nActualSize+m_nBlockSize];
	bool* pNewStrMap = new bool[m_nActualSize + m_nBlockSize];
	for (int i = 0 ; i <=m_nTop ; i++) {
		pNewContainer[i] = m_pContainer[i];
		pNewStrMap[i] = m_pStr1to1Map[i];
	}

	m_nActualSize += m_nBlockSize;
	delete[] m_pContainer;
	delete[] m_pStr1to1Map;
	m_pContainer = pNewContainer;
	m_pStr1to1Map = pNewStrMap;

	//adjusting size counter
	m_nSizeInBytes += sizeof(unsigned long)*m_nBlockSize;
}

void CMyStack::decContainerSize()
{
	//reduce size of container
	unsigned long* pNewContainer = new unsigned long [m_nActualSize - m_nBlockSize];
	bool * pNewStrMap = new bool[m_nActualSize - m_nBlockSize];
	for(int i =0; i <=m_nTop; i++){
		pNewContainer[i] = m_pContainer[i];
		pNewStrMap[i] = m_pStr1to1Map[i];
	}
	
	m_nActualSize -= m_nBlockSize;
	delete[] m_pContainer;
	delete[] m_pStr1to1Map;
	m_pContainer = pNewContainer;
	m_pStr1to1Map = pNewStrMap;

	//adjusting size counter
	m_nSizeInBytes -= sizeof(unsigned long)*m_nBlockSize;
}

int CMyStack::getInt(unsigned long lAddr)
{
	int * p = reinterpret_cast<int*>(lAddr);
	return *p;
}

char * CMyStack::getStr(unsigned long lAddr)
{
	char * p = reinterpret_cast<char*>(lAddr);
	return p;
}

int CMyStack::getElementCount()
{
	return m_nTop+1;
}

int CMyStack::getSize()
{
	return m_nSizeInBytes;
}

int CMyStack::getContainerSize()
{
	return m_nActualSize;
}

bool CMyStack::isEmpty()
{
	if(m_nTop == -1)
		return true;
	else
		return false;
}

char * CMyStack::getErrorMsg()
{
	return m_pszErrorMsgBuffer;
}

void CMyStack::setErrorMsg(const char * pszMsg)
{
	strcpy(m_pszErrorMsgBuffer, pszMsg);
}

unsigned long * CMyStack::pBegin(){
	return m_pContainer;
}

unsigned long * CMyStack::pEnd(){
	return (m_pContainer + m_nActualSize);
}

unsigned long * CMyStack::pTop(){
	if (isEmpty())
		return NULL;
	else
		return (m_pContainer + m_nTop);
}

bool * CMyStack::pTypeBegin(){
	return m_pStr1to1Map;
}

bool * CMyStack::pTypeEnd(){
	return (m_pStr1to1Map + m_nActualSize);
}

bool * CMyStack::pTypeTop() {
	if (isEmpty())
		return NULL;
	else
		return (m_pStr1to1Map + m_nTop);
}