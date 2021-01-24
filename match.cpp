#include "match.h"
#include <stdio.h>
#include <iostream>

// ��������� �������������
void match_init(char* line) {
	ch = line;
	nexch = ch + 1;
}

// ������������� ���������
void match_done() {
	ch = nexch = NULL;
}

// ������� ������ ��������
void skipblanks() {
	while (true) {
		if (!*ch) break;
		else if (*ch == ' ') get_ch();
		else if (*ch == '\t') get_ch();
		else break;
	}
}

//��������� ���������� �������
int get_ch() {
	if (!*ch)
		return 0;
	else {
		nexch++;
		return (*ch++);
	}
}

//����� ����������
int match(const char *lit) {
	int len = squal(ch, lit);
	int k = len;
	skipblanks(); //������� �������� � ������� ���������
	if (k > 0) {
		ch = ch + k; //�������� ��������� �� k ��������� ������
		nexch = ch + 1;
		return 1; //���������� ��������
	}
	return 0; //���������� �� ��������
}

//���������� ����� ���������� ��� 0 ���� ���� ����������
int squal(const char* s, const char* lit) {
	int k = 0;
	while (*lit) {
		if (*s == *lit) {
			k++;
			s++;
			lit++;
		}
		else return 0;
	}
	return k;
}

//����� ���������� �� �����
int amatch(char *lit, int len) {
	int k = 0;
	skipblanks();
	if (k = asqual(ch, lit, len)) {
		ch = ch + k;
		nexch = ch + 1;
		while (an(*ch)) get_ch();
		return 1;
	}
	return 0;
}

//������������ � amatch
int asqual(char *str1, char *str2, int len) {
	int k = 0;
	while (k < len) {
		if ((*str1) != (*str2)) break;
		if (*str1 == 0 || *str2 == 0) break;
		k++;
		str1++;
		str2++;
	}
	if (an(*str1)) return 0;
	if (an(*str2)) return 0;
	return k;
}

//������
int alpha(char c) {
	if ((c >= 'a') & (c <= 'z')) return 1;
	if ((c >= 'A') & (c <= 'Z')) return 1;
	return(c == '_');
}

//�����
int numeric(char c) {
	return((c >= '0')&(c <= '9'));
}

//� �� � ������
int an(char c) {
	if ((c >= '0') & (c <= '9')) return 1;
	if ((c >= 'a') & (c <= 'z')) return 1;
	if ((c >= 'A') & (c <= 'Z')) return 1;
	return (c == '_');
}

// ����� ����� (��������������) � ������
int symname(char *sname) {
	int k;
	skipblanks();
	if (alpha(*ch) == 0)
		return 0;
	k = 1;
	while (an(*ch))
		sname[k++] = get_ch();
	sname[k] = 0;
	sname[0] = k - 1;
	return 1;
}

// ������� ������� � ��������� ������ ��������
void skipchars() {
	if (an(get_ch()))
		while (an(*ch))
			get_ch();
	else
		while (an(*ch) == 0) {
			if (*ch == 0)  break;
			get_ch();
		}
	skipblanks();
}

//����� ������
void cerror(const char *error) {
	std::cout << error << std::endl;
}