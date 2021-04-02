# Raymarching Project

A simple project using raymarching and signed distance functions to render 3d scenes

## Build

```
    mkdir build
    cd build
    cmake ..
    make
```

Sample renders:

![first](assets/first.png)

![dhelix](assets/dna.png)

## Todo
    * Support arbitrary transformations for objects (cameras, shapes)
    * Allow creation of GIF animations by stringing together images
    * Support lighting and shadows
    * Add CSG primitives
    * Implement triangle inequality optimization
    * Octree binning

