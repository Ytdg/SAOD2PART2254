#pragma once
#include <memory>
#include <vector>
#include <queue>

class HC
{
public:
	/// Вспомогательный класс для построения дерева
	struct Node
	{
		char ch;
		size_t freq;
		shared_ptr<Node> left;
		shared_ptr<Node> right;
		Node(char ch, size_t freq) : ch(ch), freq(freq)
		{}
	};

	/// Вспомогательный класс для восстановления дерева по таблице кодов
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


	/// Вспомогательный класс кодов Хаффмана
	struct Code
	{
		char len;			//длина кода
		char ch;			//символ кода
		unsigned int bits;	//биты кода

		Code() :len(0), ch(0), bits(0){}
		Code(const Code& c) :len(c.len), ch(c.ch), bits(c.bits){}
		Code(char len, unsigned int bits, char ch=0) :len(len), ch(ch), bits(bits){}
		
		/// <summary>
		/// Пполучает следующий код Хаффмана при движении вверх слева или справа
		/// - увеличивает длину на 1 и сдвигает данные кода влево, 
		/// - добаяляет 0 или 1 в конец в зависимости от направления
		/// </summary>
		/// <param name="toRight"> true, если движемся вправо</param>
		/// <returns>Новый код</returns>
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
	/// Буфер для таблицы кодов
	Code codes[256];

	/// <summary>
	/// Заполняет таблицу кодов codes по дерву Хаффмана
	/// </summary>
	/// <param name="node"> текущий узел дерева</param>
	/// <param name="c"> текущий код</param>
	/// <returns></returns>
	void fillTbl(const shared_ptr<Node>& node, const Code &c)
	{
		if (!node)
			return;

		if (!node->left && !node->right)
		{
			// Нашли лист и заполняем его код в таблице
			codes[(unsigned char)node->ch] = Code(c.len,c.bits, node->ch);
		}
		else
		{	/// Продолжаем двигаться по дереву и строить коды поддеревьев
			if (node->left)
				fillTbl(node->left, c.up(false));
			if (node->right)
				fillTbl(node->right, c.up(true));
		}
	}
public:
	/// <summary>
	/// Строит дерево Хаффмана по статистике в stat и генерирует таблицу кодов 
	/// </summary>
	/// <param name="stat"> таблица символ-число появлений</param>
	/// <returns></returns>
	void GenerateCodes(std::map<char, int>& stat)
	{
		// Специальные случаи. Нест символов и 1 символ.
		if (stat.size() == 0) {
			fillTbl(make_shared<Node>(0, 0), Code());
			return;
		}
		if (stat.size() == 1) {
			fillTbl(make_shared<Node>(0, 0), Code(1, stat.begin()->first, 0));
			return;
		}

		// Очередь приоритетов в порядке обратном частоте 
		// Чем больше частота появления,тем позже в очереди 
		// благодаря переопределению операции сравнения shared_ptr<Node> ниже
		std::priority_queue<shared_ptr<Node>> minHeap;

		// Строим уровень листьев дерева Хаффмана
		for (const auto &t : stat)
		{
			minHeap.push(make_shared<Node>(t.first, t.second));
		}
		// Строим дерево методом Хаффмана
		shared_ptr<Node>current, left, right;
		// пока не дошли до корня 
		while (minHeap.size() != 1)
		{
			// Наименьший элемент делаем левым
			left = minHeap.top();
			minHeap.pop();

			// Следующий за ним - правым
			right = minHeap.top();
			minHeap.pop();

			// Образуем родительский узел
			current = make_shared<Node>(0, left->freq + right->freq);
			current->left = left;
			current->right = right;
			// помещаем его в очередь приоритетов
			minHeap.push(current);
		}
		// Заполняем таблицу кодов Хаффмана по дереву 
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
	/// Восстанавливает дерево кодов Хаффмана по таблице кодов (удобно для декодирования)
	/// </summary>
	/// <returns>указатель на корень дерева</returns>
	shared_ptr<SimpleNode> ReconstructTree()
	{
		shared_ptr<SimpleNode> root = make_shared<SimpleNode>();
		for (int i = 0; i < TblSize(); i++)
			addLeave(root, GetCode(i));
		return root;
	}

	/// <summary>
	/// Размер таблицы кодов
	/// <summary>
	/// <returns>Размер таблицы </returns>
	int TblSize(){ return sizeof(codes) / sizeof(Code);}

	/// <summary>
	/// Получение кода по символу
	/// <summary>
	/// <param name="i"> индекс кода в таблице. Совпадает с ascii кодом символа</param>
	/// <returns>Значение кода. Если длина кода 0, то код отсутствует </returns>
	Code GetCode(int i)
	{
		if (i<0 || i>=TblSize())
			throw out_of_range("Index out of range");
		return codes[i];
	}
	/// <summary>
	/// Задание кода определенного символа
	/// <summary>
	/// <param name="с"> Код </param>
	void SetCode(const Code &c)
	{
		codes[unsigned char(c.ch)] = c;
	}
};

/// <summary>
/// Оператор сравнения. Превращает очередь приоритетов в обратную для объектов типа shared_ptr<HC::Node>
/// Чем больше частота появления,тем позже в очереди при построении дерева
/// <summary>
/// <returns>true, если lh < rh иначе false </returns>
inline bool operator < (shared_ptr<HC::Node> lh, shared_ptr<HC::Node> rh) { return lh->freq>rh->freq; }
