#include "Vector.h"
typedef int Rank;

#include <iostream>
using namespace std;

#define Parent(i) ((i - 1) >> 1)		//某个秩的父节点的秩
#define LChild(i) (1 + (i << 1))		//某个秩的左孩子的秩
#define RChild(i) ((1 + i) << 1)		//某个秩的右孩子的秩
#define InHeap(n, i) (( -1 < i ) && ( i < n ) )
#define LChildValid(n, i) InHeap( n, LChild( i ) )
#define RChildValid(n, i) InHeap( n, RChild( i ) )

//~ #define Bigger(PQ, i, j)  ( lt( PQ[i], PQ[j] ) ? j : i )
//~ #define ProperParent(PQ, n, i) /*父子（至多）三者中的大者*/
            //~ ( RChildValid(n, i) ? Bigger( PQ, Bigger( PQ, i, LChild(i) ), RChild(i) ) :
				//~ ( LChildValid(n, i) ? Bigger( PQ, i, LChild(i) ) : i)
            //~ ) //相等时父节点优先，如此可避免不必要的交换


template <typename T> struct PQ{
	
	virtual void insert(T) = 0;		//按照优先级次序插入词条
	
	virtual T getMax() = 0;			//取出优先级最高的词条
	
	virtual T delMax() = 0;			//删除优先级最高的词条
	
};

template <typename T> class PQ_comlHeap : public PQ<T>, public Vector<T>{
	
	protected : 
		Rank percolateDown(Rank n, Rank i);	//下滤
		Rank percalateUp(Rank i);	//上滤
		void heapify(Rank n);	//Floyd建堆算法
		
	public : 
		PQ_comlHeap(){}
		PQ_comlHeap(T *A, Rank n){
			this->copyFrom(A, 0, n);
			heapify(n);
		}
		~PQ_comlHeap(){}
		void insert(T e);
		T getMax();
		T delMax();	
		void show();
};


template <typename T> Rank PQ_comlHeap<T>::percolateDown(Rank n, Rank i){
	
	//~ Rank j = ( this->_elem[i] < this->_elem[LChild(i)] ) ? //先和左孩子比较
			 //~ ( (this->_elem[LChild(i)] > this->_elem[RChild(i)]) ? LChild(i) : RChild(i)) : //左孩子大于父 , 左孩子接着和右孩子对比, 返回较大者
			 //~ ( (this->_elem[i] < this->_elem[RChild(i)]) ? RChild(i) :  LChild(i));	// 左孩子小于父, 右孩子和父对比, 返回较大者
	Rank j;
		
	while( i != j && i < n){
		
			j = ( this->_elem[i] < this->_elem[LChild(i)] ) ? 
			( (this->_elem[LChild(i)] > this->_elem[RChild(i)]) ? LChild(i) : RChild(i)) : 
			( (this->_elem[i] < this->_elem[RChild(i)]) ? RChild(i) :  LChild(i));
	
			//交换位置
			int temp = this->_elem[i];
			this->_elem[i] = this->_elem[j];
			this->_elem[j] = temp;
			i = j;	
	}
	
	return i;
}

template <typename T> Rank PQ_comlHeap<T>::percalateUp(Rank i){
		
	while(0 < i){
		Rank j = Parent(i);
		
		if(this->_elem[i] < this->_elem[j] ){
			break;
		}
		//交换位置
		int temp = this->_elem[i];
		this->_elem[i] = this->_elem[j];
		this->_elem[j] = temp;
		i = j;
	}
	
	return 0;
}


template <typename T> T PQ_comlHeap<T>::getMax(){
	return this->_elem[0];
}

template <typename T> T PQ_comlHeap<T>::delMax(){
	T maxElem = this->_elem[0];	this->_elem[0] = this->_elem[--this->_size];
	
	percolateDown(this->_size, 0);
	
	return maxElem;
}

template <typename T> void PQ_comlHeap<T>::insert(T e){
	Vector<T>::insert(e);	this->percalateUp(this->_size - 1);
}



template <typename T> void PQ_comlHeap<T>::show(){
	cout << "\n";
	for(int i = 0; i < this->_size; i++){
		cout << this->_elem[i] << ",";
	}
	cout << "\n";
}


template <typename T> void PQ_comlHeap<T>::heapify(Rank n){
	for ( int i = n/2 - 1; 0 <= i; i-- ) {
		percolateDown ( n, i ); //下滤各内部节点
	}
      
}
