
/*
 * strokeFactory.cpp
 * src
 *
 * Created by Julian Mann on 29/04/2018.
 *
 * Copyright (c) 2018 Julian Mann
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

#include <maya/MFnMessageAttribute.h>


#include <maya/MArrayDataHandle.h>
#include <maya/MAngle.h>

#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>

#include <maya/MFnVectorArrayData.h>

#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MTransformationMatrix.h>



#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnEnumAttribute.h>

#include <maya/MFnNurbsCurveData.h>
#include "strokeFactory.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);


MObject strokeFactory::aPlaneMatrix;
MObject strokeFactory::aStrokeRotationTexture;
MObject strokeFactory::aStrokeTranslationTexture;
MObject strokeFactory::aStrokeTranslationSampleDistance;
MObject strokeFactory::aStrokeCountFactor;
MObject strokeFactory::aRotateOrder;
MObject strokeFactory::aOutputUnit;
// MObject strokeFactory::aStrokeApproachDistance;
MObject strokeFactory::aClusterApproachObject;
MObject strokeFactory::aToolChangeApproachObject;
MObject strokeFactory::aHomeApproachObject;

MObject strokeFactory::aLinearSpeed; // cm/sec
MObject strokeFactory::aAngularSpeed; // per sec
MObject strokeFactory::aApproximationDistance;
MObject strokeFactory::aInMatrix;

MObject strokeFactory::aCurve;
MObject strokeFactory::aSubcurveMin;
MObject strokeFactory::aSubcurveMax;
MObject strokeFactory::aSubcurve;
MObject strokeFactory::aPointDensity;
MObject strokeFactory::aStrokeLength;
MObject strokeFactory::aRandomLengthFactor;
MObject strokeFactory::aRandomOverlapFactor;

MObject strokeFactory::aRepeats;
MObject strokeFactory::aRepeatOffset;
MObject strokeFactory::aRepeatMirror;
MObject strokeFactory::aRepeatOscillate;

MObject strokeFactory::aRepeatAdvance;

MObject strokeFactory::aBrushId;
MObject strokeFactory::aPaintId;
MObject strokeFactory::aActive;
MObject strokeFactory::aOverlap;
MObject strokeFactory::aStrokeRotation;
MObject strokeFactory::aStrokeTranslation;
MObject strokeFactory::aPivotFraction;
MObject strokeFactory::aForceDip;

MObject strokeFactory::aApproachDistanceStart;
MObject strokeFactory::aApproachDistanceMid;
MObject strokeFactory::aApproachDistanceEnd;
MObject strokeFactory::aApproachDistance;




MObject strokeFactory::aBrushRotateTilt;
MObject strokeFactory::aBrushRotateBank;
MObject strokeFactory::aBrushRotateTwist;
MObject strokeFactory::aBrushRotate;
MObject strokeFactory::aBrushFollowStroke;
// MObject strokeFactory::aBrushFrontAxis;
// MObject strokeFactory::aBrushUpAxis;



MObject strokeFactory::aCurves;

//  brushes
MObject strokeFactory::aBrushLiftLength;
MObject strokeFactory::aBrushLiftHeight;
MObject strokeFactory::aBrushLiftBias;
MObject strokeFactory::aBrushLift;


MObject strokeFactory::aBrushMatrix;


MObject strokeFactory::aBrushRetention;
MObject strokeFactory::aBrushWidth;
MObject strokeFactory::aBrushes;

// paint
MObject strokeFactory::aPaintColorR;
MObject strokeFactory::aPaintColorG;
MObject strokeFactory::aPaintColorB;
MObject strokeFactory::aPaintColor;
MObject strokeFactory::aPaintName;
MObject strokeFactory::aPaintOpacity;
MObject strokeFactory::aPaintMaxArcLength;
MObject strokeFactory::aPaints;

MObject strokeFactory::aOutCounts;
MObject strokeFactory::aOutBrushIds;
MObject strokeFactory::aOutPaintIds;
MObject strokeFactory::aOutCurveIds;

MObject strokeFactory::aOutTargets;
MObject strokeFactory::aOutTangents;

MObject strokeFactory::aOutPosition;
MObject strokeFactory::aOutRotation;


MObject strokeFactory::aOutBrushWidths;
MObject strokeFactory::aOutPaintColors;
MObject strokeFactory::aOutPaintOpacities;
MObject strokeFactory::aOutForceDips;


MObject strokeFactory::aOutArcLengths;

MObject strokeFactory::aOutApproachStarts;
MObject strokeFactory::aOutApproachEnds;

MObject strokeFactory::aOutPlaneMatrixWorld;
MObject strokeFactory::aDisplayPoints;
MObject strokeFactory::aDisplayIds;
MObject strokeFactory::aDisplayBrush;

MObject strokeFactory::aDisplayApproach;
MObject strokeFactory::aDisplaySegments;
MObject strokeFactory::aSegmentOutlineThickness;

MObject strokeFactory::aPointSize;
MObject strokeFactory::aNormalLength;
MObject strokeFactory::aWireColor;

MObject strokeFactory::aDisplayBrushLift;
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
  MStatus st;
  MString method("strokeFactory::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
  MFnCompoundAttribute cAttr;
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;
  MFnMessageAttribute msgAttr;

  MMatrix identity;
  identity.setToIdentity();

  aPlaneMatrix = mAttr.create( "planeMatrix", "pmat",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);
  addAttribute(aPlaneMatrix);

  aStrokeRotationTexture = nAttr.create( "strokeRotationTexture", "srtx",
                                         MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aStrokeRotationTexture);

  aStrokeTranslationTexture = nAttr.create( "strokeTranslationTexture", "strx",
                              MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aStrokeTranslationTexture);

  aStrokeTranslationSampleDistance = nAttr.create( "strokeTranslationSampleDistance",
                                     "stsd", MFnNumericData::kDouble);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aStrokeTranslationSampleDistance);

  aStrokeCountFactor = nAttr.create( "strokeCountFactor",
                                     "stcf", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  nAttr.setDefault(1.0);
  addAttribute(aStrokeCountFactor);




  // aStrokeApproachDistance = nAttr.create( "strokeApproachDistance",
  //                                         "sapd", MFnNumericData::kDouble);
  // nAttr.setKeyable(true);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setMin(0.00);
  // nAttr.setSoftMax(20.0);
  // nAttr.setDefault(5.0);
  // addAttribute(aStrokeApproachDistance);


  aClusterApproachObject =  msgAttr.create("clusterApproachObject", "clao");
  addAttribute(aClusterApproachObject);

  aToolChangeApproachObject =  msgAttr.create("toolChangeApproachObject", "tcao");
  addAttribute(aToolChangeApproachObject);

  aHomeApproachObject =  msgAttr.create("homeApproachObject", "hmao");
  addAttribute(aHomeApproachObject);


  aLinearSpeed =  nAttr.create( "linearSpeed",
                                "lnsp", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(100);
  nAttr.setDefault(50);
  nAttr.setKeyable(true);
  addAttribute(aLinearSpeed);


  const double minAngSpeed = mayaMath::single_pi / 900.00;

  aAngularSpeed = uAttr.create( "angularSpeed", "agsp", MFnUnitAttribute::kAngle );
  uAttr.setStorable(true);
  uAttr.setReadable(true);
  uAttr.setMin(minAngSpeed);
  uAttr.setMax((mayaMath::single_pi));
  addAttribute(aAngularSpeed);


  aApproximationDistance =  nAttr.create( "approximationDistance",
                                          "apxd", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(100);
  nAttr.setDefault(10);
  nAttr.setKeyable(true);
  addAttribute(aApproximationDistance);

  aRotateOrder = eAttr.create( "rotateOrder", "ro", MTransformationMatrix::kZYX);
  eAttr.addField("xyz", MTransformationMatrix::kXYZ);
  eAttr.addField("yzx", MTransformationMatrix::kYZX);
  eAttr.addField("zxy", MTransformationMatrix::kZXY);
  eAttr.addField("xzy", MTransformationMatrix::kXZY);
  eAttr.addField("yxz", MTransformationMatrix::kYXZ);
  eAttr.addField("zyx", MTransformationMatrix::kZYX);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute( aRotateOrder ); er;

  aOutputUnit = eAttr.create( "outAngularUnit", "oang", MAngle::kDegrees);
  eAttr.addField("radians", MAngle::kRadians);
  eAttr.addField("degrees", MAngle::kDegrees);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute( aOutputUnit ); er;


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
  nAttr.setStorable(true);

  nAttr.setDefault( 0.1 );


  aRepeats = nAttr.create("repeats", "rpts", MFnNumericData::kShort);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setStorable(true);

  nAttr.setDefault(0);

  aRepeatOffset = nAttr.create("repeatOffset", "rpof", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.0 );
  nAttr.setStorable(true);


  aRepeatMirror = nAttr.create( "repeatMirror", "rpmr", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);


  aRepeatOscillate = nAttr.create( "repeatOscillate", "rpoc", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);

  aRepeatAdvance = nAttr.create("repeatAdvance", "rpav", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.0 );
  nAttr.setStorable(true);



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

  aBrushRotateTilt = uAttr.create( "brushRotateTilt", "brtt", MFnUnitAttribute::kAngle);
  aBrushRotateBank = uAttr.create( "brushRotateBank", "brbk", MFnUnitAttribute::kAngle);
  aBrushRotateTwist = uAttr.create( "brushRotateTwist", "brtw", MFnUnitAttribute::kAngle);
  aBrushRotate = nAttr.create( "brushRotate", "brt", aBrushRotateTilt, aBrushRotateBank,
                               aBrushRotateTwist );
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );

  aBrushFollowStroke = nAttr.create( "followStroke", "fst", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);

  aForceDip = nAttr.create( "forceDip", "fcdp", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);


  aSubcurveMin = nAttr.create( "subcurveMin", "scvn", MFnNumericData::kDouble );
  aSubcurveMax = nAttr.create( "subcurveMax", "scvx", MFnNumericData::kDouble );
  aSubcurve = nAttr.create( "subcurve", "scv", aSubcurveMin, aSubcurveMax);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );






  aApproachDistanceStart = nAttr.create( "approachDistanceStart",
                                         "apds", MFnNumericData::kDouble);

  aApproachDistanceMid = nAttr.create( "approachDistanceMid",
                                       "apdm", MFnNumericData::kDouble);

  aApproachDistanceEnd = nAttr.create( "approachDistanceEnd",
                                       "apde", MFnNumericData::kDouble);

  aApproachDistance = nAttr.create( "approachDistance",
                                    "apd", aApproachDistanceStart, aApproachDistanceMid, aApproachDistanceEnd);


  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);


  aCurves = cAttr.create("curves", "crvs");
  cAttr.addChild(aCurve);
  cAttr.addChild(aSubcurve);
  cAttr.addChild(aPointDensity);
  cAttr.addChild(aStrokeLength);
  cAttr.addChild(aOverlap);
  cAttr.addChild(aRandomLengthFactor);
  cAttr.addChild(aRandomOverlapFactor);
  cAttr.addChild(aForceDip);
  cAttr.addChild(aRepeats);
  cAttr.addChild(aRepeatOffset);
  cAttr.addChild(aRepeatMirror);
  cAttr.addChild(aRepeatOscillate);
  cAttr.addChild(aRepeatAdvance);
  cAttr.addChild(aStrokeRotation);
  cAttr.addChild(aStrokeTranslation);
  cAttr.addChild(aPivotFraction);
  cAttr.addChild(aBrushId);
  cAttr.addChild(aPaintId);
  cAttr.addChild(aBrushRotate);
  cAttr.addChild(aBrushFollowStroke);
  cAttr.addChild(aActive);
  cAttr.addChild(aApproachDistance);




  cAttr.setArray( true );
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute( aCurves ); er;

  // brushes

  aBrushWidth  = nAttr.create("brushWidth", "brwd", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.1 );

  aBrushLiftLength = nAttr.create( "brushLiftLength", "bllg", MFnNumericData::kDouble );
  aBrushLiftHeight = nAttr.create( "brushLiftHeight", "blht", MFnNumericData::kDouble );
  aBrushLiftBias = nAttr.create( "brushLiftBias", "blbi", MFnNumericData::kDouble);
  aBrushLift = nAttr.create( "brushLift", "bl", aBrushLiftLength, aBrushLiftHeight,
                             aBrushLiftBias );
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );

  aBrushMatrix = mAttr.create( "brushMatrix", "bmat",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);

  aBrushRetention  = nAttr.create("brushRetention", "brrt", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 1.0 );


  aBrushes = cAttr.create("brushes", "bsh");
  cAttr.addChild(aBrushWidth);
  cAttr.addChild(aBrushLift);
  cAttr.addChild(aBrushRetention);
  cAttr.addChild(aBrushMatrix);
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

  aPaintMaxArcLength = nAttr.create( "paintMaxArcLength", "pmxl", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aPaintMaxArcLength);

  aPaints = cAttr.create("paints", "pts");
  cAttr.addChild(aPaintColor);
  cAttr.addChild(aPaintName);
  cAttr.addChild(aPaintOpacity);
  cAttr.addChild(aPaintMaxArcLength);
  cAttr.setArray( true );
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute( aPaints ); er;


  aDisplayPoints = nAttr.create( "displayPoints", "dps", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayPoints );

  aDisplayIds = nAttr.create( "displayIds", "dids", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayIds );



  aDisplayBrush = eAttr.create( "displayBrush", "dbr");
  eAttr.addField( "none",    strokeFactory::kNone);
  eAttr.addField( "line",   strokeFactory::kBrushLine );
  eAttr.addField( "matrix",   strokeFactory::kBrushMatrix );
  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setDefault(strokeFactory::kNone);
  addAttribute(aDisplayBrush );


  aDisplaySegments = nAttr.create( "displayPaint", "dpt", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplaySegments );


  aDisplayBrushLift = nAttr.create( "displayBrushLift", "dal", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayBrushLift );

  aDisplayApproach = nAttr.create( "displayApproach", "dapp", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayApproach );

  aDisplaySegmentOutlines = eAttr.create( "displayPaintOutlines", "dpo",
                                          MFnNumericData::kBoolean);

  eAttr.addField( "none",    strokeFactory::kOutlinesNone);
  eAttr.addField( "borders",   strokeFactory::kOutlinesBorders );
  eAttr.addField( "arrows",   strokeFactory::kOutlinesArrows );
  eAttr.addField( "both",   strokeFactory::kOutlinesBoth );

  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setDefault(strokeFactory::kOutlinesBorders);
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

  aOutCurveIds  = tAttr.create("outCurveIds", "ocvids", MFnData::kIntArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutCurveIds ); er;


  aOutCounts  = tAttr.create("outCounts", "octs", MFnData::kIntArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutCounts ); er;


  aOutTargets = tAttr.create("outTargets", "otg", MFnData::kMatrixArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutTargets ); er;

  aOutTangents = tAttr.create("outTangents", "otan", MFnData::kVectorArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutTangents ); er;


  aOutPosition = tAttr.create("outPosition", "opos", MFnData::kVectorArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutPosition ); er;


  aOutRotation = tAttr.create("outRotation", "orot", MFnData::kVectorArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutRotation ); er;

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

  aOutForceDips  = tAttr.create("outForceDips", "ofd", MFnData::kIntArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutForceDips ); er;


  aOutArcLengths = tAttr.create("outArcLengths", "oarc", MFnData::kDoubleArray,
                                &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutArcLengths ); er;

  aOutApproachStarts = tAttr.create("outApproachStarts", "oaps", MFnData::kDoubleArray,
                                    &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutApproachStarts ); er;

  aOutApproachEnds = tAttr.create("outApproachEnds", "oape", MFnData::kDoubleArray,
                                  &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutApproachEnds ); er;



  aOutPlaneMatrixWorld = mAttr.create( "outPlaneMatrixWorld", "opmw",
                                       MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  addAttribute(aOutPlaneMatrixWorld);

  st = attributeAffects(aCurve, aOutTargets);
  st = attributeAffects(aPointDensity, aOutTargets);
  st = attributeAffects(aSubcurve, aOutTargets);
  st = attributeAffects(aSubcurveMin, aOutTargets);
  st = attributeAffects(aSubcurveMax, aOutTargets);
  st = attributeAffects(aStrokeCountFactor, aOutTargets);
  st = attributeAffects(aStrokeLength, aOutTargets);
  st = attributeAffects(aRandomLengthFactor, aOutTargets);
  st = attributeAffects(aCurves, aOutTargets);
  st = attributeAffects(aOverlap, aOutTargets);
  st = attributeAffects(aRandomOverlapFactor, aOutTargets);
  st = attributeAffects(aStrokeRotation, aOutTargets);
  st = attributeAffects(aStrokeTranslation, aOutTargets);
  st = attributeAffects(aActive, aOutTargets);
  st = attributeAffects(aPlaneMatrix, aOutTargets);
  st = attributeAffects(aStrokeRotationTexture, aOutTargets);
  st = attributeAffects(aStrokeTranslationTexture, aOutTargets);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutTargets);
  st = attributeAffects(aPivotFraction, aOutTargets);
  st = attributeAffects(aBrushLift, aOutTargets);
  st = attributeAffects(aBrushRotate, aOutTargets);
  st = attributeAffects(aRepeats, aOutTargets);
  st = attributeAffects(aRepeatOffset, aOutTargets);
  st = attributeAffects(aRepeatAdvance, aOutTargets);
  st = attributeAffects(aRepeatMirror, aOutTargets);
  st = attributeAffects(aRepeatOscillate, aOutTargets);
  // st = attributeAffects(aBrushFrontAxis, aOutTargets);
  // st = attributeAffects(aBrushUpAxis, aOutTargets);
  st = attributeAffects(aBrushFollowStroke, aOutTargets);


  st = attributeAffects(aCurve, aOutTangents);
  st = attributeAffects(aPointDensity, aOutTangents);
  st = attributeAffects(aSubcurve, aOutTangents);
  st = attributeAffects(aSubcurveMin, aOutTangents);
  st = attributeAffects(aSubcurveMax, aOutTangents);
  st = attributeAffects(aStrokeCountFactor, aOutTangents);
  st = attributeAffects(aStrokeLength, aOutTangents);
  st = attributeAffects(aRandomLengthFactor, aOutTangents);
  st = attributeAffects(aCurves, aOutTangents);
  st = attributeAffects(aOverlap, aOutTangents);
  st = attributeAffects(aRandomOverlapFactor, aOutTangents);
  st = attributeAffects(aStrokeRotation, aOutTangents);
  st = attributeAffects(aStrokeTranslation, aOutTangents);
  st = attributeAffects(aActive, aOutTangents);
  st = attributeAffects(aPlaneMatrix, aOutTangents);
  st = attributeAffects(aStrokeRotationTexture, aOutTangents);
  st = attributeAffects(aStrokeTranslationTexture, aOutTangents);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutTangents);
  st = attributeAffects(aPivotFraction, aOutTangents);
  st = attributeAffects(aBrushLift, aOutTangents);
  st = attributeAffects(aRepeats, aOutTangents);
  st = attributeAffects(aRepeatOffset, aOutTangents);
  st = attributeAffects(aRepeatAdvance, aOutTangents);
  st = attributeAffects(aRepeatMirror, aOutTangents);
  st = attributeAffects(aRepeatOscillate, aOutTangents);




  st = attributeAffects(aInMatrix, aOutPosition);
  st = attributeAffects(aCurve, aOutPosition);
  st = attributeAffects(aPointDensity, aOutPosition);
  st = attributeAffects(aSubcurve, aOutPosition);
  st = attributeAffects(aSubcurveMin, aOutPosition);
  st = attributeAffects(aSubcurveMax, aOutPosition);
  st = attributeAffects(aStrokeCountFactor, aOutPosition);
  st = attributeAffects(aStrokeLength, aOutPosition);
  st = attributeAffects(aRandomLengthFactor, aOutPosition);
  st = attributeAffects(aCurves, aOutPosition);
  st = attributeAffects(aOverlap, aOutPosition);
  st = attributeAffects(aRandomOverlapFactor, aOutPosition);
  st = attributeAffects(aStrokeRotation, aOutPosition);
  st = attributeAffects(aStrokeTranslation, aOutPosition);
  st = attributeAffects(aActive, aOutPosition);
  st = attributeAffects(aPlaneMatrix, aOutPosition);
  st = attributeAffects(aStrokeRotationTexture, aOutPosition);
  st = attributeAffects(aStrokeTranslationTexture, aOutPosition);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutPosition);
  st = attributeAffects(aPivotFraction, aOutPosition);
  st = attributeAffects(aBrushLift, aOutPosition);
  st = attributeAffects(aBrushRotate, aOutPosition);
  st = attributeAffects(aRepeats, aOutPosition);
  st = attributeAffects(aRepeatOffset, aOutPosition);
  st = attributeAffects(aRepeatAdvance, aOutPosition);
  st = attributeAffects(aRepeatMirror, aOutPosition);
  st = attributeAffects(aRepeatOscillate, aOutPosition);



  st = attributeAffects(aInMatrix, aOutRotation);
  st = attributeAffects(aCurve, aOutRotation);
  st = attributeAffects(aPointDensity, aOutRotation);
  st = attributeAffects(aSubcurve, aOutRotation);
  st = attributeAffects(aSubcurveMin, aOutRotation);
  st = attributeAffects(aSubcurveMax, aOutRotation);
  st = attributeAffects(aStrokeCountFactor, aOutRotation);
  st = attributeAffects(aStrokeLength, aOutRotation);
  st = attributeAffects(aRandomLengthFactor, aOutRotation);
  st = attributeAffects(aCurves, aOutRotation);
  st = attributeAffects(aOverlap, aOutRotation);
  st = attributeAffects(aRandomOverlapFactor, aOutRotation);
  st = attributeAffects(aStrokeRotation, aOutRotation);
  st = attributeAffects(aStrokeTranslation, aOutRotation);
  st = attributeAffects(aStrokeTranslationTexture, aOutRotation);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutRotation);
  st = attributeAffects(aPivotFraction, aOutRotation);
  st = attributeAffects(aActive, aOutRotation);
  st = attributeAffects(aPlaneMatrix, aOutRotation);
  st = attributeAffects(aStrokeRotationTexture, aOutRotation);
  st = attributeAffects(aBrushLift, aOutRotation);
  st = attributeAffects(aBrushRotate, aOutRotation);
  st = attributeAffects(aRotateOrder, aOutRotation);
  st = attributeAffects(aOutputUnit, aOutRotation);
  st = attributeAffects(aRepeats, aOutRotation);
  st = attributeAffects(aRepeatOffset, aOutRotation);
  st = attributeAffects(aRepeatAdvance, aOutRotation);
  st = attributeAffects(aRepeatMirror, aOutRotation);
  st = attributeAffects(aRepeatOscillate, aOutRotation);
  st = attributeAffects(aBrushFollowStroke, aOutRotation);

  st = attributeAffects(aCurve, aOutCounts);
  st = attributeAffects(aPointDensity, aOutCounts);
  st = attributeAffects(aSubcurve, aOutCounts);
  st = attributeAffects(aSubcurveMin, aOutCounts);
  st = attributeAffects(aSubcurveMax, aOutCounts);
  st = attributeAffects(aStrokeCountFactor, aOutCounts);
  st = attributeAffects(aStrokeLength, aOutCounts);
  st = attributeAffects(aOverlap, aOutCounts);
  st = attributeAffects(aRandomOverlapFactor, aOutCounts);
  st = attributeAffects(aRandomLengthFactor, aOutCounts);
  st = attributeAffects(aCurves, aOutCounts);
  st = attributeAffects(aActive, aOutCounts);
  st = attributeAffects(aRepeats, aOutCounts);
  st = attributeAffects(aRepeatOffset, aOutCounts);
  st = attributeAffects(aRepeatAdvance, aOutCounts);
  st = attributeAffects(aRepeatMirror, aOutCounts);
  st = attributeAffects(aRepeatOscillate, aOutCounts);
  st = attributeAffects(aPlaneMatrix, aOutCounts);

  st = attributeAffects(aCurve, aOutBrushIds);
  st = attributeAffects(aPointDensity, aOutBrushIds);
  st = attributeAffects(aSubcurve, aOutBrushIds);
  st = attributeAffects(aSubcurveMin, aOutBrushIds);
  st = attributeAffects(aSubcurveMax, aOutBrushIds);
  st = attributeAffects(aStrokeCountFactor, aOutBrushIds);
  st = attributeAffects(aStrokeLength, aOutBrushIds);
  st = attributeAffects(aOverlap, aOutBrushIds);
  st = attributeAffects(aRandomOverlapFactor, aOutBrushIds);
  st = attributeAffects(aRandomLengthFactor, aOutBrushIds);
  st = attributeAffects(aCurves, aOutBrushIds);
  st = attributeAffects(aActive, aOutBrushIds);
  st = attributeAffects(aRepeats, aOutBrushIds);
  st = attributeAffects(aRepeatOffset, aOutBrushIds);
  st = attributeAffects(aRepeatAdvance, aOutBrushIds);
  st = attributeAffects(aRepeatMirror, aOutBrushIds);
  st = attributeAffects(aRepeatOscillate, aOutBrushIds);
  st = attributeAffects(aPlaneMatrix, aOutBrushIds);

  st = attributeAffects(aCurve, aOutPaintIds);
  st = attributeAffects(aPointDensity, aOutPaintIds);
  st = attributeAffects(aSubcurve, aOutPaintIds);
  st = attributeAffects(aSubcurveMin, aOutPaintIds);
  st = attributeAffects(aSubcurveMax, aOutPaintIds);
  st = attributeAffects(aStrokeCountFactor, aOutPaintIds);
  st = attributeAffects(aStrokeLength, aOutPaintIds);
  st = attributeAffects(aOverlap, aOutPaintIds);
  st = attributeAffects(aRandomOverlapFactor, aOutPaintIds);
  st = attributeAffects(aRandomLengthFactor, aOutPaintIds);
  st = attributeAffects(aCurves, aOutPaintIds);
  st = attributeAffects(aActive, aOutPaintIds);
  st = attributeAffects(aRepeats, aOutPaintIds);
  st = attributeAffects(aRepeatOffset, aOutPaintIds);
  st = attributeAffects(aRepeatAdvance, aOutPaintIds);
  st = attributeAffects(aRepeatMirror, aOutPaintIds);
  st = attributeAffects(aRepeatOscillate, aOutPaintIds);
  st = attributeAffects(aPlaneMatrix, aOutPaintIds);

  st = attributeAffects(aCurve, aOutCurveIds);
  st = attributeAffects(aPointDensity, aOutCurveIds);
  st = attributeAffects(aSubcurve, aOutCurveIds);
  st = attributeAffects(aSubcurveMin, aOutCurveIds);
  st = attributeAffects(aSubcurveMax, aOutCurveIds);
  st = attributeAffects(aStrokeCountFactor, aOutCurveIds);
  st = attributeAffects(aStrokeLength, aOutCurveIds);
  st = attributeAffects(aOverlap, aOutCurveIds);
  st = attributeAffects(aRandomOverlapFactor, aOutCurveIds);
  st = attributeAffects(aRandomLengthFactor, aOutCurveIds);
  st = attributeAffects(aCurves, aOutCurveIds);
  st = attributeAffects(aActive, aOutCurveIds);
  st = attributeAffects(aRepeats, aOutCurveIds);
  st = attributeAffects(aRepeatOffset, aOutCurveIds);
  st = attributeAffects(aRepeatAdvance, aOutCurveIds);
  st = attributeAffects(aRepeatMirror, aOutCurveIds);
  st = attributeAffects(aRepeatOscillate, aOutCurveIds);
  st = attributeAffects(aPlaneMatrix, aOutCurveIds);

  st = attributeAffects(aCurve, aOutBrushWidths);
  st = attributeAffects(aPointDensity, aOutBrushWidths);
  st = attributeAffects(aSubcurve, aOutBrushWidths);
  st = attributeAffects(aSubcurveMin, aOutBrushWidths);
  st = attributeAffects(aSubcurveMax, aOutBrushWidths);
  st = attributeAffects(aStrokeCountFactor, aOutBrushWidths);
  st = attributeAffects(aStrokeLength, aOutBrushWidths);
  st = attributeAffects(aOverlap, aOutBrushWidths);
  st = attributeAffects(aRandomOverlapFactor, aOutBrushWidths);
  st = attributeAffects(aRandomLengthFactor, aOutBrushWidths);
  st = attributeAffects(aCurves, aOutBrushWidths);
  st = attributeAffects(aBrushWidth, aOutBrushWidths);
  st = attributeAffects(aActive, aOutBrushWidths);
  st = attributeAffects(aRepeats, aOutBrushWidths);
  st = attributeAffects(aRepeatOffset, aOutBrushWidths);
  st = attributeAffects(aRepeatAdvance, aOutBrushWidths);
  st = attributeAffects(aRepeatMirror, aOutBrushWidths);
  st = attributeAffects(aRepeatOscillate, aOutBrushWidths);
  st = attributeAffects(aPlaneMatrix, aOutBrushWidths);

  st = attributeAffects(aCurve, aOutPaintColors);
  st = attributeAffects(aPointDensity, aOutPaintColors);
  st = attributeAffects(aSubcurve, aOutPaintColors);
  st = attributeAffects(aSubcurveMin, aOutPaintColors);
  st = attributeAffects(aSubcurveMax, aOutPaintColors);
  st = attributeAffects(aStrokeCountFactor, aOutPaintColors);
  st = attributeAffects(aStrokeLength, aOutPaintColors);
  st = attributeAffects(aOverlap, aOutPaintColors);
  st = attributeAffects(aRandomOverlapFactor, aOutPaintColors);
  st = attributeAffects(aRandomLengthFactor, aOutPaintColors);
  st = attributeAffects(aCurves, aOutPaintColors);
  st = attributeAffects(aPaintColor, aOutPaintColors);
  st = attributeAffects(aActive, aOutPaintColors);
  st = attributeAffects(aRepeats, aOutPaintColors);
  st = attributeAffects(aRepeatOffset, aOutPaintColors);
  st = attributeAffects(aRepeatAdvance, aOutPaintColors);
  st = attributeAffects(aRepeatMirror, aOutPaintColors);
  st = attributeAffects(aRepeatOscillate, aOutPaintColors);
  st = attributeAffects(aPlaneMatrix, aOutPaintColors);

  st = attributeAffects(aCurve, aOutPaintOpacities);
  st = attributeAffects(aPointDensity, aOutPaintOpacities);
  st = attributeAffects(aSubcurve, aOutPaintOpacities);
  st = attributeAffects(aSubcurveMin, aOutPaintOpacities);
  st = attributeAffects(aSubcurveMax, aOutPaintOpacities);
  st = attributeAffects(aStrokeCountFactor, aOutPaintOpacities);
  st = attributeAffects(aStrokeLength, aOutPaintOpacities);
  st = attributeAffects(aOverlap, aOutPaintOpacities);
  st = attributeAffects(aRandomOverlapFactor, aOutPaintOpacities);
  st = attributeAffects(aRandomLengthFactor, aOutPaintOpacities);
  st = attributeAffects(aCurves, aOutPaintOpacities);
  st = attributeAffects(aPaintOpacity, aOutPaintOpacities);
  st = attributeAffects(aActive, aOutPaintOpacities);
  st = attributeAffects(aRepeats, aOutPaintOpacities);
  st = attributeAffects(aRepeatOffset, aOutPaintOpacities);
  st = attributeAffects(aRepeatAdvance, aOutPaintOpacities);
  st = attributeAffects(aRepeatMirror, aOutPaintOpacities);
  st = attributeAffects(aRepeatOscillate, aOutPaintOpacities);
  st = attributeAffects(aPlaneMatrix, aOutPaintOpacities);

  st = attributeAffects(aInMatrix, aOutArcLengths);
  st = attributeAffects(aCurve, aOutArcLengths);
  st = attributeAffects(aPointDensity, aOutArcLengths);
  st = attributeAffects(aSubcurve, aOutArcLengths);
  st = attributeAffects(aSubcurveMin, aOutArcLengths);
  st = attributeAffects(aSubcurveMax, aOutArcLengths);
  st = attributeAffects(aStrokeLength, aOutArcLengths);
  st = attributeAffects(aRandomLengthFactor, aOutArcLengths);
  st = attributeAffects(aCurves, aOutArcLengths);
  st = attributeAffects(aOverlap, aOutArcLengths);
  st = attributeAffects(aRandomOverlapFactor, aOutArcLengths);
  st = attributeAffects(aActive, aOutArcLengths);
  st = attributeAffects(aPlaneMatrix, aOutArcLengths);
  st = attributeAffects(aPivotFraction, aOutArcLengths);
  st = attributeAffects(aRepeats, aOutArcLengths);
  st = attributeAffects(aRepeatOffset, aOutArcLengths);
  st = attributeAffects(aRepeatAdvance, aOutArcLengths);
  st = attributeAffects(aRepeatMirror, aOutArcLengths);
  st = attributeAffects(aRepeatOscillate, aOutArcLengths);
  st = attributeAffects(aPlaneMatrix, aOutArcLengths);

  st = attributeAffects(aInMatrix, aOutForceDips);
  st = attributeAffects(aCurve, aOutForceDips);
  st = attributeAffects(aPointDensity, aOutForceDips);
  st = attributeAffects(aSubcurve, aOutForceDips);
  st = attributeAffects(aSubcurveMin, aOutForceDips);
  st = attributeAffects(aSubcurveMax, aOutForceDips);
  st = attributeAffects(aStrokeLength, aOutForceDips);
  st = attributeAffects(aRandomLengthFactor, aOutForceDips);
  st = attributeAffects(aCurves, aOutForceDips);
  st = attributeAffects(aOverlap, aOutForceDips);
  st = attributeAffects(aRandomOverlapFactor, aOutForceDips);
  st = attributeAffects(aActive, aOutForceDips);
  st = attributeAffects(aPlaneMatrix, aOutForceDips);
  st = attributeAffects(aPivotFraction, aOutForceDips);
  st = attributeAffects(aRepeats, aOutForceDips);
  st = attributeAffects(aRepeatOffset, aOutForceDips);
  st = attributeAffects(aRepeatAdvance, aOutForceDips);
  st = attributeAffects(aRepeatMirror, aOutForceDips);
  st = attributeAffects(aRepeatOscillate, aOutForceDips);


  st = attributeAffects(aCurve, aOutApproachStarts);
  st = attributeAffects(aSubcurve, aOutApproachStarts);
  st = attributeAffects(aSubcurveMin, aOutApproachStarts);
  st = attributeAffects(aSubcurveMax, aOutApproachStarts);
  st = attributeAffects(aStrokeCountFactor, aOutApproachStarts);
  st = attributeAffects(aStrokeLength, aOutApproachStarts);
  st = attributeAffects(aRandomLengthFactor, aOutApproachStarts);
  st = attributeAffects(aCurves, aOutApproachStarts);
  st = attributeAffects(aOverlap, aOutApproachStarts);
  st = attributeAffects(aRandomOverlapFactor, aOutApproachStarts);
  st = attributeAffects(aStrokeRotation, aOutApproachStarts);
  st = attributeAffects(aStrokeTranslation, aOutApproachStarts);
  st = attributeAffects(aActive, aOutApproachStarts);
  st = attributeAffects(aPlaneMatrix, aOutApproachStarts);
  st = attributeAffects(aStrokeRotationTexture, aOutApproachStarts);
  st = attributeAffects(aStrokeTranslationTexture, aOutApproachStarts);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutApproachStarts);
  st = attributeAffects(aPivotFraction, aOutApproachStarts);
  st = attributeAffects(aRepeats, aOutApproachStarts);
  st = attributeAffects(aRepeatOffset, aOutApproachStarts);
  st = attributeAffects(aRepeatAdvance, aOutApproachStarts);
  st = attributeAffects(aRepeatMirror, aOutApproachStarts);
  st = attributeAffects(aRepeatOscillate, aOutApproachStarts);
  st = attributeAffects(aApproachDistance, aOutApproachStarts);


  st = attributeAffects(aCurve, aOutApproachEnds);
  st = attributeAffects(aSubcurve, aOutApproachEnds);
  st = attributeAffects(aSubcurveMin, aOutApproachEnds);
  st = attributeAffects(aSubcurveMax, aOutApproachEnds);
  st = attributeAffects(aStrokeCountFactor, aOutApproachEnds);
  st = attributeAffects(aStrokeLength, aOutApproachEnds);
  st = attributeAffects(aRandomLengthFactor, aOutApproachEnds);
  st = attributeAffects(aCurves, aOutApproachEnds);
  st = attributeAffects(aOverlap, aOutApproachEnds);
  st = attributeAffects(aRandomOverlapFactor, aOutApproachEnds);
  st = attributeAffects(aStrokeRotation, aOutApproachEnds);
  st = attributeAffects(aStrokeTranslation, aOutApproachEnds);
  st = attributeAffects(aActive, aOutApproachEnds);
  st = attributeAffects(aPlaneMatrix, aOutApproachEnds);
  st = attributeAffects(aStrokeRotationTexture, aOutApproachEnds);
  st = attributeAffects(aStrokeTranslationTexture, aOutApproachEnds);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutApproachEnds);
  st = attributeAffects(aPivotFraction, aOutApproachEnds);
  st = attributeAffects(aRepeats, aOutApproachEnds);
  st = attributeAffects(aRepeatOffset, aOutApproachEnds);
  st = attributeAffects(aRepeatAdvance, aOutApproachEnds);
  st = attributeAffects(aRepeatMirror, aOutApproachEnds);
  st = attributeAffects(aRepeatOscillate, aOutApproachEnds);
  st = attributeAffects(aApproachDistance, aOutApproachEnds);



  st = attributeAffects(aPlaneMatrix, aPlaneMatrix);
  st = attributeAffects(aInMatrix, aOutPlaneMatrixWorld);
  st = attributeAffects(aPlaneMatrix, aOutPlaneMatrixWorld);

  return ( MS::kSuccess );

}


MStatus strokeFactory::setData(MDataBlock &block, MObject &attribute,
                               const MMatrix &data) {

  MDataHandle h = block.outputValue(attribute);
  h.set(data);
  return block.setClean( attribute );
}


MStatus strokeFactory::setData(MDataBlock &block, MObject &attribute,
                               const MVectorArray &data) {
  MDataHandle h = block.outputValue(attribute);
  MFnVectorArrayData fn;
  MObject d = fn.create(data);
  h.set(d);
  return block.setClean( attribute );
}

MStatus strokeFactory::setData(MDataBlock &block, MObject &attribute,
                               const MDoubleArray &data) {
  MDataHandle h = block.outputValue(attribute);
  MFnDoubleArrayData fn;
  MObject d = fn.create(data);
  h.set(d);
  return block.setClean( attribute );
}

MStatus strokeFactory::setData(MDataBlock &block, MObject &attribute,
                               const MMatrixArray &data) {
  MDataHandle h = block.outputValue(attribute);
  MFnMatrixArrayData fn;
  MObject d = fn.create(data);
  h.set(d);
  return block.setClean( attribute );
}

MStatus strokeFactory::setData(MDataBlock &block, MObject &attribute,
                               const MIntArray &data) {
  MDataHandle h = block.outputValue(attribute);
  MFnIntArrayData fn;
  MObject d = fn.create(data);
  h.set(d);
  return block.setClean( attribute );
}



MStatus strokeFactory::getData( MObject &attribute,  MIntArray &array) {
  MStatus st;
  MFnIntArrayData fn;
  MPlug plug(thisMObject(), attribute);
  MObject d;
  st = plug.getValue(d); ert;
  fn.setObject(d);
  array = fn.array(&st); ert;
  return MS::kSuccess;
}


MStatus strokeFactory::getData( MObject &attribute,  MDoubleArray &array) {
  MStatus st;
  MFnDoubleArrayData fn;
  MPlug plug(thisMObject(), attribute);
  MObject d;
  st = plug.getValue(d); ert;
  fn.setObject(d);
  array = fn.array(&st); ert;
  return MS::kSuccess;

}

MStatus strokeFactory::getData( MObject &attribute,  MVectorArray &array) {
  MStatus st;
  MFnVectorArrayData fn;
  MPlug plug(thisMObject(), attribute);
  MObject d;
  st = plug.getValue(d); ert;
  fn.setObject(d);
  array = fn.array(&st); ert;
  return MS::kSuccess;

}



unsigned int strokeFactory::getStrokeBoundaries(
  const MObject &curve,
  double strokeLength,
  double randomLengthFactor,
  double overlap,
  double randomOverlapFactor,
  double subcurveMin,
  double subcurveMax,
  MVectorArray &result
) const  {

  /*
    generate an array of start and end param for strokes
  */

  MStatus st = MS::kSuccess;

  MFnNurbsCurve curveFn(curve, &st);
  if (st.error()) { return 0; }
  double curveLen = curveFn.length(epsilon);

  // subcurve values are used to gate the start and end of the curve
  if ((subcurveMin + subcurveMax) > curveLen) {
    subcurveMin = 0.0;
    subcurveMax = 0.0;
  }


  if (randomLengthFactor  < 0) { randomLengthFactor = 0;  }
  if (randomLengthFactor  > 1) { randomLengthFactor = 1;  }
  if (randomOverlapFactor < 0) { randomOverlapFactor = 0; }
  if (randomOverlapFactor > 1) { randomOverlapFactor = 1; }

  // double startDist = 0;
  double curveMaxDist = curveLen - subcurveMax;

  double startDist = subcurveMin;
  double endDist;

  do {
    double thisLength = strokeLength + (strokeLength * randomLengthFactor * 2 *
                                        (drand48() - 0.5) );
    if (thisLength < 0.1) { thisLength =  0.1; }
    endDist = fmin((startDist + thisLength), curveMaxDist);

    result.append(MVector(startDist, endDist));

    if (endDist >= curveMaxDist) { break; }


    double thisOverlap = overlap + (overlap * randomOverlapFactor * 2 *  (drand48() - 0.5) );

    double newStartDist =   endDist - thisOverlap;
    if (newStartDist < (startDist + 0.1)) { newStartDist = (startDist + 0.1); }
    if (newStartDist >= curveMaxDist) { break; }
    startDist = newStartDist;
  }
  while ( true );

  return result.length();
}

