#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <math.h>

#define GRAY_CODE 127

int save_image(SDL_Surface *image, const char *output_path) {
    if (SDL_SaveBMP(image, output_path) != 0) {
        printf("Failed to save image: %s\n", SDL_GetError());
        return -1;
    }
    printf("Image saved successfully to %s\n", output_path);
    return 0;
}


SDL_Surface* load_image(char* path) {
    SDL_Surface *image = IMG_Load(path);
    if (!image) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return  NULL;
    }

    return image;
}


SDL_Surface* binarize_image(SDL_Surface* image) {
    Uint32 *pixels = (Uint32 *)image->pixels;
    int height = image -> h;
    int width = image -> w;
    SDL_PixelFormat * format = image -> format;

    //Iterate through all pixels of image
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint32 pixel = pixels[y * width + x];
            Uint8 r, g, b, grayscale;

            //get image grayscale
            SDL_GetRGB(pixel, format, &r, &g, &b);
            grayscale = 0.3 * r + 0.59 * g + 0.11 * b;
            Uint32 new_value = (grayscale > GRAY_CODE) ? 255 : 0;
            //binarize image
            Uint32 new_pixel = SDL_MapRGB(format, new_value, new_value, new_value);
            pixels[y * width + x] = new_pixel;
        }
        
    }

    return image;
}

SDL_Surface* rotate_image(SDL_Surface *image, double angle) {
    double radians = angle * 3.141593 / 180.0;

    int width = image->w;
    int height = image->h;

    // Calculate new dimensions for the rotated image
    int new_width = (int)(fabs(width * cos(radians)) + fabs(height * sin(radians)));
    int new_height = (int)(fabs(width * sin(radians)) + fabs(height * cos(radians)));

    SDL_Surface *rotated_image = SDL_CreateRGBSurface(
        0, new_width, new_height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000
        );

    if (!rotated_image) {
        printf("Failed to create rotated surface: %s\n", SDL_GetError());
        return NULL;
    }

    // Get the center of the original and new images
    int middle_x_old = width / 2;
    int middle_y_old = height / 2;
    int middle_x_new = new_width / 2;
    int middle_y_new = new_height / 2;

    Uint32 *original_pixels = (Uint32 *)image->pixels;
    Uint32 *rotated_pixels = (Uint32 *)rotated_image->pixels;

    // Iterate through all pixels of the new image
    for (int y_new = 0; y_new < new_height; y_new++) 
    {
        for (int x_new = 0; x_new < new_width; x_new++) 
        {
            // Calculate the corresponding source pixel in the original image
            int x_old = (int)((x_new - middle_x_new) * cos(radians) + (y_new - middle_y_new) * sin(radians)) + middle_x_old;
            int y_old = (int)(-(x_new - middle_x_new) * sin(radians) + (y_new - middle_y_new) * cos(radians)) + middle_y_old;

            // If the source pixel is within the bounds of the original image, copy it
            if (x_old >= 0 && x_old < width && y_old >= 0 && y_old < height) {
                rotated_pixels[y_new * new_width + x_new] = original_pixels[y_old * width + x_old];
            } else {
                rotated_pixels[y_new * new_width + x_new] = SDL_MapRGB(rotated_image->format, 255, 255, 255); 
            }
        }
    }

    return rotated_image;
}


int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <input_image> <angle> <output_image>\n", argv[0]);
        return -1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_Surface *image = load_image(argv[1]);
    if (!image) {
        return -1;
    }

    SDL_Surface *binarized_image = binarize_image(image);
    SDL_Surface *rotated_image = rotate_image(image, (double)(atoi(argv[2])));
    save_image(rotated_image, argv[3]);

    SDL_FreeSurface(image); 
    IMG_Quit();
    SDL_Quit();

    return 0;
}