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

# Log file format
1. Destination folder
    1. It's will live in the relative path that you run the session
    2. the storing path will be like this
        - PATH_THAT_YOU_CALLED_PROGRAM/data/src/month-year/date(in that month)/hour(in 24 hour format)
        - if you interest in dst just change src to dst
        - forexample if you run in your home and you want to see src ip log in noon ~/data/src/06-2020/09/12
2. File name and its format
    1. This program will save log file in csv format
    2. The file name format will be like this
        - min(in that hour):sec(in that min)_no of packets in 1 min-tot frame size in 1 min.csv
        - forexample 1:1_42069-555555.csv