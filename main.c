#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>


struct winsize window_size;


const char ANSI_ESC[] = "\033";
const char ANSI_RESET[] = "[0m";
const char ANSI_CHAR_COLOR_BASE[] = "[38;2;";
const char ANSI_BCKG_COLOR_BASE[] = "[48;2;";


/*
Returns the height of the current terminal session.
Pixel height = num. of lines * 2
*/
int get_window_height() {
    ioctl(0, TIOCGWINSZ, &window_size);
    
    return window_size.ws_row * 2;
}


/*
Returns the width of the current terminal session.
Pixel height = num. of cols
*/
int get_window_width() {
    ioctl(0, TIOCGWINSZ, &window_size);
    
    return window_size.ws_col;
}


void print_pixel(unsigned char r, unsigned char g, unsigned char b) {
    printf("%s%s%d;%d;%dm██", ANSI_ESC, ANSI_CHAR_COLOR_BASE, r, g, b);
}


/*
Prints two pixels - the ▀ char is the upper pixel and the background is the lower one.
*/
void print_two_pixels(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2) {
    printf("%s%s%d;%d;%dm%s%s%d;%d;%dm▀",
        ANSI_ESC, ANSI_CHAR_COLOR_BASE, r1, g1, b1,
        ANSI_ESC, ANSI_BCKG_COLOR_BASE, r2, g2, b2
    );
}


void print_img(int w, int h, unsigned char *pixels) {
    for (int i=0; i<h; i+=2) {
        for (int j=0; j<w; j++) {
            // print_pixel(pixels[(i * w * 3) + j * 3 + 0], pixels[(i * w * 3) + j * 3 + 1], pixels[(i * w * 3) + j * 3 + 2]);
            print_two_pixels(
                pixels[(i * w * 3) + j * 3 + 0], pixels[(i * w * 3) + j * 3 + 1], pixels[(i * w * 3) + j * 3 + 2],
                pixels[((i + 1) * w * 3) + j * 3 + 0], pixels[((i + 1) * w * 3) + j * 3 + 1], pixels[((i + 1) * w * 3) + j * 3 + 2]
            );
        }
        printf("%s%s\n", ANSI_ESC, ANSI_RESET);
    }
}


/*
Scales the image so that it fits in the terminal window
Also pads it if the lines are not even
*/
void scale_image(int w, int h, unsigned char *pixels) {
    
}


int main() {
    FILE *img = fopen("test_img_mini.ppm", "rb");
    
    int width, height;
    
    // skip magic word (P6)
	while (fgetc(img) != '\n');
	
	fscanf(img, "%d%d", &width, &height);
	
	// skip \n and 255\n
	while (fgetc(img) != '\n');
	while (fgetc(img) != '\n');
	
	// read pixels
	int blockSize = 3 * width * height;
	unsigned char *pixels = (unsigned char *) malloc(blockSize * sizeof(unsigned char));
	fread(pixels, sizeof(unsigned char), blockSize, img);    
    
    fclose(img);
    
    
    print_img(width, height, pixels);
    
    return 0;
}