MStatus strokeFactory::getBrushes(MDataBlock &data,
                                  std::map<short, Brush> &brushes ) const {

  /*
  Generate a std::map of Brushes
  */
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
    double retention = hComp.child(strokeFactory::aBrushRetention).asDouble() ;


    MDataHandle hLift = hComp.child(aBrushLift);
    double liftLength = hLift.child( aBrushLiftLength).asDouble();
    double liftHeight = hLift.child( aBrushLiftHeight).asDouble();
    double liftBias = hLift.child( aBrushLiftBias).asDouble();

    mayaMath::axis frontAxis, upAxis;

    brushes[index] = Brush(index, width, retention, liftLength , liftHeight , liftBias);
  }
  return MS::kSuccess;
}

MStatus strokeFactory::getPaints(MDataBlock &data,
                                 std::map<short, Paint> &paints ) const {
  MStatus st;
  MArrayDataHandle hPaints = data.inputArrayValue(aPaints, &st ); ert;
  unsigned nPlugs = hPaints.elementCount();

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
                                  const MVector &planeNormal,
                                  const MMatrix &inversePlaneMatrix,
                                  const   std::map<short, Brush> brushes,
                                  const   std::map<short, Paint> paints,
                                  std::vector<Stroke> &strokes ) const {




  // // JPMDBG;
  // cerr << "planeNormal: " << planeNormal << endl;
  MStatus st;
  MArrayDataHandle hCurves = data.inputArrayValue(aCurves, &st ); ert;
  unsigned nPlugs = hCurves.elementCount();


  double countFactor = data.inputValue(aStrokeCountFactor).asDouble();


  for (unsigned c = 0; c < nPlugs; c++, hCurves.next()) {
    unsigned elId = hCurves.elementIndex();
    srand48(elId);

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
    if (pointDensity < 0.001) { pointDensity = 0.001; }

    double strokeLength = hComp.child( aStrokeLength).asDouble();
    double randomLengthFactor = hComp.child( aRandomLengthFactor).asDouble();
    double randomOverlapFactor = hComp.child( aRandomOverlapFactor).asDouble();
    double overlap  =  hComp.child( aOverlap).asDouble();





    MDataHandle hSubcurve = hComp.child(aSubcurve);

    double subcurveMin = hSubcurve.child( aSubcurveMin).asDouble();
    double subcurveMax = hSubcurve.child( aSubcurveMax).asDouble();

    MDataHandle hRotate = hComp.child(aBrushRotate);

    double tilt = hRotate.child( aBrushRotateTilt).asAngle().value();
    double bank = hRotate.child( aBrushRotateBank).asAngle().value();
    double twist = hRotate.child( aBrushRotateTwist).asAngle().value();
    MVector brushRotate(tilt, bank, twist);

    MDataHandle hApproachDistance = hComp.child(aApproachDistance);
    double apStart = hApproachDistance.child( aApproachDistanceStart).asDouble();
    double apMid = hApproachDistance.child( aApproachDistanceMid).asDouble();
    double apEnd = hApproachDistance.child( aApproachDistanceEnd).asDouble();
    MVector approachDistance(apStart, apMid, apEnd);



    short repeats = hComp.child(aRepeats).asShort();
    double repeatOffset = hComp.child(aRepeatOffset).asDouble();
    double repeatAdvance = hComp.child(aRepeatAdvance).asDouble();

    bool repeatMirror = hComp.child(aRepeatMirror).asShort();
    bool repeatOscillate = hComp.child(aRepeatOscillate).asBool();

    short brushId =  hComp.child( aBrushId).asShort();
    short paintId =  hComp.child( aPaintId).asShort();

    double rotation = hComp.child(aStrokeRotation).asAngle().value();
    double translation = hComp.child(aStrokeTranslation).asDouble();
    double pivotFraction = hComp.child(aPivotFraction).asDouble();

    bool follow =  hComp.child( aBrushFollowStroke).asBool();
    bool forceDip =  hComp.child( aForceDip).asBool();






    MVectorArray boundaries;
    unsigned numStrokePacks =  getStrokeBoundaries(dCurve, strokeLength, randomLengthFactor,
                               overlap, randomOverlapFactor,
                               subcurveMin,
                               subcurveMax,
                               boundaries);

    if (! numStrokePacks) {
      continue;
    }
    // JPMDBG;
    std::map<short, Brush>::const_iterator brushIter = brushes.find(brushId);
    Brush brush;
    // JPMDBG;
    if (brushIter !=  brushes.end()) {
      brush = brushIter->second;
    }
    // JPMDBG;
    std::map<short, Paint>::const_iterator paintIter = paints.find(paintId);
    Paint paint;
    if (paintIter !=  paints.end()) {
      paint = paintIter->second;
    }
    // JPMDBG;


    std::vector<Stroke> curveStrokes;

    bool first = true;
    for (int i = 0; i < numStrokePacks; ++i) {
      if (drand48() < countFactor) {
        bool forceDipFirst = forceDip && first;
        first = false;
        Stroke motherStroke = Stroke(
                                elId,
                                boundaries[i].x,
                                boundaries[i].y,
                                pointDensity,
                                planeNormal,
                                rotation,
                                translation,
                                pivotFraction,
                                brushRotate,
                                follow,
                                forceDipFirst,
                                brush,
                                paint,
                                dCurve);

        if (motherStroke.overlapsPlane(inversePlaneMatrix)) {
          curveStrokes.push_back(motherStroke);
        }

        for (int j = 0; j < repeats; ++j)
        {
          bool reverse = repeatOscillate &&  (j % 2 == 0);
          double offset = repeatOffset * (j + 1);
          Stroke offsetStroke = Stroke(motherStroke, offset, repeatAdvance, reverse, planeNormal);
          if (offsetStroke.overlapsPlane(inversePlaneMatrix)) {
            curveStrokes.push_back(offsetStroke);
          }
          if (repeatMirror) {
            Stroke mirrorOffsetStroke = Stroke(motherStroke, -offset, repeatAdvance, reverse,
                                               planeNormal);
            if (mirrorOffsetStroke.overlapsPlane(inversePlaneMatrix)) {
              curveStrokes.push_back(mirrorOffsetStroke);
            }
          }
        }
      }
    }

    std::vector<Stroke>::iterator  iter = curveStrokes.begin();
    for (; iter != curveStrokes.end(); iter++) {
      double strokeApproachStart = apMid, strokeApproachEnd = apMid;
      if (iter == curveStrokes.begin()) {
        strokeApproachStart = apStart;
      }
      if (std::next(iter) == curveStrokes.end()) {
        strokeApproachEnd = apEnd;
      }
      iter->setApproach(strokeApproachStart, strokeApproachEnd);
    }


    strokes.insert(strokes.end(), curveStrokes.begin(), curveStrokes.end());
  }
  return MS::kSuccess;
}

