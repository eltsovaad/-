#include <fstream>
#include <iostream>
#include<locale>
#include "Windows.h"
#include "string.h"
#include "stdio.h"
#include <cctype>

using namespace std;
void load_from_base(char name_base[], struct marsh *&head);
struct marsh * init(char data[][25]);
struct marsh_list * add_edit(marsh_list *last);
void del_el(marsh *&head);
void add_el(marsh *&head, char data[][25]);
int check_menu(int menu, int num);
char* in_data(char data[][25], int flag);
void edit(marsh *head);
struct marsh_list* search(struct marsh* head, int flag, int &k);
void leakes();
void del_all(struct marsh *head);
void list_out(marsh* head);
void sort(marsh* &head, int flag);
int is_equal(marsh* head, char data[][25]);
void del_ed(marsh_list* edit_head);
marsh* my_swap(marsh*&head, marsh* el1, marsh* el2);
void save_base(char name_base[], struct marsh *head);

struct marsh
{
	char start[25];
	char end[25];
	char num[5];
	struct marsh* next;//��������� �� ��������� �������
};

struct marsh_list
{
	int n;
	struct marsh* el;
	struct marsh_list* next;//��������� �� ��������� �������
};

struct marsh * init(char data[][25]) //������������� ������� �������� ������,��� �- �������� ������� ����
{
	struct marsh *head;
	// ��������� ������ ��� ������ ������
	head = new struct marsh;
	strcpy_s(head->start, data[0]);
	strcpy_s(head->end, data[1]);
	strcpy_s(head->num, data[2]);
	head->next = NULL;
	return(head);
}

struct marsh_list * add_edit(marsh_list *last)
{
	struct marsh_list *temp;
	temp = new struct marsh_list;
	last->next = temp;
	temp->next = NULL;
	return(temp);
}

void del_el(marsh *&head)//�������� ������ �������� ������
{
	if (head == NULL) {
		cout << "������ ��������� ����!" << endl;
	}
	else {
		struct marsh *prev, *del;
		del = NULL;
		struct marsh_list *del_edit, *del_head;
		int choice = -1, k = 1;
		del_edit = search(head, 1, k);
		del_head = del_edit;
		if (k > 2) {
			cout << "������� ���������� ����� ������� �������� �� ���������, ��� 0, ���� ������ ������� ��: ";
			cin >> choice;
			if (choice != 0) {
				choice = check_menu(choice, k - 1);
				k = 1;
			}
			else {
				k--;
			}
		}
		else {
			choice = 1;
			k = 1;
		}
		for (int i = 0; i < k; i++) {
			if (choice != 0) {
				while (del_edit != NULL) {
					if (del_edit->n == choice) {
						del = del_edit->el;
						break;
					}
					del_edit = del_edit->next;
				}
			}
			else {
				while (del_edit != NULL) {
					if (del_edit->n == i + 1) {
						del = del_edit->el;
						break;
					}
					del_edit = del_edit->next;
				}
			}
			if (del_edit != NULL) {
				prev = head;
				if (del != head) {
					while (prev->next != del) // ������������� ������ ������� � �����
					{ // ���� �� ������ ����, �������������� del
						prev = prev->next;
					}
					if (del->next == NULL) {
						prev->next = NULL;
					}
					else {
						prev->next = del->next; // ������������ ���������
					}
					delete(del);
					del = NULL;// ����������� ������ ���������� ����
				}
				else {
					if (head == NULL) {
						delete(head);
						head = NULL;
					}
					else {
						del = head;
						head = head->next;
						delete(del);
						del = NULL;// ����������� ������ ���������� ����
					}
				}


				cout << "�������� ��������� �������!" << endl;

			}
			else {
				cout << "�������� ����������!" << endl;
			}
		}
		del_ed(del_head);
	}
}

void add_el(marsh *&head, char data[][25])//���������� ��������
{
	if (head == NULL) {
		head = init(data);
	}
	else {
		struct marsh *temp, *last;
		last = head;
		while (last->next != NULL) {
			last = last->next;
		}
		temp = new struct marsh;
		//p = last->next; // ���������� ��������� �� ��������� �������
		last->next = temp; // ���������� ���� ��������� �� �����������
		strcpy_s(temp->start, data[0]); // ���������� ���� ������ ������������ ����
		strcpy_s(temp->end, data[1]);
		strcpy_s(temp->num, data[2]);
		temp->next = NULL; // ��������� ���� ��������� �� ��������� �������
	}
	//return(head);
}

