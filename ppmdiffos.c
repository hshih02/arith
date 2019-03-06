#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "pnm.h"

#define A2 A2Methods_UArray2

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

// static void
// usage(const char *progname)
// {
//         fprintf(stderr, "Usage: %s [-rotate <angle>] "
//                         "[-{row,col,block}-major] [filename]\n",
//                         progname);
//         exit(1);
// }

void checkfile(FILE* file);
void closefile(FILE* file);
void diffeq(Pnm_ppm img1, Pnm_ppm img2);

int main(int argc, char *argv[]) 
{
        int   i;
        FILE *input1 = NULL;
        FILE *input2 = NULL;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        assert(argc < 4);  /* check that arguments don't exceed 1 */

        for (i = 1; i < argc; i++) {

                if (*argv[i] == '-') {
                        printf("calling from stdin\n");
                } else if (input1 == NULL) {                        
                        input1 = fopen(argv[i], "rb");
                        checkfile(input1);       
                } else if (input2 == NULL) {
                        input2 = fopen(argv[i], "rb");
                        checkfile(input2);
                }

        }

        if (input1 == NULL) {
                input1 = stdin;
        }

        // if (input1 != NULL) {
        //         printf("input is opened\n");
        // }


        assert(input1 != NULL && input2 != NULL);
        Pnm_ppm image1 = Pnm_ppmread(input1, methods);
        Pnm_ppm image2 = Pnm_ppmread(input2, methods);


        // these work
        // printf("image 1 height: %u \n image 2 height: %u \n", image1->height, image2->height);
        // printf("image 1 width: %u \n image 2 width: %u \n", image1->width, image2->width);

        // double reso_diff = (double)image1->height - (double)image2->height;
        
        //this also works
        // printf("resolution difference: %f\n", reso_diff);

        assert(fabs(image1->height - image2->height) <= 1);
        assert(fabs(image1->width - image2->width) <= 1);
        
        // also works
        // Pnm_rgb testprint = (Pnm_rgb) methods->at(array1, 0, 0);
        // printf("for image 1 at 0,0, R:%u, G:%u, B:%u\n", testprint->red, testprint->green, testprint->blue); 


        diffeq(image1, image2); //currently segfaulting
        
        // methods->free(&array1);
        // methods->free(&array2);

        Pnm_ppmfree(&image1);
        Pnm_ppmfree(&image2);
     



        // A2 new_pixels = transform(image, methods, *map,
        //                                         rotation, timings_file);
        // image->pixels = new_pixels;
        // image->height = methods->height(new_pixels);
        // image->width = methods->width(new_pixels);



        closefile(input1);
        closefile(input2);

        return EXIT_SUCCESS;
}

void checkfile(FILE* file) 
{
        if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file\n");
        exit(EXIT_FAILURE);
        }
}

void closefile(FILE* file)
{
        if (file != NULL) {
                printf ("input is not null\n");
                fclose(file);
                printf ("file closed.\n");
        }
}

void diffeq(Pnm_ppm img1, Pnm_ppm img2)
{
        Pnm_rgb rgbpix, rgbpix_prime;
        unsigned R, G, B, Rp, Gp, Bp;

        int i, j; /*iterators*/
        int w, h; /*smallest width and height*/

        /*store minimum "inner" dimensions to w and h*/
        if (img1->height >= img2->height) {
                h = img2->height;
                fprintf (stderr, "h = %d\n", h);
        } 
        if (img1->height <= img2->height) {
                h = img1->height;
                fprintf (stderr, "h = %d\n", h);
        }         
        if (img1->width >= img2->width) {
                w = img2->width;
                fprintf (stderr, "w = %d\n", w);
        }         
        if (img1->width <= img2->width) {
                w = img1->width;
                fprintf (stderr, "w = %d\n", w);
        } 


        for (i = 0; i < w; i++) {
                for (j = 0; j < h; j++) {
                        rgbpix = (Pnm_rgb) img1->methods->at(img1->pixels, i, j);

                        R = rgbpix->red;
                        G = rgbpix->green;
                        B = rgbpix->blue;

                        rgbpix_prime = (Pnm_rgb) img2->methods->at(img2->pixels, i, j);

                        Rp = rgbpix_prime->red;
                        Gp = rgbpix_prime->green;
                        Bp = rgbpix_prime->blue;

                        printf("R:%u,\tRprime: %u\n", R, Rp);
                        printf("G:%u,\tGprime: %u\n", G, Gp);
                        printf("B:%u,\tBprime: %u\n", B, Bp);
                        printf("\n");


                }
        }


}

// float rms(unsigned R, unsigned Rprime, unsigned G, unsigned Gprime
//           unsigned B, unsigned Bprime)
// {


// }