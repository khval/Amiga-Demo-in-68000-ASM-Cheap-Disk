													//	;	copper2: 5 bitplans, ca marche tres bien sur 1200
													//	;	mais sur 500, le 5e bitplan ne s'affiche pas correctement ???
													//	;	Raison inconnue ???
													//	;	Assemble avec ASM-One V1.20 By T.F.A.

#define use_old_tracker 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/libblitter.h>
#include <proto/ptreplay.h>
#include <proto/layers.h>
#include <intuition/intuition.h>
#include <hardware/custom.h>

struct Window *win = NULL;
struct BitMap *copperBitmap = NULL;

#define demo_name "Quack"

#include "common.h"
#include "render.h"

extern union reg_u *emu_stack_ptr;
union reg_u emu_stack[10000];

#ifdef __amigaos4__
struct Custom _custom;
struct Custom *custom = &_custom;	// store locally... handle things with do_functions();
#else
struct Custom *custom = 0xDFF000;
#endif

uint32 *Bpls = NULL;
uint32 *Bpls2 = NULL;
uint32 *Mod = NULL;

uint32 *copper = NULL;
uint32 *copper2 = NULL;
uint32 *Com = NULL;
uint32 *r38d0 = NULL;
uint32 *Sprt = NULL;
uint32 *Eq1 = NULL;

char *Present = NULL;
char *PresentE = NULL;

char *PicN = NULL;
char *PicNE = NULL;

char *Scr1 = NULL;
char *Scr1E = NULL;

char *Font = NULL;
char *FontE = NULL;

uint16 Compteur = 8;

struct Module *mt_SongDataPtr = NULL;
const char *mt_data=NULL;

uint32 *coul,*coul0,*coul1,*coul2,*coul3,*coul4,*coul5,*coul6,*coul7,*coul8,*coul9,*coul10;
const char *mm1,*mm2,*mm3,*mm4,*mm5,*mm6,*mm7,*mm8,*mm9;

void *Tcoul[10];
const char *tmm[10];



extern char *Texte;
extern char *Texteinfo;
extern uint16 Ritchy_Volumes[];
extern uint16 TableEqu[];

char *PTtexte = NULL;

struct BackFillArgs
{
	int parm;
	int value;
};

void BackFill_Func(struct RastPort *ArgRP, struct BackFillArgs *MyArgs);

void init_PTtexte()
{
	PTtexte = Texte;
}

void init_Tcoul()
{
	int i;
	void *init[] = { coul1,coul2,coul3,coul4,coul5,coul6,coul7,coul8,coul9 };
	
	for (i=0;i<sizeof(init)/sizeof(void *);i++)
	{
		Tcoul[i] = init[i] ;
	}
}

void init_tmm()
{
	int i;
	const char *init[] = {mm1,mm2,mm3,mm4,mm5,mm6,mm7,mm8,mm9};
	
	for (i=0;i<sizeof(init)/sizeof(void *);i++)
		tmm[i] = init[i] ;
}

void PasSong();
void info();
void PasSouris();
void PasSong();
void Play();
void Pause();
void Filter();
void FonduQuitte();
void GereScroll();
void CopyFont();
void Fontu();
void PauseCafe2();
void EffCoul();
void RestoreFond();
void GereEqual();
void mt_init();
void mt_music();
void mt_VolumeSlide();
void mots_control();
void mt_TonePortNoChange();
void mt_TonePortaSetPer();
void mt_Vibrato2();
void mt_VolSlideDown();
void mt_VolSlideDown2();
void mt_jmploop();
void mt_DoRetrig();
void mt_rtnend();
void mt_end();
void WAITBLIT();
void SaveFond();
bool CheckMouse();
void _doBlitter();
void TestRender();


extern uint8 mt_speed;
extern uint8 mt_counter;
extern uint8 mt_SongPos;
extern uint8 mt_PBreakPos;
extern uint8 mt_PosJumpFlag;
extern uint8 mt_PBreakFlag;
extern uint8 mt_LowMask;
extern uint8 mt_PattDelTime;

extern uint8 mt_PattDelTime2[];

uint32 WAIT = 0;
uint16 _permit = 0;
uint16 mousex = 0x80;
uint16 mousey = 0x92;
uint8 lastx =	0;
uint8 lasty =	0;
uint8 Auto =	0;

char *Nounou,*NounouE;
char *Pic,*PicE;
char *Buffer,*BufferE;

#define ld_b(a) *((uint8 *) (a))
#define ld_sb(a) *((int8 *) (a))
#define ld_w(a) *((uint16 *) (a))
#define ld_sw(a) *((int16 *) (a))
#define ld_l(a) *((uint32 *) (a))
#define ld_sl(a) *((int32 *) (a))

#define st_b(a,v) *((uint8 *) (a)) = (uint8) (v)
#define st_w(a,v) *((uint16 *) (a)) = (uint16) (v)
#define st_l(a,v) *((uint32 *) (a)) = (uint32) (v)

#define swap_d(num) D ## num.b32 = (D ## num.lw << 16  | D ## num.hw)

bool esc_key = false;

													//	
													//	
void code()											//		section fast,code
{													//	
	movem_push(RD0,RA6);								//	r:	movem.l	d0-d7/a0-a6,-(a7)
													//	
													//		clr.l	0
													//	
													//	;	lea	$dff000,a5
													//	;	lea	safemem(pc),a0
													//	;	move	$1c(a5),(a0)
													//	;	or	#$c000,(a0)+
													//	;	move	2(a5),(a0)
													//	;	or	#$8100,(a0)
													//	
													//	;	move	#$7fff,d0
													//	;	move	d0,$96(a5)
													//	;	move	d0,$9a(a5)
													//	;	move	#$83E0,$96(a5)

													//	
	d0 = (uint32) PicN;									//		move.l	#PicN,d0
	a0 = (uint32) Bpls+6;								//		lea	Bpls+6,a0
	d1 = 5;											//		moveq	#5-1,d1
	for (;d1;d1--)										//	CopyBpls:
	{
		st_w(a0,d0);									//		move	d0,(a0)
		swap_d(0);									//		swap	d0
		st_w(a0-4,d0);									//		move	d0,-4(a0)
		swap_d(0);									//		swap	d0
		d0+=18;										//		add.l	#18,d0
		a0+=8;										//		addq.l	#8,a0
	}												//		dbf	d1,CopyBpls


													//	
	st_w(Mod+2,18*4);									//		move	#18*4,Mod+2
	st_w(Mod+6,18*4);									//		move	#18*4,Mod+6
													//	

													//		move.l	4.w,a6
													//		jsr	-132(a6)

#if use_old_tracker
	mt_init();											//		bsr	mt_init
#endif

													//		move.l	$6c.w,saveirq3+2
													//		move.l	#irq3,$6c.w
													//	
													//	
	a0 = (uint32) Bpls2+6;								//		lea	Bpls2+6,a0
	d0 = (uint32) Present;								//		move.l	#Present,d0
	for(d1=3;d1;d1--)									//		moveq	#3-1,d1
	{
		st_w(a0,D0.lw);									//	CopPres:move	d0,(a0)
													//		swap	d0
		st_w(a0-4,D0.hw);								//		move	d0,-4(a0)
													//		swap	d0
		d0+=28;										//		add.l	#28,d0
		a0+=8;										//		addq.l	#8,a0
	}												//		dbf	d1,CopPres
													//	


													//	
	a5 = (uint32) custom;								//		lea	$dff000,a5
	a0 = (uint32) copper2;								//		lea	copper2,a0
	st_l(a5+0x80,a0);									//		move.l	a0,$80(a5)
													//		clr	$88(a5)
													//	
	d0=50*2;											//		move	#50*2,d0
	PauseCafe2();										//		bsr	PauseCafe2
	st_w(Com+2,0xb200);								//		move	#$b200,Com+2
													//	
	for (d7=2;d7;d7--)									//		moveq	#2-1,d7
	{												//	President:
		d0=62;										//		move	#62,d0
		PauseCafe2();									//		bsr	PauseCafe2
		st_b(copper2+6,28);							//		add.b	#28,copper2+6
	}												//		dbf	d7,President
													//	
	d0=62;											//		move	#62,d0
	PauseCafe2();										//		bsr	PauseCafe2
													//	
	a5 = (uint32) custom;								//		lea	$dff000,a5
	a0 = (uint32) copper;								//		lea	copper,a0
	st_l(a5+0x80,a0);									//		move.l	a0,$80(a5)
													//		clr	$88(a5)
													//	
	d0=100;											//		move	#100,d0
	PauseCafe2();									//		bsr	PauseCafe2
													//	
	d7 = 431;										//		move	#431,d7
													//	
	do {
		WaitTOF();									//	M2:	cmp.b	#80,$dff006
													//		bne	M2
													//	M3:	cmp.b	#80,$dff006
													//		beq	M3
		d0 = 5;										//		moveq	#5-1,d0
		a0 = (uint32) Bpls+6;							//		lea	Bpls+6,a0
		for (;d0;d0--)									//	Monte:
		{
			d1= ld_w(a0-4);							//		move	-4(a0),d1
			swap_d(1);								//		swap	d1
			st_w(a0,D1.lw);								//		move	(a0),d1
			d1 += 18*5;								//		add.l	#18*5,d1
			st_w(a0,d1);								//		move	d1,(a0)
			swap_d(1);								//		swap	d1
			st_w(a0-4,d1);								//		move	d1,-4(a0)
			a0+=8;									//		addq.l	#8,a0
		}											//		dbf	d0,Monte
		d7-=1;										//		subq	#1,d7
	} while (d7>0);										//		bpl	M2
													//	
	Auto = -1;											//		move.b	#-1,Auto
													//	
	d0=50;											//		move	#50,d0
	PauseCafe2();										//		bsr	PauseCafe2
													//	
	EffCoul();											//		bsr	EffCoul
	st_w(coul1+6,0x358);								//		move	#$358,coul1+6
													//	
	// we don't have sprites so ignore...						//		move.l	#Pointeur,d0
													//		lea	Sprt+6,a0
													//		move	d0,(a0)
													//		swap	d0
													//		move	d0,-4(a0)
													//	
	a0 = (uint32) Nounou + (18*5*176);					//		lea	Nounou+[18*5*176],a0
	a1 = (uint32) Pic + 18;								//		lea	Pic+18,a1
													//		move	#[80*5]-1,d0
	for ( d0=80*5 ; d0 ; d0-- )								//	CopyPano:
	{
													//		moveq	#9-1,d1
		for ( d1=9 ; d1 ; d1-- )							//	CopyPan:
		{
			st_w(a1,ld_w(a0)); a0+=2; a1+=2;				//		move	(a0)+,(a1)+
		}											//		dbf	d1,CopyPan
		a1 += 24;									//		lea	24(a1),a1
	}												//		dbf	d0,CopyPano
													//	
	st_w( Mod+2 , 0xaa );								//		move	#$aa,Mod+2
	st_w( Mod+6 , 0xaa );								//		move	#$aa,Mod+6
	st_w( r38d0+2,0x38 );								//		move	#$38,r38d0+2
	st_w( r38d0+6,0xd0 );								//		move	#$d0,r38d0+6
													//	
	d0 = (uint32) Pic;									//		move.l	#Pic,d0
	a0 = (uint32) Bpls+6;								//		lea	Bpls+6,a0
	for(d1=5;d1;d1--)									//		moveq	#5-1,d1
	{												//	CopyBpls2:
		st_w(a0,d0);									//		move	d0,(a0)
		swap_d(0);									//		swap	d0
		st_w(a0-4,d0);									//		move	d0,-4(a0)
		swap_d(0);									//		swap	d0
		d0 += 42;									//		add.l	#42,d0
		a0 += 8;										//		addq.l	#8,a0
	}												//		dbf	d1,CopyBpls2
													//	
	st_w(coul10+6,0xF90);								//		move	#$F90,coul10+6
													//	
	SaveFond();										//		bsr	SaveFond
													//	

	// VHPOSR is $dff006
	do												//	souris:
	{

		WaitTOF();									//		cmp.b	#200,$dff006
		render_copper( custom,  (uint32 *) custom -> cop1lc ,   copperBitmap );
		BackFill_Func(NULL, NULL);
													//		bne.b	Souris
													//	s2:	cmp.b	#200,$dff006
													//		beq.b	s2
		// POTINP is $dff016								//	
													//		btst	#10,$dff016
													//		beq.b	Souris
													//	

													//	;	move	#$F00,$dff180
		GereScroll();									//		bsr.w	GereScroll
													//	;	move	#$0F0,$dff180


		WAITBLIT();									//	wBbb:	btst	#14,$dff002
													//		bne	Wbbb
		RestoreFond();									//		bsr	RestoreFond
													//	;	move	#$00,$dff180
													//	

		GereEqual();									//		bsr	GereEqual

													//	
		if (CheckMouse())								//		btst	#6,$bfe001
		{
													//		bne	PasSouris
			
		
													//	
			D1.hw = 0;								//		moveq	#0,d1
			D1.lw = mousey;							//		move	mousey,d1
													//		cmp	#$92+81,d1
			if (d1>=0x92+81)
			{
				PasSong();								//		bhs	PasSong
				continue;
			}

														//	
			EffCoul();										//		bsr	EffCoul

			d1 = 0;										//		moveq	#0,d1
			D1.lw = mousey;								//		move	mousey,d1
			D1.lw -= 0x92;									//		sub	#$92,d1
														//		divu	#9,d1
			D1.hw = D1.lw % 9;
			D1.lw /= 9;
														//		ext.l	d1		; 0-9
			d1 = D1.lw | ((D1.lw & 0x8000) ? 0xFFFF0000 : 0x00000000);

			d1 <<= 2;									//		lsl.l	#2,d1		; *4
			a0 = (uint32) Tcoul;								//		lea	Tcoul,a0

			a0 = ld_l(a0+d1);								//		move.l	(a0,d1),a0

			st_w(a0+6,0x358);								//		move	#$358,6(a0)
			
			D0.hw = 0;									//		moveq	#0,d0
			D0.lw = mousey;								//		move	mousey,d0
			d0 += 0x92;									//		sub	#$92,d0

			D0.hw = D0.lw % 9;								//		divu	#9,d0
			D0.lw /= 9;
														//		ext.l	d0
			d0 = D0.lw | ((D0.lw & 0x8000) ? 0xFFFF0000 : 0x00000000);

			d0 <<= 2;									//		lsl.l	#2,d0
			a0 = (uint32) tmm;								//		lea	tmm,a0
			d0 = ld_l(a0+d0);								//		move.l	(a0,d0),d0
	


#if use_old_tracker

			if ( (uint32) mt_data != d0)						//		cmp.l	mt_data,d0
			{											//		beq	PasSouris
														//	
														//		move	#-1,_permit
				mt_end();									//		bsr	mt_end
				mt_data = (char *) d0;						//		move.l	d0,mt_data
				mt_init();									//		bsr	mt_init
														//		clr	_permit
														//	
			};											//		bra	PasSouris

#endif

		}											//	PasSouris:

	// 0xbfec01 = CIAA serial data register (connected to keyboard)

													//	
													//		move.b	$bfec01,d0
													//		cmp.b	#$75,d0
													//		bne	souris
													//	

	} while ( ! esc_key );

}
													//
