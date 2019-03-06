/**************************************************************
 *
 *                     inverse_dct.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  Omar Badr, Henning Shih 
 *     Date:     Mar 3, 2019
 *
 *     conducts the inverse of discrete cosine transformation 
 *     
 *
 **************************************************************/

#include "arithstructs.h"
#define DENOMINATOR 255
static float get_y1_value(post_dct unpacked_word);
static float get_y2_value(post_dct unpacked_word);
static float get_y3_value(post_dct unpacked_word);
static float get_y4_value(post_dct unpacked_word);
static Seq_T set_component_video(post_dct word, Seq_T component_video);
Seq_T inverse_dct_transform(post_dct unpacked_word);

int main()
{
        Seq_T component_video;
        post_dct test1, test2, test3, test4;

        comp_vid temp;

        test1.a = 169;
        test1.b = -7;
        test1.c = -7;
        test1.d = -3;
        test1.index_pb = 11;
        test1.index_pr = 3;

       
       component_video = inverse_dct_transform(test1);


       for(int i = 0; i < 4; i++){
                temp = Seq_get(component_video, i);
                printf(" Test1: Y%d: %f\n\n", i, temp->y);
               // temp = (Seq_remlo(component_video));
               //  FREE(temp);
        }

        test2.a = 287;
        test2.b = 11;
        test2.c = 7;
        test2.d = 4;
        test2.index_pb = 4;
        test2.index_pr = 13;

        component_video = inverse_dct_transform(test2);

       for(int i = 0; i < 4; i++){
                temp = Seq_get(component_video, i);
                printf("Test2: Y%d: %f\n\n", i, temp->y);
                // temp = (Seq_remlo(component_video));
                // FREE(temp);
        }

        test3.a = 240;
        test3.b = 0;
        test3.c = 0;
        test3.d = 4;
        test3.index_pb = 10;
        test3.index_pr = 3;

        component_video = inverse_dct_transform(test3);

        for(int i = 0; i < 4; i++){
                temp = Seq_get(component_video, i);
                printf("TEST3: Y%d: %f\n\n", i, temp->y);
               // temp = (Seq_remlo(component_video));
               //  FREE(temp);
        }

        test4.a = 287;
        test4.b = -4;
        test4.c = 7;
        test4.d = -11;
        test4.index_pb = 4;
        test4.index_pr = 13;


       component_video = inverse_dct_transform(test4);

        for(int i = 0; i < 4; i++){
                temp = Seq_get(component_video, i);
                printf("TEST4: Y%d: %f\n\n", i, temp->y);
                // temp = (Seq_remlo(component_video));
                // FREE(temp);
        }




}
Seq_T inverse_dct_transform(post_dct unpacked_word)
{
        Seq_T component_video = Seq_new(DCT_REQ);

        component_video = set_component_video(unpacked_word, component_video);

        return component_video;
}

float get_y1_value(post_dct unpacked_word)
{
        float y = (unpacked_word.a - unpacked_word.b - unpacked_word.c + 
                unpacked_word.d);
        y /=255;
        return y;
}

float get_y2_value(post_dct unpacked_word)
{
        float y = (unpacked_word.a - unpacked_word.b + unpacked_word.c - 
                unpacked_word.d);
        y /=255;
        return y;
}

float get_y3_value(post_dct unpacked_word)
{
        float y = (unpacked_word.a + unpacked_word.b - unpacked_word.c - 
                unpacked_word.d);
        y /=255;
        return y;
}

float get_y4_value(post_dct unpacked_word)
{
        float y = (unpacked_word.a + unpacked_word.b + unpacked_word.c + 
                unpacked_word.d);
        y /=255;
        return y;
}
static Seq_T set_component_video(post_dct word, Seq_T component_video)
{
        comp_vid cv_pixel1, cv_pixel2, cv_pixel3, cv_pixel4;

        float avgpb, avgpr;

        NEW(cv_pixel1);
        NEW(cv_pixel2); 
        NEW(cv_pixel3);
        NEW(cv_pixel4);

        avgpb = Arith40_chroma_of_index(word.index_pb);
        avgpr = Arith40_chroma_of_index(word.index_pr);

        cv_pixel1->y  = get_y1_value(word);
        cv_pixel1->pb = avgpb;
        cv_pixel1->pr = avgpr;

        cv_pixel2->y  = get_y2_value(word);
        cv_pixel2->pb = avgpb;
        cv_pixel2->pr = avgpr;

        cv_pixel3->y  = get_y3_value(word);
        cv_pixel3->pb = avgpb;
        cv_pixel3->pr = avgpr;

        cv_pixel4->y  = get_y4_value(word);
        cv_pixel4->pb = avgpb;
        cv_pixel4->pr = avgpr;

        Seq_addlo(component_video, cv_pixel1);
        Seq_addlo(component_video, cv_pixel2);
        Seq_addlo(component_video, cv_pixel3);
        Seq_addlo(component_video, cv_pixel4);

        return component_video;
}
