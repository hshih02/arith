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
// static int64_t mask_of_s(unsigned width, unsigned lsb); returns mask with leading 1's, likely not needed

Except_T Bitpack_Overflow = { "Overflow packing bits" };


// int main()  /*test main*/
// {
//         // printf("fitsu test; n = 7, 3 bits, returns: %u\n", Bitpack_fitsu(7, 3));

//         printf("extracted getsu val: %lu\n\n", Bitpack_getu(0x3f4, 6 ,2));

//         // printf("extracted getss val: %lu\n", Bitpack_gets(0x3f4, 6 ,2));

//         // msb_is_neg(0x3f4, 6 ,2);
//         // msb_is_neg(0x3f4, 5 ,2);
//         // msb_is_neg(0x3f4, 4 ,2);  /*for testing msb_is_neg*/
//         // msb_is_neg(0x3f4, 3 ,2);
//         // msb_is_neg(0x3f4, 2 ,2);
//         // msb_is_neg(0x3f4, 1 ,2);
        
//         /*for testing get s*/
//         // printf("extracted getss val: %li\n\n", Bitpack_gets(0x3f4, 6 ,2));
//         // printf("extracted getss val: %li\n\n", Bitpack_gets(0x2dd, 7 ,2));
        

//         /*testing newu*/

//         // uint64_t word = 0x6c2; //1730
//         // uint64_t value = 0x5; //5
//         // unsigned width, lsb;
//         // width = 4;
//         // lsb = 4;


//         // uint64_t packedword = Bitpack_newu(word, width, lsb, value);

//         // printf("packedword with word: %lu  value: %lu \n%lu\n\n", word, value, packedword);

//         /*test news*/
//         // uint64_t word = 0xaaf9a; // 700314
//         // int64_t value = -47; // -47
//         // unsigned width, lsb;
//         // width = 8;
//         // lsb = 4;
//         // uint64_t packedword = Bitpack_news(word, width, lsb, value);

//         // printf("news packedword with word: %lu  value: %lu \n%lu\n\n", word, value, packedword);


//         // return 1;
// }



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
        mask = mask_of(width, lsb);
        field_val = word & mask;

        if (msb_is_neg(field_val, width, lsb)) {
                field_val = lshift(field_val, (BITLEN - (lsb + width)) );
                field_val = rshift_s(field_val, BITLEN - width );

                // printf("field_val = %lu\n", field_val);


                // mask = mask_of_s(width, lsb); /*old version with mask of s*/
                // field_val = word & mask;
                // field_val = rshift_s(field_val, lsb);

                // printf("field_val = %lu\n", field_val);

                // field_val = field_val;

        } else {
                field_val = rshift_s(field_val, lsb);
        }


        return field_val;
}
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                                                     uint64_t value)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);

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
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                                                     int64_t value)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);

        if ( Bitpack_fitss(value, width) == false ) {
                RAISE(Bitpack_Overflow);
                assert(0);
        }

        // printf("calling msb is neg with word: %lu, width: %u, lsb: %u, and value: %li\n\n", word, width, lsb, value);
        if (msb_is_neg(value, width, 0) == true) {      /*use 0 as lsb      */
                // printf("msb is neg == true\n");         /*because we pass in*/
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



static uint64_t mask_of(unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);

        uint64_t mask = ~0;

        mask = lshift(mask, BITLEN - (lsb + width) ); 
        mask = rshift_u(mask, BITLEN - width);
        mask = lshift(mask, lsb);

        return mask;
}


// static int64_t mask_of_s(unsigned width, unsigned lsb)
// {
//         assert(width <= BITLEN && width + lsb <= BITLEN); 

//         int64_t mask = ~0;
//         mask = lshift(mask, BITLEN - (lsb + (width - 1)) );
//         mask = rshift_s(mask, BITLEN - (width - 1) );
//         mask = lshift(mask, lsb);

//         printf ("width: %u, lsb: %u,   mask of s: %lu\n",width, lsb, mask);

//         return mask;
// }

/*works as a modified getu to return the msb instead of entire field*/
/*still partially untested*/
static bool msb_is_neg(uint64_t value, unsigned width, unsigned lsb)
{
        assert(width <= BITLEN && width + lsb <= BITLEN);

        uint64_t mask = mask_of(width, lsb);

        uint64_t msb = (value & mask);
        
        msb = rshift_u( msb, (lsb + (width - 1)) );

        /*offset width by 1 to prevent shifting past msb*/

        if (msb == 1) {
                // printf("w:%u lsb:%u    msb is negative\n", width, lsb);
                return true;
        }

        // printf("w:%u lsb:%u    msb is positive\n", width, lsb);
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
 
