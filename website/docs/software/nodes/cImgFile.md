---
title: cImgFile
author: Julian Mann
email: julian.mann@gmail.com
last_update: 
  date: 2023/02/26
description: An image file as cImgData
plugin: Uprising
type: Maya Node
---
### Synopsis

Loads an image file and wraps it as cImgData so it is available through the dependency graph to other cImg nodes. 

cImgData is a data type that encapsulates the whole image. This makes it possible to perform image transformations and other effects that can't be done with Maya's texture nodes.

### Detailed Attribute Description

<div class="attribs">
    <div class="attribs-header attribs-row">
        <div class="attribs-name attribs-cell">Name (short name)</div>
        <div class="attribs-type attribs-cell">Type</div>
        <div class="attribs-default attribs-cell">Default</div>
        <div class="attribs-flags attribs-cell">Flags</div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">imageFilename (im)</div>
            <div class="attribs-type attribs-cell">string</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCS            </div>
        </div>
        <div class="attribs-desc">
            <p>The input image.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">resize (rsz)</div>
            <div class="attribs-type attribs-cell">bool</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCS            </div>
        </div>
        <div class="attribs-desc">
            <p>Resample the input image.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">resolution (rrg)</div>
            <div class="attribs-type attribs-cell">long2</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>The output image resolution if resized.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">interpolation (int)</div>
            <div class="attribs-type attribs-cell">enum</div>
            <div class="attribs-default attribs-cell">1</div>
            <div class="attribs-flags attribs-cell">IOCS            </div>
        </div>
        <div class="attribs-desc">
            <p>Pixel interpolation method if resized.
Options are: "noInterpolation", "nearestNeighbor", "movingAverage", "linear", "grid", "cubic", "lanczos"</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">output (out)</div>
            <div class="attribs-type attribs-cell">cImgData</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOC            </div>
        </div>
        <div class="attribs-desc">
            <p>The output image.</p>
        </div>
    </div>
</div>
