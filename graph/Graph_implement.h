
#include "Graph_definition.h"

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
template <typename Tv,typename Te> void Graph<Tv,Te>::bfs(int s){
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
template <typename Tv,typename Te> void Graph<Tv,Te>::dfs(int s){
	reset();  int v = s; int clock = 0;
	
	do{
		if(status(v) == UNDISCOVERED){
			DFS(v, clock);
		}
	}while(s != (v = (++v%n)));
	
}
