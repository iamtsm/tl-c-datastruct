#include "ListNode_implement.h"



typedef int Rank;
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
