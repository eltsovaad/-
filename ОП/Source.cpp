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
	struct marsh* next;//указатель на следующий элемент
};

struct marsh_list
{
	int n;
	struct marsh* el;
	struct marsh_list* next;//указатель на следующий элемент
};

struct marsh * init(char data[][25]) //Инициализация первого элемента списка,где а- значение первого узла
{
	struct marsh *head;
	// выделение памяти под корень списка
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

void del_el(marsh *&head)//удаление одного элемента списка
{
	if (head == NULL) {
		cout << "Список маршрутов пуст!" << endl;
	}
	else {
		struct marsh *prev, *del;
		del = NULL;
		struct marsh_list *del_edit, *del_head;
		int choice = -1, k = 1;
		del_edit = search(head, 1, k);
		del_head = del_edit;
		if (k > 2) {
			cout << "Введите порядковый номер нужного маршрута из найденных, или 0, если хотите удалить всё: ";
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
					while (prev->next != del) // просматриваем список начиная с корня
					{ // пока не найдем узел, предшествующий del
						prev = prev->next;
					}
					if (del->next == NULL) {
						prev->next = NULL;
					}
					else {
						prev->next = del->next; // переставляем указатель
					}
					delete(del);
					del = NULL;// освобождаем память удаляемого узла
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
						del = NULL;// освобождаем память удаляемого узла
					}
				}


				cout << "Удаление произошло успешно!" << endl;

			}
			else {
				cout << "Удаление невозможно!" << endl;
			}
		}
		del_ed(del_head);
	}
}

void add_el(marsh *&head, char data[][25])//добавление элемента
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
		//p = last->next; // сохранение указателя на следующий элемент
		last->next = temp; // предыдущий узел указывает на создаваемый
		strcpy_s(temp->start, data[0]); // сохранение поля данных добавляемого узла
		strcpy_s(temp->end, data[1]);
		strcpy_s(temp->num, data[2]);
		temp->next = NULL; // созданный узел указывает на следующий элемент
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
		cout << "Неверный ввод. Повторите: ";
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
	//flag==0 - добавление элемента ==1 - поиск по отправлению 
	//==2 - поиск по назначению ==3 - по номеру
	std::locale loc("Russian_Russia.1251");
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	if ((flag == 0) || (flag == 1)) {
		cout << "Введите пункт отправления: ";
		cin.getline(data[0], 24, '\n');
		if (isupper(data[0][0], loc) == 0) {
			data[0][0] = (char)toupper(data[0][0]);
			cout << "Строчная буква преобразована!" << endl;
		}
		while (cin.fail() /*|| (isupper(data[0][0], loc) == 0)*/) {
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Название пункта не должно превышать 25 символов" << endl;
			cout << "Введите название повторно: ";
			cin.getline(data[0], 25, '\n');
		}
	}
	if ((flag == 0) || (flag == 2)) {
		cout << "Введите пункт назначения: ";
		cin.getline(data[1], 25, '\n');
		if (isupper(data[1][0], loc) == 0) {
			data[1][0] = (char)toupper(data[1][0]);
			cout << "Строчная буква преобразована!" << endl;
		}
		while (cin.fail() /*|| (isupper(data[1][0], loc) == 0)*/) {
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Название пункта не должно превышать 25 символов" << endl;
			cout << "Введите название повторно: ";
			cin.getline(data[1], 25, '\n');
		}
	}
	if ((flag == 0) || (flag == 3)) {
		cout << "Введите номер маршрута: ";
		cin.getline(data[2], 5, '\n');
		while (cin.fail()) {
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Номер содержит больше пяти символов!" << endl;
			cout << "Введите номер маршрута повторно: ";
			cin.getline(data[2], 5, '\n');
		}
	}
	//написать is_equal
	char* data_p = &data[0][0];
	return data_p;
}

void edit(marsh *head)//изменение элемента
{
	if (head == NULL) {
		cout << "Список маршрутов пуст!" << endl;
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
			cout << "Введите порядковый номер нужного маршрута из найденных: ";
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
			cout << "Введите новые значения для полей маршрута: " << endl;
			data_p = in_data(data, 0);
			int flag_eq = 0;
			flag_eq = is_equal(head, data);
			while (flag_eq == 1) {
				int ch = 0;
				cout << "Такой маршрут уже существует!" << endl;
				cout << "Хотите ввести новые данные? 1- Да, 2 - Нет" << endl;
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
			cout << "Изменение произошло успешно!" << endl;
			del_ed(edit_head);
		}
		else {
			cout << "Изменение невозможно!" << endl;
			delete edit_head;
			edit_head = NULL;
		}

	}
}


int is_equal(marsh* head, char data[][25]) {
	marsh* temp;
	temp = head;
	int flag = 0;//0 -если нет полного совпадения, 1 если есть
	while (temp != NULL) {
		if ((strcmp(data[2], temp->num) == 0) && (strcmp(temp->start, data[0]) == 0) && (strcmp(data[1], temp->end) == 0)) {
			flag = 1;
			break;
		}
		temp = temp->next;
	}
	return flag;
}



