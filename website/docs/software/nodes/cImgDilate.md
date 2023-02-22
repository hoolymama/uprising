---
title: cImgDilate
author: Julian Mann
email: julian.mann@gmail.com
last_update: 
  date: 2023/02/19
description: Dilate or erode a cImg
plugin: Uprising
type: Maya Node
---
### Synopsis

cImgDilate is an image transformation that can be used to grow or shrink the white areas of a mask. Besides dilation, it also allows for some common image operations that are useful when dilating an image, such as blur and median filters.

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
            <div class="attribs-name attribs-cell">invert (inv)</div>
            <div class="attribs-type attribs-cell">bool</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCS            </div>
        </div>
        <div class="attribs-desc">
            <p>Invert the input.</p>
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
            <p>The output image</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">amount (amt)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">1</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Dilation amount. A negative value erodes the image.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">blur (blr)</div>
            <div class="attribs-type attribs-cell">float</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Perform a gaussian blur prior to dilating.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">threshold (thr)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set a threshold value on the image. thi is useful for ensuring that the dilation is performed in an image of black and white only.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">median (med)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">1</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Perform a median filter to fill small holes.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">bitwise (btw)</div>
            <div class="attribs-type attribs-cell">enum</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Since the dilation operation can expand or shrink the original, the bitwise ops make it possible to, for example isolate only the area that is different, thereby resulting in a border mask.  
Available operations are: "None", "OR", "AND", "XOR"</p>
        </div>
    </div>
</div>
