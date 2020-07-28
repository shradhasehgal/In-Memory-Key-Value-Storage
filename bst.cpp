#include <iostream>
#include <string.h>
using namespace std;
// #pragma GCC OPTIMIZE("O2", "Ofast", "Os")

struct BSTNode
{
	int left : 7, right : 7, c : 7;
	int data;
	int ends;
};

class BST
{
public:
	BST()__attribute__((aligned(256),hot));
	bool resize()__attribute__((aligned(256),cold));
	int inorder(int &, int)__attribute__((aligned(256),hot));
	bool insert(uint8_t, int, int)__attribute__((aligned(256)));
	bool change_ends(uint8_t, int)__attribute__((aligned(256)));
	int find(uint8_t c)__attribute__((aligned(256),hot));
	int par(uint8_t c)__attribute__((aligned(256)));
	bool remove_bst(uint8_t c)__attribute__((aligned(256)));

	BSTNode *nodes;
	int free_head : 7, free_tail : 7, sz : 7, size : 8;
};
	inline BST::BST()
	{
		size = 2;
		nodes = (BSTNode *)malloc(size * sizeof(BSTNode));
		free_head = 0;
		free_tail = size - 1;
		sz = 0;
		for (int i = 0; i < size - 1; i++)
		{
			nodes[i].data = i + 1;
			nodes[i].left = -1;
			nodes[i].right = -1;
		}
	}
	bool BST::resize()
	{
		int old_size = size;
		size <<= 1;
		size = min(53, size);
		BSTNode *new_nodes = (BSTNode *)calloc(size, sizeof(BSTNode));
		memcpy(new_nodes, nodes, sizeof(BSTNode) * old_size);

		delete[] nodes;
		// nodes = NULL;
		nodes = new_nodes;
		nodes[free_tail].data = old_size;
		for (int i = old_size; i < size - 1; i++)
		{
			nodes[i].data = i + 1;
			nodes[i].left = -1;
			nodes[i].right = -1;
		}
		free_tail = size - 1;
		return true;
	}
	int BST::inorder(int &N, int cur = 0)
	{
		// cout<<"INORDER: "<<N<<" "<<cur<<" "<<sz<<" "<<size<<endl;
		if (cur < 0 || cur > size)
			return -1;
		if (nodes[cur].left == cur || nodes[cur].right == cur)
			return -1;

		if (!sz || !N)
			return -1;
		register int ret = -1;
		if (nodes[cur].left != -1)
			ret = inorder(N, nodes[cur].left);

		if (ret != -1)
			return ret;

		if (N <= nodes[cur].ends)
			return cur;

		N -= nodes[cur].ends;

		if (nodes[cur].right != -1)
			return inorder(N, nodes[cur].right);

		return -1;
	}
	bool BST::insert(uint8_t c, int data, int ends)
	{
		register int cur = 0;
		while (1)
		{
			if (!sz)
				break;
			if (nodes[cur].c == c)
			{
				nodes[cur].data = data;
				nodes[cur].ends = ends;
				return false;
			}
			int ncur;
			if (nodes[cur].c < c)
				ncur = nodes[cur].right;
			else
				ncur = nodes[cur].left;
			if (ncur == -1)
			{
				ncur = free_head;
				if (nodes[cur].c < c)
					nodes[cur].right = ncur;
				else
					nodes[cur].left = ncur;
				cur = ncur;
				break;
			}
			cur = ncur;
		}
		free_head = nodes[cur].data;
		nodes[cur].c = c;
		nodes[cur].data = data;
		nodes[cur].ends = ends;
		nodes[cur].left = nodes[cur].right = -1;
		sz++;
		if (size == sz + 1)
			resize();
		return true;
	}
	bool BST::change_ends(uint8_t c, int change)
	{
		int cur = find(c);
		if (cur == -1)
			return false;
		nodes[cur].ends += change;
		return true;
	}
	inline int BST::find(uint8_t c)
	{
		// cout<<sz<<endl;
		if (!sz)
			return -1;
		int cur = 0;
		while (cur != -1)
		{
			if (nodes[cur].c == c)
				return cur;
			if (nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
			if (cur <= 0 || cur > size)
				return -1;
		}
		return -1;
	}
	int BST::par(uint8_t c)
	{
		if (!sz)
			return -1;
		int cur = 0, par = -1;
		while (cur != -1)
		{
			if (nodes[cur].c == c)
				return par;
			par = cur;
			if (nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
		}
		return -1;
	}
	bool BST::remove_bst(uint8_t c)
	{
		if (!sz)
			return false;
		if (sz == 1)
		{
			sz = 0;
			nodes[0].left = -1;
			nodes[0].right = -1;
			nodes[0].data = free_head;
			free_head = 0;
			return true;
		}
		int cur = find(c);
		if (cur == -1)
			return false;

		if (nodes[cur].left == -1 && nodes[cur].right == -1)
		{
			int p = par(c);
			nodes[cur].data = 0;
			nodes[free_tail].data = cur;
			free_tail = cur;
			if (nodes[p].c < c)
				nodes[p].right = -1;
			else
				nodes[p].left = -1;
			sz--;
			return true;
		}

		if (nodes[cur].left == -1)
		{
			int ncur = nodes[cur].right;
			nodes[cur].left = nodes[ncur].left;
			nodes[cur].right = nodes[ncur].right;
			nodes[cur].c = nodes[ncur].c;
			nodes[cur].data = nodes[ncur].data;
			nodes[cur].ends = nodes[ncur].ends;
			nodes[ncur].left = -1;
			nodes[ncur].right = -1;
			nodes[ncur].data = 0;
			nodes[free_tail].data = ncur;
			free_tail = ncur;
			sz--;
		}
		else
		{
			int ncur = nodes[cur].left;
			while (nodes[ncur].right != -1)
				ncur = nodes[ncur].right;

			int character = nodes[ncur].c;
			int data = nodes[ncur].data;
			int ends = nodes[ncur].ends;

			remove_bst(nodes[ncur].c);

			nodes[cur].c = character;
			nodes[cur].data = data;
			nodes[cur].ends = ends;
		}
		return true;
	}
