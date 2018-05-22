
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

#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnMatrixData.h>



#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnEnumAttribute.h>

#include <maya/MFnNurbsCurveData.h>
#include "strokeFactory.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"


// MObject strokeFactory::aNormal;
MObject strokeFactory::aPlaneMatrix;
MObject strokeFactory::aStrokeRotationTexture;
MObject strokeFactory::aStrokeTranslationTexture;
MObject strokeFactory::aStrokeTranslationSampleDistance;

MObject strokeFactory::aInMatrix;
MObject strokeFactory::aCurve;
MObject strokeFactory::aPointDensity;
MObject strokeFactory::aStrokeLength;
MObject strokeFactory::aRandomLengthFactor;
MObject strokeFactory::aRandomOverlapFactor;
MObject strokeFactory::aAttack;
MObject strokeFactory::aLift;
MObject strokeFactory::aElevation;
MObject strokeFactory::aBrushId;
MObject strokeFactory::aPaintId;
MObject strokeFactory::aActive;
MObject strokeFactory::aOverlap;
MObject strokeFactory::aStrokeRotation;
MObject strokeFactory::aStrokeTranslation;
MObject strokeFactory::aPivotFraction;
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
MObject strokeFactory::aPaintOpacity;
MObject strokeFactory::aPaints;

MObject strokeFactory::aOutCounts;
MObject strokeFactory::aOutBrushIds;
MObject strokeFactory::aOutPaintIds;
MObject strokeFactory::aOutPointsWorld;
MObject strokeFactory::aOutPointsLocal;
MObject strokeFactory::aOutNormalsWorld;
MObject strokeFactory::aOutNormalsLocal;
MObject strokeFactory::aOutBrushWidths;
MObject strokeFactory::aOutPaintColors;
MObject strokeFactory::aOutPaintOpacities;


MObject strokeFactory::aDisplayPoints;
MObject strokeFactory::aDisplayNormals;
MObject strokeFactory::aDisplaySegments;
MObject strokeFactory::aSegmentOutlineThickness;

MObject strokeFactory::aPointSize;
MObject strokeFactory::aNormalLength;
MObject strokeFactory::aWireColor;

