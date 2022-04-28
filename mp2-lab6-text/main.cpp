#define _CRT_SECURE_NO_WARNINGS
#define HOME  71
#define DOWN  80
#define NEXT  77
#define UP	  72
#define ESC	  27
#define INS	  82
#define DEL	  83
#define ENTER 13

#include <iostream>
#include <conio.h>
#include "TText.h"
#include "ConsoleExt.h"

TMem TNode::mem;

void OnEsc(TText& t) {

	cout << "\nTText.Print():\n";
	cout << "*************\n";
	t.print();
	cout << '\n';

	cout << "Printing via iteration:\n";
	cout << "**********************\n";
	for (t.reset(); !t.isEnd(); t.goNext()) {
		cout << t.getCurrentLine() << '\n';
	}

	t.save("..\\out.txt");
	cout << "\nOutput saved to out.txt.\n\n";
}

int main() {
	TNode::initMem(100);
	TText t;

	t.load("..\\text.txt");
	t.goFirstNode();

	char ch;

	do {
		cout << ">,v,^, Home, Ins, Del, Enter, Esc\n";
		cout << "*********************************\n";

		t.print();
		cout << '\n';

		ch = _getch();

		if (ch == 0xE0) {
			ch = _getch();
		}
		if (ch == ESC) {
			OnEsc(t);
			break;
		}

		if (ch != ENTER) {
			ch = _getch();
		}

		switch (ch)
		{
		case ENTER: break;
		case HOME:
			t.goFirstNode();
			break;
		case DOWN:
			t.goDownNode();
			break;
		case NEXT:
			t.goNextNode();
			break;
		case UP:
			t.goUp();
			break;
		case INS:
			cout << "Enter new text item: ";
			char item[256];
			if (fgets(item, sizeof(item), stdin)) {
				item[strcspn(item, "\r\n")] = 0;

				cout << "Insert:\n1 - Next line, "
					<< "2 - Next section, 3 - Down line, "
					<< "4 - Down section\n";
				int o1;
				cin >> o1;
				if (o1 == 1) {
					t.insNextLine(item);
				}
				else if (o1 == 2) {
					t.insNextSection(item);
				}
				else if (o1 == 3) {
					t.insDownLine(item);
				}
				else if (o1 == 4) {
					t.insDownSection(item);
				}
			}
			cin.ignore(256, '\n');
			break;
		case DEL:
			int o2;
			cout << "1 - Delete down, 2 - Delete next\n";
			cin >> o2;
			if (o2 == 1) {
				t.delDown();
			}
			else if (o2 == 2) {
				t.delNext();
			}
			cin.ignore(256, '\n');
			break;
		}

		ClearScreen();
	} while (ch != ESC);

	cout << ">> Free nodes <<:\n";
	cout << "****************\n";
	TNode::printFreeNodes();
	cout << '\n';

	TNode::cleanMem(t);
	cout << ">> Free nodes after garbage collection <<:\n";
	cout << "*****************************************\n";
	TNode::printFreeNodes();

	cout << "\n>> Final output <<:\n";
	cout << "******************\n";
	t.print();

	cout << "\nCopy test:\n";
	cout << "*********\n";

	TText* t2 = t.getCopy();
	t2->print();
}