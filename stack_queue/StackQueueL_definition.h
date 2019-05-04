#include "../list/List_implement.h"

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


template <typename T> class Queue: public List<char>{	//队列模板类
	
	public :
		void enqueue(T const& e){insertAsLast(e);} //插入
		
		T dequeue(){return remove(first());} //弹出
		
		T & front(){return first()->data;} //返回头元素
};
