#include <iostream>
using namespace std;
typedef int Rank;
#define DEFAULT_CAPACITY  3 
#define ListNodePosi(T) ListNode<T>* //ListNode类型改个名字而已
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




template <typename T> class Vector{ //向量模板类
	protected:
	
	Rank _size; Rank _capacity; char *_elem; 							//规模 容量　数据区
	void copyFrom(T const* A,Rank lo,Rank hi);						//复制数组区间[lo,hi]
	void expand();														//看见不足时扩容
	void shrink(); 														//装填因子过小时压缩
	void swap(char &A, char &B);										//交换
	Rank bubble(Rank li,Rank hi);										//扫描交换
	void bubbleSort(Rank lo,Rank hi);									//起泡排序算法
	Rank max(Rank lo,Rank hi);											//选取最大元素
	void selectionSort(Rank lo, Rank hi); 								//选择排序算法
	void merge(Rank lo, Rank mi, Rank hi); 								//归并算法
	void mergeSort(Rank lo, Rank hi);									//归并排序算法
	Rank partition(Rank lo, Rank hi);									//轴点构造算法
	void quickSort(Rank lo, Rank hi);									//快速排序算法
	void heapSort(Rank lo, Rank hi);									//堆排序
	
	public :
	//构造
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T *v = 0){			//容量为c 规模为s 所有元素初始化为v
		_elem = new T[_capacity = c];
		int i = 0;
		for(_size = 0; _size < s; _elem[_size++] = v[i++]); 			//s <= c
	}
	//当是以数组的形式存放的数据,构造函数可以通过复制数组中的区间作为初始向量,靠传入的参数判定
	Vector(T const *A,Rank lo, Rank hi){
		copyFrom(A,lo,hi);												//数组区间复制
	}
	//本身是数组,复制整个数组
	Vector(T const *A,Rank n){
		copyFrom(A,0,n);												//数组整体复制
	}
	//本身是向量 复制某个区间
	Vector(Vector<T> const&V,Rank lo,Rank hi){
		copyFrom(V._elem,lo,hi);										//向量区间复制
	}
	//本身是向量, 复制整个向量
	Vector(Vector<T> const&V){
		copyFrom(V._elem,0,V._size);									//向量整体复制
	}
	//析构
	~Vector(){
		delete [] _elem;
	}
	
	//只读访问接口
	Rank size() const{return _size;} 									//规模
	bool empty() const{return !_size;}									//判空
	int disordered() const;												//判断向量是否排序
	Rank find(T const&e) const{return find(e,0,_size);}				//无序向量的整体查找
	Rank find(T const&e,Rank lo, Rank hi) const;						//无序向量区间查找
	Rank search(T const&e) const 									//有序向量的整体查找
	{return (0 >= _size ? -1 : search(e,0,_size));}
	Rank search(T const&e,Rank lo,Rank hi)const;						//有序向量的区间查找
	
	//可写访问接口
	T &operator[](Rank r)const;             							//重载下标操作符，可以类似与数组形式引用各元素
	const T &operator = (Vector<T> const&); 									//重载赋值操作符，以便直接克隆向量
	T remove(Rank r); 												//删除秩为r的元素
	int remove(Rank lo,Rank hi); 										//删除秩在区间[lo,hi]内的元素
	Rank insert(Rank lo,T const&e); 									//添加元素
	Rank insert(T const &e){return insert(_size,e);}					//默认作为末元素添加
	void sort(Rank lo,Rank hi);											//对[lo,hi)排序
	void sort(){sort(0,_size);} 										//对整体排序
	void unsort(Rank lo,Rank hi); 										//对[lo,hi)置乱
	void unsort(){unsort(0,_size);} 									//对整体置乱
	int deduplicate(); 													//无序去重
	int uniquify_low(); 												//有序去重 低效
	int uniquify_height(); 												//有序去重  高效
    Rank binSearch(T *A,T const&e,Rank lo,Rank hi);				//二分查找版本A
    Rank fibSearch(T *A,T&e,Rank lo,Rank hi);						//fib查找
	Rank binSearch_Batter(T *A,T const&e,Rank lo,Rank hi);		//二分查找稳定版
	void show();
	//遍历
	void traverse(void (*)(T&)); //遍历(使用函数指针,只读或局部性修改)
};


