#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

int GetNumberOfCommand();

struct Node
{
	int digit;
	Node* next = nullptr;
	Node* prev = nullptr;
};

class List
{
	Node *head, *tail;
	unsigned size;
public:
	List() {
		head = tail = nullptr;
		size = 0;
	}

	Node* get_head() { return head; }
	Node* get_tail() { return tail; }

	void call_func_by_number(int i) {
		int digit, pos, count, j;
		string str;

		if (i == 1 || i == 2) {
			while (1) {
				try {
					cout << "Enter the value you want to add: ";
					cin >> str;
					digit = stoi(str);
					if (i == 1) push_back(digit);
					else push_front(digit);
					print_list();
					break;
				}
				catch (const exception& e) {
					throw invalid_argument("Incorrect value was entered.\n");
				}
			}
		}
		else if (i == 3 || i == 4) {
			if (size == 0) cout << "The list is empty.\n";
			else {
				if (i == 3) delete_last();
				else delete_first();
				print_list();
			}
		}
		else if (i == 5) {
			while (1) {
				try {
					cout << "Enter the value you want to add: ";
					cin >> str;
					digit = stoi(str);
					break;
				}
				catch (const exception& e) {
					cout << "Incorrect value was entered.\n";
				}
			}
			
			while (1) {
				try {
					cout << "Enter the index (from 0 to " << size << "): ";
					cin >> str;
					pos = stoi(str);
					if (pos < 0 || pos > size) throw exception();
					else break;
				}
				catch (const exception& e) {
					cout << "Incorrect digit entered.\n";
				}
			}
			push_element_by_index(digit, pos);
			print_list();
		}
		else if (i == 6 || i == 7 || i == 10) {
			if (size == 0) cout << "The list is empty.\n";
			else {
				while (1) {
					try {
						cout << "Enter the index " <<
							"(from 0 to " << (size - 1) << ") : ";
						cin >> str;
						pos = stoi(str);
						if (pos < 0 || pos >= size) throw exception();
						else break;
					}
					catch (const exception& e) {
						cout << "Incorrect digit entered.\n";
					}
				}
				if (i == 6)
					cout << "The element you got: " << get_element_by_index(pos)->digit << endl;
				else if (i == 7) delete_element_by_index(pos);
				else {
					while (1) {
						try {
							cout << "Enter the new value: ";
							cin >> str;
							digit = stoi(str);
							set_element_by_index(digit, pos);
							break;
						}
						catch (const exception& e) {
							cout << "Incorrect value entered.\n";
						}
					}
					
				}
				print_list();
			}
		}
		else if (i == 8) {
			cout << "The length the list: " << get_size() << endl;
		}
		else if (i == 9) {
			clear();
			cout << "Done. The list is clear.\n";
			print_list();
		}
		else if (i == 11) {
			if (is_clear()) cout << "The list is empty.\n";
			else cout << "The list is not empty.\n";
		}
		else if (i == 12) {
			if (size == 0) cout << "The list is empty.\n";
			else {
				reverse();
				print_list();
			}
		}
		else if (i ==  13 || i == 14 || i == 15 || i == 16 || i == 17 || i == 18) {
			while (1) {
				try {
					if (i == 16)
						cout << "Enter the length of the sublist: ";
					else
						cout << "Enter the length of your list: ";
					cin >> str;
					count = stoi(str);
					break;
				}
				catch (const exception& e) {
					cout << "Incorrect value was entered.\n";
				}
			}

			List NewList;
			j = 1;
			while (j < (count + 1)) {
				while (1) {
					try {
						cout << "\tEnter " << j << " element of your list: ";
						cin >> str;
						digit = stoi(str);
						NewList.push_back(digit);
						break;
					}
					catch (const exception& e) {
						cout << "Incorrect value was entered.\n";
					}
				}
				j++;
			}
			cout << endl;

			if (i == 13) {;
				while (1) {
					try {
						cout << "Enter the index from which the new list will start (from 0 to " << size << "): ";
						cin >> str;
						pos = stoi(str);
						break;
					}
					catch (const exception& e) {
						cout << "Incorrect value was entered.\n";
					}
				}
			}

			if (i == 13) push_list_by_index(&NewList, pos);
			else if (i == 14) push_list_back(&NewList);
			else if (i == 15) push_list_beg(&NewList);
			else if (i == 16) {
				if (is_list_in(NewList)) cout << "Your list is here.\n";
				else cout << "Your list is not here.\n";
			}
			else if (i == 17)
				cout << "Index of the first inclusion: " << find_first_inclusion(NewList) << endl;
			else
				cout << "Index of the last inclusipon: " << find_last_inclusion(NewList) << endl;
			print_list();
		}
		else if (i == 19) {
			while (1) {
				try {
					cout << "Enter the index of the first element (from 0 to " << (size - 1) << "): ";
					cin >> str;
					pos = stoi(str);
					if (pos < 0 || pos >= size) {
						throw out_of_range("Incorrect value was entered.");
					}
					else break;
				}
				catch (const out_of_range& e) {
					cout << e.what() << endl;
				}
			}
			while (1) {
				try {
					cout << "Enter the index of the second element (from 0 to " << (size - 1) << "): ";
					cin >> str;
					j = stoi(str);
					if (j < 0 || j >= size) {
						throw out_of_range("Incorrect value was entered.");
					}
					else break;
				}
				catch (const exception& e) {
					cout << e.what() << endl;
				}
			}
			permutation(pos, j);
			print_list();
		}
	}
	void print_list()
	{
		Node* tmp = head;
		cout << "Elements of the current list:\n";
		while (tmp != nullptr) {
			cout << tmp->digit << " ";
			tmp = tmp->next;
		}
		cout << "\n\n";
	}
	void push_back(int digit)
	{
		if (tail == nullptr) {
			tail = new Node;
			tail->digit = digit;
			head = tail;
		}
		else {
			tail->next = new Node;
			tail->next->digit = digit;
			tail->next->prev = tail;
			tail = tail->next;
		}
		size++;
	}
	void push_front(int digit)
	{
		if (head == nullptr) {
			head = new Node;
			head->digit = digit;
			tail = head;
		}
		else {
			head->prev = new Node;
			head->prev->digit = digit;
			head->prev->next = head;
			head = head->prev;
		}
		size++;
	}
	void delete_last() {
		if (size != 0) {
			Node* tmp = tail;
			tail = tail->prev;
			delete tmp;
			if (tail != nullptr)
				tail->next = nullptr;
			size--;
		}
	}
	void delete_first() {
		if (size != 0) {
			Node* tmp = head;
			head = head->next;
			delete tmp;
			if (head != nullptr)
				head->prev = nullptr;
			size--;
		}
	}
	void push_element_by_index(int digit, int index) {
		if (head == nullptr) {
			head = new Node;
			head->digit = digit;
			tail = head;
		}
		else if (index == 0) {
			Node* tmp = head;
			head->prev = new Node;
			head->prev->digit = digit;
			head = head->prev;
			head->next = tmp;
		}
		else if (index == size) {
			Node* tmp = tail;
			tail->next = new Node;
			tail->next->digit = digit;
			tail = tail->next;
			tail->prev = tmp;
		}
		else {
			int i = 1;
			Node* tmp = head;
			while (i != index) {
				i++;
				tmp = tmp->next;
			}
			Node* tmp2 = tmp->next;
			tmp->next = new Node;
			tmp->next->digit = digit;
			tmp2->prev = tmp->next;
			tmp->next->prev = tmp;
			tmp->next->next = tmp2;
		}
		size++;
	}
	Node* get_element_by_index(int pos) {
		int i = 0;
		Node* tmp = head;
		while (i != pos) {
			tmp = tmp->next;
			i++;
		}
		return tmp;
	}
	void delete_element_by_index(int pos) {
		int i = 0;
		Node* tmp = head;
		while (i != pos) {
			tmp = tmp->next;
			i++;
		}
		if (tmp == head) {
			Node* tmp = head;
			head = head->next;
			delete tmp;
			head->prev = nullptr;
		}
		else if (tmp == tail) {
			Node* tmp = tail;
			tail = tail->prev;
			delete tmp;
			tail->next = nullptr;
		}
		else {
			tmp->prev->next = tmp->next;
			tmp->next->prev = tmp->prev;
			delete tmp;
		}
		size--;
	}
	unsigned get_size() {
		return size;
	}
	void clear() {
		Node* tmp = head, * tmp2;
		while (tmp != nullptr) {
			tmp2 = tmp;
			tmp = tmp->next;
			delete tmp2;
		}
		head = tail = nullptr;
		size = 0;
	}
	void set_element_by_index(int digit, int pos) {
		int i = 0;
		Node* tmp = head;
		while (i != pos) {
			tmp = tmp->next;
			i++;
		}
		tmp->digit = digit;
	}
	bool is_clear() {
		if (size == 0) return 1;
		else return 0;
	}
	void reverse() {
		if (size > 1) {
			Node* tmp1 = head;
			Node* tmp2 = tail;
			int tmp;
			if (size % 2 == 0) {
				do {
					tmp = tmp1->digit;
					tmp1->digit = tmp2->digit;
					tmp2->digit = tmp;

					tmp1 = tmp1->next;
					tmp2 = tmp2->prev;
				} 
				while (tmp2->next != tmp1);
			}
			else {
				do {
					tmp = tmp1->digit;
					tmp1->digit = tmp2->digit;
					tmp2->digit = tmp;

					tmp1 = tmp1->next;
					tmp2 = tmp2->prev;
				} while (tmp1 != tmp2);
			}
		}
	}
	void push_list_by_index(List* list, int index) {
		if (index == 0) push_list_beg(list);
		else if (index == size) push_list_back(list);
		else {
			int ind = 1;
			Node* tmp = head;
			while (ind != index) {
				ind++;
				tmp = tmp->next;
			}
			tmp->next->prev = list->get_tail();
			list->get_tail()->next = tmp->next;
			tmp->next = list->get_head();
			list->get_head()->prev = tmp;
			size += list->get_size();
		}
	}
	void push_list_back(List* list) {
		tail->next = list->get_head();
		list->get_head()->prev = tail;
		tail = list->get_tail();
		size += list->get_size();
	}
	void push_list_beg(List* list) {
		head->prev = list->get_tail();
		list->get_tail()->next = head;
		head = list->get_head();
		size += list->get_size();
	}
	bool is_list_in(List list) {
		int index = 0, index_tmp = 0;
		int index2 = 1;
		while (index < size) {
			if (get_element_by_index(index)->digit == list.get_element_by_index(0)->digit) {
				if (list.get_size() == 1) return 1;

				index_tmp = index++;
				while (index < size) {
					if (get_element_by_index(index)->digit == list.get_element_by_index(index2)->digit) {
						index++;
						index2++;
						if (index2 == list.get_size()) return 1;
					}
					else {
						index = index_tmp;
						index2 = 1;
						break;
					}
				}
			}
			index++;
		}

		return 0;
	}
	int find_first_inclusion(List list) {
		int index = 0, index_tmp = 0;
		int index2 = 1;
		while (index < size) {
			if (get_element_by_index(index)->digit == list.get_element_by_index(0)->digit) {
				if (list.get_size() == 1) return index;

				index_tmp = ++index;
				while (index < size) {
					if (get_element_by_index(index)->digit == list.get_element_by_index(index2)->digit) {
						index++;
						index2++;
						if (index2 == list.get_size()) return (index_tmp - 1);
					}
					else {
						index = index_tmp;
						index2 = 1;
						break;
					}
				}
			}
			index++;
		}

		return -1;
	}
	int find_last_inclusion(List list) {
		int index = size-1, index_tmp = size-1;
		int index2 = list.get_size() - 2;
		while (index >= 0) {
			if (get_element_by_index(index)->digit == list.get_element_by_index(list.get_size() - 1)->digit) {
				if (list.get_size() == 1) return index;

				index_tmp = --index;
				while (index >= 0) {
					if (get_element_by_index(index)->digit == list.get_element_by_index(index2)->digit) {
						index--;
						index2--;
						if (index2 == -1) return (index+1);
					}
					else {
						index = index_tmp;
						index2 = list.get_size() - 2;
						break;
					}
				}
			}
			index--;
		}

		return -1;
	}
	void permutation(int ind1, int ind2) {
		Node* tmp1 = get_element_by_index(ind1);
		Node* tmp2 = get_element_by_index(ind2);
		int tmp = tmp1->digit;
		tmp1->digit = tmp2->digit;
		tmp2->digit = tmp;
	}
};

