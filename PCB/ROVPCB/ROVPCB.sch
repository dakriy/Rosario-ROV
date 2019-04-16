EESchema Schematic File Version 4
LIBS:ROVPCB-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "15 nov 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ROVPCB-rescue:+5V-power-ROVPCB-rescue #PWR01
U 1 1 580C1B61
P 3100 950
F 0 "#PWR01" H 3100 800 50  0001 C CNN
F 1 "+5V" H 3100 1090 50  0000 C CNN
F 2 "" H 3100 950 50  0000 C CNN
F 3 "" H 3100 950 50  0000 C CNN
	1    3100 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 950  3100 1100
Wire Wire Line
	3100 1100 2900 1100
Wire Wire Line
	3100 1200 2900 1200
Connection ~ 3100 1100
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR02
U 1 1 580C1D11
P 3000 3150
F 0 "#PWR02" H 3000 2900 50  0001 C CNN
F 1 "GND" H 3000 3000 50  0000 C CNN
F 2 "" H 3000 3150 50  0000 C CNN
F 3 "" H 3000 3150 50  0000 C CNN
	1    3000 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 1300 3000 1700
Wire Wire Line
	3000 2700 2900 2700
Wire Wire Line
	3000 2500 2900 2500
Connection ~ 3000 2700
Wire Wire Line
	3000 2000 2900 2000
Connection ~ 3000 2500
Wire Wire Line
	3000 1700 2900 1700
Connection ~ 3000 2000
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR03
U 1 1 580C1E01
P 2300 3150
F 0 "#PWR03" H 2300 2900 50  0001 C CNN
F 1 "GND" H 2300 3000 50  0000 C CNN
F 2 "" H 2300 3150 50  0000 C CNN
F 3 "" H 2300 3150 50  0000 C CNN
	1    2300 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 3000 2400 3000
Wire Wire Line
	2300 1500 2300 2300
Wire Wire Line
	2300 2300 2400 2300
Connection ~ 2300 3000
Connection ~ 2200 1100
Wire Wire Line
	2200 1900 2400 1900
Wire Wire Line
	2200 1100 2400 1100
Wire Wire Line
	2200 950  2200 1100
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue #PWR04
U 1 1 580C1BC1
P 2200 950
F 0 "#PWR04" H 2200 800 50  0001 C CNN
F 1 "+3.3V" H 2200 1090 50  0000 C CNN
F 2 "" H 2200 950 50  0000 C CNN
F 3 "" H 2200 950 50  0000 C CNN
	1    2200 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1500 2400 1500
Connection ~ 2300 2300
Wire Wire Line
	2400 1200 1250 1200
Wire Wire Line
	1250 1300 2400 1300
Wire Wire Line
	1250 1400 2400 1400
Wire Wire Line
	2400 1600 1250 1600
Wire Wire Line
	1250 1700 2400 1700
Wire Wire Line
	1250 1800 2400 1800
Wire Wire Line
	2400 2000 1250 2000
Wire Wire Line
	1250 2100 2400 2100
Wire Wire Line
	1250 2200 2400 2200
Wire Wire Line
	2400 2400 1250 2400
Wire Wire Line
	1250 2500 2400 2500
Wire Wire Line
	1250 2600 2400 2600
Wire Wire Line
	2400 2700 1250 2700
Wire Wire Line
	1250 2800 2400 2800
Wire Wire Line
	1250 2900 2400 2900
Wire Wire Line
	2900 2800 3950 2800
Wire Wire Line
	2900 2900 3950 2900
Wire Wire Line
	2900 2300 3950 2300
Wire Wire Line
	2900 2400 3950 2400
Wire Wire Line
	2900 2100 3950 2100
Wire Wire Line
	2900 2200 3950 2200
Wire Wire Line
	2900 1800 3950 1800
Wire Wire Line
	2900 1900 3950 1900
Wire Wire Line
	2900 1500 3950 1500
Wire Wire Line
	2900 1600 3950 1600
Wire Wire Line
	2900 1400 3950 1400
Wire Wire Line
	2900 2600 3950 2600
