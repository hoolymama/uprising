---
sidebar_label: 'Using CImg Nodes'
sidebar_position: 1
---

# Using CImg Nodes

### Introduction

CImg nodes are plugin nodes that pass data through the dependency graph as cImgData. Unlike Maya's shading network nodes, which pass only a single sample, cImgNodes pass along the entire image. This is more akin to a compositing network such as Nuke, and allows greater flexibility in terms of image processing. For example, it allows blurring, and other convoltions such as edge detection and detail extraction. It also allows for image transformations like a medial axis transform (MAT), which is useful for finding the skeleton of shapes.

### Get started

The best way to create cImg networks is through Maya's node editor. 

* Go to **Windows->Node Editor** then hit **tab**, start typing [`cImgFile`](../nodes/cImgFile) and hit **Enter** to autocomplete and drop the node.
* With the cImgFile node selected, use the **Attribute Editor** to Browse your hard disk for an image.
* To view the image, open up **Robot Tools** from the **Hooly** menu, then in the Robot Tools menu bar choose **Images->Show in Monitor.**

cImgNodes can be integrated into Maya's shading network if needed via the cImgShader node.

