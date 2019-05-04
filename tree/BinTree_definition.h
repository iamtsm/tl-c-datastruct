#include <iostream>
#include "BinNode_implement.h"

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
		~BinTree(){if ( 0 < _size ) {cout << " size : " << _size; remove ( _root );}} //析构函数

		//test
		BinNodePosi(T) genRandomTree(BinNodePosi(T) x,int h);  //生成测试用的深度为h的树
};
