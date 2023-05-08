# Meeting Scheduler

**EE450 Socket Programming Project**

## Idiosyncrasy

- This project must be run on **Ubuntu 22.04 ARM64 systems**.
- When Main Server detects all valid usernames are located in one of the Backend Servers, it will contact that specific server **only**. The other server will not be contacted in this case.
- The program can handle duplicate usernames, but it is not recommended to have duplicate usernames in input as there are chances that the program behaves oddly in this scenario.
- You can, though it is not recommended, enter empty meeting time for the bonus part, which must be formatted as `[]`. See "Message Format" section for more details.


## Accomplishments

This meeting scheduler can perform both **basic** and **bonus** functionalities. In other words, it is able to find the intersection of time intervals for a list of users requested by the client; it is also able to register a meeting time for them (i.e., remove meeting time from users' availabilities).


## File Descriptions

- `client.cpp`: implementation of Client
- `Makefile`: building and compiling instruction of this project
- `serverM.cpp`: implementation of Main Server
- `serverA.cpp`: implementation of Backend Server A
- `serverB.cpp`: implementation of Backend Server B
- `utils.cpp`: implementation of a list of utility functions
- `utils.h`: header file of utils.cpp
- `README.md`: introduction and overview of this project


## Message Format

- A list of availabilities is formatted as `[[a,b],[c,d]]`.
- For the bonus part, meeting time is formatted as `[a,b]`. If you would like to enter an empty meeting time, you must enter it as `[]`, otherwise the program can fail.
- Usernames are separated by comma and space in output, like `a, b, c`, for better readability.
- Usernames are separated by comma in internal communications, like `a,b,c`.
- Customized output may be displayed, because some edge cases are not introduced in handout. They are displayed to help users understand the program behaviors. Customized output will always be marked as `[Customized output]` at the beginning.


## Reference

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- Solutions of [Leetcode 56](https://leetcode.com/problems/merge-intervals/)
- Solutions of [Leetcode 57](https://leetcode.com/problems/insert-interval/)
- Solutions of [Leetcode 986](https://leetcode.com/problems/interval-list-intersections/)
