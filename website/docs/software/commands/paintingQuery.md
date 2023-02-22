---
title: paintingQuery
author: Julian Mann
email: julian.mann@gmail.com
last_update: 
  date: 2023/02/20
description: paintingQuery brief description
plugin: Uprising
type: Maya Plugin Command
---
### Synopsis

How to use paintingQuery

### Return Value

Describe paintingQuery return value

### Flags

<div class="opts">
    <div class="opts-header opts-row">
        <div class="opts-name opts-cell">Long Name (short name)</div>
        <div class="opts-type opts-cell">Argument types</div>
        <div class="opts-default opts-cell">Properties</div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-clusterBrushId (-cbi)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Return the brushId for the cluster with the given given index. You must provide the clusterIndex.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-clusterCount (-cc)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Return the number of clusters in the painting.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-clusterIndex (-ci)</div>
            <div class="opts-type opts-cell">Int</div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Specifies the cluster index for several other queries.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-clusterPaintId (-cpi)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Return the paintId for the cluster with the given given index. You must provide the clusterIndex.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-clusterReason (-cr)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Return the reason for the change to the cluster with the given index. You must provide the clusterIndex. Possible results are:
<ul>
<li>tool: The brush was changed.</li> 
<li>dip: The brush was dipped in paint.</li> 
<li>tcp: The brush changed its tool control point parameter.</li>
</ul></p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-clusterTravel (-ct)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>The distance that the cluster with the given index travels. You must provide the clusterIndex.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-clusterTravelCutoff (-ctc)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>The distance threshold at which the system stops adding strokes to a cluster. The last stroke of the cluster usually extends further than the cutoff. You must provide the clusterIndex.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-clusterPotId (-cti)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>The pot in which the cluster with the given index dips. You must provide the clusterIndex.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-globalStrokeId (-gsi)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Given a cluster index and a stroke index within the cluster, return the stroke index with respect to the entire painting. You must provide the clusterIndex and the strokeIndex.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-json (-js)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Return a JSON array containing object with the following fields for each stroke:
brushId, paintId, layerId, parentId</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeLayerId (-lid)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-query">Q</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Return the layerId for the given stroke. You must provide the clusterIndex and the strokeIndex.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-rotateOrder (-ro)</div>
            <div class="opts-type opts-cell">String</div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Some commands require the rotateOrder to be provided. For example, the getRotations option needs to know how to deconstruct target matrices.</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-rotateUnit (-ru)</div>
            <div class="opts-type opts-cell">String</div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -rotateUnit</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeApoproximationDistance (-sad)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeApoproximationDistance</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeArcLength (-sal)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeArcLength</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeArrivalPositions (-sap)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeArrivalPositions</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeArrivalRotations (-sar)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeArrivalRotations</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeCount (-sc)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeCount</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeColors (-scl)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeColors</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeCreator (-scr)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeCreator</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeDeparturePosition (-sdp)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeDeparturePosition</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeDepartureRotation (-sdr)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeDepartureRotation</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeIndex (-si)</div>
            <div class="opts-type opts-cell">Int</div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeIndex</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokePositions (-sp)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokePositions</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeParentIndex (-spi)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeParentIndex</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeRotations (-sr)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeRotations</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeSpeedAngular (-ssa)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeSpeedAngular</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeSpeedLinear (-ssl)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeSpeedLinear</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeXAxis (-sxa)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeXAxis</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeYAxis (-sya)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeYAxis</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-strokeZAxis (-sza)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -strokeZAxis</p>
        </div>
    </div>
    <div class="opts-block">
        <div class="opts-body opts-row">
            <div class="opts-name opts-cell">-toolCombinations (-tc)</div>
            <div class="opts-type opts-cell"></div>
            <div class="opts-properties opts-cell">
<span class="opts-prop-create">C</span>
<span class="opts-prop-edit">E</span>
<span class="opts-prop-query">Q</span>
<span class="opts-prop-multiuse">M</span>
            </div>
        </div>
        <div class="opts-desc">
            <p>Describe -toolCombinations</p>
        </div>
    </div>
</div>
### MEL Examples

```// Enter Valid MEL here```