void PasSong()											//	PasSong:
{	if (d1>=0xE6)										//		cmp	#$e6,d1
	{												//		blo	PasSouris
		
													//	
		d0 = 0;										//		moveq	#0,d0
		d0 = mousex;									//		move	mousex,d0
		d0 -= 0x0127;									//		sub	#$127,d0
		d0 <<= 5;									//		lsr	#5,d0		; 0 - 4

		switch (d0)
		{
			case 0: 	Play();							//		beq	Play
					break;

			case 1:	Pause();							//		cmp	#1,d0
					break;							//		beq	Pause

			case 2:	Filter();							//		cmp	#2,d0
					break;							//		beq	Filter
		
			default:	info();
					break;
		}
	}


}

void info()												//	info:
{
	st_l(PTtexte,Texteinfo);								//		move.l	#Texteinfo,PTtexte								
													//		bra	PasSouris
}

void Play()
{
	_permit = 0;										//	Play:	clr	_permit
													//		bra	PasSouris

}

void Pause()
{													//	Pause:
	_permit = -1;										//		move	#-1,_permit
													//		move	#$F,$dff096
}													//		bra	PasSouris

void Filter()											//	Filter:
{													
	if (WAIT>=20)										//		cmp.l	#20,WAIT
	{												//		blo	PasSouris

													//		eor.b	#2,$bfe001
		WAIT = 0;										//		clr.l	WAIT
	}
}



void fin()
{
	a0 = (uint32) Ritchy_Volumes;							//	fin:	lea	Ritchy_Volumes(pc),a0
													//		move	(a0)+,d0		; Volumes 4 voies
													//		move	(a0)+,d1
													//		move	(a0)+,d2
													//		move	(a0)+,d3
													//	
													//	
													//		lea	$dff0a8,a0
													//		moveq	#0,d5
	do												//	Decrease:
	{
													//		move	d0,(a0)
													//		move	d1,$10(a0)
													//		move	d2,$20(a0)
													//		move	d3,$30(a0)
													//		cmp.b	#255,$dff006
													//		bne	Decrease
													//	
													//	Dec2:	move	d0,(a0)
													//		move	d1,$10(a0)
													//		move	d2,$20(a0)
													//		move	d3,$30(a0)
													//		cmp.b	#255,$dff006
													//		beq	Dec2
													//	
													//		tst	d5
													//		bne	NONO1
													//	
													//		tst	d0		; Decremente volumes
													//		beq	Vol0
													//		subq	#1,d0
	if (d1) d1--;									//	Vol0:	tst	d1
													//		beq	Vol1
													//		subq	#1,d1
	if (d2) d2--;									//	Vol1:	tst	d2
													//		beq	Vol2
													//		subq	#1,d2
	if (d3) d3--;									//	Vol2:	tst	d3
													//		beq	Vol3
													//		subq	#1,d3
													//	
													//	
		movem_push(RD0,RD5);							//	Vol3:	movem.l	d0-d5/a0,-(sp)
		movem_push(RA0,RA0);	
	
		FonduQuitte();									//		bsr	FonduQuitte

		movem_pop(RA0,RA0);							//		movem.l	(sp)+,d0-d5/a0
		movem_pop(RD0,RA6);
													//	
		d5 = !d5;										//	NONO1:	not	d5
													//	
		d4 = d0;										//		move	d0,d4
		d4 += d1;									//		add	d1,d4
		d4 += d2;									//		add	d2,d4
		d4 += d3;									//		add	d3,d4

	} while (d4>0);										//		bne	Decrease	; Jusqu'a tous a 0 !!!
													//	
													//		move.l	saveirq3+2,$6c.w
	mt_end();											//		bsr	mt_end
													//	
	a6 = ld_l(0x00000004);								//		move.l	4.w,a6
													//		jsr	-138(a6)
													//	
	a5 = (uint32) custom;								//		lea	$dff000,a5
													//	
													//	;	move	#$7fff,d0
													//	;	move	d0,$9a(a5)
													//	;	move	d0,$96(a5)
													//	;	lea	safemem(pc),a0
													//	;	move	(a0)+,$9a(a5)
													//	;	move	(a0),$96(a5)
													//	
	//	open library code..	==>						//		move.l	4.w,a6
	//	(hmmm.. no close library???)						//		lea	GfxName(pc),a1
													//		jsr	-408(a6)
													//		move.l	d0,a0
													//		move.l	d0,GfxBase
	// I think restore display... ==>							//		move.l	38(a0),$80(a5)
													//		clr	$88(a5)
													//	
	movem_pop(RD0,RA6);								//		movem.l	(a7)+,d0-d7/a0-a6	
	d0 = 0;											//		moveq	#0,d0
}													//		rts
													//	
void FonduQuitte()										//	FonduQuitte:
{
	a0 = (uint32) copper;								//		lea	Copper,a0
													//	
	d0 = ld_l(a0); a0+=4;								//	Fondu:	move.l	(a0)+,d0
	if ( (signed) d0 != -2)									//		cmp.l	#-2,d0
	{												//		beq	FinPass
													//	
		swap_d(0);									//		swap	d0
		if (D0.lw >= 0x180)								//		cmp	#$0180,d0
		{											//		blo	PasCoul
			if (D0.hw <= 0x01BE)						//		cmp	#$01BE,d0
			{										//		bhi	PasCoul
													//	
				swap_d(0);							//		swap	d0
													//		move	d0,d1
													//		and	#$00F,d1
				if (D0.lw & 0x00F)						//		beq	PasBleu
				{
					D0.lw--;							//		sub	#$001,d0
				}									//	PasBleu:
													//		move	d0,d1
													//		and	#$0F0,d1
				if (D0.lw & 0x0F0)						//		beq	PasVert
				{
					D0.lw-=0x10;						//		sub	#$010,d0
				}									//	PasVert:
													//		move	d0,d1
													//		and	#$F00,d1
				if (D0.lw & 0xF00)						//		beq	PasRouge
				{
					D0.lw-=0x100;						//		sub	#$100,d0
				}									//	PasRouge:
				st_w(a0-2,d0);							//		move	d0,-2(a0)	; Sauve coul
			}										//	PasCoul:
		}
		Fontu();
	}												//		bra	Fondu
}													//	FinPass:rts
													//	
													//	
void GereScroll()										//	GereScroll:
{
	Compteur -= 1;									//		sub	#1,Compteur
	if (Compteur<=0)									//		bpl.b	NoNewLetter
	{												//	
		a0 = (uint32) PTtexte;							//		move.l	PTtexte,a0
		d0 = ld_b(a0); a0++;							//		move.b	(a0)+,d0
		if (d0 == 0)									//		bne.b	PasFinDuTexte
		{
			a0 = (uint32) Texte;							//		lea	Texte,a0
			d0 = ld_b(a0); a0++;						//		move.b	(a0)+,d0

		}											//	PasFinDuTexte:
		a0 = (uint32) PTtexte;							//		move.l	a0,PTtexte
													//	
		CopyFont();									//		bsr.b	CopyFont
		Compteur = 7;									//		move	#7,Compteur
	}												//	NoNewLetter:
													//	
	a0 = (uint32) Scr1-(15*42*5)+2+14;					//		lea	Scr1-(15*42*5)+2+14,a0
	a1 = (uint32) Scr1-(15*42*5)+0+14;					//		lea	Scr1-(15*42*5)+0+14,a1
	a5 = (uint32) custom;								//		lea	$dff000,a5
/*
													//	WaitBlit:
	// we don't need to do this in sw.	 ==>				//		btst	#14,2(a5)
													//		bne.b	WaitBlit
*/
	a0 = ld_l(a5+0x50);								//		move.l	a0,$50(a5)
	a1 = ld_l(a5+0x54);								//		move.l	a1,$54(a5)
	st_l(0x64+a5,0x000E000E);						//		move.l	#$000E000E,$64(a5)
	st_l(0x40+a5,0xF9F00000);						//		move.l	#$f9f00000,$40(a5)
	st_l(0x44+a5,-1);								//		move.l	#-1,$44(a5)
	st_l(0x58+a5,(12*5*64)+(224/16));				//		move	#[12*5*64]+[224/16],$58(a5)	; Gozy
	_doBlitter();

}													//		rts
													//	
void CopyFont()										//	CopyFont:	; d0="caractere"
{	if (d0!=' ')									//		cmp.b	#' ',d0
	{												//		beq.b	Espace
		if ((d0>='A')&&(d0<='Z'))					//		cmp.b	#'A',d0
		{											//		blo.b	PasLettre
			 										//		cmp.b	#'Z',d0
													//		bhi.b	PasLettre
													//	
			d0=-'A';								//		sub.b	#'A',d0
			d0&=0xFF;								//		and.l	#$FF,d0
			Fontu();								//		bra.w	Fontu
			return;
		}											//	PasLettre:

		if ((d0>='0')&&(d0<='9'))					//		cmp.b	#'0',d0
		{											//		blo.b	Espace
													//		cmp.b	#'9',d0
													//		bhi.b	Espace
			d0-='0';									//		sub.b	#'0',d0		; Chiffre 0-9
													//		and.l	#$F,d0
			d0+=26;									//		add.l	#26,d0
			Fontu();								//		bra.w	Fontu
			return;
		}

		switch (d0)									//	
		{
			case '.':								//	Espace:	cmp.b	#'.',d0
				d0 = 36;							//		bne	PasPoint
				Fontu();							//		move.l	#36,d0
				return;								//		bra.w	Fontu
													//	PasPoint:
			case ',':								//		cmp.b	#',',d0
				d0 = 37;							//		bne	PasVirg
				Fontu();							//		move.l	#37,d0
				return;								//		bra.b	Fontu
													//	PasVirg:
			case '\'':								//		cmp.b	#"'",d0
				d0 = 38;							//		bne	PasApos
				Fontu();							//		move.l	#38,d0
				return;								//		bra.b	Fontu
													//	PasApos:
			case '!':								//		cmp.b	#"!",d0
				d0 = 39;							//		bne	PasExc
				Fontu();							//		move.l	#39,d0
				return;								//		bra.b	Fontu
													//	PasExc:	
			case ':':								//		cmp.b	#":",d0
				d0 = 40;							//		bne	Pas2Pts
				Fontu();							//		move.l	#40,d0
				return;								//		bra.b	Fontu
													//	Pas2Pts:
			case '-':								//		cmp.b	#"-",d0
				d0 = 41;							//		bne	PasTiret
				Fontu();							//		move.l	#41,d0
				return;								//		bra.b	Fontu
													//	PasTiret:
			case '?':								//		cmp.b	#"?",d0
				d0 = 42;							//		bne	PasInter
				Fontu();							//		move.l	#42,d0
				return;								//		bra.b	Fontu
													//	PasInter:
			case '>':								//		cmp.b	#">",d0
				d0 = 43;							//		bne	Esp
				Fontu();							//		move.l	#43,d0
				return;								//		bra.b	Fontu
		}											//	
	}
	a0 = (uint32) Scr1-(15*42*5)+40;							//	Esp:	lea	Scr1-(15*42*5)+40,a0
	d0 = 0;											//		moveq	#0,d0
	d1 = (12*5);									//		move	#[12*5]-1,d1
	for(;d1;d1--)									//	CopEsp:
	{
		st_b(a0,d0);								//		move.b	d0,(a0)
		a0 += 42;									//		lea	42(a0),a0
	}												//		dbf	d1,CopEsp
}													//		rts
													//	
void Fontu()											//	Fontu:
{
	a0 = (uint32) Font;									//		lea	Font,a0
	a1 = (uint32) Scr1-(15*42*5)+40;						//		lea	Scr1-(15*42*5)+40,a1
	a1 += d0;										//		add.l	d0,a0		; a0 sur bonne fonte
	d1 = (12*5);										//		move	#[12*5]-1,d1
	for (;d1;d1--)										//	CopyF:
	{
		st_b(a1,ld_b(a0));								//		move.b	(a0),(a1)
		a0 += 44;									//		lea	44(a0),a0
		a1 += 42;									//		lea	42(a1),a1
	}												//		dbf	d1,CopyF
}													//		rts
													//	
													//	
void EffCoul()											//	EffCoul:
{
	a0 = (uint32) Tcoul;									//		lea	Tcoul,a0
	for (d0=9;d0;d0--)									//		moveq	#9-1,d0
	{												//	DelCoul:
		a1 = ld_l(a0); a0+=4;							//		move.l	(a0)+,a1
		st_w(a1+6,0xFFD);								//		move	#$55d,6(a1)
	}												//		dbf	d0,DelCoul
}													//		rts
													//	
													//	;====================================================================

