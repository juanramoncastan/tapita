//----------------------------------------------------------------------
//
//  Copyright (C) 2013 Juan Ramon Castan Guillen <juanramoncastan@yahoo.es>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//----------------------------------------------------------------------

/***************************************************************************
 *
 * Tapita version 0.3.1 alpha 8
 *
 * utils.h
 *
 ***************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#define PI 3.1416

#define true 1
#define false 0
typedef int bool;


/* double_to_char declaration for double_to_char()*/
//#define PRECISION   2 // Precision of decimal point. We can
char *double_to_char (double, int) ;
char *itoa(int,char*,int);
float linear2db (float linear);
float db2linear (float db);

