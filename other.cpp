#include <iostream>
#include <stdarg.h>
#include "cppHeader.h"
using namespace std;

void RemoveDuplicates(char str[]) {
	if (NULL == str)
		return;
	int length = strlen(str);
	if (length < 2)
		return;

	bool flags[256] = {false};
	flags[str[0]] = true;
	int tail = 1;
	for (int i = 1; i < length; ++i) {
		if (!flags[str[i]]) {
			str[tail] = str[i];
			++tail;
			flags[str[i]] = true;
		}

	}
	str[tail] = '\0';
}

void test (char *str) {
	printf("%s\t", str);
	RemoveDuplicates(str);
	printf("%s\n", str);
}
//int main() {
//	char *str = NULL;
//	test(str);
//
//	char str0[] = "a";
//	test(str0);
//
//	char str1[] = "abcd";
//	test(str1);
//
//	char str2[] = "";
//	test(str2);
//
//	char str3[] = "aaa";
//	test(str3);
//
//	char str4[] = "aaabbbcccc";
//	test(str4);
//
//	char str5[] = "ababa1232";
//	test(str5);
//	return 0;
//}

class INT {
private:
	int m;
public:
	INT(int i): m(i) {}
	int& operator*() const {
		return (int&)m;
	}

	const INT operator++(int) {
		INT temp(*this);
		++(*this);
		return temp;
	}

	INT& operator++() {
		++(this->m);
		return *this;
	}

	friend ostream& operator<< (ostream& os, const INT& i);
};

ostream& operator<< (ostream& os, const INT& obj) {
	os << '[' << obj.m << ']';
	return os;
}

//int main() {
//	INT test(1);
//	cout << *test << endl;
//	cout << test++ << endl;
//	cout << ++test << endl;
//	int a[] = {1,2,3,4,5,6,7,8,9,10};
//	vector<int> ivec(a, a + sizeof(a) / sizeof(int));
//	list<int> ilist;
//
//	vector<int>::reverse_iterator rstart, rend;
//	rstart = ivec.rbegin();
//	for (int i = 0; i < 3; ++i)
//		++rstart;
//	rend = rstart;
//	for (int i = 0; i < 5; ++i)
//		++rend;
//	copy(rstart, rend, inserter(ilist, ilist.begin()));
//
//	list<int>::iterator liter = ilist.begin();
//	while (liter != ilist.end())
//		cout << *liter++ << " ";
//	cout << endl;
//	vector<int>::reverse_iterator riter = ivec.rbegin();
//	while (riter != ivec.rend())
//		cout << *riter++ << " ";
//	cout << endl;
//
//	vector<int>::iterator iter = ivec.end();
//	do {
//		cout << *--iter << " ";
//	}while (iter != ivec.begin());
//	cout << endl;
//}
//int main() {
//	vector<string> svec;
//	svec.push_back("a23");
//	svec.push_back("123");
//	svec.push_back("12");
//	svec.push_back("01234");
//	svec.push_back("000");
//	stable_sort(svec.begin(), svec.end(), isShort);
//
//	vector<string>::iterator iter = svec.begin();
//	while (iter != svec.end())
//		cout << *iter++ << " ";
//	cout << endl;
//}
//template<typename T>
//class MakeFinal {
//	friend T; //class FinalClss;
//private:
//	MakeFinal() {}
//	~MakeFinal() {}
//};
//
//class FinalClass: virtual public MakeFinal<FinalClass> {
//public:
//	FinalClass() {}
//	~FinalClass() {}
//};

//class MakeFinal {
//	friend class FinalClass;
//private:
//	int a;
//	MakeFinal() {}
//	~MakeFinal() {}
//};
//
////FinalClass是一个不能被继承的类，同时，可以在
////堆和栈上产生该类的实例
//class FinalClass: virtual public MakeFinal {
//public:
//	void test() {cout << a << endl;}
// 	FinalClass() {}
//	~FinalClass() {}
//};
//
////这里尝试继承FinalClass的行为会引发编译错误:
////FinalClass不能被继承
//class Try: public FinalClass {
//public:
//	Try() {}
//	~Try() {}
//};
//
//int main() {
//	FinalClass *f1;
//	FinalClass obj;
//}

class Test{
public:
	inline virtual void test() {
		cout << "here" << endl;
	}
};
template<typename T>
class MakeFinal {
public:
	
	friend T;
private:
	int a;
	MakeFinal() {}
	~MakeFinal() {}
};

//FinalClass是一个不能被继承的类，同时，可以在
//堆和栈上产生该类的实例
class FinalClass: virtual public MakeFinal<FinalClass> {
public:
	void test() {cout << a << endl;}
	FinalClass() {}
	~FinalClass() {}
};

////这里尝试继承FinalClass的行为会引发编译错误:
////FinalClass不能被继承
//class Try: public FinalClass {
//public:
//	Try() {}
//	~Try() {}
//};


//int main() {
//	int a[] = {1,2,3,4,5,6};
//	disply(a, sizeof(a) / sizeof(int));
//	for (int i = 0; i < sizeof(a) / sizeof(int); ++i)
//		cout << a[i] << endl;
//	/*Test t;
//	t.test();
//	cout << sizeof(t) << endl;
//	FinalClass *f1;
//	FinalClass obj;*/
//}
