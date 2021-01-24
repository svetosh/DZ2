#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <cmath>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <algorithm>
#include "match.h"

using namespace std;

extern char *ch = NULL; //указатель на разбираемую строку
extern char *nexch = NULL; //тот же указатель только смотрит на символ вперед
map<string, float> var; //для учета переменной
string res = ""; //результат разбора

//функция для работы с рекурсивно-нисходящим анализатором
int first_value(int& val, int& minus);
int number_int(int& val);
int number_float(float& val);
int constant(float& val);
int variable();
int func();
int expr();
int mult();
int add();
float calc(vector<string> s);

//разделяем строку по пробелу
vector<string> razdel(string st) {
	vector<string> vec;
	string r = "";
	for (int i = 0; i < st.size(); i++) {
		if (st[i] == ' ' && r.size() > 0) {
			vec.push_back(r);
			r = "";
		}
		//пропускаем функции (их будем разбирать отдельно)
		else if (st[i] == '[') {
			int j = 1; //учитываем также вложенные функции
			while (st[i] != ']' || j > 1) {
				if (st[i] == '[') {
					j++;
				}
				//смотрим на 1 символ вперед
				else if ((st[i+1] == ']') && ((i+1) < st.size())) {
					j--;
				}
				r += st[i];
				i++;
			}
			r += st[i];
			vec.push_back(r);
			r = "";
		}
		else if(st[i] != ' '){ //если отличный от скобки и пробела символ
			r += st[i];
		}
	}
	//если ещё что-то осталось
	if (r.size() > 0) {
		vec.push_back(r);
	}
	return vec;
}

//открываем внешние скобки функции поскольку могут быть функции вида tg(tg(...(x)...)
float open(string s) {
	string next = "";
	if (s[s.size() - 1] == ']') {
		string name = ""; //пишем сюда название функции
		s.pop_back(); //убираем скобку
		while (s[s.size()-1] != ' ') {
			name += s[s.size() - 1];
			s.pop_back();
		}
		reverse(name.begin(), name.end()); //переворачиваем так как мы писали с конца
		//теперь смотрим что это за функция
		if (name == "cos") {
			//читаем все содержимое функции
			for (int i = 1; i < s.size() - 1; i++) {
				next += s[i];
			}
			return cos(calc(razdel(next))); //вычисляем
		}
		else if (name == "sin") {
			for (int i = 1; i < s.size() - 1; i++) {
				next += s[i];
			}
			return sin(calc(razdel(next)));
		}
		else if (name == "tg") {
			for (int i = 1; i < s.size() - 1; i++) {
				next += s[i];
			}
			return tan(calc(razdel(next)));
		}
		else if (name == "ctg") {
			for (int i = 1; i < s.size() - 1; i++) {
				next += s[i];
			}
			return 1.0/tan(calc(razdel(next)));
		}
		else if (name == "sqrt") {
			for (int i = 1; i < s.size() - 1; i++) {
				next += s[i];
			}
			return sqrt(calc(razdel(next)));
		}
		else if (name == "exp") {
			for (int i = 1; i < s.size() - 1; i++) {
				next += s[i];
			}
			return exp(calc(razdel(next)));
		}
		else if (name.find("pow") != string::npos){ //учет степени
			for (int i = 1; i < s.size() - 1; i++) {
				next += s[i];
			}
			string p = ""; //степень
			for (int i = 0; i < name.size() && name[i] != '_'; i++) {
				p += name[i];
			}
			return pow(calc(razdel(next)),atoi(p.c_str()));
		}
	}
}

