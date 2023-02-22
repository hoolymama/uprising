---
title: curveStroke
author: Julian Mann
email: julian.mann@gmail.com
last_update: 
  date: 2023/02/19
description: curveStroke brief description
plugin: Uprising
type: Maya Node
---
### Synopsis

The curveStroke node allows you to create a set of brushstrokes given one or more nurbs curves as input. The brush strokes are placed along the curves and you have control over several characteristics of the strokes.

Like all stroke generators and modifier nodes, strokes can be sorted and filtered based on stroke properties such as brushId or paintId, or on the value of a projected texture.

Since a stroke is a collection of targets for the robot to reach, the node provides controls to manage the orientation of targets along the stroke.

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
            <div class="attribs-name attribs-cell">strokeSortList (stsl)</div>
            <div class="attribs-type attribs-cell">compound</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCAS            </div>
        </div>
        <div class="attribs-desc">
            <p>An array of sorting definitions that can be used to manage the order of output strokes. Each definition consists of a key, such as brushId, and a direction, either ascending or descending.
The order of elements matters. Definitions first in the array take prescedence over later definitions.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">strokeSortKey (stsk)</div>
            <div class="attribs-type attribs-cell">enum</div>
            <div class="attribs-default attribs-cell">1</div>
            <div class="attribs-flags attribs-cell">IOCS            </div>
        </div>
        <div class="attribs-desc">
            <p>Stroke properties that may be used t configure the sorting operation.
Enumerated strings are: "Stroke Id", "Brush Id", "Paint Id", "Repeat Id", "Layer Id", "Parent Id", "Target Count", "Map Red", "Map Green", "Map Blue", "Brush Model Id", "Brush Shape", "Segment Id"</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">strokeSortDirection (stsd)</div>
            <div class="attribs-type attribs-cell">enum</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCS            </div>
        </div>
        <div class="attribs-desc">
            <p>The direction to apply the sort for this key.
Enumerated strings are: "Ascending", "Descending"</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">strokeSortTexture (stst)</div>
            <div class="attribs-type attribs-cell">float3</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Strokes may be sorted based on channels of a projected texture.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">applySort (apst)</div>
            <div class="attribs-type attribs-cell">bool</div>
            <div class="attribs-default attribs-cell">true</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Turn sorting on or off.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">strokeFilterList (stfl)</div>
            <div class="attribs-type attribs-cell">compound</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCAS            </div>
        </div>
        <div class="attribs-desc">
            <p>An array of filter definitions that can be used to filter out strokes from the output. Each filter definition consists of a key, an operator, and an operand. For example, a filter can be configured to filter out strokes whose brushId is less than 3.
The order of elements is not important since a stroke is only kept if it returns true for <i><strong>all</strong></i> filter entry comparisons.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">strokeFilterKey (stfk)</div>
            <div class="attribs-type attribs-cell">enum</div>
            <div class="attribs-default attribs-cell">1</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Filter brush strokes by comparing a specific stroke property with a value. 
The strokeFilterKey property enables filtering based on the following stroke properties:
"Stroke Id", "Brush Id", "Paint Id", "Repeat Id", "Layer Id", "Parent Id", "Target Count", "Map Red", "Map Green", "Map Blue", "Brush Model Id", "Brush Shape", "Segment Id"</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">strokeFilterOperator (stfop)</div>
            <div class="attribs-type attribs-cell">enum</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>The comparison operator that compares the strokeFilterKey with a value.
