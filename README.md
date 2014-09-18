easymeter_mon
=============

Query EasyMeter Q3CA smart meters via IR and output in Zabbix Active Check format.

The protocol on the IR interface is [Smart Message Language (SML)](http://de.wikipedia.org/wiki/Smart_Message_Language). The messages contain the values in the [OBIS](http://de.wikipedia.org/wiki/OBIS-Kennzahlen) format.

## Acknowledgements

This project basically just puts the building blocks togehter. Thanks to:

* [libsml](https://github.com/dailab/libsml) for SML parsing
* [vzlogger](http://www.volkszaehler.org/) for obis.c
* [Sven Jordan](http://www.s-jordan.de/index.php?page=easymeter_raspberrypi) for IR reader electronics
* [Zabbix](http://www.zabbix.com/) for the montioring software

## Dependencies 

### Hardware

* EasyMeter Q3CA smart meter for solar inverters
* IR - UART Interface ([PNG](/misc/easymon_reader.png) / [SCHEME](/misc/easymon_reader.sch))

### Software

* Compilation
  * CMake
  * libsml (https://github.com/dailab/libsml)
  * Essentials build utils (git, gcc, ...)
* Data usage
  * Zabbix

## Compilation & Installation

#### Example for Debian/Ubuntu based systems

* Tested for 
 * Debian Wheezy (7) on RaspberryPi
 * Ubuntu Precise (12.04) x64
 * Q3CA

```bash
# Install dependencies
apt-get install build-essential cmake uuid-dev uuid-runtime git debhelper

# Compile & install libsml
git clone git://github.com/simonswine/libsml.git
cd libsml
dpkg-buildpackage -b
cd ..
dpkg -i libsml*deb

# Compile & install easymeter_mon
git clone git://github.com/simonswine/easymeter_mon.git
cd easymeter_mon
mkdir build 
cd build 
cmake ..
make 
make install
```

## Usage

### Parameters

| Parameter    | Default value | Description                     |
|--------------|---------------|---------------------------------|
| `-d <device>`|`/dev/ttyAMA0` |Specify serial device to query   |
| `-h <host>`  |`powermon`     |Hostname for Zabbix output format|    

### Query smart meter

```bash
easymeter_mon -z powermon -d /dev/ttyAMA0
powermon power.counter-in 8208517.71
powermon power.counter-out 6000398.39
powermon power.counter-in-t1 8202860.00
powermon power.counter-in-t2 1130.00
powermon power.counter-in-t3 1100.00
powermon power.counter-in-t4 1110.00
powermon power.counter-in-t5 1100.00
powermon power.counter-in-t6 1210.00
powermon power.counter-out-t7 5999270.00
powermon power.counter-out-t8 1120.00
powermon power.power -1561.21
powermon power.power-l1 -1815.88
powermon power.power-l2 236.09
powermon power.power-l3 18.58
```

### Send values to Zabbix

* Setup a Zabbix host which is linked to this template [Zabbix Template](/misc/zabbix_template_powermon.xml)
* Make sure this script is run on a periodic basis:
```bash
easymeter_mon -z <zabbix_host> -d /dev/ttyAMA0 | zabbix_sender -i - -z <zabbix_server>
```

### Example charts from Zabbix

* Inverter production vs. consumption of energy
![Image](/misc/production_vs_consumption.png)
* Power per phase
![Image](/misc/power_per_phase.png)


