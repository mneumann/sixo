
/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-*/
/*******************************************************************************

  bmp2sixo 
  convert 1bit logo Bitmap files into bitmap C arrays for SiXO source

  Author: Markus Mueller (mm@rsj.de)

  should compile on any system providing an ANSI-C runtime, e.g.

    GCC (Win):     gcc -o bmp2sixo.exe bmp2sixo.c
    GCC (Linux):   gcc -o bmp2sixo bmp2sixo.c
    M$ Visual C++: cl bmp2sixo.c

*******************************************************************************/

/*---- includes --------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*---- defines ---------------------------------------------------------------*/

#define VERSION "0.03"


/*---- function prototypes ---------------------------------------------------*/

int     main        (int argc,
                     char **argv);

void    usage       (void);

int     convert_pbm (char *bmp_fn);

int     convert_bmp (char *bmp_fn);

/*---- types -----------------------------------------------------------------*/

#pragma pack(1)
typedef struct { 
  unsigned short bfType; 
  unsigned long  bfSize; 
  unsigned short bfReserved1; 
  unsigned short bfReserved2; 
  unsigned long  bfOffBits; 
  } BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct {
  unsigned long  biSize; 
  long           biWidth; 
  long           biHeight; 
  unsigned short biPlanes; 
  unsigned short biBitCount; 
  unsigned long  biCompression; 
  unsigned long  biSizeImage; 
  long           biXPelsPerMeter; 
  long           biYPelsPerMeter; 
  unsigned long  biClrUsed; 
  unsigned long  biClrImportant; 
  } BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

typedef struct {
  unsigned char blue; 
  unsigned char green; 
  unsigned char red; 
  unsigned char reserved; 
  } RGBQUAD; 


#pragma pack()

/*---- module data ----------------------------------------------------------*/


/*---- start of code --------------------------------------------------------*/

/*******************************************************************************
  main()
*******************************************************************************/
int main(int argc, char **argv)
{
char *bmp_fn = NULL;   /* input bitmap file name */
FILE *fp;
char buf[10];
int ret;

if (argc < 2 || argv[1][1] == '?' || tolower(argv[1][1]) == 'h') {
  usage();
  return(1);
  }

/* bitmap file name */
bmp_fn = argv[1];

if (bmp_fn == NULL) {
  usage();
  return(1);
  }

/* PBM or BMP?? */
if ((fp = fopen(bmp_fn, "rb")) == NULL) {
  perror("Failed to open input file");
  return(errno);
  }

/* first two bytes of BMP's are 'BM', of PBM 'P1' */
if (fread(buf, 1, 2, fp) != 2) {
  perror("Failed to read input file");
  fclose(fp);
  return(errno);
  }

fclose(fp);

if (memcmp(buf, "BM", 2) == 0) {
  ret = convert_bmp(bmp_fn);
  } else if (memcmp(buf, "P1", 2) == 0) {
  ret = convert_pbm(bmp_fn);
  } else {
  fprintf(stderr, "Unknown file format; only 1 bit BMPs and PBMs suppoerted.\n");
  ret = 2;
  }

return(ret);
}

/*******************************************************************************
  convert_pbm()
*******************************************************************************/
int convert_pbm(char *bmp_fn)
{
fprintf(stderr, "PBMs not (yet?) supported...\n");
return(1);
}

