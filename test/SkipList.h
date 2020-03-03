#include "List.h" //引入列表
#include "Entry.h" //引入词条
#include "Quadlist.h" //引入Quadlist
#include "Dictionary.h" //引入词典


//key、value
template <typename K, typename V> class Skiplist : 
	public Dictionary<K, V>, public List<Quadlist<Entry<K, V>>*> {  //符合Dictionary接口的Skiplist模板类（但隐含假设元素之间可比较大小）
	protected:
	    bool skipSearch (
			ListNode<Quadlist<Entry<K, V>>*>* &qlist,
			QuadlistNode<Entry<K, V>>* &p, 
			K& k );
	
	public:
		int size() const { return empty() ? 0 : last()->data->size(); } //底层Quadlist的规模
		int level() { return List::size(); } //层高 == #Quadlist，不一定要开放
		bool put ( K, V ); //插入（注意与Map有别——Skiplist允许词条重复，故必然成功）
		V* get ( K k ); //读取
		bool remove ( K k ); //删除
};
