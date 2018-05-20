
/*
 * strokeFactory.cpp
 * src
 *
 * Created by Julian Mann on 29/06/2010.
 *
 * Copyright (c) 2010 Julian Mann
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>


#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>

#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MArrayDataHandle.h>

#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>


#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnEnumAttribute.h>

#include <maya/MFnNurbsCurveData.h>
#include "strokeFactory.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"


MObject strokeFactory::aNormal;

MObject strokeFactory::aInMatrix;
MObject strokeFactory::aCurve;
MObject strokeFactory::aPointDensity;
MObject strokeFactory::aStrokeLength;
MObject strokeFactory::aAttack;
MObject strokeFactory::aLift;
MObject strokeFactory::aElevation;
MObject strokeFactory::aBrushId;
MObject strokeFactory::aPaintId;
MObject strokeFactory::aCurves;


//  brushes
MObject strokeFactory::aBrushWidth;
MObject strokeFactory::aBrushName;
MObject strokeFactory::aBrushTcp;
MObject strokeFactory::aBrushTip;
MObject strokeFactory::aBrushes;

// paint
MObject strokeFactory::aPaintColorR;
MObject strokeFactory::aPaintColorG;
MObject strokeFactory::aPaintColorB;
MObject strokeFactory::aPaintColor;
MObject strokeFactory::aPaintName;
MObject strokeFactory::aPaintViscosity;
MObject strokeFactory::aPaints;

MObject strokeFactory::aOutCounts;
MObject strokeFactory::aOutBrushIds;
MObject strokeFactory::aOutPaintIds;
MObject strokeFactory::aOutPointsWorld;
MObject strokeFactory::aOutPointsLocal;
MObject strokeFactory::aOutNormalsWorld;
MObject strokeFactory::aOutNormalsLocal;
MObject strokeFactory::aOutMesh;


MObject strokeFactory::aDisplayPoints;
MObject strokeFactory::aDisplayNormals;
MObject strokeFactory::aDisplaySegments;

MObject strokeFactory::aPointSize;
MObject strokeFactory::aNormalLength;
MObject strokeFactory::aNormalColor;

MObject strokeFactory::aColor;



MTypeId strokeFactory::id( k_strokeFactory );

strokeFactory::strokeFactory() {}

strokeFactory::~strokeFactory() {}

void *strokeFactory::creator() {
  return new strokeFactory();
}

const double epsilon = 0.0001;


MStatus strokeFactory::initialize()
{
  //
  MStatus st;

  MString method("strokeFactory::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
  MFnCompoundAttribute cAttr;
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;

  aNormal = nAttr.create("normal", "nrm", MFnNumericData::k3Double);
  nAttr.setWritable(true);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute( aNormal ); er;


  aInMatrix = mAttr.create( "inMatrix", "imat",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  addAttribute(aInMatrix);

  aCurve = tAttr.create( "curve", "crv", MFnNurbsCurveData::kNurbsCurve, &st); er;
  tAttr.setReadable(false);
  tAttr.setStorable(false);

  aStrokeLength = nAttr.create( "strokeLength", "stl", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aStrokeLength);

  aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.1 );

  aAttack = nAttr.create( "attack", "atk", MFnNumericData::k3Double );
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 1.0, 0.1, 0.01);

  aLift = nAttr.create( "lift", "lft", MFnNumericData::k3Double );
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 1.0, 0.1, 0.01);

  aElevation = uAttr.create( "elevation", "ele", MFnUnitAttribute::kAngle,
                             mayaMath::half_pi );
  uAttr.setStorable(true);
  uAttr.setReadable(true);
  uAttr.setMin(mayaMath::quarter_pi);
  uAttr.setMax(mayaMath::quarter_pi +  mayaMath::half_pi);


  aBrushId = nAttr.create("brushId", "brid", MFnNumericData::kShort); er;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aBrushId); er;

  aPaintId = nAttr.create("paintId", "ptid", MFnNumericData::kShort); er;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aPaintId); er;

  aCurves = cAttr.create("curves", "crvs");
  cAttr.addChild(aCurve);
  cAttr.addChild(aPointDensity);
  cAttr.addChild(aStrokeLength);
  cAttr.addChild(aAttack);
  cAttr.addChild(aLift);
  cAttr.addChild(aElevation);
  cAttr.setArray( true );
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute( aCurves ); er;


  // //  brushes
  aBrushWidth  = nAttr.create("brushWidth", "brwd", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.1 );

  aBrushName = tAttr.create( "brushName", "brnm", MFnData::kString );
  tAttr.setStorable(true);

  aBrushTcp = nAttr.create( "brushTcp", "btcp", MFnNumericData::k3Double );
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 1.0, 0.1, 0.01);

  aBrushTip = nAttr.create("brushTip", "btip", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.1 );

  aBrushes = cAttr.create("brushes", "bsh");
  cAttr.addChild(aBrushWidth);
  cAttr.addChild(aBrushName);
  cAttr.addChild(aBrushTcp);
  cAttr.addChild(aBrushTip);
  cAttr.setArray( true );
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute( aBrushes ); er;


  aPaintColorR = nAttr.create( "paintColorR", "pcr", MFnNumericData::kFloat);
  aPaintColorG = nAttr.create( "paintColorG", "pcg", MFnNumericData::kFloat);
  aPaintColorB = nAttr.create( "paintColorB", "pcb", MFnNumericData::kFloat);
  aPaintColor  = nAttr.create( "paintColor",  "pc" , aPaintColorR, aPaintColorG,
                               aPaintColorB);
  nAttr.setStorable(true);
  nAttr.setHidden(false);
  nAttr.setWritable(true);
  nAttr.setUsedAsColor(true);
  addAttribute(aPaintColor);

  aPaintName = tAttr.create( "paintName", "pnm", MFnData::kString );
  tAttr.setStorable(true);


  aPaintViscosity = nAttr.create("paintViscosity", "pvis", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault(0.5);

  aPaints = cAttr.create("paints", "pts");
  cAttr.addChild(aPaintColor);
  cAttr.addChild(aPaintName);
  cAttr.addChild(aPaintViscosity);
  cAttr.setArray( true );
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute( aPaints ); er;



  // eAttr.addField("Parametric", kParametric);
  // eAttr.addField("ArcLength", kArcLen);
  // eAttr.setKeyable(true);
  // eAttr.setWritable(true);


  aDisplayPoints = nAttr.create( "displayPoints", "dps", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayPoints );

  aDisplayNormals = nAttr.create( "displayNormals", "dts", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayNormals );

  aDisplaySegments = nAttr.create( "displaySegments", "dsg", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplaySegments );



  // aDisplayStrokeGap = nAttr.create( "strokeGap", "sg", MFnNumericData::kDouble);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setMin(0.00);
  // nAttr.setSoftMax(2.0);
  // nAttr.setDefault(0.1);
  // addAttribute(aDisplayStrokeGap);

  aPointSize = nAttr.create( "pointSize", "psi", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aPointSize);

  aNormalLength = nAttr.create( "normalLength", "nln", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aNormalLength);

  aColor = nAttr.createColor( "color", "col");
  nAttr.setStorable(true);
  nAttr.setKeyable(false);
  nAttr.setConnectable(false);
  addAttribute(aColor);

  aNormalColor = nAttr.createColor( "normalColor", "ncl");
  nAttr.setStorable(true);
  nAttr.setKeyable(false);
  nAttr.setConnectable(false);
  addAttribute(aNormalColor);


  aOutBrushIds  = tAttr.create("outBrushIds", "obids", MFnData::kIntArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutBrushIds ); er;

  aOutPaintIds  = tAttr.create("outPaintIds", "opids", MFnData::kIntArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutPaintIds ); er;

  aOutCounts  = tAttr.create("outCounts", "octs", MFnData::kIntArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutCounts ); er;


  aOutPointsLocal = tAttr.create("outPointsLocal", "opl", MFnData::kVectorArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutPointsLocal ); er;

  aOutPointsWorld = tAttr.create("outPointsWorld", "opw", MFnData::kVectorArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutPointsWorld ); er;

  aOutNormalsLocal = tAttr.create("outNormalsLocal", "otl", MFnData::kVectorArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutNormalsLocal ); er;

  aOutNormalsWorld = tAttr.create("outNormalsWorld", "otw", MFnData::kVectorArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutNormalsWorld ); er;





  aOutMesh = tAttr.create( "outMesh", "out", MFnData::kMesh, &st ); er
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  st = addAttribute(aOutMesh); er;

  st = attributeAffects(aInMatrix, aOutPointsLocal);
  st = attributeAffects(aCurve, aOutPointsLocal);
  st = attributeAffects(aPointDensity, aOutPointsLocal);
  st = attributeAffects(aStrokeLength, aOutPointsLocal);
  st = attributeAffects(aAttack, aOutPointsLocal);
  st = attributeAffects(aLift, aOutPointsLocal);
  st = attributeAffects(aElevation, aOutPointsLocal);
  st = attributeAffects(aCurves, aOutPointsLocal);
  st = attributeAffects(aNormal, aOutPointsLocal);

  st = attributeAffects(aInMatrix, aOutPointsWorld);
  st = attributeAffects(aCurve, aOutPointsWorld);
  st = attributeAffects(aPointDensity, aOutPointsWorld);
  st = attributeAffects(aStrokeLength, aOutPointsWorld);
  st = attributeAffects(aAttack, aOutPointsWorld);
  st = attributeAffects(aLift, aOutPointsWorld);
  st = attributeAffects(aElevation, aOutPointsWorld);
  st = attributeAffects(aCurves, aOutPointsWorld);
  st = attributeAffects(aNormal, aOutPointsWorld);

  st = attributeAffects(aInMatrix, aOutNormalsLocal);
  st = attributeAffects(aCurve, aOutNormalsLocal);
  st = attributeAffects(aPointDensity, aOutNormalsLocal);
  st = attributeAffects(aStrokeLength, aOutNormalsLocal);
  st = attributeAffects(aAttack, aOutNormalsLocal);
  st = attributeAffects(aLift, aOutNormalsLocal);
  st = attributeAffects(aElevation, aOutNormalsLocal);
  st = attributeAffects(aCurves, aOutNormalsLocal);
  st = attributeAffects(aNormal, aOutNormalsLocal);

  st = attributeAffects(aInMatrix, aOutNormalsWorld);
  st = attributeAffects(aCurve, aOutNormalsWorld);
  st = attributeAffects(aPointDensity, aOutNormalsWorld);
  st = attributeAffects(aStrokeLength, aOutNormalsWorld);
  st = attributeAffects(aAttack, aOutNormalsWorld);
  st = attributeAffects(aLift, aOutNormalsWorld);
  st = attributeAffects(aElevation, aOutNormalsWorld);
  st = attributeAffects(aCurves, aOutNormalsWorld);
  st = attributeAffects(aNormal, aOutNormalsWorld);

  st = attributeAffects(aInMatrix, aOutCounts);
  st = attributeAffects(aCurve, aOutCounts);
  st = attributeAffects(aPointDensity, aOutCounts);
  st = attributeAffects(aStrokeLength, aOutCounts);
  st = attributeAffects(aAttack, aOutCounts);
  st = attributeAffects(aLift, aOutCounts);
  st = attributeAffects(aElevation, aOutCounts);
  st = attributeAffects(aCurves, aOutCounts);
  st = attributeAffects(aNormal, aOutCounts);

  st = attributeAffects(aInMatrix, aOutBrushIds);
  st = attributeAffects(aCurve, aOutBrushIds);
  st = attributeAffects(aPointDensity, aOutBrushIds);
  st = attributeAffects(aStrokeLength, aOutBrushIds);
  st = attributeAffects(aAttack, aOutBrushIds);
  st = attributeAffects(aLift, aOutBrushIds);
  st = attributeAffects(aElevation, aOutBrushIds);
  st = attributeAffects(aCurves, aOutBrushIds);
  st = attributeAffects(aNormal, aOutBrushIds);

  st = attributeAffects(aInMatrix, aOutPaintIds);
  st = attributeAffects(aCurve, aOutPaintIds);
  st = attributeAffects(aPointDensity, aOutPaintIds);
  st = attributeAffects(aStrokeLength, aOutPaintIds);
  st = attributeAffects(aAttack, aOutPaintIds);
  st = attributeAffects(aLift, aOutPaintIds);
  st = attributeAffects(aElevation, aOutPaintIds);
  st = attributeAffects(aCurves, aOutPaintIds);
  st = attributeAffects(aNormal, aOutPaintIds);

  st = attributeAffects(aInMatrix, aOutMesh);
  st = attributeAffects(aCurve, aOutMesh);
  st = attributeAffects(aPointDensity, aOutMesh);
  st = attributeAffects(aStrokeLength, aOutMesh);
  st = attributeAffects(aAttack, aOutMesh);
  st = attributeAffects(aLift, aOutMesh);
  st = attributeAffects(aElevation, aOutMesh);
  st = attributeAffects(aCurves, aOutMesh);
  st = attributeAffects(aNormal, aOutMesh);
  st = attributeAffects(aBrushId, aOutMesh);
  st = attributeAffects(aPaintId, aOutMesh);



  return ( MS::kSuccess );


}


unsigned int strokeFactory::getStrokeBoundaries(
  const MObject &curve, double &maxStrokeLength, MVectorArray &result
) const  {
  MStatus st = MS::kSuccess;

  MFnNurbsCurve curveFn(curve, &st);
  if (st.error()) { return 0; }
  double curveLen = curveFn.length(epsilon);
  unsigned nstrokes = ceil(curveLen / maxStrokeLength);
  double strokeLength  = curveLen / nstrokes;

  for (int i = 0; i < nstrokes; ++i)
  {
    double startDist = i / double(nstrokes) * curveLen;
    double endDist = startDist + strokeLength;
    result.append(MVector(startDist, endDist));
  }
  return result.length();

}




MStatus strokeFactory::getBrushes(MDataBlock &data,
                                  std::map<short, brush> &brushes ) const {
  MStatus st;
  MArrayDataHandle hBrushes = data.inputArrayValue(aBrushes, &st ); ert;
  unsigned nPlugs = hBrushes.elementCount();
  cerr << "HERE 1" << endl;
  for (unsigned i = 0; i < nPlugs; i++, hBrushes.next()) {
    cerr << "HERE LOOP 1" << endl;
    short index = short(hBrushes.elementIndex(&st));
    if (st.error()) {
      continue;
    }
    cerr << "HERE LOOP 2" << endl;
    MDataHandle hComp = hBrushes.inputValue(&st );
    cerr << "HERE LOOP 3" << endl;

    double width =  hComp.child(strokeFactory::aBrushWidth).asDouble() ;
    double tip = hComp.child(strokeFactory::aBrushTip).asDouble() ;
    MVector tcp = hComp.child(strokeFactory::aBrushTcp).asVector() ;
    MString name = hComp.child(strokeFactory::aBrushName).asString();
    cerr << "HERE LOOP 4" << endl;

    brush b = brush(width, tip, tcp, name);
    brushes[index] = b;
    cerr << "HERE LOOP 5" << endl;

  }
  return MS::kSuccess;
}

MStatus strokeFactory::getPaints(MDataBlock &data,
                                 std::map<short, paint> &paints ) const {
  MStatus st;
  MArrayDataHandle hPaints = data.inputArrayValue(aPaints, &st ); ert;
  unsigned nPlugs = hPaints.elementCount();

  for (unsigned i = 0; i < nPlugs; i++, hPaints.next()) {

    short index = short(hPaints.elementIndex(&st));
    if (st.error()) {
      continue;
    }

    MDataHandle hComp = hPaints.inputValue(&st );


    MVector vc =  hComp.child(strokeFactory::aPaintColor).asVector() ;
    MColor color(vc.x, vc.y, vc.z);

    double viscosity = hComp.child(strokeFactory::aPaintViscosity).asDouble() ;
    MString name = hComp.child(strokeFactory::aPaintName).asString();

    paints[index] = paint(color, viscosity, name);

  }
  return MS::kSuccess;
}


MStatus strokeFactory::getStrokes(MDataBlock &data,
                                  const   std::map<short, brush> brushes,
                                  std::vector<stroke> &strokes ) const {
  MStatus st;
  MArrayDataHandle hCurves = data.inputArrayValue(aCurves, &st ); ert;
  unsigned nPlugs = hCurves.elementCount();

  MVector normal = data.inputValue(aNormal).asVector();

  for (unsigned i = 0; i < nPlugs; i++, hCurves.next()) {

    MDataHandle hComp = hCurves.inputValue(&st );
    double pointDensity =  hComp.child(strokeFactory::aPointDensity).asDouble() ;
    if (pointDensity < 0) { pointDensity = 0.001; }

    double strokeLength = hComp.child( aStrokeLength).asDouble();
    double elevation  =  hComp.child( aElevation).asAngle().value();

    double3    &attack = hComp.child( aAttack).asDouble3();
    double3   &lift = hComp.child( aLift).asDouble3();

    short brushId =  hComp.child( aBrushId).asShort();
    short paintId =  hComp.child( aPaintId).asShort();


    MObject  dCurve =  hComp.child(strokeFactory::aCurve).asNurbsCurveTransformed();
    MFnNurbsCurve curveFn(dCurve, &st);
    if ( st.error()) {
      continue;
    }

    MVectorArray boundaries;
    unsigned numStrokes =  getStrokeBoundaries(dCurve, strokeLength, boundaries);

    double tipDist = 0;
    std::map<short, brush>::const_iterator brushIter = brushes.find(brushId);
    if (brushIter !=  brushes.end()) {
      tipDist = brushIter->second.tip;
    }
    for (int i = 0; i < numStrokes; ++i)
    {


      strokes.push_back(stroke(
                          boundaries[i].x,
                          boundaries[i].y,
                          pointDensity,
                          normal,
                          attack,
                          lift,
                          elevation,
                          tipDist,
                          brushId,
                          paintId,
                          dCurve));
    }
  }
  return MS::kSuccess;
}

MStatus strokeFactory::compute( const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  MString method("strokeFactory::compute");

  if (!(
        plug == aOutPointsLocal ||
        plug == aOutPointsWorld ||
        plug == aOutNormalsLocal ||
        plug == aOutNormalsWorld ||
        plug == aOutCounts ||
        plug == aOutMesh
      )) { return ( MS::kUnknownParameter ); }


  // MArrayDataHandle hCurves = data.inputArrayValue(aCurves, &st );
  MVectorArray outPoints; outPoints.clear();
  MVectorArray outNormals; outNormals.clear();
  MIntArray outCounts; outCounts.clear();
  MIntArray outBrushIds; outBrushIds.clear();
  MIntArray outPaintIds; outPaintIds.clear();

  // MVector normal = data.inputValue(aNormal).asVector();

  // unsigned nPlugs = hCurves.elementCount();
  MMatrix wm = data.inputValue(strokeFactory::aInMatrix).asMatrix();

  std::map<short, brush> brushes;
  st = getBrushes(data, brushes) ;

  std::map<short, paint> paints;
  st = getPaints(data, paints) ;

  std::vector<stroke>  strokes;
  st = getStrokes(data, brushes, strokes) ;



  std::vector<stroke>::const_iterator iter = strokes.begin();
  for (; iter != strokes.end(); iter++) {
    const MVectorArray &pts = iter->points;
    const MVectorArray &nrm = iter->normals;
    const unsigned len = pts.length();
    outCounts.append(len);
    outBrushIds.append(iter->brushId());
    outPaintIds.append(iter->paintId());


    for (int i = 0; i < len; ++i)
    {
      outPoints.append(pts[i]);
      outNormals.append(nrm[i]);
    }
  }

  unsigned len = outPoints.length();
  // cerr << "len: " << len << endl;

  MVectorArray ptsWorld(len);
  MVectorArray normalsWorld(len);

  for (int i = 0; i < len; ++i)
  {
    ptsWorld[i] = MVector(MPoint(outPoints[i]) * wm);
    normalsWorld[i] = outNormals[i] * wm;
  }

  MDataHandle hOutPointsLocal = data.outputValue(aOutPointsLocal);
  MFnVectorArrayData fnOutPointsLocal;
  MObject dOutPointsLocal = fnOutPointsLocal.create(outPoints);
  hOutPointsLocal.set(dOutPointsLocal);
  st = data.setClean( aOutPointsLocal ); er;

  MDataHandle hOutPointsWorld = data.outputValue(aOutPointsWorld);
  MFnVectorArrayData fnOutPointsWorld;
  MObject dOutPointsWorld = fnOutPointsWorld.create(ptsWorld);
  hOutPointsWorld.set(dOutPointsWorld);
  st = data.setClean( aOutPointsWorld ); er;

  MDataHandle hOutNormalsLocal = data.outputValue(aOutNormalsLocal);
  MFnVectorArrayData fnOutNormalsLocal;
  MObject dOutNormalsLocal = fnOutNormalsLocal.create(outNormals);
  hOutNormalsLocal.set(dOutNormalsLocal);
  st = data.setClean( aOutNormalsLocal ); er;

  MDataHandle hOutNormalsWorld = data.outputValue(aOutNormalsWorld);
  MFnVectorArrayData fnOutNormalsWorld;
  MObject dOutNormalsWorld = fnOutNormalsWorld.create(normalsWorld);
  hOutNormalsWorld.set(dOutNormalsWorld);
  st = data.setClean( aOutNormalsWorld ); er;

  MDataHandle hOutCounts = data.outputValue(aOutCounts);
  MFnIntArrayData fnOutCounts;
  MObject dOutCounts = fnOutCounts.create(outCounts);
  hOutCounts.set(dOutCounts);
  st = data.setClean( aOutCounts ); er;

  MDataHandle hOutBrushIds = data.outputValue(aOutBrushIds);
  MFnIntArrayData fnOutBrushIds;
  MObject dOutBrushIds = fnOutBrushIds.create(outBrushIds);
  hOutBrushIds.set(dOutBrushIds);
  st = data.setClean( aOutBrushIds ); er;

  MDataHandle hOutPaintIds = data.outputValue(aOutPaintIds);
  MFnIntArrayData fnOutPaintIds;
  MObject dOutPaintIds = fnOutPaintIds.create(outPaintIds);
  hOutPaintIds.set(dOutPaintIds);
  st = data.setClean( aOutPaintIds ); er;

  return MS::kSuccess;

}


MVector binormal(const MVector &p1, const MVector &p2, const MVector &normal) {
  return ((p2 - p1) ^ normal).normal();
}



void strokeFactory::draw( M3dView &view,
                          const MDagPath &path,
                          M3dView::DisplayStyle style,
                          M3dView:: DisplayStatus status  )
{

  MStatus st;

  MObject thisObj = thisMObject();

  // get sample positions from output
  MPlug positionPlug(thisObj, aOutPointsLocal);
  MObject dPositions;
  st = positionPlug.getValue(dPositions); er;
  MFnVectorArrayData fnV(dPositions);
  MVectorArray positions = fnV.array(&st); er;
  unsigned pl = positions.length();

  MPlug normalsPlug(thisObj, aOutNormalsLocal);
  MObject dNormals;
  st = normalsPlug.getValue(dNormals); er;
  fnV.setObject(dNormals);
  MVectorArray normals = fnV.array(&st); er;

  MPlug countsPlug(thisObj, aOutCounts);
  MObject dCounts;
  st = countsPlug.getValue(dCounts); er;
  MFnIntArrayData fnI(dCounts);
  fnI.setObject(dCounts);
  MIntArray counts = fnI.array(&st); er;
  unsigned numStrokes = counts.length();



  // double x,y,z;
  double planeNormal[3];
  MPlug  normalPlug (thisObj, aNormal);
  normalPlug.child(0).getValue(planeNormal[0]);
  normalPlug.child(1).getValue(planeNormal[1]);
  normalPlug.child(2).getValue(planeNormal[2]);


  view.beginGL();

  bool doDisplayPoints;
  bool doDisplayNormals;
  bool doDisplaySegments;

  MPlug(thisObj, aDisplayPoints).getValue(doDisplayPoints);
  MPlug(thisObj, aDisplayNormals).getValue(doDisplayNormals);
  MPlug(thisObj, aDisplaySegments).getValue(doDisplaySegments);

  MPlug colorPlug (thisObj, aColor);
  float colorRed, colorGreen, colorBlue;
  colorPlug.child(0).getValue(colorRed);
  colorPlug.child(1).getValue(colorGreen);
  colorPlug.child(2).getValue(colorBlue);

  MPlug normalColorPlug (thisObj, aNormalColor);
  float normalColorRed, normalColorGreen, normalColorBlue;
  normalColorPlug.child(0).getValue(normalColorRed);
  normalColorPlug.child(1).getValue(normalColorGreen);
  normalColorPlug.child(2).getValue(normalColorBlue);


  // double strokeGap;
  // MPlug(thisObj, aDisplayStrokeGap).getValue(strokeGap);

  // points and segments
  if (doDisplayPoints)
  {
    MPlug pointSizePlug (thisObj, aPointSize);
    double pointSize;
    pointSizePlug.getValue(pointSize);

    view.setDrawColor( MColor( MColor::kRGB, colorRed, colorGreen, colorBlue ) );

    glPushAttrib(GL_CURRENT_BIT);
    glPointSize(float(pointSize));

    glBegin( GL_POINTS );

    for (unsigned i = 0; i < pl; i++) {
      glVertex3f(float(positions[i].x), float(positions[i].y), float(positions[i].z));
    }
    glEnd();
    glPopAttrib();
  }

  if (doDisplaySegments)
  {

    // view.setDrawColor( MColor( MColor::kRGB, colorRed, colorGreen, colorBlue ) );

    // glColor3f(m_col_start[i].x , m_col_start[i].y , m_col_start[i].z );
    // glVertex3f( m_start[i].x , m_start[i].y , m_start[i].z );
    // glColor3f(m_col_end[i].x , m_col_end[i].y , m_col_end[i].z );
    // glVertex3f(     m_end[i].x , m_end[i].y, m_end[i].z);


    glPushAttrib(GL_CURRENT_BIT);

    unsigned i = 0;
    for (int j = 0; j < numStrokes; ++j)
    {
      glBegin(GL_TRIANGLE_STRIP);
      unsigned numPoints = counts[j];
      for (int k = 0; k < numPoints ; ++k)
      {
        MVector side;
        if (k == 0)  {
          side = binormal(positions[i], positions[(i + 1)], planeNormal) * 0.1;
        }
        else if (k == (numPoints - 1)) {
          side = binormal(positions[(i - 1)], positions[i], planeNormal) * 0.1;
        }
        else {
          MVector side1 = binormal(positions[(i - 1)], positions[i], planeNormal);
          MVector side2  = binormal(positions[i], positions[(i + 1)], planeNormal);
          side = (side1 + side2).normal() * 0.1;

        }


        MFloatVector a = MFloatVector(positions[i] + side);
        MFloatVector b = MFloatVector(positions[i] - side);
        glVertex3f( a.x , a.y , a.z );
        glVertex3f( b.x , b.y, b.z);
        i++;
      }
      glEnd();
    }

    // glBegin(GL_TRIANGLE_STRIP);

    // glVertex3f(-1.0f, -0.5f, -4.0f);    // A
    // glVertex3f( 1.0f, -0.5f, -4.0f);    // B

    // glVertex3f( 0.0f,  0.5f, -4.0f);    // C
    // glVertex3f(1.5f,  0.0f, -4.0f);     // D
    // glVertex3f(2.0f, -1.5f, -4.0f);     // E

    // glEnd();







    // glBegin( GL_LINES );
    // unsigned i = 0;
    // for (int j = 0; j < numStrokes; ++j)
    // {
    //   unsigned numPoints = counts[j];
    //   for (int k = 0; k < (numPoints - 1); ++k)
    //   {
    //     const MVector &next = positions[(i + 1)];
    //     MVector unit = (next - positions[i]).normal();
    //     MFloatVector start, end;
    //     start = MFloatVector(positions[i]);
    //     end = MFloatVector(next);
    //     glVertex3f( start.x , start.y , start.z );
    //     glVertex3f( end.x , end.y, end.z);
    //     i++;
    //   }
    //   i++;
    // }



    // for (unsigned i = 0; i < (pl - 1); i++) {
    //   MFloatVector start(positions[i]);
    //   MFloatVector end(positions[(i + 1)]);
    //   glVertex3f( start.x , start.y , start.z );
    //   glVertex3f( end.x , end.y, end.z);

    // }
    glEnd();
    glPopAttrib();
  }

  if (doDisplayNormals)
  {

    double lineLength;
    MPlug(thisObj, aNormalLength).getValue(lineLength);

    view.setDrawColor( MColor( MColor::kRGB, normalColorRed, normalColorGreen,
                               normalColorBlue ) );

    glPushAttrib(GL_CURRENT_BIT);

    glBegin( GL_LINES );

    for (unsigned i = 0; i < pl; i++) {
      MFloatVector start(positions[i]);
      MFloatVector end((normals[i].normal() * lineLength) + positions[i]);

      glVertex3f( start.x , start.y , start.z );
      glVertex3f( end.x , end.y, end.z);

    }


    glEnd();
    glPopAttrib();
  }


  view.endGL();

}


// int strokeFactory::gridValues(int density, MFloatArray & result) const {

//  if (density < 2) {
//    result.append(0.0f);
//  } else {
//    float spacing = 1.0 / double(density -1);
//    for (int i=0; i < density; i++) {
//      result.append( float(-0.5+(i*spacing))  );
//    }
//  }
//  return  result.length();
// }


// bool strokeFactory::isInDepth(int current, int num, int depth) const {
//  if (depth < 0) return true;
//  if (depth == 0) return false;
//  if (
//    ((current+1) > depth) &&
//    ((num-current) > depth) )
//  {
//    return false;
//  }
//  return true;
// }



// MStatus strokeFactory::getFractions(
//   unsigned nPts,
//   double min,
//   double max,
//   MDoubleArray &result,
//   MFnNurbsCurve::Form form
// ) const
// {
//   MStatus st = MS::kSuccess;

//   if (max < min) {
//     double tmp = min;
//     min = max;
//     max = tmp;
//   }
//   double range  = max - min;

//   result.setLength(nPts);


//   if (nPts == 1) {
//     result.set(0.5, 0);
//   }
//   else {
//     if (form == MFnNurbsCurve::kOpen) {
//       double recip = 1.0 / (nPts - 1);
//       for (unsigned i = 0; i < nPts; i++) {
//         result.set( (double(i) * recip)     , i);
//       }
//     }
//     else {
//       double recip = 1.0 / (nPts);
//       for (unsigned i = 0; i < nPts; i++) {
//         result.set(   (double(i) * recip)    , i);
//       }
//     }
//   }

//   // cerr << "fractions" << fractions << endl;
//   if (! ((range  == 1.0)  && (min == 0))) {
//     for (unsigned i = 0; i < nPts; i++) {
//       result[i] =  (result[i] * range) + min;
//     }
//   }
//   return st;

// }


// MStatus strokeFactory::sampleCurve(
//   const MObject &curve,
//   const MDoubleArray &fractions,
//   const MVector &rotateTangent,
//   MVectorArray &outPoints,
//   MVectorArray &outNormals,
//   Spac spacing
// ) const  {
//   MStatus st = MS::kSuccess;

//   MFnNurbsCurve curveFn(curve, &st);
//   if (st.error()) { return MS::kFailure; }
//   unsigned npts = fractions.length();

//   // outPoints.setLength(npts);
//   // outNormals.setLength(npts);

//   MPoint resultPoint;
//   double param;
//   MMatrix mat = MEulerRotation(rotateTangent).asMatrix();

//   if (spacing == strokeFactory::kParametric)  {
//     double startParam, endParam, rangeParam;
//     curveFn.getKnotDomain(startParam, endParam);

//     rangeParam =  endParam - startParam;
//     for (unsigned i = 0; i < npts; i++) {
//       param = (fractions[i] * rangeParam ) + startParam;
//       st = curveFn.getPointAtParam(param, resultPoint, MSpace::kObject);
//       outPoints.append( MVector(resultPoint) );
//       outNormals.append(curveFn.tangent(param)  * mat );
//     }
//   }
//   else {
//     double arclen = curveFn.length(epsilon, &st); er;
//     for (unsigned i = 0; i < npts; i++) {
//       double fracLen = fractions[i] * arclen;
//       param = curveFn.findParamFromLength(fracLen, &st); er;
//       st = curveFn.getPointAtParam(param, resultPoint, MSpace::kObject);
//       outPoints.append( MVector(resultPoint)  );
//       outNormals.append(curveFn.tangent(param)  * mat  );
//     }
//   }


//   return st;
// }



bool strokeFactory::isBounded() const
{
  return true;
}

MBoundingBox strokeFactory::boundingBox() const
{

  MStatus st;

  MObject thisObj = thisMObject();

  // get sample positions from output
  MPlug positionPlug(thisObj, aOutPointsLocal);
  MObject dPositions;
  st = positionPlug.getValue(dPositions); er;
  MFnVectorArrayData fnV(dPositions);
  MVectorArray positions = fnV.array(&st); er;
  unsigned pl = positions.length();
  MBoundingBox bb;
  if (pl > 0) {
    bb = MBoundingBox(positions[0], positions[0]);
  }
  for (unsigned i = 1; i < pl; i++) {
    bb.expand(positions[i]);
  }

  return bb;
}

void strokeFactory::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());
  nodeFn.setName("strokeFactoryShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}



