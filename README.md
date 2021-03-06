[![License][license-image]][license-url]

# HashCalculator
Utility for segmentation of file and calculate hash each segment

## Build project
`cmake -B build -DFIND_MODULES=ON -DFIND_TESTS=ON -DENABLE_TIDY=ON`

`cmake --build build/ -j4`

## Run tests
`cd build`

`ctest`

## Application options
```
Allowed options:
  --help                produce help message
  --input arg           set input file
  --output arg          set output file
  --size arg            set segment size
```

## Run application
`./HashCalculator --input=/home/pi/GOTURN.zip --output=./out.txt --size=400`

## Related project
[Cmake scripts for build c++ projects](https://github.com/nicledomaS/cmake/blob/master/README.md)

[license-image]: https://img.shields.io/badge/License-Apache%202.0-blue.svg
[license-url]: LICENSE
