#!/bin/sh
sudo modprobe w1-gpio
sudo modprobe w1-therm
sudo gpio load i2c
