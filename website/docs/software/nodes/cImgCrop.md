---
title: cImgCrop
author: Julian Mann
email: julian.mann@gmail.com
last_update: 
  date: 2023/02/26
description: Crop a cImgData image 
plugin: Uprising
type: Maya Node
---
### Synopsis

Crops an image from any node that passes cImgData.

The image may be resized before cropping. 

The output is always a square image. Letterbox padding is added if the input image is not square.

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
            <div class="attribs-name attribs-cell">input (in)</div>
            <div class="attribs-type attribs-cell">cImgData</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">ICKS            </div>
        </div>
        <div class="attribs-desc">
            <p>The input image.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">boundary (bnd)</div>
            <div class="attribs-type attribs-cell">enum</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCS            </div>
        </div>
        <div class="attribs-desc">
            <p>Boundary conditions when cropped. Options are: "dirichlet", "neumann"</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">letterbox (lbx)</div>
            <div class="attribs-type attribs-cell">enum</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Letterbox color. Options are: "black", "white", "repeat"</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">resize (rsz)</div>
            <div class="attribs-type attribs-cell">bool</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Whether to resize the input image.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">resizeResolution (rrs)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>The resolution of the longest dimension when resizing. The shorter dimension is scaled in order to preserve the original aspect ratio.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">applyCrop (apc)</div>
            <div class="attribs-type attribs-cell">bool</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Whether to crop the input image. If the image is cropped, the crop area is square and the letterbox area is ignored. Therefore, you can't specify a crop resolution that would cause the cropped area to be outside the image proper.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">cropCorner (ccr)</div>
            <div class="attribs-type attribs-cell">long2</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>The top left corner of the crop area.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">cropResolution (crs)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>The resolution of the crop area. Since the output is always square, only one resolution value is required.</p>
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
            <p>The output image as cImgData.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">outputCropFactor (ocf)</div>
            <div class="attribs-type attribs-cell">float</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">OC            </div>
        </div>
        <div class="attribs-desc">
            <p>The fraction of the full uncropped square that the cropped area represents.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">outputOffsetFactorX (oofx)</div>
            <div class="attribs-type attribs-cell">float</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">OC            </div>
        </div>
        <div class="attribs-desc">
            <p>The fraction across from the left of the full square where the crop region begins.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">outputOffsetFactorY (oofy)</div>
            <div class="attribs-type attribs-cell">float</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">OC            </div>
        </div>
        <div class="attribs-desc">
            <p>The fraction down from the top of the full square where the crop region begins.</p>
        </div>
    </div>
</div>
