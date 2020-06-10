# Dependency
- DPDK 20.02(latest version is recommended)
    - To install https://doc.dpdk.org/guides/index.html
- gcc 7.5.0
- sqlite3
- libsqlite3-dev(debian machine)
- libsqlite3x-devel.x86_64(centos,redhat machine)

# Installation Guide
1. Install latest version of DPDK
    - as suggest before please go to https://doc.dpdk.org/guides/index.html and follow the guide
2. clone this repo into your machine
3. compile this repo by using make(GNU make not cmake)
4. done!!!

# Use Guide
- Please study about the dpdk parameter
    - link about parameter http://doc.dpdk.org/guides/linux_gsg/linux_eal_parameters.html

## This program is base on the dpdk example(rxtx_callback)