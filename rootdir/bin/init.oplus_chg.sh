#! /vendor/bin/sh

export PATH=/vendor/bin

prefix="/sys/class/oplus_chg"

if [[ -d "$prefix" ]]
then
    for i in `ls "$prefix"`
    do
        for j in `ls "$prefix"/"$i"`
        do
            #skip directories to prevent possible security issues.
            if [[ -d "$prefix"/"$i"/"$j" ]]
            then
                continue
            else
                chown -h system.system "$prefix"/"$i"/"$j"
            fi
        done
    done
fi