Text Label 1250 1200 0    50   ~ 0
GPIO2(SDA1)
Text Label 1250 1300 0    50   ~ 0
GPIO3(SCL1)
Text Label 1250 1400 0    50   ~ 0
GPIO4(GCLK)
Text Label 1250 1600 0    50   ~ 0
GPIO17(GEN0)
Text Label 1250 1700 0    50   ~ 0
GPIO27(GEN2)
Text Label 1250 1800 0    50   ~ 0
GPIO22(GEN3)
Text Label 1250 2000 0    50   ~ 0
GPIO10(SPI0_MOSI)
Text Label 1250 2100 0    50   ~ 0
GPIO9(SPI0_MISO)
Text Label 1250 2200 0    50   ~ 0
GPIO11(SPI0_SCK)
Text Label 1250 2400 0    50   ~ 0
ID_SD
Text Label 1250 2500 0    50   ~ 0
GPIO5
Text Label 1250 2600 0    50   ~ 0
GPIO6
Text Label 1250 2700 0    50   ~ 0
GPIO13(PWM1)
Text Label 1250 2800 0    50   ~ 0
GPIO19(SPI1_MISO)
Text Label 1250 2900 0    50   ~ 0
GPIO26
Text Label 3950 2900 2    50   ~ 0
GPIO20(SPI1_MOSI)
Text Label 3950 2800 2    50   ~ 0
GPIO16
Text Label 3950 2600 2    50   ~ 0
GPIO12(PWM0)
Text Label 3950 2400 2    50   ~ 0
ID_SC
Text Label 3950 2300 2    50   ~ 0
GPIO7(SPI1_CE_N)
Text Label 3950 2200 2    50   ~ 0
GPIO8(SPI0_CE_N)
Text Label 3950 2100 2    50   ~ 0
GPIO25(GEN6)
Text Label 3950 1900 2    50   ~ 0
GPIO24(GEN5)
Text Label 3950 1800 2    50   ~ 0
GPIO23(GEN4)
Text Label 3950 1600 2    50   ~ 0
GPIO18(GEN1)(PWM0)
Text Label 3950 1500 2    50   ~ 0
GPIO15(RXD0)
Text Label 3950 1400 2    50   ~ 0
GPIO14(TXD0)
Wire Wire Line
	3000 1300 2900 1300
Connection ~ 3000 1700
Text Notes 650  7600 0    50   ~ 0
ID_SD and ID_SC PINS:\nThese pins are reserved for HAT ID EEPROM.\n\nAt boot time this I2C interface will be\ninterrogated to look for an EEPROM\nthat identifes the attached board and\nallows automagic setup of the GPIOs\n(and optionally, Linux drivers).\n\nDO NOT USE these pins for anything other\nthan attaching an I2C ID EEPROM. Leave\nunconnected if ID EEPROM not required.
$Comp
L ROVPCB-rescue:Mounting_Hole-Mechanical-ROVPCB-rescue MK1
U 1 1 5834FB2E
P 3000 7200
F 0 "MK1" H 3100 7246 50  0000 L CNN
F 1 "M2.5" H 3100 7155 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm_M2.5" H 3000 7200 60  0001 C CNN
F 3 "" H 3000 7200 60  0001 C CNN
	1    3000 7200
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:Mounting_Hole-Mechanical-ROVPCB-rescue MK3
U 1 1 5834FBEF
P 3450 7200
F 0 "MK3" H 3550 7246 50  0000 L CNN
F 1 "M2.5" H 3550 7155 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm_M2.5" H 3450 7200 60  0001 C CNN
F 3 "" H 3450 7200 60  0001 C CNN
	1    3450 7200
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:Mounting_Hole-Mechanical-ROVPCB-rescue MK2
U 1 1 5834FC19
P 3000 7400
F 0 "MK2" H 3100 7446 50  0000 L CNN
F 1 "M2.5" H 3100 7355 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm_M2.5" H 3000 7400 60  0001 C CNN
F 3 "" H 3000 7400 60  0001 C CNN
	1    3000 7400
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:Mounting_Hole-Mechanical-ROVPCB-rescue MK4
U 1 1 5834FC4F
P 3450 7400
F 0 "MK4" H 3550 7446 50  0000 L CNN
F 1 "M2.5" H 3550 7355 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm_M2.5" H 3450 7400 60  0001 C CNN
F 3 "" H 3450 7400 60  0001 C CNN
	1    3450 7400
	1    0    0    -1  
