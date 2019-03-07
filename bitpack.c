/**************************************************************
 *
 *                     bitpack.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 1, 2019
 *
 *     don't forget to write stuff here later
 *     
 *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "bitpack.h"


static const uint64_t BITLEN = 64;

static uint64_t mask_of(unsigned width, unsigned lsb);
static int64_t mask_of_s(unsigned width, unsigned lsb);
static bool msb_is_neg(uint64_t word, unsigned width, unsigned lsb);
static uint64_t lshift(uint64_t word, unsigned n);
static uint64_t rshift_u(uint64_t word, unsigned n);
static int64_t rshift_s(int64_t word, unsigned n);


int main()
{
        // printf("n = 7, 3 bits, returns: %u\n", Bitpack_fitsu(7, 3));

        printf("extracted getsu val: %lu\n\n", Bitpack_getu(0x3f4, 6 ,2));

        // printf("extracted getss val: %lu\n", Bitpack_gets(0x3f4, 6 ,2));

        // msb_is_neg(0x3f4, 6 ,2);
        // msb_is_neg(0x3f4, 5 ,2);
        // msb_is_neg(0x3f4, 4 ,2);
        // msb_is_neg(0x3f4, 3 ,2);
        // msb_is_neg(0x3f4, 2 ,2);
        // msb_is_neg(0x3f4, 1 ,2);
        
        printf("extracted getss val: %lu\n\n", Bitpack_gets(0x3f4, 6 ,2));
        int64_t word = -24;

        word = rshift_s(word, 64);
        printf("word post shift = %ld\n", word);
        

        return 1;
}


extern Except_T Bitpack_Overflow;

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        double range = exp2(width)-1;
        if (n <= range) {
                return true;
        }

        return false;
}

bool Bitpack_fitss( int64_t n, unsigned width)
{
        double range = -(exp2(width));
        range /= 2;
        if (n > range && n <= fabsf(range) - 1)
        {
                return true;
        }

        return false;
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);

        uint64_t field_val;
        uint64_t mask = mask_of(width, lsb);

        field_val = (word & mask);
        field_val = rshift_u(field_val, lsb);

        return field_val;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);
        
        int64_t mask;
        int64_t field_val;
        int64_t temp;

        if (msb_is_neg(word, width, lsb)) {
                mask = mask_of_s(width, lsb);
                temp = word ^ mask;
                temp = rshift_s(temp, lsb);

                printf("temp = %lu\n", temp);

                field_val = temp;

        } else {
                mask = mask_of(width,lsb);
                field_val = word & mask;
                field_val = rshift_s(field_val, lsb);
        }


        return field_val;
}
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                                                     uint64_t value)
{
        (void) word;
        (void) width;
        (void) lsb;
        (void) value;
        return 1;
}
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                                                     int64_t value)
{
        (void) word;
        (void) width;
        (void) lsb;
        (void) value;
        return 1;
}


// static uint64_t mask_of(unsigned width, unsigned lsb)
// {
//         assert(width <= BITLEN && width + lsb <= BITLEN);

//         uint64_t ones = ~0;
//         uint64_t mask = ones << ( BITLEN - (lsb + width) )
//                              >> ( BITLEN - width )
//                              << lsb;

//         return mask;    
// }

static uint64_t mask_of(unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);

        uint64_t mask = ~0;

        mask = lshift(mask, BITLEN - (lsb + width)); 
        mask = rshift_u(mask, BITLEN - width);
        mask = lshift(mask, lsb);

        return mask;
}

// static uint64_t mask_of_s(unsigned width, unsigned lsb)
// {
//         assert(width <= BITLEN && width + lsb <= BITLEN); 

//         uint64_t ones = ~0;
//         uint64_t mask = ones << ( BITLEN - (lsb + (width - 1)) )
//                              >> ( BITLEN - (width - 1) )
//                              << lsb;
//         printf ("mask of s: %lu\n", mask);

//         return mask;
// }


static int64_t mask_of_s(unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN); 

        int64_t mask = ~0;
        mask = lshift(mask, BITLEN - (lsb + (width - 1)) );
        mask = rshift_s(mask, BITLEN - (width - 1) );
        mask = lshift(mask, lsb);

        printf ("mask of s: %lu\n", mask);

        return mask;
}

/*works as a modified getu to return the msb instead of entire field*/
/*still partially untested*/
static bool msb_is_neg(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);

        uint64_t mask = mask_of(width, lsb);

        uint64_t msb = (word & mask) >> (lsb + (width - 1)); 
        /*offset width by 1 to prevent shifting past msb*/

        if (msb == 1) {
                // printf("msb is negative\n");
                return true;
        }

        // printf("msb is positive\n");
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
 
