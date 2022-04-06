#!/bin/bash

subtlety_aliveWC=$(cat subtlety_alive | wc -c)
subtlety_aliveDF=$(diff subtlety_alive expected_out/subtlety_alive_out | wc -l)

subtlety_deadWC=$(cat subtlety_dead | wc -c)
subtlety_deadDF=$(diff subtlety_dead expected_out/subtlety_dead_out | wc -l)

if [ $subtlety_aliveWC -eq 0 ]; then
	printf "subtlety path - Adventure program stops producing alive heroes. That's weird."
	exit 100
fi

if [ $subtlety_aliveDF -gt 0 ]; then

	printf "subtlety path - Adventure program isn't producing the alive heroes in the correct order."
	exit 101
fi


if [ $subtlety_deadWC -eq 0 ]; then
	printf "subtlety path - Adventure program stops producing dead heroes. That's weird."
	exit 100
fi

if [ $subtlety_deadDF -gt 0 ]; then
	printf "subtlety path - Adventure program isn't producing the dead heroes in the correct order."
	exit 101
fi
