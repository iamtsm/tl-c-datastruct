
#include "ListNode_definition.h"

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
	ListNodePosi(T) x = new ListNode(e,succ,this); //创建新节点，并初始化两条指向
	succ->pred = x; succ = x; return x; //更新其他两条指向，插入完成，并返回新插入的节点
}
