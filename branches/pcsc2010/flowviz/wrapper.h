/**
 * Wrapper structure declarations for primitive Data Types
 *
 * @author Joseph Anthony C. Hermocilla
 * @version 1.0
 *
 */


/* For strcpy() */
#include <string.h>
#include <stdlib.h>

#ifndef __WRAPPER_H_
#define __WRAPPER_H_

typedef struct _Integer{
	int data;
} *Integer;

typedef struct _Char{
	char data;
} *Char;

typedef struct _Float{
	float data;
} *Float;

typedef struct _Double{
	double data;
} *Double;

typedef struct _String{
	char data[255];
} *String;


/* Creation/Allocation functions for the above declarations */

Integer Integer_new(int i){
	Integer tmp = (Integer)malloc(sizeof(struct _Integer));
	tmp->data = i;
	return tmp;
}

Char Char_new(char c){
	Char tmp = (Char)malloc(sizeof(struct _Char));
	tmp->data = c;
	return tmp;
}

Float Float_new(float f){
	Float tmp = (Float)malloc(sizeof(struct _Float));
	tmp->data = f;
	return tmp;
}

Double Double_new(double d){
	Double tmp = (Double)malloc(sizeof(struct _Double));
	tmp->data = d;
	return tmp;
}

String String_new(char *str){
	String tmp = (String)malloc(sizeof(struct _String));
	strcpy(tmp->data,str);
	return tmp;
}

#endif
