
#include <iostream>
#include "Vector.h"

using namespace std;

template <typename T> class Stack: public Vector<char> {	//栈模板类
	
	public :
		void push(T const &e) {
			insertAsLast(e);
		}
		T pop() {
			return remove(size() - 1);
		}
		T & top() {
			return (*this)[size() - 1];
		}
};



template <typename T> class Stack_example {
	
	protected : 
		Stack<T> S;
	
	public : 
		void convert(int n, int base);
		void show();
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



