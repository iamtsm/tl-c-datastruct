#include "BinTree.h"


/**
 * BinTree派生,  二叉搜索树
 * */
template <typename T> class BST : public BinTree<int>{
	protected : 
		BinNodePosi(T) _hot;	//命中节点的父亲
		BinNodePosi(T) connect34(	//3+4重构
			BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
			BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),BinNodePosi(T)
		);
		BinNodePosi(T) rotateAt(BinNodePosi(T));	//旋转调整

	public : 
		virtual BinNodePosi(T) & search(const T &e);	//查找
		virtual BinNodePosi(T) & insert(const T &e);	//插入
		virtual bool remove(const T &e);	//删除
		
		/**
		 * 查找某个树根下的某个值,  递归
		 * */
		BinNodePosi(T) & searchIn(BinNodePosi(T) &v, const T &e, BinNodePosi(T) &hot);
		/**
		 * 删除某个元素，　这里需要分两种情况讨论
		 * */
		BinNodePosi(T) removeAt(BinNodePosi(T) &x , BinNodePosi(T) &hot);
};

/**
 * 在树中查找某个值
 * */
template <typename T> BinNodePosi(T) & BST<T>::search(const T &e){
	return searchIn(this->_root, e , this-> _hot = NULL);	//从根节点查找
}
template <typename T> BinNodePosi(T)& BST<T>::searchIn(BinNodePosi(T) &v, const T &e, BinNodePosi(T) &hot){
	if(!v || e == v->data) return v;	//递归基，成功匹配，或者直到节点为NULL时达到递归基
	hot = v;	//约定　hot存放的是查找到的节点的父节点
	//尾递归  如果待查找元素小于当前节点，递归到左子树，否则递归到右子树
	return searchIn(((e < v->data) ? v = v->lChild : v = v->rChild), e, hot );
}


/**
 * 在树中插入一个元素,  这里不考虑雷同元素的存在,  假设每个元素都是互异的
 * */
template <typename T> BinNodePosi(T) & BST<T>::insert(const T &e){
	
	BinNodePosi(T) &x = search(e);	//拿到命中节点的父节点, 这个是拿到引用
	
	if(!x){ //e不存在, 更新的_hot节点就是带插入节点的父节点
		x = new BinNode<T>(e, _hot);	//将e封装成一个节点，_hot为它的父节点
		_size++;	updateHeightAbove(x);	//更新树节点大小，以及x树及其祖先高度
	}
	return x;
}


/**
 * 在树中删除一个元素,  
 * */
template <typename T> bool BST<T>::remove(const T &e){
	
	BinNodePosi(T) &x = search(e);	//定位目标节点, 返回的是其父节点
	
	if(!x) return false;	//如果这个节点不存在，可直接返回
	
	removeAt(x, _hot);	//删除节点, 此时待删除节点必定存在
	
	_size--;
	updateHeightAbove(_hot);	//更新节点_hot和其祖先高度
	return true;
}

/**
 * 分两种情况，一种是待删除节点只有一个孩子节点，　可以直接用孩子节点覆盖待删除节点
 * 不存在两个孩子都没有的情况。待删除节点存在才会调用removeAt, 所以返回的hot必定有一个孩子＝待删除节点
 * 第二种是待删除节点有左右两个孩子，我们需要交换待删除节点和其直接后继节点，于是情况退化成第一种, 可以直接用孩子节点覆盖待删除节点
 * */
template <typename T> BinNodePosi(T) BST<T>::removeAt(BinNodePosi(T) &x , BinNodePosi(T) &hot){
	BinNodePosi(T) w = x;	//用与操作实际删除的节点, 第二种情况需要用上
	BinNodePosi(T) succ = NULL;	//待删除节点的孩子节点, 第一种情况需要用上
	/**
	 * 情况一 : 只有单个孩子节点
	 * */
	if(!HasLChild(*x)) succ = x = x->rChild;	//假如没有左孩子，直接用右孩子覆盖待删除节点
	else if(!HasRChild(*x)) succ = x = x->lChild;	//假如没有右孩子，直接用左孩子覆盖待删除节点
	/**
	 * 情况二 : 有两个孩子节点
	 * */
	else{
		w = w->succ();	//获取待删除节点x的后继 
		swap(x->data, w->data);	//交换数据(代表交换了节点)
		/**
		 * 情况退化成第一种, 只有单个孩子节点了
		 * */
		 BinNodePosi(T) u = w->parent;
		 (u == x ? u->rChild : u->lChild) = succ = w->rChild;
	}
	hot = w->parent;	//记录实际被删除节点的父亲
	if(succ) succ->parent = hot;
	
	//w->data = NULL; w = NULL;	//删除节点
	
	return succ;
}


template <typename T> BinNodePosi(T) BST<T>::rotateAt ( BinNodePosi(T) v ) { //v为非空孙辈节点
    BinNodePosi(T) p = v->parent; BinNodePosi(T) g = p->parent; //视v、p和g相对位置分四种情况
    if ( IsLChild ( *p ) ) /* zig */{
	   if ( IsLChild ( *v ) ) { /* zig-zig */
          p->parent = g->parent; //向上联接
          return connect34 ( v, p, g, v->lChild, v->rChild, p->rChild, g->rChild );
       } else { /* zig-zag */
          v->parent = g->parent; //向上联接
          return connect34 ( p, v, g, p->lChild, v->lChild, v->rChild, g->rChild );
       }	
	}
    else  /* zag */{
	   if ( IsRChild ( *v ) ) { /* zag-zag */
          p->parent = g->parent; //向上联接
          return connect34 ( g, p, v, g->lChild, p->lChild, v->lChild, v->rChild );
       } else { /* zag-zig */
          v->parent = g->parent; //向上联接
          return connect34 ( g, v, p, g->lChild, v->lChild, v->rChild, p->rChild );
       }	
	}
}


/**
 * 总体思想是依靠中序遍历的不变性
 * */
template <typename T> BinNodePosi(T) BST<T>::connect34 (
    BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,	//节点
    BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3 //局部子树
 ) {
    a->lChild = T0; if ( T0 ) T0->parent = a;
    a->rChild = T1; if ( T1 ) T1->parent = a; updateHeight ( a );
    c->lChild = T2; if ( T2 ) T2->parent = c;
    c->rChild = T3; if ( T3 ) T3->parent = c; updateHeight ( c );
    b->lChild = a; a->parent = b;
    b->rChild = c; c->parent = b; updateHeight ( b );
    return b; //该子树新的根节点
}
