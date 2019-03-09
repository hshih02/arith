/**************************************************************
 *
 *                     bitpack.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 1, 2019
 *
 *     Performs bitpacking operations using 64 bit signed and 
 *     unsigned integer words and values
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "bitpack.h"

static const uint64_t BITLEN = 64;
static uint64_t mask_of(unsigned width, unsigned lsb);
static bool msb_is_neg(uint64_t value, unsigned width, unsigned lsb);
static uint64_t lshift(uint64_t word, unsigned n);
static uint64_t rshift_u(uint64_t word, unsigned n);
static int64_t rshift_s(int64_t word, unsigned n);

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/*
*       Bitpack_fitsu
*       Takes in a uint64_t and an unsigned width value, returns true 
*       if the value can be represented in width bits, otherwise false
*/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        double range = exp2(width)-1;
        if (n <= range) {
                return true;
        }

        return false;
}

/*
*       Bitpack_fitss
*       Takes in a int64_t and an unsigned width value, returns true 
*       if the value can be represented in width bits (two's complement),
*       otherwise false
*/
bool Bitpack_fitss(int64_t n, unsigned width)
{
        double range = -(exp2(width));
        range /= 2;
        if (n > range && n <= fabsf(range) - 1)
        {
                return true;
        }

        return false;
}

/*
*       Bitpack_getu
*       Takes in a uint64_t, an unsigned width value and unsigned lsb 
*       Returns the value of the specified field of the word at defined 
*       lsb and width 
*/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);
        assert( (int) width >= 0);
        
        uint64_t field_val;
        uint64_t mask = mask_of(width, lsb);

        field_val = (word & mask);
        field_val = rshift_u(field_val, lsb);

        return field_val;
}

/*
*       Bitpack_gets
*       Takes in a uint64_t, an unsigned width value and unsigned lsb
*       Returns the value of the specified field of the word at defined 
*       lsb and width (in two's complement)
*/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);
        assert( (int) width >= 0);

        int64_t mask;
        int64_t field_val;
        mask = mask_of(width, lsb);
        field_val = word & mask;

        if (msb_is_neg(field_val, width, lsb)) {
                field_val = lshift(field_val, (BITLEN - (lsb + width)) );
                field_val = rshift_s(field_val, BITLEN - width );

        } else {
                field_val = rshift_s(field_val, lsb);
        }


        return field_val;
}

/*
*       Bitpack_newu
*       Takes in a uint64_t, an unsigned width, unsigned lsb, and
*       uint32_t value
*       Writes value to the specified field of the word at defined 
*       lsb and width 
*/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                                                     uint64_t value)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);
        assert( (int) width >= 0);

        if ( Bitpack_fitsu(value, width) == false ) {
                RAISE(Bitpack_Overflow);
                assert(0);
        }
        
        uint64_t mask = mask_of(width, lsb);

        word = word | mask;
        word = word ^ mask;

        value = lshift(value, lsb);
        word = (word | value);

        return word;
}

/*
*       Bitpack_news
*       Takes in a uint64_t, an unsigned width, unsigned lsb, and 
*       int64_t value
*       Writes value to the specified field of the word at defined 
*       lsb and width (in two's complement)
*/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                                                     int64_t value)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);
        assert( (int) width >= 0);

        if ( Bitpack_fitss(value, width) == false ) {
                RAISE(Bitpack_Overflow);
                assert(0);
        }

        if (msb_is_neg(value, width, 0) == true) {      /*use 0 as lsb      */
                                                        /*because we pass in*/
                uint64_t mask = mask_of(width, lsb);    /*value, not a word */
                uint64_t mask2 = ~0;
                
                mask2 = lshift(mask2, (lsb + width) );
                /*
                / mask2 | shifted value will 0 out all other fields except
                / target field
                */

                value = lshift(value, lsb);
                value = value ^ mask2;

                word = word | mask;   /*make target field all 1's*/
                word = word ^ mask;   /*make target field all 0's*/
                word = word | value;  /*write value to target field*/

        } else {
                word = Bitpack_newu(word, width, lsb, value);
        }

        return word;
}

/*
*       mask_of
*       Takes in an unsigned width and unsigned lsb, returns a word
*       with 1's in the field at specified lsb and width and 0's 
*       in all other bits
*/

static uint64_t mask_of(unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);
        assert( (int) width >= 0);        

        uint64_t mask = ~0;

        mask = lshift(mask, BITLEN - (lsb + width) ); 
        mask = rshift_u(mask, BITLEN - width);
        mask = lshift(mask, lsb);

        return mask;
}

/*
*       msb_is_neg
*       returns true if the msb of a specified field in a uint64_t is 
*       1, false if 0
*/

static bool msb_is_neg(uint64_t value, unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);
        assert( (int) width >= 0); 

        uint64_t mask = mask_of(width, lsb);

        uint64_t msb = (value & mask);
        
        msb = rshift_u( msb, (lsb + (width - 1)) );

        /*offset width by 1 to prevent shifting past msb*/

        if (msb == 1) {
                return true;
        }

        return false;
}

static uint64_t lshift(uint64_t word, unsigned n)
{
        assert(n <= BITLEN);
        
        if (n == BITLEN) {
                return 0;
        }
        return word << n;
}

static uint64_t rshift_u(uint64_t word, unsigned n)
{
        assert(n <= BITLEN);

        if (n == BITLEN) {
                return 0;
        }
        return word >> n;
}
 
static int64_t rshift_s(int64_t word, unsigned n)
{
        assert(n <= BITLEN);

        if (n == BITLEN) {
                if (word < 0) {
                        return -1;
                }
                return 0;
        }
        return word >> n;
}
 