int check_menu(int menu, int num) {
	int flag = 1;
	for (int i = 1; i <= num; i++) {
		if (menu == i) {
			flag = 0;
			break;
		}
	}

	while (cin.fail() || (flag == 1)) {
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cout << "�������� ����. ���������: ";
		cin >> menu;
		for (int i = 1; i <= num; i++) {
			if (menu == i) {
				flag = 0;
				break;
			}
		}
	}
	return menu;
}

char* in_data(char data[][25], int flag) {
	//flag==0 - ���������� �������� ==1 - ����� �� ����������� 
	//==2 - ����� �� ���������� ==3 - �� ������
	std::locale loc("Russian_Russia.1251");
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	if ((flag == 0) || (flag == 1)) {
		cout << "������� ����� �����������: ";
		cin.getline(data[0], 24, '\n');
		if (isupper(data[0][0], loc) == 0) {
			data[0][0] = (char)toupper(data[0][0]);
			cout << "�������� ����� �������������!" << endl;
		}
		while (cin.fail() /*|| (isupper(data[0][0], loc) == 0)*/) {
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "�������� ������ �� ������ ��������� 25 ��������" << endl;
			cout << "������� �������� ��������: ";
			cin.getline(data[0], 25, '\n');
		}
	}
	if ((flag == 0) || (flag == 2)) {
		cout << "������� ����� ����������: ";
		cin.getline(data[1], 25, '\n');
		if (isupper(data[1][0], loc) == 0) {
			data[1][0] = (char)toupper(data[1][0]);
			cout << "�������� ����� �������������!" << endl;
		}
		while (cin.fail() /*|| (isupper(data[1][0], loc) == 0)*/) {
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "�������� ������ �� ������ ��������� 25 ��������" << endl;
			cout << "������� �������� ��������: ";
			cin.getline(data[1], 25, '\n');
		}
	}
	if ((flag == 0) || (flag == 3)) {
		cout << "������� ����� ��������: ";
		cin.getline(data[2], 5, '\n');
		while (cin.fail()) {
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "����� �������� ������ ���� ��������!" << endl;
			cout << "������� ����� �������� ��������: ";
			cin.getline(data[2], 5, '\n');
		}
	}
	//�������� is_equal
	char* data_p = &data[0][0];
	return data_p;
}

void edit(marsh *head)//��������� ��������
{
	if (head == NULL) {
		cout << "������ ��������� ����!" << endl;
	}
	else {
		char data[3][25];
		struct marsh *edit;
		char* data_p = &data[1][1];
		struct marsh_list *ed, *edit_head;
		int choice = 1;
		int k = 1;
		ed = search(head, 1, k);
		edit_head = ed;
		if (k > 2) {
			cout << "������� ���������� ����� ������� �������� �� ���������: ";
			cin >> choice;
			choice = check_menu(choice, k - 1);
			while (ed != NULL) {
				if (ed->n == choice) {
					edit = ed->el;
					break;
				}
				ed = ed->next;
			}
		}
		else {
			if (ed != NULL) {
				edit = edit_head->el;
			}
		}
		if (ed != NULL) {
			cout << "������� ����� �������� ��� ����� ��������: " << endl;
			data_p = in_data(data, 0);
			int flag_eq = 0;
			flag_eq = is_equal(head, data);
			while (flag_eq == 1) {
				int ch = 0;
				cout << "����� ������� ��� ����������!" << endl;
				cout << "������ ������ ����� ������? 1- ��, 2 - ���" << endl;
				cin >> ch;
				ch = check_menu(ch, 2);
				if (ch == 1) {
					flag_eq = 0;
					data_p = in_data(data, 0);
					flag_eq = is_equal(head, data);
				}
				else {
					break;
				}
			}
			if ((edit != NULL) || (flag_eq == 0)) {
				strcpy_s(edit->start, data[0]);
				strcpy_s(edit->end, data[1]);
				strcpy_s(edit->num, data[2]);
			}
			cout << "��������� ��������� �������!" << endl;
			del_ed(edit_head);
		}
		else {
			cout << "��������� ����������!" << endl;
			delete edit_head;
			edit_head = NULL;
		}

	}
}