struct marsh_list* search(struct marsh* head, int flag, int &k)//поиск элемента
{//flag 0 - поиск с выводом, 1 - поиск для del и edit
	if (head == NULL) {
		cout << "Список маршрутов пуст!" << endl;
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
		int fl_nothing = 1;//если ничего не было найдено ==1
		int choice = 1;
		k = 1;
		cout << "Выберите способ поиска: " << endl;
		cout << "1. По пункту отправления" << endl;
		cout << "2. По пункту назначения" << endl;
		cout << "3. По номеру маршрута" << endl;
		cout << "Введите ваш выбор: ";
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
			cout << "Ничего не было найдено" << endl << endl;
			temp = NULL;
			delete(edit_head);
			edit_head = NULL;
		}
		else {
			edit = edit_head;
			if (k > 1) {
				cout << endl << "По вашему запросу было найдено:" << endl << endl;
				cout << "№|";
				cout << "Номер маршрута      |";
				cout << "Пункт отправления        |";
				cout << "Пункт назначения " << endl;
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
	//функция сортировки
	//с помощью flag выбирается способ сортировки
	//1 - по отправлению, 2 - по прибытию, 3 - по номеру
	marsh* temp;
	int k = 0;//количество элементов в массиве
	temp = head;
	while (temp->next != NULL) {//первый пробег для подсчета количества элементов
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
	for (int i = 0; i < k; i++) {//стандартная сортировка пузырьком, с использованием ранее полученного k
		f = 0;//флаг наличия перестановок. Если не было сделано ни одной - массив отсортирован
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
		cout << "Список маршрутов пуст!" << endl;
	}
	else {
		int menu = 0;
		marsh* temp;
		int k = 1;
		temp = head;
		cout << "№|";
		cout << "Номер маршрута      |";
		cout << "Пункт отправления        |";
		cout << "Пункт назначения " << endl;
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
			cout << "Ошибка создания базы данных" << endl << endl;
			fout.close();

		}
		else {
			flag_err0 = 1;
			cout << "База данных пуста" << endl << endl;
			fout.close();
			f.open(name_base);
			if (f.fail() || !f.is_open()) {
				flag_err0 = 1;
				cout << "Файл не открыт" << endl;
			}
		}
	}
	else {
		f.get(first);
		if (first == '\0') {
			flag_err0 = 1;
			cout << "База данных пуста" << endl << endl;
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
			cout << "Ошибка при чтении строчки 1 базы данных. Строка будет пропущена" << endl;
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
			cout << "Ошибка при чтении строчки " << n << " базы данных. Строка будет пропущена" << endl;
		}
		n++;
	}
	if ((flag_err == 0) && (flag_err0 == 0)) {
		cout << "База данных успешно загружена!" << endl;
	}
}

void save_base(char name_base[], struct marsh *head) {
	marsh*temp;
	ofstream fout;
	fout.open(name_base);
	if (fout.fail() || !fout.is_open()) {
		cout << "Ошибка создания базы данных" << endl << endl;
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
			cout << "База успешно сохранена" << endl;
		}
		else {
			cout << "Список маршрутов пуст!" << endl;
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
	char name_base[100] = "D:\\учеба\\Основы программирования\\Курсовая\\base.txt";
	cout << "Загрузка базы данных по умолчанию..." << endl;
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
		cout << endl << "Выберете дальнейшее действие:" << endl;
		cout << "1. Добавить маршрут" << endl;
		cout << "2. Изменить маршрут" << endl;
		cout << "3. Найти маршрут" << endl;
		cout << "4. Удалить маршрут" << endl;
		cout << "5. Просмотреть все маршруты" << endl;
		cout << "6. Сортировка списка" << endl;
		cout << "7. Загрузить новую базу" << endl;
		cout << "8. Сохранить текущую базу" << endl;
		cout << "9. Выход" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
		menu = check_menu(menu, 9);


		switch (menu) {
		case 1: {
			int flag = 0;
			data_p = in_data(data, 0);
			flag = is_equal(head, data);
			while (flag == 1) {
				int ch = 0;
				cout << "Такой маршрут уже существует!" << endl;
				cout << "Хотите ввести новые данные? 1- Да, 2 - Нет" << endl;
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
				cout << "Выберите способ сортировки: " << endl;
				cout << "1. Сортировать по пункту отправления" << endl;
				cout << "2. Сортировать по пункту назначения" << endl;
				cout << "3. Сортировать по номеру маршрута" << endl;
				cout << "Введите ваш выбор: ";
				cin >> menu_s;
				menu_s = check_menu(menu_s, 3);
				sort(head, menu_s);
				cout << "Список отсортирован!" << endl;
			}
			else {
				cout << "Список маршрутов пуст!" << endl;
			}
			break;
		}
		case 7: {
			int menu_v = 0;
			cout << "Внимание, все не сохраненные данные будут потеряны, при выборе новой базы!" << endl;
			cout << "Сохранить изменения?" << endl;
			cout << "1. Да" << endl;
			cout << "2. Нет" << endl;
			cout << "Ваш выбор: ";
			cin >> menu_v;
			menu_v = check_menu(menu_v, 2);
			if (menu_v == 1) {
				save_base(name_base, head);
			}
			cout << "Введите путь к новому файлу для базы данных: ";
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
			cout << "Сохранить изменения?" << endl;
			cout << "1. Да" << endl;
			cout << "2. Нет" << endl;
			cout << "Ваш выбор: ";
			cin >> menu_v;
			menu_v = check_menu(menu_v, 2);
			if (menu_v == 1) {
				save_base(name_base, head);
			}
			cout << "Завершение работы..." << endl;
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