#include "BST.h"
#define Balanced(x) (stature( (x).lChild ) == stature( (x).rChild )) //完全平衡
#define BalFac(x) ( stature((x).lChild) - stature((x).rChild) )	//平衡因子
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))	//AVL平衡条件
#define tallerChild(x) (stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild : (stature((x)->rChild) > stature((x)->lChild) ? (x)->rChild : (IsLChild(*(x)) ? (x)->lChild : (x)->rChild)))



template <typename T> class AVL:public BST<T> {
	public  : 
		BinNodePosi(T) & insert(const T &e);	//重写插入
		bool remove(const T & e);	//重写删除
		
		//测试
		void test_insert(int count);
		void test_remove(int count);
};


template <typename T> BinNodePosi(T) & AVL<T>::insert(const T & e){ //AVL插入节点算法

	BinNodePosi(T) &x = this->search(e);	//定位e待插入的合适位置
	if(x) return x;	//不存在
	x = new BinNode<T>(e, this->_hot);	this->_size++;	BinNodePosi(T) xx = x;	//不存在就创建x
	
	//根据平衡因子调整平衡
	for(BinNodePosi(T) g = x->parent; g; g = g->parent){
		if( !AvlBalanced(*g) ){	//节点失去平衡, 需要重新恢复平衡, 只需要恢复最低祖先, 其余祖先会自动平衡
			
			FromParentTo(*g) = this->rotateAt(tallerChild(tallerChild(g))); //将诠子树联至原父亲
			
			break;
		}else{	//节点平衡不变, 高度可能变化
			this->updateHeight(g);
		}
	}
	
	return xx;
}



template <typename T> bool AVL<T>::remove(const T & e){ //AVL节点删除算法

	BinNodePosi(T) &x = this->search(e);	//定位e待插入的合适位置
	if(!x) return false;	//存在
	this->removeAt(x,this->_hot);	this->_size--;	//BST节点删除
	
	//根据平衡因子调整平衡
	//由于删除导致的失衡会向上传播, 最高会需要做O(logn)次调整
	for(BinNodePosi(T) g = this->_hot; g; g = g->parent){
		if( !AvlBalanced(*g) ){	//节点失去平衡, 需要重新恢复平衡
			
			g = FromParentTo(*g) = this->rotateAt(tallerChild(tallerChild(g)));
			
			break;
		}
		//更新高度
		this->updateHeight(g);	
	}
	
	return true;
}

template <typename T> void AVL<T>::test_insert(int count){
	for(int i = 1; i <= count; i++){
		 insert(i);
	}
	//~ cout << "树高 : " << this->_root->height << " 根节点 : " << this->_root->data << endl;
}


template <typename T> void AVL<T>::test_remove(int count){
	for(int i = 1; i <= count; i++){
		 remove(i);
	}
}

