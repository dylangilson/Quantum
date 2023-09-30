/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "parser.h"

typedef struct Generator {
    NodeExit root;
} Generator;

Generator *create_generator(NodeExit root);
char *generate(Generator *generator, char *buffer);
