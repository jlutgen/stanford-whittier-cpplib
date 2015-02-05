#!/bin/bash
# Usage: update-all-assignments

SCRIPT=./update-assignment.sh

#$SCRIPT assign1-intro ../assign1-intro/assign1-intro-qtc             
$SCRIPT wordladder-randomwriter ../assign2-wordladder-randomwriter/assign2-wordladder-randomwriter-qtc 
$SCRIPT maze ../assign3-maze/assign3-maze-qtc                  
$SCRIPT recursion-problems ../assign4-recursion-problems/assign4-recursion-problems-qtc
$SCRIPT boggle ../assign5-boggle/assign5-boggle-qtc
$SCRIPT priority-queue ../assign6-priority-queue/assign6-priority-queue-qtc
$SCRIPT pathfinder ../assign7-pathfinder/assign7-pathfinder-qtc
$SCRIPT steganography ../assignX-steganography/assignX-steganography-qtc