int main()
{
	List List;
	bool end = false;
	string str;
	setlocale(LC_ALL, "ru");
	SetConsoleCP(1251); // ��� ������ ������� ����	

	while (!end) {
		List.call_func_by_number(GetNumberOfCommand());
		while (1) {
			cout << "Do you want to continue? (yes/no)\n" <<
				"Your answer: ";
			cin >> str;
			if (str == "Yes" || str == "yes") {
				cout << "\n";
				break;
			}
			else if (str == "No" || str == "no") {
				end = true;
				break;
			}
			else {
				cout << "The answer is not defined.\n";
			}
		}
	}
	List.clear();
	
	return 0;
}

int GetNumberOfCommand() {
	cout << "Select one of the possible commands:\n" <<
		"\t1) add an element to the end;\n" <<
		"\t2) add an element to the start;\n" <<
		"\t3) delete the last element;\n" <<
		"\t4) delete the first element;\n" <<
		"\t5) add an element by index;\n" <<
		"\t6) get an element by index;\n" <<
		"\t7) delete an element by index;\n" <<
		"\t8) get the list size;\n" <<
		"\t9) delete all elements;\n" <<
		"\t10) replace an element by index;\n" <<
		"\t11) check for list emptiness;\n" <<
		"\t12) reverse the order of elements;\n" <<
		"\t13) insert another list starting from the index;\n" <<
		"\t14) insert another list at the end;\n" <<
		"\t15) insert another list at the beginning;\n" <<
		"\t16) check for the inclusion of another list;\n" <<
		"\t17) search for the index of the first inclusion of another list;\n" <<
		"\t18) search for the index of the last inclusion of another list;\n" <<
		"\t19) exchange of two list items by indexes.\n" <<
		"Your choice: ";
	try {
		int i; string str;
		cin >> str;
		i = stoi(str);
		if (i < 1 || i > 19) {
			throw exception();
		}
		cout << "\n";
		return i;
	}
	catch (const exception& e) {
		cout << "Incorrect number entered.\n";
		return -1;
	}
}