$EndComp
Text Notes 3000 7050 0    50   ~ 0
Mounting Holes
$Comp
L ROVPCB-rescue:Conn_02x20_Odd_Even-Connector_Generic-ROVPCB-rescue P1
U 1 1 59AD464A
P 2600 2000
F 0 "P1" H 2650 3117 50  0000 C CNN
F 1 "Conn_02x20_Odd_Even" H 2650 3026 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x20_P2.54mm_Vertical" H -2250 1050 50  0001 C CNN
F 3 "" H -2250 1050 50  0001 C CNN
	1    2600 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3000 3950 3000
Text Label 3950 3000 2    50   ~ 0
GPIO21(SPI1_SCK)
Wire Wire Line
	3100 1100 3100 1200
Wire Wire Line
	3000 2700 3000 3150
Wire Wire Line
	3000 2500 3000 2700
Wire Wire Line
	3000 2000 3000 2500
Wire Wire Line
	2300 3000 2300 3150
Wire Wire Line
	2200 1100 2200 1900
Wire Wire Line
	2300 2300 2300 3000
Wire Wire Line
	3000 1700 3000 2000
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 5C61CD30
P 6900 1650
F 0 "J1" H 7006 1928 50  0000 C CNN
F 1 "Conn_01x04_Male" H 7006 1837 50  0000 C CNN
F 2 "Connector_Hirose:Hirose_DF13-04P-1.25DSA_1x04_P1.25mm_Vertical" H 6900 1650 50  0001 C CNN
F 3 "~" H 6900 1650 50  0001 C CNN
	1    6900 1650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J2
U 1 1 5C61CDA2
P 6900 2150
F 0 "J2" H 7006 2428 50  0000 C CNN
F 1 "Conn_01x04_Male" H 7006 2337 50  0000 C CNN
F 2 "Connector_Hirose:Hirose_DF13-04P-1.25DSA_1x04_P1.25mm_Vertical" H 6900 2150 50  0001 C CNN
F 3 "~" H 6900 2150 50  0001 C CNN
	1    6900 2150
	1    0    0    -1  
$EndComp
Text Notes 6550 1150 0    157  ~ 31
Sensor Connectors
Text Label 8050 1650 2    50   ~ 0
GPIO2(SDA1)
Text Label 8050 1750 2    50   ~ 0
GPIO3(SCL1)
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR06
U 1 1 5C61D5CB
P 8400 2350
F 0 "#PWR06" H 8400 2100 50  0001 C CNN
F 1 "GND" H 8400 2200 50  0000 C CNN
F 2 "" H 8400 2350 50  0000 C CNN
F 3 "" H 8400 2350 50  0000 C CNN
	1    8400 2350
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue #PWR05
U 1 1 5C61D5E4
P 8150 1450
F 0 "#PWR05" H 8150 1300 50  0001 C CNN
F 1 "+3.3V" H 8150 1590 50  0000 C CNN
F 2 "" H 8150 1450 50  0000 C CNN
F 3 "" H 8150 1450 50  0000 C CNN
	1    8150 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 1850 8400 1850
Wire Wire Line
	8400 1850 8400 2350
Wire Wire Line
	7100 2350 8400 2350
Connection ~ 8400 2350
Wire Wire Line
	7100 1550 8150 1550
Wire Wire Line
	8150 1550 8150 1450
Wire Wire Line
	7100 2050 8150 2050
Wire Wire Line
	8150 2050 8150 1550
Connection ~ 8150 1550
Text Label 8050 2150 2    50   ~ 0
GPIO2(SDA1)
Text Label 8050 2250 2    50   ~ 0
GPIO3(SCL1)
Wire Wire Line
	8050 1650 7100 1650
Wire Wire Line
	7100 1750 8050 1750
Wire Wire Line
	8050 2150 7100 2150
Wire Wire Line
	7100 2250 8050 2250
$Comp
L Sensor_Motion:MPU-6050 U1
U 1 1 5C61FB5F
P 2550 5350
F 0 "U1" H 2550 4564 50  0000 C CNN
F 1 "MPU-6050" H 2550 4473 50  0000 C CNN
F 2 "Sensor_Motion:InvenSense_QFN-24_4x4mm_P0.5mm" H 2550 4550 50  0001 C CNN
F 3 "https://store.invensense.com/datasheets/invensense/MPU-6050_DataSheet_V3%204.pdf" H 2550 5200 50  0001 C CNN
	1    2550 5350
	1    0    0    -1  
