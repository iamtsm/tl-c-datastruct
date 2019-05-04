#include "../vector/Vector_implement.h"


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

template <typename T> class Queue: public Vector<char>{	//队列模板类
	
	public :
		void enqueue(T const& e){
			insertAsLast(e);
		} //插入
		
		T dequeue(){
			return remove(0);
		} //弹出
		
		T & front(){
			return (*this)[0];
		} //返回头元素
};
