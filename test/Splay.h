#include "BST.h"

template <typename T> class Splay : public BST<T>{
	
	protected : 
		BinNodePosi(T) splay(BinNodePosi(T) v);
		
	public : 
		BinNodePosi(T) & search(const T &e);	//重写查找
		BinNodePosi(T) insert(const T &e);	//重写插入
		bool remove(const T & e);	//重写删除
	
	
};

template <typename T> inline void attachAsLChild(BinNodePosi(T) p, BinNodePosi(T) lc){
	p->lChild = lc;
	if(lc) {
		lc->parent = p;
	}
}

template <typename T> inline void attachAsRChild(BinNodePosi(T) p, BinNodePosi(T) rc){
	p->rChild = rc;
	if(rc) {
		rc->parent = p;
	}
}

template <typename T> BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v){
	
	if(!v) return NULL;
	
	BinNodePosi(T) p; BinNodePosi(T) g;	//需要旋转的节点是v节点的父亲和祖父节点

	while( (p = v->parent) && (g = p->parent) ){
		BinNodePosi(T) gg = g->parent;
		if(IsLChild(*v)){	//如果v节点是左孩子
			if(IsLChild(*p)){ //如果p节点是左孩子	zig-zig
				attachAsLChild(g, p->rChild);	//(可以改成2+3重构)
				attachAsLChild(p, v->rChild);
				attachAsRChild(p, g);
				attachAsRChild(v, p);
			}else{	//如果p节点是右孩子 zig-zag
				attachAsLChild(p, v->rChild); 
				attachAsRChild(g, v->lChild);
				attachAsLChild(v, g); 
				attachAsRChild(v, p);
			}
		}else if (IsRChild(*p)) { //zag-zag
			attachAsRChild(g, p->lChild); 
			attachAsRChild(p, v->lChild);
			attachAsLChild(p, g); 
			attachAsLChild(v, p);
		} else { //zag-zig
			attachAsRChild(p, v->lChild); 
			attachAsLChild(g, v->rChild);
			attachAsRChild(v, g); 
			attachAsLChild(v, p);
		}
		
		if (!gg) v->parent = NULL; //若*v原先癿曾祖父*gg丌存在,则*v现在应为树根
		
		else //否则,*gg此后应诠以*v作为左戒右孩子
		(g == gg->lChild) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);

		updateHeight(g); updateHeight(p); updateHeight(v);
	}
	
    if ( p = v->parent ) { //若p果真非空，则额外再做一次单旋
       if ( IsLChild ( *v ) ) { 
		   attachAsLChild ( p, v->rc );
		   attachAsRChild ( v, p ); 
	   }else{ 
		   attachAsRChild ( p, v->lc ); 
		   attachAsLChild ( v, p ); 
	   }
       updateHeight ( p ); updateHeight ( v );
    }
    v->parent = NULL; return v;
}


template <typename T> BinNodePosi(T) & Splay<T>::search (const T &e){
	
	BinNodePosi(T) p = searchIn(this._root, e, this._hot = NULL); //BST接口查找到合适节点, 成功->返回该节点, 失败->返回其父节点
	
	this._root = splay(p ? p : this._hot);	//最后被访问的节点需要伸展到树根
	
	return this. _root;	//查找始终返回根节点
}


template <typename T> BinNodePosi(T) Splay<T>::insert(const T & e){
	
	if(!this._root){
		this._size++; 
		return new BinNode<T>(e);	//处理树退化空树的情况
	}
	
	if(e == search(e)->data) {	//节点不存在
		return this._root;
	}
	
	this._size++;	BinNodePosi(T) t = this._root;
	
	if(this._root->data < e){		//当前根节点需要作为e节点的左孩子,因为当前根节点数据域小于待插入节点
		t->parent = new BinNode<T>(e,NULL,t,t->rChild);	//以e为数据域新建一个节点 t为左孩子,t的右孩子作为新节点的右孩子
		if(HasRChild(*t)){	//如果有右孩子，将右孩子分离
			t->rChild->parent = this._root;
			t->rChild = NULL;
		}
	}else{	//
		t->parent = new BinNode<T>(e,NULL,t->lChild, t);  //以e为数据域, t的左孩子为新节点的左孩子,t为右孩子 
		if(HasLChild(*t)){	//如果有左孩子, 将左孩子分离
			t->lChild->parent = this._root;
			t->rChild = NULL;
		}
	}
	
	updateHeightAbove(t);	//更新t和其祖先高度
	
	return this._root;	//返回树根，新节点
}


template <typename T> bool Splay<T>::remove(const T & e){
	if(!this._root || (e != search(e)->data)) { //树空，或者e不存在
		return false;
	}
	BinNodePosi(T) w = this._root;
	
	if(!HasLChild(*this._root)){ //没有左孩子
		this._root = this._root->rChild;
		if(this._root){
			this._root->parent = NULL;
		}
	}else if(!HasRChild(*this._root)){	//没有右孩子
		this._root = this._root->lChild;
		if(this._root){
			this._root->parent = NULL;
		}
	}else{ //左右孩子都有
		BinNodePosi(T) lTree = this._root->lChild;	//切除左子树(或者切除右子树也可以) 
		lTree->parent = NULL;
		this._root->lChild = NULL;
		
		this._root = this._root->rChild;	//不太懂这个地方
		this._root->parent = NULL;
		
		search(w->data);	//重新查找一次，使得右子树最小节点伸展到树根, 且左子树必定为空(左孩子必定小于根节点)
		this._root->lChild = lTree;
		lTree->parent = this._root;
	}
	
	this._size--;
	
	if(this._root){
		updateHeight(this._root);
	} 

	return true;
}
