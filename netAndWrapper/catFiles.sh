#!/bin/bash
for i in {1..50}
do
   cat noErrTest.csv >> $1
   sleep 1
   cat errTest.csv >> $1
   sleep 1
done