int is_equal(marsh* head, char data[][25]) {
	marsh* temp;
	temp = head;
	int flag = 0;//0 -���� ��� ������� ����������, 1 ���� ����
	while (temp != NULL) {
		if ((strcmp(data[2], temp->num) == 0) && (strcmp(temp->start, data[0]) == 0) && (strcmp(data[1], temp->end) == 0)) {
			flag = 1;
			break;
		}
		temp = temp->next;
	}
	return flag;
}



struct marsh_list* search(struct marsh* head, int flag, int &k)//����� ��������
{//flag 0 - ����� � �������, 1 - ����� ��� del � edit
	if (head == NULL) {
		cout << "������ ��������� ����!" << endl;
		return NULL;
	}
	else {
		char data[3][25];
		char* data_p = &data[0][0];
		int menu = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 25; j++) {
				data[i][j] = 0;
			}
		}
		struct marsh *temp;
		struct marsh_list *edit_head;
		edit_head = new struct marsh_list;
		struct marsh_list *edit = edit_head;
		temp = head;
		int fl_nothing = 1;//���� ������ �� ���� ������� ==1
		int choice = 1;
		k = 1;
		cout << "�������� ������ ������: " << endl;
		cout << "1. �� ������ �����������" << endl;
		cout << "2. �� ������ ����������" << endl;
		cout << "3. �� ������ ��������" << endl;
		cout << "������� ��� �����: ";
		cin >> menu;
		menu = check_menu(menu, 3);
		data_p = in_data(data, menu);
		switch (menu) {
		case 1: {
			while (temp != NULL) {
				if (strcmp(temp->start, data[0]) == 0) {
					edit->n = k;
					edit->el = temp;
					edit->next = add_edit(edit);
					edit = edit->next;
					k++;
					fl_nothing = 0;
				}
				temp = temp->next;
			}
			break;
		}
		case 2: {
			while (temp != NULL) {
				if (strcmp(temp->end, data[1]) == 0) {
					edit->n = k;
					edit->el = temp;
					edit->next = add_edit(edit);
					edit = edit->next;

					k++;
					fl_nothing = 0;
				}
				temp = temp->next;
			}
			break;
		}
		case 3: {
			while (temp != NULL) {
				if (strcmp(temp->num, data[2]) == 0) {
					edit->n = k;
					edit->el = temp;
					edit->next = add_edit(edit);
					edit = edit->next;

					k++;
					fl_nothing = 0;
				}
				temp = temp->next;
			}
			break;
		}
		}
		if (fl_nothing == 1) {
			cout << "������ �� ���� �������" << endl << endl;
			temp = NULL;
			delete(edit_head);
			edit_head = NULL;
		}
		else {
			edit = edit_head;
			if (k > 1) {
				cout << endl << "�� ������ ������� ���� �������:" << endl << endl;
				cout << "�|";
				cout << "����� ��������      |";
				cout << "����� �����������        |";
				cout << "����� ���������� " << endl;
			}
			while (edit->next != NULL) {
				cout << edit->n << "|";
				cout.width(20);
				cout.fill(' ');
				cout << left << edit->el->num << "|";
				cout.width(25);
				cout << left << edit->el->start << "|";
				cout << left << edit->el->end << endl;
				edit = edit->next;
			}
			if (flag == 0) {
				del_ed(edit_head);
			}

		}
		return(edit_head);
	}
}

void del_ed(marsh_list* edit_head) {
	marsh_list* edit;
	edit = edit_head;
	while (edit->next != NULL) {
		edit = edit_head->next;
		delete edit_head;
		edit_head = edit;

	}

	delete(edit_head);
	edit_head = NULL;
}

marsh* my_swap(marsh*&head, marsh* el1, marsh* el2) {

	if (el1 == head) {
		head->next = el2->next;
		el2->next = head;
		el1 = head;
		head = el2;
		return head;
	}
	else {
		marsh* prev, *temp;
		temp = head;
		while (temp->next != el1) {
			temp = temp->next;
		}
		prev = temp;
		if (el2->next == NULL) {
			prev->next = el2;
			el2->next = el1;
			el1->next = NULL;
			return el2;
		}
		else {
			prev->next = el2;
			el1->next = el2->next;
			el2->next = el1;
			return el2;
		}
	}
}

