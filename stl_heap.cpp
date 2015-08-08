//stl_heap.h
#ifndef __SGI_STL_INTERNAL_HEAP_H
#define __SGI_STL_INTERNAL_HEAP_H

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1209
#endif

	//////////////////////////////////////////////////////
	//push_heap()操作前要保证新添加的元素已经加入到容器末尾!!!
	///////////////////////////////////////////////////////
	template <class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
	Distance topIndex, T value)
{
	// 首先找出待处理元素的父结点
	Distance parent = (holeIndex - 1) / 2;

	// 判断当前待处理结点是否优先级高于其父结点, 如果是则将其父结点向下移动
	// 设置当前结点为父结点位置, 继续, 直到优先级小于父结点或者已经到达heap顶端
	while (holeIndex > topIndex && *(first + parent) < value) {
		//当尚未到达顶端且父节点小于新值
		*(first + holeIndex) = *(first + parent); //令洞值为父值
		holeIndex = parent;  //调整洞号，向上提升至父节点
		parent = (holeIndex - 1) / 2;  //新洞的父节点
	}
	// 将找到的合适的位置设置成正确值
	*(first + holeIndex) = value;
}

template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first,
	RandomAccessIterator last, Distance*, T*)
{
	// 因为first所指的那个元素不是heap的组成元素, 所以计算距离要减去1
	__push_heap(first, Distance((last - first) - 1), Distance(0),
		T(*(last - 1)));
}

// 调用此函数前要先把待处理元素追加到容器末尾
template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__push_heap_aux(first, last, distance_type(first), value_type(first));
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
	Distance topIndex, T value, Compare comp)
{
	Distance parent = (holeIndex - 1) / 2;
	while (holeIndex > topIndex && comp(*(first + parent), value)) {
		*(first + holeIndex) = *(first + parent);
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;
}

template <class RandomAccessIterator, class Compare, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first,
	RandomAccessIterator last, Compare comp,
	Distance*, T*)
{
	__push_heap(first, Distance((last - first) - 1), Distance(0),
		T(*(last - 1)), comp);
}

// 这个除了用户自己指定优先级决策判别式外和默认的无区别
template <class RandomAccessIterator, class Compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last,
	Compare comp)
{
	__push_heap_aux(first, last, comp, distance_type(first), value_type(first));
}

/////////////////////////////////////////////////////////
// 注意: pop_heap()操作, 执行完操作后要自己将容器尾元素弹出
//////////////////////////////////////////////////////////
// 这里以默认的heap优先级决策来说
// STL采用的是先将待pop的元素复制到heap尾部, 然后将整个heap向上调整
// 这样就会将最后空出一个hole, 将原来最后的元素在这里进行push()操作
// 这就是两个shift_up的过程
// 个人感觉使用使用shift_down的算法更高效, 虽然时间复杂度一样, 但是shift_down
// 进行操作的元素会更少,
// 之所以用shift_up这可能也是STL设计理念的问题吧, 能复用就不写新的^_^
////////////////////////////////////////////////////////////////////
template <class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex,
	Distance len, T value)
{
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;     // 弹出元素的有子孩

	// 调整heap元素位置
	while (secondChild < len) {
		// 选择两个子孩中较大的进行操作, 使用secondChild表示其偏移
		if (*(first + secondChild) < *(first + (secondChild - 1)))
			secondChild--;

		// 将较大元素向上填充, 并将整体偏移向下调整, 继续调整
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChild = 2 * (secondChild + 1);
	}

	if (secondChild == len) {
		*(first + holeIndex) = *(first + (secondChild - 1));
		holeIndex = secondChild - 1;
	}

	// 这里就是shift_up过程了, 将最初的heap末尾元素向上调整
	// 侯捷老师对这里的理解有误, :-), 人非圣贤, 孰能无过, ^_^
	__push_heap(first, holeIndex, topIndex, value);
}

template <class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
	RandomAccessIterator result, T value, Distance*)
{
	// 将弹出的元素调整到heap末尾, 这个元素需要用户手动弹出
	*result = *first;

	// 去掉末尾哪个弹出的元素, 调整heap
	__adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first,
	RandomAccessIterator last, T*)
{
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__pop_heap_aux(first, last, value_type(first));
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex,
	Distance len, T value, Compare comp)
{
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2; //洞节点的右孩子节点
	while (secondChild < len) {
		//比较洞节点左右两孩子，然后以secondChild代表大的子节点
		if (comp(*(first + secondChild), *(first + (secondChild - 1))))
			secondChild--;
		//下滤：令较大的孩子值为洞值，令洞号下移到较大的孩子节点
		*(first + holeIndex) = *(first + secondChild); 
		holeIndex = secondChild; 
		secondChild = 2 * (secondChild + 1); //找到新洞节点的右孩子节点
	}
	if (secondChild == len) { //没有右孩子，只有左孩子
		//下滤：令左孩子为洞值，再令洞下移到左孩子节点
		*(first + holeIndex) = *(first + (secondChild - 1));
		holeIndex = secondChild - 1;
	}
	//将欲调整值插入目前的洞内
	__push_heap(first, holeIndex, topIndex, value, comp);
}

template <class RandomAccessIterator, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
	RandomAccessIterator result, T value, Compare comp,
	Distance*)
{
	*result = *first;
	__adjust_heap(first, Distance(0), Distance(last - first), value, comp);
}

template <class RandomAccessIterator, class T, class Compare>
inline void __pop_heap_aux(RandomAccessIterator first,
	RandomAccessIterator last, T*, Compare comp)
{
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp,
		distance_type(first));
}

template <class RandomAccessIterator, class Compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last,
	Compare comp)
{
	__pop_heap_aux(first, last, value_type(first), comp);
}

// 建立堆的过程就是一系列插入，即下滤过程
template <class RandomAccessIterator, class T, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*,
	Distance*)
{
	if (last - first < 2) return;  //堆仅含0或1个元素，不必重新排序
	Distance len = last - first;
	Distance parent = (len - 2)/2;

	while (true) {
		__adjust_heap(first, parent, len, T(*(first + parent)));
		if (parent == 0) return; //走完了根节点，结束
		parent--; 
	}
}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__make_heap(first, last, value_type(first), distance_type(first));
}

template <class RandomAccessIterator, class Compare, class T, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last,
	Compare comp, T*, Distance*)
{
	if (last - first < 2) return;
	Distance len = last - first;
	Distance parent = (len - 2)/2;

	while (true) {
		__adjust_heap(first, parent, len, T(*(first + parent)), comp);
		if (parent == 0) return;
		parent--;
	}
}

template <class RandomAccessIterator, class Compare>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last,
	Compare comp)
{
	__make_heap(first, last, comp, value_type(first), distance_type(first));
}

// 堆排序，保证heap有序，每次将堆的最值放在vector的末尾
template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	while (last - first > 1) pop_heap(first, last--);
}

template <class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last,
	Compare comp)
{
	while (last - first > 1) pop_heap(first, last--, comp);
}

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1209
#endif

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_HEAP_H */
