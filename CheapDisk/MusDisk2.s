����  2O  y?  �  �  ����2���2���2���2�	t�/;	Copper2: 5 bitplans, ca marche tres bien sur 1200
;	mais sur 500, le 5e bitplan ne s'affiche pas correctement ???
;	Raison inconnue ???
;	Assemble avec ASM-One V1.20 By T.F.A.


	section fast,code

r:	movem.l	d0-d7/a0-a6,-(a7)

	clr.l	0

;	lea	$dff000,a5
;	lea	safemem(pc),a0
;	move	$1c(a5),(a0)
;	or	#$c000,(a0)+
;	move	2(a5),(a0)
;	or	#$8100,(a0)

;	move	#$7fff,d0
;	move	d0,$96(a5)
;	move	d0,$9a(a5)
;	move	#$83E0,$96(a5)

	move.l	#PicN,d0
	lea	Bpls+6,a0
	moveq	#5-1,d1
CopyBpls:
	move	d0,(a0)
	swap	d0
	move	d0,-4(a0)
	swap	d0
	add.l	#18,d0
	addq.l	#8,a0
	dbf	d1,CopyBpls

	move	#18*4,Mod+2
	move	#18*4,Mod+6

	move.l	4.w,a6
	jsr	-132(a6)
	bsr	mt_init
	move.l	$6c.w,saveirq3+2
	move.l	#irq3,$6c.w


	lea	Bpls2+6,a0
	move.l	#Present,d0
	moveq	#3-1,d1
CopPres:move	d0,(a0)
	swap	d0
	move	d0,-4(a0)
	swap	d0
	add.l	#28,d0
	addq.l	#8,a0
	dbf	d1,CopPres


	lea	$dff000,a5
	lea	copper2,a0
	move.l	a0,$80(a5)
	clr	$88(a5)

	move	#50*2,d0
	bsr	PauseCafe2
	move	#$b200,Com+2

	moveq	#2-1,d7
President:
	move	#62,d0
	bsr	PauseCafe2
	add.b	#28,Copper2+6
	dbf	d7,President

	move	#62,d0
	bsr	PauseCafe2

	lea	$dff000,a5
	lea	copper,a0
	move.l	a0,$80(a5)
	clr	$88(a5)

	move	#100,d0
	bsr	PauseCafe2

	move	#431,d7

M2:	cmp.b	#80,$dff006
	bne	M2
M3:	cmp.b	#80,$dff006
	beq	M3
	moveq	#5-1,d0
	lea	Bpls+6,a0
Monte:	move	-4(a0),d1
	swap	d1
	move	(a0),d1
	add.l	#18*5,d1
	move	d1,(a0)
	swap	d1
	move	d1,-4(a0)
	addq.l	#8,a0
	dbf	d0,Monte
	subq	#1,d7
	bpl	M2

	move.b	#-1,Auto

	move	#50,d0
	bsr	PauseCafe2

	bsr	EffCoul
	move	#$358,Coul1+6

	move.l	#Pointeur,d0
	lea	Sprt+6,a0
	move	d0,(a0)
	swap	d0
	move	d0,-4(a0)

	lea	Nounou+[18*5*176],a0
	lea	Pic+18,a1
	move	#[80*5]-1,d0
CopyPano:
	moveq	#9-1,d1
CopyPan:move	(a0)+,(a1)+
	dbf	d1,CopyPan
	lea	24(a1),a1
	dbf	d0,CopyPano

	move	#$aa,Mod+2
	move	#$aa,Mod+6
	move	#$38,R38d0+2
	move	#$d0,R38d0+6

	move.l	#Pic,d0
	lea	Bpls+6,a0
	moveq	#5-1,d1
CopyBpls2:
	move	d0,(a0)
	swap	d0
	move	d0,-4(a0)
	swap	d0
	add.l	#42,d0
	addq.l	#8,a0
	dbf	d1,CopyBpls2

	move	#$F90,Coul10+6

	bsr	SaveFond

souris:	cmp.b	#200,$dff006
	bne.b	Souris
s2:	cmp.b	#200,$dff006
	beq.b	s2

	btst	#10,$dff016
	beq.b	Souris

;	move	#$F00,$dff180
	bsr.w	GereScroll
;	move	#$0F0,$dff180
wBbb:	btst	#14,$dff002
	bne	Wbbb
	bsr	RestoreFond
;	move	#$00,$dff180

	bsr	GereEqual

	btst	#6,$bfe001
	bne	PasSouris

	moveq	#0,d1
	move	MouseY,d1
	cmp	#$92+81,d1
	bhs	PasSong

	bsr	EffCoul
	moveq	#0,d1
	move	MouseY,d1
	sub	#$92,d1
	divu	#9,d1
	ext.l	d1		; 0-9
	lsl.l	#2,d1		; *4
	lea	Tcoul,a0
	move.l	(a0,d1),a0
	move	#$358,6(a0)

	moveq	#0,d0
	move	MouseY,d0
	sub	#$92,d0
	divu	#9,d0
	ext.l	d0
	lsl.l	#2,d0
	lea	tmm,a0
	move.l	(a0,d0),d0
	cmp.l	MT_DATA,d0
	beq	PasSouris

	move	#-1,Permit
	bsr	MT_END
	move.l	d0,MT_DATA
	bsr	MT_INIT
	clr	Permit

	bra	PasSouris

PasSong:cmp	#$e6,d1
	blo	PasSouris

	moveq	#0,d0
	move	MouseX,d0
	sub	#$127,d0
	lsr	#5,d0		; 0 - 4
	beq	Play
	cmp	#1,d0
	beq	Pause
	cmp	#2,d0
	beq	Filter

Info:	move.l	#TexteInfo,PTtexte
	bra	PasSouris
Play:	clr	Permit
	bra	PasSouris
Pause:	move	#-1,Permit
	move	#$F,$dff096
	bra	PasSouris
Filter:	cmp.l	#20,Wait
	blo	PasSouris
	eor.b	#2,$bfe001
	clr.l	Wait
PasSouris:

	move.b	$bfec01,d0
	cmp.b	#$75,d0
	bne	souris

fin:	lea	Ritchy_Volumes(pc),a0
	move	(a0)+,d0		; Volumes 4 voies
	move	(a0)+,d1
	move	(a0)+,d2
	move	(a0)+,d3


	lea	$dff0a8,a0
	moveq	#0,d5
Decrease:
	move	d0,(a0)
	move	d1,$10(a0)
	move	d2,$20(a0)
	move	d3,$30(a0)
	cmp.b	#255,$dff006
	bne	Decrease

Dec2:	move	d0,(a0)
	move	d1,$10(a0)
	move	d2,$20(a0)
	move	d3,$30(a0)
	cmp.b	#255,$dff006
	beq	Dec2

	tst	d5
	bne	NONO1

	tst	d0		; Decremente volumes
	beq	Vol0
	subq	#1,d0
Vol0:	tst	d1
	beq	Vol1
	subq	#1,d1
Vol1:	tst	d2
	beq	Vol2
	subq	#1,d2
Vol2:	tst	d3
	beq	Vol3
	subq	#1,d3


Vol3:	movem.l	d0-d5/a0,-(sp)
	bsr	FonduQuitte
	movem.l	(sp)+,d0-d5/a0

NONO1:	not	d5

	move	d0,d4
	add	d1,d4
	add	d2,d4
	add	d3,d4
	bne	Decrease	; Jusqu'a tous a 0 !!!

	move.l	saveirq3+2,$6c.w
	bsr	MT_end

	move.l	4.w,a6
	jsr	-138(a6)

	lea	$dff000,a5

;	move	#$7fff,d0
;	move	d0,$9a(a5)
;	move	d0,$96(a5)
;	lea	safemem(pc),a0
;	move	(a0)+,$9a(a5)
;	move	(a0),$96(a5)

	move.l	4.w,a6
	lea	GfxName(pc),a1
	jsr	-408(a6)
	move.l	d0,a0
	move.l	d0,GfxBase
	move.l	38(a0),$80(a5)
	clr	$88(a5)

	movem.l	(a7)+,d0-d7/a0-a6
	moveq	#0,d0
	rts

FonduQuitte:
	lea	Copper,a0

Fondu:	move.l	(a0)+,d0
	cmp.l	#-2,d0
	beq	FinPass

	swap	d0
	cmp	#$0180,d0
	blo	PasCoul
	cmp	#$01BE,d0
	bhi	PasCoul

	swap	d0
	move	d0,d1
	and	#$00F,d1
	beq	PasBleu
	sub	#$001,d0
PasBleu:move	d0,d1
	and	#$0F0,d1
	beq	PasVert
	sub	#$010,d0
PasVert:move	d0,d1
	and	#$F00,d1
	beq	PasRouge
	sub	#$100,d0
PasRouge:
	move	d0,-2(a0)	; Sauve coul
PasCoul:
	bra	Fondu
FinPass:rts


