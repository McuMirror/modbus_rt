/*
 * [Warning!] This file is auto-generated by pika compiler.
 * Do not edit it manually.
 * The source code is *.pyi file.
 * More details: 
 * English Doc:
 * https://pikadoc-en.readthedocs.io/en/latest/PikaScript%20%E6%A8%A1%E5%9D%97%E6%A6%82%E8%BF%B0.html
 * Chinese Doc:
 * http://pikapython.com/doc/PikaScript%20%E6%A8%A1%E5%9D%97%E6%A6%82%E8%BF%B0.html
 */

#ifndef ___random__H
#define ___random__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New__random(Args *args);

void _random___init__(PikaObj *self);
int _random_randint(PikaObj *self, int a, int b);
pika_float _random_random(PikaObj *self);
int _random_randrange(PikaObj *self, int start, int stop, int step);
PikaObj* _random_seed(PikaObj *self, int a);
pika_float _random_uniform(PikaObj *self, pika_float a, pika_float b);

#endif
