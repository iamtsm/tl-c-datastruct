
/**
 * 词条模板类
 * */
template <typename K, typename V> struct Entry{
	
	K key;	//关键码
	V value;	//数值
	
	Entry(K k = K(), V v = V()) : key(v),value(v){};	//默认构造函数,通过调用K,V的构造函数初始化Entry
	Entry(Entry<K,V> const &e) : key(e.key), value(e.value){};	//传入一个enrty对象进行构造
	
	/**
	 * 比较器, 判等器
	 * */
	bool operator < (Entry<K,V> const& e){ return key < e.key; }	//是否小于
	bool operator > (Entry<K,V> const& e){ return key > e.key; }	//是否大于
	bool operator == (Entry<K,V> const& e){ return key == e.key; }	//是否等于
	bool operator != (Entry<K,V> const& e){ return key != e.key; }	//是否不等

};