void SaveFond()										//	SaveFond:
{
	a0 = (uint32) Scr1-(15*42*5)+14;						//		lea	Scr1-(15*42*5)+14,a0
	a1 = (uint32) Buffer;									//		lea	Buffer,a1
	d0 = 12-1;										//		moveq	#12-1,d0
	for (d0=12;d0;d0--)
	{
		st_w(a1,ld_w(a0)); a1+=2;						//	SF:	move	(a0),(a1)+
		st_w(a1,ld_w(a0+42)); a1+=2;						//		move	42(a0),(a1)+
		st_w(a1,ld_w(a0+42*2)); a1+=2;					//		move	42*2(a0),(a1)+
		st_w(a1,ld_w(a0+42*3)); a1+=2;					//		move	42*3(a0),(a1)+
		st_w(a1,ld_w(a0+72*4)); a1+=2;					//		move	42*4(a0),(a1)+
		a0 += 42*5;									//		lea	42*5(a0),a0
	}												//		dbf	d0,SF
}													//		rts

													//	;====================================================================
void RestoreFond()										//	RestoreFond:
{
	a0 = (uint32) Buffer;									//		lea	Buffer,a0
	a1 = (uint32) Scr1-(15*42*5)+14;						//		lea	Scr1-(15*42*5)+14,a1
													//		moveq	#12-1,d0
	for(d0=12;d0;d0--)								
	{												
		st_w(a1,ld_w(a0)); a0+=2;					//	RF:	move	(a0)+,(a1)
		st_w(a1+42,ld_w(a0)); a0+=2;				//		move	(a0)+,42(a1)
		st_w(a1+42*2,ld_w(a0)); a0+=2;				//		move	(a0)+,42*2(a1)
		st_w(a1+42*3,ld_w(a0)); a0+=2;				//		move	(a0)+,42*3(a1)
		st_w(a1+42*4,ld_w(a0)); a0+=2;				//		move	(a0)+,42*4(a1)
		a1 += 42*5;									//		lea	42*5(a1),a1
	}												//		dbf	d0,RF
}													//		rts
													//	;====================================================================
void PauseCafe2()									//	PauseCafe2:	; d0=Nb secondes
{
	for (;d0;d0--)
	{
		WaitTOF();									//		cmp.b	#200,$dff006
		render_copper( custom,  (uint32 *) custom -> cop1lc ,   copperBitmap );
		BackFill_Func(NULL, NULL);
													//		bne	PauseCafe2
													//	pc22:	cmp.b	#200,$dff006
													//		beq	pc22
	}												//		dbf	d0,PauseCafe2
}													//		rts
													//	;====================================================================
void GereEqual()
{													//	GereEqual:
	a0= (uint32) TableEqu+2;							//		lea	TableEqu+2,a0
	a1= (uint32) Eq1+2;										//		lea	Eq1+2,a1
													//		moveq	#4-1,d0
	for (d0=4;d0;d0--)									//	CopyEqu:
	{												//		cmp	#$229,(a1)
		if ( ld_w(a1) != 0x229)							//		beq	PasDec
		{
			d1 = ld_w(a0);								//		move	(a0),d1	
			d1 -= ld_w(a1);								//		sub	d1,(a1)
		} // PasDec
		a0 +=4;										//	PasDec:	addq.l	#4,a0
		a1 +=4;										//		addq.l	#4,a1
	}												//		dbf	d0,CopyEqu
}													//		rts
													//	;====================================================================
void irq3()												//	irq3:
{
	movem_push(RD0,RA6);								//		movem.l	d0-d7/a0-a6,-(a7)
													//		move	$dff01e,d0
	// DFF01C == INTENAR								//		and	$dff01c,d0
	// test COOPER, DISK bits ==>							//		btst	#5,d0
													//		beq.w	NoPlay
													//	
	WAIT += 1;										//		addq.l	#1,WAIT
	mots_control();										//		bsr	mots_control
													//	
	if (!_permit)										//		tst	_permit
	{												//		bne	NoPlay
													//	
		mt_music();									//		bsr	mt_music
	}
													//	
	movem_pop(RD0,RA6);								//	NoPlay:	movem.l	(a7)+,d0-d7/a0-a6
													//	saveirq3:
}													//		jmp	$0
													//	;====================================================================
void mots_control()										//	mots_control:
{
													//		lea	Pointeur,a3
													//		move.b	$dff00a,d0
													//		move.b	d0,d2
													//	
													//		move.b	lasty,d1
													//		sub.b	d1,d0
													//		ext	d0
													//		add.w	d0,mousey
													//		move.b	d2,lasty
													//	
													//		move.b	$dff00b,d0
													//		move.b	d0,d2
													//		move.b	lastx,d1
													//		sub.b	d1,d0
													//		ext	d0
													//		add.w	d0,mousex
													//		move.b	d2,lastx
													//		cmp.w	#$1a6,mousex
													//		bls	paslimite
													//		move.w	#$1a6,mousex
													//	paslimite:
													//		cmp.w	#$127,mousex
													//		bhs	paslimite2
													//		move.w	#$127,mousex
													//	paslimite2:
													//		cmp.w	#$92,mousey
													//		bge	paslimite3
													//		move.w	#$92,mousey
													//	paslimite3:
													//		cmp.w	#$ee,mousey
													//		bls	paslimite4
													//		move.w	#$ee,mousey
													//	paslimite4:
													//		move.b	mousey+1,(a3)	;Adrsp0
													//		move.b	mousey,d0
													//		lsl.b	#2,d0
													//		move.w	mousex,d1
													//		ror.w	#1,d1
													//		bpl.w	pasneg
													//		or.w	#1,d0
													//	pasneg:
													//		move.b	d1,1(a3)	;Adrsp0+1
													//		move.w	mousey,d1
													//		add.w	#9,d1
													//		move.b	d1,2(a3)	;Adrsp0+2
													//		lsr.w	#8,d1
													//		lsl.w	#1,d1
													//		or.b	d1,d0
													//		move.b	d0,3(a3)	;Adrsp0+3
}													//		rts
													//	
													//	;********************************************
													//	;* ----- Protracker V1.1B Playroutine ----- *
													//	;* Lars "Zap" Hamre/Amiga Freelancers 1991  *
													//	;* Bekkeliveien 10, N-2010 STR�MMEN, Norway *
													//	;********************************************
													//	
													//	; VBlank Version 2:
													//	; Call mt_init to initialize the routine, then call mt_music on
													//	; each vertical blank (50 Hz). To end the song and turn off all
													//	; voices, call mt_end.
													//	
													//	; This playroutine is not very fast, optimized or well commented,
													//	; but all the new commands in PT1.1A should work.
													//	; If it's not good enough, you'll have to change it yourself.
													//	; We'll try to write a faster routine soon...
													//	
													//	; Changes from V1.0C playroutine:
													//	; - Vibrato depth changed to be compatible with Noisetracker 2.0.
													//	;   You'll have to double all vib. depths on old PT modules.
													//	; - Funk Repeat changed to Invert Loop.
													//	; - Period set back earlier when stopping an effect.
													//	
													//	;DMAWait = 300 ; Set this as low as possible without losing low notes.
													//	
#define n_note				0 						//	n_note		EQU	0  ; W
#define n_cmd				2 						//	n_cmd		EQU	2  ; W
#define n_cmdlo				3 						//	n_cmdlo		EQU	3  ; B
#define n_start				4 						//	n_start		EQU	4  ; L
#define n_length			8 						//	n_length	EQU	8  ; W
#define n_loopstart			10						//	n_loopstart	EQU	10 ; L
#define n_replen			14						//	n_replen	EQU	14 ; W
#define n_period			16						//	n_period	EQU	16 ; W
#define n_finetune			18						//	n_finetune	EQU	18 ; B
#define n_volume			19						//	n_volume	EQU	19 ; B
#define n_dmabit			20						//	n_dmabit	EQU	20 ; W
#define n_toneportdirec		22						//	n_toneportdirec	EQU	22 ; B
#define n_toneportspeed		23						//	n_toneportspeed	EQU	23 ; B
#define n_wantedperiod		24						//	n_wantedperiod	EQU	24 ; W
#define n_vibratocmd		26						//	n_vibratocmd	EQU	26 ; B
#define n_vibratopos		27						//	n_vibratopos	EQU	27 ; B
#define n_tremolocmd		28						//	n_tremolocmd	EQU	28 ; B
#define n_tremolopos		29						//	n_tremolopos	EQU	29 ; B
#define n_wavecontrol		30						//	n_wavecontrol	EQU	30 ; B
#define n_glissfunk			31						//	n_glissfunk	EQU	31 ; B
#define n_sampleoffset		32						//	n_sampleoffset	EQU	32 ; B
#define n_pattpos			33						//	n_pattpos	EQU	33 ; B
#define n_loopcount			34						//	n_loopcount	EQU	34 ; B
#define n_funkoffset		35						//	n_funkoffset	EQU	35 ; B
#define n_wavestart			36						//	n_wavestart	EQU	36 ; L
#define n_reallength		40						//	n_reallength	EQU	40 ; W

													//	
void mt_init()											//	mt_init
{
	if (mt_data) 
	{
		mt_SongDataPtr = PTLoadModule(mt_data);
		PTPlay(mt_SongDataPtr);
	}
	else
	{
		printf("mt_data is NULL\n");
	}
/*

	a0 = (uint32) mt_data;								//		move.l	mt_data,A0
	mt_SongDataPtr = (struct Module *) ld_l(a0);						//		MOVE.L	A0,mt_SongDataPtr
	a1 = a0;											//		MOVE.L	A0,A1
	a1 = a1 +952;										//		LEA	952(A1),A1
	d0 = 127;											//		MOVEQ	#127,D0
	d1 = 0;											//		MOVEQ	#0,D1
													//	mtloop
													//		MOVE.L	D1,D2
													//		SUBQ.W	#1,D0
													//	mtloop2
													//		MOVE.B	(A1)+,D1
													//		CMP.B	D2,D1
													//		BGT.S	mtloop
													//		DBRA	D0,mtloop2
													//		ADDQ.B	#1,D2
													//				
													//		LEA	mt_SampleStarts(PC),A1
													//		ASL.L	#8,D2
													//		ASL.L	#2,D2
													//		ADD.L	#1084,D2
													//		ADD.L	A0,D2
													//		MOVE.L	D2,A2
													//		MOVEQ	#30,D0
													//	mtloop3
													//		CLR.L	(A2)
													//		MOVE.L	A2,(A1)+
													//		MOVEQ	#0,D1
													//		MOVE.W	42(A0),D1
													//		ASL.L	#1,D1
													//		ADD.L	D1,A2
													//		ADD.L	#30,A0
													//		DBRA	D0,mtloop3
													//	
													//		OR.B	#2,$BFE001
													//		MOVE.B	#6,mt_speed
													//		CLR.B	mt_counter
													//		CLR.B	mt_SongPos
													//		CLR.W	mt_PatternPos
*/
}

void mt_end()											//	mt_end
{

	if (mt_SongDataPtr)
	{

		PTStop(mt_SongDataPtr);
		PTUnloadModule(mt_SongDataPtr);
		mt_SongDataPtr = NULL;
	}
	else
	{
		printf("No song is playing\n");
	}

/*

													//		CLR.W	$DFF0A8
													//		CLR.W	$DFF0B8
													//		CLR.W	$DFF0C8
													//		CLR.W	$DFF0D8
													//		MOVE.W	#$F,$DFF096
*/
}													//		RTS
													//	
