#include <iostream>
using namespace std;
typedef unsigned int Rank;

//不使用bit位实现，换用栈结构实现，可以在O(1)的时间定位位置，设置标记，测试存在与否

template <typename T> class BitMapStack{
	private :
	Rank *S; Rank top;  //T数组用于set数据入栈，数据可以是模板类型
	Rank *F; Rank N;	//F数组存放 被标记数据在T栈中的秩(下标)
	Rank fsize;	 //F大小
	
	protected : 
	inline bool valid(Rank r){return (0 <= r) && (r < top);} //确保下标不越界 r就是F[n]中的数据
	
	inline bool erased(Rank k){return valid(F[k]) && !(S[F[k]] + 1 + k);}
	
	public :
	BitMapStack(Rank n = 8){
		N = n; F = new Rank[N]; S = new Rank[N]; top = 0; fsize = 0;
	}
	~BitMapStack(){delete []F,delete []S;}
	
	inline bool test(Rank k){
		return valid(F[k]) && (k == S[F[k]]);	//下标不越界且栈中数据相等
	}
	
	inline void set(Rank k){
		if(test(k)) return ;
		if(!erased(k)) F[k] = top++;
		S[F[k]] = k;
		fsize += 1;
	}
	
	inline void clear(Rank k){
		if(test(F[k])) S[F[k]] = -1-k;
		fsize += 1;
	}
	
	void show(){
		cout << "F[i] : ";
		for(Rank i = 0; i < fsize; i++){
			cout << F[i] << ",";
		}
		cout << "\n\nS[F[i]] : ";
		for(Rank i = 0; i < fsize; i++){
			cout << S[F[i]] << ",";
		}
		cout << "\n";
	}	
	
};
