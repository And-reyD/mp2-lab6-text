#pragma once
#include "TStack.h";
#include <fstream>;

struct TNode;
class TText;

struct TMem {
	TNode* pFirst;
	TNode* pFree;
	TNode* pLast;
};

struct TNode {
	char str[81];
	TNode* pNext;
	TNode* pDown;

	bool Garbage;

	static TMem mem;

	TNode(char* _str = nullptr, TNode* _pNext = nullptr, TNode* _pDown = nullptr);

	void* operator new(size_t size);
	void operator delete(void* ptr);

	static void initMem(size_t size);
	static void printFreeNodes();

	static void cleanMem(TText& t);
};

class TText {
private:
	TNode* pFirst, * pCurr;
	TStack<TNode*> st;

	TNode* readRec(ifstream& fin);

	int textLevel = 0;

	void printRec(TNode* p);
	void writeRec(TNode* p, ostream& out);

	TNode* copyNode(TNode* p);
public:
	TText();
	TText(TNode* p);

	TText* getCopy();

	void goNextNode();
	void goDownNode();

	void goUp();
	void goFirstNode();

	void insNextLine(char* _str);
	void insNextSection(char* _str);
	void insDownLine(char* _str);
	void insDownSection(char* _str);

	void delNext();
	void delDown();

	void reset();
	void goNext();
	bool isEnd();

	char* getCurrentLine();

	void load(string fn);
	void print();
	void save(string fn);
	void notGarbage();
};


TNode::TNode(char* _str, TNode* _pNext, TNode* _pDown) {
	if (_str == nullptr) {
		str[0] = '\0';
	}
	else {
		strcpy(str, _str);
	}
	pNext = _pNext;
	pDown = _pDown;
}

void* TNode::operator new(size_t size) {
	TNode* tmp = mem.pFree;
	if (tmp == nullptr) {
		throw "Out of memory";
	}
	mem.pFree = mem.pFree->pNext;
	return tmp;
}

void TNode::operator delete(void* ptr) {
	TNode* tmp = mem.pFree;
	TNode* p1 = (TNode*)ptr;
	p1->pNext = tmp;
	mem.pFree = p1;
}

void TNode::initMem(size_t size) {
	mem.pFirst = (TNode*) new char[size * sizeof(TNode)];
	mem.pFree = mem.pFirst;
	mem.pLast = mem.pFirst + (size - 1);

	TNode* p = mem.pFirst;
	for (int i = 0; i < size - 1; i++) {
		p->pNext = p + 1;
		p->str[0] = 0;
		p->Garbage = true;

		p += 1;
	}

	mem.pLast->pNext = nullptr;
	mem.pLast->str[0] = 0;
}

void TNode::printFreeNodes() {
	TNode* p = mem.pFree;
	while (p != nullptr) {
		if (p->str[0] != '\0') {
			cout << p->str << '\n';
		}
		p = p->pNext;
	}
}

void TNode::cleanMem(TText& t) {
	for (t.reset(); !t.isEnd(); t.goNext()) {
		t.notGarbage();
	}

	TNode* p = TNode::mem.pFree;
	while (p != nullptr) {
		p->Garbage = false;
		p = p->pNext;
	}

	p = TNode::mem.pFirst;
	for (p = TNode::mem.pFirst; p <= TNode::mem.pLast; p++) {
		if (p->Garbage) {
			delete p;
			p->Garbage = false;
		}
	}
}


TNode* TText::readRec(ifstream& fin) {
	TNode* pTemp = nullptr, * pHead = nullptr;
	char str[81];

	while (!fin.eof()) {
		fin.getline(str, 81, '\n');
		if (str[0] == '{') {
			pTemp->pDown = readRec(fin);
		}
		else if (str[0] == '}') {
			break;
		}
		else if (strcmp(str, "") == 0) {
			continue;
		}
		else
		{
			TNode* newNode = new TNode(str);
			if (pHead == nullptr) {
				pTemp = pHead = newNode;
			}
			else {
				pTemp->pNext = newNode;
			}
			pTemp = newNode;
		}
	}
	return pHead;
}

void TText::printRec(TNode* p) {
	if (p != nullptr) {
		for (int i = 0; i < textLevel; i++) {
			cout << "   ";
		}

		if (p == pCurr) {
			cout << "*";
		}
		else {
			cout << " ";
		}

		cout << p->str << '\n';

		textLevel++;
		printRec(p->pDown);
		textLevel--;
		printRec(p->pNext);
	}
}

