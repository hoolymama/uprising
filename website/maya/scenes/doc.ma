//Maya ASCII 2022 scene
//Name: doc.ma
//Last modified: Mon, Feb 20, 2023 02:43:18 AM
//Codeset: UTF-8
requires maya "2022";
requires -nodeType "cImgDilate" -nodeType "curveStroke" -dataType "cImgData" -dataType "cImgFloatData"
		 -dataType "pearlChainData" -dataType "particleTrailsData" -dataType "skChainData"
		 -dataType "brushData" -dataType "brushShopData" -dataType "paletteData" -dataType "strokeData"
		 -dataType "paintingData" -dataType "lightPaintingData" "Uprising" "999.0.0";
requires "stereoCamera" "10.0";
requires "mtoa" "5.1.1";
requires "stereoCamera" "10.0";
requires "mtoa" "5.1.1";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2022";
fileInfo "version" "2022";
fileInfo "cutIdentifier" "202110272215-ad32f8f1e6";
fileInfo "osv" "Mac OS X 10.16";
fileInfo "UUID" "13911401-1B4D-EA39-1E97-D5BA7D31EDBB";
createNode transform -s -n "persp";
	rename -uid "8B163665-2642-1F91-33B6-1EAE1ACEC02C";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 288 -288 216 ;
	setAttr ".r" -type "double3" 62.06164727039765 -1.5902773407317581e-14 45.000000000000007 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "FB8AAF95-4640-EDFB-E65F-CDAD5134B8CB";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 461.02494509516509;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	rename -uid "4089421F-9644-379F-D0B7-56B38BD723CB";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "97449A94-E44C-8595-52FF-3799AAE159C7";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "front";
	rename -uid "84FD8683-B644-2BB7-257C-A3AD74B1DE5C";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 -1000.1 0 ;
	setAttr ".r" -type "double3" 89.999999999999986 0 0 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "E0356D7C-F74E-DD02-896B-C4912AD08AD2";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "side";
	rename -uid "2E757088-BD49-EAE6-55BA-D69FB7408ECC";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 90 1.2722218725854067e-14 89.999999999999986 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "4BF4A9E6-7E45-65BF-6604-8381E4DFFF8C";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -n "painting_query";
	rename -uid "1D329369-3E4C-DB91-490C-3D8459920774";
	addAttr -ci true -sn "docCommandName" -ln "docCommandName" -dt "string";
	addAttr -ci true -sn "docMeta_Author" -ln "docMeta_Author" -dt "string";
	addAttr -ci true -sn "docMeta_Email" -ln "docMeta_Email" -dt "string";
	addAttr -ci true -sn "docMeta_Description" -ln "docMeta_Description" -dt "string";
	addAttr -ci true -sn "docBlock_Synopsis" -ln "docBlock_Synopsis" -dt "string";
	addAttr -ci true -sn "docBlock_ReturnValue" -ln "docBlock_ReturnValue" -dt "string";
	addAttr -ci true -sn "docBlock_MelExamples" -ln "docBlock_MelExamples" -dt "string";
	addAttr -ci true -sn "docFlag_ClusterBrushId" -ln "docFlag_ClusterBrushId" -dt "string";
	addAttr -ci true -sn "docFlagContext_ClusterBrushId" -ln "docFlagContext_ClusterBrushId" 
		-dt "string";
	addAttr -ci true -sn "docFlag_ClusterCount" -ln "docFlag_ClusterCount" -dt "string";
	addAttr -ci true -sn "docFlagContext_ClusterCount" -ln "docFlagContext_ClusterCount" 
		-dt "string";
	addAttr -ci true -sn "docFlag_ClusterIndex" -ln "docFlag_ClusterIndex" -dt "string";
	addAttr -ci true -sn "docFlagContext_ClusterIndex" -ln "docFlagContext_ClusterIndex" 
		-dt "string";
	addAttr -ci true -sn "docFlag_ClusterPaintId" -ln "docFlag_ClusterPaintId" -dt "string";
	addAttr -ci true -sn "docFlagContext_ClusterPaintId" -ln "docFlagContext_ClusterPaintId" 
		-dt "string";
	addAttr -ci true -sn "docFlag_ClusterReason" -ln "docFlag_ClusterReason" -dt "string";
	addAttr -ci true -sn "docFlagContext_ClusterReason" -ln "docFlagContext_ClusterReason" 
		-dt "string";
	addAttr -ci true -sn "docFlag_ClusterTravel" -ln "docFlag_ClusterTravel" -dt "string";
	addAttr -ci true -sn "docFlagContext_ClusterTravel" -ln "docFlagContext_ClusterTravel" 
		-dt "string";
	addAttr -ci true -sn "docFlag_ClusterTravelCutoff" -ln "docFlag_ClusterTravelCutoff" 
		-dt "string";
	addAttr -ci true -sn "docFlagContext_ClusterTravelCutoff" -ln "docFlagContext_ClusterTravelCutoff" 
		-dt "string";
	addAttr -ci true -sn "docFlag_ClusterPotId" -ln "docFlag_ClusterPotId" -dt "string";
	addAttr -ci true -sn "docFlagContext_ClusterPotId" -ln "docFlagContext_ClusterPotId" 
		-dt "string";
	addAttr -ci true -sn "docFlag_GlobalStrokeId" -ln "docFlag_GlobalStrokeId" -dt "string";
	addAttr -ci true -sn "docFlagContext_GlobalStrokeId" -ln "docFlagContext_GlobalStrokeId" 
		-dt "string";
	addAttr -ci true -sn "docFlag_Json" -ln "docFlag_Json" -dt "string";
	addAttr -ci true -sn "docFlagContext_Json" -ln "docFlagContext_Json" -dt "string";
	addAttr -ci true -sn "docFlag_StrokeLayerId" -ln "docFlag_StrokeLayerId" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeLayerId" -ln "docFlagContext_StrokeLayerId" 
		-dt "string";
	addAttr -ci true -sn "docFlag_RotateOrder" -ln "docFlag_RotateOrder" -dt "string";
	addAttr -ci true -sn "docFlagContext_RotateOrder" -ln "docFlagContext_RotateOrder" 
		-dt "string";
	addAttr -ci true -sn "docFlag_RotateUnit" -ln "docFlag_RotateUnit" -dt "string";
	addAttr -ci true -sn "docFlagContext_RotateUnit" -ln "docFlagContext_RotateUnit" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeApoproximationDistance" -ln "docFlag_StrokeApoproximationDistance" 
		-dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeApoproximationDistance" -ln "docFlagContext_StrokeApoproximationDistance" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeArcLength" -ln "docFlag_StrokeArcLength" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeArcLength" -ln "docFlagContext_StrokeArcLength" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeArrivalPositions" -ln "docFlag_StrokeArrivalPositions" 
		-dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeArrivalPositions" -ln "docFlagContext_StrokeArrivalPositions" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeArrivalRotations" -ln "docFlag_StrokeArrivalRotations" 
		-dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeArrivalRotations" -ln "docFlagContext_StrokeArrivalRotations" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeCount" -ln "docFlag_StrokeCount" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeCount" -ln "docFlagContext_StrokeCount" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeColors" -ln "docFlag_StrokeColors" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeColors" -ln "docFlagContext_StrokeColors" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeCreator" -ln "docFlag_StrokeCreator" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeCreator" -ln "docFlagContext_StrokeCreator" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeDeparturePosition" -ln "docFlag_StrokeDeparturePosition" 
		-dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeDeparturePosition" -ln "docFlagContext_StrokeDeparturePosition" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeDepartureRotation" -ln "docFlag_StrokeDepartureRotation" 
		-dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeDepartureRotation" -ln "docFlagContext_StrokeDepartureRotation" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeIndex" -ln "docFlag_StrokeIndex" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeIndex" -ln "docFlagContext_StrokeIndex" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokePositions" -ln "docFlag_StrokePositions" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokePositions" -ln "docFlagContext_StrokePositions" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeParentIndex" -ln "docFlag_StrokeParentIndex" 
		-dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeParentIndex" -ln "docFlagContext_StrokeParentIndex" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeRotations" -ln "docFlag_StrokeRotations" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeRotations" -ln "docFlagContext_StrokeRotations" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeSpeedAngular" -ln "docFlag_StrokeSpeedAngular" 
		-dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeSpeedAngular" -ln "docFlagContext_StrokeSpeedAngular" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeSpeedLinear" -ln "docFlag_StrokeSpeedLinear" 
		-dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeSpeedLinear" -ln "docFlagContext_StrokeSpeedLinear" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeXaxis" -ln "docFlag_StrokeXaxis" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeXaxis" -ln "docFlagContext_StrokeXaxis" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeYaxis" -ln "docFlag_StrokeYaxis" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeYaxis" -ln "docFlagContext_StrokeYaxis" 
		-dt "string";
	addAttr -ci true -sn "docFlag_StrokeZaxis" -ln "docFlag_StrokeZaxis" -dt "string";
	addAttr -ci true -sn "docFlagContext_StrokeZaxis" -ln "docFlagContext_StrokeZaxis" 
		-dt "string";
	addAttr -ci true -sn "docFlag_ToolCombinations" -ln "docFlag_ToolCombinations" -dt "string";
	addAttr -ci true -sn "docFlagContext_ToolCombinations" -ln "docFlagContext_ToolCombinations" 
		-dt "string";
	setAttr ".docCommandName" -type "string" "paintingQuery";
	setAttr ".docMeta_Author" -type "string" "Julian Mann";
	setAttr ".docMeta_Email" -type "string" "julian.mann@gmail.com";
	setAttr ".docMeta_Description" -type "string" "paintingQuery brief description";
	setAttr ".docBlock_Synopsis" -type "string" "How to use paintingQuery";
	setAttr ".docBlock_ReturnValue" -type "string" "Describe paintingQuery return value";
	setAttr ".docBlock_MelExamples" -type "string" "```// Enter Valid MEL here```";
	setAttr ".docFlag_ClusterBrushId" -type "string" "Return the brushId for the cluster with the given given index. You must provide the clusterIndex.";
	setAttr ".docFlagContext_ClusterBrushId" -type "string" "Q";
	setAttr ".docFlag_ClusterCount" -type "string" "Return the number of clusters in the painting.";
	setAttr ".docFlagContext_ClusterCount" -type "string" "Q";
	setAttr ".docFlag_ClusterIndex" -type "string" "Specifies the cluster index for several other queries.";
	setAttr ".docFlagContext_ClusterIndex" -type "string" "C";
	setAttr ".docFlag_ClusterPaintId" -type "string" "Return the paintId for the cluster with the given given index. You must provide the clusterIndex.";
	setAttr ".docFlagContext_ClusterPaintId" -type "string" "Q";
	setAttr ".docFlag_ClusterReason" -type "string" "Return the reason for the change to the cluster with the given index. You must provide the clusterIndex. Possible results are:\n<ul>\n<li>tool: The brush was changed.</li> \n<li>dip: The brush was dipped in paint.</li> \n<li>tcp: The brush changed its tool control point parameter.</li>\n</ul>";
	setAttr ".docFlagContext_ClusterReason" -type "string" "Q";
	setAttr ".docFlag_ClusterTravel" -type "string" "The distance that the cluster with the given index travels. You must provide the clusterIndex.";
	setAttr ".docFlagContext_ClusterTravel" -type "string" "Q";
	setAttr ".docFlag_ClusterTravelCutoff" -type "string" "The distance threshold at which the system stops adding strokes to a cluster. The last stroke of the cluster usually extends further than the cutoff. You must provide the clusterIndex.";
	setAttr ".docFlagContext_ClusterTravelCutoff" -type "string" "Q";
	setAttr ".docFlag_ClusterPotId" -type "string" "The pot in which the cluster with the given index dips. You must provide the clusterIndex.";
	setAttr ".docFlagContext_ClusterPotId" -type "string" "Q";
	setAttr ".docFlag_GlobalStrokeId" -type "string" "Given a cluster index and a stroke index within the cluster, return the stroke index with respect to the entire painting. You must provide the clusterIndex and the strokeIndex.";
	setAttr ".docFlagContext_GlobalStrokeId" -type "string" "Q";
	setAttr ".docFlag_Json" -type "string" "Return a JSON array containing object with the following fields for each stroke:\nbrushId, paintId, layerId, parentId";
	setAttr ".docFlagContext_Json" -type "string" "Q";
	setAttr ".docFlag_StrokeLayerId" -type "string" "Return the layerId for the given stroke. You must provide the clusterIndex and the strokeIndex.";
	setAttr ".docFlagContext_StrokeLayerId" -type "string" "Q";
	setAttr ".docFlag_RotateOrder" -type "string" "Some commands require the rotateOrder to be provided. For example, the getRotations option needs to know how to deconstruct target matrices.";
	setAttr ".docFlagContext_RotateOrder" -type "string" "C";
	setAttr ".docFlag_RotateUnit" -type "string" "Describe -rotateUnit";
	setAttr ".docFlagContext_RotateUnit" -type "string" "C";
	setAttr ".docFlag_StrokeApoproximationDistance" -type "string" "Describe -strokeApoproximationDistance";
	setAttr ".docFlagContext_StrokeApoproximationDistance" -type "string" "CEQM";
	setAttr ".docFlag_StrokeArcLength" -type "string" "Describe -strokeArcLength";
	setAttr ".docFlagContext_StrokeArcLength" -type "string" "CEQM";
	setAttr ".docFlag_StrokeArrivalPositions" -type "string" "Describe -strokeArrivalPositions";
	setAttr ".docFlagContext_StrokeArrivalPositions" -type "string" "CEQM";
	setAttr ".docFlag_StrokeArrivalRotations" -type "string" "Describe -strokeArrivalRotations";
	setAttr ".docFlagContext_StrokeArrivalRotations" -type "string" "CEQM";
	setAttr ".docFlag_StrokeCount" -type "string" "Describe -strokeCount";
	setAttr ".docFlagContext_StrokeCount" -type "string" "CEQM";
	setAttr ".docFlag_StrokeColors" -type "string" "Describe -strokeColors";
	setAttr ".docFlagContext_StrokeColors" -type "string" "CEQM";
	setAttr ".docFlag_StrokeCreator" -type "string" "Describe -strokeCreator";
	setAttr ".docFlagContext_StrokeCreator" -type "string" "CEQM";
	setAttr ".docFlag_StrokeDeparturePosition" -type "string" "Describe -strokeDeparturePosition";
	setAttr ".docFlagContext_StrokeDeparturePosition" -type "string" "CEQM";
	setAttr ".docFlag_StrokeDepartureRotation" -type "string" "Describe -strokeDepartureRotation";
	setAttr ".docFlagContext_StrokeDepartureRotation" -type "string" "CEQM";
	setAttr ".docFlag_StrokeIndex" -type "string" "Describe -strokeIndex";
	setAttr ".docFlagContext_StrokeIndex" -type "string" "CEQM";
	setAttr ".docFlag_StrokePositions" -type "string" "Describe -strokePositions";
	setAttr ".docFlagContext_StrokePositions" -type "string" "C";
	setAttr ".docFlag_StrokeParentIndex" -type "string" "Describe -strokeParentIndex";
	setAttr ".docFlagContext_StrokeParentIndex" -type "string" "CEQM";
	setAttr ".docFlag_StrokeRotations" -type "string" "Describe -strokeRotations";
	setAttr ".docFlagContext_StrokeRotations" -type "string" "CEQM";
	setAttr ".docFlag_StrokeSpeedAngular" -type "string" "Describe -strokeSpeedAngular";
	setAttr ".docFlagContext_StrokeSpeedAngular" -type "string" "CEQM";
	setAttr ".docFlag_StrokeSpeedLinear" -type "string" "Describe -strokeSpeedLinear";
	setAttr ".docFlagContext_StrokeSpeedLinear" -type "string" "CEQM";
	setAttr ".docFlag_StrokeXaxis" -type "string" "Describe -strokeXAxis";
	setAttr ".docFlagContext_StrokeXaxis" -type "string" "CEQM";
	setAttr ".docFlag_StrokeYaxis" -type "string" "Describe -strokeYAxis";
	setAttr ".docFlagContext_StrokeYaxis" -type "string" "CEQM";
	setAttr ".docFlag_StrokeZaxis" -type "string" "Describe -strokeZAxis";
	setAttr ".docFlagContext_StrokeZaxis" -type "string" "CEQM";
	setAttr ".docFlag_ToolCombinations" -type "string" "Describe -toolCombinations";
	setAttr ".docFlagContext_ToolCombinations" -type "string" "CEQM";
