easymeter_mon
=============

Query EasyMeter Q3CXX smart meters via IR and output in Zabbix Active Check format.

The protocol on the IR interface is [Smart Message Language (SML)](http://de.wikipedia.org/wiki/Smart_Message_Language). The messages contain the values in the [OBIS](http://de.wikipedia.org/wiki/OBIS-Kennzahlen) format.

## Acknowledgements

This project basically just puts the building blocks togehter. Thanks to:

* [libsml](https://github.com/dailab/libsml) for SML parsing
* [vzlogger](http://www.volkszaehler.org/) for obis.c
* [Sven Jordan](http://www.s-jordan.de/index.php?page=easymeter_raspberrypi) for IR reader electronics


## Dependencies 

### Hardware

* EasyMeter Q3XX smart meter
* IR - UART Interface (cf. misc/xx)

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

```shell
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



### Send values to zabbix

```
```