template <typename T> struct ListNode{	//列表节点结构体
	T data;	//数值
	ListNodePosi(T) pred; //前驱
	ListNodePosi(T) succ; //后继
	ListNode(){}
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		:data(e),pred(p),succ(s){} //默认构造器
	ListNodePosi(T) insertAsPred(T const &e); //前插入算法    其他插入都是以这个为基础
	ListNodePosi(T) insertAsSucc(T const &e); //后插入算法
};


template <typename T> class List { //列表模板类
 
 private:
    int _size; ListNodePosi(T) header; ListNodePosi(T) trailer; //规模、头哨兵、尾哨兵
 
 protected:
    void init(); //列表创建时的初始化
    int clear(); //清除所有节点
    void copyNodes ( ListNodePosi(T), int ); //复制列表中自位置p起的n项
    void merge ( ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int ); //归并
    void mergeSort ( ListNodePosi(T)&, int ); //对从p开始连续的n个节点归并排序
    void selectionSort ( ListNodePosi(T), int ); //对从p开始连续的n个节点选择排序
    void insertionSort ( ListNodePosi(T), int ); //对从p开始连续的n个节点插入排序
 
 public:
    // 构造函数
    List() { init(); } //默认
    List ( List<T> const& L ); //整体复制列表L
    List ( List<T> const& L, Rank r, int n ); //复制列表L中自第r项起的n项
    List ( ListNodePosi(T) p, int n ); //复制列表中自位置p起的n项
    // 析构函数
    ~List(); //释放（包含头、尾哨兵在内的）所有节点
    
    // 只读访问接口
    Rank size() const { return _size; } //规模
    bool empty() const { return _size <= 0; } //判空
    T & operator[] ( Rank r ) const; //重载，支持循秩访问（效率低） 复杂度是O(r)
    
	ListNodePosi(T) first() const { return header->succ; } //首节点位置
    ListNodePosi(T) last() const { return trailer->pred; } //末节点位置
    bool valid ( ListNodePosi(T) p ) { return p && ( trailer != p ) && ( header != p ); }  //判断位置p是否对外合法 //将头、尾节点等同于NULL
    
    int disordered() const; //判断列表是否已排序
    ListNodePosi(T) find ( T const& e ) const { return find ( e, _size, trailer ); }//无序列表查找
    ListNodePosi(T) find ( T const& e, int n, ListNodePosi(T) p ) const; //无序区间查找
    ListNodePosi(T) search ( T const& e ) const { return search ( e, _size, trailer ); }//有序列表查找
    ListNodePosi(T) search ( T const& e, int n, ListNodePosi(T) p ) const; //有序区间查找
    ListNodePosi(T) selectMax ( ListNodePosi(T) p, int n ); //在p及其n-1个后继中选出最大者
    ListNodePosi(T) selectMax() { return selectMax ( header->succ, _size ); } //整体最大者
    
	// 可写访问接口
    ListNodePosi(T) insertAsFirst ( T const& e ); //将e当作首节点插入
    ListNodePosi(T) insertAsLast ( T const& e ); //将e当作末节点插入
    ListNodePosi(T) insertAfter ( ListNodePosi(T) p, T const& e ); //将e当作p的后继插入（After）
    ListNodePosi(T) insertBefore ( ListNodePosi(T) p, T const& e ); //将e当作p的前驱插入（Before）

    
    T remove ( ListNodePosi(T) p ); //删除合法位置p处的节点,返回被删除节点
    void merge ( List<T>& L ) { merge ( first(), size, L, L.first(), L._size ); } //全列表归并
    void sort ( ListNodePosi(T) p, int n ); //列表区间排序
    void sort() { sort ( first(), _size ); } //列表整体排序
    int deduplicate(); //无序去重
    int uniquify(); //有序去重
    void reverse(); //前后倒置（习题）
	// 遍历
    void traverse ( void (* ) ( T& ) ); //遍历，依次实施visit操作（函数指针，只读或局部性修改）
    template <typename VST> //操作器
    void traverse ( VST& ); //遍历，依次实施visit操作（函数对象，可全局性修改）
 };


template <typename T> class Stack: public List<char> {	//栈模板类
	
	public :
		void push(T const &e) {
			insertAsLast(e);
		}
		T pop() {
			return remove(last()); 
		}
		T & top() {
			return last()->data;
		}
};


template <typename T> class Queue: public List<char>{	//队列模板类
	
	public :
		void enqueue(T const& e){insertAsLast(e);} //插入
		
		T dequeue(){return remove(first());} //弹出
		
		T & front(){return first()->data;} //返回头元素
};


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


/** 
 * ********************************************Vector实现************************************************
 **/

