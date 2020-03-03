#include <iostream>
using namespace std;
typedef int Rank;


template <typename T> class BitMap{
	
	private : 
	T *elem; //数据域
	Rank _size; Rank eSize; Rank N; //数组大小, 元素bit大小,总共需要bit空间大小
	
	public :
	 
	BitMap(Rank s = 4){	//s代表数组大小
		_size = s;
		elem = new T[_size];
		N = _size * sizeof(T) * 8;
		eSize = N / _size;
		cout << "初始化完成\n" 
			 << "sizeof(T) : " << sizeof(T) << "\n"
			 << "数组大小 : " << _size << "\n" 
			 << "数组元素bit大小 : " << eSize << "\n"
			 << "数组总共bit大小 : " << N << "\n";
	}
	~BitMap(){if(elem) {delete elem;elem = NULL;_size = 0;}}

	int expand(T n);

	void set(T n);
	bool test(T n);
	void clear(T n);
	void dectobin(unsigned int n);
	
	void show(){
		cout << "10进制 : ";
		for(Rank i = 0; i < _size; i++){
			cout << elem[i] << ",";
		}
		cout << "\n";
		
		cout << "2进制 : ";
		for(Rank i = 0; i < _size; i++){
			dectobin(elem[i]);
			cout << ",";
		}
		cout << "\n";
	}
	
};

template <typename T> int BitMap<T>::expand(T n){
	if((n / eSize) < _size) return 0; //如果该数值定位到的下标 超过数组大小
	
	cout << "\n开始扩容\n";
	T *oldElem = elem;	Rank oldSize = _size;  
	
	_size = n << 1;	//扩容一倍
	elem = new T[_size];
	N = _size * sizeof(T) << 3;
	eSize = N / _size;
	for(Rank i = 0; i < oldSize; i++){
		elem[i] = oldElem[i];
	}
	delete oldElem;
	
	return 1;
}

template <typename T> void BitMap<T>::set(T n){
	//先将带插入数值定位下标(n/sizeof(T) << 3)，再定位余数在某个T类型的bit位置(sizeof(T) >> (n % sizeof(T) << 3))，最后做或运算即可
	//~ cout << "下标 ：" << n / eSize << "\n";
	//~ cout << "余数 : " << n % eSize << "\n";
	//~ cout << "右移 : " << (0x80000000 >> (n % eSize)) << "\n";

	expand(n);
	elem[ n / eSize] |= (0x80000000 >> (n % eSize));
}

template <typename T> bool BitMap<T>::test(T n){
	expand(n);
	return elem[ n / eSize] & (0x80000000 >> (n % eSize));
} 

template <typename T> void BitMap<T>::clear(T n){
	expand(n);
	elem[ n / eSize] &= ~(0x80000000 >> (n % eSize));
}


template <typename T> void BitMap<T>::dectobin(unsigned int n){
	if(n/2>0){
		dectobin(n/2);
		printf("%d",n%2);
	}else{
		printf("%d",n);
	}
}




