
#include "glOric.h"
#include "params.h"

#include "colorimg.c"
#include "dda.c"

#define LORES_SCREEN_ADDRESS            ((unsigned int)0xBB80)
#define HIRES_SCREEN_ADDRESS            ((unsigned int)0xA000)
#define STANDARD_CHARSET_ADDRESS        ((unsigned int)0xB400)
#define ALTERNATE_CHARSET_ADDRESS       ((unsigned int)0xB800)
#define STANDARD_HIRES_CHARSET_ADDRESS  ((unsigned int)0x9800)
#define NB_LESS_LINES_4_COLOR           4

#define CHANGE_INK_TO_BLACK	            0		
#define CHANGE_INK_TO_RED	            1		
#define CHANGE_INK_TO_GREEN	            2		
#define CHANGE_INK_TO_YELLOW	        3		
#define CHANGE_INK_TO_BLUE	            4		
#define CHANGE_INK_TO_MAGENTA           5			
#define CHANGE_INK_TO_CYAN	            6		
#define CHANGE_INK_TO_WHITE	            7	

#define USE_STANDARD_CHARSET    	                    8		
#define USE_ALTERNATE_CHARSET	                        9		
#define USE_DOUBLE_SIZE_STANDARD_CHARSET	            10		
#define USE_DOUBLE_SIZE_ALTERNATE_CHARSET	            11		
#define USE_DOUBLE_SIZE_BLINKING_STANDARD_CHARSET       12		
#define USE_DOUBLE_SIZE_BLINKING_ALTERNATE_CHARSET      13		
#define USE_BLINKING_STANDARD_CHARSET	                14		
#define USE_BLINKING_ALTERNATE_CHARSET	                15		
// Change Paper (background) color	
#define CHANGE_PAPER_TO_BLACK			                16		
#define CHANGE_PAPER_TO_RED	                            17		
#define CHANGE_PAPER_TO_GREEN	                        18		
#define CHANGE_PAPER_TO_YELLOW	                        19		
#define CHANGE_PAPER_TO_BLUE	                        20		
#define CHANGE_PAPER_TO_MAGENTA	                        21		
#define CHANGE_PAPER_TO_CYAN	                        22		
#define CHANGE_PAPER_TO_WHITE	                        23		
// Video control attributes	
#define SWITCH_TO_TEXT_MODE_60HZ		                24		
#define SWITCH_TO_TEXT_MODE_50HZ		                26		
#define SWITCH_TO_HIRES_MODE_60HZ	                    28		
#define SWITCH_TO_HIRES_MODE_50HZ                       30		


/*
 * VISIBILITY LIMITS
 */
#define ANGLE_MAX 0xC0
#define ANGLE_VIEW 0xE0

int multi40[] = {0, 40, 80, 120, 160, 200, 240, 280, 320, 360, 400, 440, 480, 520, 560, 600, 640, 680, 720, 760, 800, 840, 880, 920, 960, 1000, 1040};


//
// ===== screen.s =====
//
extern void ScreenCopyHires();
extern void HiresClear();

extern unsigned char exp_tab[];
extern unsigned char log2_tab[];

#define abs(x)                 (((x)<0)?-(x):(x))

#include "hrs.c"
#include "myHires.c"
#include "img.c"

#define lrscrmem(l,c,v) poke (LORES_SCREEN_ADDRESS+((l)*SCREEN_WIDTH)+(c),(v))

#define hrscrmem(l,c,v) \
    poke (HIRES_SCREEN_ADDRESS+((0+(l))*SCREEN_WIDTH)+(c),(v)); \
    poke (HIRES_SCREEN_ADDRESS+((1+(l))*SCREEN_WIDTH)+(c),(v)); \
    poke (HIRES_SCREEN_ADDRESS+((2+(l))*SCREEN_WIDTH)+(c),(v)); \
    poke (HIRES_SCREEN_ADDRESS+((3+(l))*SCREEN_WIDTH)+(c),(v)); \
    poke (HIRES_SCREEN_ADDRESS+((4+(l))*SCREEN_WIDTH)+(c),(v)); \
    poke (HIRES_SCREEN_ADDRESS+((5+(l))*SCREEN_WIDTH)+(c),(v)); \
    poke (HIRES_SCREEN_ADDRESS+((6+(l))*SCREEN_WIDTH)+(c),(v)); \
    poke (HIRES_SCREEN_ADDRESS+((7+(l))*SCREEN_WIDTH)+(c),(v)); 


