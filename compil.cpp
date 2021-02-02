#include "pch.h"
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
#include "match.hpp"

using namespace std;

extern char *ch = nullptr; 
extern char *nexch = nullptr; 
map<string, float> var; 
string res = ""; 
bool gminus = false;

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

vector<string> razdel(string st) 
{
	vector<string> vec;
	string r = "";
	for (int i = 0; i < st.size(); i++) 
	{
		if (st[i] == ' ' && r.size() > 0) 
		{
			vec.push_back(r);
			r = "";
		}
		else if (st[i] == '[') 
		{
			int j = 1; 
			while (st[i] != ']' || j > 1) 
			{
				if (st[i] == '[') 
				{
					j++;
				}
				else if ((st[i + 1] == ']') && ((i + 1) < st.size())) 
				{
					j--;
				}
				r += st[i];
				i++;
			}
			r += st[i];
			vec.push_back(r);
			r = "";
		}
		else if (st[i] != ' ') 
		{ 
			r += st[i];
		}
	}
	if (r.size() > 0) 
	{
		vec.push_back(r);
	}
	return vec;
}

float open(string s) 
{
	string next = "";
	if (s[s.size() - 1] == ']') 
	{
		string name = "";
		s.pop_back();
		while (s[s.size() - 1] != ' ') 
		{
			name += s[s.size() - 1];
			s.pop_back();
		}
		reverse(name.begin(), name.end());
		if (name == "cos") 
		{
			for (int i = 1; i < s.size() - 1; i++) 
			{
				next += s[i];
			}
			return cos(calc(razdel(next)));
		}
		else if (name == "sin") 
		{
			for (int i = 1; i < s.size() - 1; i++) 
			{
				next += s[i];
			}
			return sin(calc(razdel(next)));
		}
		else if (name == "tg")
		{
			for (int i = 1; i < s.size() - 1; i++) 
			{
				next += s[i];
			}
			return tan(calc(razdel(next)));
		}
		else if (name == "ctg") 
		{
			for (int i = 1; i < s.size() - 1; i++)
			{
				next += s[i];
			}
			return 1.0 / tan(calc(razdel(next)));
		}
		else if (name == "sqrt")
		{
			for (int i = 1; i < s.size() - 1; i++)
			{
				next += s[i];
			}
			return sqrt(calc(razdel(next)));
		}
		else if (name == "exp")
		{
			for (int i = 1; i < s.size() - 1; i++)
			{
				next += s[i];
			}
			return exp(calc(razdel(next)));
		}
		else if (name.find("pow") != string::npos)
		{ 
			for (int i = 1; i < s.size() - 1; i++)
			{
				next += s[i];
			}
			string p = "";
			for (int i = 0; i < name.size() && name[i] != '_'; i++) 
			{
				p += name[i];
			}
			return pow(calc(razdel(next)), atoi(p.c_str()));
		}
	}
}

float calc(vector<string> s) 
{
	stack<float> x;
	for (auto i : s)
	{
		if (i.size() == 1) 
		{
			if (isdigit(i[0])) 
			{
				x.push(atof(i.c_str()));
			}
			else 
			{
				char c = i[0];
				switch (c) 
				{
					case '+': 
					{
						float a = x.top(); x.pop(); 
						float b = x.top(); x.pop();
						x.push(a + b); 
						break;
					}
					case '-': 
					{
						float a = x.top(); x.pop();
						float b = x.top(); x.pop();
						x.push(b - a);
						break;
					}
					case '*': 
					{
						float a = x.top(); x.pop();
						float b = x.top(); x.pop();
						x.push(a * b);
						break;
					}
					case '/': 
					{
						float a = x.top(); x.pop();
						float b = x.top(); x.pop();
						if (abs(a) != 0) 
						{
							x.push(b / a);
						}
						else 
						{
							cerror("Ошибка при выполнении операции!");
							break;
						}
						break;
					}
				}
			}
		}
		else 
		{
			if (i[0] == '[') 
			{
				x.push(open(i));
			}
			else if (i[0] == '-' && i[1] == '[')
			{
				string w(i);
				w.erase(w.begin());
				x.push(-open(w));
			}
			else 
			{
				if (isdigit(i[0]) || (isdigit(i[1]) && i[0] == '-')) 
				{
					x.push(atof(i.c_str()));
				}
				else 
				{
					cerror("Ошибка при разборе выражения!");
					break;
				}
			}
		}
	}
	if (x.size() != 1) 
	{
		cerror("В стеке находится больше чем одно значение!");
		return -1;
	}
	else 
	{
		return x.top();
	}
}

