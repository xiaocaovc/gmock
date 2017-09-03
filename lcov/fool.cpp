/*************************************************************************
	> File Name: fool.cpp
	> Author: zhongxin.cao
	> Mail: zhongxin.cao@pt.sony.cn 
	> Created Time: 2015年03月01日 星期日 14时46分37秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
using namespace std;
#include "gtest/gtest.h" 
#include "gmock/gmock.h"
#include "fool.h"
using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::MakePolymorphicAction;
using ::testing::MakeAction;
using ::testing::NiceMock;
using ::testing::PolymorphicAction;
using ::testing::Action;
using ::testing::ActionInterface;

class Foo
{
public:
	virtual ~Foo() {}
	virtual char DoThis(int n) = 0;
	virtual void DoThat(const char* s,int* p) = 0;
};
class FakeFoo :public Foo 
{
public:
	virtual char DoThis(int n)
	{
	int a=1;
	if(n==0)
	{
	a=0;
	}
	else
	{
	a=2;
	}
		return (n > 0) ?'+' :
			(n < 0) ? '-' :'0';
	}

	virtual void DoThat(const char* s,int* p) 
	{
		*p = strlen(s);
	}
};
class MockFoo :public Foo 
{
public:
	// Normal mock method definitions using Google Mock.
	MOCK_METHOD1(DoThis, char(int n));
	MOCK_METHOD2(DoThat, void(const char* s,int* p));

	// Delegates the default actions of the methods to a FakeFoo object.
	// This must be called *before* the custom ON_CALL() statements.
	void DelegateToFake() 
	{
		ON_CALL(*this, DoThis(_))
			.WillByDefault(Invoke(&fake_, &FakeFoo::DoThis));
		ON_CALL(*this, DoThat(_, _))
			.WillByDefault(Invoke(&fake_, &FakeFoo::DoThat));
	}
private:
	FakeFoo fake_; // Keeps an instance of the fake in the mock.
			  
};

// mock和fake混合
TEST(MockMyBaseq,test)
{
	MockFoo ba;
	ba.DelegateToFake();
	EXPECT_CALL(ba, DoThis(_));
	EXPECT_EQ('+', ba.DoThis(5));
	EXPECT_CALL(ba, DoThis(_))
	.WillOnce(Return('-'));
	EXPECT_EQ('-', ba.DoThis(5));
}

int main(int argc, char** argv)
 {
  std::cout << "Running main() from gmock_main.cc\n";
  // Since Google Mock depends on Google Test, InitGoogleMock() is
  // also responsible for initializing Google Test.  Therefore there's
  // no need for calling testing::InitGoogleTest() separately.
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}