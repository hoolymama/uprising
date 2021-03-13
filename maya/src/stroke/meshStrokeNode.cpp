
#include <maya/MIOStream.h>

#include <vector>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include "meshStrokeNode.h"
#include "stroke.h"

#include <maya/MItMeshEdge.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshPolygon.h>

#include <jMayaIds.h>
#include "errorMacros.h"

#include "toonGraph/tNode.h"
#include "toonGraph/tGraph.h"

const double rad_to_deg = (180 / 3.1415927);

MObject meshStrokeNode::aMesh;
MObject meshStrokeNode::aViewpoint;
MObject meshStrokeNode::aPointDensity;

// MObject meshStrokeNode::aWorldMatrix;

MTypeId meshStrokeNode::id(k_meshStrokeNode);

meshStrokeNode::meshStrokeNode() {}

meshStrokeNode::~meshStrokeNode() {}

void *meshStrokeNode::creator()
{
    return new meshStrokeNode();
}

const double epsilon = 0.0001;

MStatus meshStrokeNode::initialize()
{

    MStatus st;
    MFnTypedAttribute tAttr;
    MFnNumericAttribute nAttr;
    MFnMatrixAttribute mAttr;
    
    inheritAttributesFrom("strokeNodeBase");


    aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(1.0f);
    st = addAttribute(aPointDensity);
    mser;

    aMesh = tAttr.create("mesh", "msh", MFnData::kMesh);
    tAttr.setReadable(false);
    addAttribute(aMesh);

    aViewpoint = nAttr.createPoint("viewpoint", "vpt");
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    addAttribute(aViewpoint);

    st = attributeAffects(aMesh, aOutput);
    st = attributeAffects(aPointDensity, aOutput);
    st = attributeAffects(aViewpoint, aOutput);

    return (MS::kSuccess);
}

MStatus meshStrokeNode::generateStrokeGeometry(
    MDataBlock &data,
    std::vector<Stroke> *pStrokes) const
{

    MStatus st;

    MDataHandle hMesh = data.inputValue(aMesh, &st);
    msert;
    MObject dInMesh = hMesh.asMesh();
    MFnMesh fnMesh(dInMesh, &st);
    msert;

    double pointDensity = data.inputValue(aPointDensity).asDouble();
    float3 &fpoint = data.inputValue(aViewpoint).asFloat3();
    MPoint viewPoint(fpoint[0], fpoint[1], fpoint[2]);

    MItMeshEdge edgeIter(dInMesh, &st);
    msert;
    MItMeshVertex vertexIter(dInMesh, &st);
    msert;
    MItMeshPolygon faceIter(dInMesh, &st);
    msert;

    tGraph toonStrokeGraph;

    edgeIter.reset();
    vertexIter.reset();
    faceIter.reset();

    int strokeIndex = 0;
    int prevIndex;
    for (; !edgeIter.isDone(); edgeIter.next())
    {
        int numFaces;
        edgeIter.numConnectedFaces(numFaces);
        if (numFaces < 1 || numFaces > 2)
        {
            continue;
        }
        MFloatPointArray pts;

        int vertexIndex0 = edgeIter.index(0);
        int vertexIndex1 = edgeIter.index(1);
        vertexIter.setIndex(vertexIndex0, prevIndex);
        pts.append(vertexIter.position(MSpace::kWorld));
        vertexIter.setIndex(vertexIndex1, prevIndex);
        pts.append(vertexIter.position(MSpace::kWorld));

        if (edgeIter.onBoundary())
        {
            toonStrokeGraph.addEdge(tcoord(vertexIndex0), tcoord(vertexIndex1), pts[0], pts[1]);
        }
        else
        {
            // Make strokes from silhouette edges:
            MIntArray faceIds;
            edgeIter.getConnectedFaces(faceIds);
            // will be 0 and 1
            MVector normal1;
            MVector normal0;
            MPoint center0;
            MPoint center1;

            faceIter.setIndex(faceIds[0], prevIndex);
            st = faceIter.getNormal(normal0, MSpace::kWorld);
            mser;
            center0 = faceIter.center(MSpace::kWorld);

            faceIter.setIndex(faceIds[1], prevIndex);
            st = faceIter.getNormal(normal1, MSpace::kWorld);
            mser;
            center1 = faceIter.center(MSpace::kWorld);

            MVector view = (edgeIter.center(MSpace::kWorld) - viewPoint).normal();
            double d0 = view * normal0;
            double d1 = view * normal1;
            double dist0 = viewPoint.distanceTo(center0);
            double dist1 = viewPoint.distanceTo(center1);

            if ((d0 < 0.0 && d1 > 0.0 && dist1 > dist0) || (d1 <= 0.0 && d0 >= 0.0 && dist0 >= dist1))
            {
                toonStrokeGraph.addEdge(tcoord(vertexIndex0), tcoord(vertexIndex1), pts[0], pts[1]);
            }
        }
    }

    std::vector<MFloatPointArray> chains;
    toonStrokeGraph.getChains(chains);

    std::vector<MFloatPointArray>::const_iterator iter = chains.begin();
    for (; iter != chains.end(); iter++, strokeIndex++) {
        pStrokes->push_back(Stroke((*iter), strokeIndex));
    }

    return (MS::kSuccess);
}

void meshStrokeNode::postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
