#include "BTNode.h"

template <typename T> class BTree{
	
	protected : 
		int _size; int order;	BTNodePosi(T) _root;	//关键码总数, 阶次, 根节点 
		BTNodePosi(T) _hot;	//同BST;
		void solveOverflow(BTNodePosi(T) v);	//因插入发生上溢的分裂处理
		void solveUnderflow(BTNodePosi(T) v);	//因删除发生下溢的合并处理
	public : 
	
	     BTree ( int order = 3 ) : _order ( order ), _size ( 0 ) //构造函数：默认为最低的3阶
		{ _root = new BTNode<T>(); }
		~BTree() { if ( _root ) release ( _root ); } //析构函数：释放所有节点
		int const order() { return _order; } //阶次
		int const size() { return _size; } //规模
		BTNodePosi(T) & root() { return _root; } //树根
		bool empty() const { return !_root; } //判空
	
		BTNodePosi(T) search(const T &e);	//查找
		bool insert(const T &e);	//插入
		bool remove(const T &e);	//删除
};


template <typename T> bool BTree<T>::insert(const T &e){
	BTNodePosi(T) v= search(e);	//树中查找
	
	if(v) return false;	//已经存在元素, 直接返回
	
	Rank r = _hot->key.search(e);	//确定插入的位置 , 向量查找,返回不大于e的最后一个位置
	
	_hot->key.insert(r + 1, e);	//插入到vector中
	_hot->child.insert(r + 2, NULL);	//创建空指针
	
	_size++;	solveOverflow(_hot);	//发生上溢,分裂
	
	return true;
}


template <typename T> bool BTree<T>::remove(const T &e){
	BTNodePosi(T) v= search(e);	//树中查找
		
	if(!v) return false;	//不存在元素,直接返回

	Rank r = v->key.search(e);	//确定待删除元素的位置
	
	if(v->child[0]){	//如果有分支, 说明不是叶节点
		BTNodePosi(T) u = v->child[r + 1];	//找到v节点的右侧分支
		while(u->child[0]) u = u->child[0];	//不断向左侧深入,直到叶节点
		v->key[r] = u->key[0];	//将当前待删除元素和深入的叶节点互换位置(转换成删除叶节点) 
		v = u; r = 0;
	}
	
	v->key.remove(r);	v->child.remove(r + 1);	//删除节点关键码和分支
	
	_size--; solveUnderflow(v);		//发生下溢,旋转或者合并
	return true;
}


/**
 * 上溢节点处理
 * */
template <typename T> void BTree<T>::solveOverflow(BTNodePosi(T) v){
	
	if(_order >= v->child.size()) return ;	//递归基  判断节点的阶次是否大于孩子数量，一旦阶次较大或者相等，则可以判断该节点未上溢
	
	/**
	 * 分解上溢节点,将轴点右侧分离到一个新节点(包括关键码和其指针指向的孩子需要全部分离到新节点)
	 * */
	Rank s = _order / 2;	//轴点
	BTNodePosi(T) u = new BTNode<T>();	//轴点右侧关键码组成新节点
	for(Rnak j = 0; j < _order-s-1; j++){
		u->child.insert(j,v->child.remove(s+1));	//逐个移动孩子
		u->key.insert(j,v-key.remove(s+1));		//移动关键码
	}
	
	u->child[_order-s-1] = v->child.remove(s+1);	//移动最靠右的孩子
	if(u->child[0]){ //若u的孩子非空
		for(Rank j = 0; j < _order-s; j++){
			u->child[j]->parent = u;	//更新孩子parent指针指向u
		}
	}
	
	/**
	 * 关键码分离后上升到父节点中,(先找到关键码待插入的位置，随机插入分离的关键码即可)
	 * */
	BTNodePosi(T) p = v->parent;	//v当前的父节点
	if(!p) {		//如果父节点不存在就创建
		_root = p = new BTNode<T>();
		p->child[0] = v;
		v->parent = p;
	}
	
	Rank r = 1 + p->key.search(v->key[0]);	//找到v父节点p中的指针指向u的秩
	p->key.insert(r,v->key.remove(s));	//轴点关键码上升一层
	p->child.insert(r+1,u);	u->parent = p;	//新节点u 
	
	solveOverflow(p);	//向上递归分裂所有上溢节点  最多树高(logn = h)次
}


/**
 * 下溢节点处理
 * */