void TText::writeRec(TNode* p, ostream& out) {
	if (p != nullptr) {
		out << p->str << '\n';
		if (p->pDown != nullptr) {
			out << "{\n";
			writeRec(p->pDown, out);

			out << "}\n";
		}
		writeRec(p->pNext, out);
	}
}

TNode* TText::copyNode(TNode* p) {
	TNode* pd, * pn, * pCopy;

	if (p->pDown != nullptr) {
		pd = copyNode(p->pDown);
	}
	else {
		pd = nullptr;
	}

	if (p->pNext != nullptr) {
		pn = copyNode(p->pNext);
	}
	else {
		pn = nullptr;
	}

	pCopy = new TNode(p->str, pn, pd);
	return pCopy;
}

TText::TText() { }

TText::TText(TNode* p) {
	pFirst = p;
	pCurr = nullptr;
}

TText* TText::getCopy() {
	TText* res;
	res = new TText(copyNode(pFirst));
	return res;
}

void TText::goNextNode() {
	if (pCurr != nullptr && pCurr->pNext != nullptr) {
		st.push(pCurr);
		pCurr = pCurr->pNext;
	}
}

void TText::goDownNode() {
	if (pCurr != nullptr && pCurr->pDown != nullptr) {
		st.push(pCurr);
		pCurr = pCurr->pDown;
	}
}

void TText::goUp() {
	if (!st.empty()) {
		TNode* prevNode = st.pop();
		pCurr = prevNode;
	}
}

void TText::goFirstNode() {
	st.clear();
	pCurr = pFirst;
}

void TText::insNextLine(char* _str) {
	if (pCurr != nullptr) {
		TNode* newNode = new TNode(_str);
		newNode->pNext = pCurr->pNext;
		pCurr->pNext = newNode;
	}
}

void TText::insNextSection(char* _str) {
	if (pCurr != nullptr) {
		TNode* newNode = new TNode(_str);
		newNode->pDown = pCurr->pNext;
		pCurr->pNext = newNode;
	}
}

void TText::insDownLine(char* _str) {
	if (pCurr != nullptr) {
		TNode* newNode = new TNode(_str);
		newNode->pNext = pCurr->pDown;
		pCurr->pDown = newNode;
	}
}

void TText::insDownSection(char* _str) {
	if (pCurr != nullptr) {
		TNode* newNode = new TNode(_str);
		newNode->pDown = pCurr->pDown;
		pCurr->pDown = newNode;
	}
}

void TText::delNext() {
	if (pCurr != nullptr) {
		TNode* pDel = pCurr->pNext;
		if (pDel != nullptr) {
			pCurr->pNext = pDel->pNext;
			delete pDel;
		}
	}
}

void TText::delDown() {
	if (pCurr != nullptr) {
		TNode* pDel = pCurr->pDown;
		if (pDel != nullptr) {
			pCurr->pDown = pDel->pNext;
			delete pDel;
		}
	}
}

void TText::reset() {
	st.clear();

	if (pFirst != nullptr) {
		pCurr = pFirst;
		st.push(pCurr);

		if (pCurr->pNext != nullptr) {
			st.push(pCurr->pNext);
		}
		if (pCurr->pDown != nullptr) {
			st.push(pCurr->pDown);
		}
	}
}

void TText::goNext() {
	pCurr = st.pop();

	if (pCurr != pFirst) {
		if (pCurr->pNext != nullptr) {
			st.push(pCurr->pNext);
		}
		if (pCurr->pDown != nullptr) {
			st.push(pCurr->pDown);
		}
	}
}

bool TText::isEnd() {
	return st.empty();
}

char* TText::getCurrentLine() {
	return pCurr->str;
}

void TText::load(string fn) {
	ifstream ifs(fn);
	pFirst = readRec(ifs);
}

void TText::print() {
	printRec(pFirst);
}

void TText::save(string fn) {
	std::ofstream out;
	out.open(fn);

	if (!out.is_open()) {
		throw "Export exception!";
	}

	writeRec(pFirst, out);
}

void TText::notGarbage() {
	pCurr->Garbage = false;
}