GereScroll:
	sub	#1,Compteur
	bpl.b	NoNewLetter

	move.l	PTtexte,a0
	move.b	(a0)+,d0
	bne.b	PasFinDuTexte
	lea	Texte,a0
	move.b	(a0)+,d0
PasFinDuTexte:
	move.l	a0,PTtexte

	bsr.b	CopyFont
	move	#7,Compteur
NoNewLetter:

	lea	Scr1-(15*42*5)+2+14,a0
	lea	Scr1-(15*42*5)+0+14,a1
	lea	$dff000,a5
WaitBlit:
	btst	#14,2(a5)
	bne.b	WaitBlit
	move.l	a0,$50(a5)
	move.l	a1,$54(a5)
	move.l	#$000E000E,$64(a5)
	move.l	#$f9f00000,$40(a5)
	move.l	#-1,$44(a5)
	move	#[12*5*64]+[224/16],$58(a5)	; Gozy
	rts

CopyFont:	; d0="caractere"
	cmp.b	#' ',d0
	beq.b	Espace
	cmp.b	#'A',d0
	blo.b	PasLettre
	cmp.b	#'Z',d0
	bhi.b	PasLettre

	sub.b	#'A',d0
	and.l	#$FF,d0
	bra.w	Fontu
PasLettre:
	cmp.b	#'0',d0
	blo.b	Espace
	cmp.b	#'9',d0
	bhi.b	Espace
	sub.b	#'0',d0		; Chiffre 0-9
	and.l	#$F,d0
	add.l	#26,d0
	bra.w	Fontu

Espace:	cmp.b	#'.',d0
	bne	PasPoint
	move.l	#36,d0
	bra.w	Fontu
PasPoint:
	cmp.b	#',',d0
	bne	PasVirg
	move.l	#37,d0
	bra.b	Fontu
PasVirg:
	cmp.b	#"'",d0
	bne	PasApos
	move.l	#38,d0
	bra.b	Fontu
PasApos:
	cmp.b	#"!",d0
	bne	PasExc
	move.l	#39,d0
	bra.b	Fontu
PasExc:	cmp.b	#":",d0
	bne	Pas2Pts
	move.l	#40,d0
	bra.b	Fontu
Pas2Pts:cmp.b	#"-",d0
	bne	PasTiret
	move.l	#41,d0
	bra.b	Fontu
PasTiret:
	cmp.b	#"?",d0
	bne	PasInter
	move.l	#42,d0
	bra.b	Fontu
PasInter:
	cmp.b	#">",d0
	bne	Esp
	move.l	#43,d0
	bra.b	Fontu

Esp:	lea	Scr1-(15*42*5)+40,a0
	moveq	#0,d0
	move	#[12*5]-1,d1
CopEsp:	move.b	d0,(a0)
	lea	42(a0),a0
	dbf	d1,CopEsp
	rts

Fontu:	lea	Font,a0
	lea	Scr1-(15*42*5)+40,a1
	add.l	d0,a0		; a0 sur bonne fonte
	move	#[12*5]-1,d1
CopyF:	move.b	(a0),(a1)
	lea	44(a0),a0
	lea	42(a1),a1
	dbf	d1,CopyF
	rts


EffCoul:lea	Tcoul,a0
	moveq	#9-1,d0
DelCoul:move.l	(a0)+,a1
	move	#$55d,6(a1)
	dbf	d0,DelCoul
	rts

;====================================================================
SaveFond:
	lea	Scr1-(15*42*5)+14,a0
	lea	Buffer,a1
	moveq	#12-1,d0
SF:	move	(a0),(a1)+
	move	42(a0),(a1)+
	move	42*2(a0),(a1)+
	move	42*3(a0),(a1)+
	move	42*4(a0),(a1)+
	lea	42*5(a0),a0
	dbf	d0,SF
	rts
;====================================================================
RestoreFond:
	lea	Buffer,a0
	lea	Scr1-(15*42*5)+14,a1
	moveq	#12-1,d0
RF:	move	(a0)+,(a1)
	move	(a0)+,42(a1)
	move	(a0)+,42*2(a1)
	move	(a0)+,42*3(a1)
	move	(a0)+,42*4(a1)
	lea	42*5(a1),a1
	dbf	d0,RF
	rts
;====================================================================
PauseCafe2:	; d0=Nb secondes
	cmp.b	#200,$dff006
	bne	PauseCafe2
pc22:	cmp.b	#200,$dff006
	beq	pc22
	dbf	d0,PauseCafe2
	rts
;====================================================================
GereEqual:
	lea	TableEqu+2,a0
	lea	Eq1+2,a1
	moveq	#4-1,d0
CopyEqu:cmp	#$229,(a1)
	beq	PasDec
	move	(a0),d1
	sub	d1,(a1)
PasDec:	addq.l	#4,a0
	addq.l	#4,a1
	dbf	d0,CopyEqu
	rts
;====================================================================
irq3:	movem.l	d0-d7/a0-a6,-(a7)
	move	$dff01e,d0
	and	$dff01c,d0
	btst	#5,d0
	beq.w	NoPlay

	addq.l	#1,Wait
	bsr	mots_control

	tst	Permit
	bne	NoPlay

	bsr	mt_music

NoPlay:	movem.l	(a7)+,d0-d7/a0-a6
saveirq3:
	jmp	$0
;====================================================================
mots_control:
	lea	Pointeur,a3
	move.b	$dff00a,d0
	move.b	d0,d2

	move.b	lasty,d1
	sub.b	d1,d0
	ext	d0
	add.w	d0,mousey
	move.b	d2,lasty

	move.b	$dff00b,d0
	move.b	d0,d2
	move.b	lastx,d1
	sub.b	d1,d0
	ext	d0
	add.w	d0,mousex
	move.b	d2,lastx
	cmp.w	#$1a6,mousex
	bls	paslimite
	move.w	#$1a6,mousex
paslimite:
	cmp.w	#$127,mousex
	bhs	paslimite2
	move.w	#$127,mousex
paslimite2:
	cmp.w	#$92,mousey
	bge	paslimite3
	move.w	#$92,mousey
paslimite3:
	cmp.w	#$ee,mousey
	bls	paslimite4
	move.w	#$ee,mousey
paslimite4:
	move.b	mousey+1,(a3)	;Adrsp0
	move.b	mousey,d0
	lsl.b	#2,d0
	move.w	mousex,d1
	ror.w	#1,d1
	bpl.w	pasneg
	or.w	#1,d0
pasneg:
	move.b	d1,1(a3)	;Adrsp0+1
	move.w	mousey,d1
	add.w	#9,d1
	move.b	d1,2(a3)	;Adrsp0+2
	lsr.w	#8,d1
	lsl.w	#1,d1
	or.b	d1,d0
	move.b	d0,3(a3)	;Adrsp0+3
	rts

;********************************************
;* ----- Protracker V1.1B Playroutine ----- *
;* Lars "Zap" Hamre/Amiga Freelancers 1991  *
;* Bekkeliveien 10, N-2010 STR�MMEN, Norway *
;********************************************

; VBlank Version 2:
; Call mt_init to initialize the routine, then call mt_music on
; each vertical blank (50 Hz). To end the song and turn off all
; voices, call mt_end.

; This playroutine is not very fast, optimized or well commented,
; but all the new commands in PT1.1A should work.
; If it's not good enough, you'll have to change it yourself.
; We'll try to write a faster routine soon...

; Changes from V1.0C playroutine:
; - Vibrato depth changed to be compatible with Noisetracker 2.0.
;   You'll have to double all vib. depths on old PT modules.
; - Funk Repeat changed to Invert Loop.
; - Period set back earlier when stopping an effect.

;DMAWait = 300 ; Set this as low as possible without losing low notes.

n_note		EQU	0  ; W
n_cmd		EQU	2  ; W
n_cmdlo		EQU	3  ; B
n_start		EQU	4  ; L
n_length	EQU	8  ; W
n_loopstart	EQU	10 ; L
n_replen	EQU	14 ; W
n_period	EQU	16 ; W
n_finetune	EQU	18 ; B
n_volume	EQU	19 ; B
n_dmabit	EQU	20 ; W
n_toneportdirec	EQU	22 ; B
n_toneportspeed	EQU	23 ; B
n_wantedperiod	EQU	24 ; W
n_vibratocmd	EQU	26 ; B
n_vibratopos	EQU	27 ; B
n_tremolocmd	EQU	28 ; B
n_tremolopos	EQU	29 ; B
n_wavecontrol	EQU	30 ; B
n_glissfunk	EQU	31 ; B
n_sampleoffset	EQU	32 ; B
n_pattpos	EQU	33 ; B
n_loopcount	EQU	34 ; B
n_funkoffset	EQU	35 ; B
n_wavestart	EQU	36 ; L
n_reallength	EQU	40 ; W

mt_init	move.l	mt_data,A0
	MOVE.L	A0,mt_SongDataPtr
	MOVE.L	A0,A1
	LEA	952(A1),A1
	MOVEQ	#127,D0
	MOVEQ	#0,D1
