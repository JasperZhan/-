#ifndef __ADJ_MATRIX_DIR_GRAPH_H__
#define __ADJ_MATRIX_DIR_GRAPH_H__

#include "utility.h"							// 实用程序软件包

// 有向网的邻接矩阵类模板
template <class ElemType, class WeightType>
class AdjMatrixDirNetwork
{
protected:
// 邻接矩阵的数据成员:
	int vexNum, edgeNum;						// 顶点个数和边数
	WeightType **Matrix;						// 邻接矩阵
	ElemType *elems;							// 顶点数据
	mutable StatusCode *tag;					// 指向标志数组的指针
	WeightType infinity;						// 无穷大

// 辅助函数模板:
	void DestroyHelp();							// 销毁有向网,释放有向网占用的空间

public:
// 抽象数据类型方法声明及重载编译系统默认方法声明:
	AdjMatrixDirNetwork(ElemType es[], int vertexNum = DEFAULT_SIZE, 
		WeightType infinit = (WeightType)DEFAULT_INFINITY);	
		// 构造数据元素es[],顶点个数为vertexNum,infinit表示无穷大,边数为0的有向网
	AdjMatrixDirNetwork(int vertexNum = DEFAULT_SIZE, 
		WeightType infinit = DEFAULT_INFINITY);	
		// 构造顶点个数为vertexNum,infinit表示无穷大,边数为0的有向网
	~AdjMatrixDirNetwork();						// 析构函数模板
	StatusCode GetElem(int v, ElemType &e) const;// 求顶点的元素	
	StatusCode SetElem(int v, const ElemType &e);// 设置顶点的元素值
	WeightType GetInfinity() const;				// 返回无穷大			 
	int GetVexNum() const;						// 返回顶点个数			 
	int GetEdgeNum() const;						// 返回边数个数			 
	int FirstAdjVex(int v) const;				// 返回顶点v的第一个邻接点			 
	int NextAdjVex(int v1, int v2) const;		// 返回顶点v1的相对于v2的下一个邻接点			 
	void InsertEdge(int v1, int v2, WeightType w);	// 插入顶点为v1和v2,权为w的边			 
	void DeleteEdge(int v1, int v2);			// 删除顶点为v1和v2的边			 
	WeightType GetWeight(int v1, int v2) const;	// 返回顶点为v1和v2的边的权值
	void SetWeight(int v1, int v2, WeightType w);// 设置顶点为v1和v2的边的权值
	StatusCode GetTag(int v) const;			// 返回顶点v的标志		 
	void SetTag(int v, StatusCode val) const;	// 设置顶点v的标志为val		 
	AdjMatrixDirNetwork(const AdjMatrixDirNetwork<ElemType, WeightType> &copy);	
		// 复制构造函数模板
	AdjMatrixDirNetwork<ElemType, WeightType> &operator =
		(const AdjMatrixDirNetwork<ElemType, WeightType> &copy); // 重载赋值运算符
};

template <class ElemType, class WeightType>
void Display(const AdjMatrixDirNetwork<ElemType, WeightType> &g, bool showVexElem);// 显示邻接矩阵有向网


// 有向网的邻接矩阵类模板的实现部分
template <class ElemType, class WeightType>
AdjMatrixDirNetwork<ElemType, WeightType>::AdjMatrixDirNetwork(ElemType es[], int vertexNum, WeightType infinit)
// 操作结果：构造顶点数据为es[],顶点个数为vertexNum,infinit表示无穷大,边数为0的有向网
{
	if (vertexNum < 0)	throw Error("顶点个数不能为负!");// 抛出异常

	infinity = infinit;								// 无穷大
	vexNum = vertexNum;								// 顶点数为vertexNum
	edgeNum = 0;									// 边数为0

	elems = new ElemType[vexNum];				// 生成顶点数据标数组
	
	int u, v;									// 临时变量 
	for (v = 0; v < vexNum; v++)
	{	// 初始化标志数组
		elems[v] = es[v];
	}

	tag = new StatusCode[vexNum];					// 生成标志数组
	for (v = 0; v < vexNum; v++)
	{	// 初始化标志数组
		tag[v] = UNVISITED;
	}

	Matrix = (WeightType **)new WeightType *[vexNum];// 生成邻接矩阵
	for (v = 0; v < vexNum; v++)
	{	// 生成邻接矩阵的行
		Matrix[v] = new WeightType[vexNum];	
	}

	for (u = 0; u < vexNum; u++)
	{
		for (v = 0; v < vexNum; v++)
		{	// 为邻接矩阵元素赋值
			Matrix[u][v] = infinity;
		}
	}
}

