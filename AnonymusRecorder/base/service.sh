#!/bin/sh

if pkill AnonymusRecorder; then
    echo "killed"
	/opt/AnonymusRecorder/base/ServiceOff
else
    echo "starting"
    /opt/AnonymusRecorder/bin/AnonymusRecorder
fi