void hrDrawChar(char code, unsigned char line, unsigned char column) {

    unsigned char* adr;
    adr =(unsigned char*)(STANDARD_HIRES_CHARSET_ADDRESS + code * 8);
    poke (HIRES_SCREEN_ADDRESS+((0+(line))*SCREEN_WIDTH)+(column),0x40 | *(adr++)); // (*(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8))); // 0x40 | *((unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8    )));
    poke (HIRES_SCREEN_ADDRESS+((1+(line))*SCREEN_WIDTH)+(column),0x40 | *(adr++)); // (*(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 1))); // 0x40 | *((unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 1)));
    poke (HIRES_SCREEN_ADDRESS+((2+(line))*SCREEN_WIDTH)+(column),0x40 | *(adr++)); // (*(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 2))); // 0x40 | *((unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 2)));
    poke (HIRES_SCREEN_ADDRESS+((3+(line))*SCREEN_WIDTH)+(column),0x40 | *(adr++)); // (1)); // 0x40 | *((unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 3)));
    poke (HIRES_SCREEN_ADDRESS+((4+(line))*SCREEN_WIDTH)+(column),0x40 | *(adr++)); // (1)); // 0x40 | *((unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 4)));
    poke (HIRES_SCREEN_ADDRESS+((5+(line))*SCREEN_WIDTH)+(column),0x40 | *(adr++)); // (1)); // 0x40 | *((unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 5)));
    poke (HIRES_SCREEN_ADDRESS+((6+(line))*SCREEN_WIDTH)+(column),0x40 | *(adr++)); // (1)); // 0x40 | *((unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 6)));
    poke (HIRES_SCREEN_ADDRESS+((7+(line))*SCREEN_WIDTH)+(column),0x40 | *(adr++)); // (1)); // 0x40 | *((unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 7)));

}
void hrplot (l,c){
    char cv ;
    unsigned char hidx = c/6;   
    unsigned char pidx = c%6;
    cv = *((char*)(HIRES_SCREEN_ADDRESS+l*SCREEN_WIDTH + hidx));
    cv |= 0x20>>pidx; 
    *((char*)(HIRES_SCREEN_ADDRESS+l*SCREEN_WIDTH + hidx)) = cv;
}
void chrplot (l,c, ink){
    char cv ;
    unsigned char hidx = c/6;   
    unsigned char pidx = c%6;
    *((char*)(HIRES_SCREEN_ADDRESS+l*SCREEN_WIDTH + hidx - 1)) = ink;
    cv = *((char*)(HIRES_SCREEN_ADDRESS+l*SCREEN_WIDTH + hidx));
    cv |= 0x20>>pidx; 
    *((char*)(HIRES_SCREEN_ADDRESS+l*SCREEN_WIDTH + hidx)) = cv;
}

void displayImage(){
       // image
   hires();
   poke(0x26a, 10);
   memcpy((void *)0xa000, img, 8000);
   key();
   get();
}


extern void SetUpTables();