mtloop	MOVE.L	D1,D2
	SUBQ.W	#1,D0
mtloop2	MOVE.B	(A1)+,D1
	CMP.B	D2,D1
	BGT.S	mtloop
	DBRA	D0,mtloop2
	ADDQ.B	#1,D2
			
	LEA	mt_SampleStarts(PC),A1
	ASL.L	#8,D2
	ASL.L	#2,D2
	ADD.L	#1084,D2
	ADD.L	A0,D2
	MOVE.L	D2,A2
	MOVEQ	#30,D0
mtloop3	CLR.L	(A2)
	MOVE.L	A2,(A1)+
	MOVEQ	#0,D1
	MOVE.W	42(A0),D1
	ASL.L	#1,D1
	ADD.L	D1,A2
	ADD.L	#30,A0
	DBRA	D0,mtloop3

	OR.B	#2,$BFE001
	MOVE.B	#6,mt_speed
	CLR.B	mt_counter
	CLR.B	mt_SongPos
	CLR.W	mt_PatternPos
mt_end	CLR.W	$DFF0A8
	CLR.W	$DFF0B8
	CLR.W	$DFF0C8
	CLR.W	$DFF0D8
	MOVE.W	#$F,$DFF096
	RTS

mt_music
	MOVEM.L	D0-D4/A0-A6,-(SP)
	ADDQ.B	#1,mt_counter
	MOVE.B	mt_counter(PC),D0
	CMP.B	mt_speed(PC),D0
	BLO.S	mt_NoNewNote
	CLR.B	mt_counter
	TST.B	mt_PattDelTime2
	BEQ.S	mt_GetNewNote
	BSR.S	mt_NoNewAllChannels
	BRA	mt_dskip

mt_NoNewNote
	BSR.S	mt_NoNewAllChannels
	BRA	mt_NoNewPosYet

mt_NoNewAllChannels
	LEA	$DFF0A0,A5
	LEA	mt_chan1temp(PC),A6
	BSR	mt_CheckEfx
	LEA	$DFF0B0,A5
	LEA	mt_chan2temp(PC),A6
	BSR	mt_CheckEfx
	LEA	$DFF0C0,A5
	LEA	mt_chan3temp(PC),A6
	BSR	mt_CheckEfx
	LEA	$DFF0D0,A5
	LEA	mt_chan4temp(PC),A6
	BRA	mt_CheckEfx

mt_GetNewNote
	MOVE.L	mt_SongDataPtr(PC),A0
	LEA	12(A0),A3
	LEA	952(A0),A2	;pattpo
	LEA	1084(A0),A0	;patterndata
	MOVEQ	#0,D0
	MOVEQ	#0,D1
	MOVE.B	mt_SongPos(PC),D0
	MOVE.B	(A2,D0.W),D1
	ASL.L	#8,D1
	ASL.L	#2,D1
	ADD.W	mt_PatternPos(PC),D1
	CLR.W	mt_DMACONtemp

	LEA	$DFF0A0,A5
	LEA	mt_chan1temp(PC),A6
	BSR.S	mt_PlayVoice
	LEA	$DFF0B0,A5
	LEA	mt_chan2temp(PC),A6
	BSR.S	mt_PlayVoice
	LEA	$DFF0C0,A5
	LEA	mt_chan3temp(PC),A6
	BSR.S	mt_PlayVoice
	LEA	$DFF0D0,A5
	LEA	mt_chan4temp(PC),A6
	BSR.S	mt_PlayVoice
	BRA	mt_SetDMA

mt_PlayVoice
	TST.L	(A6)
	BNE.S	mt_plvskip
	BSR	mt_PerNop
mt_plvskip
	MOVE.L	(A0,D1.L),(A6)
	ADDQ.L	#4,D1
	MOVEQ	#0,D2
	MOVE.B	n_cmd(A6),D2
	AND.B	#$F0,D2
	LSR.B	#4,D2
	MOVE.B	(A6),D0
	AND.B	#$F0,D0
	OR.B	D0,D2
	TST.B	D2
	BEQ	mt_SetRegs
	MOVEQ	#0,D3
	LEA	mt_SampleStarts(PC),A1
	MOVE	D2,D4
	SUBQ.L	#1,D2
	ASL.L	#2,D2
	MULU	#30,D4
	MOVE.L	(A1,D2.L),n_start(A6)

	tst.b	AUTO
	beq	NOAUTO

	movem.l	d0-d1/a5,-(sp)	; EQUALIZER
	move.l	a5,d0
	sub.l	#$DFF0A0,d0
	lsr	#2,d0		; 0-4-8-16
	lea	TableEqu,a5
	move	(a5,d0.w),d1	; InitCol
	lea	Eq1+2,a5
	move	d1,(a5,d0.w)
	movem.l	(sp)+,d0-d1/a5

NOAUTO:

	MOVE.W	(A3,D4.L),n_length(A6)
	MOVE.W	(A3,D4.L),n_reallength(A6)
	MOVE.B	2(A3,D4.L),n_finetune(A6)
	MOVE.B	3(A3,D4.L),n_volume(A6)
	MOVE.W	4(A3,D4.L),D3 ; Get repeat
	TST.W	D3
	BEQ.S	mt_NoLoop
	MOVE.L	n_start(A6),D2	; Get start
	ASL.W	#1,D3
	ADD.L	D3,D2		; Add repeat
	MOVE.L	D2,n_loopstart(A6)
	MOVE.L	D2,n_wavestart(A6)
	MOVE.W	4(A3,D4.L),D0	; Get repeat
	ADD.W	6(A3,D4.L),D0	; Add replen
	MOVE.W	D0,n_length(A6)
	MOVE.W	6(A3,D4.L),n_replen(A6)	; Save replen
	MOVEQ	#0,D0
	MOVE.B	n_volume(A6),D0
	MOVE.W	D0,8(A5)	; Set volume

	movem.l	d1/a5,-(sp)

	move.l	a5,d1
	sub.l	#$DFF0A0,d1		; $00 $10 $20 $30
	lsr	#3,d1			; 0 2 4 6
	lea	Ritchy_Volumes,a5
	move	d0,(a5,d1.w)		; Sauve volume pour quitter

	movem.l	(sp)+,d1/a5

	BRA.S	mt_SetRegs

mt_NoLoop
	MOVE.L	n_start(A6),D2
	ADD.L	D3,D2
	MOVE.L	D2,n_loopstart(A6)
	MOVE.L	D2,n_wavestart(A6)
	MOVE.W	6(A3,D4.L),n_replen(A6)	; Save replen
	MOVEQ	#0,D0
	MOVE.B	n_volume(A6),D0
	MOVE.W	D0,8(A5)	; Set volume
mt_SetRegs
	MOVE.W	(A6),D0
	AND.W	#$0FFF,D0
	BEQ	mt_CheckMoreEfx	; If no note
	MOVE.W	2(A6),D0
	AND.W	#$0FF0,D0
	CMP.W	#$0E50,D0
	BEQ.S	mt_DoSetFineTune
	MOVE.B	2(A6),D0
	AND.B	#$0F,D0
	CMP.B	#3,D0	; TonePortamento
	BEQ.S	mt_ChkTonePorta
	CMP.B	#5,D0
	BEQ.S	mt_ChkTonePorta
	CMP.B	#9,D0	; Sample Offset
	BNE.S	mt_SetPeriod
	BSR	mt_CheckMoreEfx
	BRA.S	mt_SetPeriod

mt_DoSetFineTune
	BSR	mt_SetFineTune
	BRA.S	mt_SetPeriod

mt_ChkTonePorta
	BSR	mt_SetTonePorta
	BRA	mt_CheckMoreEfx

mt_SetPeriod
	MOVEM.L	D0-D1/A0-A1,-(SP)
	MOVE.W	(A6),D1
	AND.W	#$0FFF,D1
	LEA	mt_PeriodTable(PC),A1
	MOVEQ	#0,D0
	MOVEQ	#36,D7
mt_ftuloop
	CMP.W	(A1,D0.W),D1
	BHS.S	mt_ftufound
	ADDQ.L	#2,D0
	DBRA	D7,mt_ftuloop
mt_ftufound
	MOVEQ	#0,D1
	MOVE.B	n_finetune(A6),D1
	MULU	#36*2,D1
	ADD.L	D1,A1
	MOVE.W	(A1,D0.W),n_period(A6)
	MOVEM.L	(SP)+,D0-D1/A0-A1

	MOVE.W	2(A6),D0
	AND.W	#$0FF0,D0
	CMP.W	#$0ED0,D0 ; Notedelay
	BEQ	mt_CheckMoreEfx

	MOVE.W	n_dmabit(A6),$DFF096
	BTST	#2,n_wavecontrol(A6)
	BNE.S	mt_vibnoc
	CLR.B	n_vibratopos(A6)
