#include <iostream>

using namespace std;
#define BinNodePosi(T) BinNode<T>* //节点位置
#define stature(p)((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）
#define IsRoot(x) (!((x).parent)) //是否是根节点
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rChild)) //是否是右孩子
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lChild)) //是否是左孩子
#define HasParent(x) (!IsRoot(x))	//是否有父节点
#define HasRChild(x) ((x).rChild) //是否有右孩子
#define HasLChild(x) ((x).lChild) //是否有左孩子
#define HasChild(x) (HasRChild(x) || HasLChild(x))  //是否有其中一个孩子
#define HasBothChild(x) (HasRChild(x) && HasLChild(x)) //是否有左右两个孩子
#define IsLeaf(x) (!HasChild(x))  //是否达到叶子节点
#define FromParentTo(x) (IsRoot(x) ? this->_root : (IsLChild(x) ? (x).parent->lChild : (x).parent->rChild))
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rChild : (p)->parent->lChild)	//返回兄弟节点
#define uncle(x) (IsLChild( *((x)->parent) ) ? (x)->parent->parent->rChild : (x)->parent->parent->lChild) //返回叔叔节点
typedef enum { RB_RED, RB_BLACK} RBColor; //节点颜色




template <typename T> struct BinNode { //树节点结构体
	
	BinNodePosi(T) parent; 
	BinNodePosi(T) lChild; 
	BinNodePosi(T) rChild;	//父亲,孩子
	T data; //数据域
	int height; //高度
	int npl; //Null Path Length（左式堆，也可直接用height代替）
	RBColor color; //颜色(红黑树)
	
	BinNode():parent(NULL), lChild(NULL), rChild(NULL), height(0),npl(0),color(RB_RED){} //默认构造函数
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL,BinNodePosi(T) rc = NULL,
			int h = 0,int l = 1, RBColor c = RB_RED):
			data(e),parent(p), lChild(lc), rChild(rc), height(h),npl(l),color(c){} //参数构造
	
	BinNodePosi(T) insertAsLC(T const & e);	//作为左孩子插入
	BinNodePosi(T) insertAsRC(T const & e); //作为右孩子插入
	
	int size(); //子树规模
	BinNodePosi(T) succ();	//(中序遍历下的当前节点的直接后继)
	void travLevel(BinNodePosi(T) x); //子树层次遍历
	void travPre(BinNodePosi(T) x); //子树先序遍历
	void travIn(BinNodePosi(T) x); //子树中序遍历
	void travPost(BinNodePosi(T) x); //子树后序遍历
	
	//~ void travPreI1(BinNodePosi(T) x); //先序遍历迭代版1
	//~ void visitAlongLeftBranch(BinNodePosi(T) x,Stack<BinNodePosi(T)> &S); //遍历左子树，使其依次入栈,并访问
	//~ void travPreI2(BinNodePosi(T) x); //先序遍历迭代版2主算法
	//~ void travInI1(BinNodePosi(T) x); //中序遍历迭代版1主算法
	//~ void goAlongLeftBranch(BinNodePosi(T) x,Stack<BinNodePosi(T)> &S);	//遍历左子树，使其依次入栈
};




template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const &e){
	return lChild = new BinNode(e,this); //当前节点的左孩子指向新开辟的节点
}

template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const &e){
	return rChild = new BinNode(e,this);	//前节点的右孩子指向新开辟的节点
}

template <typename T> int BinNode<T>::size(){ //统计当前节点后代总数，即以其为根的子树规模
	int s = 1;	//计入本身
	if (lChild) s += lChild->size(); //递归计入左子树规模
	if (rChild) s += rChild->size();
	return s;
}


template <typename T> void BinNode<T>::travPre(BinNodePosi(T) x){ //先序遍历
	if(!x) return; //递归基
	cout << x->data << "->";
	travPre(x->lChild);
	travPre(x->rChild);
}

template <typename T> void BinNode<T>::travIn(BinNodePosi(T) x){ //中序遍历
	if(!x) return; //递归基
	travPre(x->lChild);
	cout << x->data << "->";
	travPre(x->rChild);
}

