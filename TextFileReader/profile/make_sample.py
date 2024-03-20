#!/usr/bin/env python3

for xDim in [10, 100, 1_000]:
    for yDim in [1_000, 10_000, 100_000, 1_000_000]:
        dstPath = f'{xDim}_{yDim}.txt'
        with open(dstPath, 'w') as fo:
            line = 'x' * xDim + '\n'
            for _ in range(yDim):
                fo.write(line)
              