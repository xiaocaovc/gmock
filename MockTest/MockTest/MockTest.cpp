// MockTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "gtest\gtest.h" 
#include "gmock\gmock.h"
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


class CMockTest
{
public:
	MOCK_CONST_METHOD1(funa,int(int a));
	int funa2(int a)
	{
		return 2;
	}
};
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
class MyBase
{
public:
	int fun(int a)
	{
		return a;
	}
	char DoThis(int n)
	{
		return (n > 0) ?'+' :
			(n < 0) ? '-' :'0';
	}
};
class MockMyBase
{
public:
	MOCK_METHOD1(DoThis, int(int n));
	MOCK_METHOD1(fun, int(int n));
	void DelegateToFake() 
	{
		ON_CALL(*this, DoThis(_))
			.WillByDefault(Invoke(&Base_, &MyBase::DoThis));
		ON_CALL(*this, fun(_))
			.WillByDefault(Invoke(&Base_, &MyBase::fun));
	}
private:
	MyBase Base_;
};
TEST(we,wer)
{
	NiceMock<CMockTest> mock_foo;
	ON_CALL(mock_foo,funa(_))
	.WillByDefault(Return(1));
	int aab=mock_foo.funa(1);
	EXPECT_CALL(mock_foo,funa(_))
	.Times(AtLeast(1))
	.WillOnce(Return(2))
	.WillRepeatedly(Return(4));
	aab=mock_foo.funa(1);
	aab=mock_foo.funa(1);
	aab=mock_foo.funa2(3);
}
TEST(AbcTest, Xyz)
{
	MockFoo foo;
	foo.DelegateToFake(); // Enables the fake for delegation.

	// Put your ON_CALL(foo, ...)s here, if any.

	// No action specified, meaning to use the default action.
	EXPECT_CALL(foo, DoThis(_))
		.WillOnce(Return('-'));
	EXPECT_CALL(foo, DoThat(_, _));

	int n = 0;
	EXPECT_EQ('-', foo.DoThis(5)); // FakeFoo::DoThis() is invoked.
	//EXPECT_EQ('+', foo.DoThis(5)); // FakeFoo::DoThis() is invoked.
	foo.DoThat("Hi", &n);          // FakeFoo::DoThat() is invoked.
	EXPECT_EQ(2, n);
}
// mock和fake混合
TEST(MockMyBaseq,test)
{
	MockMyBase ba;
	ba.DelegateToFake();
	EXPECT_CALL(ba, DoThis(_));
	EXPECT_EQ('+', ba.DoThis(5));
	EXPECT_CALL(ba, DoThis(_))
	.WillOnce(Return('-'));
	EXPECT_EQ('-', ba.DoThis(5));

	EXPECT_CALL(ba, fun(_));
	EXPECT_EQ(5, ba.fun(5));

}
int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleMock(&argc, argv);
	RUN_ALL_TESTS();  
	system("pause");	
	return 0;
}

