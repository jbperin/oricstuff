extern int LargeX0;
extern int LargeY0;
extern int LargeX1;
extern int LargeY1;
extern void DrawClippedLine();

#define NB_POINTS_SKEL 8
#define NB_SEGMENTS_SKEL 12
#define CUBE_SIZE 32

char points3Dskel[]={
	 -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0// +CUBE_SIZE, 0 // P0
	,-CUBE_SIZE, -CUBE_SIZE,  -CUBE_SIZE, 0// -CUBE_SIZE, 0 // P1
	,+CUBE_SIZE, -CUBE_SIZE,  -CUBE_SIZE, 0// -CUBE_SIZE, 0 // P2
	,+CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0// +CUBE_SIZE, 0 // P3
	,-CUBE_SIZE, +CUBE_SIZE, CUBE_SIZE, 0// +CUBE_SIZE, 0 // P4
	,-CUBE_SIZE, +CUBE_SIZE,   -CUBE_SIZE, 0// -CUBE_SIZE, 0 // P5
	,+CUBE_SIZE, +CUBE_SIZE,  -CUBE_SIZE, 0// -CUBE_SIZE, 0 // P6
	,+CUBE_SIZE, +CUBE_SIZE, CUBE_SIZE, 0// +CUBE_SIZE, 0 // P7
};

unsigned char segmentsSkel[]={
	0, 1,
	1, 2,
	2, 3,
	3, 0,
	4, 5,
	5, 6,
	6, 7,
	7, 4,
	0, 4,
	1, 5,
	2, 6,
	3, 7,
};

char points2Dskel [NB_POINTS_SKEL	* SIZEOF_2DPOINT];


void drawSegments (unsigned char segments[], char pts2d[], unsigned char nbSeg ) {
	unsigned char ii;
    signed char aH1, aV1, aH2, aV2;
    unsigned char idxPt1, idxPt2;
    signed char   tmpH, tmpV;
    unsigned char m1, m2;
    unsigned char v1, v2;
    unsigned char isSegment2BeDrawn;

	for (ii=0; ii< nbSeg; ii++) {
		idxPt1 = segments[(ii<<1)+0];
        idxPt2 = segments[(ii<<1)+1];

        aH1=pts2d[(idxPt1<<2)+0];
        aV1=pts2d[(idxPt1<<2)+1];
        aH2=pts2d[(idxPt2<<2)+0];
        aV2=pts2d[(idxPt2<<2)+1];


        if (abs(aH2) < abs(aH1)) {
            tmpH = aH1;
            tmpV = aV1;
            aH1 = aH2;
            aV1 = aV2;
            aH2 = tmpH;
            aV2 = tmpV;
        }

        m1 = aH1 & ANGLE_MAX;
        m2 = aH2 & ANGLE_MAX;
        v1 = aH1 & ANGLE_VIEW;
        v2 = aH2 & ANGLE_VIEW;

        isSegment2BeDrawn = 0;
        if ((m1 == 0x00) || (m1 == ANGLE_MAX)) {
            if ((v1 == 0x00) || (v1 == ANGLE_VIEW)) {
                if((aH1 & 0x80) != (aH2 & 0x80)) {
                    if ((abs(aH2) < 127 - abs(aH1))) {
                        isSegment2BeDrawn = 1;
                    }
                }else {
                    isSegment2BeDrawn = 1;
                }
            }else{
                // P1 FRONT
                if ((m2 == 0x00) || (m2 == ANGLE_MAX)) {
                    // P2 FRONT
                    isSegment2BeDrawn = 1;
                } else {
                    // P2 BACK
                    if ((aH1 & 0x80) != (aH2 & 0x80)) {
                        if (abs(aH2) < 127 - abs(aH1)) {
                            isSegment2BeDrawn=1;
                        }
                    }
                }

            }
        }

		if (isSegment2BeDrawn) {
			LargeX0= 120  + ((int)(aH1)<<2);
			LargeY0= 100 - ((int)(aV1)<<2);
			LargeX1= 120  + ((int)(aH2)<<2);
			LargeY1= 100 - ((int)(aV2)<<2);
			DrawClippedLine();
		}
	}
}
void exampleDraw(){
    hires ();

    GenerateTables();  // for line8

    glCamPosX = -64;
    glCamPosY = 0;
    glCamPosZ = 0;

    glCamRotZ = 0;  // -128 -> -127 unit : 2PI/(2^8 - 1)
    glCamRotX = 0;

    glProject(points2Dskel, points3Dskel, NB_POINTS_SKEL, 0);

    HiresClear();

    drawSegments (segmentsSkel, points2Dskel, NB_SEGMENTS_SKEL );
    ScreenCopyHires();

}