void sort(marsh* &head, int flag) {
	//������� ����������
	//� ������� flag ���������� ������ ����������
	//1 - �� �����������, 2 - �� ��������, 3 - �� ������
	marsh* temp;
	int k = 0;//���������� ��������� � �������
	temp = head;
	while (temp->next != NULL) {//������ ������ ��� �������� ���������� ���������
		switch (flag) {
		case 1: {
			if (strcmp(temp->start, temp->next->start) != 0) {
				if (strcmp(temp->start, temp->next->start) > 0) {
					temp = my_swap(head, temp, temp->next);
				}
			}
			else {
				if (strcmp(temp->num, temp->next->num) != 0) {
					if (strcmp(temp->num, temp->next->num) > 0) {
						temp = my_swap(head, temp, temp->next);
					}
				}
				else {
					if (strcmp(temp->end, temp->next->end) != 0) {
						if (strcmp(temp->end, temp->next->end) > 0) {
							temp = my_swap(head, temp, temp->next);
						}
					}
				}
			}
			break;
		}
		case 2: {
			if (strcmp(temp->end, temp->next->end) != 0) {
				if (strcmp(temp->end, temp->next->end) > 0) {
					temp = my_swap(head, temp, temp->next);
				}
			}
			else {
				if (strcmp(temp->num, temp->next->num) != 0) {
					if (strcmp(temp->num, temp->next->num) > 0) {
						temp = my_swap(head, temp, temp->next);
					}
				}
				else {
					if (strcmp(temp->start, temp->next->start) != 0) {
						if (strcmp(temp->start, temp->next->start) > 0) {
							temp = my_swap(head, temp, temp->next);
						}
					}
				}
			}
			break;
		}
		case 3: {
			if (strcmp(temp->num, temp->next->num) != 0) {
				if (strcmp(temp->num, temp->next->num) > 0) {
					temp = my_swap(head, temp, temp->next);
				}
			}
			else {
				if (strcmp(temp->start, temp->next->start) != 0) {
					if (strcmp(temp->start, temp->next->start) > 0) {
						temp = my_swap(head, temp, temp->next);
					}
				}
				else {
					if (strcmp(temp->end, temp->next->end) != 0) {
						if (strcmp(temp->end, temp->next->end) > 0) {
							temp = my_swap(head, temp, temp->next);
						}
					}
				}
			}
			break;
		}
		}
		temp = temp->next;
		k++;
	}
	temp = head;
	int f;
	for (int i = 0; i < k; i++) {//����������� ���������� ���������, � �������������� ����� ����������� k
		f = 0;//���� ������� ������������. ���� �� ���� ������� �� ����� - ������ ������������
		for (int j = 0; j < k - i; j++) {
			switch (flag) {
			case 1: {
				if (strcmp(temp->start, temp->next->start) != 0) {
					if (strcmp(temp->start, temp->next->start) > 0) {
						temp = my_swap(head, temp, temp->next);
						f++;
					}
				}
				else {
					if (strcmp(temp->num, temp->next->num) != 0) {
						if (strcmp(temp->num, temp->next->num) > 0) {
							temp = my_swap(head, temp, temp->next);
							f++;
						}
					}
					else {
						if (strcmp(temp->end, temp->next->end) != 0) {
							if (strcmp(temp->end, temp->next->end) > 0) {
								temp = my_swap(head, temp, temp->next);
								f++;
							}
						}
					}
				}
				break;
			}
			case 2: {
				if (strcmp(temp->end, temp->next->end) != 0) {
					if (strcmp(temp->end, temp->next->end) > 0) {
						temp = my_swap(head, temp, temp->next);
						f++;
					}

				}
				else {
					if (strcmp(temp->num, temp->next->num) != 0) {
						if (strcmp(temp->num, temp->next->num) > 0) {
							temp = my_swap(head, temp, temp->next);
							f++;
						}

					}
					else {
						if (strcmp(temp->start, temp->next->start) != 0) {
							if (strcmp(temp->start, temp->next->start) > 0) {
								temp = my_swap(head, temp, temp->next);
								f++;
							}

						}
					}
				}
				break;
			}
			case 3: {
				if (strcmp(temp->num, temp->next->num) != 0) {
					if (strcmp(temp->num, temp->next->num) > 0) {
						temp = my_swap(head, temp, temp->next);
						f++;
					}

				}
				else {
					if (strcmp(temp->start, temp->next->start) != 0) {
						if (strcmp(temp->start, temp->next->start) > 0) {
							temp = my_swap(head, temp, temp->next);
							f++;
						}

					}
					else {
						if (strcmp(temp->end, temp->next->end) != 0) {
							if (strcmp(temp->end, temp->next->end) > 0) {
								temp = my_swap(head, temp, temp->next);
								f++;
							}

						}
					}
				}
				break;
			}
			}
			temp = temp->next;
		}
		if (f == 0) {
			break;
		}
		temp = head;
	}
}