template <class ElemType, class WeightType>
AdjMatrixDirNetwork<ElemType, WeightType>::AdjMatrixDirNetwork(int vertexNum, WeightType infinit)
// 操作结果：构造顶点个数为vertexNum,infinit表示无穷大,边数为0的有向网
{
	if (vertexNum < 0)	throw Error("顶点个数不能为负!");// 抛出异常

	infinity = infinit;								// 无穷大
	vexNum = vertexNum;								// 顶点数为vertexNum
	edgeNum = 0;									// 边数为0

	elems = new ElemType[vexNum];					// 生成顶点数据标数组
	
	int u, v;										// 临时变量 
	tag = new StatusCode[vexNum];					// 生成标志数组
	for (v = 0; v < vexNum; v++)
	{	// 初始化标志数组
		tag[v] = UNVISITED;
	}

	Matrix = (WeightType **)new WeightType *[vexNum];// 生成邻接矩阵
	for (v = 0; v < vexNum; v++)
	{	// 生成邻接矩阵的行
		Matrix[v] = new WeightType[vexNum];	
	}

	for (u = 0; u < vexNum; u++)
	{
		for (v = 0; v < vexNum; v++)
		{	// 为邻接矩阵元素赋值
			Matrix[u][v] = infinity;
		}
	}
}

template <class ElemType, class WeightType>
void AdjMatrixDirNetwork<ElemType, WeightType>::DestroyHelp()
// 操作结果：销毁有向网,释放有向网占用的空间
{
	delete []elems;										// 释放顶点数据
	delete []tag;										// 释放标志

	for (int iPos = 0; iPos < vexNum; iPos++)
	{	// 释放邻接矩阵的行
		delete []Matrix[iPos];
	}
	delete []Matrix;									// 释放邻接矩阵
}

template <class ElemType, class WeightType>
AdjMatrixDirNetwork<ElemType, WeightType>::~AdjMatrixDirNetwork()
// 操作结果：释放对象所占用空间
{
	DestroyHelp();
}

template <class ElemType, class WeightType>
StatusCode AdjMatrixDirNetwork<ElemType, WeightType>::GetElem(int v, ElemType &e) const
// 操作结果：求顶点v的元素, v的取值范围为0 ≤ v ＜ vexNum, v合法时返回
//	SUCCESS, 否则返回RANGE_ERROR
{
	if (v < 0 || v >= vexNum)
	{	// v范围错
		return NOT_PRESENT;	// 元素不存在
	}
	else
	{	// v合法
		e = elems[v];		// 将顶点v的元素值赋给e
		return ENTRY_FOUND;	// 元素存在
	}
}	

template <class ElemType, class WeightType>
StatusCode AdjMatrixDirNetwork<ElemType, WeightType>::SetElem(int v, const ElemType &e)
// 操作结果：设置顶点的元素值v的取值范围为0 ≤ v ＜ vexNum, v合法时返回
//	SUCCESS, 否则返回RANGE_ERROR
{
	if (v < 0 || v >= vexNum)
	{	// v范围错
		return RANGE_ERROR;	// 位置错
	}
	else
	{	// v合法
		elems[v] = e;		// 顶点元素
		return SUCCESS;		// 成功
	}
}

template <class ElemType, class WeightType>
WeightType AdjMatrixDirNetwork<ElemType, WeightType>::GetInfinity() const
// 操作结果：返回无穷大
{
	return infinity;
}

template <class ElemType, class WeightType>
int AdjMatrixDirNetwork<ElemType, WeightType>::GetVexNum() const
// 操作结果：返回顶点个数			 
{
	return vexNum;
}

template <class ElemType, class WeightType>
int AdjMatrixDirNetwork<ElemType, WeightType>::GetEdgeNum() const
// 操作结果：返回边数个数
{
	return edgeNum;
}		 

