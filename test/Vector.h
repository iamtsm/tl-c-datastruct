

typedef int Rank;
#define DEFAULT_CAPACITY  3 

template <typename T> class Vector{ //向量模板类
	protected:
	
	Rank _size; Rank _capacity; char *_elem; 							//规模 容量　数据区
	void copyFrom(T const* A,Rank lo,Rank hi);						    //复制数组区间[lo,hi]
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
