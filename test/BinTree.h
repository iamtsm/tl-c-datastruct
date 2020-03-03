#include <iostream>

#include "BinNode.h"


template <typename T> class BinTree{	//二叉树模板类
	
	protected : 
		int _size; //规模
		BinNodePosi(T) _root;	//根节点
		virtual int updateHeight(BinNodePosi(T) x); //更新节点x高度
		void updateHeightAbove(BinNodePosi(T) x);	//更新x和其祖先高度
		
	public : 
	
		void init(int h);	//初始化
		int size() const {return _size;} //规模
		bool empty() const {return !_root;} //判空
		BinNodePosi(T) root() const {return _root;} //返回树根
		
		BinNodePosi(T) insertAsRoot ( T const& e ); //插入根节点
		BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e); //插入右孩子节点
		BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e); //插入左孩子节点
		BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &S ); //T作为x左子树接入
		BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &S ); //T作为x右子树接入
		
		int remove ( BinNodePosi(T) x ); //删除以位置x处节点为根的子树，返回该子树原先的规模
		int removeAt(BinNodePosi(T) x ); //递归删除数据域和指针空间
		BinTree<T>* secede ( BinNodePosi(T) x ); //将子树x从当前树中摘除，并将其转换为一棵独立子树
		
		BinTree():_size(0),_root(NULL){}	//构造函数
		~BinTree(){
			if ( 0 < _size ) {
				//~ cout << " size : " << _size; 
				//~ remove ( _root );
			}
		} //析构函数
	
	
		//test
		BinNodePosi(T) genRandomTree(BinNodePosi(T) x,int h);  //生成测试用的深度为h的树
};



template <typename T> int BinTree<T>::updateHeight(BinNodePosi(T) x){
	return x->height = 1 + max(stature(x->lChild),stature(x->rChild)); //节点高度更新为(左右孩子中的最大高度+1)
}

template <typename T> void BinTree<T>::updateHeightAbove(BinNodePosi(T) x){
	while(x){
		updateHeight(x); x = x->parent; //遍历父节点
	} 
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot ( T const& e ){
	_size = 1;
	return _root = new BinNode<T>(e);
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const &e){
	_size++; //更新树规模大小	
	x->insertAsLC(e); //插入新节点	
	updateHeightAbove(x); //更新当前节点和其父代节点
	return x->lChild; //返回新插入的节点位置 (当前节点的左孩子)
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const &e){
	_size++; //更新树规模大小	
	x->insertAsRC(e); //插入新节点	
	updateHeightAbove(x); //更新当前节点和其父代节点
	return x->rChild; //返回新插入的节点位置 (当前节点的右孩子)
}

template <typename T> int BinTree<T>::remove ( BinNodePosi(T) x ){
	cout << "执行删除";
	
	FromParentTo(*x) = NULL;
	
	updateHeightAbove(x); //更新
	int n = removeAt(x); //删除x为根节点的子树
	_size -= n; //更新_size规模，也就是当前树规模
	return n;
}

template <typename T> int BinTree<T>::removeAt(BinNodePosi(T) x){
	if(!x) return 0; //递归基
	int n = 1 + removeAt(x->lChild) + removeAt(x->rChild);  //递归删除，1是加上根节点
	delete x;
	return n;
}

template <typename T> BinTree<T>* BinTree<T>::secede ( BinNodePosi(T) x ){
	if(!x) return NULL;
	
	FromParentTo(*x) = NULL; //分离祖先指向子树
	updateHeightAbove(x->parent);	//更新主树高度
	x->parent = NULL; //分离子树指向祖先
	
	BinTree<T> *S = new BinTree<T>(x->_size,x); 	//构造子树
	
	_size -= S->_size;	//更新主树大小
	
	return S;
}


template <typename T> BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S){ //作为左子树接入
	S->_root->parent = x;  //S子树的根节点指向主树x
	x->lChild = S->_root;  //主树x的左孩子节点指向S子树根节点
	
	_size += S->_size;
	updateHeightAbove(x);
	S->_root = NULL;	S->_size = 0; //remove(S); 
	S = NULL; 
	return x;	//返回接入节点
}


template <typename T> BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S){ //作为右子树接入
	S->root->parent = x;		//S子树的根节点指向主树x
	x->rChild = S->_root;		//主树x的右孩子节点指向S子树根节点
	
	_size += S->_size;
	updateHeightAbove(x);
	S->_root = NULL;	S->_size = 0;	//remove(S);	
	S = NULL;
	return x;	//返回接入节点
}

int factorial( int n ){ 
	int f = 1; 
	while ( n > 1 ) f *= n--;
    return f; 
} //阶乘运算（迭代版）


/**
template <typename T> BinNodePosi(T) BinTree<T>::genRandomTree(BinNodePosi(T) x,int h){ //生成测试用的深度为h的树
	Queue<BinNodePosi(T)> Q;
	int i = 0;
	
	h = factorial(h+1) - 1;	//最终需要生成的节点数,每次dequeue()一个节点
	
	while(0 < h--){
		Q.enqueue(x->insertAsLC(i++));
		Q.enqueue(x->insertAsRC(i++));
		x = Q.dequeue();
	}
}
* */