void list_out(marsh* head) {
	if (head == NULL) {
		cout << "������ ��������� ����!" << endl;
	}
	else {
		int menu = 0;
		marsh* temp;
		int k = 1;
		temp = head;
		cout << "�|";
		cout << "����� ��������      |";
		cout << "����� �����������        |";
		cout << "����� ���������� " << endl;
		while (temp != NULL) {
			cout << k << "|";
			cout.width(20);
			cout.fill(' ');
			cout << left << temp->num << "|";
			cout.width(25);
			cout << left << temp->start << "|";
			cout << left << temp->end << endl;
			k++;
			temp = temp->next;
		}
	}
}


void load_from_base(char name_base[], struct marsh *&head) {
	struct marsh *temp;
	temp = head;
	char first = 0;
	char data[3][25];
	int flag_err0 = 0, flag_err = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 25; j++) {
			data[i][j] = 0;
		}
	}
	ifstream f;
	f.open(name_base);
	if (f.fail() || !f.is_open()) {
		f.close();
		ofstream fout;
		fout.open(name_base);
		if (fout.fail() || !fout.is_open()) {
			flag_err0 = 1;
			cout << "������ �������� ���� ������" << endl << endl;
			fout.close();

		}
		else {
			flag_err0 = 1;
			cout << "���� ������ �����" << endl << endl;
			fout.close();
			f.open(name_base);
			if (f.fail() || !f.is_open()) {
				flag_err0 = 1;
				cout << "���� �� ������" << endl;
			}
		}
	}
	else {
		f.get(first);
		if (first == '\0') {
			flag_err0 = 1;
			cout << "���� ������ �����" << endl << endl;
		}
	}
	if (!f.eof()) {
		f.seekg(0, ios_base::beg);
		f.getline(data[0], 25, '#');
		if (f.fail() || data[0][0] == '\0') {
			f.clear();
			f.ignore(100, '\n');
			flag_err = 1;
		}
		f.getline(data[1], 25, '#');
		if (f.fail() || data[1][0] == '\0') {
			f.clear();
			f.ignore(100, '\n');
			flag_err = 1;
		}
		f.getline(data[2], 5, '\n');
		if (f.fail() || data[1][0] == '\0') {
			f.clear();
			f.ignore(100, '\n');
			flag_err = 1;
		}
		if (flag_err == 0) {
			head = init(data);
		}
		else {
			cout << "������ ��� ������ ������� 1 ���� ������. ������ ����� ���������" << endl;
		}
	}
	temp = head;
	int n = 2;
	while (!f.eof()) {
		flag_err = 0;
		f.getline(data[0], 25, '#');
		if (f.fail() || data[0][0] == '\0') {
			f.clear();
			//f.ignore(f.rdbuf()->in_avail());
			f.ignore(100, '\n');
			flag_err = 1;
		}
		f.getline(data[1], 25, '#');
		if (f.fail() || data[1][0] == '\0') {
			f.clear();
			//f.ignore(f.rdbuf()->in_avail());
			f.ignore(100, '\n');
			flag_err = 1;
		}
		f.getline(data[2], 5, '\n');
		if (f.fail() || data[2][0] == '\0') {
			f.clear();
			//f.ignore(f.rdbuf()->in_avail());
			f.ignore(100, '\n');
			flag_err = 1;
		}
		if (flag_err == 0) {
			add_el(head, data);
		}
		else {
			if ((data[0][0] == '\0') && (data[1][0] == '\0') && (data[2][0] == '\0')) {
				break;
			}
			cout << "������ ��� ������ ������� " << n << " ���� ������. ������ ����� ���������" << endl;
		}
		n++;
	}
	if ((flag_err == 0) && (flag_err0 == 0)) {
		cout << "���� ������ ������� ���������!" << endl;
	}
}

void save_base(char name_base[], struct marsh *head) {
	marsh*temp;
	ofstream fout;
	fout.open(name_base);
	if (fout.fail() || !fout.is_open()) {
		cout << "������ �������� ���� ������" << endl << endl;
		fout.close();
	}
	else {
		if (head != NULL) {
			temp = head;
			while (temp != NULL) {
				fout << temp->start;
				fout << '#';
				fout << temp->end;
				fout << '#';
				fout << temp->num;
				fout << '\n';
				temp = temp->next;
			}
			fout.close();
			cout << "���� ������� ���������" << endl;
		}
		else {
			cout << "������ ��������� ����!" << endl;
		}
	}
}

