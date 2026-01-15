#include <iostream>
using namespace std;
#include "AVL.hpp"

int main()
{
	AVL_tree<int> t;
	t.insert(5);
	t.insert(12);
	t.insert(18);
	t.insert(25);
	t.insert(27);
	t.insert(28);
	t.insert(29);
	t.insert(30);
	t.insert(46);
	t.insert(88);
	t.insert(95);
	

	t.remove(46);
	t.remove(88);
	t.remove(817);
	t.remove(28);
	t.remove(25);
	t.remove(95);
	t.remove(29);

	t.pre_order();
	t.in_order();
	t.post_order();
	t.level_order();

	cout << t.find(5) << endl;
	cout << t.find(1) << endl;
	cout << t.max() << " " << t.min() << endl;

	return 0;
}