#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

//Memory leak detection using CRT library -> taken from msdn website
#ifdef _DEBUG  
#ifndef DBG_NEW  
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )  
#define new DBG_NEW  
#endif  
#endif  // _DEBUG  

//#include "header.h"
#include "MyStack.h"
#define MAX_BLOCK_SIZE 100

//User defined fuctions
void displayAll(CMyStack &st);
void displayTop(CMyStack &st);
int safe_cin_int();

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	int nBSize;
	bool flag_loop = true;
	cout << "Dynamic Stack Initialising...\n";
	do{
	cout << "Enter Block Size ( Max = "<<MAX_BLOCK_SIZE << " ) = ";
	nBSize = safe_cin_int();
	}while(nBSize<=0||nBSize > MAX_BLOCK_SIZE);
	
	cout << "Creating Stack..." << endl;
	CMyStack st(nBSize);
	cout << "Stack Successfully created!" << endl;
	system("pause");
	system("cls");
	
	int nChoice,nSubChoice,nValue;
	char szString[BUFF_SIZE];
	while(flag_loop){
		cout << "\n::::MAIN MENU::::\n"
			<< "1.Insert a new value\n"
			<< "2.Show top of the stack\n"
			<< "3.Display all values\n"
			<< "4.Pop\n"
			<< "5.Get Previous Error Message\n"
			<< "6.Exit\n";
		cout << "Enter Choice : ";
		//cin>> nChoice;
		nChoice = safe_cin_int();
		switch(nChoice){
		case 1: //Push Options
			cout << "\n1.Enter INTEGER\n2.Enter STRING\nChoice :";
			nSubChoice = safe_cin_int();
			switch(nSubChoice){
			case 1: //Enter an Int Value
				cout << "Enter Value : ";
				nValue = safe_cin_int();
				try{
					st.push(nValue);
					cout << "Element pushed successfully!" << endl;
				}
				catch (bad_alloc &err){
					cout << "Error : " << err.what() << endl;
					st.setErrorMsg(err.what());
					cout << "Element push failed!" << endl;
				}
				break;
			case 2: //Enter a String
				cout << "Enter String (spaces allowed): ";
				cin.getline(szString,BUFF_SIZE,'\n');
				cin.getline(szString,BUFF_SIZE,'\n');
				//cout << "Entered String : " << szString << endl;
				try{
					st.push(szString);
					cout << "Element pushed successfully!" << endl;
				}
				catch (bad_alloc &err){
					cout << "Error : " << err.what() << endl;
					st.setErrorMsg(err.what());
					cout << "Element push failed!" << endl;
				}
				break;
			default :
				cout << "Incorrect Option.Switching to Main Menu!!!\n";
			}
			break;
		case 2: //Show top of Stack
			displayTop(st);
			break;
		case 3: //Show All
			displayAll(st);
			break;
		case 4:
			try{
				st.pop();
				cout << "Element successfully popped!" << endl;
			}
			catch(underflow_error &err){
				cout << "Error : " << err.what() << endl;
			}
			break;
		case 5:
			cout << "Message =  ";
			if(strcmp(st.getErrorMsg(),"\0") == 0)
				cout <<" Message Buffer empty ";
			else
				cout << st.getErrorMsg(); 
			cout << endl;
			break;
		case 6: 
			flag_loop = false;
			cout << "Exitting..." << endl;
			break;
		default:
			cout << "Wrong Option.Try Again!!!\n";
		}
		cout << endl;
		//print stack information
		cout << "\nElement Count = " << st.getElementCount() << " / " << st.getContainerSize()
			 << " (Total) | " << st.getSize() << " Bytes\n";
		//Skips pausing and screen clear if exit is selected!
		if(nChoice!=6){
			system("pause");
			system("cls");
		}
	}
	return 0;
}

void displayAll(CMyStack &st)
{
	cout << "<<<STACK>>>" << endl;
	if (st.isEmpty()){
		st.setErrorMsg("Empty Stack!");
		cout << "--EMPTY--" << endl;
		return;
	}
	unsigned long * iter = st.pBegin();
	bool * iter_type = st.pTypeBegin();
	for (; iter != st.pTop(); iter++, iter_type++){
		if (*iter_type){
			cout << st.getStr(*iter) << endl;
		}
		else{
			cout << st.getInt(*iter) << endl;
		}
	}
	//display top element
	if (*iter_type){
		cout << st.getStr(*iter);
	}
	else{
		cout << st.getInt(*iter);
	}
	cout << "\t<-TOP\n";
}

void displayTop(CMyStack &st)
{
	if (st.isEmpty()){
		st.setErrorMsg("Stack is Empty!");
		cout << "Stack is Empty!" << endl;
		return;
	}
	unsigned long *iter = st.pTop();
	bool * iter_type = st.pTypeTop();
	if (*iter_type){
		cout << st.getStr(*iter);
	}
	else{
		cout << st.getInt(*iter);
	}
	cout << endl;
}

int safe_cin_int()
{
	int ntemp;
	cin >> ntemp;
	while (cin.fail())
	{
		cin.clear();
		//call ignore to clear extra entries in the buffer till '\n'
		//if '\n' not specified, input is ignore till eof
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please enter a valid Integer : ";
		cin >> ntemp;
	}
	return ntemp;
}