mt_vibnoc
	BTST	#6,n_wavecontrol(A6)
	BNE.S	mt_trenoc
	CLR.B	n_tremolopos(A6)
mt_trenoc
	MOVE.L	n_start(A6),(A5)	; Set start
	MOVE.W	n_length(A6),4(A5)	; Set length
	MOVE.W	n_period(A6),D0
	MOVE.W	D0,6(A5)		; Set period
	MOVE.W	n_dmabit(A6),D0
	OR.W	D0,mt_DMACONtemp
	BRA	mt_CheckMoreEfx
 
mt_SetDMA
	MOVE.W	#600,D0
mt_WaitDMA
	DBRA	D0,mt_WaitDMA
	MOVE.W	mt_DMACONtemp(PC),D0
	OR.W	#$8000,D0
	MOVE.W	D0,$DFF096
	MOVE.W	#600,D0
mt_WaitDMA2
	DBRA	D0,mt_WaitDMA2

	LEA	$DFF000,A5
	LEA	mt_chan4temp(PC),A6
	MOVE.L	n_loopstart(A6),$D0(A5)
	MOVE.W	n_replen(A6),$D4(A5)
	LEA	mt_chan3temp(PC),A6
	MOVE.L	n_loopstart(A6),$C0(A5)
	MOVE.W	n_replen(A6),$C4(A5)
	LEA	mt_chan2temp(PC),A6
	MOVE.L	n_loopstart(A6),$B0(A5)
	MOVE.W	n_replen(A6),$B4(A5)
	LEA	mt_chan1temp(PC),A6
	MOVE.L	n_loopstart(A6),$A0(A5)
	MOVE.W	n_replen(A6),$A4(A5)

mt_dskip
	ADD.W	#16,mt_PatternPos
	MOVE.B	mt_PattDelTime,D0
	BEQ.S	mt_dskc
	MOVE.B	D0,mt_PattDelTime2
	CLR.B	mt_PattDelTime
mt_dskc	TST.B	mt_PattDelTime2
	BEQ.S	mt_dska
	SUBQ.B	#1,mt_PattDelTime2
	BEQ.S	mt_dska
	SUB.W	#16,mt_PatternPos
mt_dska	TST.B	mt_PBreakFlag
	BEQ.S	mt_nnpysk
	SF	mt_PBreakFlag
	MOVEQ	#0,D0
	MOVE.B	mt_PBreakPos(PC),D0
	CLR.B	mt_PBreakPos
	LSL.W	#4,D0
	MOVE.W	D0,mt_PatternPos
mt_nnpysk
	CMP.W	#1024,mt_PatternPos
	BLO.S	mt_NoNewPosYet
mt_NextPosition	
	MOVEQ	#0,D0
	MOVE.B	mt_PBreakPos(PC),D0
	LSL.W	#4,D0
	MOVE.W	D0,mt_PatternPos
	CLR.B	mt_PBreakPos
	CLR.B	mt_PosJumpFlag
	ADDQ.B	#1,mt_SongPos
	AND.B	#$7F,mt_SongPos
	MOVE.B	mt_SongPos(PC),D1
	MOVE.L	mt_SongDataPtr(PC),A0
	CMP.B	950(A0),D1
	BLO.S	mt_NoNewPosYet
	CLR.B	mt_SongPos
mt_NoNewPosYet	
	TST.B	mt_PosJumpFlag
	BNE.S	mt_NextPosition
	MOVEM.L	(SP)+,D0-D4/A0-A6
	RTS

mt_CheckEfx
	BSR	mt_UpdateFunk
	MOVE.W	n_cmd(A6),D0
	AND.W	#$0FFF,D0
	BEQ.S	mt_PerNop
	MOVE.B	n_cmd(A6),D0
	AND.B	#$0F,D0
	BEQ.S	mt_Arpeggio
	CMP.B	#1,D0
	BEQ	mt_PortaUp
	CMP.B	#2,D0
	BEQ	mt_PortaDown
	CMP.B	#3,D0
	BEQ	mt_TonePortamento
	CMP.B	#4,D0
	BEQ	mt_Vibrato
	CMP.B	#5,D0
	BEQ	mt_TonePlusVolSlide
	CMP.B	#6,D0
	BEQ	mt_VibratoPlusVolSlide
	CMP.B	#$E,D0
	BEQ	mt_E_Commands
SetBack	MOVE.W	n_period(A6),6(A5)
	CMP.B	#7,D0
	BEQ	mt_Tremolo
	CMP.B	#$A,D0
	BEQ	mt_VolumeSlide
mt_Return2
	RTS

mt_PerNop
	MOVE.W	n_period(A6),6(A5)
	RTS

mt_Arpeggio
	MOVEQ	#0,D0
	MOVE.B	mt_counter(PC),D0
	DIVS	#3,D0
	SWAP	D0
	CMP.W	#0,D0
	BEQ.S	mt_Arpeggio2
	CMP.W	#2,D0
	BEQ.S	mt_Arpeggio1
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	LSR.B	#4,D0
	BRA.S	mt_Arpeggio3

mt_Arpeggio1
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#15,D0
	BRA.S	mt_Arpeggio3

mt_Arpeggio2
	MOVE.W	n_period(A6),D2
	BRA.S	mt_Arpeggio4

mt_Arpeggio3
	ASL.W	#1,D0
	MOVEQ	#0,D1
	MOVE.B	n_finetune(A6),D1
	MULU	#36*2,D1
	LEA	mt_PeriodTable(PC),A0
	ADD.L	D1,A0
	MOVEQ	#0,D1
	MOVE.W	n_period(A6),D1
	MOVEQ	#36,D7
mt_arploop
	MOVE.W	(A0,D0.W),D2
	CMP.W	(A0),D1
	BHS.S	mt_Arpeggio4
	ADDQ.L	#2,A0
	DBRA	D7,mt_arploop
	RTS

mt_Arpeggio4
	MOVE.W	D2,6(A5)
	RTS

mt_FinePortaUp
	TST.B	mt_counter
	BNE.S	mt_Return2
	MOVE.B	#$0F,mt_LowMask
mt_PortaUp
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	mt_LowMask(PC),D0
	MOVE.B	#$FF,mt_LowMask
	SUB.W	D0,n_period(A6)
	MOVE.W	n_period(A6),D0
	AND.W	#$0FFF,D0
	CMP.W	#113,D0
	BPL.S	mt_PortaUskip
	AND.W	#$F000,n_period(A6)
	OR.W	#113,n_period(A6)
mt_PortaUskip
	MOVE.W	n_period(A6),D0
	AND.W	#$0FFF,D0
	MOVE.W	D0,6(A5)
	RTS	
 
mt_FinePortaDown
	TST.B	mt_counter
	BNE	mt_Return2
	MOVE.B	#$0F,mt_LowMask
mt_PortaDown
	CLR.W	D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	mt_LowMask(PC),D0
	MOVE.B	#$FF,mt_LowMask
	ADD.W	D0,n_period(A6)
	MOVE.W	n_period(A6),D0
	AND.W	#$0FFF,D0
	CMP.W	#856,D0
	BMI.S	mt_PortaDskip
	AND.W	#$F000,n_period(A6)
	OR.W	#856,n_period(A6)
mt_PortaDskip
	MOVE.W	n_period(A6),D0
	AND.W	#$0FFF,D0
	MOVE.W	D0,6(A5)
	RTS

mt_SetTonePorta
	MOVE.L	A0,-(SP)
	MOVE.W	(A6),D2
	AND.W	#$0FFF,D2
	MOVEQ	#0,D0
	MOVE.B	n_finetune(A6),D0
	MULU	#37*2,D0
	LEA	mt_PeriodTable(PC),A0
	ADD.L	D0,A0
	MOVEQ	#0,D0
mt_StpLoop
	CMP.W	(A0,D0.W),D2
	BHS.S	mt_StpFound
	ADDQ.W	#2,D0
	CMP.W	#37*2,D0
	BLO.S	mt_StpLoop
	MOVEQ	#35*2,D0
mt_StpFound
	MOVE.B	n_finetune(A6),D2
	AND.B	#8,D2
	BEQ.S	mt_StpGoss
	TST.W	D0
	BEQ.S	mt_StpGoss
	SUBQ.W	#2,D0
mt_StpGoss
	MOVE.W	(A0,D0.W),D2
	MOVE.L	(SP)+,A0
	MOVE.W	D2,n_wantedperiod(A6)
	MOVE.W	n_period(A6),D0
	CLR.B	n_toneportdirec(A6)
	CMP.W	D0,D2
	BEQ.S	mt_ClearTonePorta
	BGE	mt_Return2
	MOVE.B	#1,n_toneportdirec(A6)
	RTS

mt_ClearTonePorta
	CLR.W	n_wantedperiod(A6)
	RTS

mt_TonePortamento
	MOVE.B	n_cmdlo(A6),D0
	BEQ.S	mt_TonePortNoChange
	MOVE.B	D0,n_toneportspeed(A6)
	CLR.B	n_cmdlo(A6)
