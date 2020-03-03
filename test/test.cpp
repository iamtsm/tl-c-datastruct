#include <iostream>

//#include "rpn.h"
//#include "stack_list.h"
//#include "stack_vector.h"
//#include "queue_list.h"
//#include "BinTree.h"

//#include "refer.h"


//#include "BitMap.h"
//#include "BitMapStack.h"


//~ #include "AVL.h"
//~ #include <time.h>
//~ #include <unistd.h>

using namespace std;


/**
 * 测试逆波兰表达式
 * */

//~ int main ( int argc, char* argv[] ) { //表达式求值（入口）
	 //~ Rpn<char> R;
	
    //~ for ( int i = 1; i < argc; i++ ) { //逐一处理各命令行参数（表达式）
		
       //~ char* rpn = ( char* ) malloc ( sizeof ( char ) * 1 );   rpn[0] = '\0'; //逆波兰表达式
       
       //~ float value = R.evaluate ( R.removeSpace ( argv[i] ), rpn ); //求值
       
       //~ cout << "value  : " << value;
       
       //~ free ( rpn );   rpn = NULL;
    //~ }
    //~ return 0;
//~ }


/**
 * 测试栈
 * */
//~ int main(){
	//~ Stack_example<char> s;
	//~ s.convert(180,16);
	//~ s.show();
//~ }


/**
 * 测试队列
 * */
//~ int main(){
	
	//~ Queue_example<char> q;
	
	//~ char s[] = {'t','s','m',':','i','s',' ','a',' ','b','o','y'};
	//~ q.init(s,12);
	//~ q.show();
	
	
	//~ q.getQueue().enqueue('a');
	//~ char p = q.getQueue().front();	//创建新的引用
	//~ cout << '\n' << p;
	//~ p = 'b';
	//~ cout << '\n' << q.getQueue().front(); //不会输出b
	
	//~ q.getQueue().front() = 'c';  //返回的引用
	
	//~ cout << '\n' << q.getQueue().front(); //输出c 
//~ }


//~ int  sum(int A[], int & len, int s) {
    //~ if (len-- == 1)   return s + A[0];	//递归基
    //~ return sum(A, len, s + A[len]);
//~ }

//~ int main(){
	
	//~ BinTree<char> B;
	//~ BinNode<char> *r = B.insertAsRoot('a');

	//~ B.insertAsRC(r,'3');
	//~ B.insertAsLC(r,'2');
	
	//~ r->travPre(r);
	
	//~ return 0;
//~ }



/**
 * bitmap 测试
 * */
//~ int main(void){
	
	//~ BitMap<int> B;
	
	//~ B.set(0);B.set(10);B.set(9);B.set(31);
	//~ B.set(32);B.set(45);B.set(63);
	//~ B.set(64);B.set(78);B.set(96);
	//~ B.set(127);
	//~ //需要扩容
	//~ B.set(128);B.set(129);
	//~ B.show();B.clear(10); B.show();
	//~ if(B.test(127)){cout << "\n存在";}
	//~ else{cout << "\n不存在";}

	//~ BitMap<int> B;
	//~ B.set('a' - '0');
	//~ cout << 'a' - '0' << "\n";
	//~ B.show();
	//~ if(B.test('b' - '0')){
		//~ cout << "\n存在";
	//~ }else{
		//~ cout << "\n不存在";
	//~ }
	
	
	//~ return 0;
//~ }



/**
 * AVL树测试
 * */
 //~ int main(void){
	 
	 //~ AVL<int> avl;
	
	 //~ int count = 10000;
	 //~ clock_t start = clock();
	 //~ avl.test_insert(count);	//测试插入
	 //~ clock_t end = clock();
	 //~ cout << "当前插入 " << count << " 个节点" << endl;
	 //~ printf("运行时间 : %f 秒\n",(float)(end-start)/CLOCKS_PER_SEC);
	 
	 //~ avl.travIn();
	 
	 //~ cout << "\n\n\n";
	 
	 
	 //~ count = 20000;
	 //~ start = clock();
	 //~ avl.test_remove(count);	//测试删除
	 //~ end = clock();
	 //~ cout << "当前删除 " << count << " 个节点" << endl;
	 //~ printf("运行时间 : %f 秒\n",(float)(end-start)/CLOCKS_PER_SEC);
	 
	 //~ cout << "\n\n\n";
	 
	 
	 //~ count = 30000;
	 //~ start = clock();
	 //~ avl.test_insert(count);	//测试插入
	 //~ end = clock();
	 //~ cout << "当前插入 " << count << " 个节点" << endl;
	 //~ printf("运行时间 : %f 秒\n",(float)(end-start)/CLOCKS_PER_SEC);
	 
	 //~ cout << "\n\n\n";
	 //~ usleep(100);
	 
	 
	 //~ count = 30000;
	 //~ start = clock();
	 //~ avl.test_remove(count);	//测试删除
	 //~ end = clock();
	 //~ cout << "当前删除 " << count << " 个节点" << endl;
	 //~ printf("运行时间 : %f 秒\n",(float)(end-start)/CLOCKS_PER_SEC);
	
	 
	 //~ return 0;
 //~ }



/**
 * 二叉堆测试
 * */
 
 #include "PQ.h"
 int main(){
	 
	 //~ char x[] = {'5','3','4','1','2'};
	 char x[] = {'3','4','2','1','7','6','8','5','9'};
	 
	 PQ_comlHeap<char> p(x,10);
	 
	 //~ p.insert('8');
	 	 
	 //~ p.delMax();
	 //~ p.delMax();
	 
	 
	 p.show();
	 
	 return 0;
 }
 






