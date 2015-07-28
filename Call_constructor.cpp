/*
第一个例子：
复制控制（复制构造函数、赋值操作符、析构函数）的调用时机测试(一)
*/
class A {
	int data;
public:
	A() {
		cout << "\tA()" << endl;
	}

	A(const A & a) {
		data = a.data;
		cout << "\t复制构造函数" << endl;
	}

	A& operator=(const A& rhs) {
		cout << "\toperator=" << endl;
		return *this;
	}
};

A global;                     //调用构造函数

A foo(A arg) { 
	//调用复制构造函数，用arg初始化局部对象local
	cout << "2---------------" << endl;
	A local = arg;

	//这里是复制初始化方式：先调用指定构造函数创建临时对象，然后
	//用复制构造函数将临时对象复制到正在创建的对象。
	//但是编译器一般会优化，直接跳过复制构造函数直接创建对象，
	//所以这里并没有调用复制构造函数
	cout << "3---------------" << endl;
	A local2 = A();
	cout << "4---------------" << endl;

	//下面两句均调用复制构造函数
	A local3 = arg;
	A local4 = A(arg);
	local4 = arg;          //使用赋值操作符
	cout << "41--------------" << endl;
	A *heap = new A(global); //调用复制构造函数，根据全局对象global创建新的A对象
	cout << "5---------------" << endl;

	//这句没有调用(复制)构造函数,使用赋值操作符
	*heap = local;           
	cout << "6---------------" << endl;

	// 这里两次调用复制构造函数、两次调用默认构造函数
	// 如果没有为类类型数组提供元素初始化式，则将用默认构造函数初始化每个元素；
	// 如果使用常规的花括号括住数组初始化列表（如下）来显示初始化，则使用复制
	// 初始化来初始化每个元素。根据指定的值创建适当类型的元素，然后用复制构造函数
	// 将该值复制到相应元素。如下的数组使用花括号，因此使用复制构造函数来初始化。
    A pa[4] = {local, *heap};
	cout << "7---------------" << endl;

    /*
    如果是这样的定义，则只调用四次构造函数，不会调用复制构造函数
    A pa[4];   
	*/

	return *heap;            //调用复制构造函数，从函数返回A对象*heap的副本
}

int main() {
	cout << "A aa: "; 
	A aa;        //调用构造函数
	foo(aa);     //调用复制构造函数，复制一个对象，将其作为实参传递给一个函数

	cout << endl << "8----------------" << endl;

	//使用了五次默认构造函数和复制构造函数，先使用A的默认构造函数创建临时值
	//来初始化avec，然后使用复制构造函数将临时值复制到avec的每个元素。
	vector<A> avec(5);  
	return 0;
}


/*
第二个例子：
复制控制（复制构造函数、赋值操作符、析构函数）的调用时机测试(二)
*/
class Exmpl {
public:
	Exmpl() {	cout << "Exmpl()" << endl;  }
	Exmpl(const Exmpl&) {  cout << "拷贝构造函数" << endl;  }
	Exmpl& operator= (const Exmpl & rhs) {
		cout << "oprator=" << endl;
		return *this;
	}
	~Exmpl() {  cout << "~Exmpl" << endl; }
};

void func1(Exmpl obj) {}   //形参为Exmpl对象
void func2(Exmpl& obj) {}  //形参为Exmpl对象的引用
Exmpl func3() {
	Exmpl obj;    return obj;
}

const Exmpl& func4(const Exmpl& obj) {
	return obj;
}
int main() {
	//调用默认构造函数创建Exmpl对象obj
	Exmpl obj;

	//调用复制构造函数
	//将形参Exmpl对象创建为实参Exmpl对象的副本
	//函数执行完毕后，调用析构函数撤销形参Exmpl对象
	func1(obj); 

	//形参为对象的引用，因此不需要调用复制构造函数传递实参
	func2(obj);

	//调用默认构造函数创建局部Exmpl对象
	//函数返回时调用复制构造函数创建作为返回值副本的Exmpl对象
	//然后调用析构函数撤销局部对象
	//调用赋值操作符完成赋值操作
	//调用析构函数撤销作为返回值副本的Exmpl对象
	obj = func3();

	//函数形参和返回值都是引用，不会调用(拷贝)构造函数
	func4(obj);

	//func4不会调用任何复制控制操作
	//由于exm也是一个引用，因此下一句不会调用任何构造、析构函数
	const Exmpl& exm = func4(obj);

	//调用拷贝构造函数来将exm2对象初始化为func4返回的对象的引用
	const Exmpl exm2 = func4(obj);

	//调用默认构造函数动态创建对象
	//该对象需要手动才能销毁
	Exmpl *p = new Exmpl;

	//三次调用默认构造函数创建临时对象
	//三次调用复制构造函数将临时对象复制到容器evec的每个元素
	//调用析构函数撤销临时对象
	vector<Exmpl> evec(3);
	
	//调用析构函数撤销动态创建的对象
	delete p;

	//evec和obj的生命结束，调用析构函数撤销
	//evec撤销需要调用三次析构函数
	return 0;
}