mt_TonePortNoChange
	TST.W	n_wantedperiod(A6)
	BEQ	mt_Return2
	MOVEQ	#0,D0
	MOVE.B	n_toneportspeed(A6),D0
	TST.B	n_toneportdirec(A6)
	BNE.S	mt_TonePortaUp
mt_TonePortaDown
	ADD.W	D0,n_period(A6)
	MOVE.W	n_wantedperiod(A6),D0
	CMP.W	n_period(A6),D0
	BGT.S	mt_TonePortaSetPer
	MOVE.W	n_wantedperiod(A6),n_period(A6)
	CLR.W	n_wantedperiod(A6)
	BRA.S	mt_TonePortaSetPer

mt_TonePortaUp
	SUB.W	D0,n_period(A6)
	MOVE.W	n_wantedperiod(A6),D0
	CMP.W	n_period(A6),D0
	BLT.S	mt_TonePortaSetPer
	MOVE.W	n_wantedperiod(A6),n_period(A6)
	CLR.W	n_wantedperiod(A6)

mt_TonePortaSetPer
	MOVE.W	n_period(A6),D2
	MOVE.B	n_glissfunk(A6),D0
	AND.B	#$0F,D0
	BEQ.S	mt_GlissSkip
	MOVEQ	#0,D0
	MOVE.B	n_finetune(A6),D0
	MULU	#36*2,D0
	LEA	mt_PeriodTable(PC),A0
	ADD.L	D0,A0
	MOVEQ	#0,D0
mt_GlissLoop
	CMP.W	(A0,D0.W),D2
	BHS.S	mt_GlissFound
	ADDQ.W	#2,D0
	CMP.W	#36*2,D0
	BLO.S	mt_GlissLoop
	MOVEQ	#35*2,D0
mt_GlissFound
	MOVE.W	(A0,D0.W),D2
mt_GlissSkip
	MOVE.W	D2,6(A5) ; Set period
	RTS

mt_Vibrato
	MOVE.B	n_cmdlo(A6),D0
	BEQ.S	mt_Vibrato2
	MOVE.B	n_vibratocmd(A6),D2
	AND.B	#$0F,D0
	BEQ.S	mt_vibskip
	AND.B	#$F0,D2
	OR.B	D0,D2
mt_vibskip
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$F0,D0
	BEQ.S	mt_vibskip2
	AND.B	#$0F,D2
	OR.B	D0,D2
mt_vibskip2
	MOVE.B	D2,n_vibratocmd(A6)
mt_Vibrato2
	MOVE.B	n_vibratopos(A6),D0
	LEA	mt_VibratoTable(PC),A4
	LSR.W	#2,D0
	AND.W	#$001F,D0
	MOVEQ	#0,D2
	MOVE.B	n_wavecontrol(A6),D2
	AND.B	#$03,D2
	BEQ.S	mt_vib_sine
	LSL.B	#3,D0
	CMP.B	#1,D2
	BEQ.S	mt_vib_rampdown
	MOVE.B	#255,D2
	BRA.S	mt_vib_set
mt_vib_rampdown
	TST.B	n_vibratopos(A6)
	BPL.S	mt_vib_rampdown2
	MOVE.B	#255,D2
	SUB.B	D0,D2
	BRA.S	mt_vib_set
mt_vib_rampdown2
	MOVE.B	D0,D2
	BRA.S	mt_vib_set
mt_vib_sine
	MOVE.B	0(A4,D0.W),D2
mt_vib_set
	MOVE.B	n_vibratocmd(A6),D0
	AND.W	#15,D0
	MULU	D0,D2
	LSR.W	#7,D2
	MOVE.W	n_period(A6),D0
	TST.B	n_vibratopos(A6)
	BMI.S	mt_VibratoNeg
	ADD.W	D2,D0
	BRA.S	mt_Vibrato3
mt_VibratoNeg
	SUB.W	D2,D0
mt_Vibrato3
	MOVE.W	D0,6(A5)
	MOVE.B	n_vibratocmd(A6),D0
	LSR.W	#2,D0
	AND.W	#$003C,D0
	ADD.B	D0,n_vibratopos(A6)
	RTS

mt_TonePlusVolSlide
	BSR	mt_TonePortNoChange
	BRA	mt_VolumeSlide

mt_VibratoPlusVolSlide
	BSR.S	mt_Vibrato2
	BRA	mt_VolumeSlide

mt_Tremolo
	MOVE.B	n_cmdlo(A6),D0
	BEQ.S	mt_Tremolo2
	MOVE.B	n_tremolocmd(A6),D2
	AND.B	#$0F,D0
	BEQ.S	mt_treskip
	AND.B	#$F0,D2
	OR.B	D0,D2
mt_treskip
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$F0,D0
	BEQ.S	mt_treskip2
	AND.B	#$0F,D2
	OR.B	D0,D2
mt_treskip2
	MOVE.B	D2,n_tremolocmd(A6)
mt_Tremolo2
	MOVE.B	n_tremolopos(A6),D0
	LEA	mt_VibratoTable(PC),A4
	LSR.W	#2,D0
	AND.W	#$001F,D0
	MOVEQ	#0,D2
	MOVE.B	n_wavecontrol(A6),D2
	LSR.B	#4,D2
	AND.B	#$03,D2
	BEQ.S	mt_tre_sine
	LSL.B	#3,D0
	CMP.B	#1,D2
	BEQ.S	mt_tre_rampdown
	MOVE.B	#255,D2
	BRA.S	mt_tre_set
mt_tre_rampdown
	TST.B	n_vibratopos(A6)
	BPL.S	mt_tre_rampdown2
	MOVE.B	#255,D2
	SUB.B	D0,D2
	BRA.S	mt_tre_set
mt_tre_rampdown2
	MOVE.B	D0,D2
	BRA.S	mt_tre_set
mt_tre_sine
	MOVE.B	0(A4,D0.W),D2
mt_tre_set
	MOVE.B	n_tremolocmd(A6),D0
	AND.W	#15,D0
	MULU	D0,D2
	LSR.W	#6,D2
	MOVEQ	#0,D0
	MOVE.B	n_volume(A6),D0
	TST.B	n_tremolopos(A6)
	BMI.S	mt_TremoloNeg
	ADD.W	D2,D0
	BRA.S	mt_Tremolo3
mt_TremoloNeg
	SUB.W	D2,D0
mt_Tremolo3
	BPL.S	mt_TremoloSkip
	CLR.W	D0
mt_TremoloSkip
	CMP.W	#$40,D0
	BLS.S	mt_TremoloOk
	MOVE.W	#$40,D0
mt_TremoloOk
	MOVE.W	D0,8(A5)
	MOVE.B	n_tremolocmd(A6),D0
	LSR.W	#2,D0
	AND.W	#$003C,D0
	ADD.B	D0,n_tremolopos(A6)
	RTS

mt_SampleOffset
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	BEQ.S	mt_sononew
	MOVE.B	D0,n_sampleoffset(A6)
mt_sononew
	MOVE.B	n_sampleoffset(A6),D0
	LSL.W	#7,D0
	CMP.W	n_length(A6),D0
	BGE.S	mt_sofskip
	SUB.W	D0,n_length(A6)
	LSL.W	#1,D0
	ADD.L	D0,n_start(A6)
	RTS
mt_sofskip
	MOVE.W	#$0001,n_length(A6)
	RTS

mt_VolumeSlide
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	LSR.B	#4,D0
	TST.B	D0
	BEQ.S	mt_VolSlideDown
mt_VolSlideUp
	ADD.B	D0,n_volume(A6)
	CMP.B	#$40,n_volume(A6)
	BMI.S	mt_vsuskip
	MOVE.B	#$40,n_volume(A6)
mt_vsuskip
	MOVE.B	n_volume(A6),D0
	MOVE.W	D0,8(A5)
	RTS

mt_VolSlideDown
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
mt_VolSlideDown2
	SUB.B	D0,n_volume(A6)
	BPL.S	mt_vsdskip
	CLR.B	n_volume(A6)
mt_vsdskip
	MOVE.B	n_volume(A6),D0
	MOVE.W	D0,8(A5)
	RTS

mt_PositionJump
	MOVE.B	n_cmdlo(A6),D0
	SUBQ.B	#1,D0
	MOVE.B	D0,mt_SongPos
mt_pj2	CLR.B	mt_PBreakPos
	ST 	mt_PosJumpFlag
	RTS

mt_VolumeChange
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	CMP.B	#$40,D0
	BLS.S	mt_VolumeOk
	MOVEQ	#$40,D0
mt_VolumeOk
	MOVE.B	D0,n_volume(A6)
	MOVE.W	D0,8(A5)
	RTS

mt_PatternBreak
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	MOVE.L	D0,D2
	LSR.B	#4,D0
	MULU	#10,D0
	AND.B	#$0F,D2
	ADD.B	D2,D0
	CMP.B	#63,D0
	BHI.S	mt_pj2
	MOVE.B	D0,mt_PBreakPos
	ST	mt_PosJumpFlag
	RTS

