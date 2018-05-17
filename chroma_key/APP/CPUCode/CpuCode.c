
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int check(uint8_t *out_image, uint8_t *expected, int size)
{
	int status = 0;
	for(int i=0; i < size; i++)
	{
		if(out_image[i] > expected[i] + 5 || out_image[i] < expected[i] - 5) {
			fprintf(stderr, "Output image @ %d = %d (expected %d)\n",
				i, out_image[i], expected[i]);
			status = 1;
		}
	}
	return status;
}

int cr_from_rgb(int r, int g, int b)
{
    return (int) (128 + (0.5 * r) - (0.418688 * g) - (0.081312 * b) + 0.5);
}

int cb_from_rgb(int r, int g, int b)
{
    return (int)(128 - (0.168736 * r) - (0.331264 * g) + (0.5 * b) + 0.5);
}

void calc_tolerances(int width, int height, uint8_t *in_image, int key_cb, int key_cr, float *tol_a, float *tol_b)
{

    float max_radius = 0;

    for(int i = 0; i < width * height; i++)
    {
        int r = in_image[i + 0];
        int g = in_image[i + 1];
        int b = in_image[i + 2];

        int cb = cb_from_rgb(r,g,b);
        int cr = cr_from_rgb(r,g,b);

        float current_radius = sqrt(pow(cb - key_cb, 2) + pow(cr - key_cr, 2));

        if(current_radius > max_radius){
            max_radius = current_radius;
        }

    }

    *tol_a = max_radius / 4;
    *tol_b = max_radius * 0.5;
    
}

float calc_mask(float key_cb, float key_cr, float cb, float cr, float tol_a, float tol_b)
{
    float diff = sqrt(pow(key_cb - cb, 2) + pow(key_cr - cr, 2));

    if(diff < tol_a){
        return 0;
    }else if(diff < tol_b){
        return (diff - tol_a) / (tol_b - tol_a);
    }else{
        return 1;
    }
}
        


void chroma_key(int width, int height, int key_cb, int key_cr, float tol_a, float tol_b, uint8_t *in_image, uint8_t *bg_image, uint8_t *out_image)
{

    for(int i = 0; i < width * height * 3 ; i = i + 3)
    {
        int r = in_image[i + 0];
        int g = in_image[i + 1];
        int b = in_image[i + 2];

        int cb = cb_from_rgb(r,g,b);
        int cr = cr_from_rgb(r,g,b);

        float mask = 1 - calc_mask(key_cb, key_cr, cb, cr, tol_a, tol_b);


        for(int j = 0; j < 3; j++)
        {
            out_image[i + j] = in_image[i + j] - mask * in_image[i + j] + mask * bg_image[i + j];
        }
        

    }
}


int main()
{
    int width, height, bpp;
    int bg_width, bg_height, bg_bpp;

    uint8_t *in_image = stbi_load("image_old.jpg", &width, &height, &bpp, 3);
    uint8_t *bg_image = stbi_load("background.png", &bg_width, &bg_height, &bg_bpp, 3);

    int size = width * height * 3;

    if(width != bg_width || height != bg_height){
        fprintf(stderr, "ERROR: Images must have the same size!\n");
        exit(EXIT_FAILURE);
    }

    if(in_image == NULL || bg_image == NULL){
        fprintf(stderr, "ERROR: Images could not be loaded!\n");
        exit(EXIT_FAILURE);
    }

    uint8_t *out_image = (uint8_t *)malloc(size * sizeof(uint8_t));
    uint8_t *expected = (uint8_t *)malloc(size * sizeof(uint8_t));


    
    int key_r = in_image[0];
    int key_g = in_image[1];
    int key_b = in_image[2];
    
    int key_cb = cb_from_rgb(key_r, key_g, key_b);
    int key_cr = cr_from_rgb(key_r, key_g, key_b);

    float tol_a;
    float tol_b;

    calc_tolerances(width, height, in_image, key_cb, key_cr, &tol_a, &tol_b);


    chroma_key(width, height, key_cb, key_cr, tol_a, tol_b, in_image, bg_image, expected);
    ChromaKey(size, key_cb, key_cr, tol_a, tol_b, in_image, bg_image, out_image);

    printf("Running DFE.\n");
    int status = check(out_image, expected, size);
    
    stbi_write_jpg("image_new.jpg", width, height, 3, out_image, 90);

    stbi_image_free(in_image);
    stbi_image_free(bg_image);
    free(out_image);
    free(expected);
	
    if (status)
            printf("Test failed.\n");
    else
            printf("Test passed OK!\n");
    return status;
	
}


