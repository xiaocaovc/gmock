// MyTest.cpp : 定义控制台应用程序的入口点。
//---gtest参数化测试----------

#include "stdafx.h"
#include<math.h>
#include "gtest\gtest.h" 
#include "gmock\gmock.h"

using ::testing::internal::FilePath;  
using ::testing::Message;

// ---------判断是否为质数------
bool IsPrime(int n)
{
	int i,m;
	m=(int)sqrt((float)n);
	for(i=2;i<=m;i++)
		if(!(n%i))/*如果整除则不为质数*/
			return false;
	return true;
}
class IsPrimeParamTest : public::testing::TestWithParam<int>
{
};
TEST_P(IsPrimeParamTest, Negative)
{
	int n =  GetParam();
	EXPECT_TRUE(IsPrime(n));
}
INSTANTIATE_TEST_CASE_P(TrueReturn, IsPrimeParamTest, testing::Values(4, 5, 11, 23, 17));
int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();  
	system("pause");
	return 0;
}

