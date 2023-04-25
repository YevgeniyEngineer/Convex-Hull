# Convex-Hull
Implementation of 2D and 3D Convex Hull algorithms in C++.

## Build and Run
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./test_convex_hull
```

## Example Output
```bash
./test_convex_hull 
Generating convex hull from 99999 points

Constructing convex hull using GRAHAM SCAN 
Elapsed time (s): 0.0119361
Number of hull points: 28
Hull is oriented clockwise
Convex Hull Indices:
44545 72921 45419 51933 9410 83719 89959 59528 97246 79732 32609 81055 27213 87075 45277 84236 76177 453 6943 9481 8745 53232 65302 40991 62451 324 77649 29064 

Constructing convex hull using ANDREW MONOTONE CHAIN 
Elapsed time (s): 0.0100496
Number of hull points: 28
Hull is oriented clockwise
Convex Hull Indices:
44545 72921 45419 51933 9410 83719 89959 59528 97246 79732 32609 81055 27213 87075 45277 84236 76177 453 6943 9481 8745 53232 65302 40991 62451 324 77649 29064 

Constructing convex hull using JARVIS MARCH 
Elapsed time (s): 0.0123828
Number of hull points: 28
Hull is oriented clockwise
Convex Hull Indices:
59528 97246 79732 32609 81055 27213 87075 45277 84236 76177 453 6943 9481 8745 53232 65302 40991 62451 324 77649 29064 44545 72921 45419 51933 9410 83719 89959 

Constructing convex hull using CHAN 
Elapsed time (s): 0.00752822
Number of hull points: 28
Hull is oriented clockwise
Convex Hull Indices:
59528 97246 79732 32609 81055 27213 87075 45277 84236 76177 453 6943 9481 8745 53232 65302 40991 62451 324 77649 29064 44545 72921 45419 51933 9410 83719 89959
```