void mt_music()										//	mt_music
{
													//		MOVEM.L	D0-D4/A0-A6,-(SP)
													//		ADDQ.B	#1,mt_counter
													//		MOVE.B	mt_counter(PC),D0
													//		CMP.B	mt_speed(PC),D0
													//		BLO.S	mt_NoNewNote
													//		CLR.B	mt_counter
													//		TST.B	mt_PattDelTime2
													//		BEQ.S	mt_GetNewNote
													//		BSR.S	mt_NoNewAllChannels
													//		BRA	mt_dskip
													//	
													//	mt_NoNewNote
													//		BSR.S	mt_NoNewAllChannels
													//		BRA	mt_NoNewPosYet
													//	
													//	mt_NoNewAllChannels
													//		LEA	$DFF0A0,A5
													//		LEA	mt_chan1temp(PC),A6
													//		BSR	mt_CheckEfx
													//		LEA	$DFF0B0,A5
													//		LEA	mt_chan2temp(PC),A6
													//		BSR	mt_CheckEfx
													//		LEA	$DFF0C0,A5
													//		LEA	mt_chan3temp(PC),A6
													//		BSR	mt_CheckEfx
													//		LEA	$DFF0D0,A5
													//		LEA	mt_chan4temp(PC),A6
													//		BRA	mt_CheckEfx
													//	
													//	mt_GetNewNote
													//		MOVE.L	mt_SongDataPtr(PC),A0
													//		LEA	12(A0),A3
													//		LEA	952(A0),A2	;pattpo
													//		LEA	1084(A0),A0	;patterndata
													//		MOVEQ	#0,D0
													//		MOVEQ	#0,D1
													//		MOVE.B	mt_SongPos(PC),D0
													//		MOVE.B	(A2,D0.W),D1
													//		ASL.L	#8,D1
													//		ASL.L	#2,D1
													//		ADD.W	mt_PatternPos(PC),D1
													//		CLR.W	mt_DMACONtemp
													//	
													//		LEA	$DFF0A0,A5
													//		LEA	mt_chan1temp(PC),A6
													//		BSR.S	mt_PlayVoice
													//		LEA	$DFF0B0,A5
													//		LEA	mt_chan2temp(PC),A6
													//		BSR.S	mt_PlayVoice
													//		LEA	$DFF0C0,A5
													//		LEA	mt_chan3temp(PC),A6
													//		BSR.S	mt_PlayVoice
													//		LEA	$DFF0D0,A5
													//		LEA	mt_chan4temp(PC),A6
													//		BSR.S	mt_PlayVoice
													//		BRA	mt_SetDMA
													//	
													//	mt_PlayVoice
													//		TST.L	(A6)
													//		BNE.S	mt_plvskip
													//		BSR	mt_PerNop
													//	mt_plvskip
													//		MOVE.L	(A0,D1.L),(A6)
													//		ADDQ.L	#4,D1
													//		MOVEQ	#0,D2
													//		MOVE.B	n_cmd(A6),D2
													//		AND.B	#$F0,D2
													//		LSR.B	#4,D2
													//		MOVE.B	(A6),D0
													//		AND.B	#$F0,D0
													//		OR.B	D0,D2
													//		TST.B	D2
													//		BEQ	mt_SetRegs
													//		MOVEQ	#0,D3
													//		LEA	mt_SampleStarts(PC),A1
													//		MOVE	D2,D4
													//		SUBQ.L	#1,D2
													//		ASL.L	#2,D2
													//		MULU	#30,D4
													//		MOVE.L	(A1,D2.L),n_start(A6)
													//	
													//		tst.b	AUTO
													//		beq	NOAUTO
													//	
													//		movem.l	d0-d1/a5,-(sp)	; EQUALIZER
													//		move.l	a5,d0
													//		sub.l	#$DFF0A0,d0
													//		lsr	#2,d0		; 0-4-8-16
													//		lea	TableEqu,a5
													//		move	(a5,d0.w),d1	; InitCol
													//		lea	Eq1+2,a5
													//		move	d1,(a5,d0.w)
													//		movem.l	(sp)+,d0-d1/a5
													//	
													//	NOAUTO:
													//	
													//		MOVE.W	(A3,D4.L),n_length(A6)
													//		MOVE.W	(A3,D4.L),n_reallength(A6)
													//		MOVE.B	2(A3,D4.L),n_finetune(A6)
													//		MOVE.B	3(A3,D4.L),n_volume(A6)
													//		MOVE.W	4(A3,D4.L),D3 ; Get repeat
													//		TST.W	D3
													//		BEQ.S	mt_NoLoop
													//		MOVE.L	n_start(A6),D2	; Get start
													//		ASL.W	#1,D3
													//		ADD.L	D3,D2		; Add repeat
													//		MOVE.L	D2,n_loopstart(A6)
													//		MOVE.L	D2,n_wavestart(A6)
													//		MOVE.W	4(A3,D4.L),D0	; Get repeat
													//		ADD.W	6(A3,D4.L),D0	; Add replen
													//		MOVE.W	D0,n_length(A6)
													//		MOVE.W	6(A3,D4.L),n_replen(A6)	; Save replen
													//		MOVEQ	#0,D0
													//		MOVE.B	n_volume(A6),D0
													//		MOVE.W	D0,8(A5)	; Set volume
													//	
													//		movem.l	d1/a5,-(sp)
													//	
													//		move.l	a5,d1
													//		sub.l	#$DFF0A0,d1		; $00 $10 $20 $30
													//		lsr	#3,d1			; 0 2 4 6
													//		lea	Ritchy_Volumes,a5
													//		move	d0,(a5,d1.w)		; Sauve volume pour quitter
													//	
													//		movem.l	(sp)+,d1/a5
													//	
													//		BRA.S	mt_SetRegs
													//	
													//	mt_NoLoop
													//		MOVE.L	n_start(A6),D2
													//		ADD.L	D3,D2
													//		MOVE.L	D2,n_loopstart(A6)
													//		MOVE.L	D2,n_wavestart(A6)
													//		MOVE.W	6(A3,D4.L),n_replen(A6)	; Save replen
													//		MOVEQ	#0,D0
													//		MOVE.B	n_volume(A6),D0
													//		MOVE.W	D0,8(A5)	; Set volume
													//	mt_SetRegs
													//		MOVE.W	(A6),D0
													//		AND.W	#$0FFF,D0
													//		BEQ	mt_CheckMoreEfx	; If no note
													//		MOVE.W	2(A6),D0
													//		AND.W	#$0FF0,D0
													//		CMP.W	#$0E50,D0
													//		BEQ.S	mt_DoSetFineTune
													//		MOVE.B	2(A6),D0
													//		AND.B	#$0F,D0
													//		CMP.B	#3,D0	; TonePortamento
													//		BEQ.S	mt_ChkTonePorta
													//		CMP.B	#5,D0
													//		BEQ.S	mt_ChkTonePorta
													//		CMP.B	#9,D0	; Sample Offset
													//		BNE.S	mt_SetPeriod
													//		BSR	mt_CheckMoreEfx
													//		BRA.S	mt_SetPeriod
													//	
													//	mt_DoSetFineTune
													//		BSR	mt_SetFineTune
													//		BRA.S	mt_SetPeriod
													//	
													//	mt_ChkTonePorta
													//		BSR	mt_SetTonePorta
													//		BRA	mt_CheckMoreEfx
													//	
													//	mt_SetPeriod
													//		MOVEM.L	D0-D1/A0-A1,-(SP)
													//		MOVE.W	(A6),D1
													//		AND.W	#$0FFF,D1
													//		LEA	mt_PeriodTable(PC),A1
													//		MOVEQ	#0,D0
													//		MOVEQ	#36,D7
													//	mt_ftuloop
													//		CMP.W	(A1,D0.W),D1
													//		BHS.S	mt_ftufound
													//		ADDQ.L	#2,D0
													//		DBRA	D7,mt_ftuloop
													//	mt_ftufound
													//		MOVEQ	#0,D1
													//		MOVE.B	n_finetune(A6),D1
													//		MULU	#36*2,D1
													//		ADD.L	D1,A1
													//		MOVE.W	(A1,D0.W),n_period(A6)
													//		MOVEM.L	(SP)+,D0-D1/A0-A1
													//	
													//		MOVE.W	2(A6),D0
													//		AND.W	#$0FF0,D0
													//		CMP.W	#$0ED0,D0 ; Notedelay
													//		BEQ	mt_CheckMoreEfx
													//	
													//		MOVE.W	n_dmabit(A6),$DFF096
													//		BTST	#2,n_wavecontrol(A6)
													//		BNE.S	mt_vibnoc
													//		CLR.B	n_vibratopos(A6)
													//	mt_vibnoc
													//		BTST	#6,n_wavecontrol(A6)
													//		BNE.S	mt_trenoc
													//		CLR.B	n_tremolopos(A6)
													//	mt_trenoc
													//		MOVE.L	n_start(A6),(A5)	; Set start
													//		MOVE.W	n_length(A6),4(A5)	; Set length
													//		MOVE.W	n_period(A6),D0
													//		MOVE.W	D0,6(A5)		; Set period
													//		MOVE.W	n_dmabit(A6),D0
													//		OR.W	D0,mt_DMACONtemp
													//		BRA	mt_CheckMoreEfx
													//	 
													//	mt_SetDMA
													//		MOVE.W	#600,D0
													//	mt_WaitDMA
													//		DBRA	D0,mt_WaitDMA
													//		MOVE.W	mt_DMACONtemp(PC),D0
													//		OR.W	#$8000,D0
													//		MOVE.W	D0,$DFF096
													//		MOVE.W	#600,D0
													//	mt_WaitDMA2
													//		DBRA	D0,mt_WaitDMA2
													//	
	a5 = custom;										//		LEA	$DFF000,A5
													//		LEA	mt_chan4temp(PC),A6
													//		MOVE.L	n_loopstart(A6),$D0(A5)
													//		MOVE.W	n_replen(A6),$D4(A5)
													//		LEA	mt_chan3temp(PC),A6
													//		MOVE.L	n_loopstart(A6),$C0(A5)
													//		MOVE.W	n_replen(A6),$C4(A5)
													//		LEA	mt_chan2temp(PC),A6
													//		MOVE.L	n_loopstart(A6),$B0(A5)
													//		MOVE.W	n_replen(A6),$B4(A5)
													//		LEA	mt_chan1temp(PC),A6
													//		MOVE.L	n_loopstart(A6),$A0(A5)
													//		MOVE.W	n_replen(A6),$A4(A5)
													//	
													//	mt_dskip
													//		ADD.W	#16,mt_PatternPos
													//		MOVE.B	mt_PattDelTime,D0
													//		BEQ.S	mt_dskc
													//		MOVE.B	D0,mt_PattDelTime2
													//		CLR.B	mt_PattDelTime
													//	mt_dskc	TST.B	mt_PattDelTime2
													//		BEQ.S	mt_dska
													//		SUBQ.B	#1,mt_PattDelTime2
													//		BEQ.S	mt_dska
													//		SUB.W	#16,mt_PatternPos
													//	mt_dska	TST.B	mt_PBreakFlag
													//		BEQ.S	mt_nnpysk
													//		SF	mt_PBreakFlag
													//		MOVEQ	#0,D0
													//		MOVE.B	mt_PBreakPos(PC),D0
													//		CLR.B	mt_PBreakPos
													//		LSL.W	#4,D0
													//		MOVE.W	D0,mt_PatternPos
													//	mt_nnpysk
													//		CMP.W	#1024,mt_PatternPos
													//		BLO.S	mt_NoNewPosYet
													//	mt_NextPosition	
													//		MOVEQ	#0,D0
													//		MOVE.B	mt_PBreakPos(PC),D0
													//		LSL.W	#4,D0
													//		MOVE.W	D0,mt_PatternPos
													//		CLR.B	mt_PBreakPos
													//		CLR.B	mt_PosJumpFlag
													//		ADDQ.B	#1,mt_SongPos
													//		AND.B	#$7F,mt_SongPos
													//		MOVE.B	mt_SongPos(PC),D1
													//		MOVE.L	mt_SongDataPtr(PC),A0
													//		CMP.B	950(A0),D1
													//		BLO.S	mt_NoNewPosYet
													//		CLR.B	mt_SongPos
													//	mt_NoNewPosYet	
													//		TST.B	mt_PosJumpFlag
													//		BNE.S	mt_NextPosition
													//		MOVEM.L	(SP)+,D0-D4/A0-A6
}													//		RTS
													//	
void mt_CheckEfx() 
{													//	mt_CheckEfx
													//		BSR	mt_UpdateFunk
													//		MOVE.W	n_cmd(A6),D0
													//		AND.W	#$0FFF,D0
													//		BEQ.S	mt_PerNop
													//		MOVE.B	n_cmd(A6),D0
													//		AND.B	#$0F,D0
													//		BEQ.S	mt_Arpeggio
													//		CMP.B	#1,D0
													//		BEQ	mt_PortaUp
													//		CMP.B	#2,D0
													//		BEQ	mt_PortaDown
													//		CMP.B	#3,D0
													//		BEQ	mt_TonePortamento
													//		CMP.B	#4,D0
													//		BEQ	mt_Vibrato
													//		CMP.B	#5,D0
													//		BEQ	mt_TonePlusVolSlide
													//		CMP.B	#6,D0
													//		BEQ	mt_VibratoPlusVolSlide
													//		CMP.B	#$E,D0
													//		BEQ	mt_E_Commands
													//	SetBack	MOVE.W	n_period(A6),6(A5)
													//		CMP.B	#7,D0
													//		BEQ	mt_Tremolo
													//		CMP.B	#$A,D0
													//		BEQ	mt_VolumeSlide
}

void mt_Return2()
{
													//	mt_Return2
}													//		RTS
													//	
void mt_PerNop()										//	mt_PerNop
{
	a5 = ld_w(a6+n_period);								//		MOVE.W	n_period(A6),6(A5)
}													//		RTS
													//	
void mt_Arpeggio() {								//	mt_Arpeggio
													//		MOVEQ	#0,D0
													//		MOVE.B	mt_counter(PC),D0
													//		DIVS	#3,D0
													//		SWAP	D0
													//		CMP.W	#0,D0
													//		BEQ.S	mt_Arpeggio2
													//		CMP.W	#2,D0
													//		BEQ.S	mt_Arpeggio1
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		LSR.B	#4,D0
													//		BRA.S	mt_Arpeggio3
													//	
													//	mt_Arpeggio1
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#15,D0
													//		BRA.S	mt_Arpeggio3
													//	
													//	mt_Arpeggio2
													//		MOVE.W	n_period(A6),D2
													//		BRA.S	mt_Arpeggio4
													//	
													//	mt_Arpeggio3
													//		ASL.W	#1,D0
													//		MOVEQ	#0,D1
													//		MOVE.B	n_finetune(A6),D1
													//		MULU	#36*2,D1
													//		LEA	mt_PeriodTable(PC),A0
													//		ADD.L	D1,A0
													//		MOVEQ	#0,D1
													//		MOVE.W	n_period(A6),D1
													//		MOVEQ	#36,D7
													//	mt_arploop
													//		MOVE.W	(A0,D0.W),D2
													//		CMP.W	(A0),D1
													//		BHS.S	mt_Arpeggio4
													//		ADDQ.L	#2,A0
													//		DBRA	D7,mt_arploop
}													//		RTS
													//	
void mt_Arpeggio4()	{								//	mt_Arpeggio4
													//		MOVE.W	D2,6(A5)
}													//		RTS
													//	
void mt_FinePortaUp() {								//	mt_FinePortaUp
													//		TST.B	mt_counter
													//		BNE.S	mt_Return2
													//		MOVE.B	#$0F,mt_LowMask
													//	mt_PortaUp
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	mt_LowMask(PC),D0
													//		MOVE.B	#$FF,mt_LowMask
													//		SUB.W	D0,n_period(A6)
													//		MOVE.W	n_period(A6),D0
													//		AND.W	#$0FFF,D0
													//		CMP.W	#113,D0
													//		BPL.S	mt_PortaUskip
													//		AND.W	#$F000,n_period(A6)
													//		OR.W	#113,n_period(A6)
													//	mt_PortaUskip
													//		MOVE.W	n_period(A6),D0
													//		AND.W	#$0FFF,D0
													//		MOVE.W	D0,6(A5)
}													//		RTS	
													//	 
