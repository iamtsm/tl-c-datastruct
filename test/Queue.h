#include <iostream>
using namespace std;
#include "List.h"

template <typename T> class Queue: public List<char>{	//队列模板类
	
	public :
		void enqueue(T const& e){insertAsLast(e);} //插入
		
		T dequeue(){return remove(first());} //弹出
		
		T & front(){return first()->data;} //返回头元素
};



template <typename T> class Queue_example {
	
	protected : 
		Queue<T> Q;
	
	public : 
		void init(char *e,int len);
		void show();
		Queue<T> & getQueue(){return Q;}
};

template <typename T> void Queue_example<T>::init(char *e,int len){
	for (int i = 0; i < len; Q.enqueue(e[i++]));
}

template <typename T> void Queue_example<T>::show(){
	while(!Q.empty()){
		cout << Q.dequeue();
	}
}



