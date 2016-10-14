#!/bin/bash

if [[ "$EUID" -ne "0" ]]; then
    echo "This script was not run as root, self-elevating to root shell"
    exec sudo -E "$0" ${1+"$@"}
fi

declare -i MAX_BRIGHTNESS=7812
declare -i MIN_BRIGHTNESS=0
readonly MAX_BRIGHTNESS
readonly MIN_BRIGHTNESS

declare -i BRIGHTNESS_INCREMENT=400

increaseBrightness() {
    brightnessResult=$((currentBrightness+BRIGHTNESS_INCREMENT))
    if [[ $brightnessResult -lt $MAX_BRIGHTNESS ]]; then
        echo "$brightnessResult"
        echo "$brightnessResult" | tee /sys/class/backlight/intel_backlight/brightness > /dev/null
    else
        echo "$brightnessResult would exceed maximum brightness ($MAX_BRIGHTNESS), setting brightness to $MAX_BRIGHTNESS instead"
        echo "$MAX_BRIGHTNESS" | tee /sys/class/backlight/intel_backlight/brightness > /dev/null
    fi
} 

decreaseBrightness() {
    brightnessResult=$((currentBrightness-BRIGHTNESS_INCREMENT))
    if [[ $brightnessResult -gt $MIN_BRIGHTNESS ]]; then
        echo "$brightnessResult"
        echo "$brightnessResult" | tee /sys/class/backlight/intel_backlight/brightness > /dev/null
    else
        echo "$brightnessResult would be less than minimum brightness ($MIN_BRIGHTNESS), setting brightness to $MIN_BRIGHTNESS instead"
        echo "$MIN_BRIGHTNESS" | tee /sys/class/backlight/intel_backlight/brightness > /dev/null
    fi
}

showUsage() {
    echo "Usage: brightness [--increase/--decrease]"
}

currentBrightness=$(cat /sys/class/backlight/intel_backlight/brightness)

if [[ "$1" == "--increase" || "$1" == "-i" ]]; then
    increaseBrightness
    newBrightness=$(cat /sys/class/backlight/intel_backlight/brightness)
    if [[ $currentBrightness -eq $MAX_BRIGHTNESS ]]; then
        echo "Brightness is already at maximum ($MAX_BRIGHTNESS)"
    else
        echo "Increased brightness from $currentBrightness to $newBrightness"
    fi
elif [[ "$1" == "--decrease" || "$1" == "-d" ]]; then
    decreaseBrightness
    newBrightness=$(cat /sys/class/backlight/intel_backlight/brightness)
    if [[ $currentBrightness -eq $MIN_BRIGHTNESS ]]; then
        echo "Brightness is already at minimum ($MIN_BRIGHTNESS)"
    else
        echo "Decreased brightness from $currentBrightness to $newBrightness"
    fi
else
    showUsage
fi
