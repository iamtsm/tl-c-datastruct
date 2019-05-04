#include "List_definition.h"


template <typename T> void List<T>::init(){ //列表初始化
	header = new ListNode<T>; //创建头哨兵节点
	trailer = new ListNode<T>; //创建尾哨兵节点
	header->succ = trailer;	header->pred = NULL;
	trailer->succ = NULL;	trailer->pred = header;
	_size = 0;
}

template <typename T> T& List<T>::operator [](Rank r) const{ //效率很低 复杂度是O(r)
	ListNodePosi(T) p = first();	//获取首节点
	while(0 < r--) {p = p->succ;}	//循环获取到第r个位置的节点
	return p->data;	//返回节点数据
}

template <typename T> ListNodePosi(T) List<T>::find( T const& e, int n, 
	ListNodePosi(T) p ) const{	//查找n个真前驱
	while(0 < n--){	
		if (e == (p = p->pred)->data) return p;	//从右至左遍历数据域
	}
	return NULL; //查找失败
}

template <typename T> ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const& e){ //插入到某个节点前
	_size++; //更新大小
	return p->insertAsPred(e);	//结构体中的元素(插入节点方法)
}

template <typename T> ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const& e){ //插入到某个节点后
	_size++; //更新大小
	return p->insertAsSucc(e);	//结构体中的元素(插入节点方法)
}

template <typename T> ListNodePosi(T) List<T>::insertAsFirst( T const& e){ //作为首节点插入
	_size++; //更新大小
	return header->insertAsSucc(e);	
}

template <typename T> ListNodePosi(T) List<T>::insertAsLast( T const& e){ //作为尾节点插入
	_size++; //更新大小
	return trailer->insertAsPred(e);	
}

template <typename T> T List<T>::remove(ListNodePosi(T) p){	//删除某个节点
	T e = p->data;	//待删除节点的数据域需要返回
	p->pred->succ = p->succ;	//更新前驱指向
	p->succ->pred = p->pred;	//更新后继指向
	delete p;	_size--; 		//释放空间，更新列表大小
	return e;
}

template <typename T> List<T>::~List(){	//析构函数，释放列表
	clear();	delete header; delete trailer;
}

template <typename T> int List<T>::clear(){	//清除所有节点
	int oldSize = _size;	//备份待清除列表大小
	while (0 < _size){		 //逐一删除，因为删除接口在每次删除节点后会自动更新指向。
		remove(header->succ);//所以header->succ在合法(_size)范围内永远有指向的节点
	}						 //且_size大小无需更新，因为在remove接口执行完，大小保持更新
	return oldSize;
}

template <typename T> int List<T>::deduplicate(){ //针对无序列表唯一化
	if (_size < 2) return 0; //平凡情况
	int oldSize = _size;	//备份大小需返回
	ListNodePosi(T) p = first(); //首节点开始
	Rank r = 1;	//唯一化大小区间(也就是待对比区间大小) 单调递增
	
	while(trailer != (p = p->succ)){
		ListNodePosi(T) q = find(p->data,r,p);	//执行find()进行遍历查找是否存在重复数据域节点，并用q指向(为什么不用p, 因为p用来是循环，如果改变，循环会出错)
		q ? remove(q) : r++;	//存在则返回数据域(q是否有值)，存在则执行remove(q) ，否则增大唯一化区间大小
	}
	return oldSize - _size;	//返回剔除总数
}

template <typename T> int List<T>::uniquify(){	//针对有序列表唯一化
	if(_size < 2) return 0;	//平凡情况
	int oldSize = _size;	//备份大小
	ListNodePosi(T) p = first();	//首节点开始
	ListNodePosi(T) q;	//用于比较控制，初始指向p的后继节点
	Rank r = 1;	//唯一化大小区间(也就是待对比区间大小) 单调递增
	
	while(trailer != (q = p->succ)){ //逐次移动q指针，用于和p指针数据域做比较
		if(q->data == p->data) {  //如果相等，则删除q指向的节点
			remove(q);	
		}else{
			p = q;  //如果不相等，则证明前一个区段重复数据域已经删除完毕
		}
	}
	return oldSize-_size;
}


template <typename T> ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p ) const{ //p节点的n个前驱中查找
	while(0 <= n--){	//从右向左遍历
		if((p = p->pred)->data <= e) break;	//返回不大于e的最右侧的一个节点
	}
	return p;
}

template <typename T> void List<T>::selectionSort(ListNodePosi(T) p, int  n){
	ListNodePosi(T) head = p->pred; //首节点
	ListNodePosi(T) tail = p;
	for(int i = 0; i < n; i++) tail = tail->succ;	//初始化tail指向距离p n个节点
	while(1 < n){	//遍历单调递减的待排序节点区间(U)，选择最大值节点，插入到tail的前驱中
		insertBefore(tail,remove(selectMax(head->succ,n)));
		tail = tail->pred;	//更新tail指向，tail永远指向已经排序的节点区间(T)的首节点
		n--; //区间单调递减
	}
}

/**
template <typename T> ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n){
	ListNodePosi(T) max = p; //初始化一个max指针
	while(1 < n--){		//遍历p节点的n个后继节点
		if(max->data <= (p = p->succ)->data){ //如果后继某个节点大于max节点数据域
			max = q;	//更新max指向
		}
	}
	return max;	//返回最大数据域节点
}**/

template <typename T> ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n){
	ListNodePosi(T) max = p; //初始化一个max指针
	for(ListNodePosi(T) cur = p; 1 < n; n--){//遍历p节点的n个后继节点
		if(max->data <= (cur = cur->succ)->data){//如果后继某个节点大于max节点数据域
			max = cur;	//更新max指向
		}
	}
	return max;	//返回最大数据域节点
}

template <typename T> void List<T>::sort(ListNodePosi(T) p , int n){ //列表区间排序
	for (int r = 0; r < n; r++){	
		insertAfter(search(p->data,r,p),p->data); //找到不大于p->data的最右侧节点，将当前节点插入到其后
		p = p->succ; 	//移动到下一节点
		remove(p->pred);	//删除已经insert的节点
	}
}



