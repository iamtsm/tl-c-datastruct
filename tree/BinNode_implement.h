#include "BinNode_definition.h"


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