MStatus strokeFactory::compute( const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  MString method("strokeFactory::compute");
  if (!(
        plug == aOutTargets ||
        plug == aOutTangents ||
        plug == aOutPosition ||
        plug == aOutRotation ||
        plug == aOutCounts ||
        plug == aOutBrushIds ||
        plug == aOutPaintIds ||
        plug == aOutCurveIds ||
        plug == aOutBrushWidths ||
        plug == aOutPaintColors ||
        plug == aOutPaintOpacities ||
        plug == aOutForceDips ||
        plug == aOutApproachStarts ||
        plug == aOutApproachEnds ||
        plug == aOutPlaneMatrixWorld
      )) { return ( MS::kUnknownParameter ); }




  // MArrayDataHandle hCurves = data.inputArrayValue(aCurves, &st );
  MMatrixArray outTargets; // outTargets.clear();
  MVectorArray outTangents; // outTargets.clear();

  MIntArray outCounts; // outCounts.clear();
  MIntArray outBrushIds; // outBrushIds.clear();
  MIntArray outPaintIds; // outPaintIds.clear();
  MIntArray outCurveIds;
  MDoubleArray outBrushWidths; // outBrushWidths.clear();
  MVectorArray outPaintColors; // outPaintColors.clear();
  MDoubleArray outPaintOpacities; // outPaintOpacities.clear();
  MIntArray outForceDips;
  MDoubleArray outArcLengths; // outArcLengths.clear();

  MDoubleArray outApproachStarts;
  MDoubleArray outApproachEnds;

  MMatrix wm = data.inputValue(strokeFactory::aInMatrix).asMatrix();
  // JPMDBG;
  MMatrix planeMatrix = data.inputValue(strokeFactory::aPlaneMatrix).asMatrix();
  MMatrix inversePlaneMatrix = planeMatrix.inverse();
  // MVector normal = data.inputValue(aNormal).asVector();
  MVector planeNormal = (MVector::zAxis * planeMatrix).normal();
  // JPMDBG;

  double sampleDistance = data.inputValue(aStrokeTranslationSampleDistance).asDouble();
  // JPMDBG;

  MMatrix outPlaneMatrixWorld = planeMatrix * wm;

  // JPMDBG;
  std::map<short, Brush> brushes;
  st = getBrushes(data, brushes) ;

  // JPMDBG;
  std::map<short, Paint> paints;
  st = getPaints(data, paints) ;

  // JPMDBG;
  std::vector<Stroke>  strokes;
  st = getStrokes(data, planeNormal, inversePlaneMatrix, brushes, paints, strokes) ;
  // JPMDBG;



  // aStrokeRotationTexture
  // aStrokeTranslationTexture
  // aStrokeTranslationSampleDistance
  // sample rotation texture and translation gradient

  std::vector<Stroke>::const_iterator citer;
  std::vector<Stroke>::iterator iter;
  // // JPMDBG;

  unsigned nStrokes = strokes.size();
  MFloatArray uVals(nStrokes);
  MFloatArray vVals(nStrokes);
  // // JPMDBG;

  citer = strokes.begin();
  for (unsigned i = 0; citer != strokes.end(); citer++, i++) {
    float &u = uVals[i];
    float &v = vVals[i];
    citer->getPivotUVs(inversePlaneMatrix, u, v);
  }
  MFloatArray rotations;
  MFloatVectorArray translations;
  // // JPMDBG;


  st = sampleUVTexture(strokeFactory::aStrokeRotationTexture, uVals, vVals, rotations);
  if (! st.error()) {
    iter = strokes.begin();
    for (unsigned i = 0; iter != strokes.end(); iter++, i++) {
      iter->rotate(rotations[i], planeNormal);
    }
  }
  // // JPMDBG;

  st = sampleUVGradient(strokeFactory::aStrokeTranslationTexture, sampleDistance, uVals,
                        vVals,
                        translations);
  if (! st.error()) {
    iter = strokes.begin();
    for (unsigned i = 0; iter != strokes.end(); iter++, i++) {
      iter->translate(translations[i], planeNormal);
    }
  }


  citer = strokes.begin();
  for (; citer != strokes.end(); citer++) {
    const MMatrixArray &targets = citer->targets();
    const MVectorArray &tangents = citer->tangents();
    const unsigned len = targets.length();

    outCounts.append(len);
    outBrushIds.append(citer->brush().id);
    outPaintIds.append(citer->paint().id);
    outArcLengths.append(citer->arcLength());
    outBrushWidths.append(citer->brush().width);
    const MColor &c = citer->paint().color;
    outPaintColors.append(MVector(c.r, c.g, c.b));
    outPaintOpacities.append(citer->paint().opacity);
    outForceDips.append(citer->forceDip());
    outCurveIds.append(citer->curveId());
    outApproachStarts.append(citer->approachStart());
    outApproachEnds.append(citer->approachEnd());

    for (int i = 0; i < len; ++i)
    {
      outTargets.append(targets[i]);
      outTangents.append(tangents[i]);
      // outNormals.append(nrm[i]);
    }
  }

  // // JPMDBG;

  unsigned len = outTargets.length();
  // cerr << "len: " << len << endl;

  // MMatrixArray outTargetsWorld(len);
  // MVectorArray normalsWorld(len);
  MVectorArray outPosition(len);
  MVectorArray outRotation(len);

  MTransformationMatrix::RotationOrder order =  MTransformationMatrix::RotationOrder(
        data.inputValue(aRotateOrder).asShort());

  MAngle::Unit outUnit = MAngle::Unit(data.inputValue(aOutputUnit).asShort());


  for (int i = 0; i < len; ++i)
  {
    double rotValue[3];
    MTransformationMatrix tMat( outTargets[i] * wm );
    tMat.reorderRotation(order);

    MTransformationMatrix::RotationOrder tmpOrd;
    tMat.getRotation( rotValue, tmpOrd );
    if (outUnit == MAngle::kDegrees) {
      rotValue[0] *= rad_to_deg;
      rotValue[1] *= rad_to_deg;
      rotValue[2] *= rad_to_deg;
    }
    outPosition[i] = tMat.getTranslation( MSpace::kWorld);
    outRotation[i] = MVector(rotValue[0], rotValue[1], rotValue[2]);
  }


  st = setData(data, strokeFactory::aOutTargets, outTargets); er;
  st = setData(data, strokeFactory::aOutTangents, outTangents); er;
  st = setData(data, strokeFactory::aOutPosition, outPosition); er;
  st = setData(data, strokeFactory::aOutRotation, outRotation); er;
  st = setData(data, strokeFactory::aOutCounts, outCounts); er;
  st = setData(data, strokeFactory::aOutBrushIds, outBrushIds); er;
  st = setData(data, strokeFactory::aOutPaintIds, outPaintIds); er;
  st = setData(data, strokeFactory::aOutCurveIds, outCurveIds); er;

  st = setData(data, strokeFactory::aOutBrushWidths, outBrushWidths); er;
  st = setData(data, strokeFactory::aOutPaintColors, outPaintColors); er;
  st = setData(data, strokeFactory::aOutPaintOpacities, outPaintOpacities); er;
  st = setData(data, strokeFactory::aOutForceDips, outForceDips); er;
  st = setData(data, strokeFactory::aOutArcLengths, outArcLengths); er;


  st = setData(data, strokeFactory::aOutApproachStarts, outApproachStarts); er;
  st = setData(data, strokeFactory::aOutApproachEnds, outApproachEnds); er;

  st = setData(data, strokeFactory::aOutPlaneMatrixWorld, outPlaneMatrixWorld); er;


  // MDataHandle hOutMat = data.outputValue(aOutPlaneMatrixWorld);
  // hOutMat.set(outPlaneMatrixWorld);
  // st = data.setClean( aOutPlaneMatrixWorld ); er;



  return MS::kSuccess;

}


