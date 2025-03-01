# Maya CImg Image Processing Library

## Overview

This library provides a collection of Maya nodes for advanced image processing operations. Unlike Maya's traditional shading networks which evaluate one sample at a time, these nodes operate on complete images passed through Maya's dependency graph using the custom `cImgData` type. This approach enables complex image transformations like Medial Axis Transform, morphological operations, and other advanced image processing techniques that require full image context.

The ability to process complete images rather than per-sample evaluations is crucial for implementing stylized image treatments and non-local image operations that consider relationships between multiple pixels or regions of the image.

## Core Components

### Base Classes
- **cImgData** - Custom Maya data type for handling complete images in the dependency graph
- **cImgFloatData** - Floating-point variant of cImgData for high-precision operations
- **cImgProcess** - Base class for image processing nodes with common functionality
- **cImgMerge** - Base class for nodes that combine multiple images

### Image Loading and Saving
- **cImgFile** - Base functionality for image file operations
- **cImgPngFile** - PNG file loading with palette support and color/index separation
- **cImgFileCrop** - Image loading with cropping and resizing capabilities
- **cImgFileSplit** - Splits images based on color analysis and sorting

### Color Operations
- **cImgQuantize** - Color reduction using various methods (uniform, median cut, k-means)
- **cImgSetRange** - Remaps color values to new ranges with linear interpolation
- **cImgReverse** - Inverts pixel values while preserving alpha
- **cImgGate** - Thresholding operations with morphological processing
- **cImgFloatGate** - Floating-point variant of gate operations

### Morphological Operations
- **cImgDilate** - Morphological dilation with optional blur and bitwise operations
- **cImgMergeDilate** - Combines dilation with image merging operations
- **cImgSkel** - Skeletonization/Medial Axis Transform with curve generation
- **cImgSmear** - Directional smearing effects with dilation control

### Filtering and Effects
- **cImgBlur** - Gaussian and other blur operations
- **cImgDetail** - Edge detection and detail enhancement
- **cImgDistance** - Distance transform computation
- **cImgGradient** - Gradient computation with configurable parameters
- **cImgNoise** - Procedural noise generation using Simplex noise

### Image Combination
- **cImgMinMax** - Per-pixel minimum/maximum operations across multiple images
- **cImgMult** - Per-pixel multiplication of multiple images
- **cImgMerge** - Base functionality for combining multiple images

### Layout and Organization
- **cImgLayout** - Image layout and composition operations
- **cImgCrop** - Cropping and resizing with boundary handling
- **cImgFloatCrop** - Floating-point variant of crop operations

### Utility Classes
- **cImgUtils** - Common utility functions for image processing
- **KMeans** - K-means clustering implementation for color quantization
- **SimplexNoise** - Noise generation utilities

### Analysis Tools
- **imageMeasureCmd** - Command for analyzing image properties
- **pngInfo** - Command for analyzing PNG files and palettes

## Dependencies

The library uses:
- CImg library for core image processing
- LodePNG for PNG file handling
- Maya API for node and command implementation

## Usage

The nodes can be connected in Maya's node editor like any other nodes. However, remember that they process complete images rather than individual samples, which may affect performance with very large images.

Each node is designed to be modular and can be combined to create complex image processing networks. The results can be used for:
- Stylized image processing
- Image analysis and measurement
- Color manipulation and optimization
- Advanced filtering effects
- Morphological operations 