MObject strokeFactory::aDisplayAttackLift;
MObject strokeFactory::aDisplaySegmentOutlines;
MObject strokeFactory::aStackGap;

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

  MMatrix identity;
  identity.setToIdentity();

  // aNormal = nAttr.create("normal", "nrm", MFnNumericData::k3Double);
  // nAttr.setWritable(true);
  // nAttr.setStorable(true);
  // nAttr.setKeyable(true);
  // st = addAttribute( aNormal ); er;


  aPlaneMatrix = mAttr.create( "planeMatrix", "pmat",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);
  addAttribute(aPlaneMatrix);

  aStrokeRotationTexture = nAttr.create( "strokeRotationTexture", "srtx",
                                         MFnNumericData::kDouble);
  // nAttr.setUsedAsColor(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  addAttribute(aStrokeRotationTexture);

  aStrokeTranslationTexture = nAttr.create( "strokeTranslationTexture", "strx",
                              MFnNumericData::kDouble);
  //nAttr.setUsedAsColor(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  addAttribute(aStrokeTranslationTexture);

  aStrokeTranslationSampleDistance = nAttr.create( "strokeTranslationSampleDistance",
                                     "stsd", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aStrokeTranslationSampleDistance);

  aInMatrix = mAttr.create( "inMatrix", "imat",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);
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

  aRandomLengthFactor = nAttr.create( "randomLengthFactor", "rlfc",
                                      MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  nAttr.setDefault(0.2);
  addAttribute(aRandomLengthFactor);

  aRandomOverlapFactor = nAttr.create( "randomOverlapFactor", "rofc",
                                       MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  nAttr.setDefault(0.2);
  addAttribute(aRandomOverlapFactor);


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

  aStrokeRotation = uAttr.create( "strokeRotation", "srot", MFnUnitAttribute::kAngle );
  uAttr.setStorable(true);
  uAttr.setReadable(true);
  uAttr.setMin(-mayaMath::single_pi);
  uAttr.setMax(mayaMath::single_pi);

  aStrokeTranslation = nAttr.create("strokeTranslation", "strn", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );

  aPivotFraction = nAttr.create("pivotFraction", "pfrc", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setMin(0.0);
  nAttr.setMax(1.0);
  nAttr.setDefault(0.5);


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

  aActive = nAttr.create( "active", "act", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);

  aOverlap  = nAttr.create("overlap", "ovlp", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.1 );

  aCurves = cAttr.create("curves", "crvs");
  cAttr.addChild(aCurve);
  cAttr.addChild(aPointDensity);
  cAttr.addChild(aStrokeLength);
  cAttr.addChild(aRandomLengthFactor);
  cAttr.addChild(aRandomOverlapFactor);
  cAttr.addChild(aAttack);
  cAttr.addChild(aLift);
  cAttr.addChild(aElevation);
  cAttr.addChild(aStrokeRotation);
  cAttr.addChild(aStrokeTranslation);
  cAttr.addChild(aPivotFraction);
  cAttr.addChild(aBrushId);
  cAttr.addChild(aPaintId);
  cAttr.addChild(aOverlap);
  cAttr.addChild(aActive);

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



  aPaintOpacity = nAttr.create("paintOpacity", "pvis", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault(0.5);


  aPaints = cAttr.create("paints", "pts");
  cAttr.addChild(aPaintColor);
  cAttr.addChild(aPaintName);
  cAttr.addChild(aPaintOpacity);
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


  aDisplaySegments = nAttr.create( "displayPaint", "dpt", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplaySegments );


  aDisplayAttackLift = nAttr.create( "displayAttackLift", "dal", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayAttackLift );


  aDisplaySegmentOutlines = nAttr.create( "displayPaintOutlines", "dpo",
                                          MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplaySegmentOutlines );

  aSegmentOutlineThickness = nAttr.create( "paintOutlineThickness", "polt",
                             MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(3.0);
  addAttribute(aSegmentOutlineThickness);


  aPointSize = nAttr.create( "pointSize", "psi", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aPointSize);

  aStackGap = nAttr.create( "stackGap", "sgap", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1);
  nAttr.setDefault(0);
  addAttribute(aStackGap);


  aNormalLength = nAttr.create( "normalLength", "nln", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aNormalLength);



  aWireColor = nAttr.createColor( "lineColor", "lcl");
  nAttr.setStorable(true);
  nAttr.setKeyable(false);
  nAttr.setConnectable(false);
  addAttribute(aWireColor);



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

  aOutNormalsLocal = tAttr.create("outNormalsLocal", "otl", MFnData::kVectorArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutNormalsLocal ); er;

  aOutNormalsWorld = tAttr.create("outNormalsWorld", "otw", MFnData::kVectorArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutNormalsWorld ); er;

  aOutBrushWidths = tAttr.create("outBrushWidths", "obrw", MFnData::kDoubleArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutBrushWidths ); er;

  aOutPaintColors = tAttr.create("outPaintColors", "optc", MFnData::kVectorArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutPaintColors ); er;

  aOutPaintOpacities = tAttr.create("outPaintOpacities", "opop", MFnData::kDoubleArray,
                                    &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutPaintOpacities ); er;





  // aOutMesh = tAttr.create( "outMesh", "out", MFnData::kMesh, &st ); er
  // tAttr.setReadable(true);
  // tAttr.setStorable(false);
  // st = addAttribute(aOutMesh); er;

  st = attributeAffects(aInMatrix, aOutPointsLocal);
  st = attributeAffects(aCurve, aOutPointsLocal);
  st = attributeAffects(aPointDensity, aOutPointsLocal);
  st = attributeAffects(aStrokeLength, aOutPointsLocal);
  st = attributeAffects(aRandomLengthFactor, aOutPointsLocal);
  st = attributeAffects(aAttack, aOutPointsLocal);
  st = attributeAffects(aLift, aOutPointsLocal);
  st = attributeAffects(aElevation, aOutPointsLocal);
  st = attributeAffects(aCurves, aOutPointsLocal);
  // st = attributeAffects(aNormal, aOutPointsLocal);
  st = attributeAffects(aBrushTip, aOutPointsLocal);
  st = attributeAffects(aOverlap, aOutPointsLocal);
  st = attributeAffects(aRandomOverlapFactor, aOutPointsLocal);
  st = attributeAffects(aStrokeRotation, aOutPointsLocal);
  st = attributeAffects(aStrokeTranslation, aOutPointsLocal);
  st = attributeAffects(aActive, aOutPointsLocal);
  st = attributeAffects(aPlaneMatrix, aOutPointsLocal);
  st = attributeAffects(aStrokeRotationTexture, aOutPointsLocal);
  st = attributeAffects(aStrokeTranslationTexture, aOutPointsLocal);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutPointsLocal);
  st = attributeAffects(aPivotFraction, aOutPointsLocal);


  st = attributeAffects(aInMatrix, aOutPointsWorld);
  st = attributeAffects(aCurve, aOutPointsWorld);
  st = attributeAffects(aPointDensity, aOutPointsWorld);
  st = attributeAffects(aStrokeLength, aOutPointsWorld);
  st = attributeAffects(aRandomLengthFactor, aOutPointsWorld);
  st = attributeAffects(aAttack, aOutPointsWorld);
  st = attributeAffects(aLift, aOutPointsWorld);
  st = attributeAffects(aElevation, aOutPointsWorld);
  st = attributeAffects(aCurves, aOutPointsWorld);
  // st = attributeAffects(aNormal, aOutPointsWorld);
  st = attributeAffects(aBrushTip, aOutPointsWorld);
  st = attributeAffects(aOverlap, aOutPointsWorld);
  st = attributeAffects(aRandomOverlapFactor, aOutPointsWorld);
  st = attributeAffects(aStrokeRotation, aOutPointsWorld);
  st = attributeAffects(aStrokeTranslation, aOutPointsWorld);
  st = attributeAffects(aActive, aOutPointsWorld);
  st = attributeAffects(aPlaneMatrix, aOutPointsWorld);
  st = attributeAffects(aStrokeRotationTexture, aOutPointsWorld);
  st = attributeAffects(aStrokeTranslationTexture, aOutPointsWorld);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutPointsWorld);
  st = attributeAffects(aPivotFraction, aOutPointsWorld);


  st = attributeAffects(aInMatrix, aOutNormalsLocal);
  st = attributeAffects(aCurve, aOutNormalsLocal);
  st = attributeAffects(aPointDensity, aOutNormalsLocal);
  st = attributeAffects(aStrokeLength, aOutNormalsLocal);
  st = attributeAffects(aRandomLengthFactor, aOutNormalsLocal);
  st = attributeAffects(aAttack, aOutNormalsLocal);
  st = attributeAffects(aLift, aOutNormalsLocal);
  st = attributeAffects(aElevation, aOutNormalsLocal);
  st = attributeAffects(aCurves, aOutNormalsLocal);
  // st = attributeAffects(aNormal, aOutNormalsLocal);
  st = attributeAffects(aBrushTip, aOutNormalsLocal);
  st = attributeAffects(aOverlap, aOutNormalsLocal);
  st = attributeAffects(aRandomOverlapFactor, aOutNormalsLocal);
  st = attributeAffects(aStrokeRotation, aOutNormalsLocal);
  st = attributeAffects(aStrokeTranslation, aOutNormalsLocal);
  st = attributeAffects(aActive, aOutNormalsLocal);
  st = attributeAffects(aPlaneMatrix, aOutNormalsLocal);
  st = attributeAffects(aStrokeRotationTexture, aOutNormalsLocal);
  st = attributeAffects(aStrokeTranslationTexture, aOutNormalsLocal);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutNormalsLocal);
  st = attributeAffects(aPivotFraction, aOutNormalsLocal);


  st = attributeAffects(aInMatrix, aOutNormalsWorld);
  st = attributeAffects(aCurve, aOutNormalsWorld);
  st = attributeAffects(aPointDensity, aOutNormalsWorld);
  st = attributeAffects(aStrokeLength, aOutNormalsWorld);
  st = attributeAffects(aRandomLengthFactor, aOutNormalsWorld);
  st = attributeAffects(aAttack, aOutNormalsWorld);
  st = attributeAffects(aLift, aOutNormalsWorld);
  st = attributeAffects(aElevation, aOutNormalsWorld);
  st = attributeAffects(aCurves, aOutNormalsWorld);
  // st = attributeAffects(aNormal, aOutNormalsWorld);
  st = attributeAffects(aBrushTip, aOutNormalsWorld);
  st = attributeAffects(aOverlap, aOutNormalsWorld);
  st = attributeAffects(aRandomOverlapFactor, aOutNormalsWorld);
  st = attributeAffects(aStrokeRotation, aOutNormalsWorld);
  st = attributeAffects(aStrokeTranslation, aOutNormalsWorld);
  st = attributeAffects(aActive, aOutNormalsWorld);
  st = attributeAffects(aPlaneMatrix, aOutNormalsWorld);
  st = attributeAffects(aStrokeRotationTexture, aOutNormalsWorld);
  st = attributeAffects(aStrokeTranslationTexture, aOutNormalsWorld);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutNormalsWorld);
  st = attributeAffects(aPivotFraction, aOutNormalsWorld);

  st = attributeAffects(aCurve, aOutCounts);
  st = attributeAffects(aPointDensity, aOutCounts);
  st = attributeAffects(aStrokeLength, aOutCounts);
  st = attributeAffects(aOverlap, aOutCounts);
  st = attributeAffects(aRandomOverlapFactor, aOutCounts);
  st = attributeAffects(aRandomLengthFactor, aOutCounts);
  st = attributeAffects(aCurves, aOutCounts);
  st = attributeAffects(aActive, aOutCounts);

  st = attributeAffects(aCurve, aOutBrushIds);
  st = attributeAffects(aPointDensity, aOutBrushIds);
  st = attributeAffects(aStrokeLength, aOutBrushIds);
  st = attributeAffects(aOverlap, aOutBrushIds);
  st = attributeAffects(aRandomOverlapFactor, aOutBrushIds);
  st = attributeAffects(aRandomLengthFactor, aOutBrushIds);
  st = attributeAffects(aCurves, aOutBrushIds);
  st = attributeAffects(aActive, aOutBrushIds);

  st = attributeAffects(aCurve, aOutPaintIds);
  st = attributeAffects(aPointDensity, aOutPaintIds);
  st = attributeAffects(aStrokeLength, aOutPaintIds);
  st = attributeAffects(aOverlap, aOutPaintIds);
  st = attributeAffects(aRandomOverlapFactor, aOutPaintIds);
  st = attributeAffects(aRandomLengthFactor, aOutPaintIds);
  st = attributeAffects(aCurves, aOutPaintIds);
  st = attributeAffects(aActive, aOutPaintIds);

  st = attributeAffects(aCurve, aOutBrushWidths);
  st = attributeAffects(aPointDensity, aOutBrushWidths);
  st = attributeAffects(aStrokeLength, aOutBrushWidths);
  st = attributeAffects(aOverlap, aOutBrushWidths);
  st = attributeAffects(aRandomOverlapFactor, aOutBrushWidths);
  st = attributeAffects(aRandomLengthFactor, aOutBrushWidths);
  st = attributeAffects(aCurves, aOutBrushWidths);
  st = attributeAffects(aBrushWidth, aOutBrushWidths);
  st = attributeAffects(aActive, aOutBrushWidths);

  st = attributeAffects(aCurve, aOutPaintColors);
  st = attributeAffects(aPointDensity, aOutPaintColors);
  st = attributeAffects(aStrokeLength, aOutPaintColors);
  st = attributeAffects(aOverlap, aOutPaintColors);
  st = attributeAffects(aRandomOverlapFactor, aOutPaintColors);
  st = attributeAffects(aRandomLengthFactor, aOutPaintColors);
  st = attributeAffects(aCurves, aOutPaintColors);
  st = attributeAffects(aPaintColor, aOutPaintColors);
  st = attributeAffects(aActive, aOutPaintColors);


  st = attributeAffects(aCurve, aOutPaintOpacities);
  st = attributeAffects(aPointDensity, aOutPaintOpacities);
  st = attributeAffects(aStrokeLength, aOutPaintOpacities);
  st = attributeAffects(aOverlap, aOutPaintOpacities);
  st = attributeAffects(aRandomOverlapFactor, aOutPaintOpacities);
  st = attributeAffects(aRandomLengthFactor, aOutPaintOpacities);
  st = attributeAffects(aCurves, aOutPaintOpacities);
  st = attributeAffects(aPaintOpacity, aOutPaintOpacities);
  st = attributeAffects(aActive, aOutPaintOpacities);



  return ( MS::kSuccess );

}