template <class ElemType, class WeightType>
int AdjMatrixDirNetwork<ElemType, WeightType>::FirstAdjVex(int v) const
// 操作结果：返回顶点v的第1个邻接点			 
{
	if (v < 0 || v >= vexNum) throw Error("v不合法!");		// 抛出异常

	for (int cur = 0; cur < vexNum; cur++)
	{	// 查找邻接点
		if (Matrix[v][cur] != infinity) return cur;
	}

	return -1;												// 返回-1表示无邻接点
}

template <class ElemType, class WeightType>
int AdjMatrixDirNetwork<ElemType, WeightType>::NextAdjVex(int v1, int v2) const
// 操作结果：返回顶点v1的相对于v2的下1个邻接点			 
{
	if (v1 < 0 || v1 >= vexNum) throw Error("v1不合法!");	// 抛出异常
	if (v2 < 0 || v2 >= vexNum) throw Error("v2不合法!");	// 抛出异常
	if (v1 == v2) throw Error("v1不能等于v2!");				// 抛出异常

	for (int cur = v2 + 1; cur < vexNum; cur++)
	{	// 查找邻接点
		if (Matrix[v1][cur] != infinity) return cur;
	}

	return -1;												// 返回-1表示无邻接点
}

template <class ElemType, class WeightType>
void AdjMatrixDirNetwork<ElemType, WeightType>::InsertEdge(int v1, int v2, WeightType w)
// 操作结果：插入顶点为v1和v2,权为w的边			 
{
	if (v1 < 0 || v1 >= vexNum) throw Error("v1不合法!");	// 抛出异常
	if (v2 < 0 || v2 >= vexNum) throw Error("v2不合法!");	// 抛出异常
	if (v1 == v2) throw Error("v1不能等于v2!");				// 抛出异常
	if (w == infinity) throw Error("w不能为无空大!");		// 抛出异常

	if (Matrix[v1][v2] == infinity)
	{	// 原有向网无边<v1, v2>,插入后边数自增1
		edgeNum++;
	}
	Matrix[v1][v2] = w;										// 修改边的权值
}

template <class ElemType, class WeightType>
void AdjMatrixDirNetwork<ElemType, WeightType>::DeleteEdge(int v1, int v2)
// 操作结果：删除顶点为v1和v2的边			 
{
	if (v1 < 0 || v1 >= vexNum) throw Error("v1不合法!");	// 抛出异常
	if (v2 < 0 || v2 >= vexNum) throw Error("v2不合法!");	// 抛出异常
	if (v1 == v2) throw Error("v1不能等于v2!");				// 抛出异常

	if (Matrix[v1][v2] != infinity)
	{	// 原有向网存在边<v1, v2>,删除后边数自减1
		edgeNum--;
	}
	Matrix[v1][v2] = infinity;								// 修改边的权值
}

template <class ElemType, class WeightType>
WeightType AdjMatrixDirNetwork<ElemType, WeightType>::GetWeight(int v1, int v2) const
// 操作结果：返回顶点为v1和v2的边的权值
{
	if (v1 < 0 || v1 >= vexNum) throw Error("v1不合法!");	// 抛出异常
	if (v2 < 0 || v2 >= vexNum) throw Error("v2不合法!");	// 抛出异常

	return Matrix[v1][v2];
}

template <class ElemType, class WeightType>
void AdjMatrixDirNetwork<ElemType, WeightType>::SetWeight(int v1, int v2, WeightType w)
// 操作结果：设置顶点为v1和v2的边的权值
{
	if (v1 < 0 || v1 >= vexNum) throw Error("v1不合法!");	// 抛出异常
	if (v2 < 0 || v2 >= vexNum) throw Error("v2不合法!");	// 抛出异常
	if (v1 == v2) throw Error("v1不能等于v2!");				// 抛出异常
	if (w == infinity) throw Error("w不能为无空大!");		// 抛出异常

	Matrix[v1][v2] = w;									// 修改边的权值
}

template <class ElemType, class WeightType>
StatusCode AdjMatrixDirNetwork<ElemType, WeightType>::GetTag(int v) const
// 操作结果：返回顶点v的标志		 
{
	if (v < 0 || v >= vexNum) throw Error("v不合法!");	// 抛出异常

	return tag[v];
}

template <class ElemType, class WeightType>
void AdjMatrixDirNetwork<ElemType, WeightType>::SetTag(int v, StatusCode val) const 
// 操作结果：设置顶点v的标志为val		 
{
	if (v < 0 || v >= vexNum) throw Error("v不合法!");	// 抛出异常

	tag[v] = val;
}

template <class ElemType, class WeightType>
AdjMatrixDirNetwork<ElemType, WeightType>::AdjMatrixDirNetwork(const AdjMatrixDirNetwork<ElemType, WeightType> &copy)
// 操作结果：由有向网的邻接矩阵copy构造新有向网的邻接矩阵copy——复制构造函数模板
{
	int iPos, jPos;								// 临时变量
	infinity =copy.infinity;					// 复制无穷大
	vexNum = copy.vexNum;						// 复制顶点数
	edgeNum = copy.edgeNum;						// 复制边数

	elems = new ElemType[vexNum];			// 生成顶点数据数组
	for (iPos = 0; iPos < vexNum; iPos++)
	{	// 复制顶点数据数组
		elems[iPos] = copy.elems[iPos];
	}

	tag = new StatusCode[vexNum];				// 生成标志数组
	for (iPos = 0; iPos < vexNum; iPos++)
	{	// 复制标志数组
		tag[iPos] = copy.tag[iPos];
	}

	Matrix = (WeightType **)new WeightType *[vexNum];	// 生成邻接矩阵
	for (iPos = 0; iPos < vexNum; iPos++)
	{	// 生成邻接矩阵的行
		Matrix[iPos] = new WeightType[vexNum];	
	}

	for (iPos = 0; iPos < vexNum; iPos++)
	{
		for (jPos = 0; jPos < vexNum; jPos++)
		{	// 复制邻接矩阵元素赋值
			Matrix[iPos][jPos] = copy.Matrix[iPos][jPos];
		}
	}
}

template <class ElemType, class WeightType>
AdjMatrixDirNetwork<ElemType, WeightType> &AdjMatrixDirNetwork<ElemType, WeightType>::operator =(const AdjMatrixDirNetwork<ElemType, WeightType> &copy)
// 操作结果：将有向网的邻接矩阵copy赋值给当前有向网的邻接矩阵——重载赋值运算符
{
	if (&copy != this)
	{
		DestroyHelp();						// 释放当前有向网占用空间
		int iPos, jPos;						// 临时变量
		infinity =copy.infinity;			// 复制无穷大
		vexNum = copy.vexNum;				// 复制顶点数
		edgeNum = copy.edgeNum;				// 复制边数

		elems = new ElemType[vexNum];	// 生成顶点数据数组
		for (iPos = 0; iPos < vexNum; iPos++)
		{	// 复制顶点数据数组
			elems[iPos] = copy.elems[iPos];
		}

		tag = new StatusCode[vexNum];		// 生成标志数组
		for (iPos = 0; iPos < vexNum; iPos++)
		{	// 复制标志数组
			tag[iPos] = copy.tag[iPos];
		}

		Matrix = (WeightType **)new WeightType*[vexNum];// 生成邻接矩阵
		for (iPos = 0; iPos < vexNum; iPos++)
		{	// 生成邻接矩阵的行
			Matrix[iPos] = new WeightType[vexNum];	
		}

		for (iPos = 0; iPos < vexNum; iPos++)
		{
			for (jPos = 0; jPos < vexNum; jPos++)
			{	// 复制邻接矩阵元素赋值
				Matrix[iPos][jPos] = copy.Matrix[iPos][jPos];
			}
		}
	}
	return *this;
}

template <class ElemType, class WeightType>
void Display(const AdjMatrixDirNetwork<ElemType, WeightType> &net, bool showVexElem = true)
// 操作结果: 显示邻接矩阵有向网
{
	WeightType infinity = net.GetInfinity();		// 无穷大

	for (int iPos = 0; iPos < net.GetVexNum(); iPos++)
	{	// 显示行
		if (showVexElem)
		{	// 显示顶点元素
			ElemType e;								// 数据元素
			net.GetElem(iPos, e);					// 取出元素值
			cout << e;								// 显示元素
		}

		for (int jPos = 0; jPos < net.GetVexNum(); jPos++)
		{	// 显示元素
			if (net.GetWeight(iPos, jPos) == infinity)
			{	// 显示无穷大
				cout << "\t" << "∞";
			}
			else
			{	// 显示一般权值
				cout << "\t" << net.GetWeight(iPos, jPos);
			}
		}
		cout << endl;								// 换行 
	}
}

#endif
