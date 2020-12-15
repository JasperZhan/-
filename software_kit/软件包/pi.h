#ifndef __PI_H__
#define __PI_H__

// 文件路径名:ex1_8\alg.h 
struct Data
{	// 存储和通项共用体
	unsigned s: 4;					// 和的一位
	unsigned u: 4;					// 通项的一位
};

void Pi(int precision)
{
	ofstream outFile("pi.txt");													// 存储结果的文件
	unsigned int n = (unsigned int)((precision + 5) / log10((double)2)) + 3;	// 计算到第n项为止
	int aSize = precision + 7 + (int)log10((double)4 * n);						// 定义数组大小
	Data *dt;						// PI为dt[0].s. dt[1].s dt[2].s dt[3].s dt[3].s ... dt[aSize-1].s
									// 级数的通项dt[0].u dt[1].u dt[2].u dt[3].u ... dt[aSize-1].u  
	int a = 1, b = 3;				// 设通项为2*1/3*2/5*3/7...*a/b,第1项为2*1/3,a=1,b=3
	int i;							// 临时变量
	
	dt = new Data[aSize];			// 为dt分配存储空间
	for (i = 0; i < aSize; i++)
	{	// 将dt的各项初始化为0
		dt[i].s = dt[i].u = 0;
	}

	dt[0].s = 2;					// 级数的第0项
	dt[0].u = 2;					// 级数的第0项
	int noZeroPos = 0;				// 数组u的第一个非零元素的下标

	for (unsigned int count = 0; count <= n; count++)
	{
		// 计算u *= a;
		int c;						// 临时变量
		int d = 0;					// 此处d表示计算过程中的进位部分
		for (i = aSize - 1; i >= noZeroPos; i--)
		{
			c = dt[i].u * a + d;	// 计算乘积
			dt[i].u = c % 10;		// 第i位
			d = c / 10;				// 进位
		}
		for (i = noZeroPos - 1; i >= 0 && d > 0; i--)
		{	// u*a向高位的进位
			c = dt[i].u * a + d;	// 计算乘积
			dt[i].u = c % 10;		// 第i位
			d = c / 10;				// 进位
		}
		noZeroPos = i + 1;			// 新的通项的第一个非零元素的下标
		
		// 计算u /= b;
		d = 0;						// 此处d表示计算过程中的余数
		for (i = noZeroPos; i < aSize; i++)
		{
			c = dt[i].u + d * 10;	// 将余数d扩大10倍后加到dt[i].u 
			dt[i].u = c / b;		// 计算新的第i位
			d = c % b;				// 计算新的余数
			if (dt[i].u != 0) break;
		}

		noZeroPos = i;				// 新的数组u的第一个非零元素的下标
		for (i = noZeroPos + 1; i<aSize; i++)
		{
			c = dt[i].u + d * 10;	// 将余数d扩大10倍后加到dt[i].u 
			dt[i].u = c / b;		// 计算新的第i位
			d = c % b;				// 计算新的余数
		}

		// 计算s += u;
		d = 0;						// 此处d表示计算过程中的进位部分
		for (i = aSize - 1; i >= noZeroPos; i--)
		{
			c = dt[i].s + dt[i].u + d;	// 计算和
			dt[i].s = c % 10;		// 计算新的第i位
			d = c / 10;				// 进位
		}
		for(i = noZeroPos - 1; i >= 0; i--)
		{
			c = dt[i].s + d;		// dt.s[i] + d可能大于10,产生新的进位
			dt[i].s = c % 10;		// 计算新的第i位
			if (c / 10 == 0) break;	// 无进位
			d = c / 10;				// 进位
		}

		a++;						// 下一项的a
		b += 2;						// 下一项的b
	}

	// 将结果输出到文件pi.txt中
	outFile << dt[0].s << ".";
	for (i = 1; i < precision; i++)
	{
		outFile << dt[i].s;
	}
	outFile << endl;
	if (dt[precision].s == 9 && dt[precision + 1].s == 9 && dt[precision + 2].s == 9 && 
		dt[precision + 3].s == 9 && dt[precision + 4].s == 9 && dt[precision + 5].s == 9 ||
		dt[precision].s == 0 && dt[precision + 1].s == 0 && dt[precision + 2].s == 0 && 
		dt[precision + 3].s == 0 && dt[precision + 4].s == 0 && dt[precision + 5].s == 0)
	{	// 计算的PI最后几位可能有误差
		outFile << "最后几位可能有误差" << endl;
	}

	outFile.close();				// 关闭文件
	delete []dt;					// 释放dt
}


#endif