createNode lightLinker -s -n "lightLinker1";
	rename -uid "74284C3D-2B4D-F2D5-2ED3-5D80887F7F5F";
	setAttr -s 2 ".lnk";
	setAttr -s 2 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "26EE6AAD-9446-8B70-CF5E-4D865C4D26DD";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "137A05B2-BD44-614F-702B-84A92769A6D9";
createNode displayLayerManager -n "layerManager";
	rename -uid "28CFA161-C24A-B276-C680-C3BD8D744ECD";
createNode displayLayer -n "defaultLayer";
	rename -uid "A1386F4A-CF40-1398-79A3-D0BB620B7AB3";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "06837A13-8344-F561-561E-8CB96F2628E2";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "2331C3F8-374C-05BC-DD29-F3B2FE5C86FC";
	setAttr ".g" yes;
createNode script -n "uiConfigurationScriptNode";
	rename -uid "70836F73-5743-B3A2-15F4-C7BF333B15BD";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $nodeEditorPanelVisible = stringArrayContains(\"nodeEditorPanel1\", `getPanel -vis`);\n\tint    $nodeEditorWorkspaceControlOpen = (`workspaceControl -exists nodeEditorPanel1Window` && `workspaceControl -q -visible nodeEditorPanel1Window`);\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\n\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n"
		+ "            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n"
		+ "            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n"
		+ "            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 0\n            -height 1014\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n"
		+ "            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n"
		+ "            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n"
		+ "            -width 794\n            -height 1014\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n"
		+ "            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n"
		+ "            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n"
		+ "            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 794\n            -height 1014\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n"
		+ "            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n"
		+ "            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -selectCommand \"print(\\\"\\\")\" \n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n"
		+ "            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n"
		+ "                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n"
		+ "                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayValues 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showPlayRangeShades \"on\" \n                -lockPlayRangeShades \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -keyMinScale 1\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -valueLinesToggle 0\n                -highlightAffectedCurves 0\n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n"
		+ "                -showPublishedAsConnected 0\n                -showParentContainers 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n"
		+ "                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayValues 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayValues 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayValues 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n"
		+ "                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif ($nodeEditorPanelVisible || $nodeEditorWorkspaceControlOpen) {\n\t\tif (\"\" == $panelName) {\n\t\t\tif ($useSceneConfig) {\n\t\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n"
		+ "                -allNodes 0\n                -autoSizeNodes 0\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n                -copyConnectionsOnPaste 0\n                -connectionStyle \"bezier\" \n                -connectionMinSegment 0.03\n                -connectionOffset 0.03\n                -connectionRoundness 0.8\n                -connectionTension -100\n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n"
		+ "                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -editorMode \"default\" \n                -hasWatchpoint 0\n                $editorName;\n\t\t\t}\n\t\t} else {\n\t\t\t$label = `panel -q -label $panelName`;\n\t\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 0\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n                -copyConnectionsOnPaste 0\n                -connectionStyle \"bezier\" \n                -connectionMinSegment 0.03\n                -connectionOffset 0.03\n                -connectionRoundness 0.8\n                -connectionTension -100\n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n"
		+ "                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -editorMode \"default\" \n                -hasWatchpoint 0\n                $editorName;\n\t\t\tif (!$useSceneConfig) {\n\t\t\t\tpanel -e -l $label $panelName;\n\t\t\t}\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\n{ string $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"|persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n"
		+ "                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n"
		+ "                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -controllers 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n"
		+ "                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName; };\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"vertical2\\\" -ps 1 50 100 -ps 2 50 100 $gMainPane;\"\n"
		+ "\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Front View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 794\\n    -height 1014\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 794\\n    -height 1014\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 794\\n    -height 1014\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 794\\n    -height 1014\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 10 -size 120 -divisions 10 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels yes -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition axis;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "75A22763-BA4D-56A0-86CD-7B820B276D63";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode curveStroke -n "curveStroke1";
	rename -uid "684EF566-D340-D7FF-C0AF-01A2868523BE";
	addAttr -ci true -sn "docMeta_Author" -ln "docMeta_Author" -dt "string";
	addAttr -ci true -sn "docMeta_Email" -ln "docMeta_Email" -dt "string";
	addAttr -ci true -sn "docMeta_Description" -ln "docMeta_Description" -dt "string";
	addAttr -ci true -sn "docBlock_Synopsis" -ln "docBlock_Synopsis" -dt "string";
	addAttr -ci true -sn "docAttr_StrokeSortList" -ln "docAttr_StrokeSortList" -dt "string";
	addAttr -ci true -sn "docAttr_StrokeSortKey" -ln "docAttr_StrokeSortKey" -dt "string";
	addAttr -ci true -sn "docAttr_StrokeSortDirection" -ln "docAttr_StrokeSortDirection" 
		-dt "string";
	addAttr -ci true -sn "docAttr_StrokeSortTexture" -ln "docAttr_StrokeSortTexture" 
		-dt "string";
	addAttr -ci true -sn "docAttr_Output" -ln "docAttr_Output" -dt "string";
	addAttr -ci true -sn "docAttr_ApplySort" -ln "docAttr_ApplySort" -dt "string";
	addAttr -ci true -sn "docAttr_StrokeFilterList" -ln "docAttr_StrokeFilterList" -dt "string";
	addAttr -ci true -sn "docAttr_StrokeFilterKey" -ln "docAttr_StrokeFilterKey" -dt "string";
	addAttr -ci true -sn "docAttr_StrokeFilterOperator" -ln "docAttr_StrokeFilterOperator" 
		-dt "string";
	addAttr -ci true -sn "docAttr_StrokeFilterOperand" -ln "docAttr_StrokeFilterOperand" 
		-dt "string";
	addAttr -ci true -sn "docAttr_StrokeFilterTexture" -ln "docAttr_StrokeFilterTexture" 
		-dt "string";
	addAttr -ci true -sn "docAttr_ApplyFilters" -ln "docAttr_ApplyFilters" -dt "string";
	addAttr -ci true -sn "docAttr_StartFrom" -ln "docAttr_StartFrom" -dt "string";
	addAttr -ci true -sn "docAttr_EndAt" -ln "docAttr_EndAt" -dt "string";
	addAttr -ci true -sn "docAttr_LinearSpeed" -ln "docAttr_LinearSpeed" -dt "string";
	addAttr -ci true -sn "docAttr_AngularSpeed" -ln "docAttr_AngularSpeed" -dt "string";
	addAttr -ci true -sn "docAttr_ApproximationDistance" -ln "docAttr_ApproximationDistance" 
		-dt "string";
	addAttr -ci true -sn "docAttr_Coats" -ln "docAttr_Coats" -dt "string";
	addAttr -ci true -sn "docAttr_LayerId" -ln "docAttr_LayerId" -dt "string";
	addAttr -ci true -sn "docAttr_PointDensity" -ln "docAttr_PointDensity" -dt "string";
	addAttr -ci true -sn "docAttr_BrushTwist" -ln "docAttr_BrushTwist" -dt "string";
	addAttr -ci true -sn "docAttr_BrushTwistStart" -ln "docAttr_BrushTwistStart" -dt "string";
	addAttr -ci true -sn "docAttr_BrushTwistEnd" -ln "docAttr_BrushTwistEnd" -dt "string";
	addAttr -ci true -sn "docAttr_BrushTilt" -ln "docAttr_BrushTilt" -dt "string";
	addAttr -ci true -sn "docAttr_BrushTiltStart" -ln "docAttr_BrushTiltStart" -dt "string";
	addAttr -ci true -sn "docAttr_BrushTiltEnd" -ln "docAttr_BrushTiltEnd" -dt "string";
	addAttr -ci true -sn "docAttr_BrushBank" -ln "docAttr_BrushBank" -dt "string";
	addAttr -ci true -sn "docAttr_BrushBankStart" -ln "docAttr_BrushBankStart" -dt "string";
	addAttr -ci true -sn "docAttr_BrushBankEnd" -ln "docAttr_BrushBankEnd" -dt "string";
	addAttr -ci true -sn "docAttr_BrushRotateOrder" -ln "docAttr_BrushRotateOrder" -dt "string";
	addAttr -ci true -sn "docAttr_CanvasMatrix" -ln "docAttr_CanvasMatrix" -dt "string";
	addAttr -ci true -sn "docAttr_StrokeLength" -ln "docAttr_StrokeLength" -dt "string";
	setAttr -s 2 ".stsl[0:1]"  0 0 1 0;
	setAttr ".docMeta_Author" -type "string" "Julian Mann";
	setAttr ".docMeta_Email" -type "string" "julian.mann@gmail.com";
	setAttr ".docMeta_Description" -type "string" "curveStroke brief description";
	setAttr ".docBlock_Synopsis" -type "string" "The curveStroke node allows you to create a set of brushstrokes given one or more nurbs curves as input. The brush strokes are placed along the curves and you have control over several characteristics of the strokes.\n\nLike all stroke generators and modifier nodes, strokes can be sorted and filtered based on stroke properties such as brushId or paintId, or on the value of a projected texture.\n\nSince a stroke is a collection of targets for the robot to reach, the node provides controls to manage the orientation of targets along the stroke.\n\n";
	setAttr ".docAttr_StrokeSortList" -type "string" "An array of sorting definitions that can be used to manage the order of output strokes. Each definition consists of a key, such as brushId, and a direction, either ascending or descending.\nThe order of elements matters. Definitions first in the array take prescedence over later definitions.";
	setAttr ".docAttr_StrokeSortKey" -type "string" "Stroke properties that may be used t configure the sorting operation.\nEnumerated strings are: \"Stroke Id\", \"Brush Id\", \"Paint Id\", \"Repeat Id\", \"Layer Id\", \"Parent Id\", \"Target Count\", \"Map Red\", \"Map Green\", \"Map Blue\", \"Brush Model Id\", \"Brush Shape\", \"Segment Id\"";
	setAttr ".docAttr_StrokeSortDirection" -type "string" "The direction to apply the sort for this key.\nEnumerated strings are: \"Ascending\", \"Descending\"";
	setAttr ".docAttr_StrokeSortTexture" -type "string" "Strokes may be sorted based on channels of a projected texture.";
	setAttr ".docAttr_Output" -type "string" "Array of strokes as strokeData. Use the strokeQuery command to interrogate this data.";
	setAttr ".docAttr_ApplySort" -type "string" "Turn sorting on or off.";
	setAttr ".docAttr_StrokeFilterList" -type "string" "An array of filter definitions that can be used to filter out strokes from the output. Each filter definition consists of a key, an operator, and an operand. For example, a filter can be configured to filter out strokes whose brushId is less than 3.\nThe order of elements is not important since a stroke is only kept if it returns true for <i><strong>all</strong></i> filter entry comparisons.";
	setAttr ".docAttr_StrokeFilterKey" -type "string" "Filter brush strokes by comparing a specific stroke property with a value. \nThe strokeFilterKey property enables filtering based on the following stroke properties:\n\"Stroke Id\", \"Brush Id\", \"Paint Id\", \"Repeat Id\", \"Layer Id\", \"Parent Id\", \"Target Count\", \"Map Red\", \"Map Green\", \"Map Blue\", \"Brush Model Id\", \"Brush Shape\", \"Segment Id\"";
	setAttr ".docAttr_StrokeFilterOperator" -type "string" "The comparison operator that compares the strokeFilterKey with a value.\nEnumerated strings are: &quot;&gt;&quot;, &quot;&lt;&quot;, &quot;==&quot;, &quot;!=&quot;, &quot;nop&quot;";
	setAttr ".docAttr_StrokeFilterOperand" -type "string" "The value against which to compare the strokeFilterKey.";
	setAttr ".docAttr_StrokeFilterTexture" -type "string" "Strokes may be filtered based on a comparison with the channels of a projected texture. For example, a filter could filter out all strokes whose pivot is in an area of 3d space where the projection value is below mid grey (128)";
	setAttr ".docAttr_ApplyFilters" -type "string" "Turn filtering on or off.";
	setAttr ".docAttr_StartFrom" -type "string" "Only output strokes within a range. startFrom works in conjunction with endAt. This can be useful for debugging for example, by evaluating only a single stroke.";
	setAttr ".docAttr_EndAt" -type "string" "endAt defines the exclusive upper limit of the stroke range to output. If startFrom is 0 and endAt is 2, then only stroke 0 and 1 will make it to the output.";
	setAttr ".docAttr_LinearSpeed" -type "string" "Speed limit for the robot's linear moves for strokes generated by this node, measured in cm per second.";
	setAttr ".docAttr_AngularSpeed" -type "string" "Speed limit for the robot's angular moves for strokes generated by this node, measured in degrees per second.";
	setAttr ".docAttr_ApproximationDistance" -type "string" "Rounding distance for targets.";
	setAttr ".docAttr_Coats" -type "string" "How many times to repeat the strokes for this node. Useful if a particular paint is not very thick, or if the brush bristles are so stiff that paint is appied too thinly.";
	setAttr ".docAttr_LayerId" -type "string" "Assign a layer Id to this node's strokes that can be used for sorting or filtering downstream in the network.";
	setAttr ".docAttr_PointDensity" -type "string" "Set the approximate number of targets per centimeter.";
	setAttr ".docAttr_BrushTwist" -type "string" "Set the twist rotation of the brush at the start and end of each stroke. Twist rotation is a rotation about the axis defined by the brush handle.\nIf the follow attribute is turned on, then the initial brush target transformations flow along the tangent of the curve, otherwise they are aligned with the canvasMatrix. Twist rotation, as well as tilt and bank, are relative to this initial rotation.";
	setAttr ".docAttr_BrushTwistStart" -type "string" "Set the twist rotation of the brush at the start of each stroke.";
	setAttr ".docAttr_BrushTwistEnd" -type "string" "Set the twist rotation of the brush at the end of each stroke.";
	setAttr ".docAttr_BrushTilt" -type "string" "Set the tilt rotation of the brush at the start and end of each stroke. Tilt rotation is a rotation back and forth in the direction of travel. It is the most important rotation to get right since it affects the ability of the brush to deposit paint on the canvas.";
	setAttr ".docAttr_BrushTiltStart" -type "string" "Set the tilt rotation of the brush at the start of each stroke.";
	setAttr ".docAttr_BrushTiltEnd" -type "string" "Set the tilt rotation of the brush at the end of each stroke.";
	setAttr ".docAttr_BrushBank" -type "string" "Set the bank rotation of the brush at the start and end of each stroke. Bank rotation is a rotation around direction of travel. It is rarely needed.";
	setAttr ".docAttr_BrushBankStart" -type "string" "Set the bank rotation of the brush at the start of each stroke.";
	setAttr ".docAttr_BrushBankEnd" -type "string" "Set the bank rotation of the brush at the end of each stroke.";
	setAttr ".docAttr_BrushRotateOrder" -type "string" "Sets the rotation order for the above rotations.\nAvailable values are: \"twistTiltBank\", \"tiltBankTwist\", \"bankTwistTilt\", \"tiltTwistBank\", \"twistBankTilt\", \"bankTiltTwist\"";
	setAttr ".docAttr_CanvasMatrix" -type "string" "A frame of reference for strokes. The canvas matrix defines the default orientation for targets. The Z axis should be normal to te canvas as the brush handle is aligned in Z. X is the axis to the side of the brush, and Y axis is the back of the brush. Negative Y faces the direction of travel if follow is on, however as mentioned, this can be offset with the twist, tilt, and bank attributes.";
	setAttr ".docAttr_StrokeLength" -type "string" "Strokes are generated at this length. If the curve is longer than strokeLength, more than one stroke is generated.";
createNode cImgDilate -n "cImgDilate1";
	rename -uid "F1739B45-2344-64C7-8B5E-3190FBA39D41";
	addAttr -ci true -sn "docMeta_Author" -ln "docMeta_Author" -dt "string";
	addAttr -ci true -sn "docMeta_Email" -ln "docMeta_Email" -dt "string";
	addAttr -ci true -sn "docMeta_Description" -ln "docMeta_Description" -dt "string";
	addAttr -ci true -sn "docBlock_Synopsis" -ln "docBlock_Synopsis" -dt "string";
	addAttr -ci true -sn "docAttr_Input" -ln "docAttr_Input" -dt "string";
	addAttr -ci true -sn "docAttr_Invert" -ln "docAttr_Invert" -dt "string";
	addAttr -ci true -sn "docAttr_Output" -ln "docAttr_Output" -dt "string";
	addAttr -ci true -sn "docAttr_Amount" -ln "docAttr_Amount" -dt "string";
	addAttr -ci true -sn "docAttr_Blur" -ln "docAttr_Blur" -dt "string";
	addAttr -ci true -sn "docAttr_Threshold" -ln "docAttr_Threshold" -dt "string";
	addAttr -ci true -sn "docAttr_Median" -ln "docAttr_Median" -dt "string";
	addAttr -ci true -sn "docAttr_Bitwise" -ln "docAttr_Bitwise" -dt "string";
	setAttr ".docMeta_Author" -type "string" "Julian Mann";
	setAttr ".docMeta_Email" -type "string" "julian.mann@gmail.com";
	setAttr ".docMeta_Description" -type "string" "Dilate or erode a cImg";
	setAttr ".docBlock_Synopsis" -type "string" "cImgDilate is an image transformation that can be used to grow or shrink the white areas of a mask. Besides dilation, it also allows for some common image operations that are useful when dilating an image, such as blur and median filters.";
	setAttr ".docAttr_Input" -type "string" "The input image.";
	setAttr ".docAttr_Invert" -type "string" "Invert the input.";
	setAttr ".docAttr_Output" -type "string" "The output image";
	setAttr ".docAttr_Amount" -type "string" "Dilation amount. A negative value erodes the image.";
	setAttr ".docAttr_Blur" -type "string" "Perform a gaussian blur prior to dilating.";
	setAttr ".docAttr_Threshold" -type "string" "Set a threshold value on the image. thi is useful for ensuring that the dilation is performed in an image of black and white only.";
	setAttr ".docAttr_Median" -type "string" "Perform a median filter to fill small holes.";
	setAttr ".docAttr_Bitwise" -type "string" "Since the dilation operation can expand or shrink the original, the bitwise ops make it possible to, for example isolate only the area that is different, thereby resulting in a border mask.  \nAvailable operations are: \"None\", \"OR\", \"AND\", \"XOR\"";
createNode nodeGraphEditorInfo -n "MayaNodeEditorSavedTabsInfo";
	rename -uid "7F447338-2445-1FCC-1D42-FB8DD06722E2";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -458.92855319238919 -89.880948809404401 ;
	setAttr ".tgi[0].vh" -type "double2" 495.83331363068771 572.02378679362653 ;
	setAttr -s 2 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" 19.642856597900391;
	setAttr ".tgi[0].ni[0].y" 131.547607421875;
	setAttr ".tgi[0].ni[0].nvs" 18304;
	setAttr ".tgi[0].ni[1].x" 243.45237731933594;
	setAttr ".tgi[0].ni[1].y" 302.97616577148438;
	setAttr ".tgi[0].ni[1].nvs" 18304;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".st";
	setAttr -k on ".an";
	setAttr -k on ".pt";
select -ne :renderGlobalsList1;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
select -ne :defaultShaderList1;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 5 ".s";
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
select -ne :defaultRenderingList1;
select -ne :initialShadingGroup;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -k on ".an";
	setAttr -k on ".il";
	setAttr -k on ".vo";
	setAttr -k on ".eo";
	setAttr -k on ".fo";
	setAttr -k on ".epo";
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr -av -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -k on ".an";
	setAttr -k on ".il";
	setAttr -k on ".vo";
	setAttr -k on ".eo";
	setAttr -k on ".fo";
	setAttr -k on ".epo";
	setAttr ".ro" yes;
select -ne :defaultRenderGlobals;
	addAttr -ci true -h true -sn "dss" -ln "defaultSurfaceShader" -dt "string";
	setAttr ".ren" -type "string" "arnold";
	setAttr ".dss" -type "string" "lambert1";
select -ne :defaultResolution;
	setAttr -av -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -av ".w";
	setAttr -av ".h";
	setAttr -av ".pa" 1;
	setAttr -av ".al";
	setAttr -av ".dar";
	setAttr -av -k on ".ldar";
	setAttr -cb on ".dpi";
	setAttr -av -k on ".off";
	setAttr -av -k on ".fld";
	setAttr -av -k on ".zsl";
	setAttr -cb on ".isu";
	setAttr -cb on ".pdu";
select -ne :defaultColorMgtGlobals;
	setAttr ".cfe" yes;
	setAttr ".cfp" -type "string" "<MAYA_RESOURCES>/OCIO-configs/Maya2022-default/config.ocio";
	setAttr ".vtn" -type "string" "ACES 1.0 SDR-video (sRGB)";
	setAttr ".vn" -type "string" "ACES 1.0 SDR-video";
	setAttr ".dn" -type "string" "sRGB";
	setAttr ".wsn" -type "string" "ACEScg";
	setAttr ".otn" -type "string" "ACES 1.0 SDR-video (sRGB)";
	setAttr ".potn" -type "string" "ACES 1.0 SDR-video (sRGB)";
select -ne :hardwareRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
	setAttr -k off ".fbfm";
	setAttr -k off -cb on ".ehql";
	setAttr -k off -cb on ".eams";
	setAttr -k off -cb on ".eeaa";
	setAttr -k off -cb on ".engm";
	setAttr -k off -cb on ".mes";
	setAttr -k off -cb on ".emb";
	setAttr -av -k off -cb on ".mbbf";
	setAttr -k off -cb on ".mbs";
	setAttr -k off -cb on ".trm";
	setAttr -k off -cb on ".tshc";
	setAttr -k off ".enpt";
	setAttr -k off -cb on ".clmt";
	setAttr -k off -cb on ".tcov";
	setAttr -k off -cb on ".lith";
	setAttr -k off -cb on ".sobc";
	setAttr -k off -cb on ".cuth";
	setAttr -k off -cb on ".hgcd";
	setAttr -k off -cb on ".hgci";
	setAttr -k off -cb on ".mgcs";
	setAttr -k off -cb on ".twa";
	setAttr -k off -cb on ".twz";
	setAttr -k on ".hwcc";
	setAttr -k on ".hwdp";
	setAttr -k on ".hwql";
	setAttr -k on ".hwfr";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "cImgDilate1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[0].dn";
connectAttr "curveStroke1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[1].dn";
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
// End of doc.ma
