#!/bin/bash

poolWC=$(cat pool_out | wc -c)
poolDF=$(diff pool_out proper_out | wc -l)

if [ $poolWC -eq 0 ]; then
	printf "Your thread pool program isn't producing any numerical output."
	exit 100
fi

if [ $poolDF -gt 0 ]; then
	printf "Your thread pool program isn't producing the values in the correct order."
	exit 101
fi

