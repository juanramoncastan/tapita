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

/****************************************************************************
 * 
 * Tapita version 0.3.1 alpha 8
 *
 * utils.c
 *
 *   To build the file object "utils.o"
 *      gcc -c utils.c -lm 
 *
 ***************************************************************************/


#include "utils.h"


// Convert double to char string
char *double_to_char (double number, int precision)
{
   char *buffer,
         *temp ;

   int  decimal_spot,
        sign,
        count,
        current_location = 0 ;

   temp = fcvt (number, precision, &decimal_spot, &sign) ;
   if ((unsigned)strlen (temp) > (unsigned)precision)
      buffer = (char *) malloc (strlen (temp) + 3) ;
   else
      buffer = (char *) malloc (precision + 3) ;

   if (buffer == NULL)
   {
      printf ("Memory allocating attempt has failed in"
              "'double_to_char'\n") ;
      exit (-1) ;
   }
    /* Add negative sign if required. */ 
   if (sign)
      buffer [current_location++] = '-' ;
    /* Place decimal point in the correct location. */ 
   if (decimal_spot > 0)
   {
      strncpy (&buffer [current_location], temp, decimal_spot) ;
        if (precision > 0 )
            buffer [decimal_spot + current_location] = '.' ;
        else
            buffer [decimal_spot + current_location] = 0 ;

      strcpy (&buffer [decimal_spot + current_location + 1], &temp [decimal_spot]) ;
   }
   else
   {
      if (precision > 0 )
        buffer [current_location] = '.' ;
      else
        buffer [decimal_spot + current_location] = 0 ;
      for(count = current_location;
             count<abs(decimal_spot)+current_location; count++)
         buffer [count + 1] = '0' ;
      strcpy (&buffer [count + 1], temp) ;
   }
   return (buffer) ;
}

char* itoa(int value, char* str, int radix) {
    static char dig[] =
    "0123456789"
    "abcdefghijklmnopqrstuvwxyz";
    int n = 0, neg = 0;
    unsigned int v;
    char* p, *q;
    char c;
    if (radix == 10 && value < 0) {
        value = -value;
        neg = 1;
    }
    v = value;
    do {
        str[n++] = dig[v%radix];
        v /= radix;
    } while (v);
    if (neg)
    str[n++] = '-';
    str[n] = '\0';
    for (p = str, q = p + n/2; p != q; ++p, --q)
    c = *p, *p = *q, *q = c;
    return str;
}

float linear2db (float linear)
{
    return (20 * (log( linear ) / log(10)) );
}


float db2linear (float db)
{
    return pow( 10 , ( db/20 )  );
}
