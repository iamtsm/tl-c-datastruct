
#define DEFAULT_CAPACITY  3
typedef int Rank;

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

