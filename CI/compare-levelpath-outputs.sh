#!/bin/bash

level_aliveWC=$(cat level_alive | wc -c)
level_aliveDF=$(diff level_alive expected_out/level_alive_out | wc -l)

level_deadWC=$(cat level_dead | wc -c)
level_deadDF=$(diff level_dead expected_out/level_dead_out | wc -l)

if [ $level_aliveWC -eq 0 ]; then
	printf "level path - Adventure program stops producing alive heroes. That's weird."
	exit 100
fi

if [ $level_aliveDF -gt 0 ]; then

	printf "level path - Adventure program isn't producing the alive heroes in the correct order."
	exit 101
fi


if [ $level_deadWC -eq 0 ]; then
	printf "level path - Adventure program stops producing dead heroes. That's weird."
	exit 100
fi

if [ $level_deadDF -gt 0 ]; then
	printf "level path - Adventure program isn't producing the dead heroes in the correct order."
	exit 101
fi