Enumerated strings are: &quot;&gt;&quot;, &quot;&lt;&quot;, &quot;==&quot;, &quot;!=&quot;, &quot;nop&quot;</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">strokeFilterOperand (stfod)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>The value against which to compare the strokeFilterKey.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">strokeFilterTexture (stft)</div>
            <div class="attribs-type attribs-cell">float3</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Strokes may be filtered based on a comparison with the channels of a projected texture. For example, a filter could filter out all strokes whose pivot is in an area of 3d space where the projection value is below mid grey (128)</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">applyFilters (apfl)</div>
            <div class="attribs-type attribs-cell">bool</div>
            <div class="attribs-default attribs-cell">true</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Turn filtering on or off.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">startFrom (stfm)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Only output strokes within a range. startFrom works in conjunction with endAt. This can be useful for debugging for example, by evaluating only a single stroke.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">endAt (edat)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">-1</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>endAt defines the exclusive upper limit of the stroke range to output. If startFrom is 0 and endAt is 2, then only stroke 0 and 1 will make it to the output.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">output (out)</div>
            <div class="attribs-type attribs-cell">strokeData</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOC            </div>
        </div>
        <div class="attribs-desc">
            <p>Array of strokes as strokeData. Use the strokeQuery command to interrogate this data.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">linearSpeed (lnsp)</div>
            <div class="attribs-type attribs-cell">float</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Speed limit for the robot's linear moves for strokes generated by this node, measured in cm per second.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">angularSpeed (agsp)</div>
            <div class="attribs-type attribs-cell">angle</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Speed limit for the robot's angular moves for strokes generated by this node, measured in degrees per second.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">approximationDistance (apxd)</div>
            <div class="attribs-type attribs-cell">float</div>
            <div class="attribs-default attribs-cell">5</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Rounding distance for targets.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">coats (cts)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">1</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>How many times to repeat the strokes for this node. Useful if a particular paint is not very thick, or if the brush bristles are so stiff that paint is appied too thinly.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">layerId (lid)</div>
            <div class="attribs-type attribs-cell">long</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Assign a layer Id to this node's strokes that can be used for sorting or filtering downstream in the network.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">pointDensity (pd)</div>
            <div class="attribs-type attribs-cell">float</div>
            <div class="attribs-default attribs-cell">1</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the approximate number of targets per centimeter.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushTwist (btw)</div>
            <div class="attribs-type attribs-cell">double2</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the twist rotation of the brush at the start and end of each stroke. Twist rotation is a rotation about the axis defined by the brush handle.
If the follow attribute is turned on, then the initial brush target transformations flow along the tangent of the curve, otherwise they are aligned with the canvasMatrix. Twist rotation, as well as tilt and bank, are relative to this initial rotation.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushTwistStart (btws)</div>
            <div class="attribs-type attribs-cell">angle</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the twist rotation of the brush at the start of each stroke.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushTwistEnd (btwe)</div>
            <div class="attribs-type attribs-cell">angle</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the twist rotation of the brush at the end of each stroke.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushTilt (btlr)</div>
            <div class="attribs-type attribs-cell">double2</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the tilt rotation of the brush at the start and end of each stroke. Tilt rotation is a rotation back and forth in the direction of travel. It is the most important rotation to get right since it affects the ability of the brush to deposit paint on the canvas.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushTiltStart (btlrs)</div>
            <div class="attribs-type attribs-cell">angle</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the tilt rotation of the brush at the start of each stroke.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushTiltEnd (btlre)</div>
            <div class="attribs-type attribs-cell">angle</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the tilt rotation of the brush at the end of each stroke.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushBank (bbkr)</div>
            <div class="attribs-type attribs-cell">double2</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the bank rotation of the brush at the start and end of each stroke. Bank rotation is a rotation around direction of travel. It is rarely needed.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushBankStart (bbkrs)</div>
            <div class="attribs-type attribs-cell">angle</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the bank rotation of the brush at the start of each stroke.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-2">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushBankEnd (bbkre)</div>
            <div class="attribs-type attribs-cell">angle</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Set the bank rotation of the brush at the end of each stroke.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">brushRotateOrder (bro)</div>
            <div class="attribs-type attribs-cell">enum</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Sets the rotation order for the above rotations.
Available values are: "twistTiltBank", "tiltBankTwist", "bankTwistTilt", "tiltTwistBank", "twistBankTilt", "bankTiltTwist"</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">canvasMatrix (cmat)</div>
            <div class="attribs-type attribs-cell">fltMatrix</div>
            <div class="attribs-default attribs-cell">n/a</div>
            <div class="attribs-flags attribs-cell">IOCK            </div>
        </div>
        <div class="attribs-desc">
            <p>A frame of reference for strokes. The canvas matrix defines the default orientation for targets. The Z axis should be normal to te canvas as the brush handle is aligned in Z. X is the axis to the side of the brush, and Y axis is the back of the brush. Negative Y faces the direction of travel if follow is on, however as mentioned, this can be offset with the twist, tilt, and bank attributes.</p>
        </div>
    </div>
    <div class="attribs-block attribs-d-1">
        <div class="attribs-body attribs-row">
            <div class="attribs-name attribs-cell">strokeLength (stl)</div>
            <div class="attribs-type attribs-cell">float</div>
            <div class="attribs-default attribs-cell">10</div>
            <div class="attribs-flags attribs-cell">IOCKS            </div>
        </div>
        <div class="attribs-desc">
            <p>Strokes are generated at this length. If the curve is longer than strokeLength, more than one stroke is generated.</p>
        </div>
    </div>
</div>
