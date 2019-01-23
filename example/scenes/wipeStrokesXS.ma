//Maya ASCII 2017ff04 scene
//Name: wipeStrokesXS.ma
//Last modified: Sun, Jan 13, 2019 09:32:36 PM
//Codeset: UTF-8
requires maya "2017ff04";
requires -nodeType "curveStroke" -dataType "brushData" -dataType "strokeData" -dataType "paintingData"
		 "Uprising" "2017.x.999";
requires "stereoCamera" "10.0";
requires -dataType "cImgData" -dataType "cImgFloatData" "Kit" "2017.dev.999";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2017";
fileInfo "version" "2017";
fileInfo "cutIdentifier" "201702071345-1015190";
fileInfo "osv" "Mac OS X 10.12.6";
createNode curveStroke -n "defaultSource_backWipe_backWipeShape_SC287";
	rename -uid "F9681936-DF40-B1DE-7415-209E0F34F302";
	setAttr ".pd" 2;
	setAttr ".fst" no;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
	setAttr ".btwr" -type "double2" 90 90 ;
	setAttr ".brrp[0]"  0 0 1;
	setAttr ".prrp[0]"  0 0 1;
	setAttr ".stl" 10000;
createNode curveStroke -n "defaultSource_longWipe2_longWipeShape2_SC287";
	rename -uid "BE9FF42E-C446-772A-62FC-8BAA1FB7FDA0";
	setAttr ".pd" 2;
	setAttr ".fst" no;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
	setAttr ".btwr" -type "double2" 180 180 ;
	setAttr ".brrp[0]"  0 0 1;
	setAttr ".prrp[0]"  0 0 1;
	setAttr ".stl" 10000;
createNode curveStroke -n "defaultSource_longWipe1_longWipe1Shape_SC287";
	rename -uid "DC9EDA3B-BD41-B3C0-5A85-54A6973D43EB";
	setAttr ".pd" 2;
	setAttr ".fst" no;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
	setAttr ".btwr" -type "double2" 0 0 ;
	setAttr ".brrp[0]"  0 0 1;
	setAttr ".prrp[0]"  0 0 1;
	setAttr ".stl" 10000;
select -ne :time1;
	setAttr ".o" 0;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -s 27 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 31 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 40 ".u";
select -ne :defaultRenderingList1;
select -ne :defaultTextureList1;
	setAttr -s 37 ".tx";
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :defaultColorMgtGlobals;
	setAttr ".vtn" -type "string" "Raw";
	setAttr ".otn" -type "string" "Raw";
	setAttr ".potn" -type "string" "Raw";
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :ikSystem;
	setAttr -s 2 ".sol";
// End of wipeStrokesXS.ma