void del_all(struct marsh *head) {
	if (head != NULL) {
		struct marsh *temp = head;
		while (temp->next != NULL) {
			temp = head->next;
			delete (head);
			head = temp;
		}
		delete (head);
		head = NULL;
	}
}

void leakes() {
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();
}


int main() {
	setlocale(LC_ALL, "rus");
	//setlocale(LC_ALL, "Russian_Russia.1251");
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);
	char name_base[100] = "D:\\�����\\������ ����������������\\��������\\base.txt";
	cout << "�������� ���� ������ �� ���������..." << endl;
	marsh *head = NULL;
	char data[3][25];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 25; j++) {
			data[i][j] = 0;
		}
	}
	char* data_p = &data[0][0];
	load_from_base(name_base, head);
	int menu = 0;
	int flag_break = 0;
	while (true) {
		cout << endl << "�������� ���������� ��������:" << endl;
		cout << "1. �������� �������" << endl;
		cout << "2. �������� �������" << endl;
		cout << "3. ����� �������" << endl;
		cout << "4. ������� �������" << endl;
		cout << "5. ����������� ��� ��������" << endl;
		cout << "6. ���������� ������" << endl;
		cout << "7. ��������� ����� ����" << endl;
		cout << "8. ��������� ������� ����" << endl;
		cout << "9. �����" << endl;
		cout << "��� �����: ";
		cin >> menu;
		menu = check_menu(menu, 9);


		switch (menu) {
		case 1: {
			int flag = 0;
			data_p = in_data(data, 0);
			flag = is_equal(head, data);
			while (flag == 1) {
				int ch = 0;
				cout << "����� ������� ��� ����������!" << endl;
				cout << "������ ������ ����� ������? 1- ��, 2 - ���" << endl;
				cin >> ch;
				ch = check_menu(ch, 2);
				if (ch == 1) {
					flag = 0;
					data_p = in_data(data, 0);
					flag = is_equal(head, data);
				}
				else {
					break;
				}
			}
			if (flag == 0) {
				add_el(head, data);
			}
			break;
		}
		case 2: {
			edit(head);
			break;
		}
		case 3: {
			int k = 1;
			search(head, 0, k);
			break;
		}
		case 4: {
			del_el(head);
			break;
		}
		case 5: {
			list_out(head);
			break;
		}
		case 6: {
			if (head != NULL) {
				int menu_s = 0;
				cout << "�������� ������ ����������: " << endl;
				cout << "1. ����������� �� ������ �����������" << endl;
				cout << "2. ����������� �� ������ ����������" << endl;
				cout << "3. ����������� �� ������ ��������" << endl;
				cout << "������� ��� �����: ";
				cin >> menu_s;
				menu_s = check_menu(menu_s, 3);
				sort(head, menu_s);
				cout << "������ ������������!" << endl;
			}
			else {
				cout << "������ ��������� ����!" << endl;
			}
			break;
		}
		case 7: {
			int menu_v = 0;
			cout << "��������, ��� �� ����������� ������ ����� ��������, ��� ������ ����� ����!" << endl;
			cout << "��������� ���������?" << endl;
			cout << "1. ��" << endl;
			cout << "2. ���" << endl;
			cout << "��� �����: ";
			cin >> menu_v;
			menu_v = check_menu(menu_v, 2);
			if (menu_v == 1) {
				save_base(name_base, head);
			}
			cout << "������� ���� � ������ ����� ��� ���� ������: ";
			cin >> name_base;
			del_all(head);
			load_from_base(name_base, head);
			break;
		}
		case 8: {
			save_base(name_base, head);
			break;
		}
		case 9: {
			int menu_v = 0;
			cout << "��������� ���������?" << endl;
			cout << "1. ��" << endl;
			cout << "2. ���" << endl;
			cout << "��� �����: ";
			cin >> menu_v;
			menu_v = check_menu(menu_v, 2);
			if (menu_v == 1) {
				save_base(name_base, head);
			}
			cout << "���������� ������..." << endl;
			flag_break = 1;
			break;
		}
		}

		if (flag_break == 1) {
			break;
		}

	}

	del_all(head);
	leakes();
	system("pause");
	return 0;
}