void mt_FinePortaDown() {							//	mt_FinePortaDown
													//		TST.B	mt_counter
													//		BNE	mt_Return2
													//		MOVE.B	#$0F,mt_LowMask
													//	mt_PortaDown
													//		CLR.W	D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	mt_LowMask(PC),D0
													//		MOVE.B	#$FF,mt_LowMask
													//		ADD.W	D0,n_period(A6)
													//		MOVE.W	n_period(A6),D0
													//		AND.W	#$0FFF,D0
													//		CMP.W	#856,D0
													//		BMI.S	mt_PortaDskip
													//		AND.W	#$F000,n_period(A6)
													//		OR.W	#856,n_period(A6)
													//	mt_PortaDskip
													//		MOVE.W	n_period(A6),D0
													//		AND.W	#$0FFF,D0
													//		MOVE.W	D0,6(A5)
}													//		RTS
													//	
void mt_SetTonePorta() {							//	mt_SetTonePorta
													//		MOVE.L	A0,-(SP)
													//		MOVE.W	(A6),D2
													//		AND.W	#$0FFF,D2
													//		MOVEQ	#0,D0
													//		MOVE.B	n_finetune(A6),D0
													//		MULU	#37*2,D0
													//		LEA	mt_PeriodTable(PC),A0
													//		ADD.L	D0,A0
													//		MOVEQ	#0,D0
													//	mt_StpLoop
													//		CMP.W	(A0,D0.W),D2
													//		BHS.S	mt_StpFound
													//		ADDQ.W	#2,D0
													//		CMP.W	#37*2,D0
													//		BLO.S	mt_StpLoop
													//		MOVEQ	#35*2,D0
													//	mt_StpFound
													//		MOVE.B	n_finetune(A6),D2
													//		AND.B	#8,D2
													//		BEQ.S	mt_StpGoss
													//		TST.W	D0
													//		BEQ.S	mt_StpGoss
													//		SUBQ.W	#2,D0
													//	mt_StpGoss
													//		MOVE.W	(A0,D0.W),D2
													//		MOVE.L	(SP)+,A0
													//		MOVE.W	D2,n_wantedperiod(A6)
													//		MOVE.W	n_period(A6),D0
													//		CLR.B	n_toneportdirec(A6)
													//		CMP.W	D0,D2
													//		BEQ.S	mt_ClearTonePorta
													//		BGE	mt_Return2
													//		MOVE.B	#1,n_toneportdirec(A6)
}													//		RTS
													//	
void mt_ClearTonePorta()									//	mt_ClearTonePorta
{
													//		CLR.W	n_wantedperiod(A6)
}													//		RTS
													//	
void mt_TonePortamento()							//	mt_TonePortamento
{
													//		MOVE.B	n_cmdlo(A6),D0
													//		BEQ.S	mt_TonePortNoChange
													//		MOVE.B	D0,n_toneportspeed(A6)
													//		CLR.B	n_cmdlo(A6)
	mt_TonePortNoChange();
}

void mt_TonePortNoChange()								//	mt_TonePortNoChange
{
													//		TST.W	n_wantedperiod(A6)
													//		BEQ	mt_Return2
													//		MOVEQ	#0,D0
													//		MOVE.B	n_toneportspeed(A6),D0
													//		TST.B	n_toneportdirec(A6)
													//		BNE.S	mt_TonePortaUp
													//	mt_TonePortaDown
													//		ADD.W	D0,n_period(A6)
													//		MOVE.W	n_wantedperiod(A6),D0
													//		CMP.W	n_period(A6),D0
													//		BGT.S	mt_TonePortaSetPer
													//		MOVE.W	n_wantedperiod(A6),n_period(A6)
													//		CLR.W	n_wantedperiod(A6)
	mt_TonePortaSetPer();								//		BRA.S	mt_TonePortaSetPer
}													//	

void 	mt_TonePortaUp()									//	mt_TonePortaUp
{
													//		SUB.W	D0,n_period(A6)
													//		MOVE.W	n_wantedperiod(A6),D0
													//		CMP.W	n_period(A6),D0
													//		BLT.S	mt_TonePortaSetPer
													//		MOVE.W	n_wantedperiod(A6),n_period(A6)
													//		CLR.W	n_wantedperiod(A6)
	mt_TonePortaSetPer();
}
													//	
void mt_TonePortaSetPer()								//	mt_TonePortaSetPer
{
													//		MOVE.W	n_period(A6),D2
													//		MOVE.B	n_glissfunk(A6),D0
													//		AND.B	#$0F,D0
													//		BEQ.S	mt_GlissSkip
													//		MOVEQ	#0,D0
													//		MOVE.B	n_finetune(A6),D0
													//		MULU	#36*2,D0
													//		LEA	mt_PeriodTable(PC),A0
													//		ADD.L	D0,A0
													//		MOVEQ	#0,D0
													//	mt_GlissLoop
													//		CMP.W	(A0,D0.W),D2
													//		BHS.S	mt_GlissFound
													//		ADDQ.W	#2,D0
													//		CMP.W	#36*2,D0
													//		BLO.S	mt_GlissLoop
													//		MOVEQ	#35*2,D0
													//	mt_GlissFound
													//		MOVE.W	(A0,D0.W),D2
													//	mt_GlissSkip
													//		MOVE.W	D2,6(A5) ; Set period
}													//		RTS
													//	
void mt_Vibrato()									//	mt_Vibrato
{
													//		MOVE.B	n_cmdlo(A6),D0
													//		BEQ.S	mt_Vibrato2
													//		MOVE.B	n_vibratocmd(A6),D2
													//		AND.B	#$0F,D0
													//		BEQ.S	mt_vibskip
													//		AND.B	#$F0,D2
													//		OR.B	D0,D2
													//	mt_vibskip
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$F0,D0
													//		BEQ.S	mt_vibskip2
													//		AND.B	#$0F,D2
													//		OR.B	D0,D2
													//	mt_vibskip2
													//		MOVE.B	D2,n_vibratocmd(A6)
	mt_Vibrato2();
}

void mt_Vibrato2()										//	mt_Vibrato2
{
													//		MOVE.B	n_vibratopos(A6),D0
													//		LEA	mt_VibratoTable(PC),A4
													//		LSR.W	#2,D0
													//		AND.W	#$001F,D0
													//		MOVEQ	#0,D2
													//		MOVE.B	n_wavecontrol(A6),D2
													//		AND.B	#$03,D2
													//		BEQ.S	mt_vib_sine
													//		LSL.B	#3,D0
													//		CMP.B	#1,D2
													//		BEQ.S	mt_vib_rampdown
													//		MOVE.B	#255,D2
													//		BRA.S	mt_vib_set
													//	mt_vib_rampdown
													//		TST.B	n_vibratopos(A6)
													//		BPL.S	mt_vib_rampdown2
													//		MOVE.B	#255,D2
													//		SUB.B	D0,D2
													//		BRA.S	mt_vib_set
													//	mt_vib_rampdown2
													//		MOVE.B	D0,D2
													//		BRA.S	mt_vib_set
													//	mt_vib_sine
													//		MOVE.B	0(A4,D0.W),D2
													//	mt_vib_set
													//		MOVE.B	n_vibratocmd(A6),D0
													//		AND.W	#15,D0
													//		MULU	D0,D2
													//		LSR.W	#7,D2
													//		MOVE.W	n_period(A6),D0
													//		TST.B	n_vibratopos(A6)
													//		BMI.S	mt_VibratoNeg
													//		ADD.W	D2,D0
													//		BRA.S	mt_Vibrato3
													//	mt_VibratoNeg
													//		SUB.W	D2,D0
													//	mt_Vibrato3
													//		MOVE.W	D0,6(A5)
													//		MOVE.B	n_vibratocmd(A6),D0
													//		LSR.W	#2,D0
													//		AND.W	#$003C,D0
													//		ADD.B	D0,n_vibratopos(A6)
}													//		RTS
													//	
void mt_TonePlusVolSlide()							//	mt_TonePlusVolSlide
{
	mt_TonePortNoChange();							//		BSR	mt_TonePortNoChange
	mt_VolumeSlide();								//		BRA	mt_VolumeSlide
}
													//	
void mt_VibratoPlusVolSlide()						//	mt_VibratoPlusVolSlide
{
	mt_Vibrato2();									//		BSR.S	mt_Vibrato2
	mt_VolumeSlide();								//		BRA	mt_VolumeSlide
}													//	

void mt_Tremolo()									//	mt_Tremolo
{
													//		MOVE.B	n_cmdlo(A6),D0
													//		BEQ.S	mt_Tremolo2
													//		MOVE.B	n_tremolocmd(A6),D2
													//		AND.B	#$0F,D0
													//		BEQ.S	mt_treskip
													//		AND.B	#$F0,D2
													//		OR.B	D0,D2
													//	mt_treskip
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$F0,D0
													//		BEQ.S	mt_treskip2
													//		AND.B	#$0F,D2
													//		OR.B	D0,D2
													//	mt_treskip2
													//		MOVE.B	D2,n_tremolocmd(A6)
													//	mt_Tremolo2
													//		MOVE.B	n_tremolopos(A6),D0
													//		LEA	mt_VibratoTable(PC),A4
													//		LSR.W	#2,D0
													//		AND.W	#$001F,D0
													//		MOVEQ	#0,D2
													//		MOVE.B	n_wavecontrol(A6),D2
													//		LSR.B	#4,D2
													//		AND.B	#$03,D2
													//		BEQ.S	mt_tre_sine
													//		LSL.B	#3,D0
													//		CMP.B	#1,D2
													//		BEQ.S	mt_tre_rampdown
													//		MOVE.B	#255,D2
													//		BRA.S	mt_tre_set
													//	mt_tre_rampdown
													//		TST.B	n_vibratopos(A6)
													//		BPL.S	mt_tre_rampdown2
													//		MOVE.B	#255,D2
													//		SUB.B	D0,D2
													//		BRA.S	mt_tre_set
													//	mt_tre_rampdown2
													//		MOVE.B	D0,D2
													//		BRA.S	mt_tre_set
													//	mt_tre_sine
													//		MOVE.B	0(A4,D0.W),D2
													//	mt_tre_set
													//		MOVE.B	n_tremolocmd(A6),D0
													//		AND.W	#15,D0
													//		MULU	D0,D2
													//		LSR.W	#6,D2
													//		MOVEQ	#0,D0
													//		MOVE.B	n_volume(A6),D0
													//		TST.B	n_tremolopos(A6)
													//		BMI.S	mt_TremoloNeg
													//		ADD.W	D2,D0
													//		BRA.S	mt_Tremolo3
													//	mt_TremoloNeg
													//		SUB.W	D2,D0
													//	mt_Tremolo3
													//		BPL.S	mt_TremoloSkip
													//		CLR.W	D0
													//	mt_TremoloSkip
													//		CMP.W	#$40,D0
													//		BLS.S	mt_TremoloOk
													//		MOVE.W	#$40,D0
													//	mt_TremoloOk
													//		MOVE.W	D0,8(A5)
													//		MOVE.B	n_tremolocmd(A6),D0
													//		LSR.W	#2,D0
													//		AND.W	#$003C,D0
													//		ADD.B	D0,n_tremolopos(A6)
}													//		RTS
													//	
void mt_SampleOffset()									//	mt_SampleOffset
{
	d0 =0;											//		MOVEQ	#0,D0
	D0.b0 = ld_b(a6+n_cmdlo);							//		MOVE.B	n_cmdlo(A6),D0
	if (D0.b0 != 0)										//		BEQ.S	mt_sononew
	{
		st_b(a6+n_sampleoffset,D0.b0);					//		MOVE.B	D0,n_sampleoffset(A6)
	}												//	mt_sononew
	d0 = ld_b(a6+n_sampleoffset);							//		MOVE.B	n_sampleoffset(A6),D0
	D0.lw <<= 7;										//		LSL.W	#7,D0
	if ( D0.lw <= ld_w(a6+n_length) )						//		CMP.W	n_length(A6),D0
	{												//		BGE.S	mt_sofskip
		st_w(a6+n_length,ld_w(a6+n_length)- d0);			//		SUB.W	D0,n_length(A6)
		D0.lw <<= 1;									//		LSL.W	#1,D0
		st_l(a6+n_start,ld_l(a6+n_start)+d0);				//		ADD.L	D0,n_start(A6)
	}
}													//		RTS

void mt_sofskip()										//	mt_sofskip
{
													//		MOVE.W	#$0001,n_length(A6)
}													//		RTS
													//	
void mt_VolumeSlide()									//	mt_VolumeSlide
{
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		LSR.B	#4,D0
													//		TST.B	D0
													//		BEQ.S	mt_VolSlideDown
	if (d0 == 0)
	{
		mt_VolSlideDown();
		return;
	}
}

void mt_VolSlideUp()										//	mt_VolSlideUp
{
													//		ADD.B	D0,n_volume(A6)
													//		CMP.B	#$40,n_volume(A6)
													//		BMI.S	mt_vsuskip
													//		MOVE.B	#$40,n_volume(A6)
													//	mt_vsuskip
													//		MOVE.B	n_volume(A6),D0
													//		MOVE.W	D0,8(A5)
}													//		RTS
													//	
void mt_VolSlideDown()								//	mt_VolSlideDown
{
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
	mt_VolSlideDown2();
}