mt_SetSpeed
	MOVE.B	3(A6),D0
	BEQ	mt_Return2
	CLR.B	mt_counter
	MOVE.B	D0,mt_speed
	RTS

mt_CheckMoreEfx
	BSR	mt_UpdateFunk
	MOVE.B	2(A6),D0
	AND.B	#$0F,D0
	CMP.B	#$9,D0
	BEQ	mt_SampleOffset
	CMP.B	#$B,D0
	BEQ	mt_PositionJump
	CMP.B	#$D,D0
	BEQ.S	mt_PatternBreak
	CMP.B	#$E,D0
	BEQ.S	mt_E_Commands
	CMP.B	#$F,D0
	BEQ.S	mt_SetSpeed
	CMP.B	#$C,D0
	BEQ	mt_VolumeChange
	BRA	mt_PerNop

mt_E_Commands
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$F0,D0
	LSR.B	#4,D0
	BEQ.S	mt_FilterOnOff
	CMP.B	#1,D0
	BEQ	mt_FinePortaUp
	CMP.B	#2,D0
	BEQ	mt_FinePortaDown
	CMP.B	#3,D0
	BEQ.S	mt_SetGlissControl
	CMP.B	#4,D0
	BEQ	mt_SetVibratoControl
	CMP.B	#5,D0
	BEQ	mt_SetFineTune
	CMP.B	#6,D0
	BEQ	mt_JumpLoop
	CMP.B	#7,D0
	BEQ	mt_SetTremoloControl
	CMP.B	#9,D0
	BEQ	mt_RetrigNote
	CMP.B	#$A,D0
	BEQ	mt_VolumeFineUp
	CMP.B	#$B,D0
	BEQ	mt_VolumeFineDown
	CMP.B	#$C,D0
	BEQ	mt_NoteCut
	CMP.B	#$D,D0
	BEQ	mt_NoteDelay
	CMP.B	#$E,D0
	BEQ	mt_PatternDelay
	CMP.B	#$F,D0
	BEQ	mt_FunkIt
	RTS

mt_FilterOnOff
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#1,D0
	ASL.B	#1,D0
	AND.B	#$FD,$BFE001
	OR.B	D0,$BFE001
	RTS	

mt_SetGlissControl
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	AND.B	#$F0,n_glissfunk(A6)
	OR.B	D0,n_glissfunk(A6)
	RTS

mt_SetVibratoControl
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	AND.B	#$F0,n_wavecontrol(A6)
	OR.B	D0,n_wavecontrol(A6)
	RTS

mt_SetFineTune
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	MOVE.B	D0,n_finetune(A6)
	RTS

mt_JumpLoop
	TST.B	mt_counter
	BNE	mt_Return2
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	BEQ.S	mt_SetLoop
	TST.B	n_loopcount(A6)
	BEQ.S	mt_jumpcnt
	SUBQ.B	#1,n_loopcount(A6)
	BEQ	mt_Return2
mt_jmploop	MOVE.B	n_pattpos(A6),mt_PBreakPos
	ST	mt_PBreakFlag
	RTS

mt_jumpcnt
	MOVE.B	D0,n_loopcount(A6)
	BRA.S	mt_jmploop

mt_SetLoop
	MOVE.W	mt_PatternPos(PC),D0
	LSR.W	#4,D0
	MOVE.B	D0,n_pattpos(A6)
	RTS

mt_SetTremoloControl
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	LSL.B	#4,D0
	AND.B	#$0F,n_wavecontrol(A6)
	OR.B	D0,n_wavecontrol(A6)
	RTS

mt_RetrigNote
	MOVE.L	D1,-(SP)
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	BEQ.S	mt_rtnend
	MOVEQ	#0,D1
	MOVE.B	mt_counter(PC),D1
	BNE.S	mt_rtnskp
	MOVE.W	(A6),D1
	AND.W	#$0FFF,D1
	BNE.S	mt_rtnend
	MOVEQ	#0,D1
	MOVE.B	mt_counter(PC),D1
mt_rtnskp
	DIVU	D0,D1
	SWAP	D1
	TST.W	D1
	BNE.S	mt_rtnend
mt_DoRetrig
	MOVE.W	n_dmabit(A6),$DFF096	; Channel DMA off
	MOVE.L	n_start(A6),(A5)	; Set sampledata pointer
	MOVE.W	n_length(A6),4(A5)	; Set length
	MOVE.W	#600,D0
mt_rtnloop1
	DBRA	D0,mt_rtnloop1
	MOVE.W	n_dmabit(A6),D0
	BSET	#15,D0
	MOVE.W	D0,$DFF096
	MOVE.W	#600,D0
mt_rtnloop2
	DBRA	D0,mt_rtnloop2
	MOVE.L	n_loopstart(A6),(A5)
	MOVE.L	n_replen(A6),4(A5)
mt_rtnend
	MOVE.L	(SP)+,D1
	RTS

mt_VolumeFineUp
	TST.B	mt_counter
	BNE	mt_Return2
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$F,D0
	BRA	mt_VolSlideUp

mt_VolumeFineDown
	TST.B	mt_counter
	BNE	mt_Return2
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	BRA	mt_VolSlideDown2

mt_NoteCut
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	CMP.B	mt_counter(PC),D0
	BNE	mt_Return2
	CLR.B	n_volume(A6)
	MOVE.W	#0,8(A5)
	RTS

mt_NoteDelay
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	CMP.B	mt_Counter,D0
	BNE	mt_Return2
	MOVE.W	(A6),D0
	BEQ	mt_Return2
	MOVE.L	D1,-(SP)
	BRA	mt_DoRetrig

mt_PatternDelay
	TST.B	mt_counter
	BNE	mt_Return2
	MOVEQ	#0,D0
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	TST.B	mt_PattDelTime2
	BNE	mt_Return2
	ADDQ.B	#1,D0
	MOVE.B	D0,mt_PattDelTime
	RTS

mt_FunkIt
	TST.B	mt_counter
	BNE	mt_Return2
	MOVE.B	n_cmdlo(A6),D0
	AND.B	#$0F,D0
	LSL.B	#4,D0
	AND.B	#$0F,n_glissfunk(A6)
	OR.B	D0,n_glissfunk(A6)
	TST.B	D0
	BEQ	mt_Return2
mt_UpdateFunk
	MOVEM.L	A0/D1,-(SP)
	MOVEQ	#0,D0
	MOVE.B	n_glissfunk(A6),D0
	LSR.B	#4,D0
	BEQ.S	mt_funkend
	LEA	mt_FunkTable(PC),A0
	MOVE.B	(A0,D0.W),D0
	ADD.B	D0,n_funkoffset(A6)
	BTST	#7,n_funkoffset(A6)
	BEQ.S	mt_funkend
	CLR.B	n_funkoffset(A6)

	MOVE.L	n_loopstart(A6),D0
	MOVEQ	#0,D1
	MOVE.W	n_replen(A6),D1
	ADD.L	D1,D0
	ADD.L	D1,D0
	MOVE.L	n_wavestart(A6),A0
	ADDQ.L	#1,A0
	CMP.L	D0,A0
	BLO.S	mt_funkok
	MOVE.L	n_loopstart(A6),A0
mt_funkok
	MOVE.L	A0,n_wavestart(A6)
	MOVEQ	#-1,D0
	SUB.B	(A0),D0
	MOVE.B	D0,(A0)
mt_funkend
	MOVEM.L	(SP)+,A0/D1
	RTS


mt_FunkTable dc.b 0,5,6,7,8,10,11,13,16,19,22,26,32,43,64,128

mt_VibratoTable	
	dc.b   0, 24, 49, 74, 97,120,141,161
	dc.b 180,197,212,224,235,244,250,253
	dc.b 255,253,250,244,235,224,212,197
	dc.b 180,161,141,120, 97, 74, 49, 24

mt_PeriodTable
; Tuning 0, Normal
	dc.w	856,808,762,720,678,640,604,570,538,508,480,453
	dc.w	428,404,381,360,339,320,302,285,269,254,240,226
	dc.w	214,202,190,180,170,160,151,143,135,127,120,113
; Tuning 1
	dc.w	850,802,757,715,674,637,601,567,535,505,477,450
	dc.w	425,401,379,357,337,318,300,284,268,253,239,225
	dc.w	213,201,189,179,169,159,150,142,134,126,119,113
; Tuning 2
	dc.w	844,796,752,709,670,632,597,563,532,502,474,447
	dc.w	422,398,376,355,335,316,298,282,266,251,237,224
	dc.w	211,199,188,177,167,158,149,141,133,125,118,112
; Tuning 3
	dc.w	838,791,746,704,665,628,592,559,528,498,470,444
	dc.w	419,395,373,352,332,314,296,280,264,249,235,222
	dc.w	209,198,187,176,166,157,148,140,132,125,118,111