$EndComp
Text Notes 1650 4050 0    157  ~ 31
Accelerometer
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0101
U 1 1 5C61FCB6
P 1700 5700
F 0 "#PWR0101" H 1700 5450 50  0001 C CNN
F 1 "GND" H 1700 5550 50  0000 C CNN
F 2 "" H 1700 5700 50  0000 C CNN
F 3 "" H 1700 5700 50  0000 C CNN
	1    1700 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 5550 1700 5550
Wire Wire Line
	1700 5550 1700 5650
Wire Wire Line
	1850 5650 1800 5650
Connection ~ 1700 5650
Wire Wire Line
	1700 5650 1700 5700
$Comp
L Device:C C2
U 1 1 5C622699
P 3450 5800
F 0 "C2" H 3565 5846 50  0000 L CNN
F 1 "0.1uF" H 3565 5755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3488 5650 50  0001 C CNN
F 3 "~" H 3450 5800 50  0001 C CNN
	1    3450 5800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5C622707
P 3900 5800
F 0 "C3" H 4015 5846 50  0000 L CNN
F 1 "2.2 nF" H 4015 5755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3938 5650 50  0001 C CNN
F 3 "~" H 3900 5800 50  0001 C CNN
	1    3900 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 5650 3450 5650
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0102
U 1 1 5C62556F
P 3450 5950
F 0 "#PWR0102" H 3450 5700 50  0001 C CNN
F 1 "GND" H 3450 5800 50  0000 C CNN
F 2 "" H 3450 5950 50  0000 C CNN
F 3 "" H 3450 5950 50  0000 C CNN
	1    3450 5950
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0103
U 1 1 5C62558E
P 3900 5950
F 0 "#PWR0103" H 3900 5700 50  0001 C CNN
F 1 "GND" H 3900 5800 50  0000 C CNN
F 2 "" H 3900 5950 50  0000 C CNN
F 3 "" H 3900 5950 50  0000 C CNN
	1    3900 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 5550 3900 5650
Wire Wire Line
	3250 5550 3900 5550
$Comp
L Device:C C1
U 1 1 5C626E58
P 2000 4450
F 0 "C1" H 2115 4496 50  0000 L CNN
F 1 "10nF" H 2115 4405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2038 4300 50  0001 C CNN
F 3 "~" H 2000 4450 50  0001 C CNN
	1    2000 4450
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0104
U 1 1 5C626ED8
P 2000 4600
F 0 "#PWR0104" H 2000 4350 50  0001 C CNN
F 1 "GND" H 2000 4450 50  0000 C CNN
F 2 "" H 2000 4600 50  0000 C CNN
F 3 "" H 2000 4600 50  0000 C CNN
	1    2000 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 4650 2450 4200
Wire Wire Line
	2450 4200 2000 4200
Wire Wire Line
	2000 4200 2000 4300
Wire Wire Line
	1850 5250 1700 5250
Wire Wire Line
	1700 5250 1700 5550
Connection ~ 1700 5550
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue #PWR0105
U 1 1 5C62A4AF
P 2650 4400
F 0 "#PWR0105" H 2650 4250 50  0001 C CNN
F 1 "+3.3V" H 2650 4540 50  0000 C CNN
F 2 "" H 2650 4400 50  0000 C CNN
F 3 "" H 2650 4400 50  0000 C CNN
	1    2650 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 4650 2650 4400
NoConn ~ 3250 5250
NoConn ~ 3250 5050
NoConn ~ 3250 5350
Text Label 1300 5050 0    50   ~ 0
GPIO2(SDA1)
Text Label 1300 5150 0    50   ~ 0
GPIO3(SCL1)
Wire Wire Line
	1300 5150 1850 5150
Wire Wire Line
	1850 5050 1300 5050
Wire Wire Line
	2550 6050 1800 6050
Wire Wire Line
	1800 6050 1800 5650
Connection ~ 1800 5650
Wire Wire Line
	1800 5650 1700 5650
$EndSCHEMATC