int constant(float& val) 
{
	char* str = ch;
	char* next = nexch;
	int minus = 1;
	if (match("-")) 
	{
		minus = -minus;
	}
	if (match("+")) 
	{
		minus = minus;
	}
	if (match("pi"))
	{
		val = 3.1415926 * minus;
		return 1;
	}
	else if (*ch == 'e' && *nexch != 'x')
	{
		ch++;
		nexch++;
		val = exp(1) * minus;
		return 1;
	}
	ch = str;
	next = nexch;
	return 0;
}

int variable()
{
	int minus = 1;
	if (match("-")) 
	{
		minus = -1;
	}
	if (match("+")) 
	{
		minus = 1;
	}
	char* str = ch;
	char* next = nexch;
	string name = "";
	while (alpha(*str)) 
	{
		name += *str++;
	}
	if (name == " " || name.size() == 0)
	{
		return 0;
	}
	if (*str == '(') 
	{
		if (name == "exp" || name == "pow" || name == "sin" ||
			name == "cos" || name == "tg" || name == "ctg" || name == "sqrt") 
		{
			return 0;
		}
	}
	else {
		ch = str;
		nexch = next;
		if (var.find(name) != var.end())
		{ 
			res += to_string(var[name]);
			res += " ";
		}
		else
		{ 
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

int func() 
{
	char* str = ch;
	char* next = nexch;
	int minus = 1;
	if (match("-")) 
	{
		minus = -1;
	}
	if (match("+"))
	{
		minus = 1;
	}
	if (match("exp"))
	{
		if (*ch != '(')
		{
			return 0;
		}
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0)
		{
			return 0;
		}
		res += "exp] ";
	}
	else if (match("sqrt"))
	{
		if (*ch != '(')
		{
			return 0;
		}
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0)
		{
			return 0;
		}
		res += "sqrt] ";
	}
	else if (match("sin"))
	{
		if (*ch != '(')
		{
			return 0;
		}
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0)
		{
			return 0;
		}
		res += "sin] ";
	}
	else if (match("cos")) 
	{
		if (*ch != '(')
		{
			return 0;
		}
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0)
		{
			return 0;
		}
		res += "cos] ";
	}
	else if (match("tg"))
	{
		if (*ch != '(')
		{
			return 0;
		}
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0)
		{
			return 0;
		}
		res += "tg] ";
	}
	else if (match("ctg"))
	{
		if (*ch != '(')
		{
			return 0;
		}
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0)
		{
			return 0;
		}
		res += "ctg] ";
	}
	else if (match("pow"))
	{
		if (*ch != '(')
		{
			return 0;
		}
		if (minus < 0)
			res += '-';
		res += "[";
		if (expr() == 0)
			return 0;
		res += "pow] ";
	}
	else
    { 
		ch = str;
		nexch = next;
		return 0;
	}
	skipblanks();
	return 1;
}

int first_value(int& val, int& minus)
{
	int k = 1;
	char c = '\0';
	while (k)
	{
		k = 0;
		if (match("+"))
			k = 1;
		if (match("-"))
		{
			minus = -1;
			k = 1;
		}
	}
	if (numeric(*ch) == 0)
		return false;
	while (numeric(*ch))
	{ 
		c = get_ch();
		k = k * 10 + (c - '0');
	}
	val = k;
	return true;
}

