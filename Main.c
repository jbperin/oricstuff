
#include "glOric.h"
#include "params.h"




#define LORES_SCREEN_ADDRESS            0xBB80 
#define HIRES_SCREEN_ADDRESS            0xA000
#define STANDARD_CHARSET_ADDRESS        0xB400
#define ALTERNATE_CHARSET_ADDRESS       0xB800
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

void mult(unsigned char x, unsigned char y, unsigned char *r){
    printf ("%d %d ", log2_tab[x] , log2_tab[y]);
    *r = exp_tab[log2_tab[x] + log2_tab[y]];
}

void det(unsigned char ux, unsigned char uy, unsigned char vx, unsigned char vy, unsigned char *r){
    unsigned char r1, r2;
    printf ("ux = %d, uy = %d, vx = %d, vy = %d\n", ux, uy, vx, vy);get();
    mult(ux, vy, &r1);
    mult(uy, vx, &r2);
    printf ("r1 = %d, r2 = %d\n", r1, r2);get();
    *r = r1-r2;
}

void uvmap (){
    unsigned char Px, Py, P0x, P0y, P1x, P1y, P2x, P2y;
    unsigned char v1x, v1y, v2x, v2y, vx, vy;
    unsigned char rx2, r02, rx1, r01, r12;
    unsigned char a, b;
    
    P0x = 20;   P0y = 20;
    P1x = 180;  P1y= 40;
    P2x = 40;   P2y = 140;
    
    Px = 100;   Py = 90;

    v1x = P1x-P0x; v1y = P1y - P0y;
    v2x = P2x-P0x; v2y = P2y - P0y;
    vx = Px - P0x;
    vy = Py - P0y;

    det(vx, vy, v2x, v2y, &rx2);
    // det(P0x, P0y, v2x, v2y, &r02);
    // det(vx, vy, v1x, v1y, &rx1);
    // det(P0x, P0y, v1x, v1y, &r01);
    // det(v1x, v1y, v2x, v2y, &r12);

    // printf ("rx2 = %d, r02 = %d, rx1 = %d, r01 = %d, r12 = %d\n", rx2, r02, rx1, r01, r12);
    // a = rx2-r02;
    // b = rx1-r01;
    // printf ("a = %d, b = %d\n", a, b);
    printf ("rx2 = %d\n", rx2);get();
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
    
    
    SetUpTables();
    // hires();
    myHires();

    // hrscrmem(10,10,CHANGE_INK_TO_GREEN);
    // hrscrmem(10,12,0x55);
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

