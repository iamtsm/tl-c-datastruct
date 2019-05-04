#include "Graph_definition.h"
#include "Vector.h"

template <typename Tv, typename Te> class GraphMatrix : public Graph<Tv,Te>{
	
	private : 
		Vector< Vertex<Tv> >	V;	//顶点集合(n个,一维向量)
		Vector< Vector< Edge<Te>* > >	E;	//边集合(n*n个,二维向量)
		
	public : 
		GraphMatrix(){ n = e = 0; }	
		~GraphMatrix(){
			for(int j = 0; j < n; j++){
				for(int k = 0; k < n; k++){
					delete E[j][k];		//清除动态申请的边记录
				}
			}
		}
		
		/**
		 * 顶点操作相关
		 * */
		Tv & vertex(int i){ return V[i].data; }	//返回顶点数据
		int inDegree(int i){ return V[i].inDegree; }	//返回入度
		int outDegree(int i){ return V[i].outDegree; }	//返回出度
		VStatus &status(int i){ return V[i].status; }	//返回状态
		int &dTime(int i){ return V[i].dTime; }	//返回时间标签dTime
		int &fTime(int i){ return V[i].fTime; }	//返回时间标签fTime
		int &parent(int i){ return V[i].parent; }	//在遍历树中的父亲节点
		int &priority(int i){ return V[i].priority; }	//优先级
		
		//获取当前枚举节点的下一个邻接节点
		int nextNbr(int i, int j){	
			while( (-1 < j) && !exists(i,--j) );	//逆序查找
			return j;
		}
		
		//获取头部关联节点的第一个邻接节点
		int firstNbr(int i){　nextNbr(i, n);　}
		
		//插入顶点, 返回顶点编号
		int insert(Tv const& vertex){	
			for(int j = 0; j < n; j++){	 //n为当前顶点数
				//E[j]是一个向量, 调用的是Vector的insert接口，往向量末端添加一个新元素
				//因为其他所有顶点可能会和新顶点有对应的边关联，所以预先插入NULL.(代表扩充E[0-n][+1]或者说扩充"行向量")
				E[j].insert(NULL);	//或者理解为给邻接矩阵往末端加了一列
			}
			n++;	//顶点数量增加
			
			//E是一个二维向量， 代表扩充E[e+1],也就是把上面的for循环扩充的向量数据，进行向量扩容
			E.insert(Vector< Edge<Te>* >(n, n, NULL));	//或者理解为给邻接矩阵末端加了一行
			
			return V.insert(Vertex<Tv>(vertex));	//插入顶点到顶点集
		}
		
		//删除顶点，返回顶点信息
		Tv remove(int i){	
			for(int j = 0; j < n; j++){
				if(exists(i, j)){	//删除所有出边
					delete E[i][j];	V[j].inDegree--;	//删除的是E[i][0~n] 相当于删除清空E[i]中的所有数据，E[i]为是一个向量
				}
			}
			E.remove(i);	n--;	//删除第i行 , 也就是E[i] ,或者理解为删除邻接矩阵的一行
			
			for(int j = 0; j < n; j++){
				if(exists(j,i)){	//删除所有入边
					delete E[j].remove(i);	V[j].outDegree--;	//删除的是E[0~n][i] , 相当于清除E[0~n]中每个向量的最后一个元素
				}
			}//或者可以理解为，删除邻接矩阵的最后一列
			
			Tv vBak = vertex(i);	//备份顶点信息
			V.remove(i);	//从顶点集删除顶点i
			return vBak;
		}
		
		
		
		
		
		/**
		 * 边操作相关
		 * */
		bool exists(int i, int j){　//判断边是否存在
			return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL;
		}
		Te & edge(int i, int j){	//获取边[i][j]的数据,前提是边存在
			return E[i][j]->data;
		}
		EStatus & status(int i, int j){ //获取边[i][j]的状态, 前提是边存在
			return E[i][j]->status;
		}
		int & weight(int i, int j){	//获取边[i][j]的权重, 前提是边存在
			return E[i][j]->weight;
		}
		void insert(Te const& edge, int w, int i, int j){ //插入一条边
			if(exists(i,j)) return ;
			
			E[i][j] = new Edge<Te>(edge,w); //插入边到E[i][j]
			
			e++;	//更新边数量
			V[i].outDegree++;	//增加关联顶点i出度
			V[j].inDegree++;	//增加关联顶点j入度
		}
		Te remove(int i, int j){	//删除一条边
			Te eBak = edge(i, j);	//备份待删除边信息，约定返回
			
			delete E[i][j]; E[i][j] = NULL;	//删除边
			
			e--;	//更新边数量
			V[i].outDegree--; //减少关联顶点i出度
			V[j].inDegree--;	//减少关联顶点i出度
			
			return eBak;
		}
		 
		 
}