// unsigned int strokeFactory::getStrokeBoundaries(
//   const MObject &curve, double maxStrokeLength, dou,
//   MVectorArray &result
// ) const  {
//   MStatus st = MS::kSuccess;

//   MFnNurbsCurve curveFn(curve, &st);
//   if (st.error()) { return 0; }
//   double curveLen = curveFn.length(epsilon);
//   unsigned nstrokes = ceil(curveLen / maxStrokeLength);
//   double strokeLength  = curveLen / nstrokes;

//   for (int i = 0; i < nstrokes; ++i)
//   {
//     double startDist = i / double(nstrokes) * curveLen;
//     double endDist = startDist + strokeLength;
//     result.append(MVector(startDist, endDist));
//   }
//   return result.length();

// }



unsigned int strokeFactory::getStrokeBoundaries(
  const MObject &curve, double strokeLength, double randomLengthFactor, double overlap,
  double randomOverlapFactor, MVectorArray &result
) const  {
  MStatus st = MS::kSuccess;

  MFnNurbsCurve curveFn(curve, &st);
  if (st.error()) { return 0; }
  double curveLen = curveFn.length(epsilon);

  if (randomLengthFactor < 0) { randomLengthFactor = 0; }
  if (randomLengthFactor > .999) { randomLengthFactor = .999; }

  if (randomOverlapFactor < 0) { randomOverlapFactor = 0; }
  if (randomOverlapFactor > .999) { randomOverlapFactor = .999; }

  double maxLengthRand =   randomLengthFactor * strokeLength * 2;
  double maxOverlapRand =   randomOverlapFactor * overlap * 2;

  double   lengthAdd = (drand48() - 0.5) * maxLengthRand ;
  // overlapAdd = (drand48() -0.5) * maxOverlapRand;
  // startDist = startDist-lengthAdd;
  // endDist = endDist + lengthAdd;


  double startDist = 0;
  double endDist = strokeLength + lengthAdd;

  do {

    result.append(MVector(startDist, endDist));

    double lengthAdd = (drand48() - 0.5) * maxLengthRand;
    double overlapAdd = (drand48() - 0.5) * maxOverlapRand;

    startDist = endDist - (overlap + overlapAdd);
    if (startDist < 0) { startDist = 0; }
    if (startDist > curveLen) { startDist = curveLen - 0.0001; }

    endDist = startDist + strokeLength + lengthAdd;
    if (endDist < 0) { endDist = 0.0001;}
    if (endDist > curveLen) { endDist = curveLen;}
    result.append(MVector(startDist, endDist));
  }
  while ( endDist < curveLen );
  return result.length();
}



