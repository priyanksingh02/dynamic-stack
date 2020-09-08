#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#include "MyStack.h"
//Memory leak detection using CRT library -> taken from msdn website
#ifdef _DEBUG  
#ifndef DBG_NEW  
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )  
#define new DBG_NEW  
#endif  
#endif  // _DEBUG

int main(){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	CMyStack s(3);
	s.push(100);
	s.push("hello");
	s.showAll();
	
	return 0;
}