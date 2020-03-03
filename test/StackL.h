#include <iostream>
using namespace std;
#include "List.h"

template <typename T> class Stack: public List<char> {	//栈模板类
	
	public :
		void push(T const &e) {
			insertAsLast(e);
		}
		T pop() {
			return remove(last()); 
		}
		T & top() {
			return last()->data;
		}
};





template <typename T> class Stack_example {
	
	protected : 
		Stack<T> S;
	
	public : 
		void convert(int n, int base);
		void show();
		Stack<T> & getStack(){return S;}
};

template <typename T> void Stack_example<T>::convert(int n, int base){
	 char digit[] = "0123456789ABCDEF";
	 while(0 < n){
		 S.push(digit[n % base]);
		 n /= base;	 
	 }
}

template <typename T> void Stack_example<T>::show(){
	while(!S.empty()){
		cout << S.pop();
	}
}