void mainTest01()
{
    int ii;
    unsigned char res;
     
    // displayImage();

    // mult(160, 6, &res);
    // printf ("%d", res); get();
    // mult(8, 12, &res);
    // printf ("%d", res); get();
    // mult(7, 9, &res);
    // printf ("%d", res); get();
    
    //uvmap ();
    char code = 'A';

    unsigned char pat01 = *(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8    );
    unsigned char pat02 = *(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 1);
    unsigned char pat03 = *(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 2);
    unsigned char pat04 = *(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 3);
    unsigned char pat05 = *(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 4);
    unsigned char pat06 = *(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 5);
    unsigned char pat07 = *(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 6);
    unsigned char pat08 = *(unsigned char*)(STANDARD_CHARSET_ADDRESS + code * 8 + 7);
    unsigned char v;
    unsigned char *adr;
 
    
    printf ("%x %x %x %x %x %x %x %x\n", 
        pat01,
        pat02,
        pat03,
        pat04,
        pat05,
        pat06,
        pat07,
        pat08
    );





    get();
    SetUpTables();
    // hires();
    myHires();

    hrscrmem(10,10,CHANGE_INK_TO_BLUE);
    hrDrawChar('A', 10, 12);


    // 
    // hrDrawChar(code, 10, 12);

    hrscrmem(20,10,CHANGE_INK_TO_GREEN);
    hrscrmem(20,12,0x55);

    // h
    

    chrplot (100,120, CHANGE_INK_TO_GREEN);
    hrplot (101,121);



    // poke (0xbfdf, 0x1E);
    // poke (0x021f, 1);


    // poke (LORES_SCREEN_ADDRESS+(00*SCREEN_WIDTH)+0,SWITCH_TO_HIRES_MODE_50HZ);
    // memset(HIRES_SCREEN_ADDRESS, 64, 5000);
    // printf ("%x %x %x", *((char*)(LORES_SCREEN_ADDRESS)), *((char*)(LORES_SCREEN_ADDRESS+1)),*((char*)(LORES_SCREEN_ADDRESS+2)));
    // memcpy (ALTERNATE_CHARSET_ADDRESS, STANDARD_CHARSET_ADDRESS, LORES_SCREEN_ADDRESS-ALTERNATE_CHARSET_ADDRESS);
    // // poke (LORES_SCREEN_ADDRESS+(25*SCREEN_WIDTH)+3,USE_ALTERNATE_CHARSET);
    // AdvancedPrint (4, 25,"\11 COUCOU");


    // // memset(HIRES_SCREEN_ADDRESS, 64, 5120); // 5120 = 0xB400 (std char) - 0xA000 (hires screen)
    // // memset(LORES_SCREEN_ADDRESS, 32, 40*25);
    // for (ii=0; ii<20 ; ii++){
    //     poke (LORES_SCREEN_ADDRESS+(ii*SCREEN_WIDTH)+0,SWITCH_TO_HIRES_MODE_50HZ);
    // }
    // poke (LORES_SCREEN_ADDRESS,SWITCH_TO_TEXT_MODE_50HZ);
    // poke (LORES_SCREEN_ADDRESS+1,CHANGE_INK_TO_RED);
    // for (ii = 0; ii<=4 ; ii++){
    //     // poke (LORES_SCREEN_ADDRESS+(ii*SCREEN_WIDTH)+0,SWITCH_TO_TEXT_MODE_50HZ);
    //     // poke (LORES_SCREEN_ADDRESS+(ii*SCREEN_WIDTH)+0,CHANGE_PAPER_TO_GREEN);
    //     poke (LORES_SCREEN_ADDRESS+(ii*SCREEN_WIDTH)+0,CHANGE_INK_TO_RED);
    //     poke (LORES_SCREEN_ADDRESS+(ii*SCREEN_WIDTH)+16,USE_STANDARD_CHARSET);
    //     poke (LORES_SCREEN_ADDRESS+(ii*SCREEN_WIDTH)+17,SWITCH_TO_HIRES_MODE_50HZ);
    //     for (jj = 0; jj < 8; jj++) {
    //         poke (HIRES_SCREEN_ADDRESS+((ii*8+jj)*SCREEN_WIDTH)+24,SWITCH_TO_TEXT_MODE_50HZ);
    //     }
    // }
    // for (ii=5; ii<16 ; ii++){
    //     poke (LORES_SCREEN_ADDRESS+(ii*SCREEN_WIDTH)+0,CHANGE_INK_TO_GREEN);
    //     poke (LORES_SCREEN_ADDRESS+(ii*SCREEN_WIDTH)+1,SWITCH_TO_HIRES_MODE_50HZ);
    //     for (jj = 0; jj < 8; jj++) {
    //         poke (HIRES_SCREEN_ADDRESS+((ii*8+jj)*SCREEN_WIDTH)+39,SWITCH_TO_TEXT_MODE_50HZ);
    //     }
    // }
    // poke (LORES_SCREEN_ADDRESS+(24*SCREEN_WIDTH)+0,CHANGE_PAPER_TO_BLACK);
    // poke (LORES_SCREEN_ADDRESS+(25*SCREEN_WIDTH)+0,CHANGE_PAPER_TO_BLACK);
    // poke (LORES_SCREEN_ADDRESS+(26*SCREEN_WIDTH)+0,CHANGE_PAPER_TO_BLACK);
    // poke (LORES_SCREEN_ADDRESS+(27*SCREEN_WIDTH)+0,CHANGE_PAPER_TO_BLACK);


}

