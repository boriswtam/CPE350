#!/bin/bash
for i in {1..50}
do
   cat noErrTest >> $1
   sleep 1
   cat errTest >> $1
   sleep 1
done
