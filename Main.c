
#include "glOric.h"
#include "params.h"




#define LORES_SCREEN_ADDRESS            0xBB80 
#define HIRES_SCREEN_ADDRESS            0xA000
#define STANDARD_CHARSET_ADDRESS        0xB400
#define ALTERNATE_CHARSET_ADDRESS       0xB800
#define STANDARD_HIRES_CHARSET_ADDRESS  0x9800
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

void main()
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