MStatus strokeFactory::getBrushes(MDataBlock &data,
                                  std::map<short, Brush> &brushes ) const {
  MStatus st;
  MArrayDataHandle hBrushes = data.inputArrayValue(aBrushes, &st ); ert;
  unsigned nPlugs = hBrushes.elementCount();
  for (unsigned i = 0; i < nPlugs; i++, hBrushes.next()) {
    short index = short(hBrushes.elementIndex(&st));
    if (st.error()) {
      continue;
    }
    MDataHandle hComp = hBrushes.inputValue(&st );

    double width =  hComp.child(strokeFactory::aBrushWidth).asDouble() ;
    double tip = hComp.child(strokeFactory::aBrushTip).asDouble() ;
    MVector tcp = hComp.child(strokeFactory::aBrushTcp).asVector() ;
    MString name = hComp.child(strokeFactory::aBrushName).asString();

    brushes[index] = Brush(index, width, tip, tcp, name);;

  }
  return MS::kSuccess;
}

MStatus strokeFactory::getPaints(MDataBlock &data,
                                 std::map<short, Paint> &paints ) const {
  MStatus st;
  MArrayDataHandle hPaints = data.inputArrayValue(aPaints, &st ); ert;
  unsigned nPlugs = hPaints.elementCount();
  // paints[-1] = Paint();
  for (unsigned i = 0; i < nPlugs; i++, hPaints.next()) {

    short index = short(hPaints.elementIndex(&st));
    if (st.error()) {
      continue;
    }

    MDataHandle hComp = hPaints.inputValue(&st );

    MFloatVector vc =  hComp.child(strokeFactory::aPaintColor).asFloatVector() ;
    MColor color(vc.x, vc.y, vc.z);

    double opacity = hComp.child(strokeFactory::aPaintOpacity).asDouble() ;
    MString name = hComp.child(strokeFactory::aPaintName).asString();

    paints[index] = Paint(index, color, opacity, name);

  }
  return MS::kSuccess;
}