template <typename T> void BTree<T>::solveUnderflow(BTNodePosi(T) v){
	
	if ((_order + 1) / 2 <= v->child.size()) return; //递归基：当前节点并未下溢
	
	BTNodePosi(T) p = v->parent;
    if ( !p ) { //递归基：已到根节点，没有孩子的下限
       if ( !v->key.size() && v->child[0] ) {
          //但倘若作为树根的v已不含关键码，却有（唯一的）非空孩子，则
          _root = v->child[0]; _root->parent = NULL; //这个节点可被跳过
          v->child[0] = NULL; release ( v ); //并因不再有用而被销毁
       } //整树高度降低一层
       return;
    }
    Rank r = 0; while ( p->child[r] != v ) r++;
    //确定v是p的第r个孩子——此时v可能不含关键码，故不能通过关键码查找
    //另外，在实现了孩子指针的判等器之后，也可直接调用Vector::find()定位
    // 情况1：向左兄弟借关键码
    if ( 0 < r ) { //若v不是p的第一个孩子，则
       BTNodePosi(T) ls = p->child[r - 1]; //左兄弟必存在
       if ( ( _order + 1 ) / 2 < ls->child.size() ) { //若该兄弟足够“胖”，则
          v->key.insert ( 0, p->key[r - 1] ); //p借出一个关键码给v（作为最小关键码）
          p->key[r - 1] = ls->key.remove ( ls->key.size() - 1 ); //ls的最大关键码转入p
          v->child.insert ( 0, ls->child.remove ( ls->child.size() - 1 ) );
          //同时ls的最右侧孩子过继给v
          if ( v->child[0] ) v->child[0]->parent = v; //作为v的最左侧孩子
          return; //至此，通过右旋已完成当前层（以及所有层）的下溢处理
       }
    } //至此，左兄弟要么为空，要么太“瘦”
	  // 情况2：向右兄弟借关键码
    if ( p->child.size() - 1 > r ) { //若v不是p的最后一个孩子，则
       BTNodePosi(T) rs = p->child[r + 1]; //右兄弟必存在
       if ( ( _order + 1 ) / 2 < rs->child.size() ) { //若该兄弟足够“胖”，则
          v->key.insert ( v->key.size(), p->key[r] ); //p借出一个关键码给v（作为最大关键码）
          p->key[r] = rs->key.remove ( 0 ); //ls的最小关键码转入p
          v->child.insert ( v->child.size(), rs->child.remove ( 0 ) );
          //同时rs的最左侧孩子过继给v
          if ( v->child[v->child.size() - 1] ) //作为v的最右侧孩子
             v->child[v->child.size() - 1]->parent = v;
          return; //至此，通过左旋已完成当前层（以及所有层）的下溢处理
       }
    } //至此，右兄弟要么为空，要么太“瘦”
	  // 情况3：左、右兄弟要么为空（但不可能同时），要么都太“瘦”——合并
    if ( 0 < r ) { //与左兄弟合并
       BTNodePosi(T) ls = p->child[r - 1]; //左兄弟必存在
       ls->key.insert ( ls->key.size(), p->key.remove ( r - 1 ) ); p->child.remove ( r );
       //p的第r - 1个关键码转入ls，v不再是p的第r个孩子
       ls->child.insert ( ls->child.size(), v->child.remove ( 0 ) );
       if ( ls->child[ls->child.size() - 1] ) //v的最左侧孩子过继给ls做最右侧孩子
          ls->child[ls->child.size() - 1]->parent = ls;
       while ( !v->key.empty() ) { //v剩余的关键码和孩子，依次转入ls
          ls->key.insert ( ls->key.size(), v->key.remove ( 0 ) );
          ls->child.insert ( ls->child.size(), v->child.remove ( 0 ) );
          if ( ls->child[ls->child.size() - 1] ) ls->child[ls->child.size() - 1]->parent = ls;
       }
       release ( v ); //释放v
       
    } else { //与右兄弟合并
       BTNodePosi(T) rs = p->child[r + 1]; //右兄度必存在
       rs->key.insert ( 0, p->key.remove ( r ) ); p->child.remove ( r );
       //p的第r个关键码转入rs，v不再是p的第r个孩子
       rs->child.insert ( 0, v->child.remove ( v->child.size() - 1 ) );
       if ( rs->child[0] ) rs->child[0]->parent = rs; //v的最左侧孩子过继给ls做最右侧孩子
       while ( !v->key.empty() ) { //v剩余的关键码和孩子，依次转入rs
          rs->key.insert ( 0, v->key.remove ( v->key.size() - 1 ) );
          rs->child.insert ( 0, v->child.remove ( v->child.size() - 1 ) );
          if ( rs->child[0] ) rs->child[0]->parent = rs;
       }
       release ( v ); //释放v
    }
    solveUnderflow ( p ); //上升一层，如有必要则继续分裂——至多递归O(logn)层
    return;
}