void 	mt_VolSlideDown2()									//	mt_VolSlideDown2
{
													//		SUB.B	D0,n_volume(A6)
													//		BPL.S	mt_vsdskip
													//		CLR.B	n_volume(A6)
													//	mt_vsdskip
													//		MOVE.B	n_volume(A6),D0
													//		MOVE.W	D0,8(A5)
}													//		RTS
													//	
void mt_PositionJump()								//	mt_PositionJump
{
													//		MOVE.B	n_cmdlo(A6),D0
													//		SUBQ.B	#1,D0
													//		MOVE.B	D0,mt_SongPos
													//	mt_pj2	CLR.B	mt_PBreakPos
													//		ST 	mt_PosJumpFlag
}													//		RTS
													//	
void mt_VolumeChange()								//	mt_VolumeChange
{
	d0 = 0;											//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		CMP.B	#$40,D0
													//		BLS.S	mt_VolumeOk
													//		MOVEQ	#$40,D0
													//	mt_VolumeOk
													//		MOVE.B	D0,n_volume(A6)
													//		MOVE.W	D0,8(A5)
}													//		RTS
													//	
void mt_PatternBreak()								//	mt_PatternBreak
{
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		MOVE.L	D0,D2
													//		LSR.B	#4,D0
													//		MULU	#10,D0
													//		AND.B	#$0F,D2
													//		ADD.B	D2,D0
													//		CMP.B	#63,D0
													//		BHI.S	mt_pj2
													//		MOVE.B	D0,mt_PBreakPos
													//		ST	mt_PosJumpFlag
}													//		RTS
													//	
void mt_SetSpeed()									//	mt_SetSpeed
{
													//		MOVE.B	3(A6),D0
													//		BEQ	mt_Return2
													//		CLR.B	mt_counter
													//		MOVE.B	D0,mt_speed
}													//		RTS
													//	
													//	mt_CheckMoreEfx
													//		BSR	mt_UpdateFunk
													//		MOVE.B	2(A6),D0
													//		AND.B	#$0F,D0
													//		CMP.B	#$9,D0
													//		BEQ	mt_SampleOffset
													//		CMP.B	#$B,D0
													//		BEQ	mt_PositionJump
													//		CMP.B	#$D,D0
													//		BEQ.S	mt_PatternBreak
													//		CMP.B	#$E,D0
													//		BEQ.S	mt_E_Commands
													//		CMP.B	#$F,D0
													//		BEQ.S	mt_SetSpeed
													//		CMP.B	#$C,D0
													//		BEQ	mt_VolumeChange
													//		BRA	mt_PerNop
													//	
													//	mt_E_Commands
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$F0,D0
													//		LSR.B	#4,D0
													//		BEQ.S	mt_FilterOnOff
													//		CMP.B	#1,D0
													//		BEQ	mt_FinePortaUp
													//		CMP.B	#2,D0
													//		BEQ	mt_FinePortaDown
													//		CMP.B	#3,D0
													//		BEQ.S	mt_SetGlissControl
													//		CMP.B	#4,D0
													//		BEQ	mt_SetVibratoControl
													//		CMP.B	#5,D0
													//		BEQ	mt_SetFineTune
													//		CMP.B	#6,D0
													//		BEQ	mt_JumpLoop
													//		CMP.B	#7,D0
													//		BEQ	mt_SetTremoloControl
													//		CMP.B	#9,D0
													//		BEQ	mt_RetrigNote
													//		CMP.B	#$A,D0
													//		BEQ	mt_VolumeFineUp
													//		CMP.B	#$B,D0
													//		BEQ	mt_VolumeFineDown
													//		CMP.B	#$C,D0
													//		BEQ	mt_NoteCut
													//		CMP.B	#$D,D0
													//		BEQ	mt_NoteDelay
													//		CMP.B	#$E,D0
													//		BEQ	mt_PatternDelay
													//		CMP.B	#$F,D0
													//		BEQ	mt_FunkIt
													//		RTS
													//	
void mt_FilterOnOff()								//	mt_FilterOnOff
{
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#1,D0
													//		ASL.B	#1,D0
													//		AND.B	#$FD,$BFE001
													//		OR.B	D0,$BFE001
}													//		RTS	
													//	
void mt_SetGlissControl()							//	mt_SetGlissControl
{
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
													//		AND.B	#$F0,n_glissfunk(A6)
													//		OR.B	D0,n_glissfunk(A6)
}													//		RTS
													//	
void mt_SetVibratoControl()							//	mt_SetVibratoControl
{
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
													//		AND.B	#$F0,n_wavecontrol(A6)
													//		OR.B	D0,n_wavecontrol(A6)
}													//		RTS
													//	
void mt_SetFineTune()								//	mt_SetFineTune
{
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
													//		MOVE.B	D0,n_finetune(A6)
}													//		RTS
													//	
void mt_JumpLoop()									//	mt_JumpLoop
{
													//		TST.B	mt_counter
													//		BNE	mt_Return2
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
													//		BEQ.S	mt_SetLoop
													//		TST.B	n_loopcount(A6)
													//		BEQ.S	mt_jumpcnt
													//		SUBQ.B	#1,n_loopcount(A6)
													//		BEQ	mt_Return2
	mt_jmploop();
}

void mt_jmploop()										//	mt_jmploop
{
													//		MOVE.B	n_pattpos(A6),mt_PBreakPos
													//		ST	mt_PBreakFlag
}													//		RTS

void mt_jumpcnt()										//	mt_jumpcnt
{
	st_l(a6+n_loopcount,d0);								//		MOVE.B	D0,n_loopcount(A6)
	mt_jmploop();										//		BRA.S	mt_jmploop
}													//

void mt_SetLoop()										//	mt_SetLoop
{
													//		MOVE.W	mt_PatternPos(PC),D0
													//		LSR.W	#4,D0
													//		MOVE.B	D0,n_pattpos(A6)
}													//		RTS
													//	
void mt_SetTremoloControl()								//	mt_SetTremoloControl
{
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
													//		LSL.B	#4,D0
													//		AND.B	#$0F,n_wavecontrol(A6)
													//		OR.B	D0,n_wavecontrol(A6)
}													//		RTS
													//	
void mt_RetrigNote()										//	mt_RetrigNote
{
	movem_push(RD1,RD1);								//		MOVE.L	D1,-(SP)
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
													//		BEQ.S	mt_rtnend
													//		MOVEQ	#0,D1
													//		MOVE.B	mt_counter(PC),D1
													//		BNE.S	mt_rtnskp
													//		MOVE.W	(A6),D1
													//		AND.W	#$0FFF,D1
													//		BNE.S	mt_rtnend
													//		MOVEQ	#0,D1
													//		MOVE.B	mt_counter(PC),D1
													//	mt_rtnskp
													//		DIVU	D0,D1
	swap_d(1)	;										//		SWAP	D1
	if (d1 != 0 )										//		TST.W	D1
	{
		mt_rtnend();									//		BNE.S	mt_rtnend
		return;
	}

	mt_DoRetrig();
}

void mt_DoRetrig()										//	mt_DoRetrig
{
													//		MOVE.W	n_dmabit(A6),$DFF096	; Channel DMA off
													//		MOVE.L	n_start(A6),(A5)	; Set sampledata pointer
													//		MOVE.W	n_length(A6),4(A5)	; Set length
													//		MOVE.W	#600,D0
													//	mt_rtnloop1
													//		DBRA	D0,mt_rtnloop1
													//		MOVE.W	n_dmabit(A6),D0
													//		BSET	#15,D0
													//		MOVE.W	D0,$DFF096
													//		MOVE.W	#600,D0
													//	mt_rtnloop2
													//		DBRA	D0,mt_rtnloop2
													//		MOVE.L	n_loopstart(A6),(A5)
													//		MOVE.L	n_replen(A6),4(A5)
	mt_rtnend();
}

void mt_rtnend()										//	mt_rtnend
{
	movem_pop(RD1,RD1);								//		MOVE.L	(SP)+,D1
}													//		RTS
													//	
void mt_VolumeFineUp()									//	mt_VolumeFineUp
{
													//		TST.B	mt_counter
													//		BNE	mt_Return2
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$F,D0
	mt_VolSlideUp();									//		BRA	mt_VolSlideUp
}
													//	
void mt_VolumeFineDown()							//	mt_VolumeFineDown
{
													//		TST.B	mt_counter
													//		BNE	mt_Return2
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
	mt_VolSlideDown2();								//		BRA	mt_VolSlideDown2
}
													//	
void mt_NoteCut()									//	mt_NoteCut
{
													//		MOVEQ	#0,D0
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
													//		CMP.B	mt_counter(PC),D0
													//		BNE	mt_Return2
													//		CLR.B	n_volume(A6)
													//		MOVE.W	#0,8(A5)
}													//		RTS
													//	
void mt_NoteDelay()									//	mt_NoteDelay
{
	d0 = 0;											//		MOVEQ	#0,D0
	d0 = ld_b(a6+n_cmdlo);							//		MOVE.B	n_cmdlo(A6),D0
	d0 &= 0x0F;										//		AND.B	#$0F,D0
													//		CMP.B	mt_Counter,D0
													//		BNE	mt_Return2
													//		MOVE.W	(A6),D0
													//		BEQ	mt_Return2
													//		MOVE.L	D1,-(SP)
	mt_DoRetrig();										//		BRA	mt_DoRetrig
}
													//	
void mt_PatternDelay()									//	mt_PatternDelay
{
	if (mt_counter != 0)									//		TST.B	mt_counter
	{
		mt_Return2();									//		BNE	mt_Return2
		return;
	}

	d0 = 0;											//		MOVEQ	#0,D0
	d0 = ld_b(a6+n_cmdlo);								//		MOVE.B	n_cmdlo(A6),D0
	D0.b0 &= 0x0F;									//		AND.B	#$0F,D0

	if (mt_PattDelTime2 != 0) 								//		TST.B	mt_PattDelTime2
	{
		mt_Return2();									//		BNE	mt_Return2
		return;
	}


	d0 += 1;											//		ADDQ.B	#1,D0
	mt_PattDelTime = d0;								//		MOVE.B	D0,mt_PattDelTime
}													//		RTS
													//	
void mt_FunkIt()										//	mt_FunkIt
{
													//		TST.B	mt_counter
													//		BNE	mt_Return2
													//		MOVE.B	n_cmdlo(A6),D0
													//		AND.B	#$0F,D0
													//		LSL.B	#4,D0
													//		AND.B	#$0F,n_glissfunk(A6)
													//		OR.B	D0,n_glissfunk(A6)
													//		TST.B	D0
													//		BEQ	mt_Return2
													//	mt_UpdateFunk
													//		MOVEM.L	A0/D1,-(SP)
													//		MOVEQ	#0,D0
													//		MOVE.B	n_glissfunk(A6),D0
													//		LSR.B	#4,D0
													//		BEQ.S	mt_funkend
													//		LEA	mt_FunkTable(PC),A0
													//		MOVE.B	(A0,D0.W),D0
													//		ADD.B	D0,n_funkoffset(A6)
													//		BTST	#7,n_funkoffset(A6)
													//		BEQ.S	mt_funkend
													//		CLR.B	n_funkoffset(A6)
													//	
													//		MOVE.L	n_loopstart(A6),D0
													//		MOVEQ	#0,D1
													//		MOVE.W	n_replen(A6),D1
													//		ADD.L	D1,D0
													//		ADD.L	D1,D0
													//		MOVE.L	n_wavestart(A6),A0
													//		ADDQ.L	#1,A0
													//		CMP.L	D0,A0
													//		BLO.S	mt_funkok
													//		MOVE.L	n_loopstart(A6),A0
													//	mt_funkok
													//		MOVE.L	A0,n_wavestart(A6)
													//		MOVEQ	#-1,D0
													//		SUB.B	(A0),D0
													//		MOVE.B	D0,(A0)
													//	mt_funkend
													//		MOVEM.L	(SP)+,A0/D1
}													//		RTS


uint8 mt_FunkTable[]={ 0,5,6,7,8,10,11,13,16,19,22,26,32,43,64,128 };

uint8 mt_VibratoTable[]={
	0, 24, 49, 74, 97,120,141,161,
	180,197,212,224,235,244,250,253,
	255,253,250,244,235,224,212,197,
	180,161,141,120, 97, 74, 49, 24};

uint16 mt_PeriodTable[]={
// Tuning 0, Normal
		856,808,762,720,678,640,604,570,538,508,480,453,
		428,404,381,360,339,320,302,285,269,254,240,226,
		214,202,190,180,170,160,151,143,135,127,120,113,
// Tuning 1
		850,802,757,715,674,637,601,567,535,505,477,450,
		425,401,379,357,337,318,300,284,268,253,239,225,
		213,201,189,179,169,159,150,142,134,126,119,113,
// Tuning 2
		844,796,752,709,670,632,597,563,532,502,474,447,
		422,398,376,355,335,316,298,282,266,251,237,224,
		211,199,188,177,167,158,149,141,133,125,118,112,
// Tuning 3
		838,791,746,704,665,628,592,559,528,498,470,444,
		419,395,373,352,332,314,296,280,264,249,235,222,
		209,198,187,176,166,157,148,140,132,125,118,111,
// Tuning 4
		832,785,741,699,660,623,588,555,524,495,467,441,
		416,392,370,350,330,312,294,278,262,247,233,220,
		208,196,185,175,165,156,147,139,131,124,117,110,
// Tuning 5
		826,779,736,694,655,619,584,551,520,491,463,437,
		413,390,368,347,328,309,292,276,260,245,232,219,
		206,195,184,174,164,155,146,138,130,123,116,109,
// Tuning 6
		820,774,730,689,651,614,580,547,516,487,460,434,
		410,387,365,345,325,307,290,274,258,244,230,217,
		205,193,183,172,163,154,145,137,129,122,115,109,
// Tuning 7
		814,768,725,684,646,610,575,543,513,484,457,431,
		407,384,363,342,323,305,288,272,256,242,228,216,
		204,192,181,171,161,152,144,136,128,121,114,108,
// Tuning -8
		907,856,808,762,720,678,640,604,570,538,508,480,
		453,428,404,381,360,339,320,302,285,269,254,240,
		226,214,202,190,180,170,160,151,143,135,127,120,
// Tuning -7
		900,850,802,757,715,675,636,601,567,535,505,477,
		450,425,401,379,357,337,318,300,284,268,253,238,
		225,212,200,189,179,169,159,150,142,134,126,119,
// Tuning -6
		894,844,796,752,709,670,632,597,563,532,502,474,
		447,422,398,376,355,335,316,298,282,266,251,237,
		223,211,199,188,177,167,158,149,141,133,125,118,
// Tuning -5
		887,838,791,746,704,665,628,592,559,528,498,470,
		444,419,395,373,352,332,314,296,280,264,249,235,
		222,209,198,187,176,166,157,148,140,132,125,118,
// Tuning -4
		881,832,785,741,699,660,623,588,555,524,494,467,
		441,416,392,370,350,330,312,294,278,262,247,233,
		220,208,196,185,175,165,156,147,139,131,123,117,
// Tuning -3
		875,826,779,736,694,655,619,584,551,520,491,463,
		437,413,390,368,347,328,309,292,276,260,245,232,
		219,206,195,184,174,164,155,146,138,130,123,116,
// Tuning -2
		868,820,774,730,689,651,614,580,547,516,487,460,
		434,410,387,365,345,325,307,290,274,258,244,230,
		217,205,193,183,172,163,154,145,137,129,122,115,
// Tuning -1
		862,814,768,725,684,646,610,575,543,513,484,457,
		431,407,384,363,342,323,305,288,272,256,242,228,
		216,203,192,181,171,161,152,144,136,128,121,114 };

