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
	3100 950  3100 1000
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
Text Notes 1600 3750 0    157  ~ 31
Accelerometer
$Comp
L Device:C C2
U 1 1 5C622699
P 3050 4300
F 0 "C2" H 3165 4346 50  0000 L CNN
F 1 "0.1uF" H 3165 4255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3088 4150 50  0001 C CNN
F 3 "~" H 3050 4300 50  0001 C CNN
	1    3050 4300
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0102
U 1 1 5C62556F
P 3050 4450
F 0 "#PWR0102" H 3050 4200 50  0001 C CNN
F 1 "GND" H 3050 4300 50  0000 C CNN
F 2 "" H 3050 4450 50  0000 C CNN
F 3 "" H 3050 4450 50  0000 C CNN
	1    3050 4450
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0103
U 1 1 5C62558E
P 3050 6200
F 0 "#PWR0103" H 3050 5950 50  0001 C CNN
F 1 "GND" H 3050 6050 50  0000 C CNN
F 2 "" H 3050 6200 50  0000 C CNN
F 3 "" H 3050 6200 50  0000 C CNN
	1    3050 6200
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue #PWR0105
U 1 1 5C62A4AF
P 3050 4050
F 0 "#PWR0105" H 3050 3900 50  0001 C CNN
F 1 "+3.3V" H 3050 4190 50  0000 C CNN
F 2 "" H 3050 4050 50  0000 C CNN
F 3 "" H 3050 4050 50  0000 C CNN
	1    3050 4050
	1    0    0    -1  
$EndComp
NoConn ~ 3150 5200
NoConn ~ 3150 5300
Text Label 1200 5000 0    50   ~ 0
GPIO2(SDA1)
Text Label 1200 5200 0    50   ~ 0
GPIO3(SCL1)
$Comp
L Sensor_Motion:MPU-9250 U1
U 1 1 5CB598AD
P 2450 5300
F 0 "U1" H 2450 4314 50  0000 C CNN
F 1 "MPU-9250" H 2450 4223 50  0000 C CNN
F 2 "Sensor_Motion:InvenSense_QFN-24_3x3mm_P0.4mm" H 2450 4300 50  0001 C CNN
F 3 "https://store.invensense.com/datasheets/invensense/MPU9250REV1.0.pdf" H 2450 5150 50  0001 C CNN
	1    2450 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 5200 1200 5200
Wire Wire Line
	3050 4050 3050 4100
Wire Wire Line
	2550 4400 2550 4100
Wire Wire Line
	2550 4100 3050 4100
Connection ~ 3050 4100
Wire Wire Line
	3050 4100 3050 4150
Wire Wire Line
	2350 4400 2350 4100
Wire Wire Line
	2350 4100 2550 4100
Connection ~ 2550 4100
$Comp
L Device:C C1
U 1 1 5CB64739
P 3600 6000
F 0 "C1" H 3715 6046 50  0000 L CNN
F 1 "0.1uF" H 3715 5955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3638 5850 50  0001 C CNN
F 3 "~" H 3600 6000 50  0001 C CNN
	1    3600 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 6150 3600 6200
Wire Wire Line
	3600 6200 3050 6200
Connection ~ 3050 6200
Wire Wire Line
	3050 6200 2450 6200
Wire Wire Line
	3600 5850 3600 5500
Wire Wire Line
	3600 5500 3150 5500
Wire Wire Line
	3050 4100 3500 4100
Wire Wire Line
	3500 4100 3500 5600
Wire Wire Line
	3500 5600 3150 5600
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0101
U 1 1 5CB6C0D3
P 3350 5700
F 0 "#PWR0101" H 3350 5450 50  0001 C CNN
F 1 "GND" H 3350 5550 50  0000 C CNN
F 2 "" H 3350 5700 50  0000 C CNN
F 3 "" H 3350 5700 50  0000 C CNN
	1    3350 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 5700 3350 5700
Wire Wire Line
	2350 4100 1600 4100
Wire Wire Line
	1600 4100 1600 5300
Wire Wire Line
	1600 5300 1750 5300
Connection ~ 2350 4100
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue #PWR0104
U 1 1 5CB71953
P 1000 4800
F 0 "#PWR0104" H 1000 4650 50  0001 C CNN
F 1 "+3.3V" H 1000 4940 50  0000 C CNN
F 2 "" H 1000 4800 50  0000 C CNN
F 3 "" H 1000 4800 50  0000 C CNN
	1    1000 4800
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0106
U 1 1 5CB74877
P 1650 5500
F 0 "#PWR0106" H 1650 5250 50  0001 C CNN
F 1 "GND" H 1650 5350 50  0000 C CNN
F 2 "" H 1650 5500 50  0000 C CNN
F 3 "" H 1650 5500 50  0000 C CNN
	1    1650 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 5500 1650 5500
Wire Wire Line
	1200 5000 1750 5000
NoConn ~ 3150 5000
$Comp
L Sensor:BME280 U2
U 1 1 5CB85C79
P 5200 6700
F 0 "U2" H 4770 6746 50  0000 R CNN
F 1 "BME280" H 4770 6655 50  0000 R CNN
F 2 "Package_LGA:Bosch_LGA-8_2.5x2.5mm_P0.65mm_ClockwisePinNumbering" H 5200 6500 50  0001 C CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-11.pdf" H 5200 6500 50  0001 C CNN
	1    5200 6700
	1    0    0    -1  
$EndComp
Text Notes 4250 5350 0    138  ~ 28
Temp/Press/Humid
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0107
U 1 1 5CB8835A
P 5200 7300
F 0 "#PWR0107" H 5200 7050 50  0001 C CNN
F 1 "GND" H 5200 7150 50  0000 C CNN
F 2 "" H 5200 7300 50  0000 C CNN
F 3 "" H 5200 7300 50  0000 C CNN
	1    5200 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 7300 5200 7300
Connection ~ 5200 7300
Wire Wire Line
	5200 7300 5300 7300
$Comp
L Device:C C3
U 1 1 5CB8AA65
P 4600 6050
F 0 "C3" H 4715 6096 50  0000 L CNN
F 1 "0.1uF" H 4715 6005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4638 5900 50  0001 C CNN
F 3 "~" H 4600 6050 50  0001 C CNN
	1    4600 6050
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0108
U 1 1 5CB8AB73
P 4600 6200
F 0 "#PWR0108" H 4600 5950 50  0001 C CNN
F 1 "GND" H 4600 6050 50  0000 C CNN
F 2 "" H 4600 6200 50  0000 C CNN
F 3 "" H 4600 6200 50  0000 C CNN
	1    4600 6200
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue #PWR0109
U 1 1 5CB8D386
P 5200 5800
F 0 "#PWR0109" H 5200 5650 50  0001 C CNN
F 1 "+3.3V" H 5200 5940 50  0000 C CNN
F 2 "" H 5200 5800 50  0000 C CNN
F 3 "" H 5200 5800 50  0000 C CNN
	1    5200 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 6100 5200 6100
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue #PWR0110
U 1 1 5CB8FE0A
P 5950 7000
F 0 "#PWR0110" H 5950 6850 50  0001 C CNN
F 1 "+3.3V" H 5950 7140 50  0000 C CNN
F 2 "" H 5950 7000 50  0000 C CNN
F 3 "" H 5950 7000 50  0000 C CNN
	1    5950 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 7000 5950 7000
Wire Wire Line
	5200 5800 5200 5900
Connection ~ 5200 6100
Wire Wire Line
	5200 6100 5300 6100
Wire Wire Line
	5200 5900 4600 5900
Connection ~ 5200 5900
Wire Wire Line
	5200 5900 5200 6000
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR0111
U 1 1 5CB97771
P 6100 6250
F 0 "#PWR0111" H 6100 6000 50  0001 C CNN
F 1 "GND" H 6100 6100 50  0000 C CNN
F 2 "" H 6100 6250 50  0000 C CNN
F 3 "" H 6100 6250 50  0000 C CNN
	1    6100 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 6600 5800 6600
Wire Wire Line
	6500 6800 5800 6800
Wire Wire Line
	6100 6250 5800 6250
Wire Wire Line
	5800 6250 5800 6400
Text Label 6500 6800 2    50   ~ 0
GPIO2(SDA1)
Text Label 6500 6600 2    50   ~ 0
GPIO3(SCL1)
Text Notes 4200 3750 0    138  ~ 28
Lux
$Comp
L Adafruit_TSL2591-eagle-import:LIGHT_TSL2591 U3
U 1 1 5CB6F38A
P 5350 4300
F 0 "U3" H 5577 4290 42  0000 L CNN
F 1 "LIGHT_TSL2591" H 5577 4211 42  0000 L CNN
F 2 "Adafruit TSL2591:DFN6_TAOS" H 5350 4300 50  0001 C CNN
F 3 "" H 5350 4300 50  0001 C CNN
	1    5350 4300
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue #PWR07
U 1 1 5CB72608
P 4600 4500
F 0 "#PWR07" H 4600 4350 50  0001 C CNN
F 1 "+3.3V" H 4600 4640 50  0000 C CNN
F 2 "" H 4600 4500 50  0000 C CNN
F 3 "" H 4600 4500 50  0000 C CNN
	1    4600 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 4500 4950 4500
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue #PWR08
U 1 1 5CB7546E
P 4800 4600
F 0 "#PWR08" H 4800 4350 50  0001 C CNN
F 1 "GND" H 4800 4450 50  0000 C CNN
F 2 "" H 4800 4600 50  0000 C CNN
F 3 "" H 4800 4600 50  0000 C CNN
	1    4800 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4600 4800 4600
Text Label 4450 4100 0    50   ~ 0
GPIO2(SDA1)
Wire Wire Line
	4950 4100 4450 4100
Text Label 4450 4200 0    50   ~ 0
GPIO3(SCL1)
Wire Wire Line
	4450 4200 4950 4200
NoConn ~ 4950 4300
NoConn ~ 4950 4400
NoConn ~ 3950 1400
NoConn ~ 3950 1500
NoConn ~ 3950 1600
NoConn ~ 3950 1800
NoConn ~ 3950 1900
NoConn ~ 3950 2100
NoConn ~ 3950 2200
NoConn ~ 3950 2300
NoConn ~ 3950 2400
NoConn ~ 3950 2600
NoConn ~ 3950 2800
NoConn ~ 3950 2900
NoConn ~ 3950 3000
NoConn ~ 1250 2900
NoConn ~ 1250 2800
NoConn ~ 1250 2700
NoConn ~ 1250 2600
NoConn ~ 1250 2500
NoConn ~ 1250 2400
NoConn ~ 1250 2200
NoConn ~ 1250 2100
NoConn ~ 1250 2000
NoConn ~ 1250 1800
NoConn ~ 1250 1700
NoConn ~ 1250 1600
NoConn ~ 1250 1400
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5CBD550D
P 5200 6000
F 0 "#FLG0101" H 5200 6075 50  0001 C CNN
F 1 "PWR_FLAG" V 5200 6128 50  0000 L CNN
F 2 "" H 5200 6000 50  0001 C CNN
F 3 "~" H 5200 6000 50  0001 C CNN
	1    5200 6000
	0    1    1    0   
$EndComp
Connection ~ 5200 6000
Wire Wire Line
	5200 6000 5200 6100
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5CBD5948
P 3100 1000
F 0 "#FLG0102" H 3100 1075 50  0001 C CNN
F 1 "PWR_FLAG" V 3100 1128 50  0000 L CNN
F 2 "" H 3100 1000 50  0001 C CNN
F 3 "~" H 3100 1000 50  0001 C CNN
	1    3100 1000
	0    1    1    0   
$EndComp
Connection ~ 3100 1000
Wire Wire Line
	3100 1000 3100 1100
$Comp
L Device:R R1
U 1 1 5CBD8F78
P 1000 4950
F 0 "R1" H 930 4904 50  0000 R CNN
F 1 "10k" H 930 4995 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 930 4950 50  0001 C CNN
F 3 "~" H 1000 4950 50  0001 C CNN
	1    1000 4950
	-1   0    0    1   
$EndComp
Wire Wire Line
	1000 5100 1750 5100
Text Notes 7750 4750 0    138  ~ 28
I2C Address Translator
$Comp
L LTC4316IDDPBF:LTC4316IDDPBF U?
U 1 1 5CBEE417
P 7200 5300
F 0 "U?" H 8825 5665 50  0000 C CNN
F 1 "LTC4316IDDPBF" H 8825 5574 50  0000 C CNN
F 2 "LTC4316IDDPBF:DFN-10_DD" H 10750 4800 50  0001 L BNN
F 3 "LTC4316IDD#PBF-ND" H 10550 5300 50  0001 L BNN
F 4 "Linear Technology/Analog" H 10400 5600 50  0001 L BNN "Field4"
F 5 "LTC4316 - Single I2C/SMBus Address Translator" H 10000 5450 50  0001 L BNN "Field5"
F 6 "LTC4316IDD#PBF" H 10650 4950 50  0001 L BNN "Field7"
F 7 "DFN-10 Linear Technology" H 10450 5150 50  0001 L BNN "Field8"
	1    7200 5300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