MStatus strokeFactory::getStrokes(MDataBlock &data,
                                  const MVector &normal,
                                  const   std::map<short, Brush> brushes,
                                  const   std::map<short, Paint> paints,
                                  std::vector<stroke> &strokes ) const {
  MStatus st;
  MArrayDataHandle hCurves = data.inputArrayValue(aCurves, &st ); ert;
  unsigned nPlugs = hCurves.elementCount();


  for (unsigned i = 0; i < nPlugs; i++, hCurves.next()) {

    srand48(hCurves.elementIndex());

    MDataHandle hComp = hCurves.inputValue(&st );
    if (! hComp.child(aActive).asBool()) {
      continue;
    }
    MObject  dCurve =  hComp.child(strokeFactory::aCurve).asNurbsCurveTransformed();
    MFnNurbsCurve curveFn(dCurve, &st);
    if ( st.error()) {
      continue;
    }

    double pointDensity =  hComp.child(strokeFactory::aPointDensity).asDouble() ;
    if (pointDensity < 0) { pointDensity = 0.001; }

    double strokeLength = hComp.child( aStrokeLength).asDouble();
    double randomLengthFactor = hComp.child( aRandomLengthFactor).asDouble();
    double randomOverlapFactor = hComp.child( aRandomOverlapFactor).asDouble();
    double elevation  =  hComp.child( aElevation).asAngle().value();
    double overlap  =  hComp.child( aOverlap).asDouble();

    double3   &attack = hComp.child( aAttack).asDouble3();
    double3   &lift = hComp.child( aLift).asDouble3();

    short brushId =  hComp.child( aBrushId).asShort();
    short paintId =  hComp.child( aPaintId).asShort();

    double rotation = hComp.child(aStrokeRotation).asAngle().value();
    double translation = hComp.child(aStrokeTranslation).asDouble();
    double pivotFraction = hComp.child(aPivotFraction).asDouble();


    MVectorArray boundaries;
    unsigned numStrokes =  getStrokeBoundaries(dCurve, strokeLength, randomLengthFactor,
                           overlap, randomOverlapFactor, boundaries);

    // double tipDist = 0;
    std::map<short, Brush>::const_iterator brushIter = brushes.find(brushId);
    Brush brush;


    if (brushIter !=  brushes.end()) {
      brush = brushIter->second;
    }

    std::map<short, Paint>::const_iterator paintIter = paints.find(paintId);
    Paint paint;
    if (paintIter !=  paints.end()) {
      paint = paintIter->second;
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
                          rotation,
                          translation,
                          pivotFraction,
                          brush,
                          paint,
                          dCurve));
    }
  }
  return MS::kSuccess;
}

