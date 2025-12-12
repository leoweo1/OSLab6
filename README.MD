# OS Lab 6: CPU Scheduling Simulator

# Name: Daniel Onwuka

## Overview
Implementation of 4 CPU scheduling algorithms:
- First-Come First-Served (FCFS)
- Shortest Job First (SJF) - Preemptive
- Priority Scheduling
- Round Robin (RR)

## Features
- Processes with arrival times, burst times, and priorities
- Thread-safe implementation
- Calculates waiting time and turnaround time
- Average performance metrics

## Usage
```bash
make
./schedsim input0.txt
Input Format
Each line: pid burst_time arrival_time waiting_time turnaround_time priority

Algorithms
FCFS: Non-preemptive, processes by arrival time

SJF: Preemptive (SRTF), shortest remaining time first

Priority: 0 = highest priority, ties broken by arrival time

RR: Time quantum = 2, accounts for arrival times