; Tuning 4
	dc.w	832,785,741,699,660,623,588,555,524,495,467,441
	dc.w	416,392,370,350,330,312,294,278,262,247,233,220
	dc.w	208,196,185,175,165,156,147,139,131,124,117,110
; Tuning 5
	dc.w	826,779,736,694,655,619,584,551,520,491,463,437
	dc.w	413,390,368,347,328,309,292,276,260,245,232,219
	dc.w	206,195,184,174,164,155,146,138,130,123,116,109
; Tuning 6
	dc.w	820,774,730,689,651,614,580,547,516,487,460,434
	dc.w	410,387,365,345,325,307,290,274,258,244,230,217
	dc.w	205,193,183,172,163,154,145,137,129,122,115,109
; Tuning 7
	dc.w	814,768,725,684,646,610,575,543,513,484,457,431
	dc.w	407,384,363,342,323,305,288,272,256,242,228,216
	dc.w	204,192,181,171,161,152,144,136,128,121,114,108
; Tuning -8
	dc.w	907,856,808,762,720,678,640,604,570,538,508,480
	dc.w	453,428,404,381,360,339,320,302,285,269,254,240
	dc.w	226,214,202,190,180,170,160,151,143,135,127,120
; Tuning -7
	dc.w	900,850,802,757,715,675,636,601,567,535,505,477
	dc.w	450,425,401,379,357,337,318,300,284,268,253,238
	dc.w	225,212,200,189,179,169,159,150,142,134,126,119
; Tuning -6
	dc.w	894,844,796,752,709,670,632,597,563,532,502,474
	dc.w	447,422,398,376,355,335,316,298,282,266,251,237
	dc.w	223,211,199,188,177,167,158,149,141,133,125,118
; Tuning -5
	dc.w	887,838,791,746,704,665,628,592,559,528,498,470
	dc.w	444,419,395,373,352,332,314,296,280,264,249,235
	dc.w	222,209,198,187,176,166,157,148,140,132,125,118
; Tuning -4
	dc.w	881,832,785,741,699,660,623,588,555,524,494,467
	dc.w	441,416,392,370,350,330,312,294,278,262,247,233
	dc.w	220,208,196,185,175,165,156,147,139,131,123,117
; Tuning -3
	dc.w	875,826,779,736,694,655,619,584,551,520,491,463
	dc.w	437,413,390,368,347,328,309,292,276,260,245,232
	dc.w	219,206,195,184,174,164,155,146,138,130,123,116
; Tuning -2
	dc.w	868,820,774,730,689,651,614,580,547,516,487,460
	dc.w	434,410,387,365,345,325,307,290,274,258,244,230
	dc.w	217,205,193,183,172,163,154,145,137,129,122,115
; Tuning -1
	dc.w	862,814,768,725,684,646,610,575,543,513,484,457
	dc.w	431,407,384,363,342,323,305,288,272,256,242,228
	dc.w	216,203,192,181,171,161,152,144,136,128,121,114

mt_chan1temp	dc.l	0,0,0,0,0,$00010000,0,  0,0,0,0
mt_chan2temp	dc.l	0,0,0,0,0,$00020000,0,  0,0,0,0
mt_chan3temp	dc.l	0,0,0,0,0,$00040000,0,  0,0,0,0
mt_chan4temp	dc.l	0,0,0,0,0,$00080000,0,  0,0,0,0

mt_SampleStarts	dc.l	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		dc.l	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

mt_SongDataPtr	dc.l 0

mt_speed	dc.b 6
mt_counter	dc.b 0
mt_SongPos	dc.b 0
mt_PBreakPos	dc.b 0
mt_PosJumpFlag	dc.b 0
mt_PBreakFlag	dc.b 0
mt_LowMask	dc.b 0
mt_PattDelTime	dc.b 0
mt_PattDelTime2	dc.b 0,0

mt_PatternPos	dc.w 0
mt_DMACONtemp	dc.w 0
;/* End of File */

Ritchy_Volumes:	blk.w	4,64
GfxBase:	dc.l	0
GfxName:	dc.b	'graphics.library',0
		even

Wait:		dc.l	0
permit:		dc.w	0
mousex:		dc.w	$80
mousey:		dc.w	$70
lastx:		dc.b	0
lasty:		dc.b	0
Auto:		dc.b	0
	even

	section chip,data

Pointeur:
	dc.w $0000,$0000
	dc.w $FC00,$F800
	dc.w $8800,$F000
	dc.w $9000,$E000
	dc.w $A000,$D800
	dc.w $C000,$9C00
	dc.w $8000,$0E00
	dc.w $0000,$0700
	dc.w $0080,$0300
	dc.w $0100,$0000
	dc.w $0000,$0000


safemem:dc.w 0
	dc.w 0

;copper list

Compteur:	dc.w	8
PTtexte:	dc.l	Texte

; ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789    .  ,  '  !  :  -  ?  =>

Texte:	dc.b	'                  '
	dc.b	'RITCHY AND EREKOSE PRESENT TO YOU THIS LITTLE'
	Dc.B " PRODUCTION FROM THE MOST"
	Dc.B " WELLKNOWN GROUP POPULAR FOR HIS AMAZING, SMOKING, DRINKING,"
	Dc.B " FUC...HUHO...HER...HOPLAING, EVERYTHINGS-ING GUYS !! YES, "
	Dc.B " ANOTHER UNBELIEVABLE PRODUCTION IS NOW UNDER YOUR EYES FROM THIS"
	Dc.B " GROUP CALLED........> PARLAFOX...HU, NO.....> BRISTOL....HU, SHIT."
	Dc.B ".....AH YEAH ! KANGOUROU BROTHERS !......AH, SHIT...> GOLANA !"
	Dc.B ".......NO.........AH THAT'S IT......A N A L O G !!    YYEEESS !"
	Dc.B " I FOUND IT !!!! YYYEEEEEEEAAAAHHHHHH !! YAHOU, YIPIE !! YARGLA !"
	Dc.B "..............HEY !....WHAT ?........WE ALL ALLREADY NOTICE THAT...."
	Dc.B "...AH ? REALLY ??.............OK OK....."
	Dc.B "WELL, AS I (EREKOSE) REALLY DONT KNOW WHAT TO SAY, LET'S WRITE SOME"
	Dc.B " BULLSHIT....SO, THIS LITTLE MUSIC DISK WAS DONE IN ONE NIGHT, AT"
	Dc.B " RITCHY'S HOME, JUST AFTER THE TRADIONNAL SPAGHETTI-YOGURT DINER."
	Dc.B " ALL CODE WAS DONE BY...!! RITCHY !!...ALL MUSIC BY EREKOSE (ME) AND MY"
	Dc.B " FRIEND MONTY>US, BUT OLD MEMBER OF ANALOG...AND AT LAST BUT NOT LEAST"
	Dc.B " ALL GFX BY THE WONDERFULL...!!! BAOBAB !!!..."
	Dc.B "AND NOW FOR SOMETHING COMPLETLY DIFFERENT....A COW WITH TWO NOSES..."
	Dc.B "NO,NO,NO....A MEN WITH THREE EYES.......NO,NO,NO...."
	Dc.B " JE TIENS ICI A SIGNALER QUE TOUTES RESSEMBLANCE AVEC DES NOMS"
	Dc.B " OU DES GROUPES CONNUS NE SERAIT QUE PUREMENT VOLONTAIRE."
	Dc.B " POP-QUIZ: WHO LIKE MAC DONALDS ? "
	Dc.B " ANSWER: NOT ME !!! "
	Dc.B "AND NOW ? WELL, I THINK I'LL JUST GIVE A LITTLE HELLO TO ALL MY OLD"
	Dc.B "FRIENDS AND CONTACT I LEFT FOR SOME TIMES (I'M NOW IN THE ARMY AND"
	Dc.B " ITS REALLY BORING !!!! NEVER DO THE ARMY IF YA BE ABLE TO !!)."
	Dc.B " SO, BIG KISS TO ALL MEMBERS OF ANALOG, MONTY AND VANESSA, ZUUL(PC),"  
	Dc.B " ALEX>MVT, BROSS>TSB, JOGEIR, KAEL, KARSTEN (INDEP), BAD CAT>IBB,"
	Dc.B " , STEPHANE, RACKLER>LEGO, MR KING, ZINKFLOYD>TRSI, WHIRLY>DELIGHT,"
	Dc.B " MANITOU>MOSQUITO, SHUN>LIQUID, SHED>CRYO, SANDMAN>EXILE, CORTEX, KILLER>ARKHAM,"
	Dc.B " SNOOPY>DEFORM.............TO ALL OF YOU, REALLY SORRY FOR MY FUCKING"
	Dc.B " BIG DELAY, BUT I REALLY DONT REACH TO BE A GOOD SWAPPER....I'M TOO"
	Dc.B " LAZY FOR THAT..................GOOD BYE !................."
	Dc.B "SCROLL TEXT RESTART...............5.........4.........3........1....."
	Dc.B " HEY !!! WHERE IS THE 2 ????.......JUST KIDDING !.....2......1....."
	Dc.B " HAVE A NICE DAY !......THANK YOU !!!!!!!!......................"

	dc.b	0
	even
