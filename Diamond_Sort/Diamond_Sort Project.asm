#   SortDiamond size-hue sorter
#
#   February 27, 2018                Joao Matheus Nascimento Francolin

.data
Array:		.alloc 169				# allocate static space for diamond

.text

Sort:	addi	$01,    $00,	Array			# set memory base
        swi		52     							# create sort diamond and update memory
        addi 	$20,    $01,	412

L1:				addi 	$03,	$01,	76		# cp = 19
        addi 	$05,    $01,	364				# fp = 91
        addi 	$11,	$00,	00				# flag = 0

L2:		slt		$10,	$03,	$20				# exit if cp < 103
        beq 	$10,	$00,	ExitL2

L3:				slt		$10,	$03,	$05		# exit if cp < fp
        beq 	$10,	$00,	ExitL3

        lw 		$12,	0($03) 					# Array[cp]
        srl		$06,	$12,	06				# diam_c = Array[cp] / 64

        lw 		$13,	-52($03)
        srl		$08,	$13,	06				# diam_m = Array[mp] / 64;
### upper element
Up:		slt		$10,	$06,	$08				# if diam_c < diam_m proceed
        beq 	$10,	$00,	Dow
        sw 		$12,	-52($03)				# Array[mp] = Array[cp]
        sw 		$13,	0($03)					# Array[cp] = Array[cp]

        lw 		$12,	0($03) 					# Array[cp]
        srl		$06,	$12,	06				# diam_c = Array[cp] / 64
        addi 	$11,	$00, 	1  				# flag = 1

### down element
Down:	lw 		$13,	52($03)
        srl		$08,	$13,	06				# diam_m = Array[mp] / 64;

        slt		$10,	$08,	$06				# if diam_m < diam_c proceed
        beq 	$10,	$00,	Left
        sw 		$12,	52($03)					# Array[mp] = Array[cp]
        sw 		$13,	0($03)					# Array[cp] = Array[cp]

        lw 		$12,	0($03) 					# Array[cp]
        addi 	$11,	$00, 	1  				# flag = 1

### left element
Left:	andi 	$07,	$12,	0x3F			# hue_c = Array[cp] % 64
        lw 		$13,	-4($03)
        andi 	$08,	$13,	0X3F		    # hue_m = Array[mp] % 64

        slt		$10,	$07,	$08				# if hue_c < hue_m proceed
        beq 	$10,	$00,	Right
        sw 		$12,	-4($03)					# Array[mp] = Array[cp]
        sw 		$13,	0($03)					# Array[cp] = Array[cp]

        lw 		$12,	0($03) 					# Array[cp]
        andi 	$07,	$12,	0x3F			# hue_c = Array[cp] % 64
        addi 	$11,	$00, 	1				# flag = 1

Right: 	lw 		$13,	4($03)
        andi 	$08,	$13,	0X3F			# hue_m = Array[mp] % 64

        slt		$10,	$08,	$07				# if hue_m < hue_c proceed
        beq 	$10,	$00,	Else
        sw 		$12,	4($03)					# Array[mp] = Array[cp]
        sw 		$13,	0($03)					# Array[cp] = Array[cp]

        addi 	$11,	$00, 	1				# flag = 1

Else:	addi 	$03,	$03,	48 				# cp = cp + 12
        j 		L3

ExitL3:	addi 	$03,	$03,	-232			# cp = cp - 58
        addi 	$05,	$05,	56				# fp = fp + 14
        j 		L2

ExitL2: bne 	$11,	$00,	L1				# loop while flag != 0
        jr		$31								# return to caller