//расчет постфиксного выражения
float calc(vector<string> s) {
	stack<float> x; //стек для расчета
	for (auto i : s) {
		//может быть как цифра, а может и символ ариф. операции
		if (i.size() == 1) {
			if (isdigit(i[0])) {
				x.push(atof(i.c_str()));
			}
			else {
				//скорее всего знак операции
				char c = i[0];
				//разбор по знаку
				switch (c) {
				case '+': {
					float a = x.top(); x.pop(); //снимаем два верхних числа со стека
					float b = x.top(); x.pop();
					x.push(a + b); //записываем результат
					break;
				}
				case '-': {
					float a = x.top(); x.pop();
					float b = x.top(); x.pop();
					x.push(b - a);
					break;
				}
				case '*': {
					float a = x.top(); x.pop();
					float b = x.top(); x.pop();
					x.push(a * b);
					break;
				}
				case '/': {
					float a = x.top(); x.pop();
					float b = x.top(); x.pop();
					if (abs(a) != 0) {
						x.push(b / a);
					}
					else {
						cerror("Ошибка при выполнении операции!");
						break;
					}
					break;
				}
				}
			}
		}
		else {
			//начало функции
			if (i[0] == '[') {
				//сначала опеределяем что это за функция после чего разбиваем её
				x.push(open(i));
			}
			else if (i[0] == '-' && i[1] == '[') {
				string w(i);
				w.erase(w.begin()); //убираем из разбора унарный минус
				x.push(-open(w));
			}
			else {
				//начало числа
				if (isdigit(i[0]) || (isdigit(i[1]) && i[0] == '-')) {
					x.push(atof(i.c_str()));
				}
				else {
					//непонятно что
					cerror("Ошибка при разборе выражения!");
					break;
				}
			}
		}
	}
	if (x.size() != 1) {
		cerror("В стеке находится больше чем одно значение!");
		return -1;
	}
	else {
		return x.top();
	}
}

//попалась константа pi или e - exp(1)
int constant(float& val) {
	char* str = ch;
	char* next = nexch;
	int minus = 1;
	//учет унарных знаков
	if (match("-")) {
		minus = -minus;
	}
	if (match("+")) {
		minus = minus;
	}
	if (match("pi")) {
		val = 3.1415926 * minus;
		return 1;
	}
	else if (*ch == 'e' && *nexch != 'x') {
		ch++; //нужно сместить тк не используем функцию match
		nexch++;
		val = exp(1) * minus;
		return 1;
	}
	ch = str; //чтобы не съедать унарный знак если он там будет
	next = nexch;
	return 0;
}

//разбор переменной
int variable() {
	int minus = 1;
	//учет унарных знаков
	if (match("-")) {
		minus = -1;
	}
	if (match("+")) {
		minus = 1;
	}
	char* str = ch;
	char* next = nexch;
	string name = "";
	while (alpha(*str)) {
		name += *str++;
	}
	if (*str == '(') {
		if (name == "exp" || name == "pow" || name == "sin" ||
			name == "cos" || name == "tg" || name == "ctg" || name == "sqrt") {
			return 0;
		}
	}
	else {
		ch = str; //обновляем указатели
		nexch = next;
		//скорее всего переменная
		if (var.find(name) != var.end()) { //уже встречалась ранее?
			res += to_string(var[name]);
			res += " ";
		}
		else { //встретилась в первый раз
			cout << "Введите значение переменной " << name << ": ";
			var[name] = 0;
			cin >> var[name];
			var[name] *= minus;
			res += to_string(var[name]);
			res += " ";
		}
		return 1;
	}
}

//разбор функций
int func() {
	char* str = ch;
	char* next = nexch;
	int minus = 1;
	//учет унарных знаков
	if (match("-")) {
		minus = -1;
	}
	if (match("+")) {
		minus = 1;
	}
	if (match("exp")) {
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0) {
			return 0;
		}
		res += "exp] ";
	}
	else if (match("sqrt")) {
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0) {
			return 0;
		}
		res += "sqrt] ";
	}
	else if (match("sin")) {
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0) {
			return 0;
		}
		res += "sin] ";
	}
	else if (match("cos")) {
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0) {
			return 0;
		}
		res += "cos] ";
	}
	else if (match("tg")) {
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0) {
			return 0;
		}
		res += "tg] ";
	}
	else if (match("ctg")) {
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0) {
			return 0;
		}
		res += "ctg] ";
	}
	else if (match("pow")) {
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0)
			return 0;
		res += "pow] ";
	}
	else { //если это не функция
		ch = str; //опять же, чтобы не есть унарный знак
		nexch = next;
		return 0;
	}
	skipblanks();
	return 1; //если это все таки функция
}

//разбор числа
int first_value(int& val, int& minus) {
	int k = 1;
	char c = '\0';
	//проверка унарного знака
	while (k) {
		k = 0;
		if (match("+")) 
			k = 1;
		if (match("-")) {
			minus = (-minus);
			k = 1;
		}
	}
	if (numeric(*ch) == 0)
		return false; //если не число
	while (numeric(*ch)) { //число оказалось больше одного разряда
		c = get_ch(); //получаем след символ
		k = k * 10 + (c - '0');
	}
	val = k;
	return true;
}

// проверка на целое число
int number_int(int& val) {
	char* str = ch; //для сброса глобального указателя
	char* next = nexch;
	int minus = 1;
	bool res = first_value(val, minus); //получаем первое число
	if (res) {
		if (minus < 0) val = -val;
		if (match(".")) { //число с плавующей точкой попалось
			ch = str; //сброс
			nexch = next;
			return 0;
		}
		else
			return 1;
	}
	else {
		ch = str; //также сброс если не число
		nexch = next;
	}
	return 0;
}

// проверка на вещественное число
int number_float(float& val) {
	char* str = ch;
	char* next = nexch;
	int minus = 1;
	int d = 0; //целая часть
	int m = 0; //дробная
	int l = 0; //длина дробной части
	char c = '\0';
	bool res = first_value(d, minus);
	if (res) {
		//если число
		if (minus < 0) val = -val;
		//разбор дробной части
		if (match(".")) {
			while (numeric(*ch)) {
				c = get_ch();
				m = m * 10 + (c - '0');
				l++;
			}
		}
		val = (float)d + (float)m / pow(10, l);
		return 1;
	}
	else { //попалось не число
		ch = str; //сброс
		nexch = next;
		return 0;
	}
}

//разбор выражения в скобках
int expr() {
	int s = 0;
	float f = 0.0f;
	//дальше скобочное выражение?
	if (match("(")) {
		if (add() == 0)
			return 0;
		//если это pow функция, то разбираем
		if (match(",")) {
			//степень
			if (number_int(s)) {
				res += to_string(s);
				res += "_";
			}
			else {
				cerror("syntax error");
				return 0;
			}
		}
		if (!match(")")) {
			cerror("mismatch ')'");
			return 0;
		}
	}
	else if (number_int(s)) { //целое число
		res += to_string(s);
		res += " ";
	}
	else if (number_float(f)) { //вещественное
		res += to_string(f);
		res += " ";
	}
	else if (constant(f)) { //константа
		res += to_string(f);
		res += " ";
	}
	else if (func()) { //функция
		return 1;
	}
	else if (variable()) { //переменная
		return 1;
	}
	else { //если непонятно что
		cerror("syntax error");
		return 0;
	}
	return 1;
}

//разбор умножения и деления
int mult() {
	//левый множитель
	if (expr() == 0)
		return 0;
	while (true) {
		if (match("*")) {
			//получаем правый множитель
			if (expr() == 0)
				return 0;
			res += "* "; //умножаем
		}
		else if (match("/")) {
			if (expr() == 0)
				return 0;
			res += "/ ";
		}
		else break;
	}
	return 1;
}

//разбор сложения и вычитания
int add() {
	//аналогично как и с умножением
	if (mult() == 0)
		return 0;
	while (true) {
		if (match("+")) {
			if (mult() == 0)
				return 0;
			res += "+ ";
		}
		else if (match("-")) {
			if (mult() == 0)
				return 0;
			res += "- ";
		}
		else break;
	}
	return 1;
}

//основная программа
int main(int argc, char** argv) {
	setlocale(LC_ALL, "RUS");
	char str[200];
	while (true) {
		cout << "Введите выражение -> ";
		if (var.size() > 0) {
			while (getchar() != '\n');
			var.clear(); //каждый новый запуск будет новое окружение переменных
		}
		cin.getline(str, 200);
		if (strcmp(str, "end") == 0) { //для остановки разбора выражений
			break;
		}
		match_init(str);
		res = "";
		//функция возвращает 1 если не было критических ошибок и 0 в противном случае
		if (add()) {
			cout <<"Постфиксная запись: " << res << endl;
			cout <<"Результат расчета: " << calc(razdel(res)) << endl;
		}
		else 
			cout << endl << "FAIL" << endl;
		cout << endl;
	}
	match_done();
	system("pause");
	return 0;
}