MStatus strokeFactory::compute( const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  MString method("strokeFactory::compute");
  // cerr << "HERE compute" << endl;
  if (!(
        plug == aOutPointsLocal ||
        plug == aOutPointsWorld ||
        plug == aOutNormalsLocal ||
        plug == aOutNormalsWorld ||
        plug == aOutCounts ||
        plug == aOutBrushIds ||
        plug == aOutPaintIds ||
        plug == aOutBrushWidths ||
        plug == aOutPaintColors ||
        plug == aOutPaintOpacities
      )) { return ( MS::kUnknownParameter ); }




  // MArrayDataHandle hCurves = data.inputArrayValue(aCurves, &st );
  MVectorArray outPoints; outPoints.clear();
  MVectorArray outNormals; outNormals.clear();
  MIntArray outCounts; outCounts.clear();
  MIntArray outBrushIds; outBrushIds.clear();
  MIntArray outPaintIds; outPaintIds.clear();
  MDoubleArray outBrushWidths; outBrushWidths.clear();
  MVectorArray outPaintColors; outPaintColors.clear();
  MDoubleArray outPaintOpacities; outPaintOpacities.clear();

  MMatrix wm = data.inputValue(strokeFactory::aInMatrix).asMatrix();
  // JPMDBG;
  MMatrix planeMatrix = data.inputValue(strokeFactory::aPlaneMatrix).asMatrix();
  MMatrix inversePlaneMatrix = planeMatrix.inverse();
  // MVector normal = data.inputValue(aNormal).asVector();
  MVector normal = (MVector::zAxis * planeMatrix).normal();
  // JPMDBG;

  double sampleDistance = data.inputValue(aStrokeTranslationSampleDistance).asDouble();
  // JPMDBG;

  std::map<short, Brush> brushes;
  st = getBrushes(data, brushes) ;

  std::map<short, Paint> paints;
  st = getPaints(data, paints) ;

  std::vector<stroke>  strokes;
  st = getStrokes(data, normal, brushes, paints, strokes) ;
  // JPMDBG;



  // aStrokeRotationTexture
  // aStrokeTranslationTexture
  // aStrokeTranslationSampleDistance
  //  sample rotation texture and translation gradient

  std::vector<stroke>::const_iterator citer;
  std::vector<stroke>::iterator iter;
  // JPMDBG;

  unsigned nStrokes = strokes.size();
  MFloatArray uVals(nStrokes);
  MFloatArray vVals(nStrokes);
  // JPMDBG;

  citer = strokes.begin();
  for (unsigned i = 0; citer != strokes.end(); citer++, i++) {
    float &u = uVals[i];
    float &v = vVals[i];
    citer->getPivotUVs(inversePlaneMatrix, u, v);
  }
  MFloatArray rotations;
  MFloatVectorArray translations;
  // JPMDBG;


  st = sampleUVTexture(strokeFactory::aStrokeRotationTexture, uVals, vVals, rotations);
  if (! st.error()) {
    iter = strokes.begin();
    for (unsigned i = 0; iter != strokes.end(); iter++, i++) {
      iter->rotate(rotations[i], normal);
    }
  }
  // JPMDBG;

  st = sampleUVGradient(strokeFactory::aStrokeTranslationTexture, sampleDistance, uVals,
                        vVals,
                        translations);
  if (! st.error()) {
    iter = strokes.begin();
    for (unsigned i = 0; iter != strokes.end(); iter++, i++) {
      iter->translate(translations[i], normal);
    }
  }
  // JPMDBG;


  citer = strokes.begin();
  for (; citer != strokes.end(); citer++) {
    const MVectorArray &pts = citer->points();
    const MVectorArray &nrm = citer->normals();
    const unsigned len = pts.length();

    outCounts.append(len);
    outBrushIds.append(citer->brush().id);
    outPaintIds.append(citer->paint().id);
    outBrushWidths.append(citer->brush().width);
    const MColor &c = citer->paint().color;
    outPaintColors.append(MVector(c.r, c.g, c.b));
    outPaintOpacities.append(citer->paint().opacity);

    for (int i = 0; i < len; ++i)
    {
      outPoints.append(pts[i]);
      outNormals.append(nrm[i]);
    }
  }

  // JPMDBG;

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


  MDataHandle hOutBrushWidths = data.outputValue(aOutBrushWidths);
  MFnDoubleArrayData fnOutBrushWidths;
  MObject dOutBrushWidths = fnOutBrushWidths.create(outBrushWidths);
  hOutBrushWidths.set(dOutBrushWidths);
  st = data.setClean( aOutBrushWidths ); er;


  MDataHandle hOutPaintColors = data.outputValue(aOutPaintColors);
  MFnVectorArrayData fnOutPaintColors;
  MObject dOutPaintColors = fnOutPaintColors.create(outPaintColors);
  hOutPaintColors.set(dOutPaintColors);
  st = data.setClean( aOutPaintColors ); er;

  MDataHandle hOutPaintOpacities = data.outputValue(aOutPaintOpacities);
  MFnDoubleArrayData fnOutPaintOpacities;
  MObject dOutPaintOpacities = fnOutPaintOpacities.create(outPaintOpacities);
  hOutPaintOpacities.set(dOutPaintOpacities);
  st = data.setClean( aOutPaintOpacities ); er;


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

  // cerr << "STROKEFACTORY::DRAW" << endl;
  MStatus st;

  MObject thisObj = thisMObject();

  MFnVectorArrayData fnV;
  MFnDoubleArrayData fnD;
  MFnIntArrayData fnI;
  // cerr << "HERE draw" << endl;

  MFnMatrixData fnX;
  MObject dX;
  MPlug plugX( thisObj, aPlaneMatrix );
  st = plugX.getValue(dX);
  MMatrix pmat;
  if (st.error()) {
    pmat.setToIdentity();
  }
  else {
    fnX.setObject(dX);
    pmat = fnX.matrix(&st);
    if (st.error()) {
      pmat.setToIdentity();
    }
  }
  // MMatrix ipmat = pmat.inverse();
  MVector planeNormal = MVector::zAxis * pmat;

  // get sample positions from output
  MPlug positionPlug(thisObj, aOutPointsLocal);
  MObject dPositions;
  st = positionPlug.getValue(dPositions); er;
  fnV.setObject(dPositions);
  MVectorArray tmpPositions = fnV.array(&st); er;
  unsigned pl = tmpPositions.length();

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);
  bool doStack = stackGap > 0;

  MVectorArray positions;
  if (stackGap > 0) {
    MVectorArray stackPositions(pl);
    for (unsigned i = 0; i < pl; i++) {
      stackPositions.set(tmpPositions[i] + (planeNormal * stackGap * i), i);
    }
    positions = stackPositions;
  }
  else {
    positions = tmpPositions;
  }

  MPlug normalsPlug(thisObj, aOutNormalsLocal);
  MObject dNormals;
  st = normalsPlug.getValue(dNormals); er;
  fnV.setObject(dNormals);
  MVectorArray normals = fnV.array(&st); er;

  MPlug countsPlug(thisObj, aOutCounts);
  MObject dCounts;
  st = countsPlug.getValue(dCounts); er;
  fnI.setObject(dCounts);
  MIntArray counts = fnI.array(&st); er;
  unsigned numStrokes = counts.length();

  MPlug widthsPlug(thisObj, aOutBrushWidths);
  MObject dBrushWidths;
  st = widthsPlug.getValue(dBrushWidths); er;
  fnD.setObject(dBrushWidths);
  MDoubleArray brushWidths = fnD.array(&st); er;

  MPlug paintColorsPlug(thisObj, aOutPaintColors);
  MObject dPaintColors;
  st = paintColorsPlug.getValue(dPaintColors); er;
  fnV.setObject(dPaintColors);
  MVectorArray paintColors = fnV.array(&st); er;

  MPlug paintOpacitiesPlug(thisObj, aOutPaintOpacities);
  MObject dPaintOpacities;
  st = paintOpacitiesPlug.getValue(dPaintOpacities); er;
  fnD.setObject(dPaintOpacities);
  MDoubleArray paintOpacities = fnD.array(&st); er;



  bool doDisplayPoints;
  bool doDisplayNormals;
  bool doDisplaySegments;
  bool doDisplayAttackLift;
  bool doDisplaySegmentOutlines;

  MPlug(thisObj, aDisplayPoints).getValue(doDisplayPoints);
  MPlug(thisObj, aDisplayNormals).getValue(doDisplayNormals);
  MPlug(thisObj, aDisplaySegments).getValue(doDisplaySegments);
  MPlug(thisObj, aDisplayAttackLift).getValue(doDisplayAttackLift);
  MPlug(thisObj, aDisplaySegmentOutlines).getValue(doDisplaySegmentOutlines);


  MPlug wireColorPlug (thisObj, aWireColor);
  float wireColorRed, wireColorGreen, wireColorBlue;
  wireColorPlug.child(0).getValue(wireColorRed);
  wireColorPlug.child(1).getValue(wireColorGreen);
  wireColorPlug.child(2).getValue(wireColorBlue);

  MColor wireColor( MColor::kRGB, wireColorRed, wireColorGreen, wireColorBlue);

  double thickness;
  MPlug(thisObj, aSegmentOutlineThickness).getValue(thickness);


  view.beginGL();



  if (doDisplayPoints)
  {
    MPlug pointSizePlug (thisObj, aPointSize);
    double pointSize;
    pointSizePlug.getValue(pointSize);

    view.setDrawColor(wireColor );

    glPushAttrib(GL_CURRENT_BIT);
    glPointSize(float(pointSize));

    glBegin( GL_POINTS );

    unsigned i = 0;
    for (int j = 0; j < numStrokes; ++j)
    {
      unsigned numPoints = counts[j];
      if (! doDisplayAttackLift) {i++;  numPoints -= 2;}
      for (int k = 0; k < numPoints ; ++k)
      {
        glVertex3f(float(positions[i].x), float(positions[i].y), float(positions[i].z));
        i++;
      }
      if (! doDisplayAttackLift) { i++; }
    }

    glEnd();
    glPopAttrib();
  }





  if (doDisplaySegments)
  {

    glPushAttrib(GL_CURRENT_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    unsigned i = 0;
    for (int j = 0; j < numStrokes; ++j)
    {
      unsigned numPoints = counts[j];
      double width = brushWidths[j] * 0.5;
      const MVector    &pcv = paintColors[j];


      float opac = float(paintOpacities[j]);
      MColor pcolor = MColor(pcv.x, pcv.y, pcv.z, opac);

      glColor4f(pcolor.r , pcolor.g , pcolor.b, opac);

      glBegin(GL_TRIANGLE_STRIP);

      unsigned first = 0;
      unsigned end = numPoints;

      if (! doDisplayAttackLift) {i++;  numPoints -= 2;}

      for (int k = 0; k < numPoints ; ++k)
      {
        MVector side;
        if (k == 0)  {
          side = binormal(positions[i], positions[(i + 1)], planeNormal) * width;
        }
        else if (k == (numPoints - 1)) {
          side = binormal(positions[(i - 1)], positions[i], planeNormal) * width;
        }
        else {
          MVector side1 = binormal(positions[(i - 1)], positions[i], planeNormal);
          MVector side2  = binormal(positions[i], positions[(i + 1)], planeNormal);
          side = (side1 + side2).normal() * width;

        }

        MFloatVector a = MFloatVector(positions[i] + side);
        MFloatVector b = MFloatVector(positions[i] - side);
        glVertex3f( a.x , a.y , a.z );
        glVertex3f( b.x , b.y, b.z);
        i++;
      }

      if (! doDisplayAttackLift) { i++; }

      glEnd();
    }
    glPopAttrib();
  }





  if (doDisplaySegmentOutlines)
  {

    glPushAttrib(GL_LINE_BIT);

    unsigned i = 0;
    for (int j = 0; j < numStrokes; ++j)
    {
      unsigned numPoints = counts[j];
      double width = brushWidths[j] * 0.5;
      view.setDrawColor(wireColor );

      glLineWidth(GLfloat(thickness));
      glBegin(GL_LINES);

      MFloatVector last_a;
      MFloatVector last_b;

      if (! doDisplayAttackLift) {i++;  numPoints -= 2;}

      for (int k = 0; k < numPoints ; ++k)
      {
        MVector side;
        if (k == 0)  {
          side = binormal(positions[i], positions[(i + 1)], planeNormal) * width;
        }
        else if (k == (numPoints - 1)) {
          side = binormal(positions[(i - 1)], positions[i], planeNormal) * width;
        }
        else {
          MVector side1 = binormal(positions[(i - 1)], positions[i], planeNormal);
          MVector side2  = binormal(positions[i], positions[(i + 1)], planeNormal);
          side = (side1 + side2).normal() * width;
        }


        MFloatVector a = MFloatVector(positions[i] + side);
        MFloatVector b = MFloatVector(positions[i] - side);
        if (k > 0) {
          glVertex3f( last_a.x , last_a.y, last_a.z);
          glVertex3f( a.x , a.y , a.z );
          glVertex3f( last_b.x , last_b.y, last_b.z);
          glVertex3f( b.x , b.y , b.z );
        }
        last_a = a;
        last_b = b;
        i++;
      }
      if (! doDisplayAttackLift) { i++; }

      glEnd();
    }
    glPopAttrib();
  }






  if (doDisplayNormals)
  {

    double lineLength;
    MPlug(thisObj, aNormalLength).getValue(lineLength);

    view.setDrawColor(wireColor );

    glPushAttrib(GL_LINE_BIT);
    glLineWidth(GLfloat(thickness));
    glBegin( GL_LINES );


    unsigned i = 0;
    for (int j = 0; j < numStrokes; ++j)
    {
      unsigned numPoints = counts[j];
      if (! doDisplayAttackLift) {i++;  numPoints -= 2;}
      for (int k = 0; k < numPoints ; ++k)
      {
        MFloatVector start(positions[i]);
        MFloatVector end((normals[i].normal() * lineLength) + positions[i]);

        glVertex3f( start.x , start.y , start.z );
        glVertex3f( end.x , end.y, end.z);
        i++;
      }
      if (! doDisplayAttackLift) { i++; }
    }

    glEnd();
    glPopAttrib();
  }


  view.endGL();

}