MVector binormal(const MMatrix &p1, const MMatrix &p2, const MVector &normal) {
  return (   MVector(p2[3][0] - p1[3][0], p2[3][1] - p1[3][1],
                     p2[3][2] - p1[3][2]) ^ normal).normal();
}



void strokeFactory::draw( M3dView &view,
                          const MDagPath &path,
                          M3dView::DisplayStyle style,
                          M3dView:: DisplayStatus status  )
{

  // cerr << "STROKEFACTORY::DRAW" << endl;
  MStatus st;

  MObject thisObj = thisMObject();

  MFnMatrixArrayData fnXA;
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
  MVector planeNormal = (MVector::zAxis * pmat).normal();

  // get sample positions from output
  MPlug targetsPlug(thisObj, aOutTargets);
  MObject dTargets;
  st = targetsPlug.getValue(dTargets); er;
  fnXA.setObject(dTargets);
  MMatrixArray tmpTargets = fnXA.array(&st); er;
  unsigned pl = tmpTargets.length();

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);
  bool doStack = stackGap > 0;

  MMatrixArray targets;

  MPlug tangentsPlug(thisObj, aOutTangents);
  MObject dTangents;
  st = tangentsPlug.getValue(dTangents); er;
  fnV.setObject(dTangents);
  MVectorArray tangents = fnV.array(&st); er;

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





  if (stackGap > 0) {
    MMatrixArray stackTargets(pl);
    unsigned i = 0;
    for (int j = 0; j < numStrokes; ++j)
    {
      MVector z(planeNormal * stackGap * j);
      unsigned numPoints = counts[j];
      for (int k = 0; k < numPoints ; ++k)
      {
        stackTargets[i] = tmpTargets[i];
        stackTargets[i][3][0] = stackTargets[i][3][0] + z.x;
        stackTargets[i][3][1] = stackTargets[i][3][1] + z.y;
        stackTargets[i][3][2] = stackTargets[i][3][2] + z.z;
        i++;
      }
    }
    targets = stackTargets;
  }
  else {
    targets = tmpTargets;
  }

  bool doDisplayPoints;
  bool doDisplayIds;
  bool doDisplaySegments;
  bool doDisplayBrushLift;
  bool doDisplayApproach;

  MPlug(thisObj, aDisplayPoints).getValue(doDisplayPoints);
  MPlug(thisObj, aDisplayIds).getValue(doDisplayIds);

  MPlug(thisObj, aDisplaySegments).getValue(doDisplaySegments);
  MPlug(thisObj, aDisplayBrushLift).getValue(doDisplayBrushLift);
  MPlug(thisObj, aDisplayApproach).getValue(doDisplayApproach);

  short brushDisp;
  MPlug(thisObj, aDisplayBrush).getValue(brushDisp);

  short displaySegmentOutlines;
  MPlug(thisObj, aDisplaySegmentOutlines).getValue(displaySegmentOutlines);
  OutlineDisplay od = OutlineDisplay(displaySegmentOutlines);

  bool doArrows = od == strokeFactory::kOutlinesArrows
                  || od == strokeFactory::kOutlinesBoth;
  bool doBorders = od == strokeFactory::kOutlinesBorders
                   || od == strokeFactory::kOutlinesBoth;

  MPlug wireColorPlug (thisObj, aWireColor);
  float wireColorRed, wireColorGreen, wireColorBlue;
  wireColorPlug.child(0).getValue(wireColorRed);
  wireColorPlug.child(1).getValue(wireColorGreen);
  wireColorPlug.child(2).getValue(wireColorBlue);

  MColor wireColor( MColor::kRGB, wireColorRed, wireColorGreen, wireColorBlue);

  double thickness;
  MPlug(thisObj, aSegmentOutlineThickness).getValue(thickness);


  // double approachDistanceStart, approachDistanceMid, approachDistanceEnd;
  // MPlug(thisObj, aStrokeApproachDistanceStart).getValue(approachDistanceStart);
  // MPlug(thisObj, aStrokeApproachDistanceMid).getValue(approachDistanceMid);
  // MPlug(thisObj, aStrokeApproachDistanceEnd).getValue(approachDistanceEnd);





  view.beginGL();


  // POINTS
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
      if (! doDisplayBrushLift) {i++;  numPoints -= 2;}
      for (int k = 0; k < numPoints ; ++k)
      {
        glVertex3f(float(targets[i][3][0]), float(targets[i][3][1]), float(targets[i][3][2]));
        i++;
      }
      if (! doDisplayBrushLift) { i++; }
    }

    glEnd();
    glPopAttrib();
  }

  // SEGMENTS
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

      if (! doDisplayBrushLift) {i++;  numPoints -= 2;}

      for (int k = 0; k < numPoints ; ++k)
      {
        MVector side = (tangents[i] ^ planeNormal).normal() * width;


        MFloatVector a(targets[i][3][0] + side.x, targets[i][3][1] + side.y,
                       targets[i][3][2] + side.z);
        MFloatVector b(targets[i][3][0] - side.x, targets[i][3][1] - side.y,
                       targets[i][3][2] - side.z);

        glVertex3f( a.x , a.y , a.z );
        glVertex3f( b.x , b.y, b.z);
        i++;
      }

      if (! doDisplayBrushLift) { i++; }

      glEnd();
    }
    glPopAttrib();
  }




  if (doDisplayApproach) {

    MDoubleArray approachStarts, approachEnds;
    getData( strokeFactory::aOutApproachStarts, approachStarts);
    getData( strokeFactory::aOutApproachEnds, approachEnds);

    // MFloatVector approachVec = MFloatVector(planeNormal * approachDistance);

    // cerr << "approachVec length : " << approachVec.length() << endl;
    glPushAttrib(GL_LINE_BIT);

    unsigned i = 0;


    view.setDrawColor(wireColor );


    glLineWidth(GLfloat(thickness));
    glBegin(GL_LINES);

    for (int j = 0; j < numStrokes; ++j)
    {

      MVector approachVec;
      approachVec = MFloatVector(planeNormal *  approachStarts[j] ); ;
      // MVector approachEnd = MFloatVector(planeNormal * approachEnds[j] ); ;


      unsigned numPoints = counts[j];
      MFloatVector lift, stroke, approach;


      // start
      lift = MFloatVector(targets[i][3][0], targets[i][3][1], targets[i][3][2]);
      stroke = MFloatVector(targets[(i + 1)][3][0], targets[(i + 1)][3][1],
                            targets[(i + 1)][3][2]);
      approach = MFloatVector(lift + approachVec);

      glVertex3f(lift.x , lift.y, lift.z);
      glVertex3f(approach.x , approach.y , approach.z);

      glVertex3f(stroke.x , stroke.y, stroke.z);
      glVertex3f(lift.x , lift.y , lift.z);

      i += numPoints - 1;


      approachVec = MFloatVector(planeNormal *  approachEnds[j] ); ;

      lift = MFloatVector(targets[i][3][0], targets[i][3][1], targets[i][3][2]);
      stroke = MFloatVector(targets[(i - 1)][3][0], targets[(i - 1)][3][1],
                            targets[(i - 1)][3][2]);
      approach = MFloatVector(lift + approachVec);

      glVertex3f(lift.x , lift.y, lift.z);
      glVertex3f(approach.x , approach.y , approach.z);

      glVertex3f(stroke.x , stroke.y, stroke.z);
      glVertex3f(lift.x , lift.y , lift.z);

      i++;

    }
    glEnd();
    glPopAttrib();
  }

  // SEGMENT OUITLINES
  if (doArrows || doBorders)
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

      if (! doDisplayBrushLift) {i++;  numPoints -= 2;}

      for (int k = 0; k < numPoints ; ++k)
      {
        MVector side = (tangents[i] ^ planeNormal).normal() * width;

        MFloatVector p(targets[i][3][0], targets[i][3][1], targets[i][3][2]);
        MFloatVector a(p + side);
        MFloatVector b(p - side);

        if (! doArrows) {
          // draw the ends unless arrows
          if (k == 0 || k == (numPoints - 1)) {
            glVertex3f(a.x , a.y, a.z);
            glVertex3f(b.x , b.y , b.z);
          }
        }
        if (k > 0) {
          if (doBorders) {
            glVertex3f( last_a.x , last_a.y, last_a.z);
            glVertex3f( a.x , a.y , a.z );
            glVertex3f( last_b.x , last_b.y, last_b.z);
            glVertex3f( b.x , b.y , b.z );

          }
          if (doArrows)  {
            glVertex3f( last_a.x , last_a.y, last_a.z);
            glVertex3f( p.x , p.y , p.z );
            glVertex3f( last_b.x , last_b.y, last_b.z);
            glVertex3f( p.x , p.y , p.z );
          }
        }


        last_a = a;
        last_b = b;
        i++;
      }
      if (! doDisplayBrushLift) { i++; }

      glEnd();
    }
    glPopAttrib();
  }




  if ( BrushDisplay(brushDisp) == strokeFactory::kBrushLine)
  {

    double lineLength;
    MPlug(thisObj, aNormalLength).getValue(lineLength);

    view.setDrawColor(MColor(0.0, 0.0, 1.0) );

    glPushAttrib(GL_LINE_BIT);
    glLineWidth(GLfloat(thickness));
    glBegin( GL_LINES );


    unsigned i = 0;
    for (int j = 0; j < numStrokes; ++j)
    {
      unsigned numPoints = counts[j];
      if (! doDisplayBrushLift) {i++;  numPoints -= 2;}
      for (int k = 0; k < numPoints ; ++k)
      {
        MFloatVector start(targets[i][3][0], targets[i][3][1], targets[i][3][2]);
        // MFloatVector end =  MFloatVector::zAxis;
        MFloatVector end( MFloatVector( MVector::zNegAxis * lineLength * targets[i]));
        end += start;

        glVertex3f( start.x , start.y , start.z );
        glVertex3f( end.x , end.y, end.z);
        i++;
      }
      if (! doDisplayBrushLift) { i++; }
    }

    glEnd();
    glPopAttrib();
  }



  if ( BrushDisplay(brushDisp) == strokeFactory::kBrushMatrix)
  {

    double lineLength;
    MPlug(thisObj, aNormalLength).getValue(lineLength);

    // view.setDrawColor(wireColor );

    glPushAttrib(GL_LINE_BIT);
    glLineWidth(GLfloat(thickness));
    glBegin( GL_LINES );


    unsigned i = 0;
    for (int j = 0; j < numStrokes; ++j)
    {
      unsigned numPoints = counts[j];
      if (! doDisplayBrushLift) {i++;  numPoints -= 2;}
      for (int k = 0; k < numPoints ; ++k)
      {
        MFloatVector start(targets[i][3][0], targets[i][3][1], targets[i][3][2]);

        MFloatVector endX( MFloatVector(MVector::xAxis * lineLength  * targets[i]) )   ;
        MFloatVector endY(  MFloatVector(MVector::yAxis * lineLength  * targets[i]) )   ;
        MFloatVector endZ(  MFloatVector(MVector::zAxis * lineLength  * targets[i]) )   ;
        endX += start;
        endY += start;
        endZ += start;


        glColor3f(1.0f , 0.0f, 0.0f );
        glVertex3f( start.x , start.y , start.z );
        glVertex3f( endX.x , endX.y, endX.z);

        glColor3f(0.0f , 1.0f, 0.0f );
        glVertex3f( start.x , start.y , start.z );
        glVertex3f( endY.x , endY.y, endY.z);

        glColor3f(0.0f , 0.0f, 1.0f );
        glVertex3f( start.x , start.y , start.z );
        glVertex3f( endZ.x , endZ.y, endZ.z);
        i++;
      }
      if (! doDisplayBrushLift) { i++; }
    }

    glEnd();
    glPopAttrib();
  }

  // IDs
  if (doDisplayIds)
  {
    glPushAttrib(GL_CURRENT_BIT);

    view.setDrawColor(wireColor );
    unsigned i = 0;
    for (int j = 0; j < numStrokes; ++j)
    {
      int k = i + 1;
      MString txt  = "";
      txt += j;
      MPoint textPos =  MPoint( targets[k][3][0], targets[k][3][1], targets[k][3][2]);
      view.drawText( txt , textPos,  M3dView::kRight);
      i +=  counts[j];
    }
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
  MVector planeNormal = (MVector::zAxis * pmat).normal();

  MFnMatrixArrayData fnXA;
  MFnIntArrayData fnI;
  // get sample positions from output
  MPlug targetsPlug(thisObj, aOutTargets);
  MObject dTargets;
  st = targetsPlug.getValue(dTargets); er;
  fnXA.setObject(dTargets);
  MMatrixArray targets = fnXA.array(&st); er;
  unsigned pl = targets.length();
  if (! pl ) {
    return MBoundingBox();
  }

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);

  MPlug countsPlug(thisObj, aOutCounts);
  MObject dCounts;
  st = countsPlug.getValue(dCounts); er;
  fnI.setObject(dCounts);
  MIntArray counts = fnI.array(&st); er;
  unsigned numStrokes = counts.length();


  MVector zn(planeNormal * stackGap);
  MVector z = MPoint(targets[0][3][0], targets[0][3][1], targets[0][3][2]);
  MBoundingBox bb(z, z);

  unsigned i = 0;
  for (int j = 0; j < numStrokes; ++j)
  {
    MVector zj(zn * j);
    unsigned numPoints = counts[j];
    z = MPoint(targets[i][3][0], targets[i][3][1], targets[i][3][2]) + zj;
    bb.expand(z);
    i += numPoints - 1;
    z = MPoint(targets[i][3][0], targets[i][3][1], targets[i][3][2]) + zj;
    bb.expand(z);
    i++;
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