/*******************************************************************************
  convert_bmp()
*******************************************************************************/
int convert_bmp(char *bmp_fn)
{
FILE *fp;
BITMAPFILEHEADER bfh;
BITMAPINFOHEADER bih;
RGBQUAD rgb;
unsigned char *bits;
unsigned long cb_bits;
int inc;
int bytes_per_line;
int row;
int col;
int i;
char *p[8];
unsigned char out;
unsigned char bit;
int r;
int lc = 0;
int comma_needed = 0;
int bytes = 0;
int negative = 0;
int padding_bytes = 0;
int bytes_in_block;

fp = fopen(bmp_fn, "rb");

if (fp == NULL ||
    fread(&bfh, 1, sizeof(bfh), fp) != sizeof(bfh) ||
    fread(&bih, 1, sizeof(bih), fp) != sizeof(bih) ||
    fread(&rgb, 1, sizeof(rgb), fp) != sizeof(rgb)) {
  perror("Failed to read bitmap file");
  fclose(fp);
  return(1);
  }

if (bih.biBitCount != 1) {
  fprintf(stderr, "This is not a  black and white bitmap...\n");
  fclose(fp);
  return(1);
  }

fprintf(stderr, "-- Source bitmap is %dx%d pixels\n", bih.biWidth, abs(bih.biHeight));

/* read the bitmap data */
fseek(fp, bfh.bfOffBits, SEEK_SET);
cb_bits = bfh.bfSize - bfh.bfOffBits;
bits = calloc(1, cb_bits);
if (fread(bits, 1, cb_bits, fp) != cb_bits) {
  perror("Failed to read bitmap file");
  fclose(fp);
  free(bits);
  return(1);
  }

fclose(fp);

bytes_per_line = bih.biWidth / 8;
if (bytes_per_line * 8 < bih.biWidth) {
  bytes_per_line++;
  }

/* calculate padding bytes; sometimes (only sometimes, otherwise,
 * things would get boring) pixel line lengths are DWORD or
 * WORD aligned
 */
padding_bytes = cb_bits / abs(bih.biHeight) - bih.biWidth / 8;
if (bih.biWidth % 8) {
  padding_bytes--;
  }

if (bih.biHeight > 0) {
  /* bottom up bitmap */
  inc = -1;
  row = bih.biHeight - 8;
  fprintf(stderr, "-- Bottom up bitmap\n");
  } else {
  fprintf(stderr, 
          "-- Warning: this is a top up/bottom down bitmap - not tested...\n"
          "   Complain to mm@rsj.de if it does not work ;-)\n");
  inc = 1;
  row = 8;
  }

/* negative (0 bit is black) bitmap? */
if (rgb.red == 0 && rgb.blue == 0 && rgb.green == 0) {
  negative = 1;
  fprintf(stderr, "-- Negative bitmap: black pixels are 0\n");
  }

printf("const unsigned char rgBitmap[] = {");

for (; row >= 0 && row < abs(bih.biHeight); row += inc * 8) {
  for (i = 0; i < 8; i++) {
    p[i] = bits + (row + i * inc * -1) * (bytes_per_line + padding_bytes);
    }

  bytes_in_block = 0;

  printf("%s\n  ", comma_needed ? "," : "");
  comma_needed = 0;
  lc = 0;
  
  for (col = 0; col < bytes_per_line; col++) {
    for (bit = 0; bit < 8; bit++) {
      out = 0x00;
      for (r = 0; r < 8; r++) {
        if (p[r][col] & ((unsigned char) (0x80 >> bit))) {
          out |= (unsigned char) (0x80 >> r);
          }
        }
      if (negative) {
        /* reverse bitmap */
        out ^= 0xff;
        }
      printf("%s0x%02x", comma_needed ? ", " : "", (int) out);
      bytes++;
      bytes_in_block++;
      comma_needed = 1;
      lc++;
      if (lc > 7) {
        printf(",\n  ");
        lc = 0;
        comma_needed = 0;
        }
      if (bytes_in_block >= bih.biWidth) {
        break;
        }
      }
    }
  }

printf("\n  };\n", row);  
fprintf(stderr, "\n-- Conversion done, dumped %d bytes.\n", bytes);
return(0);
}



/*******************************************************************************
  usage()
*******************************************************************************/
void usage(void)
{
fprintf(stderr, "\n");
fprintf(stderr, "bmp2sixo - convert 1bit Bitmap files and PBM\'s for SiXO source\n");
fprintf(stderr, "Version " VERSION "\n\n");
fprintf(stderr, "Usage:\n");
fprintf(stderr, "bmp2sixo <bitmap file name>\n\n");
fprintf(stderr, "dumps a nice C array to stdout\n");
fprintf(stderr, "e.g.:  bmp2sixo mzlogo.bmp > logo.c\n\n");
}

