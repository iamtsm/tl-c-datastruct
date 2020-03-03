#include "Vector.h"
#include "BinNode.h"

#define BTNodePosi(T) BTNode<T>*

template <typename T> struct BTNode {

	BTNodePosi(T) parent;	//指向父亲
	Vector<T> key;	//存放关键码
	Vector<BTNodePosi(T)> child;	//指向孩子	_size = key._size+1

	//构造一个节点的B树
	BTNode(){
		parent = NULL;
		child.insert(0, NULL);
	}
	//构造三个节点的B树
	BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL){
		parent = NULL;
		key.insert(0, e);
		child.insert(0,lc);	child.insert(1, rc);	//初始化两个孩子
		if(lc){
			lc->parent = this;
		} 	
		if(rc) {
			rc->parent = this;
		}
	}
};