TexteInfo:
	dc.b	'   WELCOME IN THE INFOTEXT... '
	dc.b	"  J'SUIS D'ACCORD AVEC SERGE!!!   "
	dc.b	'PRESS ESC TO QUIT CHEAP DISK    '
	dc.b	'CLICK ON NAME OF MUSIC TO CHOOSE ANOTHER ONE....INCREDIBLE'
	dc.b	' NO ?    ' 
	dc.b	'CLICK ON PAUSE TO..........PAUSE ?.....YEAH !' 
	dc.b	'CLICK ON PLAY TO REPLAY AFTER PAUSE...'
	dc.b	'CLICK ON FILTER TO TOGGLE THE FILTER ON-OFF      '
	dc.b	'CLICK ON INFO TO.......OH, YOU ALREADY DID IT !!!   '
	dc.b	'CREDITS FOR THIS ONE-NIGHT MUSIC-FILE:   '
	dc.b	'CODE > RITCHY - FONT > PLASMA - '
	dc.b	'GFX > BAOBAB - MORAL SUPPORT > WORGLUB...   '

	dc.b	'THIS MUSICAL-DEMO HAS BEEN CODED IN ONE LONG NIGHT...  '
	DC.B	"THANX TO THE COCA-COLA AND BEER, CHESTERFIELD AND ROYAL "
	DC.B	"MENTHOL CIGARETTES, MARS ICECREAM AND CREME MONTBLANC "
	DC.B	"A LA VANILLE...QU'ON A OUBLIE DE MANGER ! MAIS TANT MIEUX"
	DC.B	" CA EN FERA PLUS POUR MOI!      "
	DC.B	"CHRISTINE !!! Y'T'RESTE PAS QUE'QUES CAHUETTES ?  "
	DC.B	"         ALLEZ CHRISTINE, QUOI !! FAIT PETER LES CAHUETTES !"
	DC.B	'                    '

	DC.B	"BON, ICI JE -RITCHY- PEUX TAPER QUELQUECHOSE CAR PERSONNE "
	DC.B	"NE LE LIRA JAMAIS... JE VOUDRAIS DIRE QUE ANALOG EST "
	DC.B	"VRAIMENT UN GROUPE SUPER ! UNE DEMO PAR AN, TOUT LE MONDE "
	DC.B	"TE CRITIQUE ET DE PREFERENCE CEUX QUI NE FONT RIEN...    "
	DC.B	"BISOUS A LAETI,FIFI,STEPH,...,PAT,THIERRY,DEUFRE,SPEEDMAN,"
	DC.B	"DD FROM SPIDER!,7TH YEUX AUSSI,CHAOS,ACKERLIGHT,BAMIGA "
	DC.B	"SECTOR ONE,THE KENT TEAM,RED SECTOR,PLASMA FORCE,UNIT A,"
	DC.B	"DOC,WILD COPPER,REBELS,CBC,CASCADE,MEGAFORCE... "
	DC.B	"AND N'OUBLIEZ PAS >>> ONLY AMIGA "
	DC.B	"MAKES IT POSSIBLE!  BISOUS A TOI AUSSI PETIT LECTEUR DE "
	DC.B	"SCROLLING,CAR ON T'EMBRASSE PAS SOUVENT ET C'EST PAS JUSTE."

	dc.b	"THIS DEMO IS FREEWARE, IT MEANS THAT IS A >FREE SOFTWARE, "
	DC.B	"SO IT'S FORBIDDEN DE LA VENDRE !   "

	dc.b	'           ',0
	even


COPPER2:dc.l	$008e6981,$009085c1
	dc.l	$00920068,$00940098
Bpls2:	dc.l	$00e00000,$00e20000
	dc.l	$00e40000,$00e60000
	dc.l	$00e80000,$00ea0000
com:	dc.l	$01000000,$01020000
	dc.l	$01040033,$01FC0000
	dc.w	$10c,$11,$106,0
	dc.l	$01080038,$010a0038
	dc.w	$120,0,$122,0
	dc.w	$124,0,$126,0
	dc.w	$128,0,$12a,0
	dc.w	$12c,0,$12e,0
	dc.w	$130,0,$132,0
	dc.w	$134,0,$136,0
	dc.w	$138,0,$13a,0
	dc.w	$13c,0,$13e,0

	dc.w $0106,$0000
	dc.w $0180,$033a
	dc.w $0182,$0565
	dc.w $0184,$0676
	dc.w $0186,$0797
	dc.w $0188,$08A8
	dc.w $018A,$0ABA
	dc.w $018C,$0BDB
	dc.w $018E,$0CEC

	dc.w	$ce0f,$fffe,$180,$229

	dc.l	-2


TableEqu:
	dc.w	$F29,$100	; Init,Dec
	dc.w	$2F9,$010
	dc.w	$FF9,$110
	dc.w	$2B0,$00F

copper:	dc.l	$008e2981,$009029c1
r38d0:	dc.l	$00920080,$009400c0
Bpls:	dc.l	$00e00000,$00e20000
	dc.l	$00e40000,$00e60000
	dc.l	$00e80000,$00ea0000
	dc.l	$00ec0000,$00ee0000
	dc.l	$00f00000,$00f20000
	dc.l	$01005200,$01020000	; 5 bitplanes
	dc.l	$01040033,$01FC0000
	dc.w	$10c,$11,$106,0
Mod:	dc.l	$010800aa,$010a00aa

Sprt:	dc.w	$120,0,$122,0
	dc.w	$124,0,$126,0
	dc.w	$128,0,$12a,0
	dc.w	$12c,0,$12e,0
	dc.w	$130,0,$132,0
	dc.w	$134,0,$136,0
	dc.w	$138,0,$13a,0
	dc.w	$13c,0,$13e,0


	dc.w	$180,$033a,$182,$0fff
Coul:	dc.w	$184,$eef,$186,$0ccf
	dc.w	$188,$0bbf,$18a,$09af,$18c,$088f,$18e,$067f
	dc.w	$190,$055d,$192,$044c,$194,$033a,$196,$0229
	dc.w	$198,$0117,$19a,$0116,$19c,$0004,$19e,$0003
	dc.w	$1a0,$0ffc,$1a2,$0eea,$1a4,$0dd9,$1a6,$0cb8
	dc.w	$1a8,$0ba7,$1aa,$0b96,$1ac,$0a85,$1ae,$0964
	dc.w	$1b0,$0853,$1b2,$0742,$1b4,$0632,$1b6,$0521
	dc.w	$1b8,$0511,$1ba,$0410,$1bc,$0300,$1be,$0200


; Origine EEF
; 55D,358,55d

coul0:	dc.w	$500f,$fffe,$184,$eef

coul1:	dc.w	$930f,$fffe,$184,$eef
coul2:	dc.w	$9c0f,$fffe,$184,$eef
coul3:	dc.w	$a50f,$fffe,$184,$eef
coul4:	dc.w	$ae0f,$fffe,$184,$eef
coul5:	dc.w	$b70f,$fffe,$184,$eef
coul6:	dc.w	$c00f,$fffe,$184,$eef
coul7:	dc.w	$c90f,$fffe,$184,$eef
	dc.w	$ce0f,$fffe,$180,$229
coul8:	dc.w	$d20f,$fffe,$184,$eef
coul9:	dc.w	$db0f,$fffe,$184,$eef
coul10:	dc.w	$e40f,$fffe,$184,$eef

	dc.w	$ef0f,$fffe
Eq1:	dc.w	$184,$eef
	dc.w	$1B8,$511
	dc.w	$1BA,$410
	dc.w	$1BC,$300

	dc.w	$ffff,$ffde
	dc.w	$100f,$fffe,$1b8,$511

	dc.l	-2

Tcoul:	dc.l	coul1,coul2,coul3,coul4
	dc.l	coul5,coul6,coul7,coul8,coul9

tmm:	dc.l	mm1,mm2,mm3,mm4,mm5,mm6,mm7,mm8,mm9

mm1:	incbin	'mod.status'
mm2:	incbin	'mod.adel'
mm3:	incbin	'mod.compote'
mm4:	incbin	'mod.banzai'
mm5:	incbin	'mod.milk'
mm6:	incbin	'mod.jambon'
mm7:	incbin	'mod.atom'
mm8:	incbin	'mod.computer'
mm9:	incbin	'mod.track'

mt_data:	dc.l	mm1

Present:	incbin	'RB.Present'

Font:	incbin	'RB.Fonte8'
PicN:	blk.b	18*5*256,0
Nounou:	incbin	'RB.Nounours'
	blk.b	18*5*187,0
Pic:	incbin	'RB.Fond'
Scr1:	blk.b	42*5*12,0
Buffer:	blk.b	42*5*12,0
finintro:
