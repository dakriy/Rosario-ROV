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
L ROVPCB-rescue:+5V-power-ROVPCB-rescue-ROVPCB-rescue #PWR01
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
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR02
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
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR03
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
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue-ROVPCB-rescue #PWR04
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
L ROVPCB-rescue:Mounting_Hole-Mechanical-ROVPCB-rescue-ROVPCB-rescue MK1
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
L ROVPCB-rescue:Mounting_Hole-Mechanical-ROVPCB-rescue-ROVPCB-rescue MK3
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
L ROVPCB-rescue:Mounting_Hole-Mechanical-ROVPCB-rescue-ROVPCB-rescue MK2
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
L ROVPCB-rescue:Mounting_Hole-Mechanical-ROVPCB-rescue-ROVPCB-rescue MK4
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
L ROVPCB-rescue:Conn_02x20_Odd_Even-Connector_Generic-ROVPCB-rescue-ROVPCB-rescue P1
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
P 4750 1500
F 0 "J1" H 4856 1778 50  0000 C CNN
F 1 "Conn_01x04_Male" H 4856 1687 50  0000 C CNN
F 2 "Connector_Hirose:Hirose_DF13-04P-1.25DSA_1x04_P1.25mm_Vertical" H 4750 1500 50  0001 C CNN
F 3 "~" H 4750 1500 50  0001 C CNN
	1    4750 1500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J2
U 1 1 5C61CDA2
P 4750 2000
F 0 "J2" H 4856 2278 50  0000 C CNN
F 1 "Conn_01x04_Male" H 4856 2187 50  0000 C CNN
F 2 "Connector_Hirose:Hirose_DF13-04P-1.25DSA_1x04_P1.25mm_Vertical" H 4750 2000 50  0001 C CNN
F 3 "~" H 4750 2000 50  0001 C CNN
	1    4750 2000
	1    0    0    -1  
$EndComp
Text Notes 4400 800  0    157  ~ 31
BlueRobotics Sensors
Text Label 5900 1500 2    50   ~ 0
GPIO2(SDA1)
Text Label 5900 1600 2    50   ~ 0
GPIO3(SCL1)
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR06
U 1 1 5C61D5CB
P 6250 2200
F 0 "#PWR06" H 6250 1950 50  0001 C CNN
F 1 "GND" H 6250 2050 50  0000 C CNN
F 2 "" H 6250 2200 50  0000 C CNN
F 3 "" H 6250 2200 50  0000 C CNN
	1    6250 2200
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue-ROVPCB-rescue #PWR05
U 1 1 5C61D5E4
P 6000 1300
F 0 "#PWR05" H 6000 1150 50  0001 C CNN
F 1 "+3.3V" H 6000 1440 50  0000 C CNN
F 2 "" H 6000 1300 50  0000 C CNN
F 3 "" H 6000 1300 50  0000 C CNN
	1    6000 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 1700 6250 1700
Wire Wire Line
	6250 1700 6250 2200
Wire Wire Line
	4950 2200 6250 2200
Connection ~ 6250 2200
Wire Wire Line
	4950 1400 6000 1400
Wire Wire Line
	6000 1400 6000 1300
Wire Wire Line
	4950 1900 6000 1900
Wire Wire Line
	6000 1900 6000 1400
Connection ~ 6000 1400
Text Label 5900 2000 2    50   ~ 0
GPIO2(SDA1)
Text Label 5900 2100 2    50   ~ 0
GPIO3(SCL1)
Wire Wire Line
	5900 1500 4950 1500
Wire Wire Line
	4950 1600 5900 1600
Wire Wire Line
	5900 2000 4950 2000
Wire Wire Line
	4950 2100 5900 2100
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
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR0102
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
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR0103
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
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue-ROVPCB-rescue #PWR0105
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
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR0101
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
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR0106
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
P 5300 5350
F 0 "U2" H 4870 5396 50  0000 R CNN
F 1 "BME280" H 4870 5305 50  0000 R CNN
F 2 "Package_LGA:Bosch_LGA-8_2.5x2.5mm_P0.65mm_ClockwisePinNumbering" H 5300 5150 50  0001 C CNN
F 3 "https://cdn-shop.adafruit.com/datasheets/BST-BME280_DS001-10.pdf" H 5300 5150 50  0001 C CNN
	1    5300 5350
	1    0    0    -1  
$EndComp
Text Notes 4350 4000 0    138  ~ 28
Temp/Press/Humid
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR0107
U 1 1 5CB8835A
P 5300 5950
F 0 "#PWR0107" H 5300 5700 50  0001 C CNN
F 1 "GND" H 5300 5800 50  0000 C CNN
F 2 "" H 5300 5950 50  0000 C CNN
F 3 "" H 5300 5950 50  0000 C CNN
	1    5300 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5950 5300 5950
Connection ~ 5300 5950
Wire Wire Line
	5300 5950 5400 5950
$Comp
L Device:C C3
U 1 1 5CB8AA65
P 4700 4700
F 0 "C3" H 4815 4746 50  0000 L CNN
F 1 "0.1uF" H 4815 4655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4738 4550 50  0001 C CNN
F 3 "~" H 4700 4700 50  0001 C CNN
	1    4700 4700
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR0108
U 1 1 5CB8AB73
P 4700 4850
F 0 "#PWR0108" H 4700 4600 50  0001 C CNN
F 1 "GND" H 4700 4700 50  0000 C CNN
F 2 "" H 4700 4850 50  0000 C CNN
F 3 "" H 4700 4850 50  0000 C CNN
	1    4700 4850
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue-ROVPCB-rescue #PWR0109
U 1 1 5CB8D386
P 5300 4450
F 0 "#PWR0109" H 5300 4300 50  0001 C CNN
F 1 "+3.3V" H 5300 4590 50  0000 C CNN
F 2 "" H 5300 4450 50  0000 C CNN
F 3 "" H 5300 4450 50  0000 C CNN
	1    5300 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 4750 5300 4750
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue-ROVPCB-rescue #PWR0110
U 1 1 5CB8FE0A
P 6050 5650
F 0 "#PWR0110" H 6050 5500 50  0001 C CNN
F 1 "+3.3V" H 6050 5790 50  0000 C CNN
F 2 "" H 6050 5650 50  0000 C CNN
F 3 "" H 6050 5650 50  0000 C CNN
	1    6050 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 5650 6050 5650
Wire Wire Line
	5300 4450 5300 4550
Connection ~ 5300 4750
Wire Wire Line
	5300 4750 5400 4750
Wire Wire Line
	5300 4550 4700 4550
Connection ~ 5300 4550
Wire Wire Line
	5300 4550 5300 4650
Wire Wire Line
	6200 4900 5900 4900
Wire Wire Line
	5900 4900 5900 5050
Text Notes 4000 6500 0    138  ~ 28
Lux
$Comp
L Adafruit_TSL2591-eagle-import:LIGHT_TSL2591 U3
U 1 1 5CB6F38A
P 5150 7050
F 0 "U3" H 5377 7040 42  0000 L CNN
F 1 "LIGHT_TSL2591" H 5377 6961 42  0000 L CNN
F 2 "Adafruit TSL2591:DFN6_TAOS" H 5150 7050 50  0001 C CNN
F 3 "" H 5150 7050 50  0001 C CNN
	1    5150 7050
	1    0    0    -1  
$EndComp
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue-ROVPCB-rescue #PWR07
U 1 1 5CB72608
P 4400 7250
F 0 "#PWR07" H 4400 7100 50  0001 C CNN
F 1 "+3.3V" H 4400 7390 50  0000 C CNN
F 2 "" H 4400 7250 50  0000 C CNN
F 3 "" H 4400 7250 50  0000 C CNN
	1    4400 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 7250 4750 7250
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR08
U 1 1 5CB7546E
P 4600 7350
F 0 "#PWR08" H 4600 7100 50  0001 C CNN
F 1 "GND" H 4600 7200 50  0000 C CNN
F 2 "" H 4600 7350 50  0000 C CNN
F 3 "" H 4600 7350 50  0000 C CNN
	1    4600 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 7350 4600 7350
Text Label 4250 6850 0    50   ~ 0
GPIO2(SDA1)
Wire Wire Line
	4750 6850 4250 6850
Text Label 4250 6950 0    50   ~ 0
GPIO3(SCL1)
Wire Wire Line
	4250 6950 4750 6950
NoConn ~ 4750 7050
NoConn ~ 4750 7150
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
P 5300 4650
F 0 "#FLG0101" H 5300 4725 50  0001 C CNN
F 1 "PWR_FLAG" V 5300 4778 50  0000 L CNN
F 2 "" H 5300 4650 50  0001 C CNN
F 3 "~" H 5300 4650 50  0001 C CNN
	1    5300 4650
	0    1    1    0   
$EndComp
Connection ~ 5300 4650
Wire Wire Line
	5300 4650 5300 4750
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
Wire Wire Line
	1000 5100 1750 5100
Text Notes 7300 4500 0    138  ~ 28
I2C Address Translator
$Comp
L LTC4316IDDPBF:LTC4316IDDPBF U4
U 1 1 5CBEE417
P 9900 5650
F 0 "U4" H 11525 6015 50  0000 C CNN
F 1 "LTC4316IDDPBF" H 11525 5924 50  0000 C CNN
F 2 "LTC4316IDDPBF:DFN-10_DD" H 13450 5150 50  0001 L BNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/4316fa.pdf" H 13250 5650 50  0001 L BNN
F 4 "Linear Technology/Analog" H 13100 5950 50  0001 L BNN "Field4"
F 5 "LTC4316 - Single I2C/SMBus Address Translator" H 12700 5800 50  0001 L BNN "Field5"
F 6 "LTC4316IDD#PBF" H 13350 5300 50  0001 L BNN "Field7"
F 7 "DFN-10 Linear Technology" H 13150 5500 50  0001 L BNN "Field8"
	1    9900 5650
	-1   0    0    1   
$EndComp
Text Notes 7600 800  0    138  ~ 28
Atlas Scientific board
Text Label 7000 5250 0    50   ~ 0
GPIO2(SDA1)
Text Label 7000 5550 0    50   ~ 0
GPIO3(SCL1)
Wire Wire Line
	7000 5550 7500 5550
Wire Wire Line
	7000 5250 7500 5250
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR0111
U 1 1 5CB8FA80
P 7500 6000
F 0 "#PWR0111" H 7500 5750 50  0001 C CNN
F 1 "GND" H 7500 5850 50  0000 C CNN
F 2 "" H 7500 6000 50  0000 C CNN
F 3 "" H 7500 6000 50  0000 C CNN
	1    7500 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 5650 7500 6000
Wire Wire Line
	7500 5350 6300 5350
Wire Wire Line
	6300 5350 6300 5450
Wire Wire Line
	6300 5450 5900 5450
Wire Wire Line
	5900 5250 6750 5250
Wire Wire Line
	6750 5250 6750 5450
Wire Wire Line
	6750 5450 7500 5450
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR0112
U 1 1 5CBA3506
P 9250 5550
F 0 "#PWR0112" H 9250 5300 50  0001 C CNN
F 1 "GND" H 9250 5400 50  0000 C CNN
F 2 "" H 9250 5550 50  0000 C CNN
F 3 "" H 9250 5550 50  0000 C CNN
	1    9250 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 5550 9150 5550
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue-ROVPCB-rescue #PWR0113
U 1 1 5CBA6BEC
P 9350 5350
F 0 "#PWR0113" H 9350 5200 50  0001 C CNN
F 1 "+3.3V" H 9350 5490 50  0000 C CNN
F 2 "" H 9350 5350 50  0000 C CNN
F 3 "" H 9350 5350 50  0000 C CNN
	1    9350 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 5350 9200 5350
Wire Wire Line
	9200 5350 9200 5450
Wire Wire Line
	9200 5450 9050 5450
Connection ~ 9200 5350
Wire Wire Line
	9200 5350 9150 5350
Wire Wire Line
	9050 5650 9150 5650
Wire Wire Line
	9150 5650 9150 5550
Connection ~ 9150 5550
Wire Wire Line
	9150 5550 9050 5550
Text Notes 8850 6000 0    50   ~ 0
SDAIN: 0x67 --> Translation Byte: 0xF --> SDAOUT: 0x76
Text Notes 9550 5700 0    50   ~ 0
The translation byte is set by \nxorh and xorl according to \nthe datasheet pg. 9. This layout\nsets it to 0x0F. Sample conversion\nshown below.
Text Notes 4400 4150 0    50   ~ 0
Default I2C address: 0x77 (SDO--Vdd), 0x76 (SDO--GND)
Text Notes 1400 3850 0    50   ~ 0
Default I2C address: 0x68(AD0--Vdd), 0x69(AD0--GND)
Text Notes 3750 6600 0    50   ~ 0
Default I2C address: 0x29
Text Notes 8050 900  0    50   ~ 0
Default I2C address: 0x64
Text Notes 4550 900  0    50   ~ 0
Default I2C address--Temp: 0x77; Pressure: 0x76
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR0114
U 1 1 5CBDF2B2
P 6200 4900
F 0 "#PWR0114" H 6200 4650 50  0001 C CNN
F 1 "GND" H 6200 4750 50  0000 C CNN
F 2 "" H 6200 4900 50  0000 C CNN
F 3 "" H 6200 4900 50  0000 C CNN
	1    6200 4900
	1    0    0    -1  
$EndComp
$Comp
L Adafruit_TSL2591-eagle-import:AtlasOEM U5
U 1 1 5CBE687D
P 8650 1400
F 0 "U5" H 8650 1775 50  0000 C CNN
F 1 "AtlasOEM" H 8650 1684 50  0000 C CNN
F 2 "Adafruit TSL2591:OEM Board" H 8650 1400 50  0001 C CNN
F 3 "https://www.atlas-scientific.com/_files/_datasheets/_oem/EC_oem_datasheet.pdf" H 8650 1400 50  0001 C CNN
	1    8650 1400
	1    0    0    -1  
$EndComp
NoConn ~ 9000 1250
NoConn ~ 9000 1350
NoConn ~ 8300 1350
Text Label 7800 1250 0    50   ~ 0
GPIO2(SDA1)
Wire Wire Line
	7800 1250 8300 1250
Text Label 9550 1450 2    50   ~ 0
GPIO3(SCL1)
Wire Wire Line
	9550 1450 9000 1450
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR010
U 1 1 5CBFDF85
P 9250 1650
F 0 "#PWR010" H 9250 1400 50  0001 C CNN
F 1 "GND" H 9250 1500 50  0000 C CNN
F 2 "" H 9250 1650 50  0000 C CNN
F 3 "" H 9250 1650 50  0000 C CNN
	1    9250 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1650 9000 1650
$Comp
L ROVPCB-rescue:+3.3V-power-ROVPCB-rescue-ROVPCB-rescue #PWR09
U 1 1 5CC01F57
P 7750 1450
F 0 "#PWR09" H 7750 1300 50  0001 C CNN
F 1 "+3.3V" H 7750 1590 50  0000 C CNN
F 2 "" H 7750 1450 50  0000 C CNN
F 3 "" H 7750 1450 50  0000 C CNN
	1    7750 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 1450 8300 1450
NoConn ~ 9000 1550
Wire Wire Line
	8300 1550 7900 1550
Wire Wire Line
	8300 1650 8300 1750
Wire Wire Line
	8300 1750 8100 1750
$Comp
L ROVPCB-rescue:GND-power-ROVPCB-rescue-ROVPCB-rescue #PWR011
U 1 1 5CBE8F10
P 1000 5100
F 0 "#PWR011" H 1000 4850 50  0001 C CNN
F 1 "GND" H 1000 4950 50  0000 C CNN
F 2 "" H 1000 5100 50  0000 C CNN
F 3 "" H 1000 5100 50  0000 C CNN
	1    1000 5100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5CBED62E
P 7500 1750
F 0 "J3" H 7606 1928 50  0000 C CNN
F 1 "Conn_01x02_Male" H 7606 1837 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x02_P3.81mm_Drill0.8mm" H 7500 1750 50  0001 C CNN
F 3 "~" H 7500 1750 50  0001 C CNN
	1    7500 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 1550 7900 1750
Wire Wire Line
	7900 1750 7700 1750
Wire Wire Line
	8100 1750 8100 1850
Wire Wire Line
	8100 1850 7700 1850
Wire Wire Line
	9050 5250 9150 5250
Wire Wire Line
	9150 5250 9150 5350
Connection ~ 9150 5350
Wire Wire Line
	9150 5350 9050 5350
NoConn ~ 7500 5150
$EndSCHEMATC
