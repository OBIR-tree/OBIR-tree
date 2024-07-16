# OBIR-tree
This repository contains doubly oblivious data structures. The open-source is based on our following paper:

OBIR-tree: Secure and Efficient Oblivious Index for Spatial Keyword Queries


## Pre-requisites: ###
Our schemes were tested with the following configuration
- 64-bit Ubuntu 18.04
- c++
- SGX SDK = 2.0

Install SGX SDK 2.0: https://github.com/intel/linux-sgx/archive/refs/tags/sgx_2.0.zip

Install the above binary file in /opt/intel

Execute the following command to allow the app to find libsample_crypto.so:

sudo cp sample_libcrypto/libsample_libcrypto.so /usr/lib

## SGX2 version of the code ###
There are few changes in the SGX2 version of the code in comparison to SGX1, includeing:

- adding -lsgx_pthread link flag in Makefile
- importing "sgx_pthread.edl" in OMAP.edl and Enclave.edl
- replacing the remote attestation code of Enclave.cpp with the new version of SGXSDK

SGX2 version of Omix++ is available in [Omix++-SGX2 branch](https://github.com/jgharehchamani/graphos/tree/Omix%2B%2B-SGX2)

## Notice ###

##  Getting Started ###
OBIR-tree, and OBIR-treeSGX+RDT parallel version are provided in two separate subfolders. Use the following instruction to build them:


### OBIR-tree Compiling and Running


Compiling:
Cmake
Make
Running:
./progect1
### OBIR-treeSGX+RDT Compiling and Running


Compiling:
Cmake
Make
Running:
./app

For a sample test case, create a file in the Data folder and describe the object in the following format:

for example, a file will be like this:

JazzClub 40.733596 -74.003139
Gym 40.758102 -73.975734
IndianRestaurant 40.732456 -74.003755
IndianRestaurant 42.345907 -71.087001
SandwichPlace 39.933178 -75.159262
BowlingAlley 40.652766 -74.003092
DiveBar 40.726961 -73.980039
Bar 40.756353 -73.967676
SeafoodRestaurant 37.779837 -122.494471
Bar 34.092793 -118.281469
Nightclub 40.591334 -73.960725
JazzClub 40.733630 -74.002288
Pub 41.941562 -87.664011


