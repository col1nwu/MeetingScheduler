# Meeting Scheduler

**EE450 Socket Programming Project**

Name: Daizong (Colin) Wu

Student ID: 8443173092

April 23, 2023


## Idiosyncrasy

This project must be run on **Ubuntu 22.04 ARM64 systems**.


## Accomplishments

This meeting scheduler can perform both **basic** and **bonus** functionalities. In other words, it is able to find the intersection of time intervals for a list of users requested by the client; it is also able to register a meeting time for them.


## File Descriptions

- `client.cpp`: implementation of Client
- `Makefile`: building and compiling instruction of this project
- `serverM.cpp`: implementation of Main Server
- `serverA.cpp`: implementation of Backend Server A
- `serverB.cpp`: implementation of Backend Server B
- `utils.cpp`: implementation of a list of utility functions
- `utils.h`: header file of utils.cpp
- `README.md`: provides an introduction and overview of this project


## Message Format

- A list of availabilities is formatted as `[[a,b],[c,d]]`.
- Availability is sometimes formatted as `[a,b]`. This is rarely used though - only used when client enters a meeting time for all users.
- Usernames are separated by comma in internal communications, like `a,b,c`.
- Usernames are separated by comma and space in output, like `a, b, c`, for better readability.


## Reference

- Solutions of [Leetcode 986](https://leetcode.com/problems/interval-list-intersections/description/).