template <typename T> void Vector<T>::copyFrom(T const*A,Rank lo,Rank hi)
{
	_elem = new T[_capacity = 2*(hi-lo)];	//分配空间　规模清零
	_size = 0;
	while(lo < hi){
		_elem[_size++] = A[lo++];	//A[lo,hi)内的元素逐一复制到_elem[0,hi-lo)
	}
}

template <typename T> const T& Vector<T>::operator =(Vector<T> const&V){
	if(_elem) delete [] _elem;	//复制原有内容
	copyFrom(V._elem, 0, V._size); //整体复制
	return *this; 	//返回当前对象的引用，便于链式赋值
}

template <typename T> void Vector<T>::merge(Rank lo,Rank mi,Rank hi){
	T *A = _elem + lo;
	int lb = mi - lo; char *B = new T[lb];
	for(Rank i = 0; i < lb; i++) B[i] = A[i];	//复制前子向量
	int lc = hi - mi; char *C = _elem + mi;    //后子向量C[0,lc) = _elem[hi,hi);
	for(Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);){
		if((j < lb) && (lc <= k || (B[j] <= C[k]))) A[i++] = B[j++];
		if((k < lc) && (lb <= j || (C[k] < B[j]))) A[i++] = C[j++];
	}
	delete []B;
}


template <typename T> void Vector<T>::expand(){
	if(_size < _capacity) return;	//未满容　不必扩容
	if(_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; //不低于最小容量
	char *oldElem = _elem;  _elem = new T[_capacity <<= 1]; //容量加倍
	for(int i = 0; i < _size; i++){
		_elem[i] = oldElem[i];
	}//复制原向量内容
	delete [] oldElem;
}

template <typename T> void Vector<T>::shrink(){
	if(_capacity < DEFAULT_CAPACITY << 1) return; //不至于收缩到最低容量下
	if(_size << 2 > _capacity) return;	//25% 为界限
	char *oldElem = _elem;  _elem = new T[_capacity >>= 1];	//容量减半
	for(int i = 0; i < _size; i++){
		_elem[i] = oldElem[i];  //复制原向量内容
	}
	delete []oldElem;
}

template <typename T> T &Vector<T>::operator [](Rank r)const //重载下标操作符
{return _elem[r];	} //assert : 0 <= r < _size

template <typename T> Rank Vector<T>::find(T const &e,Rank lo,Rank hi) const {
	while((lo < hi--) && (e != _elem[hi])); //自后向前顺序查找
	return hi;	//若hi < lo　则意味着失败 否则hi 就是命中元素的秩
}

template <typename T> Rank Vector<T>::binSearch(T *A,T const&e,Rank lo,Rank hi){
	while(lo < hi){
		Rank mi = (lo + hi) >> 1;//以终点为轴
		if(e < A[mi]){hi = mi;}	//深入前半段
		else if(A[mi] < e) lo = mi + 1;	//深入后半段
		else{return mi;}//在mi处命中
	}
	return -1;		//查找失败
}

template <typename T> Rank Vector<T>::binSearch_Batter(T *A,T const&e,Rank lo,Rank hi){
	while(lo < hi){
		Rank mi = (lo + hi) >> 1;
		(e < A[mi]) ? hi = mi : lo = mi + 1;		//不会遗漏mi这个点
	}
	return --lo;
}


template <typename T> Rank Vector<T>::fibSearch(T *A,T &e,Rank lo,Rank hi){
	//这里通过fib数来确定mi的值.....需要添加
	while(lo < hi){
	//这里需要来判断需要迭代计算几次.....需要添加
		Rank mi = (lo + hi) >> 1;//以fib数为轴
		if(e < A[mi]){hi = mi;}//深入前半段
		else if(A[mi] < e){lo = mi + 1;}//深入后半段
		else{return mi;}
	}
	return -1;
}

template <typename T> Rank Vector<T>::insert(Rank r, T const&e){
	expand();	//有必要扩容
	for(int i = _size; i < r; i--) _elem[i] = _elem[i-1];	//自后向前，后继元素顺次后移一个单元 
	_elem[r] = e; _size++;	//置入新元素并更新容量	
	return r; 	//返回秩
}

template <typename T> int Vector<T>::remove(Rank lo, Rank hi){	//删除区间
	if(lo == hi) return 0;	//出于效率考虑，单独处理退化情况,比如remove(0,0)
	while(hi < _size) _elem[lo++] = _elem[hi++];	//[hi,_size]顺次前移hi - lo个单元
	_size = lo;  //更新规模 直接丢弃尾部[lo, _size = hi)区间
	shrink(); //有必要，缩容
	return hi-lo;	//返回被删除元素的数目
}

template <typename T> T Vector<T>::remove(Rank r){
	char e = _elem[r];	//备份被删除的元素
	remove(r,r+1); //调用区间删除算法
	return e; //返回被删除的元素
}

template <typename T> int Vector<T>::deduplicate(){	//无序向量去重复元素操作
	int oldSize = _size;	//记录原规模
	Rank i = 1; 		//从_elem[1] 开始
	while(i < _size){ //自前向后逐一考察各个元素_elem[i]
		(find(_elem[i], 0, i) < 0) ? i++ : remove(i);	//在其前缀中寻找相同元素(最多一个)  找到了相同元素就删除，否则继续向后逐一比对
	}
	return oldSize - _size; 	//向量规模变化量
}

template <typename T> int Vector<T>::disordered() const{	//返回向量中逆序相邻元素对的总数
	int count = 0; 	//计数器
	for(int i = 1; i < _size; i++){	//逐一检查_size - 1相邻元素
		if(_elem[i - 1] > _elem[i]){
			count++;	//逆序则记数
		}
	}
	return count;
}

template <typename T> int Vector<T>::uniquify_low(){	//有序向量的重复元素剔除
	int oldSize = _size;  int i = 1; //当前比对元素的秩　	起始于首位置
	while( i < _size){
		_elem[i+1] == _elem[i] ? remove(i) : i++;	//若相同　删除后者　否则元素向后移
	}
	return oldSize - _size;	//返回被删除元素的个数
}

template <typename T> int Vector<T>::uniquify_height(){
	Rank i = 0, j = 0;
	while(++j < _size){//逐一扫描 直到末尾元素
		if(_elem[i] != _elem[j]){	//跳过相同元素
		_elem[++i] = _elem[j];	//发现不同元素时　把该元素移动到比对元素紧邻右侧
	}
	_size = ++i; shrink();	//直接截除尾部多余元素
	}
	return j - i;	//返回被删除的元素
}

template <typename T> void Vector<T>::swap(char &A, char &B){
	char temp = A;	//交换位置
	A = B;
	B = temp;
}

template <typename T> void Vector<T>::bubbleSort(Rank lo,Rank hi){
	while(lo < (hi = bubble(lo,hi))); //逐趟扫描交换
}

template <typename T> int Vector<T>::bubble(Rank lo, Rank hi){
	Rank last = lo;	//初始化最后一个逆序对为 [0,1]
	
	while(++lo < hi){
		if(_elem[lo - 1] > _elem[lo]){
			last = lo;	//更新出现的逆序对的位置
			swap(_elem[lo - 1],_elem[lo]); //交换
		}
	}
	return last;  //返回最右侧逆序对
}

/** 
 * ********************************************ListNode实现************************************************
 **/
template <typename T> ListNodePosi(T) ListNode<T>::insertAsPred(T const &e){ //前插入
	ListNodePosi(T) x = new ListNode(e,pred,this);	//创建新节点，并初始化两条指向
	pred->succ = x; pred = x; return x;  //更新其他两条指向，插入完成，并返回新插入的节点
}

template <typename T> ListNodePosi(T) ListNode<T>::insertAsSucc(T const &e){ //后插入
	ListNodePosi(T) x = new ListNode(e,succ,this); //创建新节点，并初始化两条指向
	succ->pred = x; succ = x; return x; //更新其他两条指向，插入完成，并返回新插入的节点
}


/** 
 * ********************************************List实现************************************************
 **/
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



/** 
 * ********************************************BinNode实现************************************************
 **/
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



/** 
 * ********************************************BinTree实现************************************************
 **/

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
	
	FromParentTo(*x) = NULL;	//将指向此节点的指针置空
	
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


template <typename T> BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S){ //作为左子树接入
	S->_root->parent = x;  //S子树的根节点指向主树x
	x->lChild = S->_root;  //主树x的左孩子节点指向S子树根节点
	
	_size += S->_size;
	updateHeightAbove(x);
	S->_root = NULL;	S->_size = 0;
	//remove(S);
    S = NULL; 
	return x;	//返回接入节点
}


template <typename T> BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S){ //作为右子树接入
	S->_root->parent = x;		//S子树的根节点指向主树x
	x->rChild = S->_root;		//主树x的右孩子节点指向S子树根节点
	
	_size += S->_size;
	updateHeightAbove(x);
	S->_root = NULL;	S->_size = 0;
	//remove(S);
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
