#pragma once
#include <memory>
#include <vector>
#include <queue>

class HC
{
public:
	/// ��������������� ����� ��� ���������� ������
	struct Node
	{
		char ch;
		size_t freq;
		shared_ptr<Node> left;
		shared_ptr<Node> right;
		Node(char ch, size_t freq) : ch(ch), freq(freq)
		{}
	};

	/// ��������������� ����� ��� �������������� ������ �� ������� �����
	struct SimpleNode {
		char ch;
		shared_ptr<SimpleNode> left;
		shared_ptr<SimpleNode> right;
		SimpleNode() : ch(0) {}
		SimpleNode(char ch) : ch(ch) {}

		bool IsLeave() const {
			return !left && !right;
		}
	};


	/// ��������������� ����� ����� ��������
	struct Code
	{
		char len;			//����� ����
		char ch;			//������ ����
		unsigned int bits;	//���� ����

		Code() :len(0), ch(0), bits(0){}
		Code(const Code& c) :len(c.len), ch(c.ch), bits(c.bits){}
		Code(char len, unsigned int bits, char ch=0) :len(len), ch(ch), bits(bits){}
		
		/// <summary>
		/// ��������� ��������� ��� �������� ��� �������� ����� ����� ��� ������
		/// - ����������� ����� �� 1 � �������� ������ ���� �����, 
		/// - ��������� 0 ��� 1 � ����� � ����������� �� �����������
		/// </summary>
		/// <param name="toRight"> true, ���� �������� ������</param>
		/// <returns>����� ���</returns>
		///===============================================
		Code up(bool right) const
		{
			return Code(len + 1, (bits << 1) | (int)(right? 1 : 0));
		}
		Code down(bool &toRight) const
		{
			if (len == 0)
				return *this;
			int m = 1 << (len-1);
			toRight = (m&bits) != 0;
			return Code(len - 1, (m-1)&bits, ch);
		}
	};

private:
	/// ����� ��� ������� �����
	Code codes[256];

	/// <summary>
	/// ��������� ������� ����� codes �� ����� ��������
	/// </summary>
	/// <param name="node"> ������� ���� ������</param>
	/// <param name="c"> ������� ���</param>
	/// <returns></returns>
	void fillTbl(const shared_ptr<Node>& node, const Code &c)
	{
		if (!node)
			return;

		if (!node->left && !node->right)
		{
			// ����� ���� � ��������� ��� ��� � �������
			codes[(unsigned char)node->ch] = Code(c.len,c.bits, node->ch);
		}
		else
		{	/// ���������� ��������� �� ������ � ������� ���� �����������
			if (node->left)
				fillTbl(node->left, c.up(false));
			if (node->right)
				fillTbl(node->right, c.up(true));
		}
	}
public:
	/// <summary>
	/// ������ ������ �������� �� ���������� � stat � ���������� ������� ����� 
	/// </summary>
	/// <param name="stat"> ������� ������-����� ���������</param>
	/// <returns></returns>
	void GenerateCodes(std::map<char, int>& stat)
	{
		// ����������� ������. ���� �������� � 1 ������.
		if (stat.size() == 0) {
			fillTbl(make_shared<Node>(0, 0), Code());
			return;
		}
		if (stat.size() == 1) {
			fillTbl(make_shared<Node>(0, 0), Code(1, stat.begin()->first, 0));
			return;
		}

		// ������� ����������� � ������� �������� ������� 
		// ��� ������ ������� ���������,��� ����� � ������� 
		// ��������� ��������������� �������� ��������� shared_ptr<Node> ����
		std::priority_queue<shared_ptr<Node>> minHeap;

		// ������ ������� ������� ������ ��������
		for (const auto &t : stat)
		{
			minHeap.push(make_shared<Node>(t.first, t.second));
		}
		// ������ ������ ������� ��������
		shared_ptr<Node>current, left, right;
		// ���� �� ����� �� ����� 
		while (minHeap.size() != 1)
		{
			// ���������� ������� ������ �����
			left = minHeap.top();
			minHeap.pop();

			// ��������� �� ��� - ������
			right = minHeap.top();
			minHeap.pop();

			// �������� ������������ ����
			current = make_shared<Node>(0, left->freq + right->freq);
			current->left = left;
			current->right = right;
			// �������� ��� � ������� �����������
			minHeap.push(current);
		}
		// ��������� ������� ����� �������� �� ������ 
		fillTbl(minHeap.top(), Code());
	}

	void addLeave(shared_ptr<SimpleNode> node, const Code &c)
	{
		if (c.len == 0)
		{
			node->ch = c.ch;
			return;
		}
		bool toRight;
		Code d = c.down(toRight);
		if (toRight)
		{
			if (!node->right)
				node->right = make_shared<SimpleNode>();
			addLeave(node->right, d);
		}
		else
		{
			if (!node->left)
				node->left = make_shared<SimpleNode>();
			addLeave(node->left, d);
		}
	}
	/// <summary>
	/// ��������������� ������ ����� �������� �� ������� ����� (������ ��� �������������)
	/// </summary>
	/// <returns>��������� �� ������ ������</returns>
	shared_ptr<SimpleNode> ReconstructTree()
	{
		shared_ptr<SimpleNode> root = make_shared<SimpleNode>();
		for (int i = 0; i < TblSize(); i++)
			addLeave(root, GetCode(i));
		return root;
	}

	/// <summary>
	/// ������ ������� �����
	/// <summary>
	/// <returns>������ ������� </returns>
	int TblSize(){ return sizeof(codes) / sizeof(Code);}

	/// <summary>
	/// ��������� ���� �� �������
	/// <summary>
	/// <param name="i"> ������ ���� � �������. ��������� � ascii ����� �������</param>
	/// <returns>�������� ����. ���� ����� ���� 0, �� ��� ����������� </returns>
	Code GetCode(int i)
	{
		if (i<0 || i>=TblSize())
			throw out_of_range("Index out of range");
		return codes[i];
	}
	/// <summary>
	/// ������� ���� ������������� �������
	/// <summary>
	/// <param name="�"> ��� </param>
	void SetCode(const Code &c)
	{
		codes[unsigned char(c.ch)] = c;
	}
};

/// <summary>
/// �������� ���������. ���������� ������� ����������� � �������� ��� �������� ���� shared_ptr<HC::Node>
/// ��� ������ ������� ���������,��� ����� � ������� ��� ���������� ������
/// <summary>
/// <returns>true, ���� lh < rh ����� false </returns>
inline bool operator < (shared_ptr<HC::Node> lh, shared_ptr<HC::Node> rh) { return lh->freq>rh->freq; }
