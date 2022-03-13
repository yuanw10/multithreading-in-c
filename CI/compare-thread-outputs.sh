#!/bin/bash

unsafeWC=$(cat unsafe_out | wc -l)
unsafeDF=$(diff unsafe_out proper_out | wc -l)
safeWC=$(cat safe_out | wc -l)
safeDF=$(diff safe_out proper_out | wc -l)

if [ $unsafeWC -eq 0 ]; then
	printf "Your unsafe threads program isn't producing any numerical output."
	exit 100
fi

if [ $safeWC -eq 0 ]; then
	printf "Your safe threads program isn't producing any numerical output."
	exit 101
fi

printf "Your unsafe threads produced a series of numbers and there were $unsafeDF out of place. That's supposed to happen.\n"

if [ $safeDF -gt 0 ]; then
	printf "Your safe threads produced a series of numbers and there were $safeDF out of place. That's not supposed to happen. Make sure you properly wait and post."
	exit 102
fi