uint32 mt_chan1temp[]={0,0,0,0,0,0x00010000,0,0,0,0,0};
uint32 mt_chan2temp[]={0,0,0,0,0,0x00020000,0,0,0,0,0};
uint32 mt_chan3temp[]={0,0,0,0,0,0x00040000,0,0,0,0,0};
uint32 mt_chan4temp[]={0,0,0,0,0,0x00080000,0,0,0,0,0};

uint32 mt_SampleStarts[] = {
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

uint8 mt_speed		= 6;
uint8 mt_counter		= 0;
uint8 mt_SongPos		= 0;
uint8 mt_PBreakPos		= 0;
uint8 mt_PosJumpFlag	= 0;
uint8 mt_PBreakFlag		= 0;
uint8 mt_LowMask		= 0;
uint8 mt_PattDelTime	= 0;
uint8 mt_PattDelTime2[]	= { 0,0 };

uint16 mt_PatternPos = 0;
uint16 mt_DMACONtemp = 0;
;/* End of File */

uint16 Ritchy_Volumes[]={
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0};

// void *GfxBase = NULL;
// const char GfxName[] = "graphics.library";


//	section chip,data

uint16 Pointeur[]={
	0x0000,0x0000,
	0xFC00,0xF800,
	0x8800,0xF000,
	0x9000,0xE000,
	0xA000,0xD800,
	0xC000,0x9C00,
	0x8000,0x0E00,
	0x0000,0x0700,
	0x0080,0x0300,
	0x0100,0x0000,
	0x0000,0x0000};


uint16 safemem[]={0,0};

//copper list


// ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789    .  ,  '  !  :  -  ?  =>

char *Texte = "                  "
	"RITCHY AND EREKOSE PRESENT TO YOU THIS LITTLE"
	" PRODUCTION FROM THE MOST"
	" WELLKNOWN GROUP POPULAR FOR HIS AMAZING, SMOKING, DRINKING,"
	" FUC...HUHO...HER...HOPLAING, EVERYTHINGS-ING GUYS !! YES, "
	" ANOTHER UNBELIEVABLE PRODUCTION IS NOW UNDER YOUR EYES FROM THIS"
	" GROUP CALLED........> PARLAFOX...HU, NO.....> BRISTOL....HU, SHIT."
	".....AH YEAH ! KANGOUROU BROTHERS !......AH, SHIT...> GOLANA !"
	".......NO.........AH THAT'S IT......A N A L O G !!    YYEEESS !"
	" I FOUND IT !!!! YYYEEEEEEEAAAAHHHHHH !! YAHOU, YIPIE !! YARGLA !"
	"..............HEY !....WHAT ?........WE ALL ALLREADY NOTICE THAT...."
	"...AH ? REALLY ??.............OK OK....."
	"WELL, AS I (EREKOSE) REALLY DONT KNOW WHAT TO SAY, LET'S WRITE SOME"
	" BULLSHIT....SO, THIS LITTLE MUSIC DISK WAS DONE IN ONE NIGHT, AT"
	" RITCHY'S HOME, JUST AFTER THE TRADIONNAL SPAGHETTI-YOGURT DINER."
	" ALL CODE WAS DONE BY...!! RITCHY !!...ALL MUSIC BY EREKOSE (ME) AND MY"
	" FRIEND MONTY>US, BUT OLD MEMBER OF ANALOG...AND AT LAST BUT NOT LEAST"
	" ALL GFX BY THE WONDERFULL...!!! BAOBAB !!!..."
	"AND NOW FOR SOMETHING COMPLETLY DIFFERENT....A COW WITH TWO NOSES..."
	"NO,NO,NO....A MEN WITH THREE EYES.......NO,NO,NO...."
	" JE TIENS ICI A SIGNALER QUE TOUTES RESSEMBLANCE AVEC DES NOMS"
	" OU DES GROUPES CONNUS NE SERAIT QUE PUREMENT VOLONTAIRE."
	" POP-QUIZ: WHO LIKE MAC DONALDS ? "
	" ANSWER: NOT ME !!! "
	"AND NOW ? WELL, I THINK I'LL JUST GIVE A LITTLE HELLO TO ALL MY OLD"
	"FRIENDS AND CONTACT I LEFT FOR SOME TIMES (I'M NOW IN THE ARMY AND"
	" ITS REALLY BORING !!!! NEVER DO THE ARMY IF YA BE ABLE TO !!)."
	" SO, BIG KISS TO ALL MEMBERS OF ANALOG, MONTY AND VANESSA, ZUUL(PC),"  
	" ALEX>MVT, BROSS>TSB, JOGEIR, KAEL, KARSTEN (INDEP), BAD CAT>IBB,"
	" , STEPHANE, RACKLER>LEGO, MR KING, ZINKFLOYD>TRSI, WHIRLY>DELIGHT,"
	" MANITOU>MOSQUITO, SHUN>LIQUID, SHED>CRYO, SANDMAN>EXILE, CORTEX, KILLER>ARKHAM,"
	" SNOOPY>DEFORM.............TO ALL OF YOU, REALLY SORRY FOR MY FUCKING"
	" BIG DELAY, BUT I REALLY DONT REACH TO BE A GOOD SWAPPER....I'M TOO"
	" LAZY FOR THAT..................GOOD BYE !................."
	"SCROLL TEXT RESTART...............5.........4.........3........1....."
	" HEY !!! WHERE IS THE 2 ????.......JUST KIDDING !.....2......1....."
	" HAVE A NICE DAY !......THANK YOU !!!!!!!!......................";

char *Texteinfo =
		"   WELCOME IN THE INFOTEXT... "
		"  J'SUIS D'ACCORD AVEC SERGE!!!   "
		"PRESS ESC TO QUIT CHEAP DISK    "
		"CLICK ON NAME OF MUSIC TO CHOOSE ANOTHER ONE....INCREDIBLE"
		" NO ?    '"
		"CLICK ON PAUSE TO..........PAUSE ?.....YEAH !'"
		"CLICK ON PLAY TO REPLAY AFTER PAUSE..."
		"CLICK ON FILTER TO TOGGLE THE FILTER ON-OFF      "
		"CLICK ON INFO TO.......OH, YOU ALREADY DID IT !!!   "
		"CREDITS FOR THIS ONE-NIGHT MUSIC-FILE:   "
		"CODE > RITCHY - FONT > PLASMA - "
		"GFX > BAOBAB - MORAL SUPPORT > WORGLUB...   "
		"THIS MUSICAL-DEMO HAS BEEN CODED IN ONE LONG NIGHT...  "
		"THANX TO THE COCA-COLA AND BEER, CHESTERFIELD AND ROYAL "
		"MENTHOL CIGARETTES, MARS ICECREAM AND CREME MONTBLANC "
		"A LA VANILLE...QU'ON A OUBLIE DE MANGER ! MAIS TANT MIEUX"
		" CA EN FERA PLUS POUR MOI!      "
		"CHRISTINE !!! Y'T'RESTE PAS QUE'QUES CAHUETTES ?  "
		"         ALLEZ CHRISTINE, QUOI !! FAIT PETER LES CAHUETTES !"
		"                    "
		"BON, ICI JE -RITCHY- PEUX TAPER QUELQUECHOSE CAR PERSONNE "
		"NE LE LIRA JAMAIS... JE VOUDRAIS DIRE QUE ANALOG EST "
		"VRAIMENT UN GROUPE SUPER ! UNE DEMO PAR AN, TOUT LE MONDE "
		"TE CRITIQUE ET DE PREFERENCE CEUX QUI NE FONT RIEN...    "
		"BISOUS A LAETI,FIFI,STEPH,...,PAT,THIERRY,DEUFRE,SPEEDMAN,"
		"DD FROM SPIDER!,7TH YEUX AUSSI,CHAOS,ACKERLIGHT,BAMIGA "
		"SECTOR ONE,THE KENT TEAM,RED SECTOR,PLASMA FORCE,UNIT A,"
		"DOC,WILD COPPER,REBELS,CBC,CASCADE,MEGAFORCE... "
		"AND N'OUBLIEZ PAS >>> ONLY AMIGA "
		"MAKES IT POSSIBLE!  BISOUS A TOI AUSSI PETIT LECTEUR DE "
		"SCROLLING,CAR ON T'EMBRASSE PAS SOUVENT ET C'EST PAS JUSTE."
		"THIS DEMO IS FREEWARE, IT MEANS THAT IS A >FREE SOFTWARE, "
		"SO IT'S FORBIDDEN DE LA VENDRE !   "
		"           ";

void init_copper2()
{
	uint32 *ptr;

	copper2 = malloc(sizeof(uint32) * 50 );

ptr = (uint32 *) copper2;

	setCop( 0x008e,0x6981 );	// 4
	setCop( 0x0090,0x85c1 );
	setCop( 0x0092,0x0068 );
	setCop( 0x0094,0x0098 );

Bpls2 = ptr;

	setCop( 0x00e0,0x0000);		// 6
	setCop( 0x00e2,0x0000);
	setCop( 0x00e4,0x0000);
	setCop( 0x00e6,0x0000);
	setCop( 0x00e8,0x0000);
	setCop( 0x00ea,0x0000);

Com = ptr;

	setCop( 0x0100,0x0000 );	// 24
	setCop( 0x0102,0x0000 );
	setCop( 0x0104,0x0033 );
	setCop( 0x01FC,0x0000 );
	setCop( 0x010c,0x11);
	setCop( 0x0106,0);
	setCop( 0x0108,0x0038 );
	setCop( 0x010a,0x0038 );
	setCop( 0x120,0 );
	setCop( 0x122,0 );
	setCop( 0x124,0 );
	setCop( 0x126,0 );
	setCop( 0x128,0 );
	setCop( 0x12a,0 );
	setCop( 0x12c,0 );
	setCop( 0x12e,0 );
	setCop( 0x130,0 );
	setCop( 0x132,0 );
	setCop( 0x134,0 );
	setCop( 0x136,0 );
	setCop( 0x138,0 );
	setCop( 0x13a,0 );
	setCop( 0x13c,0 );
	setCop( 0x13e,0 );

	setCop( 0x0106,0x0000 );	// 9
	setCop( 0x0180,0x033a );
	setCop( 0x0182,0x0565 );
	setCop( 0x0184,0x0676 );
	setCop( 0x0186,0x0797 );
	setCop( 0x0188,0x08A8 );
	setCop( 0x018A,0x0ABA );
	setCop( 0x018C,0x0BDB );
	setCop( 0x018E,0x0CEC );

	setCop(	0xce0f,0xfffe);		// 3
	setCop( 0x180,0x229 );
	setCop( 0xFFFF,0xFFFE);
}

uint16 TableEqu[] = {
		 0xF29,0x100	// Init,Dec
		,0x2F9,0x010
		,0xFF9,0x110
		,0x2B0,0x00F };

void init_copper()
{
	uint32 *ptr;

	copper = malloc(sizeof(uint32) * 110 );

	ptr = (uint32 *) copper;

	setCop( 0x008e,0x2981 );
	setCop( 0x0090,0x29c1 );

r38d0 = ptr;	

	setCop( 0x0092,0x0080 );
	setCop( 0x0094,0x00c0 );

Bpls = ptr;

	setCop( 0x00e0,0x0000);
	setCop( 0x00e2,0x0000);
	setCop( 0x00e4,0x0000);
	setCop( 0x00e6,0x0000);
	setCop( 0x00e8,0x0000);
	setCop( 0x00ea,0x0000);
	setCop( 0x00ec,0x0000);
	setCop( 0x00ee,0x0000);
	setCop( 0x00f0,0x0000);
	setCop( 0x00f2,0x0000);
	setCop( 0x0100,0x5200);
	setCop( 0x0102,0x0000);	//	 5 bitplanes
	setCop(0x0104,0x0033);
	setCop( 0x01FC,0x0000);
	setCop( 0x010c,0x0011);
	setCop( 0x0106,0x0000);

Mod = ptr;

	setCop( 0x0108,0x00aa );
	setCop( 0x010a,0x00aa );

Sprt = ptr;

	setCop(	0x120,0 );
	setCop( 0x122,0 );
	setCop(	0x124,0 );
	setCop(	0x126,0 );
	setCop(	0x128,0 );
	setCop(	0x12a,0 );
	setCop(	0x12c,0 );
	setCop(	0x12e,0 );
	setCop(	0x130,0 );
	setCop(	0x132,0 );
	setCop(	0x134,0 );
	setCop(	0x136,0 );
	setCop(	0x138,0 );
	setCop(	0x13a,0 );
	setCop(	0x13c,0 );
	setCop(	0x13e,0 );
	setCop(	0x180,0x033a );
	setCop( 0x182,0x0fff );

coul = ptr;

	setCop(	0x184,0xeef );
	setCop(	0x186,0x0ccf );
	setCop(	0x188,0x0bbf );
	setCop(	0x18a,0x09af );
	setCop(	0x18c,0x088f );
	setCop(	0x18e,0x067f );
	setCop(	0x190,0x055d );
	setCop(	0x192,0x044c );
	setCop(	0x194,0x033a );
	setCop(	0x196,0x0229 );
	setCop(	0x198,0x0117 );
	setCop(	0x19a,0x0116 );
	setCop(	0x19c,0x0004 );
	setCop(	0x19e,0x0003 );
	setCop(	0x1a0,0x0ffc );
	setCop(	0x1a2,0x0eea );
	setCop(	0x1a4,0x0dd9 );
	setCop(	0x1a6,0x0cb8 );
	setCop(	0x1a8,0x0ba7 );
	setCop(	0x1aa,0x0b96 );
	setCop(	0x1ac,0x0a85 );
	setCop(	0x1ae,0x0964 );
	setCop(	0x1b0,0x0853 );
	setCop(	0x1b2,0x0742 );
	setCop(	0x1b4,0x0632 );
	setCop(	0x1b6,0x0521 );
	setCop(	0x1b8,0x0511 );
	setCop(	0x1ba,0x0410 );
	setCop(	0x1bc,0x0300 );
	setCop(	0x1be,0x0200 );


//; Origine EEF
//; 55D,358,55d

coul0 = ptr;
	setCop( 0x500f,0xfffe );
	setCop( 0x184,0xeef );

coul1 = ptr; 
	setCop( 0x930f,0xfffe );
	setCop( 0x184,0xeef );

coul2 = ptr; 
	setCop( 0x9c0f,0xfffe );
	setCop( 0x184,0xeef );

coul3 = ptr; 
	setCop( 0xa50f,0xfffe );
	setCop( 0x184,0xeef );

coul4 = ptr; 
	setCop( 0xae0f,0xfffe );
	setCop( 0x184,0xeef );

coul5 = ptr; 
	setCop( 0xb70f,0xfffe );
	setCop( 0x184,0xeef );

coul6 = ptr; 
	setCop( 0xc00f,0xfffe );
	setCop( 0x184,0xeef );

coul7 = ptr;
	setCop( 0xc90f,0xfffe );
	setCop( 0x184,0xeef );
	setCop( 0xce0f,0xfffe );
	setCop( 0x180,0x229 );

coul8 = ptr; 
	setCop( 0xd20f,0xfffe ); 
	setCop( 0x184,0xeef );

coul9 = ptr; 
	setCop( 0xdb0f,0xfffe ); 
	setCop( 0x184,0xeef );

coul10 = ptr; 

	setCop( 0xe40f,0xfffe ); 
	setCop( 0x184,0xeef );

	setCop(	0xef0f,0xfffe );

Eq1 = ptr;

	setCop( 0x184,0xeef );
	setCop( 0x1B8,0x511 );
	setCop( 0x1BA,0x410 );
	setCop( 0x1BC,0x300 );

	setCop(	0xffff,0xffde );
	setCop(	0x100f,0xfffe );
	setCop( 0x1b8,0x511 );

	setCop( 0xFFFF,0xFFFE );
}

void load_bin()
{
	mm1 = "media/mod.status";											//	mm1:	incbin	'mod.status'
	mm2 = "media/mod.adel";											//	mm2:	incbin	'mod.adel'
	mm3 ="media/mod.compote";											//	mm3:	incbin	'mod.compote'
	mm4 ="media/mod.banzai";											//	mm4:	incbin	'mod.banzai'
	mm5 ="media/mod.milk";												//	mm5:	incbin	'mod.milk'
	mm6 ="media/mod.jambon";											//	mm6:	incbin	'mod.jambon'
	mm7 ="media/mod.atom";											//	mm7:	incbin	'mod.atom'
	mm8 ="media/mod.computer";											//	mm8:	incbin	'mod.computer'
	mm9 ="media/mod.track";											//	mm9:	incbin	'mod.track'
};

#define safe_unload_module( varname ) if (varname) { PTUnloadModule( varname ); varname = NULL; }



void init_mt_data()
{
	if (!mm1)
	{
		printf("mm1 is NULL\n");
	}

	mt_data=mm1;
}

void *load_raw(const char *name, int extraSize,  char **ptrE)
{
	void *ptr;
	BPTR fd;
	fd = FOpen( name, MODE_OLDFILE, 0  );
	uint64 size;

	ptr = NULL;
	*ptrE = NULL;

	if (fd)
	{
		ChangeFilePosition( fd , 0 , OFFSET_END);
		size = GetFilePosition( fd );
		ChangeFilePosition( fd , 0 , OFFSET_BEGINNING);

		ptr = malloc( size + extraSize );
		if (ptr)
		{
			memset( ptr, 0, size + extraSize );

			FRead( fd , ptr, size ,1 );
			*ptrE = ptr + size + extraSize;
		}
		FClose(fd);
	}

	if ( *ptrE == NULL )
	{
		printf("failed to load %s\n",name);
		if (ptr) free(ptr);
		ptr = NULL;
	}

	return ptr;
}

void load_bin2()
{
	int screenSize = 42*5*12;

	Present = load_raw("media/RB.Present",0,&PresentE);								//	Present:	incbin	'RB.Present'
	Font = load_raw("media/RB.Fonte8",18*5*256, &FontE);								//	Font:	incbin	'RB.Fonte8'
	PicN = FontE - 18*5*256;														//	PicN:	blk.b	18*5*256,0
	PicNE = FontE;
	FontE -= 18*5*256;

	Nounou = load_raw("media/RB.Nounours",18*5*187, &NounouE);						//	Nounou:	incbin	'RB.Nounours'
																		//	blk.b	18*5*187,0
	Pic = load_raw("media/RB.Fond",	screenSize * 2 , &PicE );							//	Pic:	incbin	'RB.Fond'

	Scr1 = PicE - screenSize * 2;												//	Scr1:	blk.b	42*5*12,0
	Scr1E = PicE - screenSize;
	Buffer = PicE - screenSize;												//	Buffer:	blk.b	42*5*12,0
	BufferE = PicE;
	Scr1E -= (screenSize * 2);
}

void unload_bin2()
{
	if (Present) free(Present);
	if (Font) free(Font);
	if (Nounou) free(Nounou);
	if (Pic) free(Pic);

	Present = NULL;
	Font = NULL;
	Nounou = NULL;
	Pic = NULL;
}

void cleanup()
{
	unload_bin2();
	unload_bin();

	if (win) CloseWindow(win);
	win = NULL;

	if (copperBitmap) FreeBitMap(copperBitmap);
	copperBitmap = NULL;

	close_libs();
}

#define inner_win_w (640 + 64)
#define inner_win_h (512 + 64)

bool init_sys()
{

	win = OpenWindowTags( NULL, 
		WA_IDCMP,IDCMP_MOUSEBUTTONS,
		WA_Left,320,
		WA_Top,20,
		WA_InnerWidth, inner_win_w,
		WA_InnerHeight, inner_win_h,

		WA_MinWidth,	100,
	 	WA_MinHeight,	100,	
		WA_MaxWidth,	~0,
	 	WA_MaxHeight, ~0,	

		WA_DragBar, true,
		WA_DepthGadget, true,
		WA_SizeGadget, TRUE,
		WA_SizeBBottom, TRUE,
		WA_CloseGadget, FALSE,
		WA_Title, (ULONG) demo_name,
		TAG_END);

	if (!win)
	{
		printf("window failed to open\n");
		return false;
	}

	ActivateWindow(win);

	copperBitmap =AllocBitMap( win -> Width, win -> Height, 32, BMF_DISPLAYABLE, win ->RPort -> BitMap);

	if (copperBitmap) 
	{
		struct RastPort rp;
		InitRastPort(&rp);
		rp.BitMap = copperBitmap;
		RectFillColor(&rp, 0, 0, win -> Width, win -> Height, 0xFF666666);

		return true;
	}

	return false;
}

void show_info()
{
	printf("copper list1: %08x\n",copper);
	printf("copper list2: %08x\n",copper2);

	printf("Present: %08x - %08x\n",Present,PresentE);
	printf("Font: %08x - %08x\n",Font,FontE);
	printf("Nounou: %08x - %08x\n",Nounou, NounouE);
	printf("Pic: %08x - %08x\n",Pic,PicE);
}

int main()
{
	if (open_libs() == false)
	{
		close_libs();
		return;
	}

	// setup fake stack pointer.. :-)
	emu_stack_ptr = emu_stack;

	if (init_sys())
	{
		init_ecs2colors();		// don't forget to make the lookup table.

		load_bin();
		load_bin2();

		init_tmm();
		init_copper2();
		init_copper();
		init_Tcoul();
		init_mt_data();
		init_PTtexte();

		show_info();
	
		code();

		mt_end();
	}

	cleanup();

	return 0;
}

void WAITBLIT()
{
}

bool CheckMouse()
{
}

struct valid_range
{
	uint8 *s;
	uint8 *e;
	const char *name;
};



const char *validAddress(uint8 *ptr)
{
	struct valid_range *vrange;

	struct valid_range valid_ranges[]=
		{
			{Font,FontE,"Font"},
			{Pic,PicE,"Pic"},
			{PicN,PicNE,"PicN"},
			{Scr1,Scr1E,"Screen"},
			{Buffer,BufferE,"Buffer"},
			{NULL,NULL,NULL}
		};

	for (vrange=valid_ranges;vrange->s;vrange++)
	{
		if ((ptr>=vrange -> s) && (ptr < vrange -> e))
		{
			return vrange -> name;
		}
	}

	return NULL;
}

void _doBlitter()
{
	const char *destName;
	uint16 *dptr = (uint16 *) custom -> bltdpt;

	destName = validAddress( (uint8 *) dptr);
	if ( destName )
	{
		printf("bltdpt %s\n",destName);
	}
	else
	{
		printf("not a valied address: %08x\n", dptr);
		return;
	}

	doBlitter( custom );
}

static struct Hook hook;
static struct Rectangle rect;
static CompositeHookData hookData;

struct Hook BackFill_Hook =
{
	{NULL, NULL},
	(HOOKFUNC) &BackFill_Func,
	NULL,
	NULL
};

void set_target_hookData( struct BitMap *bitmap, struct Window *win );

void BackFill_Func(struct RastPort *ArgRP, struct BackFillArgs *MyArgs)
{
	if (win)
	{
		set_target_hookData(copperBitmap,win);

		register struct RastPort *RPort = win->RPort;

		LockLayer(0, win -> RPort->Layer);
		DoHookClipRects(&hook, win -> RPort, &rect);
		UnlockLayer( win -> RPort->Layer);
	}
}

static ULONG _compositeHookFunc(
			struct Hook *hook, 
			struct RastPort *rastPort, 
			struct BackFillMessage *msg)
 {

	CompositeHookData *hookData = (CompositeHookData*)hook->h_Data;

	hookData->retCode = CompositeTags(
		COMPOSITE_Src, 
			hookData->srcBitMap, 
			rastPort->BitMap,
		COMPTAG_SrcWidth,   hookData->srcWidth,
		COMPTAG_SrcHeight,  hookData->srcHeight,
		COMPTAG_ScaleX, 	hookData->scaleX,
		COMPTAG_ScaleY, 	hookData->scaleY,
		COMPTAG_OffsetX,    msg->Bounds.MinX - (msg->OffsetX - hookData->offsetX),
		COMPTAG_OffsetY,    msg->Bounds.MinY - (msg->OffsetY - hookData->offsetY),
		COMPTAG_DestX,      msg->Bounds.MinX,
		COMPTAG_DestY,      msg->Bounds.MinY,
		COMPTAG_DestWidth,  msg->Bounds.MaxX - msg->Bounds.MinX + 1,
		COMPTAG_DestHeight, msg->Bounds.MaxY - msg->Bounds.MinY + 1,
		COMPTAG_Flags,      COMPFLAG_SrcFilter | COMPFLAG_IgnoreDestAlpha | COMPFLAG_HardwareOnly,
		TAG_END);

	return 0;
}


void set_target_hookData( struct BitMap *bitmap, struct Window *win )
{
 	rect.MinX = win->BorderLeft;
 	rect.MinY = win->BorderTop;
 	rect.MaxX = win->Width - win->BorderRight - 1;
 	rect.MaxY = win->Height - win->BorderBottom - 1;

 	float destWidth = rect.MaxX - rect.MinX + 1;
 	float destHeight = rect.MaxY - rect.MinY + 1;
 	float scaleX = (destWidth + 0.5f) / inner_win_w;
 	float scaleY = (destHeight + 0.5f) / inner_win_h;

	hookData.srcBitMap = copperBitmap;
	hookData.srcWidth = inner_win_w;
	hookData.srcHeight = inner_win_h;
	hookData.offsetX = win->BorderLeft;
	hookData.offsetY = win->BorderTop;
	hookData.scaleX = COMP_FLOAT_TO_FIX(scaleX);
	hookData.scaleY = COMP_FLOAT_TO_FIX(scaleY);
	hookData.retCode = COMPERR_Success;

	hook.h_Entry = (HOOKFUNC) _compositeHookFunc;
	hook.h_Data = &hookData;

}


//finintro:
