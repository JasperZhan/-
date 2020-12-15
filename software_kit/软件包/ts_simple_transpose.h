#ifndef __SIMPLE_TRANSPOSE_H__
#define __SIMPLE_TRANSPOSE_H__

#include "tri_sparse_matrix.h"		// 稀疏矩阵的三元组顺序表

template<class ElemType>
void SimpleTranspose(const TriSparseMatrix<ElemType> &source, TriSparseMatrix<ElemType> &dest)
// 操作结果：将稀疏矩阵source转置成稀疏矩阵dest
{
	dest.rows = source.cols;							// 行数
	dest.cols = source.rows;							// 列数
	dest.num = source.num;								// 非零元素个数
	dest.maxSize = source.maxSize;						// 最大非零元素个数
	delete []dest.triElems;								// 释放存储空间
	dest.triElems = new Triple<ElemType>[dest.maxSize];	// 分配存储空间

	if (dest.num > 0)
	{
		int destPos = 0;								// 稀疏矩阵dest的下一个三元组的存放位置
		for (int col = 1; col <= source.cols; col++)
		{	// 转置前的列变为转置后的行
			for (int sourcePos = 0; sourcePos < source.num; sourcePos++)
			{	// 查找第col列的三元组
				if (source.triElems[sourcePos].col == col)
				{	// 找到第col列的一个三元组,转置后存入dest
					dest.triElems[destPos].row = source.triElems[sourcePos].col;	// 列变行
					dest.triElems[destPos].col = source.triElems[sourcePos].row;	// 行变列
					dest.triElems[destPos].value = source.triElems[sourcePos].value;
						// 非零元素值不变
					destPos++;														// 新下一个元素的存放位置
				}
			}
		}
	}
}

#endif

