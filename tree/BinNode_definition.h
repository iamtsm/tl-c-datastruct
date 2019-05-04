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
#define FromParentTo(x) (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lChild : (x).parent->rChild))
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
