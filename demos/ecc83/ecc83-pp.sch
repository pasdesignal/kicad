EESchema Schematic File Version 2
LIBS:power,device,conn,valves,linear,regul,74xx,cmos4000,adc-dac,memory,xilinx,special,microcontrollers,microchip,analog_switches,motorola,intel,audio,interface,digital-audio,philips,display,cypress,siliconi,contrib,.\ecc83-pp.cache
EELAYER 23  0
EELAYER END
$Descr A4 11700 8267
Sheet 1 1
Title "ECC Push-Pull"
Date "11 dec 2006"
Rev "0.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6050 4450 6150 4450
Wire Wire Line
	5750 3850 5750 4050
Connection ~ 4450 4500
Wire Wire Line
	4450 5400 4450 4450
Wire Wire Line
	4450 4450 4650 4450
Wire Wire Line
	9850 6350 9950 6350
Wire Wire Line
	9950 6350 9950 6150
Wire Wire Line
	4150 5300 4150 5000
Wire Wire Line
	6950 5150 6950 5000
Wire Wire Line
	4150 4800 4450 4800
Connection ~ 4450 4800
Wire Wire Line
	6950 4350 6950 4050
Connection ~ 6950 4250
Wire Wire Line
	4850 4850 4850 5400
Wire Wire Line
	6300 3900 6300 4000
Wire Wire Line
	6950 3850 5750 3850
Connection ~ 6700 3850
Wire Wire Line
	6700 4250 6950 4250
Wire Wire Line
	6150 4450 6150 3900
Connection ~ 6150 3900
Wire Wire Line
	6300 4500 6300 4900
Connection ~ 6300 4800
Wire Wire Line
	6950 4800 6700 4800
Wire Wire Line
	6700 4800 6700 5400
Wire Wire Line
	10050 6150 10050 6350
Wire Wire Line
	10050 6250 9850 6250
Connection ~ 10050 6250
Wire Wire Line
	4100 5250 4150 5250
Connection ~ 4150 5250
Wire Wire Line
	9850 6250 9850 6150
Wire Wire Line
	6300 3900 4950 3900
Wire Wire Line
	4950 3900 4950 4050
Wire Wire Line
	5850 4850 5850 4900
Wire Wire Line
	5850 4900 6300 4900
$Comp
L ECC83 U1
U 3 1 48B4F266
P 9950 5700
F 0 "U1" H 10080 6010 50  0000 C C
F 1 "ECC81_83" H 10300 5400 50  0000 C C
F 2 "VALVE-NOVAL_P" H 10220 5300 50  0001 C C
	3    9950 5700
	1    0    0    -1  
$EndComp
$Comp
L ECC83 U1
U 2 1 48B4F263
P 4950 4450
F 0 "U1" H 5080 4760 50  0000 C C
F 1 "ECC83" H 5150 4100 50  0000 C C
F 2 "VALVE-NOVAL_P" H 5220 4050 50  0001 C C
	2    4950 4450
	1    0    0    -1  
$EndComp
$Comp
L ECC83 U1
U 1 1 48B4F256
P 5750 4450
F 0 "U1" H 5880 4760 50  0000 C C
F 1 "ECC83" H 5500 4150 50  0000 C C
F 2 "VALVE-NOVAL_P" H 6020 4050 50  0001 C C
	1    5750 4450
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 457DBAF8
P 6950 5150
F 0 "#PWR01" H 6950 5150 30  0001 C C
F 1 "GND" H 6950 5080 30  0001 C C
	1    6950 5150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 457DBAF5
P 6700 5900
F 0 "#PWR02" H 6700 5900 30  0001 C C
F 1 "GND" H 6700 5830 30  0001 C C
	1    6700 5900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 457DBAF1
P 4850 5900
F 0 "#PWR03" H 4850 5900 30  0001 C C
F 1 "GND" H 4850 5830 30  0001 C C
	1    4850 5900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 457DBAEF
P 4450 5900
F 0 "#PWR04" H 4450 5900 30  0001 C C
F 1 "GND" H 4450 5830 30  0001 C C
	1    4450 5900
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG05
U 1 1 457DBAC0
P 4100 5250
F 0 "#FLG05" H 4100 5520 30  0001 C C
F 1 "PWR_FLAG" H 4100 5480 30  0000 C C
	1    4100 5250
	0    -1   -1   0   
$EndComp
$Comp
L CONN_2 P4
U 1 1 456A8ACC
P 9950 6700
F 0 "P4" V 9900 6700 40  0000 C C
F 1 "CONN_2" V 10000 6700 40  0000 C C
	1    9950 6700
	0    1    1    0   
$EndComp
$Comp
L C C1
U 1 1 4549F4BE
P 6700 4050
F 0 "C1" H 6750 4150 50  0000 L C
F 1 "10uF" H 6450 4150 50  0000 L C
	1    6700 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 4549F4B9
P 4150 5300
F 0 "#PWR06" H 4150 5300 30  0001 C C
F 1 "GND" H 4150 5230 30  0001 C C
	1    4150 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 4549F4B3
P 6950 4350
F 0 "#PWR07" H 6950 4350 30  0001 C C
F 1 "GND" H 6950 4280 30  0001 C C
	1    6950 4350
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 4549F4A5
P 7300 3950
F 0 "P3" V 7250 3950 40  0000 C C
F 1 "POWER" V 7350 3950 40  0000 C C
	1    7300 3950
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P2
U 1 1 4549F46C
P 7300 4900
F 0 "P2" V 7250 4900 40  0000 C C
F 1 "OUT" V 7350 4900 40  0000 C C
	1    7300 4900
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P1
U 1 1 4549F464
P 3800 4900
F 0 "P1" V 3750 4900 40  0000 C C
F 1 "IN" V 3850 4900 40  0000 C C
	1    3800 4900
	-1   0    0    1   
$EndComp
$Comp
L C C2
U 1 1 4549F3BE
P 6500 4800
F 0 "C2" H 6550 4900 50  0000 L C
F 1 "680nF" H 6550 4700 50  0000 L C
	1    6500 4800
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 4549F3AD
P 6700 5650
F 0 "R3" V 6780 5650 50  0000 C C
F 1 "100K" V 6700 5650 50  0000 C C
	1    6700 5650
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 4549F3A2
P 4450 5650
F 0 "R4" V 4530 5650 50  0000 C C
F 1 "47K" V 4450 5650 50  0000 C C
	1    4450 5650
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 4549F39D
P 4850 5650
F 0 "R2" V 4930 5650 50  0000 C C
F 1 "1.5K" V 4850 5650 50  0000 C C
	1    4850 5650
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 4549F38A
P 6300 4250
F 0 "R1" V 6380 4250 50  0000 C C
F 1 "1.5K" V 6300 4250 50  0000 C C
	1    6300 4250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