int number_int(int& val)
{
	char* str = ch;
	char* next = nexch;
	int minus = 1;
	bool res = first_value(val, minus);
	if (res) 
	{
		if (minus < 0)
		{
			val = -val;
		}
		if (match("."))
		{ 
			ch = str;
			nexch = next;
			return 0;
		}
		else
			return 1;
	}
	else 
	{
		ch = str;
		nexch = next;
	}
	return 0;
}

int number_float(float& val)
{
	char* str = ch;
	char* next = nexch;
	int minus = 1;
	int d = 0; 
	int m = 0; 
	int l = 0; 
	char c = '\0';
	bool res = first_value(d, minus);
	if (res) 
	{
		if (minus < 0) 
		{
			val = -val;
		}
		if (match(".")) {
			while (numeric(*ch)) 
			{
				c = get_ch();
				m = m * 10 + (c - '0');
				l++;
			}
		}
		if (match(".")) 
		{
			return 0;
		}
		val = (float)d + (float)m / pow(10, l);
		return 1;
	}
	else
	{
		ch = str;
		nexch = next;
		return 0;
	}
}

int expr() 
{
	int s = 0;
	float f = 0.0f;

	if (*ch == '-' && *nexch == '(')
	{
		ch++;
		nexch++;
		gminus = !gminus;
	}

	if (match("(")) 
	{
		if (add() == 0)
			return 0;
		if (match(",")) 
		{
			if (number_int(s)) 
			{
				res += to_string(s);
				res += "_";
			}
			else 
			{
				cerror("syntax error");
				return 0;
			}
		}
		if (!match(")"))
		{
			cerror("mismatch ')'");
			return 0;
		}
		if (gminus)
		{
			res += "-1 * ";
			gminus = !gminus;
		}
	}
	else if (number_int(s))
	{
		res += to_string(s);
		res += " ";
	}
	else if (number_float(f))
	{ 
		res += to_string(f);
		res += " ";
	}
	else if (constant(f))
	{ 
		res += to_string(f);
		res += " ";
	}
	else if (func())
	{ 
		return 1;
	}
	else if (variable())
	{ 
		return 1;
	}
	else
	{ 
		cerror("syntax error");
		return 0;
	}
	return 1;
}

int mult()
{
	if (expr() == 0)
		return 0;
	if (match("(")) 
	{
		return 0;
	}
	bool check = true;
	do
	{
		if (match("*"))
		{
			if (expr() == 0)
				return 0;
			res += "* ";
		}
		else if (match("/"))
		{
			if (expr() == 0)
				return 0;
			res += "/ ";
		}
		else
		{
			check = false;
		}
	}
	while (check);
	return 1;
}

int add() 
{
	if (mult() == 0)
		return 0;
	if (match("("))
	{
		return 0;
	}
	bool check = false;
	do {
		if (match("+")) 
		{
			if (mult() == 0)
				return 0;
			res += "+ ";
		}
		else if (match("-")) 
		{
			if (mult() == 0)
				return 0;
			res += "- ";
		}
		else 
		{
			check = false;
		}
	} while (check);
	return 1;
}

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "RUS");
	string str;
	do 
	{
		cout << "Введите выражение -> ";
		if (var.size() > 0) 
		{
			while (getchar() != '\n');
			var.clear();
		}
		getline(cin, str);
		if (str != "end")
		{ 
			match_init(const_cast<char*>(str.data()));
			res = "";
			if (add())
			{
				cout << "Постфиксная запись: " << res << endl;
				cout << "Результат расчета: " << calc(razdel(res)) << endl;
			}
			else
				cout << endl << "FAIL" << endl;
		}
		cout << endl;
	} while (str != "end");
	match_done();
	return 0;
}
