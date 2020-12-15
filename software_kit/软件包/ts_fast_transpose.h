#ifndef __SIMPLE_TRANSPOSE_H__
#define __SIMPLE_TRANSPOSE_H__

#include "tri_sparse_matrix.h"		// 稀疏矩阵的三元组顺序表

template<class ElemType>
void FastTranspose(const TriSparseMatrix<ElemType> &source, TriSparseMatrix<ElemType> &dest)
// 操作结果：将稀疏矩阵source转置成稀疏矩阵dest
{
	dest.rows = source.cols;							// 行数
	dest.cols = source.rows;							// 列数
	dest.num = source.num;								// 非零元素个数
	dest.maxSize = source.maxSize;						// 最大非零元素个数
	delete []dest.triElems;								// 释放存储空间
	dest.triElems = new Triple<ElemType>[dest.maxSize];	// 分配存储空间

	int *cNum = new int[source.cols + 1];				// source每一列的非零元个数
	int *cPos = new int[source.cols + 1];				// source每一列的第一个非零元在dest中的存储位置


	if (dest.num > 0)
	{
		for (int col = 1; col <= source.cols; col++) cNum[col] = 0;	// 初始化cNum
		for (int sourcePos = 0; sourcePos < source.num; sourcePos++)
			++cNum[source.triElems[sourcePos].col];		// 统计source每一列的非零元个数
		cPos[1] = 0;									// 第一列的第一个非零元在dest存储的起始位置
		for (col = 2; col <= source.cols; col++)
		{	// 循环求每一列的第一个非零元在dest存储的起始位置
			cPos[col] = cPos[col - 1] + cNum[col - 1];	
		}

		for (sourcePos = 0; sourcePos < source.num; sourcePos++)
		{	// 循环遍历source的三元组
			int destPos = cPos[source.triElems[sourcePos].col];	
				// 用于表示dest当前列的下一个非零元三元组的存储位置
			dest.triElems[destPos].row = source.triElems[sourcePos].col;		// 列变行
			dest.triElems[destPos].col = source.triElems[sourcePos].row;		// 行变列
			dest.triElems[destPos].value = source.triElems[sourcePos].value;	// 非零元素值不变
			++cPos[source.triElems[sourcePos].col];				
				// dest当前列的下一个非零元三元组的存储新位置
		}
	}

	delete []cNum;										// 释放cNum
	delete []cPos;										// 释放cPos
}



#endif

