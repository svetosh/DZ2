#include "pch.h"
#include "match.hpp"
#include <stdio.h>
#include <iostream>

void match_init(char* line)
{
	ch = line;
	nexch = ch + 1;
}

void match_done()
{
	ch = nexch = nullptr;
}

void skipblanks() 
{
	while (true) 
	{
		if (!*ch) break;
		else if (*ch == ' ') 
			get_ch();
		else if (*ch == '\t') 
			get_ch();
		else break;
	}
}

int get_ch() 
{
	if (!*ch)
		return 0;
	else 
	{
		nexch++;
		return (*ch++);
	}
}

int match(const char *lit) 
{
	int len = squal(ch, lit);
	int k = len;
	skipblanks(); 
	if (k > 0) 
	{
		ch = ch + k; 
		nexch = ch + 1;
		return 1; 
	}
	return 0; 
}

int squal(const char* s, const char* lit) 
{
	int k = 0;
	while (*lit) 
	{
		if (*s == *lit) 
		{
			k++;
			s++;
			lit++;
		}
		else return 0;
	}
	return k;
}

int alpha(char c) 
{
	if ((c >= 'a') & (c <= 'z')) return 1;
	if ((c >= 'A') & (c <= 'Z')) return 1;
	return(c == '_');
}

int numeric(char c) 
{
	return((c >= '0')&(c <= '9'));
}

int an(char c) 
{
	if ((c >= '0') & (c <= '9')) return 1;
	if ((c >= 'a') & (c <= 'z')) return 1;
	if ((c >= 'A') & (c <= 'Z')) return 1;
	return (c == '_');
}

void cerror(const char *error) 
{
	std::cout << error << std::endl;
}