template <typename T> void BinNode<T>::travPost(BinNodePosi(T) x){ //后序遍历
	if(!x) return; //递归基
	travPre(x->lChild);
	travPre(x->rChild);
	cout << x->data << "->";	
}

template <typename T> BinNodePosi(T) BinNode<T>::succ() { //定位节点v的直接后继
    BinNodePosi(T) s = this; //记录后继的临时变量
    if ( rChild ) { //若有右孩子，则直接后继必在右子树中，具体地就是
       s = rChild; //右子树中
       while ( HasLChild ( *s ) ) s = s->lChild; //最靠左（最小）的节点
    } else { //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
       while ( IsRChild ( *s ) ) s = s->parent; //逆向地沿右向分支，不断朝左上方移动
       s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
    }
    return s;
}




/**
 * 
template <typename T> void BinNode<T>::travPreI1(BinNodePosi(T) x){  //先序遍历迭代版1
	Stack<BinNodePosi(T)> S;	//引入一个栈
	if(x) S.push(x);	//根节点入栈
	
	while(!S.empty()){
		x = S.pop(); cout << x->data << "->";
		if(x->rChild) S.push(x->rChild); //右子树先入栈
		if(x->lChild) S.push(x->lChild); //左子树后入栈，先出栈
	}
}
 * */

/**
 * 此迭代版，左子树不入栈，直接访问(自根向下访问)，右子树入栈(自下向根访问)(先序遍历)

template <typename T> void BinNode<T>::visitAlongLeftBranch(BinNodePosi(T) x,Stack<BinNodePosi(T)> &S){ //先序遍历迭代版2子算法
	while(x){
		cout << x->data << "->";
		S.push(x->rChild);	//右节点入栈，到时候遍历pop时自然是自底而上
		x = x->lChild;
	}
}
template <typename T> void BinNode<T>::travPreI2(BinNodePosi(T) x){//先序遍历迭代版2主算法
	Stack<BinNodePosi(T)> S;
	
	while(true){
		visitAlongLeftBranch(x,S);	//循环遍历访问x节点左子树
		if(S.empty()) break;	
		x = S.pop();	// 弹出右子树,x节点不断更新,然后在子算法中进行遍历
	}
}
 * */
 
 
/**
 * 此迭代版，左子树入栈，直到最后含有左孩子的节点(中序遍历)

template <typename T> void BinNode<T>::goAlongLeftBranch(BinNodePosi(T) x,Stack<BinNodePosi(T)> &S){ //遍历左分支，使其入栈
	while(x){S.push(x); x = x->lChild;}
}

template <typename T> void BinNode<T>::travInI1(BinNodePosi(T) x){
	Stack<BinNodePosi(T)> S;
	
	while(true){
		goAlongLeftBranch(x,S);	//当前节点的左子树入栈
		if(S.empty()) break;
		x = x.pop();	//弹出左分支最后的一个节点(无左节点的最后一个节点)
		cout << x->data << "->";	//访问
		x = x->rChild;	//转向右子树
	}
} **/



/**
 * 错误版
 * 
template <typename T> template <typename VST> void BinNode<T>::travLevel(BinNodePosi(T) x){ //层次遍历
	Queue<BinNodePosi(T)> Q;
	
	while(!Q.empty()){	//不能用x==NULL判断 否则队列为空时最后还会dequeue()一次 
		cout << x->data << "->";
		if(x->lChild) Q.enqueue(x->lChild);
		if(x->rChild) Q.enqueue(x->rChild);
		x = Q.dequeue();
	}
}
*/

/**
 * 正确版
 * 
template <typename T> void BinNode<T>::travLevel(BinNodePosi(T) x){ //层次遍历
	Queue<BinNodePosi(T)> Q;
	Q.enqueue(this);
	while(!Q.empty()){	//不能用x==NULL判断 否则队列为空时最后还会dequeue()一次 
		x = Q.dequeue();
		cout << x->data << "->";
		if(x->lChild) Q.enqueue(x->lChild);
		if(x->rChild) Q.enqueue(x->rChild);
	}
}
**/
