#include <cstdio>
#include "StackL.h"
using namespace std;

typedef enum {UNDISCOVERED,DISCOVERED,VISITED} VStatus;	//顶点(节点)状态 (未发现点，发现点，已经访问点)
typedef enum {UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD} EStatus; //边状态,(未确定边，树边，跨边，向前边，向后边)
#define INT_MAX 2^20

/**
 * 顶点对象
 * */
template <typename Tv> struct Vertex{
	Tv data;	int inDegree; int outDegree; VStatus status;	//数据，入度出度，状态
	int dTime;  int fTime;	//时间标签
	int parent; int priority;	//父节点，优先级数
	
	Vertex(Tv const&d = (Tv) 0):
		data(d),inDegree(0),outDegree(0),status(UNDISCOVERED),
		dTime(-1),fTime(-1),parent(-1),priority(INT_MAX){}
};

/**
 * 边对象
 * */
template <typename Te> struct Edge { 
	Te data; int weight; EStatus status; //数据、权重、状态
	 
	Edge(Te const& d, int w):
		data(d), weight(w), status(UNDETERMINED) {}
};


/**
 * 这里的图是由顶点和边组成的，所以
 * */
template <typename Tv,typename Te> class Graph{
	private : 
		void reset(){ //所有顶点，边的辅助信息复位
			for(int i = 0; i < n; i++){
				status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1;	//顶点状态，时间标签
				parent(i) = -1; priority(i) = INT_MAX;	//顶点父节点，优先级数
				for(int j = 0; j < n; j++){ //边复位
					if(exists(i,j)) status(i,j) = UNDETERMINED;	//边状态
				}
			}
		}
		
		void bfs(int v, int & clock);
		void dfs(int v, int & clock);
		
		void BFS(int v,int & clock);	//(连通域) 广度优先搜索算法 [类似层次遍历]
		void DFS(int v,int & clock);	//深度优先搜索算法
		
		void BCC(int v,int& clock, Stack<int> &S);	//基于DFS的双连通分量分解算法
		bool TSort(int v, int & clock, Stack<Tv> *S); //基于DFS的拓扑排序算法
		template <typename PU> void PFS(int ,PU);	//优先级搜索框架
	
	public : 
		int n; //顶点数量	
		int e; //边数量
		/**
		 * 顶点操作相关
		 * */
		virtual int insert(Tv const &tv) = 0;	//插入顶点，返回其编号
		virtual Tv remove(int v) = 0;	//删除顶点，包括其关联的边信息，返回该顶点
		virtual Tv& vertex(int v) = 0;  //获取该顶点的数据
		virtual int inDegree(int v) = 0;	//获取该顶点的入度
		virtual int outDegree(int v) = 0;	//获取该顶点的出度
		virtual int firstNbr(int v) = 0;	//获取该顶点的首个邻接顶点
		virtual int nextNbr(int v,int j) = 0;	//获取该顶点v的下一个(相对于顶点j)邻接顶点
		virtual VStatus& status(int v) = 0;	//获取顶点的状态
		virtual int& dTime(int v);		//获取顶点的时间标签dTime
		virtual int& fTime(int v); 		//获取顶点的时间标签fTime
		virtual int& parent(int v);		//获取顶点的父节点
		virtual int& priority(int v);	//获取顶点的优先级数
		
		/**
		 * 边操作相关
		 * */
		virtual bool exists(int v, int u) = 0; //边(v, u)是否存在
		virtual void insert(Te const& te, int v, int u, int w) = 0; //在顶点v和u之间插入权重为w的边e
		virtual Te remove(int v, int u) = 0; //初除顶点v和u之间的边e，返回该边信息
		virtual EStatus& status(int v, int u) = 0; //边(v, u)的状态
		virtual Te& edge(int v, int u) = 0; //边(v, u)的数据(该边的确存在)
		virtual int& weight(int v, int u) = 0; //边(v, u)的权重
};


/**
 * 连通域遍历
 * 整体的思想就是从某个节点出发，每次寻找一个邻接节点，
 * 如果找到的邻接节点是首次发现，则入队，标记边为TREE,顺便改变节点状态为发现，等等操作..
 * 如果找到的邻接节点不是首次发现，标记边为CROSS
 * */
template <typename Tv,typename Te> void Graph<Tv,Te>::BFS(int v,int & clock){
	Queue<int> Q;	//因为类似层次遍历，依然引入一个队列
	status(v) = DISCOVERED;		//标记当期节点为发现状态
	Q.enqueue(v);	//入队
	
	//类似树的层次遍历
	while(!Q.empty()){
		int v = Q.dequeue();		//队首出队
		dTime(v) = ++clock;			//设置发现时间
		
		for(int u = firstNbr(v); -1 < u; u = nextNbr(v,u)){	//不断寻找v节点的邻接节点,相当于在E[v][0~n]中寻找
			if(status(u) == UNDISCOVERED){	//如果节点u状态是未发现
				status(u) = DISCOVERED;	//标记此节点
				Q.enqueue(u);	//并入队
				status(v,u) = TREE;	//标记v和u之间的边是树边
				parent(u) = v;	//节点v是u的父节点
			}else{ //如果是其他两中状态(发现或者已经访问)
				status(v,u) = CROSS;	//标记v和u之间的边为跨边
			}
		}
		status(v) = VISITED;	//访问完该节点(此节点在循环开始时出队)
	}
}
/**
 * 图遍历, 广度优先遍历
 * */
template <typename Tv,typename Te> void Graph<Tv,Te>::bfs(int v,int & clock){
	reset();  int v = s; int clock = 0;
	
	do{
		if(status(v) == UNDISCOVERED){	//对未发现的节点进行BFS遍历, 有多少个连通域就会调用几次BFS,每次调用BFS会遍历整个连通域
			BFS(v, clock);
		}
	}while(s != (v = (++v%n)));		//这个的意思就是在合法区间内进行循环
	
}

/**
 * 连通域遍历
 * */
template <typename Tv,typename Te> void Graph<Tv,Te>::DFS(int v,int & clock){
	dTime(v) = ++clock; status(v) = DISCOVERED;	//当前顶点状态为发现
	
	for(int u = firstNbr(v); -1 < u; u = nextNbr(v,u)){
		switch(status(v)){
			case UNDISCOVERED : //如果是未发现节点
				status(u,v) = TREE;		//标记边为树边
				parent(u) = v;
				DFS(u,clock);	//递归深入寻找其他的未访问节点。
				break;	
			case DISCOVERED : 	//如果是发现节点
				status(v,u) = BACKWARD;	//标记边为向后边 
				break;　 
			default : 	//如果是已经访问过的节点，
				status(v,u) = (dTime(v) < fTime(u)) ? FORWARD : CROSS;	 //根据发现时间和访问时间大小，标记为向前边或者跨越边
				break;
		}
	}
	status(v) = VISITED; fTime(v) = ++clock;	//标记v节点访问过 
}
/**
 * 图遍历, 深度优先遍历
 * */
template <typename Tv,typename Te> void Graph<Tv,Te>::dfs(int v,int & clock){
	reset();  int v = s; int clock = 0;
	
	do{
		if(status(v) == UNDISCOVERED){
			DFS(v, clock);
		}
	}while(s != (v = (++v%n)));
	
}