void prepareRGB(){
    int ii;

    // parcours de lignes de 3 en 3
    for (ii=0; ii < (SCREEN_HEIGHT - NB_LESS_LINES_4_COLOR)*8;  ii+=3){
        poke (HIRES_SCREEN_ADDRESS+((ii)*SCREEN_WIDTH),CHANGE_INK_TO_RED);
        poke (HIRES_SCREEN_ADDRESS+((ii+1)*SCREEN_WIDTH),CHANGE_INK_TO_GREEN);
        poke (HIRES_SCREEN_ADDRESS+((ii+2)*SCREEN_WIDTH),CHANGE_INK_TO_BLUE);
    }
}
unsigned char getTexturePixel(unsigned char l, unsigned char c){
    return bufimg[multi40[l] + c];
}

unsigned char encodeColor[] = { 0, 2, 5, 7 };

// line in [0..65] column in [0..79]
void colorSquare(unsigned char line, unsigned char column, unsigned char theColor){

    unsigned char r, g, b;
    unsigned char *adr;
    unsigned char current_byte;

    // retrieve the color components from the color value
    r = (theColor>>4)& 0x03;
    g = (theColor>>2)& 0x03;
    b = (theColor)& 0x03;
    // printf ("r=%d, g=%d, b=%d\n", r, g, b);
    // compute the start adress of the screen square to color
    adr = (unsigned char *)(HIRES_SCREEN_ADDRESS + (line*3)*SCREEN_WIDTH + (column>>1));
    // printf ("adr= %d\n", (unsigned int)adr);

    current_byte = *adr;
    // if lower part of the screen bytes
    if ((column&0x01) == 0) {
        current_byte = current_byte | (encodeColor [r]<<3);
    } else {
        current_byte = current_byte | encodeColor [r];
    }
    // printf ("byte change from %d to %d\n", *(adr), current_byte);
    *(adr) = current_byte;
    // printf ("adr= %d\n", adr);

    adr += SCREEN_WIDTH;

    current_byte = *adr;
    // if lower part of the screen bytes
    if ((column&0x01) == 0) {
        current_byte = current_byte | (encodeColor [g]<<3);
    } else {
        current_byte = current_byte | encodeColor [g];
    }
    *(adr) = current_byte;

    adr += SCREEN_WIDTH;

    current_byte = *adr;
    // if lower part of the screen bytes
    if ((column&0x01) == 0) {
        current_byte = current_byte | (encodeColor [b]<<3);
    } else {
        current_byte = current_byte | encodeColor [b];
    }
    *(adr) = current_byte;

}
unsigned char idxWall[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        , 0, 0, 0, 0, 0, 0, 0, 0};

unsigned char tabHeight[] = {
        40, 39, 39, 38, 37, 37, 36, 35, 34, 34, 33, 32, 32, 31, 30, 30
        , 29, 28, 27, 27, 26, 25, 25, 24, 23, 23, 22, 21, 20, 20, 19, 18
        , 18, 17, 16, 16, 15, 14, 13, 13, 12};

unsigned char tabTexCol[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
        , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
        , 32, 33, 34, 35, 36, 37, 38, 39};      

void main(){
    int ii, jj;

    signed char idxScreenLine, idxScreenCol;
    unsigned char height, texcolumn;
    hires();
    prepareRGB();

    //hrDrawChar('A', 10, 12);
    // colorSquare(30, 40, 60);
    // for (ii =0; ii < TEXTURE_WIDTH; ii++)
    //     for (jj =0; jj < TEXTURE_HEIGHT; jj++)
    //         colorSquare(jj, ii+4, bufimg[jj*TEXTURE_WIDTH + ii]);

    for (ii = 0; ii < 40; ii++) {
        height          = tabHeight[ii];
        texcolumn       = tabTexCol[ii];
        idxScreenCol    = 10+ii;

        ddaStartValue       = 0;
        ddaNbStep           = 2*height;
        ddaNbVal            = TEXTURE_HEIGHT;

        ddaInit();
        idxScreenLine       = 32 - height;
        // printf ("%d %d\n", ddaCurrentValue, idxScreenLine);
        if ((idxScreenLine >=0) && (idxScreenLine < 64)) {
            colorSquare(idxScreenLine, idxScreenCol, bufimg[multi40[ddaCurrentValue] + texcolumn]);
        }
        while (ddaCurrentValue < ddaEndValue) {
            (*ddaStepFunction)(); 
            // printf ("%d\n", ddaCurrentValue, idxScreenLine);
            if ((idxScreenLine >=0) && (idxScreenLine < 64)) {
                colorSquare(idxScreenLine, idxScreenCol, bufimg[multi40[ddaCurrentValue] + texcolumn]);
            }
            idxScreenLine   += 1;
        }

    }

}