MStatus strokeFactory::getTextureName(const MObject &attribute,
                                      MString &name) const {
  MStatus st;
  MPlugArray plugArray;
  MPlug plug(thisMObject(), attribute);
  bool hasConnection = plug.connectedTo(plugArray, 1, 0, &st); ert;
  if (! hasConnection) { return MS::kUnknownParameter; }
  name = plugArray[0].name(&st);
  return MS::kSuccess;
}



MStatus strokeFactory::sampleUVTexture(const MObject &attribute,  MFloatArray &uVals,
                                       MFloatArray &vVals, MFloatArray &result) const {

  MStatus st;
  MString plugName;
  st = getTextureName(attribute, plugName);
  if (st.error()) {   return MS::kFailure; }
  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();
  MFloatVectorArray transparencies;
  MFloatVectorArray colors;

  int n = uVals.length();

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat,
        0, &uVals, &vVals, 0, 0, 0, 0, 0, colors, transparencies); ert;

  result.setLength(n);
  for (int i = 0; i < n; ++i)
  {
    result.set(colors[i].x, i);
  }
  return MS::kSuccess;
}


MStatus strokeFactory::sampleUVGradient(const MObject &attribute, float sampleDistance,
                                        MFloatArray &uVals,
                                        MFloatArray &vVals, MFloatVectorArray &result) const {
  MStatus st;
  MString plugName;
  st = getTextureName(attribute, plugName);
  if (st.error()) {   return MS::kFailure; }

  unsigned n = uVals.length();
  result.setLength(n);

  if (sampleDistance < 0.00001) {
    sampleDistance = 0.00001;
  }
  float sampleRecip = 1.0f / sampleDistance;

  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();

  MFloatVectorArray transparencies;
  MFloatVectorArray colors;


  MFloatArray uOffsetVals(n);
  MFloatArray vOffsetVals(n);

  for (unsigned i = 0; i < n; i++) {
    uOffsetVals.set( (uVals[i] + sampleDistance) , i);
    vOffsetVals.set( (vVals[i] + sampleDistance) , i);
  }


  MFloatVectorArray uOffsetColors;
  MFloatVectorArray vOffsetColors;

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat, 0, &uVals,
        &vVals, 0, 0, 0, 0, 0, colors, transparencies); ert;
  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat, 0,
        &uOffsetVals, &vVals, 0, 0, 0, 0, 0, uOffsetColors, transparencies); ert;
  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat, 0, &uVals,
        &vOffsetVals, 0, 0, 0, 0, 0, vOffsetColors, transparencies); ert;

  for (unsigned i = 0; i < n; i++) {
    MFloatVector grad( (uOffsetColors[i].x - colors[i].x) ,
                       (vOffsetColors[i].x - colors[i].x) , 0.0);
    result.set(grad * sampleRecip, i);
  }
  return MS::kSuccess;
}


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



