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
