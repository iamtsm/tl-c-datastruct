#include <iostream>


#define ListNodePosi(T) ListNode<T>* //ListNode类型改个名字而已
template <typename T> struct ListNode{	//列表节点结构体
	T data;	//数值
	ListNodePosi(T) pred; //前驱
	ListNodePosi(T) succ; //后继
	ListNode(){}
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		:data(e),pred(p),succ(s){} //默认构造器
	ListNodePosi(T) insertAsPred(T const &e); //前插入算法    其他插入都是以这个为基础
	ListNodePosi(T) insertAsSucc(T const &e); //后插入算法
};



/**
 * 这里可以这样理解 : 新插入一个节点时，三个节点会有四条指向，初始化构造函数会更新两条指向，还有两条需要人为更新
 * 这里结构体中的构造函数有对当前结构体元素初始化，也就是新插入的节点会被初始化指向前后节点
 * 所以只需更新前一个节点的一个指向 和 后一个节点的一个指向即可
 * */

template <typename T> ListNodePosi(T) ListNode<T>::insertAsPred(T const &e){ //前插入
	ListNodePosi(T) x = new ListNode(e,pred,this);	//创建新节点，并初始化两条指向
	pred->succ = x; pred = x; return x;  //更新其他两条指向，插入完成，并返回新插入的节点
}

template <typename T> ListNodePosi(T) ListNode<T>::insertAsSucc(T const &e){ //后插入
	ListNodePosi(T) x = new ListNode(e,this,succ); //创建新节点，并初始化两条指向
	succ->pred = x; succ = x; return x; //更新其他两条指向，插入完成，并返回新插入的节点
}
