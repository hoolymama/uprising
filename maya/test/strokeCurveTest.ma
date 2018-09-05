//Maya ASCII 2017ff04 scene
//Name: strokeCurveTest.ma
//Last modified: Mon, Sep 03, 2018 09:24:55 PM
//Codeset: UTF-8
requires maya "2017ff04";
requires -nodeType "glyphShape" -nodeType "matrixGlyphShape" "Congregation" "2017.dev.999";
requires -nodeType "strokeCurve" -nodeType "painting" -dataType "strokeCurveData"
		 -dataType "paintingData" "Uprising" "2017.dev.999";
requires "stereoCamera" "10.0";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2017";
fileInfo "version" "2017";
fileInfo "cutIdentifier" "201702071345-1015190";
fileInfo "osv" "Mac OS X 10.12.6";
createNode transform -s -n "persp";
	rename -uid "4E161BFB-4046-46A3-EE6C-DD8D384A4713";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -92.58389634828302 -127.85366931517498 103.24688052203045 ;
	setAttr ".r" -type "double3" 41.061647270725842 -3.1805546814635168e-15 -73.400000001201988 ;
	setAttr ".rp" -type "double3" 1.4210854715202004e-14 8.8817841970012523e-15 5.6843418860808015e-14 ;
	setAttr ".rpt" -type "double3" -3.2126587147117258e-14 -5.2098900557035962e-14 -2.0466006432280544e-14 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "D64D2300-DA4C-D4C6-FC8B-7EBCB3B5CCEF";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 322.6673667174004;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 36.671017525582783 19.908249567891417 77.271644064924473 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	rename -uid "E2738A7A-F348-1DF6-7ED8-069DAC9E8234";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 -21.5 1002.6909561336539 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "D3321D4A-644A-4334-B309-358CEBA3D551";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1002.6909561336539;
	setAttr ".ow" 202.04025881893023;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".tp" -type "double3" 0 -21.5 0 ;
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	rename -uid "79A8C51D-B845-C7BA-BF46-33BBE5AC90A7";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 -1000.1 0 ;
	setAttr ".r" -type "double3" 89.999999999999986 0 0 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "0854F84C-3A41-AE29-EA69-A5A0F98D709B";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	rename -uid "0623B662-8441-54CD-7E30-E787C566C4C3";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 90 1.2722218725854067e-14 89.999999999999986 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "B6492A5E-F44A-E1A2-A2A3-67A8C2D4BB6C";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "matrixGlyph1";
	rename -uid "E31F4A2D-9149-4CEA-0348-0580BD6AAB8A";
	setAttr ".v" no;
createNode matrixGlyphShape -n "matrixGlyphShape1" -p "matrixGlyph1";
	rename -uid "88642C9A-8D48-9830-F994-B19171165052";
	setAttr -k off ".v";
	setAttr ".mlt" 1.75;
createNode transform -n "glyph1";
	rename -uid "0F40168F-7047-8F49-BA92-66AF2E34BABC";
createNode glyphShape -n "glyphShape1" -p "glyph1";
	rename -uid "B709D731-0E4C-562D-D4D3-5D9A2CE2F34A";
	setAttr -k off ".v";
	setAttr ".dc" -type "float3" 1 1 0 ;
createNode transform -n "mainPaintingGroup";
	rename -uid "5B9E2649-C642-1C2B-9606-6D9DBE647AF0";
	addAttr -ci true -sn "zeroPosition" -ln "zeroPosition" -min 0 -max 1 -at "bool";
	setAttr -k on ".zeroPosition" yes;
createNode transform -n "mainProjection" -p "mainPaintingGroup";
	rename -uid "F19A8D54-FA47-6801-9245-189EC9271C88";
	setAttr ".s" -type "double3" 61 61 1 ;
createNode sketchPlane -n "mainProjectionShape" -p "mainProjection";
	rename -uid "A1EA9B6C-674F-9048-422C-75BBC4DAA612";
	setAttr -k off ".v";
createNode transform -n "curves" -p "mainPaintingGroup";
	rename -uid "F757EE73-6340-5373-4457-06A501E2F19C";
createNode transform -n "c9" -p "|mainPaintingGroup|curves";
	rename -uid "CA21D0A7-BE41-6C97-BFB3-75819C03CC56";
	setAttr ".t" -type "double3" 0 -884.42700336572238 0 ;
	setAttr ".s" -type "double3" 0.86666666204514198 13.407956573523578 1 ;
createNode nurbsCurve -n "cShape9" -p "c9";
	rename -uid "80EBC74A-0945-302B-895F-D6A091A45D79";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		-70 70 0
		70 70 0
		;
createNode transform -n "c8" -p "|mainPaintingGroup|curves";
	rename -uid "328352A1-BA4B-1A47-E435-59BDDAAD6A0A";
	setAttr ".t" -type "double3" 0 -897.83495993924589 0 ;
	setAttr ".s" -type "double3" 0.86666666204514198 13.407956573523578 1 ;
createNode nurbsCurve -n "cShape8" -p "c8";
	rename -uid "AFD462E5-E244-D5D7-DAB0-A88E17C92712";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		-70 70 0
		70 70 0
		;
createNode transform -n "c7" -p "|mainPaintingGroup|curves";
	rename -uid "5654D131-594A-5F9A-088D-CDA691547669";
	setAttr ".t" -type "double3" 0 -911.2429165127694 0 ;
	setAttr ".s" -type "double3" 0.86666666204514198 13.407956573523578 1 ;
createNode nurbsCurve -n "cShape7" -p "c7";
	rename -uid "4BAA8505-7649-AFD2-B37E-8495A2739051";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		-70 70 0
		70 70 0
		;
createNode transform -n "c6" -p "|mainPaintingGroup|curves";
	rename -uid "EB757C53-DE4B-7F61-64BA-8A8291F7F3FC";
	setAttr ".t" -type "double3" 0 -924.6508730862929 0 ;
	setAttr ".s" -type "double3" 0.86666666204514198 13.407956573523578 1 ;
createNode nurbsCurve -n "cShape6" -p "c6";
	rename -uid "2D6DFF6E-1C4A-E42C-9E9C-848347D5BDAA";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		-70 70 0
		70 70 0
		;
createNode transform -n "c5" -p "|mainPaintingGroup|curves";
	rename -uid "E2D13EE7-974F-15C8-B074-0E876C663699";
	setAttr ".t" -type "double3" 0 -938.05882965981664 0 ;
	setAttr ".s" -type "double3" 0.86666666204514198 13.407956573523578 1 ;
createNode nurbsCurve -n "cShape5" -p "c5";
	rename -uid "267C2A05-1B4B-E0CF-7B6D-87ADB3F35942";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		-70 70 0
		70 70 0
		;
createNode transform -n "c4" -p "|mainPaintingGroup|curves";
	rename -uid "EDAB18B2-434B-F059-50B2-B18169870783";
	setAttr ".t" -type "double3" 0 -951.46678623334014 0 ;
	setAttr ".s" -type "double3" 0.86666666204514198 13.407956573523578 1 ;
createNode nurbsCurve -n "cShape4" -p "c4";
	rename -uid "406C5CE4-B046-E43D-8A01-1E872420D118";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		-70 70 0
		70 70 0
		;
createNode transform -n "c3" -p "|mainPaintingGroup|curves";
	rename -uid "EB759974-5844-992A-6F90-BEB147F87E70";
	setAttr ".t" -type "double3" 0 -964.87474280686365 0 ;
	setAttr ".s" -type "double3" 0.86666666204514198 13.407956573523578 1 ;
createNode nurbsCurve -n "cShape3" -p "c3";
	rename -uid "DC4F3910-ED40-6C02-EEBF-238971927459";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		-70 70 0
		70 70 0
		;
createNode transform -n "c2" -p "|mainPaintingGroup|curves";
	rename -uid "950B9693-154A-16DA-FE6B-D99B53B4184F";
	setAttr ".t" -type "double3" 0 -978.28269938038738 0 ;
	setAttr ".s" -type "double3" 0.86666666204514198 13.407956573523578 1 ;
createNode nurbsCurve -n "cShape2" -p "c2";
	rename -uid "A4089F08-E742-F24C-25F0-0CBA9F25709C";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		-70 70 0
		70 70 0
		;
createNode transform -n "c1" -p "|mainPaintingGroup|curves";
	rename -uid "177C34D4-BE40-10B5-F67B-AFB1C6E6701E";
	setAttr ".t" -type "double3" 0 -991.69065595391089 0 ;
	setAttr ".s" -type "double3" 0.86666666204514198 13.407956573523578 1 ;
createNode nurbsCurve -n "cShape1" -p "c1";
	rename -uid "EFE974C7-E84E-1F26-56BF-25A2AE2F7E5F";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		-70 70 0
		70 70 0
		;
createNode joint -n "jpos" -p "mainPaintingGroup";
	rename -uid "23369208-5846-A2A3-153B-2EB687556B7D";
	setAttr ".r" -type "double3" 8.5377364625159387e-07 1.8957583197733855e-22 1.4124500153760504e-30 ;
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 0 0 89.999999999999986 ;
	setAttr ".pa" -type "double3" 8.5377364625159387e-07 1.8957583197733855e-22 1.4124500153760504e-30 ;
	setAttr ".radi" 2;
createNode joint -n "j2" -p "|mainPaintingGroup|jpos";
	rename -uid "763E5BF1-334A-BDB0-232A-908F40A1E427";
	setAttr ".t" -type "double3" 121.99999999999997 2.7089441800853807e-14 0 ;
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".radi" 2;
createNode ikEffector -n "pasted__effector1" -p "|mainPaintingGroup|jpos";
	rename -uid "92F50B0A-504A-3341-D0AC-B1857DDB9D99";
	setAttr ".v" no;
	setAttr ".hd" yes;
createNode transform -n "mainPainting" -p "|mainPaintingGroup|jpos";
	rename -uid "636B980E-5E48-F5EB-C32E-BA8E0AD2E89F";
	setAttr ".t" -type "double3" 61 -60.999999999999972 0 ;
	setAttr ".r" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".s" -type "double3" 0.99999999999999978 0.99999999999999978 1 ;
createNode painting -n "mainPaintingShape" -p "mainPainting";
	rename -uid "83869513-3846-004A-3068-D588FF32F41F";
	setAttr -k off ".v";
	setAttr ".agsp" 20;
	setAttr -s 9 ".scrvs";
	setAttr -s 13 ".bsh";
	setAttr -s 8 ".pts";
	setAttr ".psi" 1.8181818198751321;
	setAttr ".lln" 2.4339999999999997;
	setAttr ".ltk" 2.3863636374219577;
	setAttr ".dal" no;
createNode ikHandle -n "ikHandle" -p "mainPaintingGroup";
	rename -uid "CA32DFD7-1843-DA45-D3B4-88982ADBD9E0";
	setAttr ".roc" yes;
createNode poleVectorConstraint -n "pasted__ikHandle1_poleVectorConstraint1" -p "|mainPaintingGroup|ikHandle";
	rename -uid "164F3D89-0549-8683-E374-169C66BE7AA7";
	addAttr -dcb 0 -ci true -k true -sn "w0" -ln "TRW0" -dv 1 -min 0 -at "double";
	addAttr -dcb 0 -ci true -k true -sn "w1" -ln "TRZW1" -dv 1 -min 0 -at "double";
	setAttr -k on ".nds";
	setAttr -k off ".v";
	setAttr -k off ".tx";
	setAttr -k off ".ty";
	setAttr -k off ".tz";
	setAttr -k off ".rx";
	setAttr -k off ".ry";
	setAttr -k off ".rz";
	setAttr -k off ".sx";
	setAttr -k off ".sy";
	setAttr -k off ".sz";
	setAttr ".erp" yes;
	setAttr -s 2 ".tg";
	setAttr ".rst" -type "double3" 91.225016950712956 3.1815101856877561 122.16377670308785 ;
	setAttr -k on ".w0";
	setAttr -k on ".w1";
createNode transform -n "BL" -p "mainPaintingGroup";
	rename -uid "6332278C-C74B-8AA5-5F71-39BFD1699A08";
	setAttr ".t" -type "double3" -25 177.10000000000002 37.5 ;
	setAttr -k on ".rpx";
	setAttr -k on ".rpy";
	setAttr -k on ".rpz";
	setAttr -k on ".rptx";
	setAttr -k on ".rpty";
	setAttr -k on ".rptz";
	setAttr -k on ".spx";
	setAttr -k on ".spy";
	setAttr -k on ".spz";
	setAttr -k on ".sptx";
	setAttr -k on ".spty";
	setAttr -k on ".sptz";
createNode locator -n "BLShape" -p "BL";
	rename -uid "0C5AC171-D24F-9873-2459-5AB7BA87E718";
	setAttr -k off ".v";
createNode transform -n "TL" -p "mainPaintingGroup";
	rename -uid "404C7FFF-344E-E656-EC19-C5AE4B498DF8";
	setAttr ".t" -type "double3" -25.200000000000003 176.70000000000002 159.20000000000002 ;
	setAttr -k on ".rpx";
	setAttr -k on ".rpy";
	setAttr -k on ".rpz";
	setAttr -k on ".rptx";
	setAttr -k on ".rpty";
	setAttr -k on ".rptz";
	setAttr -k on ".spx";
	setAttr -k on ".spy";
	setAttr -k on ".spz";
	setAttr -k on ".sptx";
	setAttr -k on ".spty";
	setAttr -k on ".sptz";
createNode locator -n "TLShape" -p "TL";
	rename -uid "2BD191A2-334A-A54D-0100-49A25E42B573";
	setAttr -k off ".v";
createNode transform -n "TR" -p "mainPaintingGroup";
	rename -uid "5ED1C5EA-F74C-EE21-3230-679D29B64BC5";
	setAttr ".t" -type "double3" 96.800000000000011 180.60000000000002 159.70000000000002 ;
	setAttr -k on ".rpx";
	setAttr -k on ".rpy";
	setAttr -k on ".rpz";
	setAttr -k on ".rptx";
	setAttr -k on ".rpty";
	setAttr -k on ".rptz";
	setAttr -k on ".spx";
	setAttr -k on ".spy";
	setAttr -k on ".spz";
	setAttr -k on ".sptx";
	setAttr -k on ".spty";
	setAttr -k on ".sptz";
createNode locator -n "TRShape" -p "TR";
	rename -uid "9C31BEA5-7943-1661-C923-D495522AC6D5";
	setAttr -k off ".v";
createNode transform -n "BLZ" -p "mainPaintingGroup";
	rename -uid "42DCAE37-B641-F7AC-00F5-91A8D867ECA2";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -61 -61 0 ;
	setAttr -k on ".rpx";
	setAttr -k on ".rpy";
	setAttr -k on ".rpz";
	setAttr -k on ".rptx";
	setAttr -k on ".rpty";
	setAttr -k on ".rptz";
	setAttr -k on ".spx";
	setAttr -k on ".spy";
	setAttr -k on ".spz";
	setAttr -k on ".sptx";
	setAttr -k on ".spty";
	setAttr -k on ".sptz";
createNode locator -n "BLZShape" -p "BLZ";
	rename -uid "2D5A9CF5-3D4E-391E-E99E-558D41C0114D";
	setAttr -k off ".v";
createNode transform -n "TLZ" -p "mainPaintingGroup";
	rename -uid "3FFA41B5-D740-B8F4-7E3A-E2A8E5ED9008";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -61 61 0 ;
	setAttr -k on ".rpx";
	setAttr -k on ".rpy";
	setAttr -k on ".rpz";
	setAttr -k on ".rptx";
	setAttr -k on ".rpty";
	setAttr -k on ".rptz";
	setAttr -k on ".spx";
	setAttr -k on ".spy";
	setAttr -k on ".spz";
	setAttr -k on ".sptx";
	setAttr -k on ".spty";
	setAttr -k on ".sptz";
createNode locator -n "TLZShape" -p "TLZ";
	rename -uid "2217AFB7-DC49-0672-7BC0-CE8617AB5436";
	setAttr -k off ".v";
createNode transform -n "TRZ" -p "mainPaintingGroup";
	rename -uid "8E30F670-2B48-313F-7E7B-E7BBD8C3C427";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 61 61 0 ;
	setAttr -k on ".rpx";
	setAttr -k on ".rpy";
	setAttr -k on ".rpz";
	setAttr -k on ".rptx";
	setAttr -k on ".rpty";
	setAttr -k on ".rptz";
	setAttr -k on ".spx";
	setAttr -k on ".spy";
	setAttr -k on ".spz";
	setAttr -k on ".sptx";
	setAttr -k on ".spty";
	setAttr -k on ".sptz";
createNode locator -n "TRZShape" -p "TRZ";
	rename -uid "413C5D4C-6447-CBF3-A719-008A5B32C014";
	setAttr -k off ".v";
createNode transform -n "dipPaintingGroup";
	rename -uid "981E1453-7A48-48A9-4A3A-E9A7EA731139";
	addAttr -ci true -sn "zeroPosition" -ln "zeroPosition" -min 0 -max 1 -at "bool";
	setAttr -k on ".zeroPosition";
createNode joint -n "jpos" -p "dipPaintingGroup";
	rename -uid "ED6CA764-FD42-A94E-17EA-8E99513E77D7";
	setAttr ".r" -type "double3" -0.092144202184838125 0.1889071082443852 -77.726290748903892 ;
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" -2.756005166176776e-06 -0.095369713087940025 0.0033114795614464956 ;
	setAttr ".radi" 2;
createNode joint -n "j2" -p "|dipPaintingGroup|jpos";
	rename -uid "511F90FA-5745-CDB2-16CB-F49D7D176029";
	setAttr ".t" -type "double3" 135.90882973523097 -2.9552158324642731e-14 -8.5101370395079812e-13 ;
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" -2.7560051661767782e-06 0.095369713087940081 -0.0033114795614464978 ;
	setAttr ".radi" 2;
createNode ikEffector -n "effector1" -p "|dipPaintingGroup|jpos";
	rename -uid "4D60706C-6142-F73F-9876-CAB746BCEBF1";
	setAttr ".v" no;
	setAttr ".hd" yes;
createNode pointConstraint -n "jPcon" -p "|dipPaintingGroup|jpos";
	rename -uid "F10EBE91-A949-493B-FF22-7FA4E7BC96C7";
	addAttr -dcb 0 -ci true -k true -sn "w0" -ln "rack_LZW0" -dv 1 -min 0 -at "double";
	addAttr -dcb 0 -ci true -k true -sn "w1" -ln "rack_LW1" -dv 1 -min 0 -at "double";
	setAttr -k on ".nds";
	setAttr -k off ".v";
	setAttr -k off ".tx";
	setAttr -k off ".ty";
	setAttr -k off ".tz";
	setAttr -k off ".rx";
	setAttr -k off ".ry";
	setAttr -k off ".rz";
	setAttr -k off ".sx";
	setAttr -k off ".sy";
	setAttr -k off ".sz";
	setAttr ".erp" yes;
	setAttr -s 2 ".tg";
	setAttr ".rst" -type "double3" 16.409362677194736 -6.272205896226005 16.345627889559857 ;
	setAttr -k on ".w0";
	setAttr -k on ".w1";
createNode transform -n "dipPainting" -p "|dipPaintingGroup|jpos";
	rename -uid "F9BAD2C0-D343-7C80-5C10-39BC1E138355";
	setAttr ".t" -type "double3" 66.581600851156963 113.54057955733488 -3.8016014831578029 ;
	setAttr ".r" -type "double3" 0.00030491002218308635 0.095481207854802103 -0.0033148376089951782 ;
	setAttr ".s" -type "double3" 1 1.0000000000000002 1 ;
createNode painting -n "dipPaintingShape" -p "dipPainting";
	rename -uid "1DA5EFA2-754B-F135-FB4C-FEAF6ACA4D63";
	setAttr -k off ".v";
	setAttr ".agsp" 20;
	setAttr -s 36 ".scrvs";
	setAttr -s 13 ".bsh";
	setAttr -s 8 ".pts";
	setAttr ".ltk" 0.4060913629943344;
createNode transform -n "trays" -p "|dipPaintingGroup|jpos";
	rename -uid "7AACC5B5-9B42-3427-6BB7-E48629EDC87E";
	setAttr ".t" -type "double3" -92.939479787674998 -116.59280533084616 -28.854310048199409 ;
	setAttr ".r" -type "double3" 0.095481207856154132 -0.00030490959880129719 89.996684654270439 ;
	setAttr ".s" -type "double3" 1.0000000000000004 1 1 ;
	setAttr ".rp" -type "double3" 130.1427483018243 -159.46579520620423 41.684441269655736 ;
	setAttr ".rpt" -type "double3" 29.399821221978193 289.5990915842558 -0.26510878122373133 ;
	setAttr ".sp" -type "double3" 130.14274830182424 -159.46579520620423 41.684441269655736 ;
	setAttr ".spt" -type "double3" 5.684341886080804e-14 0 0 ;
createNode transform -n "tx_0_reddish" -p "trays";
	rename -uid "7BDF1A1E-3945-7C4C-348D-8BB0000ABC66";
	addAttr -ci true -uac -sn "sfPaintColor" -ln "sfPaintColor" -at "float3" -nc 3;
	addAttr -ci true -sn "sfPaintColorR" -ln "sfPaintColorR" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorG" -ln "sfPaintColorG" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorB" -ln "sfPaintColorB" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintOpacity" -ln "sfPaintOpacity" -at "double";
	addAttr -ci true -sn "sfPaintTravel" -ln "sfPaintTravel" -at "double";
	setAttr ".t" -type "double3" 126.3681375391607 -104.8200304242059 25.390381126241763 ;
	setAttr ".r" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".rp" -type "double3" 0 0 0.073118674440063813 ;
	setAttr ".sp" -type "double3" 0 0 0.073118674440063813 ;
	setAttr -k on ".sfPaintColor" -type "float3" 1 0.382 0 ;
	setAttr -k on ".sfPaintColor";
	setAttr -k on ".sfPaintOpacity" 1;
	setAttr -k on ".sfPaintTravel" 60;
createNode mesh -n "tx_0_reddishShape" -p "tx_0_reddish";
	rename -uid "044EB4CF-804A-8A63-1561-A8BA860AF89C";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:14]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[15:19]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.74509802460670471 0.36781443655490875 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 50 ".uvst[0].uvsp[0:49]" -type "float2" 0.375 0 0.38510099
		 0.014371129 0.38510102 0.23562887 0.61519605 0.23562887 0.625 0.25 0.375 0.25 0.61519605
		 0.014371129 0.625 0 0.29166669 0 0.29166669 0.25 0.375 0.91666663 0.625 0.91666663
		 0.625 0.99999994 0.375 0.99999994 0.70833337 0 0.70833337 0.25 0.625 0.33333334 0.375
		 0.33333334 0.20833334 0 0.20833334 0.25 0.375 0.83333331 0.625 0.83333331 0.79166669
		 0 0.79166669 0.25 0.625 0.41666669 0.375 0.41666669 0.125 0 0.125 0.25 0.375 0.75
		 0.625 0.75 0.875 0 0.875 0.25 0.625 0.5 0.375 0.5 0.625 0.27153829 0.375 0.27153829
		 0.375 0.5 0.625 0.5 0.64653826 5.17414e-18 0.64653826 0.25 0.875 0.25 0.875 0 0.375
		 0.75 0.625 0.75 0.35346171 0.25 0.35346171 1.1274109e-17 0.125 0 0.125 0.25 0.375
		 0.97846174 0.625 0.97846174;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcol" yes;
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 24 ".pt[0:23]" -type "float3"  7.7621551 -0.74639893 -0.46948725 
		5.6701221 -0.48628235 -0.46948725 -6.6329327 -16.387344 -0.46948725 -37.000687 11.088562 
		1.9167554 -39.17366 10.924011 1.9167554 -6.1391048 -18.713074 -0.46948725 -22.866409 
		26.989624 1.9167554 -23.203276 28.890701 1.9167554 6.1947575 -0.47973633 -0.32764608 
		-6.5717068 -16.979736 -0.32764608 -22.85515 28.575119 1.9167554 -37.52182 12.075104 
		1.9167554 6.1947575 -0.47973633 -0.22401451 -6.5717068 -16.979736 -0.22401451 -22.85515 
		28.575119 1.9167554 -37.52182 12.075104 1.9167554 4.8153095 0.41189575 -0.29392323 
		-7.5256023 -15.538101 -0.29392323 -21.932907 28.765991 -0.31726244 -36.110703 12.815979 
		-0.31726244 -7.6484127 -14.507767 -0.36971217 -23.479355 -0.65306091 -0.31726244 
		-11.930174 14.273666 -0.31726244 3.9007664 0.41897583 -0.36971217;
	setAttr -s 24 ".vt[0:23]"  -0.81153107 -8.98333359 4.099960327 0.48139954 -7.95052719 4.099960327
		 0.48139954 7.95053101 4.099960327 29.93354797 7.95053101 4.099960327 31.1884613 8.9833374 4.099960327
		 -0.81153107 8.9833374 4.099960327 29.93354797 -7.95052719 4.099960327 31.1884613 -8.98333359 4.099960327
		 0.18846893 -8.25 3.3907547 0.18846893 8.25 3.3907547 30.18847656 -8.25 3.3907547
		 30.18847656 8.25 3.3907547 0.18846893 -8.25 2.87259674 0.18846893 8.25 2.87259674
		 30.18847656 -8.25 2.87259674 30.18847656 8.25 2.87259674 1.35513306 -7.97499847 0
		 1.35513306 7.97499847 0 29.02180481 -7.97499847 0 29.02180481 7.97499847 0 1.87381744 7.46335602 0.3789444
		 17.70475769 7.46335602 0.3789444 17.70475769 -7.46335983 0.3789444 1.87381744 -7.46335983 0.3789444;
	setAttr -s 42 ".ed[0:41]"  0 1 1 1 2 0 2 3 0 3 4 1 4 5 0 5 0 0 3 6 1
		 6 1 0 0 7 0 7 4 0 8 0 0 5 9 0 9 8 1 8 10 1 10 7 0 10 11 1 11 4 0 11 9 1 12 8 0 9 13 0
		 13 12 1 12 14 1 14 10 0 14 15 1 15 11 0 15 13 1 16 12 0 13 17 0 17 16 0 16 18 0 18 14 0
		 18 19 0 19 15 0 19 17 0 2 20 0 20 21 0 21 3 0 21 22 1 22 6 0 20 23 0 23 22 0 1 23 0;
	setAttr -s 20 -ch 84 ".fc[0:19]" -type "polyFaces" 
		f 6 0 1 2 3 4 5
		mu 0 6 0 1 2 3 4 5
		f 6 6 7 -1 8 9 -4
		mu 0 6 3 6 1 0 7 4
		f 4 10 -6 11 12
		mu 0 4 8 0 5 9
		f 4 13 14 -9 -11
		mu 0 4 10 11 12 13
		f 4 -15 15 16 -10
		mu 0 4 7 14 15 4
		f 4 -5 -17 17 -12
		mu 0 4 5 4 16 17
		f 4 18 -13 19 20
		mu 0 4 18 8 9 19
		f 4 21 22 -14 -19
		mu 0 4 20 21 11 10
		f 4 -23 23 24 -16
		mu 0 4 14 22 23 15
		f 4 -18 -25 25 -20
		mu 0 4 17 16 24 25
		f 4 26 -21 27 28
		mu 0 4 26 18 19 27
		f 4 29 30 -22 -27
		mu 0 4 28 29 21 20
		f 4 -31 31 32 -24
		mu 0 4 22 30 31 23
		f 4 -26 -33 33 -28
		mu 0 4 25 24 32 33
		f 4 -34 -32 -30 -29
		mu 0 4 33 32 29 28
		f 4 -3 34 35 36
		mu 0 4 34 35 36 37
		f 4 -7 -37 37 38
		mu 0 4 38 39 40 41
		f 4 39 40 -38 -36
		mu 0 4 36 42 43 37
		f 4 -2 41 -40 -35
		mu 0 4 44 45 46 47
		f 4 -8 -39 -41 -42
		mu 0 4 48 49 43 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "tx_1_sky" -p "trays";
	rename -uid "E643068E-4749-D5FE-6B8E-DDBB770C379D";
	addAttr -ci true -uac -sn "sfPaintColor" -ln "sfPaintColor" -at "float3" -nc 3;
	addAttr -ci true -sn "sfPaintColorR" -ln "sfPaintColorR" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorG" -ln "sfPaintColorG" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorB" -ln "sfPaintColorB" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintOpacity" -ln "sfPaintOpacity" -at "double";
	addAttr -ci true -sn "sfPaintTravel" -ln "sfPaintTravel" -at "double";
	setAttr ".t" -type "double3" 126.3681375391607 -121.29598232347806 25.390381126241763 ;
	setAttr ".r" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".rp" -type "double3" 0 0 0.073118674440063813 ;
	setAttr ".sp" -type "double3" 0 0 0.073118674440063813 ;
	setAttr -k on ".sfPaintColor" -type "float3" 0 0.359 1 ;
	setAttr -k on ".sfPaintColor";
	setAttr -k on ".sfPaintOpacity" 1;
	setAttr -k on ".sfPaintTravel" 60;
createNode mesh -n "tx_1_skyShape" -p "tx_1_sky";
	rename -uid "79A8E2C2-6946-0F86-007C-69A9BC8C95C0";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:14]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[15:19]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.7083333432674408 0.49999997019767761 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 50 ".uvst[0].uvsp[0:49]" -type "float2" 0.375 0 0.38510099
		 0.014371129 0.38510102 0.23562887 0.61519605 0.23562887 0.625 0.25 0.375 0.25 0.61519605
		 0.014371129 0.625 0 0.29166669 0 0.29166669 0.25 0.375 0.91666663 0.625 0.91666663
		 0.625 0.99999994 0.375 0.99999994 0.70833337 0 0.70833337 0.25 0.625 0.33333334 0.375
		 0.33333334 0.20833334 0 0.20833334 0.25 0.375 0.83333331 0.625 0.83333331 0.79166669
		 0 0.79166669 0.25 0.625 0.41666669 0.375 0.41666669 0.125 0 0.125 0.25 0.375 0.75
		 0.625 0.75 0.875 0 0.875 0.25 0.625 0.5 0.375 0.5 0.625 0.27153829 0.375 0.27153829
		 0.375 0.5 0.625 0.5 0.64653826 5.17414e-18 0.64653826 0.25 0.875 0.25 0.875 0 0.375
		 0.75 0.625 0.75 0.35346171 0.25 0.35346171 1.1274109e-17 0.125 0 0.125 0.25 0.375
		 0.97846174 0.625 0.97846174;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcol" yes;
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 24 ".pt[0:23]" -type "float3"  7.7621551 -0.74639893 -0.46948725 
		5.6701221 -0.48628235 -0.46948725 -6.6329327 -16.387344 -0.46948725 -37.000687 11.088562 
		1.9167554 -39.17366 10.924011 1.9167554 -6.1391048 -18.713074 -0.46948725 -22.866409 
		26.989624 1.9167554 -23.203276 28.890701 1.9167554 6.1947575 -0.47973633 -0.32764608 
		-6.5717068 -16.979736 -0.32764608 -22.85515 28.575119 1.9167554 -37.52182 12.075104 
		1.9167554 6.1947575 -0.47973633 -0.22401451 -6.5717068 -16.979736 -0.22401451 -22.85515 
		28.575119 1.9167554 -37.52182 12.075104 1.9167554 4.8153095 0.41189575 -0.29392323 
		-7.5256023 -15.538101 -0.29392323 -21.932907 28.765991 -0.31726244 -36.110703 12.815979 
		-0.31726244 -7.6484127 -14.507767 -0.36971217 -23.479355 -0.65306091 -0.31726244 
		-11.930174 14.273666 -0.31726244 3.9007664 0.41897583 -0.36971217;
	setAttr -s 24 ".vt[0:23]"  -0.81153107 -8.98333359 4.099960327 0.48139954 -7.95052719 4.099960327
		 0.48139954 7.95053101 4.099960327 29.93354797 7.95053101 4.099960327 31.1884613 8.9833374 4.099960327
		 -0.81153107 8.9833374 4.099960327 29.93354797 -7.95052719 4.099960327 31.1884613 -8.98333359 4.099960327
		 0.18846893 -8.25 3.3907547 0.18846893 8.25 3.3907547 30.18847656 -8.25 3.3907547
		 30.18847656 8.25 3.3907547 0.18846893 -8.25 2.87259674 0.18846893 8.25 2.87259674
		 30.18847656 -8.25 2.87259674 30.18847656 8.25 2.87259674 1.35513306 -7.97499847 0
		 1.35513306 7.97499847 0 29.02180481 -7.97499847 0 29.02180481 7.97499847 0 1.87381744 7.46335602 0.3789444
		 17.70475769 7.46335602 0.3789444 17.70475769 -7.46335983 0.3789444 1.87381744 -7.46335983 0.3789444;
	setAttr -s 42 ".ed[0:41]"  0 1 1 1 2 0 2 3 0 3 4 1 4 5 0 5 0 0 3 6 1
		 6 1 0 0 7 0 7 4 0 8 0 0 5 9 0 9 8 1 8 10 1 10 7 0 10 11 1 11 4 0 11 9 1 12 8 0 9 13 0
		 13 12 1 12 14 1 14 10 0 14 15 1 15 11 0 15 13 1 16 12 0 13 17 0 17 16 0 16 18 0 18 14 0
		 18 19 0 19 15 0 19 17 0 2 20 0 20 21 0 21 3 0 21 22 1 22 6 0 20 23 0 23 22 0 1 23 0;
	setAttr -s 20 -ch 84 ".fc[0:19]" -type "polyFaces" 
		f 6 0 1 2 3 4 5
		mu 0 6 0 1 2 3 4 5
		f 6 6 7 -1 8 9 -4
		mu 0 6 3 6 1 0 7 4
		f 4 10 -6 11 12
		mu 0 4 8 0 5 9
		f 4 13 14 -9 -11
		mu 0 4 10 11 12 13
		f 4 -15 15 16 -10
		mu 0 4 7 14 15 4
		f 4 -5 -17 17 -12
		mu 0 4 5 4 16 17
		f 4 18 -13 19 20
		mu 0 4 18 8 9 19
		f 4 21 22 -14 -19
		mu 0 4 20 21 11 10
		f 4 -23 23 24 -16
		mu 0 4 14 22 23 15
		f 4 -18 -25 25 -20
		mu 0 4 17 16 24 25
		f 4 26 -21 27 28
		mu 0 4 26 18 19 27
		f 4 29 30 -22 -27
		mu 0 4 28 29 21 20
		f 4 -31 31 32 -24
		mu 0 4 22 30 31 23
		f 4 -26 -33 33 -28
		mu 0 4 25 24 32 33
		f 4 -34 -32 -30 -29
		mu 0 4 33 32 29 28
		f 4 -3 34 35 36
		mu 0 4 34 35 36 37
		f 4 -7 -37 37 38
		mu 0 4 38 39 40 41
		f 4 39 40 -38 -36
		mu 0 4 36 42 43 37
		f 4 -2 41 -40 -35
		mu 0 4 44 45 46 47
		f 4 -8 -39 -41 -42
		mu 0 4 48 49 43 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "tx_2_orange" -p "trays";
	rename -uid "F3D2DE66-9043-51E7-6854-3F9A333A109F";
	addAttr -ci true -uac -sn "sfPaintColor" -ln "sfPaintColor" -at "float3" -nc 3;
	addAttr -ci true -sn "sfPaintColorR" -ln "sfPaintColorR" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorG" -ln "sfPaintColorG" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorB" -ln "sfPaintColorB" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintOpacity" -ln "sfPaintOpacity" -at "double";
	addAttr -ci true -sn "sfPaintTravel" -ln "sfPaintTravel" -at "double";
	setAttr ".t" -type "double3" 126.3681375391606 -137.69598232347803 25.390381126241756 ;
	setAttr ".r" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".rp" -type "double3" 0 0 0.073118674440056708 ;
	setAttr ".sp" -type "double3" 0 0 0.073118674440056708 ;
	setAttr -k on ".sfPaintColor" -type "float3" 1 0.667 0 ;
	setAttr -k on ".sfPaintColor";
	setAttr -k on ".sfPaintOpacity" 1;
	setAttr -k on ".sfPaintTravel" 60;
createNode mesh -n "tx_2_orangeShape" -p "tx_2_orange";
	rename -uid "3A4795A8-B042-75AC-B903-8590DF22FA93";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:14]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[15:19]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.7083333432674408 0.49999997019767761 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 50 ".uvst[0].uvsp[0:49]" -type "float2" 0.375 0 0.38510099
		 0.014371129 0.38510102 0.23562887 0.61519605 0.23562887 0.625 0.25 0.375 0.25 0.61519605
		 0.014371129 0.625 0 0.29166669 0 0.29166669 0.25 0.375 0.91666663 0.625 0.91666663
		 0.625 0.99999994 0.375 0.99999994 0.70833337 0 0.70833337 0.25 0.625 0.33333334 0.375
		 0.33333334 0.20833334 0 0.20833334 0.25 0.375 0.83333331 0.625 0.83333331 0.79166669
		 0 0.79166669 0.25 0.625 0.41666669 0.375 0.41666669 0.125 0 0.125 0.25 0.375 0.75
		 0.625 0.75 0.875 0 0.875 0.25 0.625 0.5 0.375 0.5 0.625 0.27153829 0.375 0.27153829
		 0.375 0.5 0.625 0.5 0.64653826 5.17414e-18 0.64653826 0.25 0.875 0.25 0.875 0 0.375
		 0.75 0.625 0.75 0.35346171 0.25 0.35346171 1.1274109e-17 0.125 0 0.125 0.25 0.375
		 0.97846174 0.625 0.97846174;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcol" yes;
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 24 ".pt[0:23]" -type "float3"  7.7621551 -0.74639893 -0.46948725 
		5.6701221 -0.48628235 -0.46948725 -6.6329327 -16.387344 -0.46948725 -37.000687 11.088562 
		1.9167554 -39.17366 10.924011 1.9167554 -6.1391048 -18.713074 -0.46948725 -22.866409 
		26.989624 1.9167554 -23.203276 28.890701 1.9167554 6.1947575 -0.47973633 -0.32764608 
		-6.5717068 -16.979736 -0.32764608 -22.85515 28.575119 1.9167554 -37.52182 12.075104 
		1.9167554 6.1947575 -0.47973633 -0.22401451 -6.5717068 -16.979736 -0.22401451 -22.85515 
		28.575119 1.9167554 -37.52182 12.075104 1.9167554 4.8153095 0.41189575 -0.29392323 
		-7.5256023 -15.538101 -0.29392323 -21.932907 28.765991 -0.31726244 -36.110703 12.815979 
		-0.31726244 -7.6484127 -14.507767 -0.36971217 -23.479355 -0.65306091 -0.31726244 
		-11.930174 14.273666 -0.31726244 3.9007664 0.41897583 -0.36971217;
	setAttr -s 24 ".vt[0:23]"  -0.81153107 -8.98333359 4.099960327 0.48139954 -7.95052719 4.099960327
		 0.48139954 7.95053101 4.099960327 29.93354797 7.95053101 4.099960327 31.1884613 8.9833374 4.099960327
		 -0.81153107 8.9833374 4.099960327 29.93354797 -7.95052719 4.099960327 31.1884613 -8.98333359 4.099960327
		 0.18846893 -8.25 3.3907547 0.18846893 8.25 3.3907547 30.18847656 -8.25 3.3907547
		 30.18847656 8.25 3.3907547 0.18846893 -8.25 2.87259674 0.18846893 8.25 2.87259674
		 30.18847656 -8.25 2.87259674 30.18847656 8.25 2.87259674 1.35513306 -7.97499847 0
		 1.35513306 7.97499847 0 29.02180481 -7.97499847 0 29.02180481 7.97499847 0 1.87381744 7.46335602 0.3789444
		 17.70475769 7.46335602 0.3789444 17.70475769 -7.46335983 0.3789444 1.87381744 -7.46335983 0.3789444;
	setAttr -s 42 ".ed[0:41]"  0 1 1 1 2 0 2 3 0 3 4 1 4 5 0 5 0 0 3 6 1
		 6 1 0 0 7 0 7 4 0 8 0 0 5 9 0 9 8 1 8 10 1 10 7 0 10 11 1 11 4 0 11 9 1 12 8 0 9 13 0
		 13 12 1 12 14 1 14 10 0 14 15 1 15 11 0 15 13 1 16 12 0 13 17 0 17 16 0 16 18 0 18 14 0
		 18 19 0 19 15 0 19 17 0 2 20 0 20 21 0 21 3 0 21 22 1 22 6 0 20 23 0 23 22 0 1 23 0;
	setAttr -s 20 -ch 84 ".fc[0:19]" -type "polyFaces" 
		f 6 0 1 2 3 4 5
		mu 0 6 0 1 2 3 4 5
		f 6 6 7 -1 8 9 -4
		mu 0 6 3 6 1 0 7 4
		f 4 10 -6 11 12
		mu 0 4 8 0 5 9
		f 4 13 14 -9 -11
		mu 0 4 10 11 12 13
		f 4 -15 15 16 -10
		mu 0 4 7 14 15 4
		f 4 -5 -17 17 -12
		mu 0 4 5 4 16 17
		f 4 18 -13 19 20
		mu 0 4 18 8 9 19
		f 4 21 22 -14 -19
		mu 0 4 20 21 11 10
		f 4 -23 23 24 -16
		mu 0 4 14 22 23 15
		f 4 -18 -25 25 -20
		mu 0 4 17 16 24 25
		f 4 26 -21 27 28
		mu 0 4 26 18 19 27
		f 4 29 30 -22 -27
		mu 0 4 28 29 21 20
		f 4 -31 31 32 -24
		mu 0 4 22 30 31 23
		f 4 -26 -33 33 -28
		mu 0 4 25 24 32 33
		f 4 -34 -32 -30 -29
		mu 0 4 33 32 29 28
		f 4 -3 34 35 36
		mu 0 4 34 35 36 37
		f 4 -7 -37 37 38
		mu 0 4 38 39 40 41
		f 4 39 40 -38 -36
		mu 0 4 36 42 43 37
		f 4 -2 41 -40 -35
		mu 0 4 44 45 46 47
		f 4 -8 -39 -41 -42
		mu 0 4 48 49 43 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "tx_3_purple" -p "trays";
	rename -uid "C8B38516-A748-B435-780E-F1BCD33CD8D2";
	addAttr -ci true -uac -sn "sfPaintColor" -ln "sfPaintColor" -at "float3" -nc 3;
	addAttr -ci true -sn "sfPaintColorR" -ln "sfPaintColorR" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorG" -ln "sfPaintColorG" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorB" -ln "sfPaintColorB" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintOpacity" -ln "sfPaintOpacity" -at "double";
	addAttr -ci true -sn "sfPaintTravel" -ln "sfPaintTravel" -at "double";
	setAttr ".t" -type "double3" 126.36813753916054 -153.19598232347806 25.39038112624176 ;
	setAttr ".r" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".rp" -type "double3" 0 0 0.073118674440060261 ;
	setAttr ".sp" -type "double3" 0 0 0.073118674440060261 ;
	setAttr -k on ".sfPaintColor" -type "float3" 1 0 0 ;
	setAttr -k on ".sfPaintColor";
	setAttr -k on ".sfPaintOpacity" 1;
	setAttr -k on ".sfPaintTravel" 60;
createNode mesh -n "tx_3_purpleShape" -p "tx_3_purple";
	rename -uid "8E270A49-6D4A-27C6-B9FD-92A55EA12478";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:14]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[15:19]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.7083333432674408 0.49999997019767761 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 50 ".uvst[0].uvsp[0:49]" -type "float2" 0.375 0 0.38510099
		 0.014371129 0.38510102 0.23562887 0.61519605 0.23562887 0.625 0.25 0.375 0.25 0.61519605
		 0.014371129 0.625 0 0.29166669 0 0.29166669 0.25 0.375 0.91666663 0.625 0.91666663
		 0.625 0.99999994 0.375 0.99999994 0.70833337 0 0.70833337 0.25 0.625 0.33333334 0.375
		 0.33333334 0.20833334 0 0.20833334 0.25 0.375 0.83333331 0.625 0.83333331 0.79166669
		 0 0.79166669 0.25 0.625 0.41666669 0.375 0.41666669 0.125 0 0.125 0.25 0.375 0.75
		 0.625 0.75 0.875 0 0.875 0.25 0.625 0.5 0.375 0.5 0.625 0.27153829 0.375 0.27153829
		 0.375 0.5 0.625 0.5 0.64653826 5.17414e-18 0.64653826 0.25 0.875 0.25 0.875 0 0.375
		 0.75 0.625 0.75 0.35346171 0.25 0.35346171 1.1274109e-17 0.125 0 0.125 0.25 0.375
		 0.97846174 0.625 0.97846174;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcol" yes;
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 24 ".pt[0:23]" -type "float3"  7.7621551 -0.74639893 -0.46948725 
		5.6701221 -0.48628235 -0.46948725 -6.6329327 -16.387344 -0.46948725 -37.000687 11.088562 
		1.9167554 -39.17366 10.924011 1.9167554 -6.1391048 -18.713074 -0.46948725 -22.866409 
		26.989624 1.9167554 -23.203276 28.890701 1.9167554 6.1947575 -0.47973633 -0.32764608 
		-6.5717068 -16.979736 -0.32764608 -22.85515 28.575119 1.9167554 -37.52182 12.075104 
		1.9167554 6.1947575 -0.47973633 -0.22401451 -6.5717068 -16.979736 -0.22401451 -22.85515 
		28.575119 1.9167554 -37.52182 12.075104 1.9167554 4.8153095 0.41189575 -0.29392323 
		-7.5256023 -15.538101 -0.29392323 -21.932907 28.765991 -0.31726244 -36.110703 12.815979 
		-0.31726244 -7.6484127 -14.507767 -0.36971217 -23.479355 -0.65306091 -0.31726244 
		-11.930174 14.273666 -0.31726244 3.9007664 0.41897583 -0.36971217;
	setAttr -s 24 ".vt[0:23]"  -0.81153107 -8.98333359 4.099960327 0.48139954 -7.95052719 4.099960327
		 0.48139954 7.95053101 4.099960327 29.93354797 7.95053101 4.099960327 31.1884613 8.9833374 4.099960327
		 -0.81153107 8.9833374 4.099960327 29.93354797 -7.95052719 4.099960327 31.1884613 -8.98333359 4.099960327
		 0.18846893 -8.25 3.3907547 0.18846893 8.25 3.3907547 30.18847656 -8.25 3.3907547
		 30.18847656 8.25 3.3907547 0.18846893 -8.25 2.87259674 0.18846893 8.25 2.87259674
		 30.18847656 -8.25 2.87259674 30.18847656 8.25 2.87259674 1.35513306 -7.97499847 0
		 1.35513306 7.97499847 0 29.02180481 -7.97499847 0 29.02180481 7.97499847 0 1.87381744 7.46335602 0.3789444
		 17.70475769 7.46335602 0.3789444 17.70475769 -7.46335983 0.3789444 1.87381744 -7.46335983 0.3789444;
	setAttr -s 42 ".ed[0:41]"  0 1 1 1 2 0 2 3 0 3 4 1 4 5 0 5 0 0 3 6 1
		 6 1 0 0 7 0 7 4 0 8 0 0 5 9 0 9 8 1 8 10 1 10 7 0 10 11 1 11 4 0 11 9 1 12 8 0 9 13 0
		 13 12 1 12 14 1 14 10 0 14 15 1 15 11 0 15 13 1 16 12 0 13 17 0 17 16 0 16 18 0 18 14 0
		 18 19 0 19 15 0 19 17 0 2 20 0 20 21 0 21 3 0 21 22 1 22 6 0 20 23 0 23 22 0 1 23 0;
	setAttr -s 20 -ch 84 ".fc[0:19]" -type "polyFaces" 
		f 6 0 1 2 3 4 5
		mu 0 6 0 1 2 3 4 5
		f 6 6 7 -1 8 9 -4
		mu 0 6 3 6 1 0 7 4
		f 4 10 -6 11 12
		mu 0 4 8 0 5 9
		f 4 13 14 -9 -11
		mu 0 4 10 11 12 13
		f 4 -15 15 16 -10
		mu 0 4 7 14 15 4
		f 4 -5 -17 17 -12
		mu 0 4 5 4 16 17
		f 4 18 -13 19 20
		mu 0 4 18 8 9 19
		f 4 21 22 -14 -19
		mu 0 4 20 21 11 10
		f 4 -23 23 24 -16
		mu 0 4 14 22 23 15
		f 4 -18 -25 25 -20
		mu 0 4 17 16 24 25
		f 4 26 -21 27 28
		mu 0 4 26 18 19 27
		f 4 29 30 -22 -27
		mu 0 4 28 29 21 20
		f 4 -31 31 32 -24
		mu 0 4 22 30 31 23
		f 4 -26 -33 33 -28
		mu 0 4 25 24 32 33
		f 4 -34 -32 -30 -29
		mu 0 4 33 32 29 28
		f 4 -3 34 35 36
		mu 0 4 34 35 36 37
		f 4 -7 -37 37 38
		mu 0 4 38 39 40 41
		f 4 39 40 -38 -36
		mu 0 4 36 42 43 37
		f 4 -2 41 -40 -35
		mu 0 4 44 45 46 47
		f 4 -8 -39 -41 -42
		mu 0 4 48 49 43 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "tx_4_turqoise" -p "trays";
	rename -uid "3434E29F-2A4F-387F-B651-2CB8EAB87AE8";
	addAttr -ci true -uac -sn "sfPaintColor" -ln "sfPaintColor" -at "float3" -nc 3;
	addAttr -ci true -sn "sfPaintColorR" -ln "sfPaintColorR" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorG" -ln "sfPaintColorG" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorB" -ln "sfPaintColorB" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintOpacity" -ln "sfPaintOpacity" -at "double";
	addAttr -ci true -sn "sfPaintTravel" -ln "sfPaintTravel" -at "double";
	setAttr ".t" -type "double3" 126.36813753916066 -168.89598232347805 25.39038112624176 ;
	setAttr ".r" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".rp" -type "double3" 0 0 0.073118674440060261 ;
	setAttr ".sp" -type "double3" 0 0 0.073118674440060261 ;
	setAttr -k on ".sfPaintColor" -type "float3" 0 0.80400002 0.322 ;
	setAttr -k on ".sfPaintColor";
	setAttr -k on ".sfPaintOpacity" 1;
	setAttr -k on ".sfPaintTravel" 60;
createNode mesh -n "tx_4_turqoiseShape" -p "tx_4_turqoise";
	rename -uid "66BB58CD-3142-5BB3-EEFD-229E1BF8501A";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:14]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[15:19]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.7083333432674408 0.49999997019767761 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 50 ".uvst[0].uvsp[0:49]" -type "float2" 0.375 0 0.38510099
		 0.014371129 0.38510102 0.23562887 0.61519605 0.23562887 0.625 0.25 0.375 0.25 0.61519605
		 0.014371129 0.625 0 0.29166669 0 0.29166669 0.25 0.375 0.91666663 0.625 0.91666663
		 0.625 0.99999994 0.375 0.99999994 0.70833337 0 0.70833337 0.25 0.625 0.33333334 0.375
		 0.33333334 0.20833334 0 0.20833334 0.25 0.375 0.83333331 0.625 0.83333331 0.79166669
		 0 0.79166669 0.25 0.625 0.41666669 0.375 0.41666669 0.125 0 0.125 0.25 0.375 0.75
		 0.625 0.75 0.875 0 0.875 0.25 0.625 0.5 0.375 0.5 0.625 0.27153829 0.375 0.27153829
		 0.375 0.5 0.625 0.5 0.64653826 5.17414e-18 0.64653826 0.25 0.875 0.25 0.875 0 0.375
		 0.75 0.625 0.75 0.35346171 0.25 0.35346171 1.1274109e-17 0.125 0 0.125 0.25 0.375
		 0.97846174 0.625 0.97846174;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcol" yes;
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 24 ".pt[0:23]" -type "float3"  7.7621551 -0.74639893 -0.46948725 
		5.6701221 -0.48628235 -0.46948725 -6.6329327 -16.387344 -0.46948725 -37.000687 11.088562 
		1.9167554 -39.17366 10.924011 1.9167554 -6.1391048 -18.713074 -0.46948725 -22.866409 
		26.989624 1.9167554 -23.203276 28.890701 1.9167554 6.1947575 -0.47973633 -0.32764608 
		-6.5717068 -16.979736 -0.32764608 -22.85515 28.575119 1.9167554 -37.52182 12.075104 
		1.9167554 6.1947575 -0.47973633 -0.22401451 -6.5717068 -16.979736 -0.22401451 -22.85515 
		28.575119 1.9167554 -37.52182 12.075104 1.9167554 4.8153095 0.41189575 -0.29392323 
		-7.5256023 -15.538101 -0.29392323 -21.932907 28.765991 -0.31726244 -36.110703 12.815979 
		-0.31726244 -7.6484127 -14.507767 -0.36971217 -23.479355 -0.65306091 -0.31726244 
		-11.930174 14.273666 -0.31726244 3.9007664 0.41897583 -0.36971217;
	setAttr -s 24 ".vt[0:23]"  -0.81153107 -8.98333359 4.099960327 0.48139954 -7.95052719 4.099960327
		 0.48139954 7.95053101 4.099960327 29.93354797 7.95053101 4.099960327 31.1884613 8.9833374 4.099960327
		 -0.81153107 8.9833374 4.099960327 29.93354797 -7.95052719 4.099960327 31.1884613 -8.98333359 4.099960327
		 0.18846893 -8.25 3.3907547 0.18846893 8.25 3.3907547 30.18847656 -8.25 3.3907547
		 30.18847656 8.25 3.3907547 0.18846893 -8.25 2.87259674 0.18846893 8.25 2.87259674
		 30.18847656 -8.25 2.87259674 30.18847656 8.25 2.87259674 1.35513306 -7.97499847 0
		 1.35513306 7.97499847 0 29.02180481 -7.97499847 0 29.02180481 7.97499847 0 1.87381744 7.46335602 0.3789444
		 17.70475769 7.46335602 0.3789444 17.70475769 -7.46335983 0.3789444 1.87381744 -7.46335983 0.3789444;
	setAttr -s 42 ".ed[0:41]"  0 1 1 1 2 0 2 3 0 3 4 1 4 5 0 5 0 0 3 6 1
		 6 1 0 0 7 0 7 4 0 8 0 0 5 9 0 9 8 1 8 10 1 10 7 0 10 11 1 11 4 0 11 9 1 12 8 0 9 13 0
		 13 12 1 12 14 1 14 10 0 14 15 1 15 11 0 15 13 1 16 12 0 13 17 0 17 16 0 16 18 0 18 14 0
		 18 19 0 19 15 0 19 17 0 2 20 0 20 21 0 21 3 0 21 22 1 22 6 0 20 23 0 23 22 0 1 23 0;
	setAttr -s 20 -ch 84 ".fc[0:19]" -type "polyFaces" 
		f 6 0 1 2 3 4 5
		mu 0 6 0 1 2 3 4 5
		f 6 6 7 -1 8 9 -4
		mu 0 6 3 6 1 0 7 4
		f 4 10 -6 11 12
		mu 0 4 8 0 5 9
		f 4 13 14 -9 -11
		mu 0 4 10 11 12 13
		f 4 -15 15 16 -10
		mu 0 4 7 14 15 4
		f 4 -5 -17 17 -12
		mu 0 4 5 4 16 17
		f 4 18 -13 19 20
		mu 0 4 18 8 9 19
		f 4 21 22 -14 -19
		mu 0 4 20 21 11 10
		f 4 -23 23 24 -16
		mu 0 4 14 22 23 15
		f 4 -18 -25 25 -20
		mu 0 4 17 16 24 25
		f 4 26 -21 27 28
		mu 0 4 26 18 19 27
		f 4 29 30 -22 -27
		mu 0 4 28 29 21 20
		f 4 -31 31 32 -24
		mu 0 4 22 30 31 23
		f 4 -26 -33 33 -28
		mu 0 4 25 24 32 33
		f 4 -34 -32 -30 -29
		mu 0 4 33 32 29 28
		f 4 -3 34 35 36
		mu 0 4 34 35 36 37
		f 4 -7 -37 37 38
		mu 0 4 38 39 40 41
		f 4 39 40 -38 -36
		mu 0 4 36 42 43 37
		f 4 -2 41 -40 -35
		mu 0 4 44 45 46 47
		f 4 -8 -39 -41 -42
		mu 0 4 48 49 43 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "tx_5_green" -p "trays";
	rename -uid "A59872F5-6B4E-E0ED-AB46-F387D874135E";
	addAttr -ci true -uac -sn "sfPaintColor" -ln "sfPaintColor" -at "float3" -nc 3;
	addAttr -ci true -sn "sfPaintColorR" -ln "sfPaintColorR" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorG" -ln "sfPaintColorG" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorB" -ln "sfPaintColorB" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintOpacity" -ln "sfPaintOpacity" -at "double";
	addAttr -ci true -sn "sfPaintTravel" -ln "sfPaintTravel" -at "double";
	setAttr ".t" -type "double3" 126.36813753916066 -184.59598232347807 25.390381126241763 ;
	setAttr ".r" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".rp" -type "double3" 0 0 0.073118674440063813 ;
	setAttr ".sp" -type "double3" 0 0 0.073118674440063813 ;
	setAttr -k on ".sfPaintColor" -type "float3" 0 1 0 ;
	setAttr -k on ".sfPaintColor";
	setAttr -k on ".sfPaintOpacity" 1;
	setAttr -k on ".sfPaintTravel" 60;
createNode mesh -n "tx_5_greenShape" -p "tx_5_green";
	rename -uid "360E8F21-AE4C-80AA-756B-47A0322CAA09";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:14]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[15:19]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.7083333432674408 0.49999997019767761 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 50 ".uvst[0].uvsp[0:49]" -type "float2" 0.375 0 0.38510099
		 0.014371129 0.38510102 0.23562887 0.61519605 0.23562887 0.625 0.25 0.375 0.25 0.61519605
		 0.014371129 0.625 0 0.29166669 0 0.29166669 0.25 0.375 0.91666663 0.625 0.91666663
		 0.625 0.99999994 0.375 0.99999994 0.70833337 0 0.70833337 0.25 0.625 0.33333334 0.375
		 0.33333334 0.20833334 0 0.20833334 0.25 0.375 0.83333331 0.625 0.83333331 0.79166669
		 0 0.79166669 0.25 0.625 0.41666669 0.375 0.41666669 0.125 0 0.125 0.25 0.375 0.75
		 0.625 0.75 0.875 0 0.875 0.25 0.625 0.5 0.375 0.5 0.625 0.27153829 0.375 0.27153829
		 0.375 0.5 0.625 0.5 0.64653826 5.17414e-18 0.64653826 0.25 0.875 0.25 0.875 0 0.375
		 0.75 0.625 0.75 0.35346171 0.25 0.35346171 1.1274109e-17 0.125 0 0.125 0.25 0.375
		 0.97846174 0.625 0.97846174;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcol" yes;
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 24 ".pt[0:23]" -type "float3"  7.7621551 -0.74639893 -0.46948725 
		5.6701221 -0.48628235 -0.46948725 -6.6329327 -16.387344 -0.46948725 -37.000687 11.088562 
		1.9167554 -39.17366 10.924011 1.9167554 -6.1391048 -18.713074 -0.46948725 -22.866409 
		26.989624 1.9167554 -23.203276 28.890701 1.9167554 6.1947575 -0.47973633 -0.32764608 
		-6.5717068 -16.979736 -0.32764608 -22.85515 28.575119 1.9167554 -37.52182 12.075104 
		1.9167554 6.1947575 -0.47973633 -0.22401451 -6.5717068 -16.979736 -0.22401451 -22.85515 
		28.575119 1.9167554 -37.52182 12.075104 1.9167554 4.8153095 0.41189575 -0.29392323 
		-7.5256023 -15.538101 -0.29392323 -21.932907 28.765991 -0.31726244 -36.110703 12.815979 
		-0.31726244 -7.6484127 -14.507767 -0.36971217 -23.479355 -0.65306091 -0.31726244 
		-11.930174 14.273666 -0.31726244 3.9007664 0.41897583 -0.36971217;
	setAttr -s 24 ".vt[0:23]"  -0.81153107 -8.98333359 4.099960327 0.48139954 -7.95052719 4.099960327
		 0.48139954 7.95053101 4.099960327 29.93354797 7.95053101 4.099960327 31.1884613 8.9833374 4.099960327
		 -0.81153107 8.9833374 4.099960327 29.93354797 -7.95052719 4.099960327 31.1884613 -8.98333359 4.099960327
		 0.18846893 -8.25 3.3907547 0.18846893 8.25 3.3907547 30.18847656 -8.25 3.3907547
		 30.18847656 8.25 3.3907547 0.18846893 -8.25 2.87259674 0.18846893 8.25 2.87259674
		 30.18847656 -8.25 2.87259674 30.18847656 8.25 2.87259674 1.35513306 -7.97499847 0
		 1.35513306 7.97499847 0 29.02180481 -7.97499847 0 29.02180481 7.97499847 0 1.87381744 7.46335602 0.3789444
		 17.70475769 7.46335602 0.3789444 17.70475769 -7.46335983 0.3789444 1.87381744 -7.46335983 0.3789444;
	setAttr -s 42 ".ed[0:41]"  0 1 1 1 2 0 2 3 0 3 4 1 4 5 0 5 0 0 3 6 1
		 6 1 0 0 7 0 7 4 0 8 0 0 5 9 0 9 8 1 8 10 1 10 7 0 10 11 1 11 4 0 11 9 1 12 8 0 9 13 0
		 13 12 1 12 14 1 14 10 0 14 15 1 15 11 0 15 13 1 16 12 0 13 17 0 17 16 0 16 18 0 18 14 0
		 18 19 0 19 15 0 19 17 0 2 20 0 20 21 0 21 3 0 21 22 1 22 6 0 20 23 0 23 22 0 1 23 0;
	setAttr -s 20 -ch 84 ".fc[0:19]" -type "polyFaces" 
		f 6 0 1 2 3 4 5
		mu 0 6 0 1 2 3 4 5
		f 6 6 7 -1 8 9 -4
		mu 0 6 3 6 1 0 7 4
		f 4 10 -6 11 12
		mu 0 4 8 0 5 9
		f 4 13 14 -9 -11
		mu 0 4 10 11 12 13
		f 4 -15 15 16 -10
		mu 0 4 7 14 15 4
		f 4 -5 -17 17 -12
		mu 0 4 5 4 16 17
		f 4 18 -13 19 20
		mu 0 4 18 8 9 19
		f 4 21 22 -14 -19
		mu 0 4 20 21 11 10
		f 4 -23 23 24 -16
		mu 0 4 14 22 23 15
		f 4 -18 -25 25 -20
		mu 0 4 17 16 24 25
		f 4 26 -21 27 28
		mu 0 4 26 18 19 27
		f 4 29 30 -22 -27
		mu 0 4 28 29 21 20
		f 4 -31 31 32 -24
		mu 0 4 22 30 31 23
		f 4 -26 -33 33 -28
		mu 0 4 25 24 32 33
		f 4 -34 -32 -30 -29
		mu 0 4 33 32 29 28
		f 4 -3 34 35 36
		mu 0 4 34 35 36 37
		f 4 -7 -37 37 38
		mu 0 4 38 39 40 41
		f 4 39 40 -38 -36
		mu 0 4 36 42 43 37
		f 4 -2 41 -40 -35
		mu 0 4 44 45 46 47
		f 4 -8 -39 -41 -42
		mu 0 4 48 49 43 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "tx_6_canary" -p "trays";
	rename -uid "7A1F7299-0847-3E56-9914-6A953AB69C92";
	addAttr -ci true -uac -sn "sfPaintColor" -ln "sfPaintColor" -at "float3" -nc 3;
	addAttr -ci true -sn "sfPaintColorR" -ln "sfPaintColorR" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorG" -ln "sfPaintColorG" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorB" -ln "sfPaintColorB" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintOpacity" -ln "sfPaintOpacity" -at "double";
	addAttr -ci true -sn "sfPaintTravel" -ln "sfPaintTravel" -at "double";
	setAttr ".t" -type "double3" 126.36813753916061 -200.39598232347811 25.390381126241756 ;
	setAttr ".r" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".rp" -type "double3" 0 0 0.073118674440056708 ;
	setAttr ".sp" -type "double3" 0 0 0.073118674440056708 ;
	setAttr -k on ".sfPaintColor" -type "float3" 1 0.73699999 0 ;
	setAttr -k on ".sfPaintColor";
	setAttr -k on ".sfPaintOpacity" 1;
	setAttr -k on ".sfPaintTravel" 60;
createNode mesh -n "tx_6_canaryShape" -p "tx_6_canary";
	rename -uid "594A8940-874D-03C5-D245-D68EF6A908BC";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:14]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[15:19]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.7083333432674408 0.49999997019767761 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 50 ".uvst[0].uvsp[0:49]" -type "float2" 0.375 0 0.38510099
		 0.014371129 0.38510102 0.23562887 0.61519605 0.23562887 0.625 0.25 0.375 0.25 0.61519605
		 0.014371129 0.625 0 0.29166669 0 0.29166669 0.25 0.375 0.91666663 0.625 0.91666663
		 0.625 0.99999994 0.375 0.99999994 0.70833337 0 0.70833337 0.25 0.625 0.33333334 0.375
		 0.33333334 0.20833334 0 0.20833334 0.25 0.375 0.83333331 0.625 0.83333331 0.79166669
		 0 0.79166669 0.25 0.625 0.41666669 0.375 0.41666669 0.125 0 0.125 0.25 0.375 0.75
		 0.625 0.75 0.875 0 0.875 0.25 0.625 0.5 0.375 0.5 0.625 0.27153829 0.375 0.27153829
		 0.375 0.5 0.625 0.5 0.64653826 5.17414e-18 0.64653826 0.25 0.875 0.25 0.875 0 0.375
		 0.75 0.625 0.75 0.35346171 0.25 0.35346171 1.1274109e-17 0.125 0 0.125 0.25 0.375
		 0.97846174 0.625 0.97846174;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcol" yes;
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 24 ".pt[0:23]" -type "float3"  7.7621551 -0.74639893 -0.46948725 
		5.6701221 -0.48628235 -0.46948725 -6.6329327 -16.387344 -0.46948725 -37.000687 11.088562 
		1.9167554 -39.17366 10.924011 1.9167554 -6.1391048 -18.713074 -0.46948725 -22.866409 
		26.989624 1.9167554 -23.203276 28.890701 1.9167554 6.1947575 -0.47973633 -0.32764608 
		-6.5717068 -16.979736 -0.32764608 -22.85515 28.575119 1.9167554 -37.52182 12.075104 
		1.9167554 6.1947575 -0.47973633 -0.22401451 -6.5717068 -16.979736 -0.22401451 -22.85515 
		28.575119 1.9167554 -37.52182 12.075104 1.9167554 4.8153095 0.41189575 -0.29392323 
		-7.5256023 -15.538101 -0.29392323 -21.932907 28.765991 -0.31726244 -36.110703 12.815979 
		-0.31726244 -7.6484127 -14.507767 -0.36971217 -23.479355 -0.65306091 -0.31726244 
		-11.930174 14.273666 -0.31726244 3.9007664 0.41897583 -0.36971217;
	setAttr -s 24 ".vt[0:23]"  -0.81153107 -8.98333359 4.099960327 0.48139954 -7.95052719 4.099960327
		 0.48139954 7.95053101 4.099960327 29.93354797 7.95053101 4.099960327 31.1884613 8.9833374 4.099960327
		 -0.81153107 8.9833374 4.099960327 29.93354797 -7.95052719 4.099960327 31.1884613 -8.98333359 4.099960327
		 0.18846893 -8.25 3.3907547 0.18846893 8.25 3.3907547 30.18847656 -8.25 3.3907547
		 30.18847656 8.25 3.3907547 0.18846893 -8.25 2.87259674 0.18846893 8.25 2.87259674
		 30.18847656 -8.25 2.87259674 30.18847656 8.25 2.87259674 1.35513306 -7.97499847 0
		 1.35513306 7.97499847 0 29.02180481 -7.97499847 0 29.02180481 7.97499847 0 1.87381744 7.46335602 0.3789444
		 17.70475769 7.46335602 0.3789444 17.70475769 -7.46335983 0.3789444 1.87381744 -7.46335983 0.3789444;
	setAttr -s 42 ".ed[0:41]"  0 1 1 1 2 0 2 3 0 3 4 1 4 5 0 5 0 0 3 6 1
		 6 1 0 0 7 0 7 4 0 8 0 0 5 9 0 9 8 1 8 10 1 10 7 0 10 11 1 11 4 0 11 9 1 12 8 0 9 13 0
		 13 12 1 12 14 1 14 10 0 14 15 1 15 11 0 15 13 1 16 12 0 13 17 0 17 16 0 16 18 0 18 14 0
		 18 19 0 19 15 0 19 17 0 2 20 0 20 21 0 21 3 0 21 22 1 22 6 0 20 23 0 23 22 0 1 23 0;
	setAttr -s 20 -ch 84 ".fc[0:19]" -type "polyFaces" 
		f 6 0 1 2 3 4 5
		mu 0 6 0 1 2 3 4 5
		f 6 6 7 -1 8 9 -4
		mu 0 6 3 6 1 0 7 4
		f 4 10 -6 11 12
		mu 0 4 8 0 5 9
		f 4 13 14 -9 -11
		mu 0 4 10 11 12 13
		f 4 -15 15 16 -10
		mu 0 4 7 14 15 4
		f 4 -5 -17 17 -12
		mu 0 4 5 4 16 17
		f 4 18 -13 19 20
		mu 0 4 18 8 9 19
		f 4 21 22 -14 -19
		mu 0 4 20 21 11 10
		f 4 -23 23 24 -16
		mu 0 4 14 22 23 15
		f 4 -18 -25 25 -20
		mu 0 4 17 16 24 25
		f 4 26 -21 27 28
		mu 0 4 26 18 19 27
		f 4 29 30 -22 -27
		mu 0 4 28 29 21 20
		f 4 -31 31 32 -24
		mu 0 4 22 30 31 23
		f 4 -26 -33 33 -28
		mu 0 4 25 24 32 33
		f 4 -34 -32 -30 -29
		mu 0 4 33 32 29 28
		f 4 -3 34 35 36
		mu 0 4 34 35 36 37
		f 4 -7 -37 37 38
		mu 0 4 38 39 40 41
		f 4 39 40 -38 -36
		mu 0 4 36 42 43 37
		f 4 -2 41 -40 -35
		mu 0 4 44 45 46 47
		f 4 -8 -39 -41 -42
		mu 0 4 48 49 43 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "tx_7_black" -p "trays";
	rename -uid "45CE011D-5049-711A-6634-1281E01D2D9C";
	addAttr -ci true -uac -sn "sfPaintColor" -ln "sfPaintColor" -at "float3" -nc 3;
	addAttr -ci true -sn "sfPaintColorR" -ln "sfPaintColorR" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorG" -ln "sfPaintColorG" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintColorB" -ln "sfPaintColorB" -at "float" -p "sfPaintColor";
	addAttr -ci true -sn "sfPaintOpacity" -ln "sfPaintOpacity" -at "double";
	addAttr -ci true -sn "sfPaintTravel" -ln "sfPaintTravel" -at "double";
	setAttr ".t" -type "double3" 126.36813753916073 -216.33949414403878 25.390381126241763 ;
	setAttr ".r" -type "double3" 0 0 -89.999999999999986 ;
	setAttr ".rp" -type "double3" 0 0 0.073118674440063813 ;
	setAttr ".sp" -type "double3" 0 0 0.073118674440063813 ;
	setAttr -k on ".sfPaintColor";
	setAttr -k on ".sfPaintOpacity" 1;
	setAttr -k on ".sfPaintTravel" 60;
createNode mesh -n "tx_7_blackShape" -p "tx_7_black";
	rename -uid "31D87E96-3C4B-FF3B-7867-10A6C7C4A8FE";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:14]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[15:19]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.7083333432674408 0.49999997019767761 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 50 ".uvst[0].uvsp[0:49]" -type "float2" 0.375 0 0.38510099
		 0.014371129 0.38510102 0.23562887 0.61519605 0.23562887 0.625 0.25 0.375 0.25 0.61519605
		 0.014371129 0.625 0 0.29166669 0 0.29166669 0.25 0.375 0.91666663 0.625 0.91666663
		 0.625 0.99999994 0.375 0.99999994 0.70833337 0 0.70833337 0.25 0.625 0.33333334 0.375
		 0.33333334 0.20833334 0 0.20833334 0.25 0.375 0.83333331 0.625 0.83333331 0.79166669
		 0 0.79166669 0.25 0.625 0.41666669 0.375 0.41666669 0.125 0 0.125 0.25 0.375 0.75
		 0.625 0.75 0.875 0 0.875 0.25 0.625 0.5 0.375 0.5 0.625 0.27153829 0.375 0.27153829
		 0.375 0.5 0.625 0.5 0.64653826 5.17414e-18 0.64653826 0.25 0.875 0.25 0.875 0 0.375
		 0.75 0.625 0.75 0.35346171 0.25 0.35346171 1.1274109e-17 0.125 0 0.125 0.25 0.375
		 0.97846174 0.625 0.97846174;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcol" yes;
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 24 ".pt[0:23]" -type "float3"  7.7621551 -0.74639893 -0.46948725 
		5.6701221 -0.48628235 -0.46948725 -6.6329327 -16.387344 -0.46948725 -37.000687 11.088562 
		1.9167554 -39.17366 10.924011 1.9167554 -6.1391048 -18.713074 -0.46948725 -22.866409 
		26.989624 1.9167554 -23.203276 28.890701 1.9167554 6.1947575 -0.47973633 -0.32764608 
		-6.5717068 -16.979736 -0.32764608 -22.85515 28.575119 1.9167554 -37.52182 12.075104 
		1.9167554 6.1947575 -0.47973633 -0.22401451 -6.5717068 -16.979736 -0.22401451 -22.85515 
		28.575119 1.9167554 -37.52182 12.075104 1.9167554 4.8153095 0.41189575 -0.29392323 
		-7.5256023 -15.538101 -0.29392323 -21.932907 28.765991 -0.31726244 -36.110703 12.815979 
		-0.31726244 -7.6484127 -14.507767 -0.36971217 -23.479355 -0.65306091 -0.31726244 
		-11.930174 14.273666 -0.31726244 3.9007664 0.41897583 -0.36971217;
	setAttr -s 24 ".vt[0:23]"  -0.81153107 -8.98333359 4.099960327 0.48139954 -7.95052719 4.099960327
		 0.48139954 7.95053101 4.099960327 29.93354797 7.95053101 4.099960327 31.1884613 8.9833374 4.099960327
		 -0.81153107 8.9833374 4.099960327 29.93354797 -7.95052719 4.099960327 31.1884613 -8.98333359 4.099960327
		 0.18846893 -8.25 3.3907547 0.18846893 8.25 3.3907547 30.18847656 -8.25 3.3907547
		 30.18847656 8.25 3.3907547 0.18846893 -8.25 2.87259674 0.18846893 8.25 2.87259674
		 30.18847656 -8.25 2.87259674 30.18847656 8.25 2.87259674 1.35513306 -7.97499847 0
		 1.35513306 7.97499847 0 29.02180481 -7.97499847 0 29.02180481 7.97499847 0 1.87381744 7.46335602 0.3789444
		 17.70475769 7.46335602 0.3789444 17.70475769 -7.46335983 0.3789444 1.87381744 -7.46335983 0.3789444;
	setAttr -s 42 ".ed[0:41]"  0 1 1 1 2 0 2 3 0 3 4 1 4 5 0 5 0 0 3 6 1
		 6 1 0 0 7 0 7 4 0 8 0 0 5 9 0 9 8 1 8 10 1 10 7 0 10 11 1 11 4 0 11 9 1 12 8 0 9 13 0
		 13 12 1 12 14 1 14 10 0 14 15 1 15 11 0 15 13 1 16 12 0 13 17 0 17 16 0 16 18 0 18 14 0
		 18 19 0 19 15 0 19 17 0 2 20 0 20 21 0 21 3 0 21 22 1 22 6 0 20 23 0 23 22 0 1 23 0;
	setAttr -s 20 -ch 84 ".fc[0:19]" -type "polyFaces" 
		f 6 0 1 2 3 4 5
		mu 0 6 0 1 2 3 4 5
		f 6 6 7 -1 8 9 -4
		mu 0 6 3 6 1 0 7 4
		f 4 10 -6 11 12
		mu 0 4 8 0 5 9
		f 4 13 14 -9 -11
		mu 0 4 10 11 12 13
		f 4 -15 15 16 -10
		mu 0 4 7 14 15 4
		f 4 -5 -17 17 -12
		mu 0 4 5 4 16 17
		f 4 18 -13 19 20
		mu 0 4 18 8 9 19
		f 4 21 22 -14 -19
		mu 0 4 20 21 11 10
		f 4 -23 23 24 -16
		mu 0 4 14 22 23 15
		f 4 -18 -25 25 -20
		mu 0 4 17 16 24 25
		f 4 26 -21 27 28
		mu 0 4 26 18 19 27
		f 4 29 30 -22 -27
		mu 0 4 28 29 21 20
		f 4 -31 31 32 -24
		mu 0 4 22 30 31 23
		f 4 -26 -33 33 -28
		mu 0 4 25 24 32 33
		f 4 -34 -32 -30 -29
		mu 0 4 33 32 29 28
		f 4 -3 34 35 36
		mu 0 4 34 35 36 37
		f 4 -7 -37 37 38
		mu 0 4 38 39 40 41
		f 4 39 40 -38 -36
		mu 0 4 36 42 43 37
		f 4 -2 41 -40 -35
		mu 0 4 44 45 46 47
		f 4 -8 -39 -41 -42
		mu 0 4 48 49 43 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "ledge" -p "|dipPaintingGroup|jpos";
	rename -uid "283CB06B-6E4E-1250-59B7-0E8A38767D65";
	setAttr ".t" -type "double3" 77.638305940040468 -118.60355514085502 -28.772385349686743 ;
	setAttr ".r" -type "double3" -0.16460201575095729 -0.036122794406142356 77.722736093539396 ;
	setAttr ".s" -type "double3" 1.0000000000000004 1.0000000000000002 1 ;
createNode nurbsCurve -n "ledgeShape" -p "ledge";
	rename -uid "3DBAC608-8544-8B75-C04B-EA906E6951AB";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		1 1 0 no 3
		2 0 1
		2
		99.400000000000006 101 29
		128.30000000000001 -31.800000000000001 28.600000000000001
		;
createNode transform -n "rack_LZ" -p "dipPaintingGroup";
	rename -uid "B54014BE-BE48-7B79-981C-9D9D60AD3391";
	setAttr ".t" -type "double3" -66.581274645610534 -113.54441179245201 3.691255779119718 ;
createNode locator -n "rack_LZShape" -p "rack_LZ";
	rename -uid "691A7E62-2A41-5839-1CC9-F2AFF50EF749";
	setAttr -k off ".v";
createNode transform -n "rack_RZ" -p "dipPaintingGroup";
	rename -uid "78BAA35E-214E-C71C-0E89-108AA0000488";
	setAttr ".t" -type "double3" 69.32736614642667 -113.53654760486539 3.9177424576466717 ;
createNode locator -n "rack_RZShape" -p "rack_RZ";
	rename -uid "332A55D0-F84F-8B73-6F92-C6A47DAB70FC";
	setAttr -k off ".v";
createNode transform -n "rack_P7Z" -p "dipPaintingGroup";
	rename -uid "6855403D-3D43-41DB-4F30-55991E87DF33";
	setAttr ".t" -type "double3" 56.604882821818471 -104.82913832107768 0.058816278047853388 ;
	setAttr ".r" -type "double3" -0.25796560850873901 -0.056122777104057733 77.726126342394537 ;
	setAttr ".s" -type "double3" 6.8233096967761036 6.8233096967761036 6.8233096967761036 ;
createNode locator -n "rack_P7ZShape" -p "rack_P7Z";
	rename -uid "FDE8E407-594F-FC70-7976-2DA208650912";
	setAttr -k off ".v";
createNode transform -n "rack_L" -p "dipPaintingGroup";
	rename -uid "C413CF03-4744-5777-7447-84B09FDF6A53";
	setAttr ".t" -type "double3" 99.4 101 29 ;
createNode locator -n "rack_LShape" -p "rack_L";
	rename -uid "1D107C58-FA4D-7327-9A12-3A8256109C54";
	setAttr -k off ".v";
createNode transform -n "rack_R" -p "dipPaintingGroup";
	rename -uid "F76866F7-EF40-E568-536F-2C93F95BA91A";
	setAttr ".t" -type "double3" 128.3 -31.8 28.6 ;
createNode locator -n "rack_RShape" -p "rack_R";
	rename -uid "1228F95E-B746-9FC3-64E4-8F876A4887BB";
	setAttr -k off ".v";
createNode transform -n "rack_P7" -p "dipPaintingGroup";
	rename -uid "AEB902E6-1441-83DD-7F32-50A0C5D27891";
	setAttr ".t" -type "double3" 134.1 -17.5 24.8 ;
	setAttr ".s" -type "double3" 6.8233096967761036 6.8233096967761036 6.8233096967761036 ;
createNode locator -n "rack_PShape7" -p "rack_P7";
	rename -uid "9D74A98E-F843-04AE-156F-71B31D7CD982";
	setAttr -k off ".v";
createNode ikHandle -n "ikHandle" -p "dipPaintingGroup";
	rename -uid "A095B84B-8545-3BB1-FC41-29B67F24F26F";
	setAttr ".twi" 113.783;
	setAttr ".roc" yes;
createNode poleVectorConstraint -n "ikPvcon" -p "|dipPaintingGroup|ikHandle";
	rename -uid "EA2AD0B8-7649-BE3E-314F-3C8562F83520";
	addAttr -dcb 0 -ci true -k true -sn "w0" -ln "rack_P7ZW0" -dv 1 -min 0 -at "double";
	addAttr -dcb 0 -ci true -k true -sn "w1" -ln "rack_P7W1" -dv 1 -min 0 -at "double";
	setAttr -k on ".nds";
	setAttr -k off ".v";
	setAttr -k off ".tx";
	setAttr -k off ".ty";
	setAttr -k off ".tz";
	setAttr -k off ".rx";
	setAttr -k off ".ry";
	setAttr -k off ".rz";
	setAttr -k off ".sx";
	setAttr -k off ".sy";
	setAttr -k off ".sz";
	setAttr ".erp" yes;
	setAttr -s 2 ".tg";
	setAttr ".rst" -type "double3" 161.93371605651976 52.379842631913171 8.738152359904209 ;
	setAttr -k on ".w0";
	setAttr -k on ".w1";
createNode pointConstraint -n "ikPcon" -p "|dipPaintingGroup|ikHandle";
	rename -uid "CEA8BD9D-774C-640F-A777-B8A11CBD6F52";
	addAttr -dcb 0 -ci true -k true -sn "w0" -ln "rack_RZW0" -dv 1 -min 0 -at "double";
	addAttr -dcb 0 -ci true -k true -sn "w1" -ln "rack_RW1" -dv 1 -min 0 -at "double";
	setAttr -k on ".nds";
	setAttr -k off ".v";
	setAttr -k off ".tx";
	setAttr -k off ".ty";
	setAttr -k off ".tz";
	setAttr -k off ".rx";
	setAttr -k off ".ry";
	setAttr -k off ".rz";
	setAttr -k off ".sx";
	setAttr -k off ".sy";
	setAttr -k off ".sz";
	setAttr ".erp" yes;
	setAttr -s 2 ".tg";
	setAttr ".rst" -type "double3" 98.81368307321334 -72.668273802432694 16.258871228823338 ;
	setAttr -k on ".w0";
	setAttr -k on ".w1";
createNode transform -n "dipProjection" -p "dipPaintingGroup";
	rename -uid "10EE5D4E-A04E-E316-5752-C3A569B29B48";
	setAttr ".t" -type "double3" 0.48335579759006109 -97.637871113190229 0 ;
	setAttr ".s" -type "double3" 65.789629629013575 17.622222228650021 1 ;
createNode sketchPlane -n "dipProjectionShape" -p "dipProjection";
	rename -uid "5C5FDE35-D849-6B1A-FB56-7ABB71C2ECC9";
	setAttr -k off ".v";
createNode transform -n "curves" -p "dipPaintingGroup";
	rename -uid "69EBE95D-744D-D281-2C20-9DB14A633E37";
createNode transform -n "dcx_p04_b06" -p "|dipPaintingGroup|curves";
	rename -uid "D2B80513-8B49-0D61-2849-9CAAE397EAC9";
	setAttr ".t" -type "double3" 9.4301871172737606 -103.77461076266351 0.073118674440067366 ;
	setAttr ".s" -type "double3" 1.0000000000000002 1.0000000000000007 1 ;
createNode transform -n "dcx_p04_b06_c00" -p "dcx_p04_b06";
	rename -uid "9147BDCF-434D-CCCA-0EF6-689C1BCE429C";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "dcx_p04_b06_c0Shape0" -p "dcx_p04_b06_c00";
	rename -uid "1C64167A-6344-F962-D8A7-1DB5F9666A1F";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p04_b06_c01" -p "dcx_p04_b06";
	rename -uid "84257EB1-2445-F3D9-42AD-4291FE7C3F84";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p04_b06_c0Shape1" -p "dcx_p04_b06_c01";
	rename -uid "021FE65E-7A44-76E4-C581-C0B339E840C6";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p04_b06_c02" -p "dcx_p04_b06";
	rename -uid "896F61D6-4E4C-6AA9-8B9B-38A3FE4AF85B";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p04_b06_c0Shape2" -p "dcx_p04_b06_c02";
	rename -uid "20037A75-1C4E-6271-33F8-BAA5A13EE634";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p04_b06_c03" -p "dcx_p04_b06";
	rename -uid "88E62D5C-C440-8EB7-C5CF-F0B2F821FCA7";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p04_b06_c0Shape3" -p "dcx_p04_b06_c03";
	rename -uid "68926A74-8C4B-06C9-B46A-A3850F3DE5A8";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p02_b02" -p "|dipPaintingGroup|curves";
	rename -uid "E708954F-3244-07FD-DFB8-08A9E09B68B8";
	setAttr ".t" -type "double3" -21.769812882726256 -103.77461076266357 0.073118674440063813 ;
	setAttr ".s" -type "double3" 1.0000000000000002 1.0000000000000007 1 ;
createNode transform -n "dcx_p02_b02_c00" -p "dcx_p02_b02";
	rename -uid "809AFBCD-6A4A-4BA1-9EBF-4799E5CE808C";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "dcx_p02_b02_c0Shape0" -p "dcx_p02_b02_c00";
	rename -uid "4B859CC9-7244-28DA-ACA1-839ECCC56CC5";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p02_b02_c01" -p "dcx_p02_b02";
	rename -uid "3101FC92-364F-BF39-17F9-D7BDA1150EB0";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p02_b02_c0Shape1" -p "dcx_p02_b02_c01";
	rename -uid "92509057-9F47-25CF-5544-E19674183D29";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p02_b02_c02" -p "dcx_p02_b02";
	rename -uid "4DD539A3-164C-7521-A4CB-D393162A8F08";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p02_b02_c0Shape2" -p "dcx_p02_b02_c02";
	rename -uid "BCADC265-8042-C330-FA74-ABA1BBE07EE4";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p02_b02_c03" -p "dcx_p02_b02";
	rename -uid "378A0AE9-0341-6B82-A4E4-68905B4BB69A";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p02_b02_c0Shape3" -p "dcx_p02_b02_c03";
	rename -uid "0745647B-9C4F-056E-B3B7-C4B5A90C5533";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p05_b07" -p "|dipPaintingGroup|curves";
	rename -uid "9F8A26B8-B142-6537-AB2E-D1BC5E4B7679";
	setAttr ".t" -type "double3" 25.130187117273778 -103.77461076266351 0.073118674440070919 ;
	setAttr ".s" -type "double3" 1.0000000000000002 1.0000000000000007 1 ;
createNode transform -n "dcx_p05_b07_c00" -p "dcx_p05_b07";
	rename -uid "810012F6-3949-EAC9-13F3-2DB7416815EF";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "dcx_p05_b07_c0Shape0" -p "dcx_p05_b07_c00";
	rename -uid "1950B948-984E-52E3-A7B8-2A8A312E1E4C";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p05_b07_c01" -p "dcx_p05_b07";
	rename -uid "EE740323-0044-6825-7CFA-239E95FE4B88";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p05_b07_c0Shape1" -p "dcx_p05_b07_c01";
	rename -uid "9DA6049A-2440-F36C-DCC2-DFB5126AE893";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p05_b07_c02" -p "dcx_p05_b07";
	rename -uid "5688A815-EE46-4F7B-52ED-AA84D335FBBB";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p05_b07_c0Shape2" -p "dcx_p05_b07_c02";
	rename -uid "FF67E6B0-4348-E488-7DC4-8AAF6880E235";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p05_b07_c03" -p "dcx_p05_b07";
	rename -uid "F96D3977-844C-2712-462E-838D57EFA1C4";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p05_b07_c0Shape3" -p "dcx_p05_b07_c03";
	rename -uid "C1A9D6F1-764C-CFE8-E706-5893BA32F7C3";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p07_b05" -p "|dipPaintingGroup|curves";
	rename -uid "1DBD355B-7D43-7ED4-8DF3-249C37D3CF9C";
	setAttr ".t" -type "double3" 56.87369893783449 -103.77461076266344 0.073118674440070919 ;
	setAttr ".s" -type "double3" 1.0000000000000002 1.0000000000000007 1 ;
createNode transform -n "dcx_p07_b05_c00" -p "dcx_p07_b05";
	rename -uid "2BE5A883-CC42-5C7E-FAC5-DE9CF0F50948";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "dcx_p07_b05_c0Shape0" -p "dcx_p07_b05_c00";
	rename -uid "DCC4ADCD-6547-E05E-E0CF-35B1D31711B3";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p07_b05_c01" -p "dcx_p07_b05";
	rename -uid "4E2DBB6A-AD46-FC1E-918D-168E2DC38896";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p07_b05_c0Shape1" -p "dcx_p07_b05_c01";
	rename -uid "AE594B4D-E34D-6CB6-F4DF-308BA4DF32DE";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p07_b05_c02" -p "dcx_p07_b05";
	rename -uid "9A06DE9C-E24E-61FC-F3DC-F2B96CF95FCC";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p07_b05_c0Shape2" -p "dcx_p07_b05_c02";
	rename -uid "58B476CF-B44D-5EAE-FE51-12B320F34C73";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p07_b05_c03" -p "dcx_p07_b05";
	rename -uid "66FD5C25-F041-E8F4-5E41-B9B98C17D845";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p07_b05_c0Shape3" -p "dcx_p07_b05_c03";
	rename -uid "195666F7-2F4D-C280-3ACE-C7B01F464D7E";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p06_b08" -p "|dipPaintingGroup|curves";
	rename -uid "B1A677E4-4F4C-4547-9989-138263E2AA0C";
	setAttr ".t" -type "double3" 40.930187117273817 -103.77461076266356 0.073118674440063813 ;
	setAttr ".s" -type "double3" 1.0000000000000002 1.0000000000000007 1 ;
createNode transform -n "dcx_p06_b08_c00" -p "dcx_p06_b08";
	rename -uid "C5950C9A-5B4A-35DD-3687-EFBC1F29FCD3";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "dcx_p06_b08_c0Shape0" -p "dcx_p06_b08_c00";
	rename -uid "36B1A1CE-F044-9EA7-AC87-C4A98C03409B";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p06_b08_c01" -p "dcx_p06_b08";
	rename -uid "A581D166-B44A-F00F-136B-F9966EFBE504";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p06_b08_c0Shape1" -p "dcx_p06_b08_c01";
	rename -uid "017B7250-734D-7DB0-6D1E-D8A0D90E2706";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p06_b08_c02" -p "dcx_p06_b08";
	rename -uid "2C154295-954C-FB65-8B64-0482C04F5694";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p06_b08_c0Shape2" -p "dcx_p06_b08_c02";
	rename -uid "274F38C6-9645-E67A-8CD4-64B49BBDDC3F";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p06_b08_c03" -p "dcx_p06_b08";
	rename -uid "021252CC-5642-5223-2F30-CFA7CFC152D4";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p06_b08_c0Shape3" -p "dcx_p06_b08_c03";
	rename -uid "2D901379-0B4E-BC6F-3A0B-63903F7F3A1B";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p01_b04" -p "|dipPaintingGroup|curves";
	rename -uid "7596BBF0-FD4B-FD9C-C994-EBAFCF2BABAA";
	setAttr ".t" -type "double3" -38.16981288272622 -103.77461076266347 0.073118674440070919 ;
	setAttr ".s" -type "double3" 1.0000000000000002 1.0000000000000007 1 ;
createNode transform -n "dcx_p01_b04_c00" -p "dcx_p01_b04";
	rename -uid "3786D023-8D4A-6F30-60B0-F0896E3A43B5";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "dcx_p01_b04_c0Shape0" -p "dcx_p01_b04_c00";
	rename -uid "BDC2CB43-EF4F-B734-AE2D-43B0A627A4F6";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p01_b04_c01" -p "dcx_p01_b04";
	rename -uid "68B98C29-9E43-2EF1-6165-F2AF24F6F4D7";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p01_b04_c0Shape1" -p "dcx_p01_b04_c01";
	rename -uid "66155B73-D144-3177-0E69-C39B4840C482";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p01_b04_c02" -p "dcx_p01_b04";
	rename -uid "608CFD1B-CE4A-22AF-21E8-D194D333A94C";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p01_b04_c0Shape2" -p "dcx_p01_b04_c02";
	rename -uid "0D3EF419-AC41-AFE9-B7A3-77BE227F75C9";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p01_b04_c03" -p "dcx_p01_b04";
	rename -uid "C7CFDAFF-E943-5ECC-FA93-19B7DC91CB42";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p01_b04_c0Shape3" -p "dcx_p01_b04_c03";
	rename -uid "22D7ECE8-CF47-4CC0-1DAC-A0A2938B8210";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p03_b01" -p "|dipPaintingGroup|curves";
	rename -uid "3CAB6096-3246-77A6-0F72-16A0F12867F3";
	setAttr ".t" -type "double3" -6.2698128827262281 -103.77461076266363 0.073118674440067366 ;
	setAttr ".s" -type "double3" 1.0000000000000002 1.0000000000000007 1 ;
createNode transform -n "dcx_p03_b01_c00" -p "dcx_p03_b01";
	rename -uid "487D3ED1-7F45-D327-6EBD-95A3015E21FB";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "dcx_p03_b01_c0Shape0" -p "dcx_p03_b01_c00";
	rename -uid "36834EFB-8B43-86A5-ABC2-2E8F92395C72";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p03_b01_c01" -p "dcx_p03_b01";
	rename -uid "D669CD5B-0F4D-39BF-985C-AE8218005208";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p03_b01_c0Shape1" -p "dcx_p03_b01_c01";
	rename -uid "8D0E95B0-9944-1771-A1B3-CF88A1B59940";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p03_b01_c02" -p "dcx_p03_b01";
	rename -uid "C3C2B927-E04C-DAF4-32FA-2B92773E5788";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p03_b01_c0Shape2" -p "dcx_p03_b01_c02";
	rename -uid "38D3B244-E641-02DA-F8C7-4B986DCE94E7";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p03_b01_c03" -p "dcx_p03_b01";
	rename -uid "8E4D85F9-4F49-C7D1-C8D2-E5AC47070577";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p03_b01_c0Shape3" -p "dcx_p03_b01_c03";
	rename -uid "35F46AC1-3545-C679-FAD5-7499C24AB2D5";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p06_b03" -p "|dipPaintingGroup|curves";
	rename -uid "3C0F73A2-4F48-E3D9-134D-6E8739344A21";
	setAttr ".t" -type "double3" 40.930187117273817 -103.77461076266356 0.073118674440063813 ;
	setAttr ".s" -type "double3" 1.0000000000000002 1.0000000000000007 1 ;
createNode transform -n "dcx_p06_b03_c00" -p "dcx_p06_b03";
	rename -uid "7DC4E833-AC47-4271-24B8-068E55742DAF";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "dcx_p06_b03_c0Shape0" -p "dcx_p06_b03_c00";
	rename -uid "754EECD4-A746-6213-C293-21AB4261BBAF";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p06_b03_c01" -p "dcx_p06_b03";
	rename -uid "44881403-894A-CF15-0034-5F8969D59713";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p06_b03_c0Shape1" -p "dcx_p06_b03_c01";
	rename -uid "08A6980F-3F49-DE70-E1B5-C7A7F5457017";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p06_b03_c02" -p "dcx_p06_b03";
	rename -uid "FC7DE42B-294B-ABA4-BE87-17A65EAD3FA9";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p06_b03_c0Shape2" -p "dcx_p06_b03_c02";
	rename -uid "D669B003-E94C-A4CF-A5C8-109B9836A779";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p06_b03_c03" -p "dcx_p06_b03";
	rename -uid "D9B8C50E-2B4E-AAE3-AD77-359E197209D8";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p06_b03_c0Shape3" -p "dcx_p06_b03_c03";
	rename -uid "39880807-BB4A-C0E1-DFAC-DBB82BDD696E";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p03_b09" -p "|dipPaintingGroup|curves";
	rename -uid "2ECED4FC-3048-9E22-9A12-64B22CD00F57";
	setAttr ".t" -type "double3" -6.2698128827262281 -103.77461076266363 0.073118674440067366 ;
	setAttr ".s" -type "double3" 1.0000000000000002 1.0000000000000007 1 ;
createNode transform -n "dcx_p03_b09_c00" -p "dcx_p03_b09";
	rename -uid "4937FE4E-6347-3351-CEAC-0DAA29675CC7";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "dcx_p03_b09_c0Shape0" -p "dcx_p03_b09_c00";
	rename -uid "B27C5DC9-4449-9323-A2A1-1892F71A3615";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p03_b09_c01" -p "dcx_p03_b09";
	rename -uid "2AC383A2-5641-5295-3129-F2A2821713F6";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p03_b09_c0Shape1" -p "dcx_p03_b09_c01";
	rename -uid "AB6D75D5-3E4A-B07C-54AE-9C860DCC731F";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p03_b09_c02" -p "dcx_p03_b09";
	rename -uid "BF288A03-3A4B-7866-3F67-5187A73E24A5";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p03_b09_c0Shape2" -p "dcx_p03_b09_c02";
	rename -uid "56F61F7C-DC44-488D-B74A-378858BE49DD";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "dcx_p03_b09_c03" -p "dcx_p03_b09";
	rename -uid "11FBE3CA-CE44-8574-784B-94B7D6793C62";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "dcx_p03_b09_c0Shape3" -p "dcx_p03_b09_c03";
	rename -uid "D88142B5-E549-F6D7-EE66-44B4C849C69A";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "brushes";
	rename -uid "254D994A-254E-22F8-5167-24959E5842DA";
createNode transform -n "brushBase" -p "brushes";
	rename -uid "CB33840D-824B-9092-16B8-499650D42116";
createNode mesh -n "brushBaseShape" -p "brushBase";
	rename -uid "AF90AE5A-FC46-66BF-3880-82B3C1EBE53E";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.77109652757644653 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 42 ".uvst[0].uvsp[0:41]" -type "float2" 0.57812506 0.020933539
		 0.42187503 0.020933509 0.34375 0.15624997 0.421875 0.29156646 0.578125 0.29156649
		 0.65625 0.15625 0.375 0.3125 0.41666666 0.3125 0.45833331 0.3125 0.49999997 0.3125
		 0.54166663 0.3125 0.58333331 0.3125 0.625 0.3125 0.375 0.43781328 0.41666666 0.43781328
		 0.45833331 0.43781328 0.49999997 0.43781328 0.54166663 0.43781328 0.58333331 0.43781328
		 0.625 0.43781328 0.375 0.56312656 0.41666666 0.56312656 0.45833331 0.56312656 0.49999997
		 0.56312656 0.54166663 0.56312656 0.58333331 0.56312656 0.625 0.56312656 0.375 0.68843985
		 0.41666666 0.68843985 0.45833331 0.68843985 0.49999997 0.68843985 0.54166663 0.68843985
		 0.58333331 0.68843985 0.625 0.68843985 0.57812506 0.70843351 0.42187503 0.70843351
		 0.34375 0.84375 0.421875 0.97906649 0.578125 0.97906649 0.65625 0.84375 0.5 0.15000001
		 0.5 0.83749998;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 26 ".pt[0:25]" -type "float3"  0 0 6.0097003 0 0 6.0097003 
		0 0 6.0097003 0 0 6.0097003 0 0 6.0097003 0 0 6.0097003 0 0 2.6272383 0 0 2.6272383 
		0 0 2.6272383 0 0 2.6272383 0 0 2.6272383 0 0 2.6272383 -1.1275413e-06 -4.0592685 
		-0.81624562 3.5154276 -2.0296347 -0.81624562 3.5154278 2.0296335 -0.81624562 3.2215453e-07 
		4.0592685 -0.81624562 -3.5154278 2.029634 -0.81624562 -3.5154278 -2.0296345 -0.81624562 
		-1.1275413e-06 -4.0592685 3.3735409 3.5154276 -2.0296347 3.3735409 3.5154278 2.0296335 
		3.3735409 3.2215453e-07 4.0592685 3.3735409 -3.5154278 2.029634 3.3735409 -3.5154278 
		-2.0296345 3.3735409 0 0 6.0097003 -1.6107727e-07 -1.6107727e-07 3.3735409;
	setAttr -s 26 ".vt[0:25]"  1.4305115e-06 6.0083250999 -6.0083250999
		 -5.20336151 3.0041632652 -6.0083250999 -5.20336246 -3.0041618347 -6.0083250999 -7.1525574e-07 -6.0083255768 -6.0083250999
		 5.20336199 -3.00416255 -6.0083250999 5.20336199 3.00416255 -6.0083250999 1.4305115e-06 6.0083250999 -2.0027749538
		 -5.20336151 3.0041632652 -2.0027749538 -5.20336246 -3.0041618347 -2.0027749538 -7.1525574e-07 -6.0083255768 -2.0027749538
		 5.20336199 -3.00416255 -2.0027749538 5.20336199 3.00416255 -2.0027749538 1.4305115e-06 6.0083250999 2.0027751923
		 -5.20336151 3.0041632652 2.0027751923 -5.20336246 -3.0041618347 2.0027751923 -7.1525574e-07 -6.0083255768 2.0027751923
		 5.20336199 -3.00416255 2.0027751923 5.20336199 3.00416255 2.0027751923 1.4305115e-06 6.0083250999 6.0083250999
		 -5.20336151 3.0041632652 6.0083250999 -5.20336246 -3.0041618347 6.0083250999 -7.1525574e-07 -6.0083255768 6.0083250999
		 5.20336199 -3.00416255 6.0083250999 5.20336199 3.00416255 6.0083250999 6.6705809e-16 -1.1553785e-15 -6.0083250999
		 -6.6705809e-16 1.1553785e-15 6.0083250999;
	setAttr -s 54 ".ed[0:53]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 1
		 7 8 1 8 9 1 9 10 1 10 11 1 11 6 1 12 13 1 13 14 1 14 15 1 15 16 1 16 17 1 17 12 1
		 18 19 0 19 20 0 20 21 0 21 22 0 22 23 0 23 18 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0
		 6 12 0 7 13 0 8 14 0 9 15 0 10 16 0 11 17 0 12 18 0 13 19 0 14 20 0 15 21 0 16 22 0
		 17 23 0 24 0 1 24 1 1 24 2 1 24 3 1 24 4 1 24 5 1 18 25 1 19 25 1 20 25 1 21 25 1
		 22 25 1 23 25 1;
	setAttr -s 30 -ch 108 ".fc[0:29]" -type "polyFaces" 
		f 4 0 25 -7 -25
		mu 0 4 6 7 14 13
		f 4 1 26 -8 -26
		mu 0 4 7 8 15 14
		f 4 2 27 -9 -27
		mu 0 4 8 9 16 15
		f 4 3 28 -10 -28
		mu 0 4 9 10 17 16
		f 4 4 29 -11 -29
		mu 0 4 10 11 18 17
		f 4 5 24 -12 -30
		mu 0 4 11 12 19 18
		f 4 6 31 -13 -31
		mu 0 4 13 14 21 20
		f 4 7 32 -14 -32
		mu 0 4 14 15 22 21
		f 4 8 33 -15 -33
		mu 0 4 15 16 23 22
		f 4 9 34 -16 -34
		mu 0 4 16 17 24 23
		f 4 10 35 -17 -35
		mu 0 4 17 18 25 24
		f 4 11 30 -18 -36
		mu 0 4 18 19 26 25
		f 4 12 37 -19 -37
		mu 0 4 20 21 28 27
		f 4 13 38 -20 -38
		mu 0 4 21 22 29 28
		f 4 14 39 -21 -39
		mu 0 4 22 23 30 29
		f 4 15 40 -22 -40
		mu 0 4 23 24 31 30
		f 4 16 41 -23 -41
		mu 0 4 24 25 32 31
		f 4 17 36 -24 -42
		mu 0 4 25 26 33 32
		f 3 -1 -43 43
		mu 0 3 1 0 40
		f 3 -2 -44 44
		mu 0 3 2 1 40
		f 3 -3 -45 45
		mu 0 3 3 2 40
		f 3 -4 -46 46
		mu 0 3 4 3 40
		f 3 -5 -47 47
		mu 0 3 5 4 40
		f 3 -6 -48 42
		mu 0 3 0 5 40
		f 3 18 49 -49
		mu 0 3 38 37 41
		f 3 19 50 -50
		mu 0 3 37 36 41
		f 3 20 51 -51
		mu 0 3 36 35 41
		f 3 21 52 -52
		mu 0 3 35 34 41
		f 3 22 53 -53
		mu 0 3 34 39 41
		f 3 23 48 -54
		mu 0 3 39 38 41;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "paintingBrushes" -p "brushes";
	rename -uid "E496BCEF-E244-5CBC-4DE2-31B88CFE790D";
createNode transform -n "bpx_0_4mm_round_soft" -p "paintingBrushes";
	rename -uid "08A34835-004A-8463-A616-AC9406CBBD07";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 15.749999999999998 ;
	setAttr -k on ".sfBrushWidth" 0.5;
	setAttr -k on ".sfBrushShape" 1;
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_0_4mm_round_softShape" -p "bpx_0_4mm_round_soft";
	rename -uid "077321B2-3E4F-D108-8940-E3AB8FAF3D93";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:59]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 83 ".uvst[0].uvsp[0:82]" -type "float2" 0 0.16666667 0.16666667
		 0.16666667 0.33333334 0.16666667 0.5 0.16666667 0.66666669 0.16666667 0.83333337
		 0.16666667 1 0.16666667 0 0.33333334 0.16666667 0.33333334 0.33333334 0.33333334
		 0.5 0.33333334 0.66666669 0.33333334 0.83333337 0.33333334 1 0.33333334 0 0.5 0.16666667
		 0.5 0.33333334 0.5 0.5 0.5 0.66666669 0.5 0.83333337 0.5 1 0.5 0 0.66666669 0.16666667
		 0.66666669 0.33333334 0.66666669 0.5 0.66666669 0.66666669 0.66666669 0.83333337
		 0.66666669 1 0.66666669 0 0.83333337 0.16666667 0.83333337 0.33333334 0.83333337
		 0.5 0.83333337 0.66666669 0.83333337 0.83333337 0.83333337 1 0.83333337 0.083333336
		 0 0.25 0 0.41666669 0 0.58333337 0 0.75 0 0.91666669 0 0.083333336 1 0.25 1 0.41666669
		 1 0.58333337 1 0.75 1 0.91666669 1 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25
		 0.25 0.75 0.375 0.25 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25
		 0.75 0.75 0.875 0.25 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305
		 0.036611661 0.50187969 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634
		 0.21338834 0.50187969 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832
		 0.50187969 1 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634
		 0.78661168 0.50187969 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 50 ".vt[0:49]"  0.06250003 0.10825317 -1.23277223 -0.062499981 0.10825319 -1.23277223
		 -0.125 1.8626451e-08 -1.23277223 -0.062500015 -0.10825317 -1.23277223 0.062499996 -0.10825318 -1.23277223
		 0.125 -4.8074066e-17 -1.23277223 0.10825323 0.1875 -0.91249996 -0.10825315 0.18750003 -0.91249996
		 -0.21650636 3.2261962e-08 -0.91249996 -0.1082532 -0.1875 -0.91249996 0.10825317 -0.18750001 -0.91249996
		 0.21650636 -2.7755574e-17 -0.91249996 0.12500006 0.21650633 -0.47499999 -0.12499996 0.21650638 -0.47499999
		 -0.25 3.7252903e-08 -0.47499999 -0.12500003 -0.21650635 -0.47499999 0.12499999 -0.21650636 -0.47499999
		 0.25 0 -0.47499999 0.10825323 0.1875 -0.037500024 -0.10825315 0.18750003 -0.037500024
		 -0.21650636 3.2261962e-08 -0.037500024 -0.1082532 -0.1875 -0.037500024 0.10825317 -0.18750001 -0.037500024
		 0.21650636 2.7755574e-17 -0.037500024 0.06250003 0.10825317 0.28277221 -0.062499981 0.10825319 0.28277221
		 -0.125 1.8626451e-08 0.28277221 -0.062500015 -0.10825317 0.28277221 0.062499996 -0.10825318 0.28277221
		 0.125 4.8074066e-17 0.28277221 0 -5.5511151e-17 -1.35000002 0 5.5511151e-17 0.40000001
		 0.35355335 0.35355335 -15.75 0 0.49999994 -15.75 -0.35355335 0.35355335 -15.75 -0.49999994 -1.5987211e-15 -15.75
		 -0.35355335 -0.35355335 -15.75 0 -0.49999997 -15.75 0.35355338 -0.35355338 -15.75
		 0.5 -1.5987211e-15 -15.75 0.35355335 0.35355335 -1.35000038 0 0.49999994 -1.35000038
		 -0.35355335 0.35355335 -1.35000038 -0.49999994 1.5987211e-15 -1.35000038 -0.35355335 -0.35355335 -1.35000038
		 0 -0.49999997 -1.35000038 0.35355338 -0.35355338 -1.35000038 0.5 1.5987211e-15 -1.35000038
		 0 -1.5987211e-15 -15.75 0 1.5987211e-15 -1.35000038;
	setAttr -s 106 ".ed[0:105]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 12 13 0 13 14 0 14 15 0 15 16 0 16 17 0 17 12 0
		 18 19 0 19 20 0 20 21 0 21 22 0 22 23 0 23 18 0 24 25 0 25 26 0 26 27 0 27 28 0 28 29 0
		 29 24 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 6 12 0 7 13 0 8 14 0 9 15 0 10 16 0
		 11 17 0 12 18 0 13 19 0 14 20 0 15 21 0 16 22 0 17 23 0 18 24 0 19 25 0 20 26 0 21 27 0
		 22 28 0 23 29 0 30 0 0 30 1 0 30 2 0 30 3 0 30 4 0 30 5 0 24 31 0 25 31 0 26 31 0
		 27 31 0 28 31 0 29 31 0 32 33 0 33 34 0 34 35 0 35 36 0 36 37 0 37 38 0 38 39 0 39 32 0
		 40 41 0 41 42 0 42 43 0 43 44 0 44 45 0 45 46 0 46 47 0 47 40 0 32 40 0 33 41 0 34 42 0
		 35 43 0 36 44 0 37 45 0 38 46 0 39 47 0 48 32 1 48 33 1 48 34 1 48 35 1 48 36 1 48 37 1
		 48 38 1 48 39 1 40 49 1 41 49 1 42 49 1 43 49 1 44 49 1 45 49 1 46 49 1 47 49 1;
	setAttr -s 60 -ch 212 ".fc[0:59]" -type "polyFaces" 
		f 4 0 31 -7 -31
		mu 0 4 0 1 8 7
		f 4 1 32 -8 -32
		mu 0 4 1 2 9 8
		f 4 2 33 -9 -33
		mu 0 4 2 3 10 9
		f 4 3 34 -10 -34
		mu 0 4 3 4 11 10
		f 4 4 35 -11 -35
		mu 0 4 4 5 12 11
		f 4 5 30 -12 -36
		mu 0 4 5 6 13 12
		f 4 6 37 -13 -37
		mu 0 4 7 8 15 14
		f 4 7 38 -14 -38
		mu 0 4 8 9 16 15
		f 4 8 39 -15 -39
		mu 0 4 9 10 17 16
		f 4 9 40 -16 -40
		mu 0 4 10 11 18 17
		f 4 10 41 -17 -41
		mu 0 4 11 12 19 18
		f 4 11 36 -18 -42
		mu 0 4 12 13 20 19
		f 4 12 43 -19 -43
		mu 0 4 14 15 22 21
		f 4 13 44 -20 -44
		mu 0 4 15 16 23 22
		f 4 14 45 -21 -45
		mu 0 4 16 17 24 23
		f 4 15 46 -22 -46
		mu 0 4 17 18 25 24
		f 4 16 47 -23 -47
		mu 0 4 18 19 26 25
		f 4 17 42 -24 -48
		mu 0 4 19 20 27 26
		f 4 18 49 -25 -49
		mu 0 4 21 22 29 28
		f 4 19 50 -26 -50
		mu 0 4 22 23 30 29
		f 4 20 51 -27 -51
		mu 0 4 23 24 31 30
		f 4 21 52 -28 -52
		mu 0 4 24 25 32 31
		f 4 22 53 -29 -53
		mu 0 4 25 26 33 32
		f 4 23 48 -30 -54
		mu 0 4 26 27 34 33
		f 3 -1 -55 55
		mu 0 3 1 0 35
		f 3 -2 -56 56
		mu 0 3 2 1 36
		f 3 -3 -57 57
		mu 0 3 3 2 37
		f 3 -4 -58 58
		mu 0 3 4 3 38
		f 3 -5 -59 59
		mu 0 3 5 4 39
		f 3 -6 -60 54
		mu 0 3 6 5 40
		f 3 24 61 -61
		mu 0 3 28 29 41
		f 3 25 62 -62
		mu 0 3 29 30 42
		f 3 26 63 -63
		mu 0 3 30 31 43
		f 3 27 64 -64
		mu 0 3 31 32 44
		f 3 28 65 -65
		mu 0 3 32 33 45
		f 3 29 60 -66
		mu 0 3 33 34 46
		f 4 66 83 -75 -83
		mu 0 4 47 48 49 50
		f 4 67 84 -76 -84
		mu 0 4 48 51 52 49
		f 4 68 85 -77 -85
		mu 0 4 51 53 54 52
		f 4 69 86 -78 -86
		mu 0 4 53 55 56 54
		f 4 70 87 -79 -87
		mu 0 4 55 57 58 56
		f 4 71 88 -80 -88
		mu 0 4 57 59 60 58
		f 4 72 89 -81 -89
		mu 0 4 59 61 62 60
		f 4 73 82 -82 -90
		mu 0 4 61 63 64 62
		f 3 -67 -91 91
		mu 0 3 65 66 67
		f 3 -68 -92 92
		mu 0 3 68 65 67
		f 3 -69 -93 93
		mu 0 3 69 68 67
		f 3 -70 -94 94
		mu 0 3 70 69 67
		f 3 -71 -95 95
		mu 0 3 71 70 67
		f 3 -72 -96 96
		mu 0 3 72 71 67
		f 3 -73 -97 97
		mu 0 3 73 72 67
		f 3 -74 -98 90
		mu 0 3 66 73 67
		f 3 74 99 -99
		mu 0 3 74 75 76
		f 3 75 100 -100
		mu 0 3 75 77 76
		f 3 76 101 -101
		mu 0 3 77 78 76
		f 3 77 102 -102
		mu 0 3 78 79 76
		f 3 78 103 -103
		mu 0 3 79 80 76
		f 3 79 104 -104
		mu 0 3 80 81 76
		f 3 80 105 -105
		mu 0 3 81 82 76
		f 3 81 98 -106
		mu 0 3 82 74 76;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_1_6mm_round_syn" -p "paintingBrushes";
	rename -uid "9C5D3517-6F40-B5EF-86B0-F3BA55994316";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.3 ;
	setAttr -k on ".sfBrushWidth" 0.9;
	setAttr -k on ".sfBrushShape" 1;
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_1_6mm_round_synShape" -p "bpx_1_6mm_round_syn";
	rename -uid "3C24DA2F-8243-353C-2AD8-A494A03FFF41";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:59]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 83 ".uvst[0].uvsp[0:82]" -type "float2" 0 0.16666667 0.16666667
		 0.16666667 0.33333334 0.16666667 0.5 0.16666667 0.66666669 0.16666667 0.83333337
		 0.16666667 1 0.16666667 0 0.33333334 0.16666667 0.33333334 0.33333334 0.33333334
		 0.5 0.33333334 0.66666669 0.33333334 0.83333337 0.33333334 1 0.33333334 0 0.5 0.16666667
		 0.5 0.33333334 0.5 0.5 0.5 0.66666669 0.5 0.83333337 0.5 1 0.5 0 0.66666669 0.16666667
		 0.66666669 0.33333334 0.66666669 0.5 0.66666669 0.66666669 0.66666669 0.83333337
		 0.66666669 1 0.66666669 0 0.83333337 0.16666667 0.83333337 0.33333334 0.83333337
		 0.5 0.83333337 0.66666669 0.83333337 0.83333337 0.83333337 1 0.83333337 0.083333336
		 0 0.25 0 0.41666669 0 0.58333337 0 0.75 0 0.91666669 0 0.083333336 1 0.25 1 0.41666669
		 1 0.58333337 1 0.75 1 0.91666669 1 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25
		 0.25 0.75 0.375 0.25 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25
		 0.75 0.75 0.875 0.25 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305
		 0.036611661 0.50187969 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634
		 0.21338834 0.50187969 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832
		 0.50187969 1 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634
		 0.78661168 0.50187969 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 50 ".vt[0:49]"  0.11250005 0.19485569 -2.019134283 -0.11249997 0.19485573 -2.019134283
		 -0.22499999 3.3527613e-08 -2.019134283 -0.11250003 -0.1948557 -2.019134283 0.11249999 -0.19485572 -2.019134283
		 0.22499999 -8.6533316e-17 -2.019134283 0.19485581 0.33749998 -1.52499998 -0.19485566 0.33750004 -1.52499998
		 -0.38971144 5.8071532e-08 -1.52499998 -0.19485576 -0.33749998 -1.52499998 0.1948557 -0.33750001 -1.52499998
		 0.38971144 -4.9960031e-17 -1.52499998 0.2250001 0.38971138 -0.85000002 -0.22499993 0.38971147 -0.85000002
		 -0.44999999 6.7055225e-08 -0.85000002 -0.22500005 -0.38971141 -0.85000002 0.22499998 -0.38971144 -0.85000002
		 0.44999999 0 -0.85000002 0.19485581 0.33749998 -0.17500007 -0.19485566 0.33750004 -0.17500007
		 -0.38971144 5.8071532e-08 -0.17500007 -0.19485576 -0.33749998 -0.17500007 0.1948557 -0.33750001 -0.17500007
		 0.38971144 4.9960031e-17 -0.17500007 0.11250005 0.19485569 0.31913424 -0.11249997 0.19485573 0.31913424
		 -0.22499999 3.3527613e-08 0.31913424 -0.11250003 -0.1948557 0.31913424 0.11249999 -0.19485572 0.31913424
		 0.22499999 8.6533316e-17 0.31913424 0 -9.992007e-17 -2.20000005 0 9.992007e-17 0.5
		 0.35355335 0.35355335 -17.29999924 0 0.49999994 -17.29999924 -0.35355335 0.35355335 -17.29999924
		 -0.49999994 -1.6764368e-15 -17.29999924 -0.35355335 -0.35355335 -17.29999924 0 -0.49999997 -17.29999924
		 0.35355338 -0.35355338 -17.29999924 0.5 -1.6764368e-15 -17.29999924 0.35355335 0.35355335 -2.19999981
		 0 0.49999994 -2.19999981 -0.35355335 0.35355335 -2.19999981 -0.49999994 1.6764368e-15 -2.19999981
		 -0.35355335 -0.35355335 -2.19999981 0 -0.49999997 -2.19999981 0.35355338 -0.35355338 -2.19999981
		 0.5 1.6764368e-15 -2.19999981 0 -1.6764368e-15 -17.29999924 0 1.6764368e-15 -2.19999981;
	setAttr -s 106 ".ed[0:105]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 12 13 0 13 14 0 14 15 0 15 16 0 16 17 0 17 12 0
		 18 19 0 19 20 0 20 21 0 21 22 0 22 23 0 23 18 0 24 25 0 25 26 0 26 27 0 27 28 0 28 29 0
		 29 24 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 6 12 0 7 13 0 8 14 0 9 15 0 10 16 0
		 11 17 0 12 18 0 13 19 0 14 20 0 15 21 0 16 22 0 17 23 0 18 24 0 19 25 0 20 26 0 21 27 0
		 22 28 0 23 29 0 30 0 0 30 1 0 30 2 0 30 3 0 30 4 0 30 5 0 24 31 0 25 31 0 26 31 0
		 27 31 0 28 31 0 29 31 0 32 33 0 33 34 0 34 35 0 35 36 0 36 37 0 37 38 0 38 39 0 39 32 0
		 40 41 0 41 42 0 42 43 0 43 44 0 44 45 0 45 46 0 46 47 0 47 40 0 32 40 0 33 41 0 34 42 0
		 35 43 0 36 44 0 37 45 0 38 46 0 39 47 0 48 32 1 48 33 1 48 34 1 48 35 1 48 36 1 48 37 1
		 48 38 1 48 39 1 40 49 1 41 49 1 42 49 1 43 49 1 44 49 1 45 49 1 46 49 1 47 49 1;
	setAttr -s 60 -ch 212 ".fc[0:59]" -type "polyFaces" 
		f 4 0 31 -7 -31
		mu 0 4 0 1 8 7
		f 4 1 32 -8 -32
		mu 0 4 1 2 9 8
		f 4 2 33 -9 -33
		mu 0 4 2 3 10 9
		f 4 3 34 -10 -34
		mu 0 4 3 4 11 10
		f 4 4 35 -11 -35
		mu 0 4 4 5 12 11
		f 4 5 30 -12 -36
		mu 0 4 5 6 13 12
		f 4 6 37 -13 -37
		mu 0 4 7 8 15 14
		f 4 7 38 -14 -38
		mu 0 4 8 9 16 15
		f 4 8 39 -15 -39
		mu 0 4 9 10 17 16
		f 4 9 40 -16 -40
		mu 0 4 10 11 18 17
		f 4 10 41 -17 -41
		mu 0 4 11 12 19 18
		f 4 11 36 -18 -42
		mu 0 4 12 13 20 19
		f 4 12 43 -19 -43
		mu 0 4 14 15 22 21
		f 4 13 44 -20 -44
		mu 0 4 15 16 23 22
		f 4 14 45 -21 -45
		mu 0 4 16 17 24 23
		f 4 15 46 -22 -46
		mu 0 4 17 18 25 24
		f 4 16 47 -23 -47
		mu 0 4 18 19 26 25
		f 4 17 42 -24 -48
		mu 0 4 19 20 27 26
		f 4 18 49 -25 -49
		mu 0 4 21 22 29 28
		f 4 19 50 -26 -50
		mu 0 4 22 23 30 29
		f 4 20 51 -27 -51
		mu 0 4 23 24 31 30
		f 4 21 52 -28 -52
		mu 0 4 24 25 32 31
		f 4 22 53 -29 -53
		mu 0 4 25 26 33 32
		f 4 23 48 -30 -54
		mu 0 4 26 27 34 33
		f 3 -1 -55 55
		mu 0 3 1 0 35
		f 3 -2 -56 56
		mu 0 3 2 1 36
		f 3 -3 -57 57
		mu 0 3 3 2 37
		f 3 -4 -58 58
		mu 0 3 4 3 38
		f 3 -5 -59 59
		mu 0 3 5 4 39
		f 3 -6 -60 54
		mu 0 3 6 5 40
		f 3 24 61 -61
		mu 0 3 28 29 41
		f 3 25 62 -62
		mu 0 3 29 30 42
		f 3 26 63 -63
		mu 0 3 30 31 43
		f 3 27 64 -64
		mu 0 3 31 32 44
		f 3 28 65 -65
		mu 0 3 32 33 45
		f 3 29 60 -66
		mu 0 3 33 34 46
		f 4 66 83 -75 -83
		mu 0 4 47 48 49 50
		f 4 67 84 -76 -84
		mu 0 4 48 51 52 49
		f 4 68 85 -77 -85
		mu 0 4 51 53 54 52
		f 4 69 86 -78 -86
		mu 0 4 53 55 56 54
		f 4 70 87 -79 -87
		mu 0 4 55 57 58 56
		f 4 71 88 -80 -88
		mu 0 4 57 59 60 58
		f 4 72 89 -81 -89
		mu 0 4 59 61 62 60
		f 4 73 82 -82 -90
		mu 0 4 61 63 64 62
		f 3 -67 -91 91
		mu 0 3 65 66 67
		f 3 -68 -92 92
		mu 0 3 68 65 67
		f 3 -69 -93 93
		mu 0 3 69 68 67
		f 3 -70 -94 94
		mu 0 3 70 69 67
		f 3 -71 -95 95
		mu 0 3 71 70 67
		f 3 -72 -96 96
		mu 0 3 72 71 67
		f 3 -73 -97 97
		mu 0 3 73 72 67
		f 3 -74 -98 90
		mu 0 3 66 73 67
		f 3 74 99 -99
		mu 0 3 74 75 76
		f 3 75 100 -100
		mu 0 3 75 77 76
		f 3 76 101 -101
		mu 0 3 77 78 76
		f 3 77 102 -102
		mu 0 3 78 79 76
		f 3 78 103 -103
		mu 0 3 79 80 76
		f 3 79 104 -104
		mu 0 3 80 81 76
		f 3 80 105 -105
		mu 0 3 81 82 76
		f 3 81 98 -106
		mu 0 3 82 74 76;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_2_8mm_round_soft" -p "paintingBrushes";
	rename -uid "9C379DD7-9A47-BA24-3341-AEBACFB4BD7B";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 16.799999999999997 ;
	setAttr -k on ".sfBrushWidth" 1;
	setAttr -k on ".sfBrushShape" 1;
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_2_8mm_round_softShape" -p "bpx_2_8mm_round_soft";
	rename -uid "E68FBC8B-BD4B-3B8E-8699-B7B525D25B53";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:59]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 83 ".uvst[0].uvsp[0:82]" -type "float2" 0 0.16666667 0.16666667
		 0.16666667 0.33333334 0.16666667 0.5 0.16666667 0.66666669 0.16666667 0.83333337
		 0.16666667 1 0.16666667 0 0.33333334 0.16666667 0.33333334 0.33333334 0.33333334
		 0.5 0.33333334 0.66666669 0.33333334 0.83333337 0.33333334 1 0.33333334 0 0.5 0.16666667
		 0.5 0.33333334 0.5 0.5 0.5 0.66666669 0.5 0.83333337 0.5 1 0.5 0 0.66666669 0.16666667
		 0.66666669 0.33333334 0.66666669 0.5 0.66666669 0.66666669 0.66666669 0.83333337
		 0.66666669 1 0.66666669 0 0.83333337 0.16666667 0.83333337 0.33333334 0.83333337
		 0.5 0.83333337 0.66666669 0.83333337 0.83333337 0.83333337 1 0.83333337 0.083333336
		 0 0.25 0 0.41666669 0 0.58333337 0 0.75 0 0.91666669 0 0.083333336 1 0.25 1 0.41666669
		 1 0.58333337 1 0.75 1 0.91666669 1 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25
		 0.25 0.75 0.375 0.25 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25
		 0.75 0.75 0.875 0.25 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305
		 0.036611661 0.50187969 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634
		 0.21338834 0.50187969 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832
		 0.50187969 1 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634
		 0.78661168 0.50187969 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 50 ".vt[0:49]"  0.12500006 0.21650633 -2.19903803 -0.12499996 0.21650638 -2.19903803
		 -0.25 3.7252903e-08 -2.19903803 -0.12500003 -0.21650635 -2.19903803 0.12499999 -0.21650636 -2.19903803
		 0.25 -9.6148133e-17 -2.19903803 0.21650647 0.375 -1.64999986 -0.2165063 0.37500006 -1.64999986
		 -0.43301272 6.4523924e-08 -1.64999986 -0.21650641 -0.375 -1.64999986 0.21650635 -0.37500003 -1.64999986
		 0.43301272 -5.5511148e-17 -1.64999986 0.25000012 0.43301266 -0.89999998 -0.24999993 0.43301275 -0.89999998
		 -0.5 7.4505806e-08 -0.89999998 -0.25000006 -0.43301269 -0.89999998 0.24999999 -0.43301272 -0.89999998
		 0.5 0 -0.89999998 0.21650647 0.375 -0.15000004 -0.2165063 0.37500006 -0.15000004
		 -0.43301272 6.4523924e-08 -0.15000004 -0.21650641 -0.375 -0.15000004 0.21650635 -0.37500003 -0.15000004
		 0.43301272 5.5511148e-17 -0.15000004 0.12500006 0.21650633 0.39903808 -0.12499996 0.21650638 0.39903808
		 -0.25 3.7252903e-08 0.39903808 -0.12500003 -0.21650635 0.39903808 0.12499999 -0.21650636 0.39903808
		 0.25 9.6148133e-17 0.39903808 0 -1.110223e-16 -2.4000001 0 1.110223e-16 0.60000002
		 0.35355335 0.35355335 -16.79999924 0 0.49999994 -16.79999924 -0.35355335 0.35355335 -16.79999924
		 -0.49999994 -1.5987211e-15 -16.79999924 -0.35355335 -0.35355335 -16.79999924 0 -0.49999997 -16.79999924
		 0.35355338 -0.35355338 -16.79999924 0.5 -1.5987211e-15 -16.79999924 0.35355335 0.35355335 -2.40000057
		 0 0.49999994 -2.40000057 -0.35355335 0.35355335 -2.40000057 -0.49999994 1.5987211e-15 -2.40000057
		 -0.35355335 -0.35355335 -2.40000057 0 -0.49999997 -2.40000057 0.35355338 -0.35355338 -2.40000057
		 0.5 1.5987211e-15 -2.40000057 0 -1.5987211e-15 -16.79999924 0 1.5987211e-15 -2.40000057;
	setAttr -s 106 ".ed[0:105]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 12 13 0 13 14 0 14 15 0 15 16 0 16 17 0 17 12 0
		 18 19 0 19 20 0 20 21 0 21 22 0 22 23 0 23 18 0 24 25 0 25 26 0 26 27 0 27 28 0 28 29 0
		 29 24 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 6 12 0 7 13 0 8 14 0 9 15 0 10 16 0
		 11 17 0 12 18 0 13 19 0 14 20 0 15 21 0 16 22 0 17 23 0 18 24 0 19 25 0 20 26 0 21 27 0
		 22 28 0 23 29 0 30 0 0 30 1 0 30 2 0 30 3 0 30 4 0 30 5 0 24 31 0 25 31 0 26 31 0
		 27 31 0 28 31 0 29 31 0 32 33 0 33 34 0 34 35 0 35 36 0 36 37 0 37 38 0 38 39 0 39 32 0
		 40 41 0 41 42 0 42 43 0 43 44 0 44 45 0 45 46 0 46 47 0 47 40 0 32 40 0 33 41 0 34 42 0
		 35 43 0 36 44 0 37 45 0 38 46 0 39 47 0 48 32 1 48 33 1 48 34 1 48 35 1 48 36 1 48 37 1
		 48 38 1 48 39 1 40 49 1 41 49 1 42 49 1 43 49 1 44 49 1 45 49 1 46 49 1 47 49 1;
	setAttr -s 60 -ch 212 ".fc[0:59]" -type "polyFaces" 
		f 4 0 31 -7 -31
		mu 0 4 0 1 8 7
		f 4 1 32 -8 -32
		mu 0 4 1 2 9 8
		f 4 2 33 -9 -33
		mu 0 4 2 3 10 9
		f 4 3 34 -10 -34
		mu 0 4 3 4 11 10
		f 4 4 35 -11 -35
		mu 0 4 4 5 12 11
		f 4 5 30 -12 -36
		mu 0 4 5 6 13 12
		f 4 6 37 -13 -37
		mu 0 4 7 8 15 14
		f 4 7 38 -14 -38
		mu 0 4 8 9 16 15
		f 4 8 39 -15 -39
		mu 0 4 9 10 17 16
		f 4 9 40 -16 -40
		mu 0 4 10 11 18 17
		f 4 10 41 -17 -41
		mu 0 4 11 12 19 18
		f 4 11 36 -18 -42
		mu 0 4 12 13 20 19
		f 4 12 43 -19 -43
		mu 0 4 14 15 22 21
		f 4 13 44 -20 -44
		mu 0 4 15 16 23 22
		f 4 14 45 -21 -45
		mu 0 4 16 17 24 23
		f 4 15 46 -22 -46
		mu 0 4 17 18 25 24
		f 4 16 47 -23 -47
		mu 0 4 18 19 26 25
		f 4 17 42 -24 -48
		mu 0 4 19 20 27 26
		f 4 18 49 -25 -49
		mu 0 4 21 22 29 28
		f 4 19 50 -26 -50
		mu 0 4 22 23 30 29
		f 4 20 51 -27 -51
		mu 0 4 23 24 31 30
		f 4 21 52 -28 -52
		mu 0 4 24 25 32 31
		f 4 22 53 -29 -53
		mu 0 4 25 26 33 32
		f 4 23 48 -30 -54
		mu 0 4 26 27 34 33
		f 3 -1 -55 55
		mu 0 3 1 0 35
		f 3 -2 -56 56
		mu 0 3 2 1 36
		f 3 -3 -57 57
		mu 0 3 3 2 37
		f 3 -4 -58 58
		mu 0 3 4 3 38
		f 3 -5 -59 59
		mu 0 3 5 4 39
		f 3 -6 -60 54
		mu 0 3 6 5 40
		f 3 24 61 -61
		mu 0 3 28 29 41
		f 3 25 62 -62
		mu 0 3 29 30 42
		f 3 26 63 -63
		mu 0 3 30 31 43
		f 3 27 64 -64
		mu 0 3 31 32 44
		f 3 28 65 -65
		mu 0 3 32 33 45
		f 3 29 60 -66
		mu 0 3 33 34 46
		f 4 66 83 -75 -83
		mu 0 4 47 48 49 50
		f 4 67 84 -76 -84
		mu 0 4 48 51 52 49
		f 4 68 85 -77 -85
		mu 0 4 51 53 54 52
		f 4 69 86 -78 -86
		mu 0 4 53 55 56 54
		f 4 70 87 -79 -87
		mu 0 4 55 57 58 56
		f 4 71 88 -80 -88
		mu 0 4 57 59 60 58
		f 4 72 89 -81 -89
		mu 0 4 59 61 62 60
		f 4 73 82 -82 -90
		mu 0 4 61 63 64 62
		f 3 -67 -91 91
		mu 0 3 65 66 67
		f 3 -68 -92 92
		mu 0 3 68 65 67
		f 3 -69 -93 93
		mu 0 3 69 68 67
		f 3 -70 -94 94
		mu 0 3 70 69 67
		f 3 -71 -95 95
		mu 0 3 71 70 67
		f 3 -72 -96 96
		mu 0 3 72 71 67
		f 3 -73 -97 97
		mu 0 3 73 72 67
		f 3 -74 -98 90
		mu 0 3 66 73 67
		f 3 74 99 -99
		mu 0 3 74 75 76
		f 3 75 100 -100
		mu 0 3 75 77 76
		f 3 76 101 -101
		mu 0 3 77 78 76
		f 3 77 102 -102
		mu 0 3 78 79 76
		f 3 78 103 -103
		mu 0 3 79 80 76
		f 3 79 104 -104
		mu 0 3 80 81 76
		f 3 80 105 -105
		mu 0 3 81 82 76
		f 3 81 98 -106
		mu 0 3 82 74 76;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_3_10mm_flat_na" -p "paintingBrushes";
	rename -uid "E5F813D5-5341-A344-D3AD-1594B4436BED";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 16.6 ;
	setAttr -k on ".sfBrushWidth" 1.2;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_3_10mm_flat_naShape" -p "bpx_3_10mm_flat_na";
	rename -uid "A9B91169-9B46-0A65-129F-1B9077E64F13";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.30000016 0.17320506 -1.35000002 -0.29999992 0.17320511 -1.35000002
		 -0.60000002 2.9802322e-08 -1.35000002 -0.30000007 -0.17320508 -1.35000002 0.29999998 -0.17320509 -1.35000002
		 0.60000002 -4.4408922e-17 -1.35000002 0.30000016 0.17320506 0.40000001 -0.29999992 0.17320511 0.40000001
		 -0.60000002 2.9802322e-08 0.40000001 -0.30000007 -0.17320508 0.40000001 0.29999998 -0.17320509 0.40000001
		 0.60000002 4.4408922e-17 0.40000001 0 -4.4408922e-17 -1.35000002 0 4.4408922e-17 0.40000001
		 0.35355335 0.35355335 -16.60000038 0 0.49999994 -16.60000038 -0.35355335 0.35355335 -16.60000038
		 -0.49999994 -1.6930901e-15 -16.60000038 -0.35355335 -0.35355335 -16.60000038 0 -0.49999997 -16.60000038
		 0.35355338 -0.35355338 -16.60000038 0.5 -1.6930901e-15 -16.60000038 0.35355335 0.35355335 -1.35000038
		 0 0.49999994 -1.35000038 -0.35355335 0.35355335 -1.35000038 -0.49999994 1.6930901e-15 -1.35000038
		 -0.35355335 -0.35355335 -1.35000038 0 -0.49999997 -1.35000038 0.35355338 -0.35355338 -1.35000038
		 0.5 1.6930901e-15 -1.35000038 0 -1.6930901e-15 -16.60000038 0 1.6930901e-15 -1.35000038;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_4_13mm_flat_na" -p "paintingBrushes";
	rename -uid "0F7E468D-FD47-3C43-AB49-99BC5D15FE81";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.099999999999998 ;
	setAttr -k on ".sfBrushWidth" 1.5;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_4_13mm_flat_naShape" -p "bpx_4_13mm_flat_na";
	rename -uid "65EBD1DF-414F-8012-3BE8-1E9EF53611C1";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.37500018 0.17320506 -1.10000002 -0.37499988 0.17320511 -1.10000002
		 -0.75 2.9802322e-08 -1.10000002 -0.37500009 -0.17320508 -1.10000002 0.37499997 -0.17320509 -1.10000002
		 0.75 -4.4408922e-17 -1.10000002 0.37500018 0.17320506 0.79999995 -0.37499988 0.17320511 0.79999995
		 -0.75 2.9802322e-08 0.79999995 -0.37500009 -0.17320508 0.79999995 0.37499997 -0.17320509 0.79999995
		 0.75 4.4408922e-17 0.79999995 0 -4.4408922e-17 -1.10000002 0 4.4408922e-17 0.79999995
		 0.35355335 0.35355335 -17.10000038 0 0.49999994 -17.10000038 -0.35355335 0.35355335 -17.10000038
		 -0.49999994 -1.7763568e-15 -17.10000038 -0.35355335 -0.35355335 -17.10000038 0 -0.49999997 -17.10000038
		 0.35355338 -0.35355338 -17.10000038 0.5 -1.7763568e-15 -17.10000038 0.35355335 0.35355335 -1.10000038
		 0 0.49999994 -1.10000038 -0.35355335 0.35355335 -1.10000038 -0.49999994 1.7763568e-15 -1.10000038
		 -0.35355335 -0.35355335 -1.10000038 0 -0.49999997 -1.10000038 0.35355338 -0.35355338 -1.10000038
		 0.5 1.7763568e-15 -1.10000038 0 -1.7763568e-15 -17.10000038 0 1.7763568e-15 -1.10000038;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_5_15mm_flat_na" -p "paintingBrushes";
	rename -uid "7F762A83-E649-8424-DF4A-7EADB01DEEA5";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 18.15 ;
	setAttr -k on ".sfBrushWidth" 1.75;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_5_15mm_flat_naShape" -p "bpx_5_15mm_flat_na";
	rename -uid "4D4F8845-854E-7892-F3C1-2CAC5D949521";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.43750021 0.17320506 -1.70000005 -0.43749988 0.17320511 -1.70000005
		 -0.875 2.9802322e-08 -1.70000005 -0.43750012 -0.17320508 -1.70000005 0.43749997 -0.17320509 -1.70000005
		 0.875 -4.4408922e-17 -1.70000005 0.43750021 0.17320506 0.5 -0.43749988 0.17320511 0.5
		 -0.875 2.9802322e-08 0.5 -0.43750012 -0.17320508 0.5 0.43749997 -0.17320509 0.5 0.875 4.4408922e-17 0.5
		 0 -4.4408922e-17 -1.70000005 0 4.4408922e-17 0.5 0.35355335 0.35355335 -18.15000153
		 0 0.49999994 -18.15000153 -0.35355335 0.35355335 -18.15000153 -0.49999994 -1.826317e-15 -18.15000153
		 -0.35355335 -0.35355335 -18.15000153 0 -0.49999997 -18.15000153 0.35355338 -0.35355338 -18.15000153
		 0.5 -1.826317e-15 -18.15000153 0.35355335 0.35355335 -1.69999981 0 0.49999994 -1.69999981
		 -0.35355335 0.35355335 -1.69999981 -0.49999994 1.826317e-15 -1.69999981 -0.35355335 -0.35355335 -1.69999981
		 0 -0.49999997 -1.69999981 0.35355338 -0.35355338 -1.69999981 0.5 1.826317e-15 -1.69999981
		 0 -1.826317e-15 -18.15000153 0 1.826317e-15 -1.69999981;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_6_17mm_flat_na" -p "paintingBrushes";
	rename -uid "DEA8053F-194E-DCB2-572A-26A5E9EE842C";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 18.4 ;
	setAttr -k on ".sfBrushWidth" 2.2;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_6_17mm_flat_naShape" -p "bpx_6_17mm_flat_na";
	rename -uid "CA4C5D24-C843-BC1C-1C9D-108F73923B13";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.55000025 0.17320506 -1.85000002 -0.54999983 0.17320511 -1.85000002
		 -1.10000002 2.9802322e-08 -1.85000002 -0.55000013 -0.17320508 -1.85000002 0.54999995 -0.17320509 -1.85000002
		 1.10000002 -4.4408922e-17 -1.85000002 0.55000025 0.17320506 0.60000002 -0.54999983 0.17320511 0.60000002
		 -1.10000002 2.9802322e-08 0.60000002 -0.55000013 -0.17320508 0.60000002 0.54999995 -0.17320509 0.60000002
		 1.10000002 4.4408922e-17 0.60000002 0 -4.4408922e-17 -1.85000002 0 4.4408922e-17 0.60000002
		 0.35355335 0.35355335 -18.39999962 0 0.49999994 -18.39999962 -0.35355335 0.35355335 -18.39999962
		 -0.49999994 -1.837419e-15 -18.39999962 -0.35355335 -0.35355335 -18.39999962 0 -0.49999997 -18.39999962
		 0.35355338 -0.35355338 -18.39999962 0.5 -1.837419e-15 -18.39999962 0.35355335 0.35355335 -1.85000038
		 0 0.49999994 -1.85000038 -0.35355335 0.35355335 -1.85000038 -0.49999994 1.837419e-15 -1.85000038
		 -0.35355335 -0.35355335 -1.85000038 0 -0.49999997 -1.85000038 0.35355338 -0.35355338 -1.85000038
		 0.5 1.837419e-15 -1.85000038 0 -1.837419e-15 -18.39999962 0 1.837419e-15 -1.85000038;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_7_20mm_flat_na" -p "paintingBrushes";
	rename -uid "8C6D7203-D14D-7D4C-47AA-E4853F0ADC9C";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.900000000000002 ;
	setAttr -k on ".sfBrushWidth" 2.3;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_7_20mm_flat_naShape" -p "bpx_7_20mm_flat_na";
	rename -uid "737E8995-D34F-27EA-A480-EC849A7B81DC";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.57500029 0.17320506 -2.20000005 -0.57499981 0.17320511 -2.20000005
		 -1.14999998 2.9802322e-08 -2.20000005 -0.57500011 -0.17320508 -2.20000005 0.57499993 -0.17320509 -2.20000005
		 1.14999998 -4.4408922e-17 -2.20000005 0.57500029 0.17320506 0.70000005 -0.57499981 0.17320511 0.70000005
		 -1.14999998 2.9802322e-08 0.70000005 -0.57500011 -0.17320508 0.70000005 0.57499993 -0.17320509 0.70000005
		 1.14999998 4.4408922e-17 0.70000005 0 -4.4408922e-17 -2.20000005 0 4.4408922e-17 0.70000005
		 0.35355335 0.35355335 -17.89999962 0 0.49999994 -17.89999962 -0.35355335 0.35355335 -17.89999962
		 -0.49999994 -1.7430501e-15 -17.89999962 -0.35355335 -0.35355335 -17.89999962 0 -0.49999997 -17.89999962
		 0.35355338 -0.35355338 -17.89999962 0.5 -1.7430501e-15 -17.89999962 0.35355335 0.35355335 -2.20000029
		 0 0.49999994 -2.20000029 -0.35355335 0.35355335 -2.20000029 -0.49999994 1.7430501e-15 -2.20000029
		 -0.35355335 -0.35355335 -2.20000029 0 -0.49999997 -2.20000029 0.35355338 -0.35355338 -2.20000029
		 0.5 1.7430501e-15 -2.20000029 0 -1.7430501e-15 -17.89999962 0 1.7430501e-15 -2.20000029;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_8_28mm_flat_na" -p "paintingBrushes";
	rename -uid "0934EF5D-864C-A517-548B-6DA9D17CEFC9";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.4 ;
	setAttr -k on ".sfBrushWidth" 3;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_8_28mm_flat_naShape" -p "bpx_8_28mm_flat_na";
	rename -uid "10AE4357-0843-53C6-0B7C-68BEF67F5714";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.75000036 0.17320506 -2.099999905 -0.74999976 0.17320511 -2.099999905
		 -1.5 2.9802322e-08 -2.099999905 -0.75000018 -0.17320508 -2.099999905 0.74999994 -0.17320509 -2.099999905
		 1.5 -4.4408922e-17 -2.099999905 0.75000036 0.17320506 0.60000002 -0.74999976 0.17320511 0.60000002
		 -1.5 2.9802322e-08 0.60000002 -0.75000018 -0.17320508 0.60000002 0.74999994 -0.17320509 0.60000002
		 1.5 4.4408922e-17 0.60000002 0 -4.4408922e-17 -2.099999905 0 4.4408922e-17 0.60000002
		 0.35355335 0.35355335 -17.39999962 0 0.49999994 -17.39999962 -0.35355335 0.35355335 -17.39999962
		 -0.49999994 -1.6986412e-15 -17.39999962 -0.35355335 -0.35355335 -17.39999962 0 -0.49999997 -17.39999962
		 0.35355338 -0.35355338 -17.39999962 0.5 -1.6986412e-15 -17.39999962 0.35355335 0.35355335 -2.099999905
		 0 0.49999994 -2.099999905 -0.35355335 0.35355335 -2.099999905 -0.49999994 1.6986412e-15 -2.099999905
		 -0.35355335 -0.35355335 -2.099999905 0 -0.49999997 -2.099999905 0.35355338 -0.35355338 -2.099999905
		 0.5 1.6986412e-15 -2.099999905 0 -1.6986412e-15 -17.39999962 0 1.6986412e-15 -2.099999905;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_9_35mm_flat_na" -p "paintingBrushes";
	rename -uid "474FD83B-554C-4C32-440C-2AA0ACF3F675";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 16.849999999999998 ;
	setAttr -k on ".sfBrushWidth" 4;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_9_35mm_flat_naShape" -p "bpx_9_35mm_flat_na";
	rename -uid "D82052C3-6949-9D59-0A90-F69FA9D58147";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  1.000000476837 0.17320506 -1.79999995 -0.9999997 0.17320511 -1.79999995
		 -2 2.9802322e-08 -1.79999995 -1.000000238419 -0.17320508 -1.79999995 0.99999994 -0.17320509 -1.79999995
		 2 -4.4408922e-17 -1.79999995 1.000000476837 0.17320506 1.29999995 -0.9999997 0.17320511 1.29999995
		 -2 2.9802322e-08 1.29999995 -1.000000238419 -0.17320508 1.29999995 0.99999994 -0.17320509 1.29999995
		 2 4.4408922e-17 1.29999995 0 -4.4408922e-17 -1.79999995 0 4.4408922e-17 1.29999995
		 0.35355335 0.35355335 -16.85000038 0 0.49999994 -16.85000038 -0.35355335 0.35355335 -16.85000038
		 -0.49999994 -1.6708857e-15 -16.85000038 -0.35355335 -0.35355335 -16.85000038 0 -0.49999997 -16.85000038
		 0.35355338 -0.35355338 -16.85000038 0.5 -1.6708857e-15 -16.85000038 0.35355335 0.35355335 -1.79999971
		 0 0.49999994 -1.79999971 -0.35355335 0.35355335 -1.79999971 -0.49999994 1.6708857e-15 -1.79999971
		 -0.35355335 -0.35355335 -1.79999971 0 -0.49999997 -1.79999971 0.35355338 -0.35355338 -1.79999971
		 0.5 1.6708857e-15 -1.79999971 0 -1.6708857e-15 -16.85000038 0 1.6708857e-15 -1.79999971;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_10_40mm_flat_na" -p "paintingBrushes";
	rename -uid "53365806-2A4A-7FC3-D744-A292172991FE";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.700000000000003 ;
	setAttr -k on ".sfBrushWidth" 4.5;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_10_40mm_flat_naShape" -p "bpx_10_40mm_flat_na";
	rename -uid "A754C3D7-2545-B41A-BA51-238459F4443D";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  1.12500048 0.17320506 -2.25 -1.12499964 0.17320511 -2.25
		 -2.25 2.9802322e-08 -2.25 -1.12500024 -0.17320508 -2.25 1.12499988 -0.17320509 -2.25
		 2.25 -4.4408922e-17 -2.25 1.12500048 0.17320506 0.90000004 -1.12499964 0.17320511 0.90000004
		 -2.25 2.9802322e-08 0.90000004 -1.12500024 -0.17320508 0.90000004 1.12499988 -0.17320509 0.90000004
		 2.25 4.4408922e-17 0.90000004 0 -4.4408922e-17 -2.25 0 4.4408922e-17 0.90000004 0.35355335 0.35355335 -17.70000076
		 0 0.49999994 -17.70000076 -0.35355335 0.35355335 -17.70000076 -0.49999994 -1.7152946e-15 -17.70000076
		 -0.35355335 -0.35355335 -17.70000076 0 -0.49999997 -17.70000076 0.35355338 -0.35355338 -17.70000076
		 0.5 -1.7152946e-15 -17.70000076 0.35355335 0.35355335 -2.25000048 0 0.49999994 -2.25000048
		 -0.35355335 0.35355335 -2.25000048 -0.49999994 1.7152946e-15 -2.25000048 -0.35355335 -0.35355335 -2.25000048
		 0 -0.49999997 -2.25000048 0.35355338 -0.35355338 -2.25000048 0.5 1.7152946e-15 -2.25000048
		 0 -1.7152946e-15 -17.70000076 0 1.7152946e-15 -2.25000048;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_11_52mm_flat_na" -p "paintingBrushes";
	rename -uid "8D109D25-334A-E222-D620-2C92D42C3A76";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.900000000000002 ;
	setAttr -k on ".sfBrushWidth" 5.5;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_11_52mm_flat_naShape" -p "bpx_11_52mm_flat_na";
	rename -uid "E0696BBA-1849-11DF-AB21-4BAF2F978934";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  1.37500072 0.17320506 -2.29999995 -1.37499964 0.17320511 -2.29999995
		 -2.75 2.9802322e-08 -2.29999995 -1.37500036 -0.17320508 -2.29999995 1.37499988 -0.17320509 -2.29999995
		 2.75 -4.4408922e-17 -2.29999995 1.37500072 0.17320506 1.89999986 -1.37499964 0.17320511 1.89999986
		 -2.75 2.9802322e-08 1.89999986 -1.37500036 -0.17320508 1.89999986 1.37499988 -0.17320509 1.89999986
		 2.75 4.4408922e-17 1.89999986 0 -4.4408922e-17 -2.29999995 0 4.4408922e-17 1.89999986
		 0.35355335 0.35355335 -17.90000153 0 0.49999994 -17.90000153 -0.35355335 0.35355335 -17.90000153
		 -0.49999994 -1.731948e-15 -17.90000153 -0.35355335 -0.35355335 -17.90000153 0 -0.49999997 -17.90000153
		 0.35355338 -0.35355338 -17.90000153 0.5 -1.731948e-15 -17.90000153 0.35355335 0.35355335 -2.30000019
		 0 0.49999994 -2.30000019 -0.35355335 0.35355335 -2.30000019 -0.49999994 1.731948e-15 -2.30000019
		 -0.35355335 -0.35355335 -2.30000019 0 -0.49999997 -2.30000019 0.35355338 -0.35355338 -2.30000019
		 0.5 1.731948e-15 -2.30000019 0 -1.731948e-15 -17.90000153 0 1.731948e-15 -2.30000019;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bpx_12_78mm_flat_na" -p "paintingBrushes";
	rename -uid "D4F2A4FD-5F48-F21D-2C1C-55AEA257E1C9";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 19.2 ;
	setAttr -k on ".sfBrushWidth" 8;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1;
createNode mesh -n "bpx_12_78mm_flat_naShape" -p "bpx_12_78mm_flat_na";
	rename -uid "479CC920-7B46-2F45-7732-A9B889FB353E";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  2.000000953674 0.17320506 -3.80000019 -1.9999994 0.17320511 -3.80000019
		 -4 2.9802322e-08 -3.80000019 -2.000000476837 -0.17320508 -3.80000019 1.99999988 -0.17320509 -3.80000019
		 4 -4.4408922e-17 -3.80000019 2.000000953674 0.17320506 1.000000119209 -1.9999994 0.17320511 1.000000119209
		 -4 2.9802322e-08 1.000000119209 -2.000000476837 -0.17320508 1.000000119209 1.99999988 -0.17320509 1.000000119209
		 4 4.4408922e-17 1.000000119209 0 -4.4408922e-17 -3.80000019 0 4.4408922e-17 1.000000119209
		 0.35355335 0.35355335 -19.20000076 0 0.49999994 -19.20000076 -0.35355335 0.35355335 -19.20000076
		 -0.49999994 -1.7097434e-15 -19.20000076 -0.35355335 -0.35355335 -19.20000076 0 -0.49999997 -19.20000076
		 0.35355338 -0.35355338 -19.20000076 0.5 -1.7097434e-15 -19.20000076 0.35355335 0.35355335 -3.80000019
		 0 0.49999994 -3.80000019 -0.35355335 0.35355335 -3.80000019 -0.49999994 1.7097434e-15 -3.80000019
		 -0.35355335 -0.35355335 -3.80000019 0 -0.49999997 -3.80000019 0.35355338 -0.35355338 -3.80000019
		 0.5 1.7097434e-15 -3.80000019 0 -1.7097434e-15 -19.20000076 0 1.7097434e-15 -3.80000019;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "dipBrushes" -p "brushes";
	rename -uid "9ACCDFAD-E640-C550-47F7-12BB5E374DDF";
createNode transform -n "bdx_0_4mm_round_soft" -p "dipBrushes";
	rename -uid "288654FB-3F4A-DED4-F965-C0AF53BA4C7D";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 15.399999999999999 ;
	setAttr -k on ".sfBrushWidth" 0.5;
	setAttr -k on ".sfBrushShape" 1;
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_0_4mm_round_softShape" -p "bdx_0_4mm_round_soft";
	rename -uid "4E144477-6443-2A8F-3D0B-1882B267D1E9";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:59]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 83 ".uvst[0].uvsp[0:82]" -type "float2" 0 0.16666667 0.16666667
		 0.16666667 0.33333334 0.16666667 0.5 0.16666667 0.66666669 0.16666667 0.83333337
		 0.16666667 1 0.16666667 0 0.33333334 0.16666667 0.33333334 0.33333334 0.33333334
		 0.5 0.33333334 0.66666669 0.33333334 0.83333337 0.33333334 1 0.33333334 0 0.5 0.16666667
		 0.5 0.33333334 0.5 0.5 0.5 0.66666669 0.5 0.83333337 0.5 1 0.5 0 0.66666669 0.16666667
		 0.66666669 0.33333334 0.66666669 0.5 0.66666669 0.66666669 0.66666669 0.83333337
		 0.66666669 1 0.66666669 0 0.83333337 0.16666667 0.83333337 0.33333334 0.83333337
		 0.5 0.83333337 0.66666669 0.83333337 0.83333337 0.83333337 1 0.83333337 0.083333336
		 0 0.25 0 0.41666669 0 0.58333337 0 0.75 0 0.91666669 0 0.083333336 1 0.25 1 0.41666669
		 1 0.58333337 1 0.75 1 0.91666669 1 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25
		 0.25 0.75 0.375 0.25 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25
		 0.75 0.75 0.875 0.25 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305
		 0.036611661 0.50187969 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634
		 0.21338834 0.50187969 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832
		 0.50187969 1 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634
		 0.78661168 0.50187969 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 50 ".vt[0:49]"  0.06250003 0.10825317 -0.88277221 -0.062499981 0.10825319 -0.88277221
		 -0.125 1.8626451e-08 -0.88277221 -0.062500015 -0.10825317 -0.88277221 0.062499996 -0.10825318 -0.88277221
		 0.125 -4.8074066e-17 -0.88277221 0.10825323 0.1875 -0.5625 -0.10825315 0.18750003 -0.5625
		 -0.21650636 3.2261962e-08 -0.5625 -0.1082532 -0.1875 -0.5625 0.10825317 -0.18750001 -0.5625
		 0.21650636 -2.7755574e-17 -0.5625 0.12500006 0.21650633 -0.125 -0.12499996 0.21650638 -0.125
		 -0.25 3.7252903e-08 -0.125 -0.12500003 -0.21650635 -0.125 0.12499999 -0.21650636 -0.125
		 0.25 0 -0.125 0.10825323 0.1875 0.31249997 -0.10825315 0.18750003 0.31249997 -0.21650636 3.2261962e-08 0.31249997
		 -0.1082532 -0.1875 0.31249997 0.10825317 -0.18750001 0.31249997 0.21650636 2.7755574e-17 0.31249997
		 0.06250003 0.10825317 0.63277221 -0.062499981 0.10825319 0.63277221 -0.125 1.8626451e-08 0.63277221
		 -0.062500015 -0.10825317 0.63277221 0.062499996 -0.10825318 0.63277221 0.125 4.8074066e-17 0.63277221
		 0 -5.5511151e-17 -1 0 5.5511151e-17 0.75 0.35355335 0.35355335 -15.39999962 0 0.49999994 -15.39999962
		 -0.35355335 0.35355335 -15.39999962 -0.49999994 -1.5987211e-15 -15.39999962 -0.35355335 -0.35355335 -15.39999962
		 0 -0.49999997 -15.39999962 0.35355338 -0.35355338 -15.39999962 0.5 -1.5987211e-15 -15.39999962
		 0.35355335 0.35355335 -1 0 0.49999994 -1 -0.35355335 0.35355335 -1 -0.49999994 1.5987211e-15 -1
		 -0.35355335 -0.35355335 -1 0 -0.49999997 -1 0.35355338 -0.35355338 -1 0.5 1.5987211e-15 -1
		 0 -1.5987211e-15 -15.39999962 0 1.5987211e-15 -1;
	setAttr -s 106 ".ed[0:105]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 12 13 0 13 14 0 14 15 0 15 16 0 16 17 0 17 12 0
		 18 19 0 19 20 0 20 21 0 21 22 0 22 23 0 23 18 0 24 25 0 25 26 0 26 27 0 27 28 0 28 29 0
		 29 24 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 6 12 0 7 13 0 8 14 0 9 15 0 10 16 0
		 11 17 0 12 18 0 13 19 0 14 20 0 15 21 0 16 22 0 17 23 0 18 24 0 19 25 0 20 26 0 21 27 0
		 22 28 0 23 29 0 30 0 0 30 1 0 30 2 0 30 3 0 30 4 0 30 5 0 24 31 0 25 31 0 26 31 0
		 27 31 0 28 31 0 29 31 0 32 33 0 33 34 0 34 35 0 35 36 0 36 37 0 37 38 0 38 39 0 39 32 0
		 40 41 0 41 42 0 42 43 0 43 44 0 44 45 0 45 46 0 46 47 0 47 40 0 32 40 0 33 41 0 34 42 0
		 35 43 0 36 44 0 37 45 0 38 46 0 39 47 0 48 32 1 48 33 1 48 34 1 48 35 1 48 36 1 48 37 1
		 48 38 1 48 39 1 40 49 1 41 49 1 42 49 1 43 49 1 44 49 1 45 49 1 46 49 1 47 49 1;
	setAttr -s 60 -ch 212 ".fc[0:59]" -type "polyFaces" 
		f 4 0 31 -7 -31
		mu 0 4 0 1 8 7
		f 4 1 32 -8 -32
		mu 0 4 1 2 9 8
		f 4 2 33 -9 -33
		mu 0 4 2 3 10 9
		f 4 3 34 -10 -34
		mu 0 4 3 4 11 10
		f 4 4 35 -11 -35
		mu 0 4 4 5 12 11
		f 4 5 30 -12 -36
		mu 0 4 5 6 13 12
		f 4 6 37 -13 -37
		mu 0 4 7 8 15 14
		f 4 7 38 -14 -38
		mu 0 4 8 9 16 15
		f 4 8 39 -15 -39
		mu 0 4 9 10 17 16
		f 4 9 40 -16 -40
		mu 0 4 10 11 18 17
		f 4 10 41 -17 -41
		mu 0 4 11 12 19 18
		f 4 11 36 -18 -42
		mu 0 4 12 13 20 19
		f 4 12 43 -19 -43
		mu 0 4 14 15 22 21
		f 4 13 44 -20 -44
		mu 0 4 15 16 23 22
		f 4 14 45 -21 -45
		mu 0 4 16 17 24 23
		f 4 15 46 -22 -46
		mu 0 4 17 18 25 24
		f 4 16 47 -23 -47
		mu 0 4 18 19 26 25
		f 4 17 42 -24 -48
		mu 0 4 19 20 27 26
		f 4 18 49 -25 -49
		mu 0 4 21 22 29 28
		f 4 19 50 -26 -50
		mu 0 4 22 23 30 29
		f 4 20 51 -27 -51
		mu 0 4 23 24 31 30
		f 4 21 52 -28 -52
		mu 0 4 24 25 32 31
		f 4 22 53 -29 -53
		mu 0 4 25 26 33 32
		f 4 23 48 -30 -54
		mu 0 4 26 27 34 33
		f 3 -1 -55 55
		mu 0 3 1 0 35
		f 3 -2 -56 56
		mu 0 3 2 1 36
		f 3 -3 -57 57
		mu 0 3 3 2 37
		f 3 -4 -58 58
		mu 0 3 4 3 38
		f 3 -5 -59 59
		mu 0 3 5 4 39
		f 3 -6 -60 54
		mu 0 3 6 5 40
		f 3 24 61 -61
		mu 0 3 28 29 41
		f 3 25 62 -62
		mu 0 3 29 30 42
		f 3 26 63 -63
		mu 0 3 30 31 43
		f 3 27 64 -64
		mu 0 3 31 32 44
		f 3 28 65 -65
		mu 0 3 32 33 45
		f 3 29 60 -66
		mu 0 3 33 34 46
		f 4 66 83 -75 -83
		mu 0 4 47 48 49 50
		f 4 67 84 -76 -84
		mu 0 4 48 51 52 49
		f 4 68 85 -77 -85
		mu 0 4 51 53 54 52
		f 4 69 86 -78 -86
		mu 0 4 53 55 56 54
		f 4 70 87 -79 -87
		mu 0 4 55 57 58 56
		f 4 71 88 -80 -88
		mu 0 4 57 59 60 58
		f 4 72 89 -81 -89
		mu 0 4 59 61 62 60
		f 4 73 82 -82 -90
		mu 0 4 61 63 64 62
		f 3 -67 -91 91
		mu 0 3 65 66 67
		f 3 -68 -92 92
		mu 0 3 68 65 67
		f 3 -69 -93 93
		mu 0 3 69 68 67
		f 3 -70 -94 94
		mu 0 3 70 69 67
		f 3 -71 -95 95
		mu 0 3 71 70 67
		f 3 -72 -96 96
		mu 0 3 72 71 67
		f 3 -73 -97 97
		mu 0 3 73 72 67
		f 3 -74 -98 90
		mu 0 3 66 73 67
		f 3 74 99 -99
		mu 0 3 74 75 76
		f 3 75 100 -100
		mu 0 3 75 77 76
		f 3 76 101 -101
		mu 0 3 77 78 76
		f 3 77 102 -102
		mu 0 3 78 79 76
		f 3 78 103 -103
		mu 0 3 79 80 76
		f 3 79 104 -104
		mu 0 3 80 81 76
		f 3 80 105 -105
		mu 0 3 81 82 76
		f 3 81 98 -106
		mu 0 3 82 74 76;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_1_6mm_round_syn" -p "dipBrushes";
	rename -uid "269B504A-0A46-CD17-84FD-EB886CAA620B";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 16.8 ;
	setAttr -k on ".sfBrushWidth" 0.9;
	setAttr -k on ".sfBrushShape" 1;
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_1_6mm_round_synShape" -p "bdx_1_6mm_round_syn";
	rename -uid "FABF3F53-6641-8DA2-3691-059102EF3806";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:59]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 83 ".uvst[0].uvsp[0:82]" -type "float2" 0 0.16666667 0.16666667
		 0.16666667 0.33333334 0.16666667 0.5 0.16666667 0.66666669 0.16666667 0.83333337
		 0.16666667 1 0.16666667 0 0.33333334 0.16666667 0.33333334 0.33333334 0.33333334
		 0.5 0.33333334 0.66666669 0.33333334 0.83333337 0.33333334 1 0.33333334 0 0.5 0.16666667
		 0.5 0.33333334 0.5 0.5 0.5 0.66666669 0.5 0.83333337 0.5 1 0.5 0 0.66666669 0.16666667
		 0.66666669 0.33333334 0.66666669 0.5 0.66666669 0.66666669 0.66666669 0.83333337
		 0.66666669 1 0.66666669 0 0.83333337 0.16666667 0.83333337 0.33333334 0.83333337
		 0.5 0.83333337 0.66666669 0.83333337 0.83333337 0.83333337 1 0.83333337 0.083333336
		 0 0.25 0 0.41666669 0 0.58333337 0 0.75 0 0.91666669 0 0.083333336 1 0.25 1 0.41666669
		 1 0.58333337 1 0.75 1 0.91666669 1 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25
		 0.25 0.75 0.375 0.25 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25
		 0.75 0.75 0.875 0.25 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305
		 0.036611661 0.50187969 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634
		 0.21338834 0.50187969 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832
		 0.50187969 1 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634
		 0.78661168 0.50187969 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 50 ".vt[0:49]"  0.11250005 0.19485569 -1.51913428 -0.11249997 0.19485573 -1.51913428
		 -0.22499999 3.3527613e-08 -1.51913428 -0.11250003 -0.1948557 -1.51913428 0.11249999 -0.19485572 -1.51913428
		 0.22499999 -8.6533316e-17 -1.51913428 0.19485581 0.33749998 -1.024999976 -0.19485566 0.33750004 -1.024999976
		 -0.38971144 5.8071532e-08 -1.024999976 -0.19485576 -0.33749998 -1.024999976 0.1948557 -0.33750001 -1.024999976
		 0.38971144 -4.9960031e-17 -1.024999976 0.2250001 0.38971138 -0.34999999 -0.22499993 0.38971147 -0.34999999
		 -0.44999999 6.7055225e-08 -0.34999999 -0.22500005 -0.38971141 -0.34999999 0.22499998 -0.38971144 -0.34999999
		 0.44999999 0 -0.34999999 0.19485581 0.33749998 0.32499996 -0.19485566 0.33750004 0.32499996
		 -0.38971144 5.8071532e-08 0.32499996 -0.19485576 -0.33749998 0.32499996 0.1948557 -0.33750001 0.32499996
		 0.38971144 4.9960031e-17 0.32499996 0.11250005 0.19485569 0.81913424 -0.11249997 0.19485573 0.81913424
		 -0.22499999 3.3527613e-08 0.81913424 -0.11250003 -0.1948557 0.81913424 0.11249999 -0.19485572 0.81913424
		 0.22499999 8.6533316e-17 0.81913424 0 -9.992007e-17 -1.70000005 0 9.992007e-17 1
		 0.35355335 0.35355335 -16.79999924 0 0.49999994 -16.79999924 -0.35355335 0.35355335 -16.79999924
		 -0.49999994 -1.6764368e-15 -16.79999924 -0.35355335 -0.35355335 -16.79999924 0 -0.49999997 -16.79999924
		 0.35355338 -0.35355338 -16.79999924 0.5 -1.6764368e-15 -16.79999924 0.35355335 0.35355335 -1.69999981
		 0 0.49999994 -1.69999981 -0.35355335 0.35355335 -1.69999981 -0.49999994 1.6764368e-15 -1.69999981
		 -0.35355335 -0.35355335 -1.69999981 0 -0.49999997 -1.69999981 0.35355338 -0.35355338 -1.69999981
		 0.5 1.6764368e-15 -1.69999981 0 -1.6764368e-15 -16.79999924 0 1.6764368e-15 -1.69999981;
	setAttr -s 106 ".ed[0:105]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 12 13 0 13 14 0 14 15 0 15 16 0 16 17 0 17 12 0
		 18 19 0 19 20 0 20 21 0 21 22 0 22 23 0 23 18 0 24 25 0 25 26 0 26 27 0 27 28 0 28 29 0
		 29 24 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 6 12 0 7 13 0 8 14 0 9 15 0 10 16 0
		 11 17 0 12 18 0 13 19 0 14 20 0 15 21 0 16 22 0 17 23 0 18 24 0 19 25 0 20 26 0 21 27 0
		 22 28 0 23 29 0 30 0 0 30 1 0 30 2 0 30 3 0 30 4 0 30 5 0 24 31 0 25 31 0 26 31 0
		 27 31 0 28 31 0 29 31 0 32 33 0 33 34 0 34 35 0 35 36 0 36 37 0 37 38 0 38 39 0 39 32 0
		 40 41 0 41 42 0 42 43 0 43 44 0 44 45 0 45 46 0 46 47 0 47 40 0 32 40 0 33 41 0 34 42 0
		 35 43 0 36 44 0 37 45 0 38 46 0 39 47 0 48 32 1 48 33 1 48 34 1 48 35 1 48 36 1 48 37 1
		 48 38 1 48 39 1 40 49 1 41 49 1 42 49 1 43 49 1 44 49 1 45 49 1 46 49 1 47 49 1;
	setAttr -s 60 -ch 212 ".fc[0:59]" -type "polyFaces" 
		f 4 0 31 -7 -31
		mu 0 4 0 1 8 7
		f 4 1 32 -8 -32
		mu 0 4 1 2 9 8
		f 4 2 33 -9 -33
		mu 0 4 2 3 10 9
		f 4 3 34 -10 -34
		mu 0 4 3 4 11 10
		f 4 4 35 -11 -35
		mu 0 4 4 5 12 11
		f 4 5 30 -12 -36
		mu 0 4 5 6 13 12
		f 4 6 37 -13 -37
		mu 0 4 7 8 15 14
		f 4 7 38 -14 -38
		mu 0 4 8 9 16 15
		f 4 8 39 -15 -39
		mu 0 4 9 10 17 16
		f 4 9 40 -16 -40
		mu 0 4 10 11 18 17
		f 4 10 41 -17 -41
		mu 0 4 11 12 19 18
		f 4 11 36 -18 -42
		mu 0 4 12 13 20 19
		f 4 12 43 -19 -43
		mu 0 4 14 15 22 21
		f 4 13 44 -20 -44
		mu 0 4 15 16 23 22
		f 4 14 45 -21 -45
		mu 0 4 16 17 24 23
		f 4 15 46 -22 -46
		mu 0 4 17 18 25 24
		f 4 16 47 -23 -47
		mu 0 4 18 19 26 25
		f 4 17 42 -24 -48
		mu 0 4 19 20 27 26
		f 4 18 49 -25 -49
		mu 0 4 21 22 29 28
		f 4 19 50 -26 -50
		mu 0 4 22 23 30 29
		f 4 20 51 -27 -51
		mu 0 4 23 24 31 30
		f 4 21 52 -28 -52
		mu 0 4 24 25 32 31
		f 4 22 53 -29 -53
		mu 0 4 25 26 33 32
		f 4 23 48 -30 -54
		mu 0 4 26 27 34 33
		f 3 -1 -55 55
		mu 0 3 1 0 35
		f 3 -2 -56 56
		mu 0 3 2 1 36
		f 3 -3 -57 57
		mu 0 3 3 2 37
		f 3 -4 -58 58
		mu 0 3 4 3 38
		f 3 -5 -59 59
		mu 0 3 5 4 39
		f 3 -6 -60 54
		mu 0 3 6 5 40
		f 3 24 61 -61
		mu 0 3 28 29 41
		f 3 25 62 -62
		mu 0 3 29 30 42
		f 3 26 63 -63
		mu 0 3 30 31 43
		f 3 27 64 -64
		mu 0 3 31 32 44
		f 3 28 65 -65
		mu 0 3 32 33 45
		f 3 29 60 -66
		mu 0 3 33 34 46
		f 4 66 83 -75 -83
		mu 0 4 47 48 49 50
		f 4 67 84 -76 -84
		mu 0 4 48 51 52 49
		f 4 68 85 -77 -85
		mu 0 4 51 53 54 52
		f 4 69 86 -78 -86
		mu 0 4 53 55 56 54
		f 4 70 87 -79 -87
		mu 0 4 55 57 58 56
		f 4 71 88 -80 -88
		mu 0 4 57 59 60 58
		f 4 72 89 -81 -89
		mu 0 4 59 61 62 60
		f 4 73 82 -82 -90
		mu 0 4 61 63 64 62
		f 3 -67 -91 91
		mu 0 3 65 66 67
		f 3 -68 -92 92
		mu 0 3 68 65 67
		f 3 -69 -93 93
		mu 0 3 69 68 67
		f 3 -70 -94 94
		mu 0 3 70 69 67
		f 3 -71 -95 95
		mu 0 3 71 70 67
		f 3 -72 -96 96
		mu 0 3 72 71 67
		f 3 -73 -97 97
		mu 0 3 73 72 67
		f 3 -74 -98 90
		mu 0 3 66 73 67
		f 3 74 99 -99
		mu 0 3 74 75 76
		f 3 75 100 -100
		mu 0 3 75 77 76
		f 3 76 101 -101
		mu 0 3 77 78 76
		f 3 77 102 -102
		mu 0 3 78 79 76
		f 3 78 103 -103
		mu 0 3 79 80 76
		f 3 79 104 -104
		mu 0 3 80 81 76
		f 3 80 105 -105
		mu 0 3 81 82 76
		f 3 81 98 -106
		mu 0 3 82 74 76;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_2_8mm_round_soft" -p "dipBrushes";
	rename -uid "05AC8274-3B42-17FF-B2FC-CCBC3846175F";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 16.2 ;
	setAttr -k on ".sfBrushWidth" 1;
	setAttr -k on ".sfBrushShape" 1;
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_2_8mm_round_softShape" -p "bdx_2_8mm_round_soft";
	rename -uid "E637AD75-6546-FD69-47D6-0E80B5A4D633";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:59]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 83 ".uvst[0].uvsp[0:82]" -type "float2" 0 0.16666667 0.16666667
		 0.16666667 0.33333334 0.16666667 0.5 0.16666667 0.66666669 0.16666667 0.83333337
		 0.16666667 1 0.16666667 0 0.33333334 0.16666667 0.33333334 0.33333334 0.33333334
		 0.5 0.33333334 0.66666669 0.33333334 0.83333337 0.33333334 1 0.33333334 0 0.5 0.16666667
		 0.5 0.33333334 0.5 0.5 0.5 0.66666669 0.5 0.83333337 0.5 1 0.5 0 0.66666669 0.16666667
		 0.66666669 0.33333334 0.66666669 0.5 0.66666669 0.66666669 0.66666669 0.83333337
		 0.66666669 1 0.66666669 0 0.83333337 0.16666667 0.83333337 0.33333334 0.83333337
		 0.5 0.83333337 0.66666669 0.83333337 0.83333337 0.83333337 1 0.83333337 0.083333336
		 0 0.25 0 0.41666669 0 0.58333337 0 0.75 0 0.91666669 0 0.083333336 1 0.25 1 0.41666669
		 1 0.58333337 1 0.75 1 0.91666669 1 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25
		 0.25 0.75 0.375 0.25 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25
		 0.75 0.75 0.875 0.25 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305
		 0.036611661 0.50187969 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634
		 0.21338834 0.50187969 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832
		 0.50187969 1 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634
		 0.78661168 0.50187969 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 50 ".vt[0:49]"  0.12500006 0.21650633 -1.59903812 -0.12499996 0.21650638 -1.59903812
		 -0.25 3.7252903e-08 -1.59903812 -0.12500003 -0.21650635 -1.59903812 0.12499999 -0.21650636 -1.59903812
		 0.25 -9.6148133e-17 -1.59903812 0.21650647 0.375 -1.049999952 -0.2165063 0.37500006 -1.049999952
		 -0.43301272 6.4523924e-08 -1.049999952 -0.21650641 -0.375 -1.049999952 0.21650635 -0.37500003 -1.049999952
		 0.43301272 -5.5511148e-17 -1.049999952 0.25000012 0.43301266 -0.30000001 -0.24999993 0.43301275 -0.30000001
		 -0.5 7.4505806e-08 -0.30000001 -0.25000006 -0.43301269 -0.30000001 0.24999999 -0.43301272 -0.30000001
		 0.5 0 -0.30000001 0.21650647 0.375 0.44999993 -0.2165063 0.37500006 0.44999993 -0.43301272 6.4523924e-08 0.44999993
		 -0.21650641 -0.375 0.44999993 0.21650635 -0.37500003 0.44999993 0.43301272 5.5511148e-17 0.44999993
		 0.12500006 0.21650633 0.99903804 -0.12499996 0.21650638 0.99903804 -0.25 3.7252903e-08 0.99903804
		 -0.12500003 -0.21650635 0.99903804 0.12499999 -0.21650636 0.99903804 0.25 9.6148133e-17 0.99903804
		 0 -1.110223e-16 -1.79999995 0 1.110223e-16 1.20000005 0.35355335 0.35355335 -16.20000076
		 0 0.49999994 -16.20000076 -0.35355335 0.35355335 -16.20000076 -0.49999994 -1.5987211e-15 -16.20000076
		 -0.35355335 -0.35355335 -16.20000076 0 -0.49999997 -16.20000076 0.35355338 -0.35355338 -16.20000076
		 0.5 -1.5987211e-15 -16.20000076 0.35355335 0.35355335 -1.80000019 0 0.49999994 -1.80000019
		 -0.35355335 0.35355335 -1.80000019 -0.49999994 1.5987211e-15 -1.80000019 -0.35355335 -0.35355335 -1.80000019
		 0 -0.49999997 -1.80000019 0.35355338 -0.35355338 -1.80000019 0.5 1.5987211e-15 -1.80000019
		 0 -1.5987211e-15 -16.20000076 0 1.5987211e-15 -1.80000019;
	setAttr -s 106 ".ed[0:105]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 12 13 0 13 14 0 14 15 0 15 16 0 16 17 0 17 12 0
		 18 19 0 19 20 0 20 21 0 21 22 0 22 23 0 23 18 0 24 25 0 25 26 0 26 27 0 27 28 0 28 29 0
		 29 24 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 6 12 0 7 13 0 8 14 0 9 15 0 10 16 0
		 11 17 0 12 18 0 13 19 0 14 20 0 15 21 0 16 22 0 17 23 0 18 24 0 19 25 0 20 26 0 21 27 0
		 22 28 0 23 29 0 30 0 0 30 1 0 30 2 0 30 3 0 30 4 0 30 5 0 24 31 0 25 31 0 26 31 0
		 27 31 0 28 31 0 29 31 0 32 33 0 33 34 0 34 35 0 35 36 0 36 37 0 37 38 0 38 39 0 39 32 0
		 40 41 0 41 42 0 42 43 0 43 44 0 44 45 0 45 46 0 46 47 0 47 40 0 32 40 0 33 41 0 34 42 0
		 35 43 0 36 44 0 37 45 0 38 46 0 39 47 0 48 32 1 48 33 1 48 34 1 48 35 1 48 36 1 48 37 1
		 48 38 1 48 39 1 40 49 1 41 49 1 42 49 1 43 49 1 44 49 1 45 49 1 46 49 1 47 49 1;
	setAttr -s 60 -ch 212 ".fc[0:59]" -type "polyFaces" 
		f 4 0 31 -7 -31
		mu 0 4 0 1 8 7
		f 4 1 32 -8 -32
		mu 0 4 1 2 9 8
		f 4 2 33 -9 -33
		mu 0 4 2 3 10 9
		f 4 3 34 -10 -34
		mu 0 4 3 4 11 10
		f 4 4 35 -11 -35
		mu 0 4 4 5 12 11
		f 4 5 30 -12 -36
		mu 0 4 5 6 13 12
		f 4 6 37 -13 -37
		mu 0 4 7 8 15 14
		f 4 7 38 -14 -38
		mu 0 4 8 9 16 15
		f 4 8 39 -15 -39
		mu 0 4 9 10 17 16
		f 4 9 40 -16 -40
		mu 0 4 10 11 18 17
		f 4 10 41 -17 -41
		mu 0 4 11 12 19 18
		f 4 11 36 -18 -42
		mu 0 4 12 13 20 19
		f 4 12 43 -19 -43
		mu 0 4 14 15 22 21
		f 4 13 44 -20 -44
		mu 0 4 15 16 23 22
		f 4 14 45 -21 -45
		mu 0 4 16 17 24 23
		f 4 15 46 -22 -46
		mu 0 4 17 18 25 24
		f 4 16 47 -23 -47
		mu 0 4 18 19 26 25
		f 4 17 42 -24 -48
		mu 0 4 19 20 27 26
		f 4 18 49 -25 -49
		mu 0 4 21 22 29 28
		f 4 19 50 -26 -50
		mu 0 4 22 23 30 29
		f 4 20 51 -27 -51
		mu 0 4 23 24 31 30
		f 4 21 52 -28 -52
		mu 0 4 24 25 32 31
		f 4 22 53 -29 -53
		mu 0 4 25 26 33 32
		f 4 23 48 -30 -54
		mu 0 4 26 27 34 33
		f 3 -1 -55 55
		mu 0 3 1 0 35
		f 3 -2 -56 56
		mu 0 3 2 1 36
		f 3 -3 -57 57
		mu 0 3 3 2 37
		f 3 -4 -58 58
		mu 0 3 4 3 38
		f 3 -5 -59 59
		mu 0 3 5 4 39
		f 3 -6 -60 54
		mu 0 3 6 5 40
		f 3 24 61 -61
		mu 0 3 28 29 41
		f 3 25 62 -62
		mu 0 3 29 30 42
		f 3 26 63 -63
		mu 0 3 30 31 43
		f 3 27 64 -64
		mu 0 3 31 32 44
		f 3 28 65 -65
		mu 0 3 32 33 45
		f 3 29 60 -66
		mu 0 3 33 34 46
		f 4 66 83 -75 -83
		mu 0 4 47 48 49 50
		f 4 67 84 -76 -84
		mu 0 4 48 51 52 49
		f 4 68 85 -77 -85
		mu 0 4 51 53 54 52
		f 4 69 86 -78 -86
		mu 0 4 53 55 56 54
		f 4 70 87 -79 -87
		mu 0 4 55 57 58 56
		f 4 71 88 -80 -88
		mu 0 4 57 59 60 58
		f 4 72 89 -81 -89
		mu 0 4 59 61 62 60
		f 4 73 82 -82 -90
		mu 0 4 61 63 64 62
		f 3 -67 -91 91
		mu 0 3 65 66 67
		f 3 -68 -92 92
		mu 0 3 68 65 67
		f 3 -69 -93 93
		mu 0 3 69 68 67
		f 3 -70 -94 94
		mu 0 3 70 69 67
		f 3 -71 -95 95
		mu 0 3 71 70 67
		f 3 -72 -96 96
		mu 0 3 72 71 67
		f 3 -73 -97 97
		mu 0 3 73 72 67
		f 3 -74 -98 90
		mu 0 3 66 73 67
		f 3 74 99 -99
		mu 0 3 74 75 76
		f 3 75 100 -100
		mu 0 3 75 77 76
		f 3 76 101 -101
		mu 0 3 77 78 76
		f 3 77 102 -102
		mu 0 3 78 79 76
		f 3 78 103 -103
		mu 0 3 79 80 76
		f 3 79 104 -104
		mu 0 3 80 81 76
		f 3 80 105 -105
		mu 0 3 81 82 76
		f 3 81 98 -106
		mu 0 3 82 74 76;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_3_10mm_flat_na" -p "dipBrushes";
	rename -uid "F74B5179-7545-F0DC-90C2-15AB7EB9DA30";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 16.25 ;
	setAttr -k on ".sfBrushWidth" 1.2;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_3_10mm_flat_naShape" -p "bdx_3_10mm_flat_na";
	rename -uid "F44F6269-2E49-F962-4A40-BB998B8D324D";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.30000016 0.17320506 -1 -0.29999992 0.17320511 -1
		 -0.60000002 2.9802322e-08 -1 -0.30000007 -0.17320508 -1 0.29999998 -0.17320509 -1
		 0.60000002 -4.4408922e-17 -1 0.30000016 0.17320506 0.75 -0.29999992 0.17320511 0.75
		 -0.60000002 2.9802322e-08 0.75 -0.30000007 -0.17320508 0.75 0.29999998 -0.17320509 0.75
		 0.60000002 4.4408922e-17 0.75 0 -4.4408922e-17 -1 0 4.4408922e-17 0.75 0.35355335 0.35355335 -16.25
		 0 0.49999994 -16.25 -0.35355335 0.35355335 -16.25 -0.49999994 -1.6930901e-15 -16.25
		 -0.35355335 -0.35355335 -16.25 0 -0.49999997 -16.25 0.35355338 -0.35355338 -16.25
		 0.5 -1.6930901e-15 -16.25 0.35355335 0.35355335 -1 0 0.49999994 -1 -0.35355335 0.35355335 -1
		 -0.49999994 1.6930901e-15 -1 -0.35355335 -0.35355335 -1 0 -0.49999997 -1 0.35355338 -0.35355338 -1
		 0.5 1.6930901e-15 -1 0 -1.6930901e-15 -16.25 0 1.6930901e-15 -1;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_4_13mm_flat_na" -p "dipBrushes";
	rename -uid "EACBB307-8C4D-EFEF-534F-D48A02931322";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.099999999999998 ;
	setAttr -k on ".sfBrushWidth" 1.5;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_4_13mm_flat_naShape" -p "bdx_4_13mm_flat_na";
	rename -uid "43806F29-F343-7E03-AAE6-DEB481F59170";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.37500018 0.17320506 -1.10000002 -0.37499988 0.17320511 -1.10000002
		 -0.75 2.9802322e-08 -1.10000002 -0.37500009 -0.17320508 -1.10000002 0.37499997 -0.17320509 -1.10000002
		 0.75 -4.4408922e-17 -1.10000002 0.37500018 0.17320506 0.79999995 -0.37499988 0.17320511 0.79999995
		 -0.75 2.9802322e-08 0.79999995 -0.37500009 -0.17320508 0.79999995 0.37499997 -0.17320509 0.79999995
		 0.75 4.4408922e-17 0.79999995 0 -4.4408922e-17 -1.10000002 0 4.4408922e-17 0.79999995
		 0.35355335 0.35355335 -17.10000038 0 0.49999994 -17.10000038 -0.35355335 0.35355335 -17.10000038
		 -0.49999994 -1.7763568e-15 -17.10000038 -0.35355335 -0.35355335 -17.10000038 0 -0.49999997 -17.10000038
		 0.35355338 -0.35355338 -17.10000038 0.5 -1.7763568e-15 -17.10000038 0.35355335 0.35355335 -1.10000038
		 0 0.49999994 -1.10000038 -0.35355335 0.35355335 -1.10000038 -0.49999994 1.7763568e-15 -1.10000038
		 -0.35355335 -0.35355335 -1.10000038 0 -0.49999997 -1.10000038 0.35355338 -0.35355338 -1.10000038
		 0.5 1.7763568e-15 -1.10000038 0 -1.7763568e-15 -17.10000038 0 1.7763568e-15 -1.10000038;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_5_15mm_flat_na" -p "dipBrushes";
	rename -uid "B6E315F6-1042-BDB0-C80B-66944356F286";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.849999999999998 ;
	setAttr -k on ".sfBrushWidth" 1.75;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_5_15mm_flat_naShape" -p "bdx_5_15mm_flat_na";
	rename -uid "6A8698EE-424B-13EB-1C58-40856A0A317A";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.43750021 0.17320506 -1.4000001 -0.43749988 0.17320511 -1.4000001
		 -0.875 2.9802322e-08 -1.4000001 -0.43750012 -0.17320508 -1.4000001 0.43749997 -0.17320509 -1.4000001
		 0.875 -4.4408922e-17 -1.4000001 0.43750021 0.17320506 0.80000001 -0.43749988 0.17320511 0.80000001
		 -0.875 2.9802322e-08 0.80000001 -0.43750012 -0.17320508 0.80000001 0.43749997 -0.17320509 0.80000001
		 0.875 4.4408922e-17 0.80000001 0 -4.4408922e-17 -1.4000001 0 4.4408922e-17 0.80000001
		 0.35355335 0.35355335 -17.85000038 0 0.49999994 -17.85000038 -0.35355335 0.35355335 -17.85000038
		 -0.49999994 -1.826317e-15 -17.85000038 -0.35355335 -0.35355335 -17.85000038 0 -0.49999997 -17.85000038
		 0.35355338 -0.35355338 -17.85000038 0.5 -1.826317e-15 -17.85000038 0.35355335 0.35355335 -1.39999962
		 0 0.49999994 -1.39999962 -0.35355335 0.35355335 -1.39999962 -0.49999994 1.826317e-15 -1.39999962
		 -0.35355335 -0.35355335 -1.39999962 0 -0.49999997 -1.39999962 0.35355338 -0.35355338 -1.39999962
		 0.5 1.826317e-15 -1.39999962 0 -1.826317e-15 -17.85000038 0 1.826317e-15 -1.39999962;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_6_17mm_flat_na" -p "dipBrushes";
	rename -uid "AE149C16-DE49-CD76-211A-8B98A56AEC35";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 18 ;
	setAttr -k on ".sfBrushWidth" 2.2;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_6_17mm_flat_naShape" -p "bdx_6_17mm_flat_na";
	rename -uid "4F9F54D8-8C44-326C-7076-4B9C739EEE4A";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.55000025 0.17320506 -1.45000005 -0.54999983 0.17320511 -1.45000005
		 -1.10000002 2.9802322e-08 -1.45000005 -0.55000013 -0.17320508 -1.45000005 0.54999995 -0.17320509 -1.45000005
		 1.10000002 -4.4408922e-17 -1.45000005 0.55000025 0.17320506 1 -0.54999983 0.17320511 1
		 -1.10000002 2.9802322e-08 1 -0.55000013 -0.17320508 1 0.54999995 -0.17320509 1 1.10000002 4.4408922e-17 1
		 0 -4.4408922e-17 -1.45000005 0 4.4408922e-17 1 0.35355335 0.35355335 -18 0 0.49999994 -18
		 -0.35355335 0.35355335 -18 -0.49999994 -1.837419e-15 -18 -0.35355335 -0.35355335 -18
		 0 -0.49999997 -18 0.35355338 -0.35355338 -18 0.5 -1.837419e-15 -18 0.35355335 0.35355335 -1.45000076
		 0 0.49999994 -1.45000076 -0.35355335 0.35355335 -1.45000076 -0.49999994 1.837419e-15 -1.45000076
		 -0.35355335 -0.35355335 -1.45000076 0 -0.49999997 -1.45000076 0.35355338 -0.35355338 -1.45000076
		 0.5 1.837419e-15 -1.45000076 0 -1.837419e-15 -18 0 1.837419e-15 -1.45000076;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_7_20mm_flat_na" -p "dipBrushes";
	rename -uid "675A4837-8D4D-1BAB-83BB-DCA55EA2BE0A";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.400000000000002 ;
	setAttr -k on ".sfBrushWidth" 2.3;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_7_20mm_flat_naShape" -p "bdx_7_20mm_flat_na";
	rename -uid "D74164AC-2441-DD46-E2CF-5BADD4AFE7AF";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.57500029 0.17320506 -1.70000005 -0.57499981 0.17320511 -1.70000005
		 -1.14999998 2.9802322e-08 -1.70000005 -0.57500011 -0.17320508 -1.70000005 0.57499993 -0.17320509 -1.70000005
		 1.14999998 -4.4408922e-17 -1.70000005 0.57500029 0.17320506 1.20000005 -0.57499981 0.17320511 1.20000005
		 -1.14999998 2.9802322e-08 1.20000005 -0.57500011 -0.17320508 1.20000005 0.57499993 -0.17320509 1.20000005
		 1.14999998 4.4408922e-17 1.20000005 0 -4.4408922e-17 -1.70000005 0 4.4408922e-17 1.20000005
		 0.35355335 0.35355335 -17.39999962 0 0.49999994 -17.39999962 -0.35355335 0.35355335 -17.39999962
		 -0.49999994 -1.7430501e-15 -17.39999962 -0.35355335 -0.35355335 -17.39999962 0 -0.49999997 -17.39999962
		 0.35355338 -0.35355338 -17.39999962 0.5 -1.7430501e-15 -17.39999962 0.35355335 0.35355335 -1.70000029
		 0 0.49999994 -1.70000029 -0.35355335 0.35355335 -1.70000029 -0.49999994 1.7430501e-15 -1.70000029
		 -0.35355335 -0.35355335 -1.70000029 0 -0.49999997 -1.70000029 0.35355338 -0.35355338 -1.70000029
		 0.5 1.7430501e-15 -1.70000029 0 -1.7430501e-15 -17.39999962 0 1.7430501e-15 -1.70000029;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_8_28mm_flat_na" -p "dipBrushes";
	rename -uid "597C2E90-CB47-D3DB-5AF2-44B1A9F1A875";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 16.9 ;
	setAttr -k on ".sfBrushWidth" 3;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_8_28mm_flat_naShape" -p "bdx_8_28mm_flat_na";
	rename -uid "CF4F2CB6-7D4B-8AB2-5D2A-7586A3ED470C";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  0.75000036 0.17320506 -1.60000002 -0.74999976 0.17320511 -1.60000002
		 -1.5 2.9802322e-08 -1.60000002 -0.75000018 -0.17320508 -1.60000002 0.74999994 -0.17320509 -1.60000002
		 1.5 -4.4408922e-17 -1.60000002 0.75000036 0.17320506 1.10000002 -0.74999976 0.17320511 1.10000002
		 -1.5 2.9802322e-08 1.10000002 -0.75000018 -0.17320508 1.10000002 0.74999994 -0.17320509 1.10000002
		 1.5 4.4408922e-17 1.10000002 0 -4.4408922e-17 -1.60000002 0 4.4408922e-17 1.10000002
		 0.35355335 0.35355335 -16.89999962 0 0.49999994 -16.89999962 -0.35355335 0.35355335 -16.89999962
		 -0.49999994 -1.6986412e-15 -16.89999962 -0.35355335 -0.35355335 -16.89999962 0 -0.49999997 -16.89999962
		 0.35355338 -0.35355338 -16.89999962 0.5 -1.6986412e-15 -16.89999962 0.35355335 0.35355335 -1.5999999
		 0 0.49999994 -1.5999999 -0.35355335 0.35355335 -1.5999999 -0.49999994 1.6986412e-15 -1.5999999
		 -0.35355335 -0.35355335 -1.5999999 0 -0.49999997 -1.5999999 0.35355338 -0.35355338 -1.5999999
		 0.5 1.6986412e-15 -1.5999999 0 -1.6986412e-15 -16.89999962 0 1.6986412e-15 -1.5999999;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_9_35mm_flat_na" -p "dipBrushes";
	rename -uid "18935628-A648-EBD0-281C-66920DDE9FA6";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 16.849999999999998 ;
	setAttr -k on ".sfBrushWidth" 4;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_9_35mm_flat_naShape" -p "bdx_9_35mm_flat_na";
	rename -uid "09011220-A543-DAC4-2CD1-57ABB3478C80";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  1.000000476837 0.17320506 -1.79999995 -0.9999997 0.17320511 -1.79999995
		 -2 2.9802322e-08 -1.79999995 -1.000000238419 -0.17320508 -1.79999995 0.99999994 -0.17320509 -1.79999995
		 2 -4.4408922e-17 -1.79999995 1.000000476837 0.17320506 1.29999995 -0.9999997 0.17320511 1.29999995
		 -2 2.9802322e-08 1.29999995 -1.000000238419 -0.17320508 1.29999995 0.99999994 -0.17320509 1.29999995
		 2 4.4408922e-17 1.29999995 0 -4.4408922e-17 -1.79999995 0 4.4408922e-17 1.29999995
		 0.35355335 0.35355335 -16.85000038 0 0.49999994 -16.85000038 -0.35355335 0.35355335 -16.85000038
		 -0.49999994 -1.6708857e-15 -16.85000038 -0.35355335 -0.35355335 -16.85000038 0 -0.49999997 -16.85000038
		 0.35355338 -0.35355338 -16.85000038 0.5 -1.6708857e-15 -16.85000038 0.35355335 0.35355335 -1.79999971
		 0 0.49999994 -1.79999971 -0.35355335 0.35355335 -1.79999971 -0.49999994 1.6708857e-15 -1.79999971
		 -0.35355335 -0.35355335 -1.79999971 0 -0.49999997 -1.79999971 0.35355338 -0.35355338 -1.79999971
		 0.5 1.6708857e-15 -1.79999971 0 -1.6708857e-15 -16.85000038 0 1.6708857e-15 -1.79999971;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_10_40mm_flat_na" -p "dipBrushes";
	rename -uid "AC850E94-4449-25CF-1F1F-788C6EBD9693";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 17.3 ;
	setAttr -k on ".sfBrushWidth" 4.5;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_10_40mm_flat_naShape" -p "bdx_10_40mm_flat_na";
	rename -uid "390FF17E-444C-E42A-6AAA-77834700967F";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  1.12500048 0.17320506 -1.85000002 -1.12499964 0.17320511 -1.85000002
		 -2.25 2.9802322e-08 -1.85000002 -1.12500024 -0.17320508 -1.85000002 1.12499988 -0.17320509 -1.85000002
		 2.25 -4.4408922e-17 -1.85000002 1.12500048 0.17320506 1.30000007 -1.12499964 0.17320511 1.30000007
		 -2.25 2.9802322e-08 1.30000007 -1.12500024 -0.17320508 1.30000007 1.12499988 -0.17320509 1.30000007
		 2.25 4.4408922e-17 1.30000007 0 -4.4408922e-17 -1.85000002 0 4.4408922e-17 1.30000007
		 0.35355335 0.35355335 -17.29999924 0 0.49999994 -17.29999924 -0.35355335 0.35355335 -17.29999924
		 -0.49999994 -1.7152946e-15 -17.29999924 -0.35355335 -0.35355335 -17.29999924 0 -0.49999997 -17.29999924
		 0.35355338 -0.35355338 -17.29999924 0.5 -1.7152946e-15 -17.29999924 0.35355335 0.35355335 -1.8499999
		 0 0.49999994 -1.8499999 -0.35355335 0.35355335 -1.8499999 -0.49999994 1.7152946e-15 -1.8499999
		 -0.35355335 -0.35355335 -1.8499999 0 -0.49999997 -1.8499999 0.35355338 -0.35355338 -1.8499999
		 0.5 1.7152946e-15 -1.8499999 0 -1.7152946e-15 -17.29999924 0 1.7152946e-15 -1.8499999;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_11_52mm_flat_na" -p "dipBrushes";
	rename -uid "BEA256DC-6146-908E-C265-2AB37371B503";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 18.3 ;
	setAttr -k on ".sfBrushWidth" 5.5;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_11_52mm_flat_naShape" -p "bdx_11_52mm_flat_na";
	rename -uid "B9A0B161-1A4B-0634-CD88-ED9FE257553A";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  1.37500072 0.17320506 -2.69999981 -1.37499964 0.17320511 -2.69999981
		 -2.75 2.9802322e-08 -2.69999981 -1.37500036 -0.17320508 -2.69999981 1.37499988 -0.17320509 -2.69999981
		 2.75 -4.4408922e-17 -2.69999981 1.37500072 0.17320506 1.49999988 -1.37499964 0.17320511 1.49999988
		 -2.75 2.9802322e-08 1.49999988 -1.37500036 -0.17320508 1.49999988 1.37499988 -0.17320509 1.49999988
		 2.75 4.4408922e-17 1.49999988 0 -4.4408922e-17 -2.69999981 0 4.4408922e-17 1.49999988
		 0.35355335 0.35355335 -18.29999924 0 0.49999994 -18.29999924 -0.35355335 0.35355335 -18.29999924
		 -0.49999994 -1.731948e-15 -18.29999924 -0.35355335 -0.35355335 -18.29999924 0 -0.49999997 -18.29999924
		 0.35355338 -0.35355338 -18.29999924 0.5 -1.731948e-15 -18.29999924 0.35355335 0.35355335 -2.69999981
		 0 0.49999994 -2.69999981 -0.35355335 0.35355335 -2.69999981 -0.49999994 1.731948e-15 -2.69999981
		 -0.35355335 -0.35355335 -2.69999981 0 -0.49999997 -2.69999981 0.35355338 -0.35355338 -2.69999981
		 0.5 1.731948e-15 -2.69999981 0 -1.731948e-15 -18.29999924 0 1.731948e-15 -2.69999981;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "bdx_12_78mm_flat_na" -p "dipBrushes";
	rename -uid "C57EB1AD-8646-3514-5C9D-56BF2A0A0D2D";
	addAttr -ci true -sn "sfBrushWidth" -ln "sfBrushWidth" -at "double";
	addAttr -ci true -sn "sfBrushShape" -ln "sfBrushShape" -min 0 -max 1 -en "flat:round" 
		-at "enum";
	addAttr -ci true -sn "sfBrushRetention" -ln "sfBrushRetention" -at "double";
	setAttr ".t" -type "double3" 0 0 18.599999999999998 ;
	setAttr -k on ".sfBrushWidth" 8;
	setAttr -k on ".sfBrushShape";
	setAttr -k on ".sfBrushRetention" 1000;
createNode mesh -n "bdx_12_78mm_flat_naShape" -p "bdx_12_78mm_flat_na";
	rename -uid "99F4C0C2-3D4F-D7DC-9748-12AC2EC624BC";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:41]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 64 ".uvst[0].uvsp[0:63]" -type "float2" 0.75187981 0.016746834
		 0.25187975 0.016746812 0.0018796921 0.12499999 0.25187963 0.23325318 0.75187969 0.23325318
		 1.0018796921 0.125 0 0.25 0.16666667 0.25 0.33333334 0.25 0.5 0.25 0.66666669 0.25
		 0.83333337 0.25 1 0.25 0 0.75 0.16666667 0.75 0.33333334 0.75 0.5 0.75 0.66666669
		 0.75 0.83333337 0.75 1 0.75 0.75187981 0.76674682 0.25187975 0.76674682 0.0018796921
		 0.875 0.25187963 0.98325318 0.75187969 0.98325318 1.0018796921 0.875 0.50187969 0.125
		 0.50187969 0.875 0 0.25 0.125 0.25 0.125 0.75 0 0.75 0.25 0.25 0.25 0.75 0.375 0.25
		 0.375 0.75 0.5 0.25 0.5 0.75 0.625 0.25 0.625 0.75 0.75 0.25 0.75 0.75 0.875 0.25
		 0.875 0.75 1 0.25 1 0.75 0.50187969 1.4901161e-08 0.85543305 0.036611661 0.50187969
		 0.125 0.14832634 0.036611661 0.0018797517 0.125 0.14832634 0.21338834 0.50187969
		 0.25 0.85543311 0.21338835 1.0018796921 0.125 0.85543311 0.96338832 0.50187969 1
		 0.50187969 0.875 0.14832634 0.96338832 0.0018797517 0.875 0.14832634 0.78661168 0.50187969
		 0.75 0.85543305 0.78661168 1.0018796921 0.875;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  2.000000953674 0.17320506 -3.20000005 -1.9999994 0.17320511 -3.20000005
		 -4 2.9802322e-08 -3.20000005 -2.000000476837 -0.17320508 -3.20000005 1.99999988 -0.17320509 -3.20000005
		 4 -4.4408922e-17 -3.20000005 2.000000953674 0.17320506 1.60000014 -1.9999994 0.17320511 1.60000014
		 -4 2.9802322e-08 1.60000014 -2.000000476837 -0.17320508 1.60000014 1.99999988 -0.17320509 1.60000014
		 4 4.4408922e-17 1.60000014 0 -4.4408922e-17 -3.20000005 0 4.4408922e-17 1.60000014
		 0.35355335 0.35355335 -18.59999847 0 0.49999994 -18.59999847 -0.35355335 0.35355335 -18.59999847
		 -0.49999994 -1.7097434e-15 -18.59999847 -0.35355335 -0.35355335 -18.59999847 0 -0.49999997 -18.59999847
		 0.35355338 -0.35355338 -18.59999847 0.5 -1.7097434e-15 -18.59999847 0.35355335 0.35355335 -3.19999981
		 0 0.49999994 -3.19999981 -0.35355335 0.35355335 -3.19999981 -0.49999994 1.7097434e-15 -3.19999981
		 -0.35355335 -0.35355335 -3.19999981 0 -0.49999997 -3.19999981 0.35355338 -0.35355338 -3.19999981
		 0.5 1.7097434e-15 -3.19999981 0 -1.7097434e-15 -18.59999847 0 1.7097434e-15 -3.19999981;
	setAttr -s 70 ".ed[0:69]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 0 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 6 0 0 6 0 1 7 0 2 8 0 3 9 0 4 10 0 5 11 0 12 0 1 12 1 1
		 12 2 1 12 3 1 12 4 1 12 5 1 6 13 1 7 13 1 8 13 1 9 13 1 10 13 1 11 13 1 14 15 0 15 16 0
		 16 17 0 17 18 0 18 19 0 19 20 0 20 21 0 21 14 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0
		 27 28 0 28 29 0 29 22 0 14 22 0 15 23 0 16 24 0 17 25 0 18 26 0 19 27 0 20 28 0 21 29 0
		 30 14 1 30 15 1 30 16 1 30 17 1 30 18 1 30 19 1 30 20 1 30 21 1 22 31 1 23 31 1 24 31 1
		 25 31 1 26 31 1 27 31 1 28 31 1 29 31 1;
	setAttr -s 42 -ch 140 ".fc[0:41]" -type "polyFaces" 
		f 4 0 13 -7 -13
		mu 0 4 6 7 14 13
		f 4 1 14 -8 -14
		mu 0 4 7 8 15 14
		f 4 2 15 -9 -15
		mu 0 4 8 9 16 15
		f 4 3 16 -10 -16
		mu 0 4 9 10 17 16
		f 4 4 17 -11 -17
		mu 0 4 10 11 18 17
		f 4 5 12 -12 -18
		mu 0 4 11 12 19 18
		f 3 -1 -19 19
		mu 0 3 1 0 26
		f 3 -2 -20 20
		mu 0 3 2 1 26
		f 3 -3 -21 21
		mu 0 3 3 2 26
		f 3 -4 -22 22
		mu 0 3 4 3 26
		f 3 -5 -23 23
		mu 0 3 5 4 26
		f 3 -6 -24 18
		mu 0 3 0 5 26
		f 3 6 25 -25
		mu 0 3 24 23 27
		f 3 7 26 -26
		mu 0 3 23 22 27
		f 3 8 27 -27
		mu 0 3 22 21 27
		f 3 9 28 -28
		mu 0 3 21 20 27
		f 3 10 29 -29
		mu 0 3 20 25 27
		f 3 11 24 -30
		mu 0 3 25 24 27
		f 4 30 47 -39 -47
		mu 0 4 28 29 30 31
		f 4 31 48 -40 -48
		mu 0 4 29 32 33 30
		f 4 32 49 -41 -49
		mu 0 4 32 34 35 33
		f 4 33 50 -42 -50
		mu 0 4 34 36 37 35
		f 4 34 51 -43 -51
		mu 0 4 36 38 39 37
		f 4 35 52 -44 -52
		mu 0 4 38 40 41 39
		f 4 36 53 -45 -53
		mu 0 4 40 42 43 41
		f 4 37 46 -46 -54
		mu 0 4 42 44 45 43
		f 3 -31 -55 55
		mu 0 3 46 47 48
		f 3 -32 -56 56
		mu 0 3 49 46 48
		f 3 -33 -57 57
		mu 0 3 50 49 48
		f 3 -34 -58 58
		mu 0 3 51 50 48
		f 3 -35 -59 59
		mu 0 3 52 51 48
		f 3 -36 -60 60
		mu 0 3 53 52 48
		f 3 -37 -61 61
		mu 0 3 54 53 48
		f 3 -38 -62 54
		mu 0 3 47 54 48
		f 3 38 63 -63
		mu 0 3 55 56 57
		f 3 39 64 -64
		mu 0 3 56 58 57
		f 3 40 65 -65
		mu 0 3 58 59 57
		f 3 41 66 -66
		mu 0 3 59 60 57
		f 3 42 67 -67
		mu 0 3 60 61 57
		f 3 43 68 -68
		mu 0 3 61 62 57
		f 3 44 69 -69
		mu 0 3 62 63 57
		f 3 45 62 -70
		mu 0 3 63 55 57;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "dipCurves" -p "brushes";
	rename -uid "E8C6814E-1649-07E8-4E19-B78D09AAAD1B";
	setAttr -k off ".rx";
	setAttr -k off ".ry";
	setAttr -k off ".rz";
	setAttr -k on ".rptx";
	setAttr -k on ".rpty";
	setAttr -k on ".rptz";
createNode transform -n "defaultSource" -p "dipCurves";
	rename -uid "DB2594C4-F843-8F7C-BE16-C998109B7E29";
createNode transform -n "sourceDip1" -p "defaultSource";
	rename -uid "B07FD34F-AE4F-9DF1-6B95-F1BD2EB97F98";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|defaultSource|sourceDip1";
	rename -uid "BB793A22-9A43-FBF6-78F8-968022DE5457";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "defaultSource";
	rename -uid "E63B5F36-804F-2A4F-ECE4-4EAA3665EBF9";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|defaultSource|sourceDip2";
	rename -uid "2FFAB2D9-774D-03FB-CC36-EA8B0891B13E";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "defaultSource";
	rename -uid "DCCA969C-0447-6D9B-1897-D0A9482EC700";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|defaultSource|sourceDip3";
	rename -uid "8B406BC9-C549-6AB8-CDE8-B2AB94716080";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "defaultSource";
	rename -uid "EBCA9B04-244D-9998-9CF2-B1B71E28DCFF";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|defaultSource|sourceDip4";
	rename -uid "A4E5EDFB-5342-5C22-3B09-E7993866F9AF";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_0_4mm_round_soft" -p "dipCurves";
	rename -uid "3F8737C8-D841-EA1F-6456-B3BC61EBDEBD";
createNode transform -n "sourceDip1" -p "bdcx_0_4mm_round_soft";
	rename -uid "FFCBECD4-C447-083F-0C87-579456A47549";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_0_4mm_round_soft|sourceDip1";
	rename -uid "5E3662D4-A145-7291-5227-7AA2EA93F1AE";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_0_4mm_round_soft";
	rename -uid "70183F2B-FB47-4A3C-293E-148FD0643E2C";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_0_4mm_round_soft|sourceDip2";
	rename -uid "6C6EA5A5-714B-B429-B491-588B5CA21832";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_0_4mm_round_soft";
	rename -uid "09E009B0-8845-DAB4-905D-3EBDD813E17D";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_0_4mm_round_soft|sourceDip3";
	rename -uid "8E18A1D5-4C49-BCF1-CF61-9BA437686FE1";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_0_4mm_round_soft";
	rename -uid "2D95D419-9845-19A8-657E-DE8DAFA8F906";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_0_4mm_round_soft|sourceDip4";
	rename -uid "34D70C1C-3E40-9F88-1185-49B66933CB16";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_1_6mm_round_syn" -p "dipCurves";
	rename -uid "2E5001E0-F248-3108-680A-598044DA87F2";
createNode transform -n "sourceDip1" -p "bdcx_1_6mm_round_syn";
	rename -uid "C4D2D828-454A-0267-E970-AFAE9853A9A2";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_1_6mm_round_syn|sourceDip1";
	rename -uid "23332FFB-DE47-26C5-4A80-99B7A063BA30";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_1_6mm_round_syn";
	rename -uid "1A178CDB-F74D-F415-B53F-9F8CAD35C28E";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_1_6mm_round_syn|sourceDip2";
	rename -uid "688F636F-7B4B-CAC5-4438-08978923E086";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_1_6mm_round_syn";
	rename -uid "3076FDE8-6244-F140-95FF-BB93C08E0AAE";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_1_6mm_round_syn|sourceDip3";
	rename -uid "32793C56-574D-B260-B52B-84B836618402";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_1_6mm_round_syn";
	rename -uid "931D734B-4B42-5858-FA5C-6A9DAF1E8180";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_1_6mm_round_syn|sourceDip4";
	rename -uid "18C517E8-1F4F-3350-159D-2F93C207CD4F";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_2_8mm_round_soft" -p "dipCurves";
	rename -uid "0ADDA90F-5743-D857-C124-8DBCA01161FF";
createNode transform -n "sourceDip1" -p "bdcx_2_8mm_round_soft";
	rename -uid "35C27E29-084D-C53A-0306-9C8FCA27BD42";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_2_8mm_round_soft|sourceDip1";
	rename -uid "87ECAC4F-5E46-7E5B-DD90-008BA03E4D6D";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_2_8mm_round_soft";
	rename -uid "6998A718-8346-6F70-A273-649B433F97EB";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_2_8mm_round_soft|sourceDip2";
	rename -uid "EF8A2E2F-1444-60F6-0E30-45959BE703DF";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_2_8mm_round_soft";
	rename -uid "18906B3B-7A43-A97A-6D03-92A80F1E763F";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_2_8mm_round_soft|sourceDip3";
	rename -uid "D7C684A9-C34F-40E1-74E8-D485321E4F8B";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_2_8mm_round_soft";
	rename -uid "B198BBDF-ED46-4904-40CD-328EBFFBE01F";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_2_8mm_round_soft|sourceDip4";
	rename -uid "B4F44D52-E14A-9854-24A5-638D1B9B6CFF";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_3_10mm_flat_na" -p "dipCurves";
	rename -uid "10389E49-9041-B244-C6D3-24BAAF7BCFD8";
createNode transform -n "sourceDip1" -p "bdcx_3_10mm_flat_na";
	rename -uid "82CC51AE-8244-4321-A39F-549578DD9F28";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_3_10mm_flat_na|sourceDip1";
	rename -uid "36F3297C-AB46-5F52-1141-509A5D7F135B";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_3_10mm_flat_na";
	rename -uid "B95EFDF6-2445-98C4-4234-8DA9CA14C304";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_3_10mm_flat_na|sourceDip2";
	rename -uid "4FA2176D-914A-9BD9-A757-4AAF4EC20FD7";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_3_10mm_flat_na";
	rename -uid "E2E94243-6C40-81BB-5406-58B9A8405EF6";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_3_10mm_flat_na|sourceDip3";
	rename -uid "98F1E82C-BD48-25F3-27EC-7B89E36EA85A";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_3_10mm_flat_na";
	rename -uid "FA1F291F-CB45-8002-19C4-72A301DCA29E";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_3_10mm_flat_na|sourceDip4";
	rename -uid "12AE3AD7-9E42-9B2C-1FFD-C5ABB391B2FB";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_4_13mm_flat_na" -p "dipCurves";
	rename -uid "F74D1115-D44B-581C-50B4-408CBAB30F19";
createNode transform -n "sourceDip1" -p "bdcx_4_13mm_flat_na";
	rename -uid "CBC8EAC6-8741-8E5B-69A2-ADBB3516418F";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_4_13mm_flat_na|sourceDip1";
	rename -uid "CB02D773-924A-0D47-91F1-80980A795296";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_4_13mm_flat_na";
	rename -uid "732422CA-074E-F4CD-2930-A6B9D69F06E0";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_4_13mm_flat_na|sourceDip2";
	rename -uid "BBB87AA4-CE45-7B71-BBB4-2AAD4754EA6F";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_4_13mm_flat_na";
	rename -uid "CFB361E1-354E-E2EA-FEBE-DD81122137D4";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_4_13mm_flat_na|sourceDip3";
	rename -uid "B556A52E-1B47-F344-2C3B-9B9CEF1144AF";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_4_13mm_flat_na";
	rename -uid "759F19E7-D64B-DCDA-2084-5F962095DCFF";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_4_13mm_flat_na|sourceDip4";
	rename -uid "7D5959C8-5C4D-D457-FAC5-00B5C824B2FB";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_5_15mm_flat_na" -p "dipCurves";
	rename -uid "2197955B-F244-37ED-46B3-668BBA6124B0";
createNode transform -n "sourceDip1" -p "bdcx_5_15mm_flat_na";
	rename -uid "A1680418-8D45-0F7E-8D3B-BEBC00F88C47";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_5_15mm_flat_na|sourceDip1";
	rename -uid "F5F9E773-9E41-540B-2924-0BB031E20E4B";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_5_15mm_flat_na";
	rename -uid "DAA13D5F-434C-8EC3-FD02-6D9638892352";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_5_15mm_flat_na|sourceDip2";
	rename -uid "FB84FA48-5341-7463-F2A8-3E8D2B8D2B1D";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_5_15mm_flat_na";
	rename -uid "0571F29C-8C4A-4FD5-6AAB-C6A12E6C9352";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_5_15mm_flat_na|sourceDip3";
	rename -uid "1AFE428D-634B-5734-DF50-21A039DCDE48";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_5_15mm_flat_na";
	rename -uid "EFDA1AA7-DB49-3E58-DDBF-BF947CBC7927";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_5_15mm_flat_na|sourceDip4";
	rename -uid "EB0B4CD9-8945-581C-75D3-549D2D9E5199";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_6_17mm_flat_na" -p "dipCurves";
	rename -uid "FDA73BC3-1746-BBB6-AFA3-57B2226638D2";
createNode transform -n "sourceDip1" -p "bdcx_6_17mm_flat_na";
	rename -uid "E242B44A-384B-FE40-6794-8996C9E8A982";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_6_17mm_flat_na|sourceDip1";
	rename -uid "B2C808C1-B24B-D5F9-7B52-CF9A04C67047";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_6_17mm_flat_na";
	rename -uid "0209DBBB-964F-D27F-5C23-ED920EC973B2";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_6_17mm_flat_na|sourceDip2";
	rename -uid "4CC5F5BD-9B42-8F37-FB4B-D58277C03314";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_6_17mm_flat_na";
	rename -uid "6AE28F36-9445-F755-61DA-FEAF55768E66";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_6_17mm_flat_na|sourceDip3";
	rename -uid "89B0B41D-1A4E-575E-8C75-D4973A932107";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_6_17mm_flat_na";
	rename -uid "DCFA1B0B-B043-A1CB-8BB4-C2B25717E0BC";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_6_17mm_flat_na|sourceDip4";
	rename -uid "0AC36B44-1745-0758-BFFC-539D7564037B";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_7_20mm_flat_na" -p "dipCurves";
	rename -uid "CA05B2BE-4448-F2B2-DA1E-F2A2AF39138F";
createNode transform -n "sourceDip1" -p "bdcx_7_20mm_flat_na";
	rename -uid "83249494-864E-8864-53EA-27A59576AF9D";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_7_20mm_flat_na|sourceDip1";
	rename -uid "177B22DC-7B4E-4973-0095-DEBA5C957835";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_7_20mm_flat_na";
	rename -uid "C6846B86-254D-18BD-7B05-99884F032B24";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_7_20mm_flat_na|sourceDip2";
	rename -uid "8B9D9A91-E948-A5E7-098D-A5841C18ABF6";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_7_20mm_flat_na";
	rename -uid "95C23507-674A-9D1B-70E5-DF9890288C3D";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_7_20mm_flat_na|sourceDip3";
	rename -uid "97CCCC2D-EE4A-2661-45FD-41A80B9FF224";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_7_20mm_flat_na";
	rename -uid "2C33A3AF-C246-7C7F-FF88-8ABB52C59311";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_7_20mm_flat_na|sourceDip4";
	rename -uid "1C258B4F-B44E-0880-C316-209C6BA35A54";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_8_28mm_flat_na" -p "dipCurves";
	rename -uid "468D924F-9647-CAD2-10EC-27B890A5BC2C";
createNode transform -n "sourceDip1" -p "bdcx_8_28mm_flat_na";
	rename -uid "9B22E208-5D43-013B-9F04-A59EFDD002B4";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_8_28mm_flat_na|sourceDip1";
	rename -uid "8C38223E-AE4F-3B16-CB3F-76920FBCCCFC";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_8_28mm_flat_na";
	rename -uid "AC05E8E8-BA44-3714-0410-2E838F161FBB";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_8_28mm_flat_na|sourceDip2";
	rename -uid "D16E8B23-704C-8AC5-B248-87A0679FE75C";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_8_28mm_flat_na";
	rename -uid "81289D8A-164D-635D-57A1-7497A40091B4";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_8_28mm_flat_na|sourceDip3";
	rename -uid "028A77E4-6B4F-1C49-0893-0B8D73AE91F6";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_8_28mm_flat_na";
	rename -uid "4884AB15-474D-1279-3109-C78F7362F75A";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_8_28mm_flat_na|sourceDip4";
	rename -uid "9AF59708-B24C-AA32-C4E7-4C830E6EC8A6";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_9_35mm_flat_na" -p "dipCurves";
	rename -uid "E85BB28F-9447-5C2A-574C-D0B41A0B40E6";
createNode transform -n "sourceDip1" -p "bdcx_9_35mm_flat_na";
	rename -uid "36485479-C94E-3753-324C-9286E5325350";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_9_35mm_flat_na|sourceDip1";
	rename -uid "EA5C661D-6A41-6EF8-482D-73ACD1C6CA64";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_9_35mm_flat_na";
	rename -uid "8C6F5F3E-D142-8B50-0951-07A38A7A9263";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_9_35mm_flat_na|sourceDip2";
	rename -uid "6417FBFF-BA4E-3115-5474-649E2171F672";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_9_35mm_flat_na";
	rename -uid "BB9062B8-034A-DC29-49FA-22BC5BD7A3E3";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_9_35mm_flat_na|sourceDip3";
	rename -uid "60F09FD3-1B43-FE35-4B73-A9A1FB9A92C0";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_9_35mm_flat_na";
	rename -uid "80E3C1A3-6D41-7EB4-C611-358EE21171A0";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_9_35mm_flat_na|sourceDip4";
	rename -uid "9105E595-0C42-4179-950E-D38A149D1D56";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_10_40mm_flat_na" -p "dipCurves";
	rename -uid "D98EAA52-D74A-2AC1-D392-039EF24631AA";
createNode transform -n "sourceDip1" -p "bdcx_10_40mm_flat_na";
	rename -uid "0C764E71-924F-7036-FA3D-538A6B7C003D";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_10_40mm_flat_na|sourceDip1";
	rename -uid "101CDA0E-9B45-D89E-94C3-589B26A9708E";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_10_40mm_flat_na";
	rename -uid "E810826F-B444-868D-4A34-00BFC7EA1D9A";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_10_40mm_flat_na|sourceDip2";
	rename -uid "D24D7EE7-8642-C627-874D-249A553C0AC4";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_10_40mm_flat_na";
	rename -uid "9AAD7BAC-E14D-1A4B-5489-44A17D5C1657";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_10_40mm_flat_na|sourceDip3";
	rename -uid "2C97FC47-B045-B4B2-77CE-358D7436680F";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_10_40mm_flat_na";
	rename -uid "333ABF21-2547-E091-387F-278F772142BF";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_10_40mm_flat_na|sourceDip4";
	rename -uid "286933D0-3B4E-4423-8903-8CA054D1EBED";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_11_52mm_flat_na" -p "dipCurves";
	rename -uid "53F46F9C-4047-8D24-0376-FF9DA961399D";
createNode transform -n "sourceDip1" -p "bdcx_11_52mm_flat_na";
	rename -uid "C3DFC7D8-E64C-BE56-706D-FC91B3B5F56E";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_11_52mm_flat_na|sourceDip1";
	rename -uid "7D26E382-CC48-716D-97D0-CCA3591A6D6A";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_11_52mm_flat_na";
	rename -uid "B2DE0243-5844-8C58-9502-10B13BD81B13";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_11_52mm_flat_na|sourceDip2";
	rename -uid "047A5186-E142-B949-7670-1ABDE82A4863";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_11_52mm_flat_na";
	rename -uid "05710266-3042-2650-FC9B-D5803E914815";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_11_52mm_flat_na|sourceDip3";
	rename -uid "421DFD3B-F742-1718-EDE4-3DB1E18DF4C3";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_11_52mm_flat_na";
	rename -uid "3687AB25-CE42-5781-36CA-1FA4259FA7CD";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_11_52mm_flat_na|sourceDip4";
	rename -uid "C845D364-FF4E-8EE2-FA2A-7598622983A3";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "bdcx_12_78mm_flat_na" -p "dipCurves";
	rename -uid "1AB26407-CD44-3D4B-FE9D-9894E9744F44";
createNode transform -n "sourceDip1" -p "bdcx_12_78mm_flat_na";
	rename -uid "D3BF4E0F-A04E-8F75-8DD5-B8AA2E06DBA7";
	setAttr ".s" -type "double3" 0.57777779719266609 1 1 ;
	setAttr ".rp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
	setAttr ".sp" -type "double3" -0.17682373808810325 -0.99445202098388563 -0.39643419041896877 ;
createNode nurbsCurve -n "sourceDipShape1" -p "|brushes|dipCurves|bdcx_12_78mm_flat_na|sourceDip1";
	rename -uid "9AC3F1E4-8F42-7CAA-80AC-41987E570FC1";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 0 no 3
		19 0 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 14 14
		17
		0.18662374405843707 -0.42238855602226977 0.028880363310619828
		-3.6259655314310635 -3.9605034269233101 0.028880363310619828
		-4.4368792980664296 2.9836068217940692 0.028880363310619842
		0.49885268971800656 4.0883324219377641 0.028880363310619828
		4.1220652719092925 1.9885791841077776 0.028880363310619828
		3.4512808223125262 -2.9993257771037918 0.028880363310619828
		1.1615494479372614 -4.218765696103759 0.028880363310619828
		-1.4912358907515255 -3.1320685143655282 0.028880363310619828
		-2.5249400674442768 -0.354189768355468 0.028880363310619828
		-1.2373571574936904 2.0702740859333062 0.028880363310619842
		0.86916070115738364 2.0702740859333062 0.028880363310619842
		2.3903348796019515 -0.96895659372367504 0.028880363310619828
		0.010949721706936089 -5.4773739340924976 0.028880363310619828
		-0.49617494796829931 -7.461576508904983 3.6030266322534779
		-0.57419412791837487 -8.7058383847706295 3.7190639726620756
		-0.41815576801828058 -10.510018104775909 3.5992159476429659
		-0.30112699809320986 -12.500837106160986 3.5566308951707466
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror";
	setAttr -k on ".sfRepeatOscillate";
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" -90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 25;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip2" -p "bdcx_12_78mm_flat_na";
	rename -uid "9DE261A4-A24C-1463-31FC-9087D7A77348";
	setAttr ".t" -type "double3" 0 0 -0.36423231769014119 ;
	setAttr ".rp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.09625183618939559 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape2" -p "|brushes|dipCurves|bdcx_12_78mm_flat_na|sourceDip2";
	rename -uid "41B738C2-D641-E957-3BD7-80A9318275A2";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.36058935231121225 -6.3108214333829693 3.98757389348029
		-0.30521466279598641 -7.0258425361858627 3.98757389348029
		-0.056028559977473691 -8.6397473110838376 3.98757389348029
		0.17931387046223435 -10.784810619492525 3.98757389348029
		0.55309302469000343 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 90;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip3" -p "bdcx_12_78mm_flat_na";
	rename -uid "B0350AAA-BA47-D1C7-ED78-9DA70546003D";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" 0.44130730220924619 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape3" -p "|brushes|dipCurves|bdcx_12_78mm_flat_na|sourceDip3";
	rename -uid "73A2E149-1443-40E9-8F33-7183B268204B";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.015533886291361654 -6.3108214333829693 3.98757389348029
		0.039840803223864185 -7.0258425361858627 3.98757389348029
		0.28902690604237691 -8.6397473110838376 3.98757389348029
		0.52436933648208495 -10.784810619492525 3.98757389348029
		0.89814849070985403 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist";
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "sourceDip4" -p "bdcx_12_78mm_flat_na";
	rename -uid "B0ACC3C6-FF45-E5D8-D250-C58E3FE15367";
	setAttr ".t" -type "double3" 0 1.0357429928825059 -0.36423231769014119 ;
	setAttr ".s" -type "double3" 1 1.374814796694253 1 ;
	setAttr ".rp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
	setAttr ".sp" -type "double3" -0.25150693909026245 -9.2321933676919485 4.4096934319811893 ;
createNode nurbsCurve -n "sourceDipShape4" -p "|brushes|dipCurves|bdcx_12_78mm_flat_na|sourceDip4";
	rename -uid "EE3F5786-D34E-C79D-F8D9-1A9D2A88A1B8";
	addAttr -ci true -sn "sfPointDensity" -ln "sfPointDensity" -at "double";
	addAttr -ci true -sn "sfStrokeLength" -ln "sfStrokeLength" -at "double";
	addAttr -ci true -sn "sfOverlap" -ln "sfOverlap" -at "double";
	addAttr -ci true -sn "sfRandomLengthFactor" -ln "sfRandomLengthFactor" -at "double";
	addAttr -ci true -sn "sfRandomOverlapFactor" -ln "sfRandomOverlapFactor" -at "double";
	addAttr -ci true -sn "sfForceDip" -ln "sfForceDip" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeats" -ln "sfRepeats" -at "short";
	addAttr -ci true -sn "sfRepeatOffset" -ln "sfRepeatOffset" -at "double";
	addAttr -ci true -sn "sfRepeatMirror" -ln "sfRepeatMirror" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatOscillate" -ln "sfRepeatOscillate" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfStrokeRotation" -ln "sfStrokeRotation" -at "doubleAngle";
	addAttr -ci true -sn "sfStrokeTranslation" -ln "sfStrokeTranslation" -at "double";
	addAttr -ci true -sn "sfPivotFraction" -ln "sfPivotFraction" -at "double";
	addAttr -ci true -sn "sfBrushId" -ln "sfBrushId" -at "short";
	addAttr -ci true -sn "sfPaintId" -ln "sfPaintId" -at "short";
	addAttr -ci true -sn "sfBrushRotateTilt" -ln "sfBrushRotateTilt" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateBank" -ln "sfBrushRotateBank" -at "doubleAngle";
	addAttr -ci true -sn "sfBrushRotateTwist" -ln "sfBrushRotateTwist" -at "doubleAngle";
	addAttr -ci true -sn "sfFollowStroke" -ln "sfFollowStroke" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfActive" -ln "sfActive" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "sfRepeatAdvance" -ln "sfRepeatAdvance" -at "double";
	addAttr -ci true -sn "sfSubcurveMin" -ln "sfSubcurveMin" -at "double";
	addAttr -ci true -sn "sfSubcurveMax" -ln "sfSubcurveMax" -at "double";
	addAttr -ci true -sn "sfApproachDistanceStart" -ln "sfApproachDistanceStart" -at "double";
	addAttr -ci true -sn "sfApproachDistanceMid" -ln "sfApproachDistanceMid" -at "double";
	addAttr -ci true -sn "sfApproachDistanceEnd" -ln "sfApproachDistanceEnd" -at "double";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 2 0 no 3
		7 0 0 0 1 2 2 2
		5
		-0.70834812759087029 -6.3108214333829693 3.98757389348029
		-0.65297343807564445 -7.0258425361858627 3.98757389348029
		-0.40378733525713173 -8.6397473110838376 3.98757389348029
		-0.16844490481742369 -10.784810619492525 3.98757389348029
		0.20533424941034539 -12.153565302000928 3.98757389348029
		;
	setAttr ".dcv" yes;
	setAttr -k on ".sfPointDensity" 0.5;
	setAttr -k on ".sfStrokeLength" 1000;
	setAttr -k on ".sfOverlap";
	setAttr -k on ".sfRandomLengthFactor";
	setAttr -k on ".sfRandomOverlapFactor";
	setAttr -k on ".sfForceDip";
	setAttr -k on ".sfRepeats";
	setAttr -k on ".sfRepeatOffset";
	setAttr -k on ".sfRepeatMirror" yes;
	setAttr -k on ".sfRepeatOscillate" yes;
	setAttr -k on ".sfStrokeRotation";
	setAttr -k on ".sfStrokeTranslation";
	setAttr -k on ".sfPivotFraction" 0.5;
	setAttr -k on ".sfBrushId";
	setAttr -k on ".sfPaintId";
	setAttr -k on ".sfBrushRotateTilt";
	setAttr -k on ".sfBrushRotateBank";
	setAttr -k on ".sfBrushRotateTwist" 180;
	setAttr -k on ".sfFollowStroke";
	setAttr -k on ".sfActive" yes;
	setAttr -k on ".sfRepeatAdvance";
	setAttr -k on ".sfSubcurveMin";
	setAttr -k on ".sfSubcurveMax";
	setAttr -k on ".sfApproachDistanceStart" 2;
	setAttr -k on ".sfApproachDistanceMid" 2;
	setAttr -k on ".sfApproachDistanceEnd" 2;
createNode transform -n "r_tray";
	rename -uid "FBCAD417-C14A-4920-1999-B9887E2BC63A";
	addAttr -ci true -sn "sfPaintColorR" -ln "sfPaintColorR" -at "float";
	addAttr -ci true -sn "sfPaintColorG" -ln "sfPaintColorG" -at "float";
	addAttr -ci true -sn "sfPaintColorB" -ln "sfPaintColorB" -at "float";
	addAttr -ci true -sn "sfPaintName" -ln "sfPaintName" -dt "string";
	addAttr -ci true -sn "sfPaintOpacity" -ln "sfPaintOpacity" -at "double";
	addAttr -ci true -sn "sfPaintMaxArcLength" -ln "sfPaintMaxArcLength" -at "double";
	setAttr ".rp" -type "double3" 5.6843418860808015e-14 2.2737367544323206e-13 0 ;
	setAttr ".sp" -type "double3" 5.6843418860808015e-14 2.2737367544323206e-13 0 ;
	setAttr -k on ".sfPaintColorR" 0.97039997577667236;
	setAttr -k on ".sfPaintColorG" 0.46129998564720154;
	setAttr -k on ".sfPaintColorB" 0.98030000925064087;
	setAttr -k on ".sfPaintName" -type "string" "orange";
	setAttr -k on ".sfPaintOpacity" 0.5;
	setAttr -k on ".sfPaintMaxArcLength" 20;
createNode mesh -n "r_trayShape" -p "r_tray";
	rename -uid "39B17E2B-B64E-A26B-6394-E58639ABB2CC";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:14]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[15:19]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.7083333432674408 0.49999997019767761 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 50 ".uvst[0].uvsp[0:49]" -type "float2" 0.375 0 0.38510099
		 0.014371129 0.38510102 0.23562887 0.61519605 0.23562887 0.625 0.25 0.375 0.25 0.61519605
		 0.014371129 0.625 0 0.29166669 0 0.29166669 0.25 0.375 0.91666663 0.625 0.91666663
		 0.625 0.99999994 0.375 0.99999994 0.70833337 0 0.70833337 0.25 0.625 0.33333334 0.375
		 0.33333334 0.20833334 0 0.20833334 0.25 0.375 0.83333331 0.625 0.83333331 0.79166669
		 0 0.79166669 0.25 0.625 0.41666669 0.375 0.41666669 0.125 0 0.125 0.25 0.375 0.75
		 0.625 0.75 0.875 0 0.875 0.25 0.625 0.5 0.375 0.5 0.625 0.27153829 0.375 0.27153829
		 0.375 0.5 0.625 0.5 0.64653826 5.17414e-18 0.64653826 0.25 0.875 0.25 0.875 0 0.375
		 0.75 0.625 0.75 0.35346171 0.25 0.35346171 1.1274109e-17 0.125 0 0.125 0.25 0.375
		 0.97846174 0.625 0.97846174;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 24 ".pt[0:23]" -type "float3"  7.7621551 -0.74639893 -0.54260594 
		5.6701221 -0.48628235 -0.54260594 -6.6329327 -16.387344 -0.54260594 -37.000687 11.088562 
		1.8436368 -39.17366 10.924011 1.8436368 -6.1391048 -18.713074 -0.54260594 -22.866409 
		26.989624 1.8436368 -23.203276 28.890701 1.8436368 6.1947575 -0.47973633 -0.40076476 
		-6.5717068 -16.979736 -0.40076476 -22.85515 28.575119 1.8436368 -37.52182 12.075104 
		1.8436368 6.1947575 -0.47973633 -0.29713321 -6.5717068 -16.979736 -0.29713321 -22.85515 
		28.575119 1.8436368 -37.52182 12.075104 1.8436368 4.8153095 0.41189575 -0.36704192 
		-7.5256023 -15.538101 -0.36704192 -21.932907 28.765991 -0.39038113 -36.110703 12.815979 
		-0.39038113 -7.6484127 -14.507767 -0.44283086 -23.479355 -0.65306091 -0.39038113 
		-11.930174 14.273666 -0.39038113 3.9007664 0.41897583 -0.44283086;
	setAttr -s 24 ".vt[0:23]"  -0.81153107 -8.98333359 4.099960327 0.48139954 -7.95052719 4.099960327
		 0.48139954 7.95053101 4.099960327 29.93354797 7.95053101 4.099960327 31.1884613 8.9833374 4.099960327
		 -0.81153107 8.9833374 4.099960327 29.93354797 -7.95052719 4.099960327 31.1884613 -8.98333359 4.099960327
		 0.18846893 -8.25 3.3907547 0.18846893 8.25 3.3907547 30.18847656 -8.25 3.3907547
		 30.18847656 8.25 3.3907547 0.18846893 -8.25 2.87259674 0.18846893 8.25 2.87259674
		 30.18847656 -8.25 2.87259674 30.18847656 8.25 2.87259674 1.35513306 -7.97499847 0
		 1.35513306 7.97499847 0 29.02180481 -7.97499847 0 29.02180481 7.97499847 0 1.87381744 7.46335602 0.3789444
		 17.70475769 7.46335602 0.3789444 17.70475769 -7.46335983 0.3789444 1.87381744 -7.46335983 0.3789444;
	setAttr -s 42 ".ed[0:41]"  0 1 1 1 2 0 2 3 0 3 4 1 4 5 0 5 0 0 3 6 1
		 6 1 0 0 7 0 7 4 0 8 0 0 5 9 0 9 8 1 8 10 1 10 7 0 10 11 1 11 4 0 11 9 1 12 8 0 9 13 0
		 13 12 1 12 14 1 14 10 0 14 15 1 15 11 0 15 13 1 16 12 0 13 17 0 17 16 0 16 18 0 18 14 0
		 18 19 0 19 15 0 19 17 0 2 20 0 20 21 0 21 3 0 21 22 1 22 6 0 20 23 0 23 22 0 1 23 0;
	setAttr -s 20 -ch 84 ".fc[0:19]" -type "polyFaces" 
		f 6 0 1 2 3 4 5
		mu 0 6 0 1 2 3 4 5
		f 6 6 7 -1 8 9 -4
		mu 0 6 3 6 1 0 7 4
		f 4 10 -6 11 12
		mu 0 4 8 0 5 9
		f 4 13 14 -9 -11
		mu 0 4 10 11 12 13
		f 4 -15 15 16 -10
		mu 0 4 7 14 15 4
		f 4 -5 -17 17 -12
		mu 0 4 5 4 16 17
		f 4 18 -13 19 20
		mu 0 4 18 8 9 19
		f 4 21 22 -14 -19
		mu 0 4 20 21 11 10
		f 4 -23 23 24 -16
		mu 0 4 14 22 23 15
		f 4 -18 -25 25 -20
		mu 0 4 17 16 24 25
		f 4 26 -21 27 28
		mu 0 4 26 18 19 27
		f 4 29 30 -22 -27
		mu 0 4 28 29 21 20
		f 4 -31 31 32 -24
		mu 0 4 22 30 31 23
		f 4 -26 -33 33 -28
		mu 0 4 25 24 32 33
		f 4 -34 -32 -30 -29
		mu 0 4 33 32 29 28
		f 4 -3 34 35 36
		mu 0 4 34 35 36 37
		f 4 -7 -37 37 38
		mu 0 4 38 39 40 41
		f 4 39 40 -38 -36
		mu 0 4 36 42 43 37
		f 4 -2 41 -40 -35
		mu 0 4 44 45 46 47
		f 4 -8 -39 -41 -42
		mu 0 4 48 49 43 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "approachTargets";
	rename -uid "F7509365-B148-1DE0-A19F-92971B081DE5";
createNode transform -n "toolChangeTarget" -p "approachTargets";
	rename -uid "F45CD204-A740-3FF0-A703-2BB57A6C22F6";
	setAttr ".t" -type "double3" 127.83147193913082 52.651873466627592 142.07110840134891 ;
	setAttr ".r" -type "double3" -181.92148771275029 128.61840608542022 -167.19898908867884 ;
	setAttr -l on ".sx";
	setAttr -l on ".sy";
	setAttr -l on ".sz";
createNode mesh -n "toolChangeTargetShape" -p "toolChangeTarget";
	rename -uid "9D1A72E8-5141-ABED-6736-2784A63E9AD3";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.375 0 0.625 0 0.375
		 0.25 0.625 0.25 0.375 0.5 0.625 0.5 0.375 0.75 0.625 0.75 0.375 1 0.625 1 0.875 0
		 0.875 0.25 0.125 0 0.125 0.25;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -5 -5 5 5 -5 5 -5 5 5 5 5 5 -5 5 -5 5 5 -5
		 -5 -5 -5 5 -5 -5;
	setAttr -s 12 ".ed[0:11]"  0 1 0 2 3 0 4 5 0 6 7 0 0 2 0 1 3 0 2 4 0
		 3 5 0 4 6 0 5 7 0 6 0 0 7 1 0;
	setAttr -s 6 -ch 24 ".fc[0:5]" -type "polyFaces" 
		f 4 0 5 -2 -5
		mu 0 4 0 1 3 2
		f 4 1 7 -3 -7
		mu 0 4 2 3 5 4
		f 4 2 9 -4 -9
		mu 0 4 4 5 7 6
		f 4 3 11 -1 -11
		mu 0 4 6 7 9 8
		f 4 -12 -10 -8 -6
		mu 0 4 1 10 11 3
		f 4 10 4 6 8
		mu 0 4 12 0 2 13;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "dipTarget" -p "approachTargets";
	rename -uid "A44C7138-4E40-DA78-31F5-43B6962EA2B2";
	setAttr ".t" -type "double3" 81.897222654113548 90.291723791093517 63.052230594223687 ;
	setAttr ".r" -type "double3" -14.58654412011111 156.56809027684562 34.824464494552117 ;
	setAttr -l on ".sx";
	setAttr -l on ".sy";
	setAttr -l on ".sz";
createNode mesh -n "dipTargetShape" -p "dipTarget";
	rename -uid "AC857937-D347-2B1C-0449-4DAD58519A74";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.375 0 0.625 0 0.375
		 0.25 0.625 0.25 0.375 0.5 0.625 0.5 0.375 0.75 0.625 0.75 0.375 1 0.625 1 0.875 0
		 0.875 0.25 0.125 0 0.125 0.25;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".pt[0:7]" -type "float3"  -4.5 -4.5 4.5 4.5 -4.5 4.5 
		-4.5 4.5 4.5 4.5 4.5 4.5 -4.5 4.5 -4.5 4.5 4.5 -4.5 -4.5 -4.5 -4.5 4.5 -4.5 -4.5;
	setAttr -s 8 ".vt[0:7]"  -0.5 -0.5 0.5 0.5 -0.5 0.5 -0.5 0.5 0.5 0.5 0.5 0.5
		 -0.5 0.5 -0.5 0.5 0.5 -0.5 -0.5 -0.5 -0.5 0.5 -0.5 -0.5;
	setAttr -s 12 ".ed[0:11]"  0 1 0 2 3 0 4 5 0 6 7 0 0 2 0 1 3 0 2 4 0
		 3 5 0 4 6 0 5 7 0 6 0 0 7 1 0;
	setAttr -s 6 -ch 24 ".fc[0:5]" -type "polyFaces" 
		f 4 0 5 -2 -5
		mu 0 4 0 1 3 2
		f 4 1 7 -3 -7
		mu 0 4 2 3 5 4
		f 4 2 9 -4 -9
		mu 0 4 4 5 7 6
		f 4 3 11 -1 -11
		mu 0 4 6 7 9 8
		f 4 -12 -10 -8 -6
		mu 0 4 1 10 11 3
		f 4 10 4 6 8
		mu 0 4 12 0 2 13;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "homeTarget" -p "approachTargets";
	rename -uid "AFA1878D-E64A-DED0-6B53-28BE03E5D902";
	setAttr ".t" -type "double3" 78.112468636033725 0 142.07110840135908 ;
	setAttr ".r" -type "double3" 0 89.999999999999915 0 ;
	setAttr -l on ".sx";
	setAttr -l on ".sy";
	setAttr -l on ".sz";
createNode mesh -n "homeTargetShape" -p "homeTarget";
	rename -uid "CDCEBA12-E24B-5371-1947-19BE6393AE28";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.375 0 0.625 0 0.375
		 0.25 0.625 0.25 0.375 0.5 0.625 0.5 0.375 0.75 0.625 0.75 0.375 1 0.625 1 0.875 0
		 0.875 0.25 0.125 0 0.125 0.25;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -5 -5 5 5 -5 5 -5 5 5 5 5 5 -5 5 -5 5 5 -5
		 -5 -5 -5 5 -5 -5;
	setAttr -s 12 ".ed[0:11]"  0 1 0 2 3 0 4 5 0 6 7 0 0 2 0 1 3 0 2 4 0
		 3 5 0 4 6 0 5 7 0 6 0 0 7 1 0;
	setAttr -s 6 -ch 24 ".fc[0:5]" -type "polyFaces" 
		f 4 0 5 -2 -5
		mu 0 4 0 1 3 2
		f 4 1 7 -3 -7
		mu 0 4 2 3 5 4
		f 4 2 9 -4 -9
		mu 0 4 4 5 7 6
		f 4 3 11 -1 -11
		mu 0 4 6 7 9 8
		f 4 -12 -10 -8 -6
		mu 0 4 1 10 11 3
		f 4 10 4 6 8
		mu 0 4 12 0 2 13;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "B42B7C07-0242-40C1-41F2-FCA1E25C2798";
	setAttr -s 16 ".lnk";
	setAttr -s 16 ".slnk";
createNode displayLayerManager -n "layerManager";
	rename -uid "F0345BFD-B441-B00E-8EDD-348CE29BA05B";
	setAttr -s 3 ".dli[1:2]"  1 7;
	setAttr -s 3 ".dli";
createNode displayLayer -n "defaultLayer";
	rename -uid "6A80EA64-4749-419F-EF64-5799265E621A";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "15DB2630-1544-C678-3893-7E805925E3AC";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "CDB1CE57-F341-2F5A-536C-B69783E1D135";
	setAttr ".g" yes;
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "DFF4EF4C-C94B-EB14-71D1-28A8FEF1822D";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "0C468C42-F64A-8240-5F56-AD826E914FA4";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "57F8CCD8-BE49-3F33-0A35-13874037C68D";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n"
		+ "            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n"
		+ "            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n"
		+ "            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n"
		+ "            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n"
		+ "            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n"
		+ "            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n"
		+ "            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 663\n            -height 645\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 1\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n"
		+ "            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n"
		+ "            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 662\n            -height 645\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n"
		+ "            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n"
		+ "            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n"
		+ "            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n"
		+ "            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n"
		+ "                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n"
		+ "                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n"
		+ "                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 1\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 1\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 0\n                -outliner \"graphEditor1OutlineEd\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n"
		+ "                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n"
		+ "                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" != $panelName) {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n"
		+ "                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n"
		+ "                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n"
		+ "                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n"
		+ "                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"vertical2\\\" -ps 1 50 100 -ps 2 50 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Front View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 663\\n    -height 645\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 663\\n    -height 645\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 1\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 662\\n    -height 645\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 1\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 662\\n    -height 645\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 50 -size 150 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels yes -displayOrthographicLabels yes -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition axis;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "A8B83D5E-2A4D-EB19-9193-C5B636089398";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode ramp -n "ramp1";
	rename -uid "1F320BFC-3844-B67F-750C-598CEF430FCA";
	setAttr -s 10 ".cel";
	setAttr ".cel[0].ep" 0.42204302549362183;
	setAttr ".cel[0].ec" -type "float3" 0 0 0 ;
	setAttr ".cel[1].ep" 0.47580644488334656;
	setAttr ".cel[1].ec" -type "float3" 1 1 1 ;
	setAttr ".cel[2].ep" 0.60752689838409424;
	setAttr ".cel[2].ec" -type "float3" 0 0 0 ;
	setAttr ".cel[3].ep" 0.551075279712677;
	setAttr ".cel[3].ec" -type "float3" 1 1 1 ;
	setAttr ".cel[4].ep" 0.74193549156188965;
	setAttr ".cel[4].ec" -type "float3" 1 1 1 ;
	setAttr ".cel[5].ep" 0.92473119497299194;
	setAttr ".cel[5].ec" -type "float3" 0 0 0 ;
	setAttr ".cel[6].ep" 0.84139782190322876;
	setAttr ".cel[6].ec" -type "float3" 1 1 1 ;
	setAttr ".cel[9].ep" 0.099462367594242096;
	setAttr ".cel[9].ec" -type "float3" 0 0 0 ;
	setAttr ".cel[10].ep" 0.35215052962303162;
	setAttr ".cel[10].ec" -type "float3" 0 0 0 ;
	setAttr ".cel[11].ep" 0.68548387289047241;
	setAttr ".cel[11].ec" -type "float3" 0 0 0 ;
createNode place2dTexture -n "place2dTexture1";
	rename -uid "41174C3A-654E-36BA-2019-6F87B989C746";
createNode materialInfo -n "materialInfo37";
	rename -uid "87796A5E-AE4D-D720-503E-ABA61951D68E";
createNode shadingEngine -n "girderShaderSG";
	rename -uid "286054E6-3A4D-1C39-01C0-3B804379F749";
	setAttr ".ihi" 0;
	setAttr -s 4 ".dsm";
	setAttr ".ro" yes;
createNode lambert -n "girderShader";
	rename -uid "6C48D21F-C548-100E-18E1-E2B066D457BA";
	setAttr ".c" -type "float3" 1 0.72939998 0.68620002 ;
	setAttr -av ".cr";
	setAttr -av ".cg";
	setAttr -av ".cb";
createNode materialInfo -n "pasted__materialInfo43";
	rename -uid "EB97A64D-8046-48B4-90C7-77BCF4BD4D12";
createNode shadingEngine -n "pasted__cable_04SG1";
	rename -uid "F6C1A29D-044B-E6E4-4C43-3292FEB2E889";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode phong -n "pasted__rubber1";
	rename -uid "C3BD5000-734E-24D3-2C57-3DB538C60817";
	setAttr ".dc" 1;
	setAttr ".c" -type "float3" 0.17094018 0.17094018 0.17094018 ;
	setAttr ".ambc" -type "float3" 0.0039215689 0.0039215689 0.0039215689 ;
	setAttr ".sc" -type "float3" 0.80102038 0.80102038 0.80102038 ;
	setAttr ".rfl" 1;
	setAttr ".cp" 3.6516854763031006;
createNode displayLayer -n "layer1";
	rename -uid "6F9B218E-1946-7D6E-F29E-359EB9D9D9C7";
	setAttr ".dt" 1;
	setAttr ".do" 1;
createNode ikRPsolver -n "ikRPsolver";
	rename -uid "662772B9-C940-37AD-821F-4A943ACC66C7";
createNode expression -n "expression1";
	rename -uid "CC381CE5-5F40-875F-7F8A-3E8C6E5E41E0";
	setAttr -k on ".nds";
	setAttr -s 6 ".out";
	setAttr ".ixp" -type "string" ".O[0] = .I[0];\n.O[1] = !  .I[0];\n\n\n.O[2] = .I[0];\n.O[3] = ! .I[0];\n\n\n.O[4] = .I[0];\n.O[5] = ! .I[0];";
createNode displayLayer -n "pasted__layer8";
	rename -uid "75153B22-FB4C-E0CC-A716-06A0D78647F4";
	setAttr ".c" 6;
	setAttr ".do" 1;
createNode expression -n "pasted__expression5";
	rename -uid "DD15E526-FE49-6833-8C67-698B9781253B";
	setAttr -k on ".nds";
	setAttr -s 13 ".in";
	setAttr -s 13 ".in";
	setAttr -s 8 ".out";
	setAttr ".ixp" -type "string" "if (.I[0]) {\n\n\n\t.O[0] = .I[1];\n\t.O[1] = .I[2];\n\t.O[2] = .I[3];\n\n\t.O[3] = .I[4];\n\t.O[4] = .I[5];\n\t.O[5] = .I[6];\n\n\t.O[6] =  0;\n\t.O[7] =  1;\n} else {\n\t.O[0] = .I[7];\n\t.O[1] = .I[8];\n\t.O[2] = .I[9];\n\n\t.O[3] = .I[10];\n\t.O[4] = .I[11];\n\t.O[5] = .I[12];\n\t\n\t.O[6] = 1;\n\t.O[7] = 0;\n\n\n\n\n}";
createNode materialInfo -n "pasted__materialInfo44";
	rename -uid "504A8F2E-A147-F0C1-4494-349A62C67FC3";
createNode shadingEngine -n "pasted__cable_04SG2";
	rename -uid "432AA41C-D041-F74F-978B-81A13EFE9ACE";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode phong -n "pasted__rubber2";
	rename -uid "CDA39521-9244-0B0B-CAB7-2C90909ACF08";
	setAttr ".dc" 1;
	setAttr ".c" -type "float3" 0.17094018 0.17094018 0.17094018 ;
	setAttr ".ambc" -type "float3" 0.0039215689 0.0039215689 0.0039215689 ;
	setAttr ".sc" -type "float3" 0.80102038 0.80102038 0.80102038 ;
	setAttr ".rfl" 1;
	setAttr ".cp" 3.6516854763031006;
createNode materialInfo -n "pasted__materialInfo37";
	rename -uid "D3067A81-BD40-3BF0-0E38-87A9BD3380DA";
createNode shadingEngine -n "pasted__girderShaderSG";
	rename -uid "DC9D2F3C-C442-EE16-F178-0AAF68947C39";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode lambert -n "pasted__girderShader";
	rename -uid "2B4501DD-5843-E20F-E535-1784B459CF1E";
	setAttr ".c" -type "float3" 1 0.72939998 0.68620002 ;
	setAttr -av ".cr";
	setAttr -av ".cg";
	setAttr -av ".cb";
createNode materialInfo -n "pasted__materialInfo45";
	rename -uid "5005DE08-484A-C3B2-2D1A-D9A386D8B500";
createNode shadingEngine -n "pasted__typeBlinn1SG";
	rename -uid "A88A52A6-2841-4FA1-1DBC-3D94C2C212AA";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode blinn -n "pasted__typeBlinn1";
	rename -uid "F790672F-234B-25E1-DDC6-DEB224EB7825";
	setAttr ".c" -type "float3" 1 1 1 ;
createNode ikRPsolver -n "pasted__ikRPsolver";
	rename -uid "0654B135-CC40-39CF-E445-A984ED5426D1";
createNode lambert -n "sx_reddish";
	rename -uid "94DF80C8-5442-8885-2198-28805A024E52";
createNode shadingEngine -n "sx_reddish_SG";
	rename -uid "71B69AB1-0E4F-59F6-AE82-EBAEDF54D94D";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo39";
	rename -uid "0F2EA302-5C47-F80A-B9AF-99B42885B55E";
createNode lambert -n "sx_sky";
	rename -uid "F8668812-F74D-BB81-2EFE-8896119421C6";
createNode shadingEngine -n "sx_sky_SG";
	rename -uid "1DCA5CD2-2844-C98F-4832-289DB5F605AE";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo40";
	rename -uid "9F23C497-8549-C0A7-F854-41BE1976A51C";
createNode lambert -n "sx_orange";
	rename -uid "8E7A55F2-7145-7E3A-4188-46BB19DB29E3";
createNode shadingEngine -n "sx_orange_SG";
	rename -uid "B140ADFD-8042-7F6E-5F55-CFA978DF45C5";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo41";
	rename -uid "440126E0-484D-0324-71F2-5FBD48A40937";
createNode lambert -n "sx_purple";
	rename -uid "6CC08DE8-B94C-CE60-803C-2FA92C2E2B9C";
createNode shadingEngine -n "sx_purple_SG";
	rename -uid "72426D43-5F4B-6EE5-0980-2C84F8F764AD";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo42";
	rename -uid "A77AC2C8-104A-76DC-6DC9-898275FBEBBF";
createNode lambert -n "sx_turqoise";
	rename -uid "5673DC33-CA4B-135F-7EC4-9DA939979E4C";
createNode shadingEngine -n "sx_turqoise_SG";
	rename -uid "23314831-0745-DE61-1BCE-B4910445BD21";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo43";
	rename -uid "567AA567-4C49-64A8-30B6-C19D77F406D4";
createNode lambert -n "sx_green";
	rename -uid "BB14F67A-CC4F-6BBB-1312-598399F908DF";
createNode shadingEngine -n "sx_green_SG";
	rename -uid "2210E029-4B43-84B0-3195-5CA918FEE61E";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo44";
	rename -uid "8E27B31F-204E-D273-59B5-CDA107E32760";
createNode lambert -n "sx_canary";
	rename -uid "76F7D6A8-0A4A-5EAF-A5BF-38A595DFFF26";
createNode shadingEngine -n "sx_canary_SG";
	rename -uid "6508C815-6A4B-3C8C-6AAC-47A329982EDF";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo45";
	rename -uid "430A6609-974F-D579-F203-67AE43422192";
createNode lambert -n "sx_black";
	rename -uid "29F2BAFD-9E4C-AE40-4976-C9A5FE38CF45";
createNode shadingEngine -n "sx_black_SG";
	rename -uid "7CF66808-784B-3160-D1E2-DFAA500CDB44";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo46";
	rename -uid "D119C17F-9641-996A-B112-6A8EAA1D508C";
createNode materialInfo -n "pasted__materialInfo46";
	rename -uid "2D54E5FD-5D43-C684-84C1-C195155038F0";
createNode shadingEngine -n "pasted__cable_04SG3";
	rename -uid "8B6B846E-A949-A027-E760-51A4E6EECF7C";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode phong -n "pasted__rubber3";
	rename -uid "A01C3376-8E44-DA53-C1C0-40A949754D87";
	setAttr ".dc" 1;
	setAttr ".c" -type "float3" 0.17094018 0.17094018 0.17094018 ;
	setAttr ".ambc" -type "float3" 0.0039215689 0.0039215689 0.0039215689 ;
	setAttr ".sc" -type "float3" 0.80102038 0.80102038 0.80102038 ;
	setAttr ".rfl" 1;
	setAttr ".cp" 3.6516854763031006;
createNode strokeCurve -n "sourceDipShape3_SC";
	rename -uid "693F143A-2544-09CF-CFBB-7FA7001F6649";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape2_SC";
	rename -uid "C8A606AE-F341-47A3-335F-C989DCA8CF23";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape1_SC";
	rename -uid "8D1CD041-7B46-970E-E2CF-7595463D8485";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape4_SC";
	rename -uid "149F9BE8-724F-F06C-8B95-20BF41DBF029";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "cShape9_SC";
	rename -uid "F14F79B7-104E-B665-1A2F-34A3B84FF9D9";
	setAttr ".pd" 0.49;
	setAttr ".stl" 33.443697815962281;
	setAttr ".ovlp" -10.350000000000001;
	setAttr ".rpof" 4.41;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".scv" -type "double2" 0 300 ;
	setAttr ".apd" -type "double3" 5.76 5.9 3.84 ;
	setAttr ".lft" -type "double3" 3.02 5 0.31999999999999995 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "cShape8_SC";
	rename -uid "C48E2C36-7D46-CF8C-FD2A-3AB6B5D54A40";
	setAttr ".pd" 0.49;
	setAttr ".stl" 33.443697815962281;
	setAttr ".ovlp" -10.350000000000001;
	setAttr ".rpof" 4.41;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".scv" -type "double2" 0 300 ;
	setAttr ".apd" -type "double3" 5.76 5.9 3.84 ;
	setAttr ".lft" -type "double3" 3.02 5 0.31999999999999995 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "cShape7_SC";
	rename -uid "F0D82BA4-9F4B-EF40-0CE8-789842FB4768";
	setAttr ".pd" 0.49;
	setAttr ".stl" 33.443697815962281;
	setAttr ".ovlp" -10.350000000000001;
	setAttr ".rpof" 4.41;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".scv" -type "double2" 0 300 ;
	setAttr ".apd" -type "double3" 5.76 5.9 3.84 ;
	setAttr ".lft" -type "double3" 3.02 5 0.31999999999999995 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "cShape6_SC";
	rename -uid "517EFB77-F64A-BB1D-369D-ADB104CAAF1D";
	setAttr ".pd" 0.49;
	setAttr ".stl" 33.443697815962281;
	setAttr ".ovlp" -10.350000000000001;
	setAttr ".rpof" 4.41;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".scv" -type "double2" 0 300 ;
	setAttr ".apd" -type "double3" 5.76 5.9 3.84 ;
	setAttr ".lft" -type "double3" 3.02 5 0.31999999999999995 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "cShape5_SC";
	rename -uid "B6B2FAB5-854E-80B7-1D4C-689724FB7BC2";
	setAttr ".pd" 0.49;
	setAttr ".stl" 33.443697815962281;
	setAttr ".ovlp" -10.350000000000001;
	setAttr ".rpof" 4.41;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".scv" -type "double2" 0 300 ;
	setAttr ".apd" -type "double3" 5.76 5.9 3.84 ;
	setAttr ".lft" -type "double3" 3.02 5 0.31999999999999995 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "cShape4_SC";
	rename -uid "809DDADF-7840-4AE2-EC32-B49157443F53";
	setAttr ".pd" 0.49;
	setAttr ".stl" 33.443697815962281;
	setAttr ".ovlp" -10.350000000000001;
	setAttr ".rpof" 4.41;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".scv" -type "double2" 0 300 ;
	setAttr ".apd" -type "double3" 5.76 5.9 3.84 ;
	setAttr ".lft" -type "double3" 3.02 5 0.31999999999999995 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "cShape3_SC";
	rename -uid "8D645F0A-6942-97ED-AB50-6CA8824E3383";
	setAttr ".pd" 0.49;
	setAttr ".stl" 33.443697815962281;
	setAttr ".ovlp" -10.350000000000001;
	setAttr ".rpof" 4.41;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".scv" -type "double2" 0 300 ;
	setAttr ".apd" -type "double3" 5.76 5.9 3.84 ;
	setAttr ".lft" -type "double3" 3.02 5 0.31999999999999995 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "cShape2_SC";
	rename -uid "5CCD2378-6040-0AC7-C866-B2ADFFA5AB8B";
	setAttr ".pd" 0.49;
	setAttr ".stl" 33.443697815962281;
	setAttr ".ovlp" -10.350000000000001;
	setAttr ".rpof" 4.41;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".scv" -type "double2" 0 300 ;
	setAttr ".apd" -type "double3" 5.76 5.9 3.84 ;
	setAttr ".lft" -type "double3" 3.02 5 0.31999999999999995 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "cShape1_SC";
	rename -uid "0E202811-BB48-0C51-7BA0-958178590B8F";
	setAttr ".pd" 0.49;
	setAttr ".stl" 33.443697815962281;
	setAttr ".ovlp" -10.350000000000001;
	setAttr ".rpof" 4.41;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".scv" -type "double2" 0 300 ;
	setAttr ".apd" -type "double3" 5.76 5.25 3.84 ;
	setAttr ".lft" -type "double3" 3.02 5 0.31999999999999995 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode groupId -n "groupId1";
	rename -uid "F1E248A4-9642-19CA-531D-BEA197365AAF";
	setAttr ".ihi" 0;
createNode groupId -n "groupId2";
	rename -uid "1917A879-3746-B31B-4149-45B232266554";
	setAttr ".ihi" 0;
createNode groupId -n "groupId3";
	rename -uid "47EA412D-DE42-55C2-E170-FCA8F663526E";
	setAttr ".ihi" 0;
createNode groupId -n "groupId4";
	rename -uid "81E2FE68-F14F-C546-BBEB-959B9BDD0AE9";
	setAttr ".ihi" 0;
createNode groupId -n "groupId5";
	rename -uid "DC48AC65-9048-25ED-A52E-E3AA528D28F6";
	setAttr ".ihi" 0;
createNode groupId -n "groupId6";
	rename -uid "4309A5A3-1D49-3C1F-E1B6-B1916C88F24B";
	setAttr ".ihi" 0;
createNode groupId -n "groupId7";
	rename -uid "6130C187-7440-1494-0A61-8FA12BCEDC79";
	setAttr ".ihi" 0;
createNode groupId -n "groupId8";
	rename -uid "81CDC4E8-6548-AF3A-E3A8-DEA42B346A77";
	setAttr ".ihi" 0;
createNode groupId -n "groupId9";
	rename -uid "D15B7BBA-754B-47DC-4354-31BF8069A0E0";
	setAttr ".ihi" 0;
createNode groupId -n "groupId10";
	rename -uid "81838E36-2048-5F09-CBF9-999536995C2F";
	setAttr ".ihi" 0;
createNode groupId -n "groupId11";
	rename -uid "78B77089-334D-5E2E-63A0-48A7B662C45A";
	setAttr ".ihi" 0;
createNode groupId -n "groupId12";
	rename -uid "7CB3BD20-B843-1296-05FF-B183A3BAE853";
	setAttr ".ihi" 0;
createNode groupId -n "groupId13";
	rename -uid "18B5945D-CE43-BEE8-2D84-BB812048D7C2";
	setAttr ".ihi" 0;
createNode groupId -n "groupId14";
	rename -uid "D2B876DC-0E4E-137A-BC34-EEBEED2A9760";
	setAttr ".ihi" 0;
createNode groupId -n "groupId15";
	rename -uid "970C2FC0-BC4A-6263-7D90-598031BC1293";
	setAttr ".ihi" 0;
createNode groupId -n "groupId16";
	rename -uid "E46D693C-1147-17B4-BE33-CF8D097B2AFC";
	setAttr ".ihi" 0;
createNode groupId -n "groupId17";
	rename -uid "5F591117-E34F-1E2C-6042-A599D9EEF72F";
	setAttr ".ihi" 0;
createNode groupId -n "groupId18";
	rename -uid "B322C82F-A04B-E057-EF44-7DA507B5C255";
	setAttr ".ihi" 0;
createNode groupId -n "groupId19";
	rename -uid "888EEE0B-AD45-0DFD-9633-93BEBAF19333";
	setAttr ".ihi" 0;
createNode groupId -n "groupId20";
	rename -uid "F4852678-FE4C-67E0-82A7-3FA16FA6D5E0";
	setAttr ".ihi" 0;
createNode groupId -n "groupId21";
	rename -uid "849AB544-1643-E6C0-3581-648A48D7AD88";
	setAttr ".ihi" 0;
createNode groupId -n "groupId22";
	rename -uid "7B43F22B-104C-3749-ED35-719471B7F57D";
	setAttr ".ihi" 0;
createNode groupId -n "groupId23";
	rename -uid "8F0E1D31-6D4F-FACA-278C-0C94193B4A16";
	setAttr ".ihi" 0;
createNode groupId -n "groupId24";
	rename -uid "C80C2EF1-3742-BF3D-7EC8-9FBDDC3A903A";
	setAttr ".ihi" 0;
createNode groupId -n "groupId25";
	rename -uid "572C310F-A54D-02DA-BBE6-6B8A0D3C5523";
	setAttr ".ihi" 0;
createNode groupId -n "groupId26";
	rename -uid "90F7E41D-BF4F-D55B-4C47-259FB19E0478";
	setAttr ".ihi" 0;
createNode strokeCurve -n "dcx_p04_b06_c0Shape0_SC";
	rename -uid "65BB0B5F-994E-FD4F-C950-54AE26493050";
	setAttr ".stl" 1000;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p04_b06_c0Shape1_SC";
	rename -uid "0958B2F9-0149-A201-949A-83857901BE40";
	setAttr ".stl" 1000;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p04_b06_c0Shape2_SC";
	rename -uid "08F25CEE-DE40-CC44-E267-D498586BA122";
	setAttr ".stl" 1000;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p04_b06_c0Shape3_SC";
	rename -uid "D8372D7F-D445-C756-BB25-948BDF8B4D43";
	setAttr ".stl" 1000;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p02_b02_c0Shape0_SC";
	rename -uid "2962EF24-A94A-73B2-D765-C1931ADBF942";
	setAttr ".stl" 1000;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p02_b02_c0Shape1_SC";
	rename -uid "CBE6343F-3F4B-3EA2-C116-6CAB1E876B4C";
	setAttr ".stl" 1000;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p02_b02_c0Shape2_SC";
	rename -uid "6B87852C-6842-3402-0E85-3CA0C0EAECBE";
	setAttr ".stl" 1000;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p02_b02_c0Shape3_SC";
	rename -uid "C27175C0-F947-A309-E2DD-98AA38BC0499";
	setAttr ".stl" 1000;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p05_b07_c0Shape0_SC";
	rename -uid "375B128A-9B44-BAEF-186B-408942459ED6";
	setAttr ".stl" 1000;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p05_b07_c0Shape1_SC";
	rename -uid "44369F1D-1A4E-7B48-32D7-1289E64CBDE5";
	setAttr ".stl" 1000;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p05_b07_c0Shape2_SC";
	rename -uid "3B1969D0-764B-F337-2F36-00AF49872BFE";
	setAttr ".stl" 1000;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p05_b07_c0Shape3_SC";
	rename -uid "A3350472-5342-56E8-5B33-04AB1383B6F1";
	setAttr ".stl" 1000;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p07_b05_c0Shape0_SC";
	rename -uid "CA8C034E-104B-5FC2-AEC2-B8A7E6CA2DEF";
	setAttr ".stl" 1000;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p07_b05_c0Shape1_SC";
	rename -uid "CF5CCCBA-0E44-ED2D-B19F-44B2911F7369";
	setAttr ".stl" 1000;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p07_b05_c0Shape2_SC";
	rename -uid "BE3631C0-564F-7015-421C-148706163F58";
	setAttr ".stl" 1000;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p07_b05_c0Shape3_SC";
	rename -uid "DCEA380E-1041-0F6E-B01B-C2A53A188D63";
	setAttr ".stl" 1000;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p06_b08_c0Shape0_SC";
	rename -uid "342A2463-374D-6F8A-FDDF-EA98D33574C0";
	setAttr ".stl" 1000;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p06_b08_c0Shape1_SC";
	rename -uid "C960D144-4E4B-D117-FFD6-2E845A49CCA7";
	setAttr ".stl" 1000;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p06_b08_c0Shape2_SC";
	rename -uid "A103FF20-4547-71A7-DAE6-D1967BCAE915";
	setAttr ".stl" 1000;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p06_b08_c0Shape3_SC";
	rename -uid "C03181CF-EF4D-B0A9-033F-84B9D76D1AE3";
	setAttr ".stl" 1000;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p01_b04_c0Shape0_SC";
	rename -uid "F0CA20DB-7E40-3489-D90D-BEA914FAF255";
	setAttr ".stl" 1000;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p01_b04_c0Shape1_SC";
	rename -uid "AFFA5D6A-1245-6663-8E7F-FE8A121AB12A";
	setAttr ".stl" 1000;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p01_b04_c0Shape2_SC";
	rename -uid "F9C94A14-9A4F-7196-DBAE-64B1DA6477DD";
	setAttr ".stl" 1000;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p01_b04_c0Shape3_SC";
	rename -uid "F410CFED-9B41-8B7C-91E9-34825DDDE32C";
	setAttr ".stl" 1000;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p03_b01_c0Shape0_SC";
	rename -uid "759C06D5-1C4B-C239-065A-82B7BFCCBA14";
	setAttr ".stl" 1000;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p03_b01_c0Shape1_SC";
	rename -uid "ECD1EED2-FD48-28CD-6690-63B507CA411D";
	setAttr ".stl" 1000;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p03_b01_c0Shape2_SC";
	rename -uid "4BA45094-3945-0D7D-4E67-399A0C797D42";
	setAttr ".stl" 1000;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p03_b01_c0Shape3_SC";
	rename -uid "7FFC7AAF-2947-A081-DCA0-6F9606D98888";
	setAttr ".stl" 1000;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p06_b03_c0Shape0_SC";
	rename -uid "8EEE0CCF-194B-90D7-B9AC-ACBEF3ED7E02";
	setAttr ".stl" 1000;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p06_b03_c0Shape1_SC";
	rename -uid "902F2BD0-904E-57D5-E6CB-FBB419271ACE";
	setAttr ".stl" 1000;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p06_b03_c0Shape2_SC";
	rename -uid "B63268A9-5942-5A0A-4977-F6AD4EBE6268";
	setAttr ".stl" 1000;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p06_b03_c0Shape3_SC";
	rename -uid "298BD266-A444-4059-580D-C593E74648C1";
	setAttr ".stl" 1000;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p03_b09_c0Shape0_SC";
	rename -uid "65AF7B11-B64B-1CDC-0BCA-1A807D2BDECE";
	setAttr ".stl" 1000;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p03_b09_c0Shape1_SC";
	rename -uid "F0CC6809-3042-391B-5DA9-EC82B9B9E758";
	setAttr ".stl" 1000;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p03_b09_c0Shape2_SC";
	rename -uid "CE2EC3B4-684E-E209-8848-9595B9CDD07D";
	setAttr ".stl" 1000;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "dcx_p03_b09_c0Shape3_SC";
	rename -uid "587ED058-664C-75EC-992B-ABBA8A85B3A6";
	setAttr ".stl" 1000;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape1_SC1";
	rename -uid "F7444ADA-0049-13C1-5FE6-60AD6A670806";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC1";
	rename -uid "0448A6E1-4D42-E6EF-D832-248EF9C0C487";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC1";
	rename -uid "ECCDD2C4-074A-DDEC-F775-A6AC3BDEED78";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC1";
	rename -uid "94533CA5-5445-44BD-E962-1A9D7BE97FB6";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC2";
	rename -uid "CEE15833-7643-1864-BB5B-859905D5343D";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC2";
	rename -uid "BC868718-0A4B-824E-5AF0-28BB479D5E3D";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC2";
	rename -uid "CDDE0C74-1744-8D65-CA7B-93B9B1279F74";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC2";
	rename -uid "DAF28555-DA47-3AD8-F6F6-A39C47040AC4";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC3";
	rename -uid "B732C911-3B4D-7EC5-1A08-D7834544F2C3";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC3";
	rename -uid "86428FF5-3A4B-601E-3A41-39A8CC3731B4";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC3";
	rename -uid "DA0F743F-2644-ACAA-4A43-76AFD4821AFC";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC3";
	rename -uid "4AF614B8-3D40-B17F-73ED-2EAB44B510F2";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC4";
	rename -uid "E8121A87-1947-7C99-9019-9DA2F3A0B938";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC4";
	rename -uid "866F55B7-BC49-9A07-A160-B783A90BE4A2";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC4";
	rename -uid "73868D08-3345-1376-99A1-4C93485A9DAE";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC4";
	rename -uid "74BC0DDA-6C43-CB9D-7E05-8A92D45158DB";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC5";
	rename -uid "B5CD29D7-0147-2BF0-B49C-16B596FC8337";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC5";
	rename -uid "83AC08D7-0245-2CDD-6278-63AA6D8C837B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC5";
	rename -uid "0FD6D237-2446-9D28-2C52-BCB3A8E006F5";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC5";
	rename -uid "7D4747E3-FB44-75A6-37EB-0E81436B1069";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC6";
	rename -uid "162D438F-554A-5F7B-B4BB-DFB8CCE27B09";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC6";
	rename -uid "277F1715-3640-6EF0-FF2C-12B84CE24F8E";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC6";
	rename -uid "12D90E75-4141-DEAD-42EE-04A9ED48A4A7";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC6";
	rename -uid "31920B7A-E046-7A85-0986-B4A85A9EB49B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC7";
	rename -uid "78807EA2-7941-D620-500D-E0B53A690B9B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC7";
	rename -uid "4D53DBB1-D545-75C2-B492-2D9467F76A38";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC7";
	rename -uid "7602D3EA-D142-32C2-A77D-2E84B7BC8771";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC7";
	rename -uid "1E3B2091-E444-4697-47A5-D0A7EF13A12F";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC8";
	rename -uid "36BA71C2-8944-E2F0-A4DE-79A59DDF6635";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC8";
	rename -uid "03FD56C5-3946-20E4-7EFC-5DA631CF8D16";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC8";
	rename -uid "50F970A1-0F45-DB5F-266A-B5BB11151E5C";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC8";
	rename -uid "BF6B5489-2742-2833-144A-1EB34B116394";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC9";
	rename -uid "479CD557-7F45-259F-2625-7B813FF5AB27";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC9";
	rename -uid "CD271D3C-4842-478A-4BBF-03AFBB2A3D87";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC9";
	rename -uid "8E881E79-6144-0A19-BB59-468DA77B31E1";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC9";
	rename -uid "29A6D887-2E45-91DD-F14C-C9814B392FD0";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC10";
	rename -uid "294DFA34-A442-87E9-2030-AD9F0FE4CC92";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC10";
	rename -uid "53AEEB9A-384F-8244-16B1-C59076274D91";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC10";
	rename -uid "4E1F74E5-1C45-1A48-F310-F897052CDD0F";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC10";
	rename -uid "2989A7E7-044B-DF7C-8C95-9C8FEC3E6EF1";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC11";
	rename -uid "3DE7CAFA-AC41-C399-F40F-33869B76418B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC11";
	rename -uid "41CBFFB5-6B42-8972-ADB6-DE814D51112A";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC11";
	rename -uid "287B7514-AF4B-B1A6-4248-389FE1EC4BF6";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC11";
	rename -uid "00D1ACCF-C749-34B4-17D4-95BF2BD9A8CB";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC12";
	rename -uid "0298F766-B043-17F8-2BD0-AC94040BBA9E";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC12";
	rename -uid "E68735A7-C04B-3251-5B54-AF93F0C5C6AB";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC12";
	rename -uid "6C7E5357-EF4C-5EE9-40B3-B19D4DC70D97";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC12";
	rename -uid "63AB5011-3844-51CE-F247-89BB33718A49";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC13";
	rename -uid "E700A763-BE48-3DCE-A88B-7497AF0BF266";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC13";
	rename -uid "94D39539-5545-62E7-62CA-6A9821DE04CD";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC13";
	rename -uid "CAC7FEA2-FF48-13E2-FA7B-93A455C06157";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC13";
	rename -uid "7557EEA7-EB4D-F7E1-C904-9A80C94BE205";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC14";
	rename -uid "C29B229A-7F41-58CB-4795-C189BDCC10C3";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC14";
	rename -uid "9C417875-924D-4B54-9458-6C8E2F31E3D4";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC14";
	rename -uid "2C9DCBD4-D043-2D28-4791-39AE29EC3324";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC14";
	rename -uid "1441144F-9D46-FBCE-63EA-3ABFE8349A43";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC15";
	rename -uid "4E4BAAD5-7347-C27D-DB44-07BB722FAB12";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC15";
	rename -uid "AF1B7084-8C4F-3420-A552-5C88B4087F7D";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC15";
	rename -uid "733963E0-C045-D26F-8F5B-A3A9BDCF3E7F";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC15";
	rename -uid "A2E4FEBB-5145-5F3C-0719-B0B53CC5F545";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC16";
	rename -uid "793C122B-804B-F8B6-04B9-EC8B4D634086";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC16";
	rename -uid "931F92CE-2540-2938-32ED-50AE0AD02519";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC16";
	rename -uid "FFD85BBF-E741-A30C-59C5-C3A95FBE8A8C";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC16";
	rename -uid "A9A6453C-BB4F-1173-07BC-3B94E0F597C8";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC17";
	rename -uid "C9B6825C-8444-E4D0-D4C1-DA933D11E5FF";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC17";
	rename -uid "6D5701AC-4941-DEBE-6BEF-7287CA5ACFAE";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC17";
	rename -uid "916F8891-4548-05CC-5206-A39606C3636A";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC17";
	rename -uid "DFCF419C-8F45-2E63-4E18-52846829EC49";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC18";
	rename -uid "853C5273-3D46-71DF-E0D3-91A2082363CD";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC18";
	rename -uid "06411156-4144-2B0D-6F22-2FB666A9FDC1";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC18";
	rename -uid "DC1E6D0E-2D47-79BD-7E20-39A014561569";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC18";
	rename -uid "99AFBD96-4149-8DE5-6121-84B8E2A1C0FF";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC19";
	rename -uid "70ABDCBC-A041-2CDC-E589-1FB1628F3084";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC19";
	rename -uid "614449F9-3447-BE9C-5A11-81BCFEFF117A";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC19";
	rename -uid "09EBFC80-BE4A-343A-5DE7-DCB1620B7546";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC19";
	rename -uid "64B6B750-C14D-AA83-092C-FDB147FF6D0A";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC20";
	rename -uid "727944C8-5541-03A1-3E0D-A6ABC9D38F4D";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC20";
	rename -uid "9B573DF5-D74D-879A-C434-27BFE5AFB5EE";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC20";
	rename -uid "0EF1C744-2147-F29C-7E49-228FE96731C8";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC20";
	rename -uid "328B06B6-C949-2ADE-961B-0AA2D036A519";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC21";
	rename -uid "55804F46-8B47-F315-DFCF-7792AC0E6325";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC21";
	rename -uid "3FC21952-344E-ED77-11A8-76AACDD1D53F";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC21";
	rename -uid "844027AD-654B-245D-7E07-899D9E2CBA53";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC21";
	rename -uid "4C213E64-044A-E0FA-7456-5E98CE354674";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC22";
	rename -uid "A8B9B3C2-9D4A-8AB8-A9EA-1B9E00E515D4";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC22";
	rename -uid "13D71213-2A4A-831E-31C2-BDA5B42B619E";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC22";
	rename -uid "B60EF466-B742-773E-7A1C-708ADBB12261";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC22";
	rename -uid "0EA52D3D-BC43-817F-836B-81811D441AE8";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC23";
	rename -uid "7434A0CB-B64A-8897-50F9-7B8DF1FEE1B7";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC23";
	rename -uid "63A3784A-6C4A-03A4-DE80-9B8EDAA12990";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC23";
	rename -uid "B176030D-8449-2D4C-9D9C-D1AE9C7F6BCC";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC23";
	rename -uid "2E1C994C-FE4E-FEC6-CD32-71897E1E54D8";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC24";
	rename -uid "161729AF-8D4D-8758-DF9B-66BF8FB5A60E";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC24";
	rename -uid "367D6F8A-5844-43ED-0CD5-0D9FEB5C0EE1";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC24";
	rename -uid "77E28696-DF41-EA9F-949A-1AB3C6CD2DB0";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC24";
	rename -uid "500C44DC-744B-19F4-B486-1BB1675CFCB3";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC25";
	rename -uid "C8CC1674-0E41-2E12-4E61-4898811C9591";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC25";
	rename -uid "89B7AE92-474C-BF75-1A6B-E5A14F4F9C95";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC25";
	rename -uid "22E72B55-C44A-D333-E001-FDA29DF8E43C";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC25";
	rename -uid "11CB28D0-D241-0747-55CC-52B6B9FE9511";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC26";
	rename -uid "57550326-1B42-E5FA-C3EB-4BB8581DE265";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC26";
	rename -uid "45F5B2C7-7B48-4BC9-8722-4BA17264D0BA";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC26";
	rename -uid "C19B9845-B84C-2F7C-48D7-5BB0A35AE232";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC26";
	rename -uid "9793D242-4B45-6D9E-811C-59A854E2A31D";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC27";
	rename -uid "CFB781B2-784C-97BF-5981-809366D0E10D";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC27";
	rename -uid "E3DC4F81-E542-AEA8-2D46-8994CABDA2D1";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC27";
	rename -uid "78F6A8C6-AE48-4B7D-66FF-D4A280323983";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC27";
	rename -uid "7D51998A-C448-F7AB-5FDB-F49AF31F9FAE";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC28";
	rename -uid "84D247F4-3844-4A79-0A85-52A955C6F961";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC28";
	rename -uid "E7910221-5045-3DF1-F6D7-77B7E59D6EE6";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC28";
	rename -uid "504848A1-7A43-776E-4D47-C49F1D163E06";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC28";
	rename -uid "BDAF83AE-5548-CA65-86B5-2682D4579CE8";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC29";
	rename -uid "CA19B653-C44D-BD18-2E3A-C0A7C5CD8A5A";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC29";
	rename -uid "A3EAF8A1-254C-8BF6-EBE1-5F9392419E30";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC29";
	rename -uid "E5B0721F-2249-69BC-830F-91A20D13A7D5";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC29";
	rename -uid "1918157A-D344-9C8F-2A44-388B9F61E44A";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC30";
	rename -uid "7D98D283-EF41-05A8-33B0-2CAA051EAD19";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC30";
	rename -uid "4CC2AABF-DE47-ACA1-DDD6-B9A78A2EC219";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC30";
	rename -uid "1545ED56-4A4A-BD5D-4329-AC8604DBC1EC";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC30";
	rename -uid "D5CC7D9C-1B4E-7A87-C8EA-3E88258DDE2C";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC31";
	rename -uid "46FF1EA6-5A45-C2B8-F123-13A7555D4E4B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC31";
	rename -uid "E7E0EFF6-1443-F1BE-0EBB-5D925AF4B99C";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC31";
	rename -uid "770D1B25-D04B-89A6-4326-33A3A5886BCC";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC31";
	rename -uid "668944C0-D341-FBF8-6FEE-9E894C83DE45";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC32";
	rename -uid "004E76A0-0946-D7CF-1005-45BDCF57912E";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC32";
	rename -uid "DCC46D2B-9640-0B5E-21EF-75AC87B60786";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC32";
	rename -uid "B24FB246-7D48-1FA0-842E-8ABDE6D576D8";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC32";
	rename -uid "67EC708B-2142-C94B-8B64-75A2EAE86705";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC33";
	rename -uid "055CF087-1D41-4BB8-6264-4DB6C06DEEDA";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC33";
	rename -uid "0C5C9A2B-F441-D0AF-F3EC-50A9E5F2E16C";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC33";
	rename -uid "11F665B7-0145-8F92-9D49-76B06AAD76B5";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC33";
	rename -uid "D3A9978F-C449-E7BF-6E56-4AA59819D210";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC34";
	rename -uid "D1DE3608-8B4B-E667-637E-76BF1A5F4853";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC34";
	rename -uid "3C945D52-D341-892D-8CD0-AEA3546BF929";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC34";
	rename -uid "4469785F-8F47-1232-DE26-2D838B55B32C";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC34";
	rename -uid "5331C728-4C48-E88A-6DF5-CC921A7119BD";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC35";
	rename -uid "FC9EF1B3-F04F-63D0-F328-29B345F0B7A8";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC35";
	rename -uid "62FA5A66-3C4F-3675-6FF5-54975D96F87B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC35";
	rename -uid "CC1511BA-E948-CA4E-F28C-499010ECC0C2";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC35";
	rename -uid "8C72A4E7-8847-37DC-6853-E8A7B41CAF9E";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC36";
	rename -uid "2D36E58A-1A4B-351E-12AD-24A36AD7C1C8";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC36";
	rename -uid "0816E046-144F-3125-945F-CAA9B75A1FE2";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC36";
	rename -uid "D8B23053-F04C-39E2-1B39-F9A353845B7B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC36";
	rename -uid "A8B02B42-494F-3202-1510-B3BF6B02BC59";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC37";
	rename -uid "1759CFAE-3D48-4F42-F502-7EBE4866BF3B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC37";
	rename -uid "403578E1-3F45-26C3-E3D8-D796A15D9351";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC37";
	rename -uid "DC44C5E8-EC48-3528-B24C-0CAF8CBD66FD";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC37";
	rename -uid "579FD3E5-A141-98AA-63AF-66BDE6E96FCB";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC38";
	rename -uid "03CF0E40-8949-DEC5-E332-B09E6097105F";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC38";
	rename -uid "8FF152A0-F14E-543C-5633-28AC44E20272";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC38";
	rename -uid "7A528682-494B-C009-4B4C-F0ADD4746DB6";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC38";
	rename -uid "95DE6C98-8247-0E36-B22E-CCA4D0FFAAB5";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC39";
	rename -uid "67CB6026-344D-A046-EF63-86B56182BE6D";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC39";
	rename -uid "3A852628-1A40-A1CA-8A3B-109303352BAC";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC39";
	rename -uid "0902862A-D34F-9CAE-A4C3-ADB33ECD45C3";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC39";
	rename -uid "976045C1-2148-16A2-8158-34B86CA5D8FC";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC40";
	rename -uid "02632B1C-C94A-DD5F-4C37-3082A3AED2C5";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC40";
	rename -uid "F4E17EA5-B242-1E3B-7180-1F9485D105CD";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC40";
	rename -uid "249741D3-B141-B82A-172F-A58B880025F5";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC40";
	rename -uid "73D09D6B-B04A-4866-26F1-DB9B05A8A734";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC41";
	rename -uid "9D1CA771-B04C-6820-91CF-E380385884A3";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC41";
	rename -uid "9B203C0E-264F-1A51-D7D3-B6822284492E";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC41";
	rename -uid "34BA8085-3647-3986-D26A-409465B1B913";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC41";
	rename -uid "42143702-8246-F663-66E1-2384A5A5EED9";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC42";
	rename -uid "A52CF655-824E-5627-3668-E4949A1AF293";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC42";
	rename -uid "AB758C2A-3B46-14EB-8C81-A0A0F580C8E8";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC42";
	rename -uid "45CA385A-8345-AEC1-CE55-259F25CAC58C";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC42";
	rename -uid "8DB7D682-0248-4079-50EE-6DAFF44BD4B5";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC43";
	rename -uid "F5AF1222-9045-1587-D703-A587BE1F0907";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC43";
	rename -uid "5C7398AE-774D-F3DF-3C48-E0A66C6D12F0";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC43";
	rename -uid "EDA93152-F64A-3515-DD79-889BEF2DF148";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC43";
	rename -uid "C082B3C3-B04C-ABB1-5D1C-1E856569543A";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC44";
	rename -uid "FFEA94B5-5141-6881-3DF3-1C8936BDBC56";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC44";
	rename -uid "49F0DEBC-6742-315D-1805-D49657884F35";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC44";
	rename -uid "2973EE77-1346-C771-7538-7EB2CF749C7A";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC44";
	rename -uid "706BCF29-BF4F-85C7-BB2D-06B4182FF0FC";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC45";
	rename -uid "B358E311-7B41-E8AD-AD37-DBAF208F4147";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC45";
	rename -uid "5C5F3A80-E340-D281-57FB-21A6EE6CB58A";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC45";
	rename -uid "EB07592A-814E-1B1C-71F6-2A997C9D9658";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC45";
	rename -uid "50D454EB-4A4C-F3BB-6AA2-02B4A67FB4AD";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC46";
	rename -uid "D1D84740-AC43-2B8F-21EE-89B936BCBEBF";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC46";
	rename -uid "F6BAA168-A641-EE37-1949-DEABA901C2BA";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC46";
	rename -uid "52DD16C2-A447-67C4-C872-0CAD36EC7068";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC46";
	rename -uid "1E247792-0546-0DB8-80A4-72892F3DA31E";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC47";
	rename -uid "A1E53F67-2F45-9D0C-614C-F4A3BC0CDBC3";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC47";
	rename -uid "70AF3C7B-7E43-2D3C-EE80-42B9CD850B73";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC47";
	rename -uid "1D18AE8C-3247-CD26-DF5F-4480D99CBD35";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC47";
	rename -uid "98D5F02D-FD48-99E0-FC5A-07B6F32E9824";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC48";
	rename -uid "2F524204-C54F-5026-6B1C-5FAC42BC811F";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC48";
	rename -uid "AEAD05E4-D346-1392-D340-DAAC3BFE9DD3";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC48";
	rename -uid "3E09C76F-0545-7BF1-FCAC-18A2AE66067B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC48";
	rename -uid "BE94F16A-864C-31FE-5841-0EBF953D66B2";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 25 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC58";
	rename -uid "ADB3152D-794C-D20B-7435-649C1AB8B7AE";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC58";
	rename -uid "9A1E0F67-C64F-086C-AECF-6EA4797C3BD0";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC58";
	rename -uid "38B1284D-FF46-D9A1-424E-5A8D39BEAAC6";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC58";
	rename -uid "9B041E9B-7C45-0BDF-B78A-EDBF0C372EB5";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC59";
	rename -uid "F26DEA23-1646-7B5D-1ED1-FBA5F60BB69B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC59";
	rename -uid "366733F7-454E-FB13-93DA-788AE1816D60";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC59";
	rename -uid "35E8FB59-9D43-3BD2-9CEA-CBB2C0FCD0B7";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC59";
	rename -uid "BC21CD30-6A41-AA96-5E45-4E8585CB867C";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC60";
	rename -uid "4973F456-A443-4C96-837F-C9A063A88FCF";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC60";
	rename -uid "AB1CFCF5-4C4B-CCAA-1F01-69B3D52F322F";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC60";
	rename -uid "322EC7DB-7741-DC85-ABAB-6C9AAA8F1CE6";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC60";
	rename -uid "454AA934-F140-C60F-5E0F-F083B3DBB301";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC61";
	rename -uid "EAB89806-E346-98C8-C614-98BF17F98F39";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC61";
	rename -uid "596959AD-2942-6D08-A177-E79DCC3E5122";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC61";
	rename -uid "65D97CB6-7542-D4F3-0BEA-23BD33A02BD4";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC61";
	rename -uid "249D967B-734D-8FE2-7775-25B0A19AF8E2";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC62";
	rename -uid "52227ED4-E84B-8756-00F5-9F81847492D7";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC62";
	rename -uid "EFF9A065-394C-8027-7F11-95BB51C1CDA6";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC62";
	rename -uid "93F7C224-F444-E97A-3DE0-A6B16B244913";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC62";
	rename -uid "B546D1EE-9E4C-F41E-C707-EDBD951F19DA";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC63";
	rename -uid "5A0E29DB-644A-6C93-5C36-20A3D2C2752B";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC63";
	rename -uid "90337541-2340-CEAB-8B64-DC88886013CC";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC63";
	rename -uid "F87B9F00-6748-827B-51B6-0EB2D33ECE87";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC63";
	rename -uid "56E4EE71-244E-09E5-2A0E-EA952FAF646E";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC64";
	rename -uid "805C7C24-9940-413B-9649-55A0CD4319AF";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC64";
	rename -uid "045FCFFD-DA43-61D8-431B-68B8B389EAE5";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC64";
	rename -uid "9894B40F-ED45-DFAC-B46F-A7B7A95E947A";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC64";
	rename -uid "42155658-F248-523E-EFF5-D794F82B2DBC";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC65";
	rename -uid "0697C73A-4645-9CF7-B3B0-E0B64CDEAD3F";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC65";
	rename -uid "95A26295-CA4D-63E2-F546-AF9B7E829F18";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC65";
	rename -uid "DF2B905F-7740-0DAB-718F-A4827AF1705A";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC65";
	rename -uid "6A41EC49-1248-52D1-B15E-969C248AD390";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC66";
	rename -uid "147D4C9A-A348-298A-A0A7-40A95455CFBE";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC66";
	rename -uid "E910F49D-7F40-0C25-F3BB-E986A7C91D09";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC66";
	rename -uid "8D95213F-B140-4DA9-4981-21991A18D25D";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC66";
	rename -uid "4394E7E4-1A45-5C55-B397-45AEF79C05E0";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC67";
	rename -uid "06C0F892-E847-2BC0-D753-60AC602A3051";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC67";
	rename -uid "76E42DD0-7E47-2786-9DAC-6B9554E6B2D4";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC67";
	rename -uid "D5533C65-984C-9A6B-F154-ABAA578D99A0";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC67";
	rename -uid "DA9C80CF-F540-BE77-4F61-C9915B90363C";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC68";
	rename -uid "40CD8F8E-A04C-86D4-F0BB-31B80D868B64";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC68";
	rename -uid "CF7D65C5-D243-FA44-984C-F4956B9F9030";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC68";
	rename -uid "2291A7CD-7A41-338B-5E46-9388EA404888";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC68";
	rename -uid "ADD046DC-F54F-EE12-A3C4-5B955AF16BFA";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC69";
	rename -uid "E4BEAF6E-9844-77FB-8935-178CEF0B1EC9";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC69";
	rename -uid "5C8FD9DE-CC4D-CB54-1290-67AE073E3DE1";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC69";
	rename -uid "0E617B21-CD4C-A8DC-133F-778257576C4E";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC69";
	rename -uid "F29B6047-4646-2F1B-2BBA-7BB4F5901BFE";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC70";
	rename -uid "22AE6CD3-DE4F-097A-612E-AA8ED4743E4A";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC70";
	rename -uid "5D1A59DC-8C46-05EC-5402-82B51517CC62";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC70";
	rename -uid "F1EA467F-0147-ABEB-4C5A-D48CB6E20224";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC70";
	rename -uid "11CA75A0-534F-6D10-9E38-D884CCAE4DD9";
	setAttr ".stl" 10000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC71";
	rename -uid "982419D9-EF46-426E-57FE-7BAAD5DD0455";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC71";
	rename -uid "AF5CBF9C-1944-EDFC-DED3-99BC379F3FF6";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC71";
	rename -uid "BC4917E2-6E48-1302-9B95-B3B60F593E24";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC71";
	rename -uid "1B81B287-B84A-0C73-454F-78B5F87FD604";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 6;
	setAttr ".ptid" 4;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC72";
	rename -uid "33590174-EC41-EE91-B1C6-0A9BCCC1DB76";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC72";
	rename -uid "13228FF5-E44B-651F-71FB-E49907A4CB30";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC72";
	rename -uid "2A78C5AA-B54B-08FC-C6F5-5AB76891D984";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC72";
	rename -uid "04AD8F68-D944-9BFB-89F4-27BBF5E34299";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 2;
	setAttr ".ptid" 2;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC73";
	rename -uid "A474500D-1644-E75D-A5F5-07B9516517D3";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC73";
	rename -uid "D8C0C1E1-CF42-AEC5-834B-79A97263CBD1";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC73";
	rename -uid "B11A2A22-984A-6FC8-E941-DF80B0BFEA8B";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC73";
	rename -uid "4D2ECA2E-6B4A-CF27-D599-4C8A458D77DB";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 7;
	setAttr ".ptid" 5;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC74";
	rename -uid "01AA486F-A54E-428C-1C13-3D8038DF2548";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC74";
	rename -uid "DD413D94-B94A-9E1A-4FEB-34AC0CB3040C";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC74";
	rename -uid "96A603F0-B34A-6B8A-C38E-BFB4F483C981";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC74";
	rename -uid "9320921A-414E-2EB0-BA5B-9AB33DEEFF30";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 5;
	setAttr ".ptid" 7;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC75";
	rename -uid "82FA2250-0445-7A36-562F-9988F9416F0B";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC75";
	rename -uid "78EAE2B8-8A42-F036-EC78-B1B6BDEEE52B";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC75";
	rename -uid "9A9672DE-FF4B-8F39-5EAC-40BDD86AFE94";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC75";
	rename -uid "49FA0594-DB49-67F0-13B2-72AFD5ED218E";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 8;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC76";
	rename -uid "15E378C3-394A-B3D2-D4E9-A094DA1BC146";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC76";
	rename -uid "00836657-344E-F068-622E-819CB248BDCE";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC76";
	rename -uid "CF7A9CC6-1E42-9626-91F6-658E23CD9FA1";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC76";
	rename -uid "2E0071E2-724A-0F00-F1A6-B594A2643792";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 4;
	setAttr ".ptid" 1;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC77";
	rename -uid "49592EF5-5344-F4B6-2433-DC95FD7A596B";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC77";
	rename -uid "FFC144D7-0646-ADD5-A3BC-639CEEAB4602";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC77";
	rename -uid "CDA7032F-2C45-9971-A503-05A7A24B31ED";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC77";
	rename -uid "ABA480B1-FA4C-33D9-1058-E2A8C03EE601";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 1;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC78";
	rename -uid "E3208633-8843-F992-2898-3A8AD8EA78F1";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC78";
	rename -uid "010A24E6-2F4E-0CD2-3389-0D94E940EF5B";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC78";
	rename -uid "AC9D1AC4-B140-E6D2-25A1-87BB427530BB";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC78";
	rename -uid "AA28DF05-5D41-82FF-99DA-6E97F584F459";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 3;
	setAttr ".ptid" 6;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode strokeCurve -n "sourceDipShape1_SC79";
	rename -uid "49936ABE-4D4E-628A-40E4-1F97400AA799";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".fcdp" yes;
	setAttr ".apd" -type "double3" 25 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.25 1;
createNode strokeCurve -n "sourceDipShape2_SC79";
	rename -uid "4143326A-954C-C35A-73F1-2EA1134549E3";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.75 1;
createNode strokeCurve -n "sourceDipShape3_SC79";
	rename -uid "E7740757-E14E-1964-9120-96B185A8F57E";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 3 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0.5 1;
createNode strokeCurve -n "sourceDipShape4_SC79";
	rename -uid "94CC2A98-7546-DBAE-C14D-C484111449E0";
	setAttr ".stl" 1000;
	setAttr ".rpmr" no;
	setAttr ".rpoc" no;
	setAttr ".brid" 9;
	setAttr ".ptid" 3;
	setAttr ".fst" no;
	setAttr ".apd" -type "double3" 3 3 10 ;
	setAttr ".spr[0]"  0 0.5 1;
	setAttr ".brtl[0]"  0 0.5 1;
	setAttr ".brbk[0]"  0 0.5 1;
	setAttr ".brtw[0]"  0 0 1;
createNode nodeGraphEditorInfo -n "MayaNodeEditorSavedTabsInfo";
	rename -uid "6F21A988-1042-7D30-2D4D-38BBE987D53F";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" 7298.8092337809039 2087.9223440687506 ;
	setAttr ".tgi[0].vh" -type "double2" 8814.2853640374833 3065.6488797181569 ;
	setAttr -s 7 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" 8148.17578125;
	setAttr ".tgi[0].ni[0].y" 2863.492431640625;
	setAttr ".tgi[0].ni[0].nvs" 18306;
	setAttr ".tgi[0].ni[1].x" 8250.0576171875;
	setAttr ".tgi[0].ni[1].y" 3006.49658203125;
	setAttr ".tgi[0].ni[1].nvs" 18305;
	setAttr ".tgi[0].ni[2].x" 7530.44189453125;
	setAttr ".tgi[0].ni[2].y" 2586.361328125;
	setAttr ".tgi[0].ni[2].nvs" 18305;
	setAttr ".tgi[0].ni[3].x" 7716.57080078125;
	setAttr ".tgi[0].ni[3].y" 2893.210693359375;
	setAttr ".tgi[0].ni[3].nvs" 18305;
	setAttr ".tgi[0].ni[4].x" 8117.39697265625;
	setAttr ".tgi[0].ni[4].y" 2653.744873046875;
	setAttr ".tgi[0].ni[4].nvs" 18305;
	setAttr ".tgi[0].ni[5].x" 8542.4228515625;
	setAttr ".tgi[0].ni[5].y" 2975.565673828125;
	setAttr ".tgi[0].ni[5].nvs" 18306;
	setAttr ".tgi[0].ni[6].x" 8012.85693359375;
	setAttr ".tgi[0].ni[6].y" 2941.428466796875;
	setAttr ".tgi[0].ni[6].nvs" 18304;
select -ne :time1;
	setAttr ".o" 120;
	setAttr ".unw" 120;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -s 16 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 18 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
select -ne :defaultRenderingList1;
select -ne :defaultTextureList1;
select -ne :initialShadingGroup;
	setAttr -s 26 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 26 ".gn";
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :ikSystem;
	setAttr -s 2 ".sol";
connectAttr "pasted__layer8.di" "mainProjection.do";
connectAttr "pasted__expression5.out[0]" "|mainPaintingGroup|jpos.tx";
connectAttr "pasted__expression5.out[1]" "|mainPaintingGroup|jpos.ty";
connectAttr "pasted__expression5.out[2]" "|mainPaintingGroup|jpos.tz";
connectAttr "|mainPaintingGroup|jpos.s" "|mainPaintingGroup|jpos|j2.is";
connectAttr "|mainPaintingGroup|jpos|j2.tx" "pasted__effector1.tx";
connectAttr "|mainPaintingGroup|jpos|j2.ty" "pasted__effector1.ty";
connectAttr "|mainPaintingGroup|jpos|j2.tz" "pasted__effector1.tz";
connectAttr "cShape9_SC.out" "mainPaintingShape.scrvs[3]";
connectAttr "cShape8_SC.out" "mainPaintingShape.scrvs[4]";
connectAttr "cShape7_SC.out" "mainPaintingShape.scrvs[5]";
connectAttr "cShape6_SC.out" "mainPaintingShape.scrvs[6]";
connectAttr "cShape5_SC.out" "mainPaintingShape.scrvs[7]";
connectAttr "cShape4_SC.out" "mainPaintingShape.scrvs[8]";
connectAttr "cShape3_SC.out" "mainPaintingShape.scrvs[9]";
connectAttr "cShape2_SC.out" "mainPaintingShape.scrvs[10]";
connectAttr "cShape1_SC.out" "mainPaintingShape.scrvs[11]";
connectAttr "bpx_0_4mm_round_soft.m" "mainPaintingShape.bsh[0].bmat";
connectAttr "bpx_0_4mm_round_soft.sfBrushWidth" "mainPaintingShape.bsh[0].brwd";
connectAttr "bpx_0_4mm_round_soft.sfBrushRetention" "mainPaintingShape.bsh[0].brrt"
		;
connectAttr "bpx_0_4mm_round_soft.sfBrushShape" "mainPaintingShape.bsh[0].bshp";
connectAttr "bpx_1_6mm_round_syn.m" "mainPaintingShape.bsh[1].bmat";
connectAttr "bpx_1_6mm_round_syn.sfBrushWidth" "mainPaintingShape.bsh[1].brwd";
connectAttr "bpx_1_6mm_round_syn.sfBrushRetention" "mainPaintingShape.bsh[1].brrt"
		;
connectAttr "bpx_1_6mm_round_syn.sfBrushShape" "mainPaintingShape.bsh[1].bshp";
connectAttr "bpx_2_8mm_round_soft.m" "mainPaintingShape.bsh[2].bmat";
connectAttr "bpx_2_8mm_round_soft.sfBrushWidth" "mainPaintingShape.bsh[2].brwd";
connectAttr "bpx_2_8mm_round_soft.sfBrushRetention" "mainPaintingShape.bsh[2].brrt"
		;
connectAttr "bpx_2_8mm_round_soft.sfBrushShape" "mainPaintingShape.bsh[2].bshp";
connectAttr "bpx_3_10mm_flat_na.m" "mainPaintingShape.bsh[3].bmat";
connectAttr "bpx_3_10mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[3].brwd";
connectAttr "bpx_3_10mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[3].brrt"
		;
connectAttr "bpx_3_10mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[3].bshp";
connectAttr "bpx_4_13mm_flat_na.m" "mainPaintingShape.bsh[4].bmat";
connectAttr "bpx_4_13mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[4].brwd";
connectAttr "bpx_4_13mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[4].brrt"
		;
connectAttr "bpx_4_13mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[4].bshp";
connectAttr "bpx_5_15mm_flat_na.m" "mainPaintingShape.bsh[5].bmat";
connectAttr "bpx_5_15mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[5].brwd";
connectAttr "bpx_5_15mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[5].brrt"
		;
connectAttr "bpx_5_15mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[5].bshp";
connectAttr "bpx_6_17mm_flat_na.m" "mainPaintingShape.bsh[6].bmat";
connectAttr "bpx_6_17mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[6].brwd";
connectAttr "bpx_6_17mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[6].brrt"
		;
connectAttr "bpx_6_17mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[6].bshp";
connectAttr "bpx_7_20mm_flat_na.m" "mainPaintingShape.bsh[7].bmat";
connectAttr "bpx_7_20mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[7].brwd";
connectAttr "bpx_7_20mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[7].brrt"
		;
connectAttr "bpx_7_20mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[7].bshp";
connectAttr "bpx_8_28mm_flat_na.m" "mainPaintingShape.bsh[8].bmat";
connectAttr "bpx_8_28mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[8].brwd";
connectAttr "bpx_8_28mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[8].brrt"
		;
connectAttr "bpx_8_28mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[8].bshp";
connectAttr "bpx_9_35mm_flat_na.m" "mainPaintingShape.bsh[9].bmat";
connectAttr "bpx_9_35mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[9].brwd";
connectAttr "bpx_9_35mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[9].brrt"
		;
connectAttr "bpx_9_35mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[9].bshp";
connectAttr "bpx_10_40mm_flat_na.m" "mainPaintingShape.bsh[10].bmat";
connectAttr "bpx_10_40mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[10].brwd";
connectAttr "bpx_10_40mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[10].brrt"
		;
connectAttr "bpx_10_40mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[10].bshp";
connectAttr "bpx_11_52mm_flat_na.m" "mainPaintingShape.bsh[11].bmat";
connectAttr "bpx_11_52mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[11].brwd";
connectAttr "bpx_11_52mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[11].brrt"
		;
connectAttr "bpx_11_52mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[11].bshp";
connectAttr "bpx_12_78mm_flat_na.m" "mainPaintingShape.bsh[12].bmat";
connectAttr "bpx_12_78mm_flat_na.sfBrushWidth" "mainPaintingShape.bsh[12].brwd";
connectAttr "bpx_12_78mm_flat_na.sfBrushRetention" "mainPaintingShape.bsh[12].brrt"
		;
connectAttr "bpx_12_78mm_flat_na.sfBrushShape" "mainPaintingShape.bsh[12].bshp";
connectAttr "tx_0_reddish.sfPaintColor" "mainPaintingShape.pts[0].pc";
connectAttr "tx_0_reddish.sfPaintOpacity" "mainPaintingShape.pts[0].pvis";
connectAttr "tx_0_reddish.sfPaintTravel" "mainPaintingShape.pts[0].ptvl";
connectAttr "tx_1_sky.sfPaintColor" "mainPaintingShape.pts[1].pc";
connectAttr "tx_1_sky.sfPaintOpacity" "mainPaintingShape.pts[1].pvis";
connectAttr "tx_1_sky.sfPaintTravel" "mainPaintingShape.pts[1].ptvl";
connectAttr "tx_2_orange.sfPaintColor" "mainPaintingShape.pts[2].pc";
connectAttr "tx_2_orange.sfPaintOpacity" "mainPaintingShape.pts[2].pvis";
connectAttr "tx_2_orange.sfPaintTravel" "mainPaintingShape.pts[2].ptvl";
connectAttr "tx_3_purple.sfPaintColor" "mainPaintingShape.pts[3].pc";
connectAttr "tx_3_purple.sfPaintOpacity" "mainPaintingShape.pts[3].pvis";
connectAttr "tx_3_purple.sfPaintTravel" "mainPaintingShape.pts[3].ptvl";
connectAttr "tx_4_turqoise.sfPaintColor" "mainPaintingShape.pts[4].pc";
connectAttr "tx_4_turqoise.sfPaintOpacity" "mainPaintingShape.pts[4].pvis";
connectAttr "tx_4_turqoise.sfPaintTravel" "mainPaintingShape.pts[4].ptvl";
connectAttr "tx_5_green.sfPaintColor" "mainPaintingShape.pts[5].pc";
connectAttr "tx_5_green.sfPaintOpacity" "mainPaintingShape.pts[5].pvis";
connectAttr "tx_5_green.sfPaintTravel" "mainPaintingShape.pts[5].ptvl";
connectAttr "tx_6_canary.sfPaintColor" "mainPaintingShape.pts[6].pc";
connectAttr "tx_6_canary.sfPaintOpacity" "mainPaintingShape.pts[6].pvis";
connectAttr "tx_6_canary.sfPaintTravel" "mainPaintingShape.pts[6].ptvl";
connectAttr "tx_7_black.sfPaintColor" "mainPaintingShape.pts[7].pc";
connectAttr "tx_7_black.sfPaintOpacity" "mainPaintingShape.pts[7].pvis";
connectAttr "tx_7_black.sfPaintTravel" "mainPaintingShape.pts[7].ptvl";
connectAttr "dipTarget.msg" "mainPaintingShape.dao";
connectAttr "toolChangeTarget.msg" "mainPaintingShape.tcao";
connectAttr "homeTarget.msg" "mainPaintingShape.hmao";
connectAttr "|mainPaintingGroup|jpos.msg" "|mainPaintingGroup|ikHandle.hsj";
connectAttr "pasted__effector1.hp" "|mainPaintingGroup|ikHandle.hee";
connectAttr "pasted__ikRPsolver.msg" "|mainPaintingGroup|ikHandle.hsv";
connectAttr "pasted__ikHandle1_poleVectorConstraint1.ctx" "|mainPaintingGroup|ikHandle.pvx"
		;
connectAttr "pasted__ikHandle1_poleVectorConstraint1.cty" "|mainPaintingGroup|ikHandle.pvy"
		;
connectAttr "pasted__ikHandle1_poleVectorConstraint1.ctz" "|mainPaintingGroup|ikHandle.pvz"
		;
connectAttr "pasted__expression5.out[3]" "|mainPaintingGroup|ikHandle.tx";
connectAttr "pasted__expression5.out[4]" "|mainPaintingGroup|ikHandle.ty";
connectAttr "pasted__expression5.out[5]" "|mainPaintingGroup|ikHandle.tz";
connectAttr "|mainPaintingGroup|ikHandle.pim" "pasted__ikHandle1_poleVectorConstraint1.cpim"
		;
connectAttr "|mainPaintingGroup|jpos.pm" "pasted__ikHandle1_poleVectorConstraint1.ps"
		;
connectAttr "|mainPaintingGroup|jpos.t" "pasted__ikHandle1_poleVectorConstraint1.crp"
		;
connectAttr "TR.t" "pasted__ikHandle1_poleVectorConstraint1.tg[0].tt";
connectAttr "TR.rp" "pasted__ikHandle1_poleVectorConstraint1.tg[0].trp";
connectAttr "TR.rpt" "pasted__ikHandle1_poleVectorConstraint1.tg[0].trt";
connectAttr "TR.pm" "pasted__ikHandle1_poleVectorConstraint1.tg[0].tpm";
connectAttr "pasted__ikHandle1_poleVectorConstraint1.w0" "pasted__ikHandle1_poleVectorConstraint1.tg[0].tw"
		;
connectAttr "TRZ.t" "pasted__ikHandle1_poleVectorConstraint1.tg[1].tt";
connectAttr "TRZ.rp" "pasted__ikHandle1_poleVectorConstraint1.tg[1].trp";
connectAttr "TRZ.rpt" "pasted__ikHandle1_poleVectorConstraint1.tg[1].trt";
connectAttr "TRZ.pm" "pasted__ikHandle1_poleVectorConstraint1.tg[1].tpm";
connectAttr "pasted__ikHandle1_poleVectorConstraint1.w1" "pasted__ikHandle1_poleVectorConstraint1.tg[1].tw"
		;
connectAttr "pasted__expression5.out[6]" "pasted__ikHandle1_poleVectorConstraint1.w0"
		;
connectAttr "pasted__expression5.out[7]" "pasted__ikHandle1_poleVectorConstraint1.w1"
		;
connectAttr "jPcon.ctx" "|dipPaintingGroup|jpos.tx";
connectAttr "jPcon.cty" "|dipPaintingGroup|jpos.ty";
connectAttr "jPcon.ctz" "|dipPaintingGroup|jpos.tz";
connectAttr "|dipPaintingGroup|jpos.s" "|dipPaintingGroup|jpos|j2.is";
connectAttr "|dipPaintingGroup|jpos|j2.tx" "effector1.tx";
connectAttr "|dipPaintingGroup|jpos|j2.ty" "effector1.ty";
connectAttr "|dipPaintingGroup|jpos|j2.tz" "effector1.tz";
connectAttr "|dipPaintingGroup|jpos.pim" "jPcon.cpim";
connectAttr "|dipPaintingGroup|jpos.rp" "jPcon.crp";
connectAttr "|dipPaintingGroup|jpos.rpt" "jPcon.crt";
connectAttr "rack_LZ.t" "jPcon.tg[0].tt";
connectAttr "rack_LZ.rp" "jPcon.tg[0].trp";
connectAttr "rack_LZ.rpt" "jPcon.tg[0].trt";
connectAttr "rack_LZ.pm" "jPcon.tg[0].tpm";
connectAttr "jPcon.w0" "jPcon.tg[0].tw";
connectAttr "rack_L.t" "jPcon.tg[1].tt";
connectAttr "rack_L.rp" "jPcon.tg[1].trp";
connectAttr "rack_L.rpt" "jPcon.tg[1].trt";
connectAttr "rack_L.pm" "jPcon.tg[1].tpm";
connectAttr "jPcon.w1" "jPcon.tg[1].tw";
connectAttr "expression1.out[0]" "jPcon.w0";
connectAttr "expression1.out[1]" "jPcon.w1";
connectAttr "bdx_0_4mm_round_soft.m" "dipPaintingShape.bsh[0].bmat";
connectAttr "bdx_0_4mm_round_soft.sfBrushWidth" "dipPaintingShape.bsh[0].brwd";
connectAttr "bdx_0_4mm_round_soft.sfBrushRetention" "dipPaintingShape.bsh[0].brrt"
		;
connectAttr "bdx_0_4mm_round_soft.sfBrushShape" "dipPaintingShape.bsh[0].bshp";
connectAttr "bdx_1_6mm_round_syn.m" "dipPaintingShape.bsh[1].bmat";
connectAttr "bdx_1_6mm_round_syn.sfBrushWidth" "dipPaintingShape.bsh[1].brwd";
connectAttr "bdx_1_6mm_round_syn.sfBrushRetention" "dipPaintingShape.bsh[1].brrt"
		;
connectAttr "bdx_1_6mm_round_syn.sfBrushShape" "dipPaintingShape.bsh[1].bshp";
connectAttr "bdx_2_8mm_round_soft.m" "dipPaintingShape.bsh[2].bmat";
connectAttr "bdx_2_8mm_round_soft.sfBrushWidth" "dipPaintingShape.bsh[2].brwd";
connectAttr "bdx_2_8mm_round_soft.sfBrushRetention" "dipPaintingShape.bsh[2].brrt"
		;
connectAttr "bdx_2_8mm_round_soft.sfBrushShape" "dipPaintingShape.bsh[2].bshp";
connectAttr "bdx_3_10mm_flat_na.m" "dipPaintingShape.bsh[3].bmat";
connectAttr "bdx_3_10mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[3].brwd";
connectAttr "bdx_3_10mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[3].brrt"
		;
connectAttr "bdx_3_10mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[3].bshp";
connectAttr "bdx_4_13mm_flat_na.m" "dipPaintingShape.bsh[4].bmat";
connectAttr "bdx_4_13mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[4].brwd";
connectAttr "bdx_4_13mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[4].brrt"
		;
connectAttr "bdx_4_13mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[4].bshp";
connectAttr "bdx_5_15mm_flat_na.m" "dipPaintingShape.bsh[5].bmat";
connectAttr "bdx_5_15mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[5].brwd";
connectAttr "bdx_5_15mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[5].brrt"
		;
connectAttr "bdx_5_15mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[5].bshp";
connectAttr "bdx_6_17mm_flat_na.m" "dipPaintingShape.bsh[6].bmat";
connectAttr "bdx_6_17mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[6].brwd";
connectAttr "bdx_6_17mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[6].brrt"
		;
connectAttr "bdx_6_17mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[6].bshp";
connectAttr "bdx_7_20mm_flat_na.m" "dipPaintingShape.bsh[7].bmat";
connectAttr "bdx_7_20mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[7].brwd";
connectAttr "bdx_7_20mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[7].brrt"
		;
connectAttr "bdx_7_20mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[7].bshp";
connectAttr "bdx_8_28mm_flat_na.m" "dipPaintingShape.bsh[8].bmat";
connectAttr "bdx_8_28mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[8].brwd";
connectAttr "bdx_8_28mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[8].brrt"
		;
connectAttr "bdx_8_28mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[8].bshp";
connectAttr "bdx_9_35mm_flat_na.m" "dipPaintingShape.bsh[9].bmat";
connectAttr "bdx_9_35mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[9].brwd";
connectAttr "bdx_9_35mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[9].brrt"
		;
connectAttr "bdx_9_35mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[9].bshp";
connectAttr "bdx_10_40mm_flat_na.m" "dipPaintingShape.bsh[10].bmat";
connectAttr "bdx_10_40mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[10].brwd";
connectAttr "bdx_10_40mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[10].brrt"
		;
connectAttr "bdx_10_40mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[10].bshp";
connectAttr "bdx_11_52mm_flat_na.m" "dipPaintingShape.bsh[11].bmat";
connectAttr "bdx_11_52mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[11].brwd";
connectAttr "bdx_11_52mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[11].brrt"
		;
connectAttr "bdx_11_52mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[11].bshp";
connectAttr "bdx_12_78mm_flat_na.m" "dipPaintingShape.bsh[12].bmat";
connectAttr "bdx_12_78mm_flat_na.sfBrushWidth" "dipPaintingShape.bsh[12].brwd";
connectAttr "bdx_12_78mm_flat_na.sfBrushRetention" "dipPaintingShape.bsh[12].brrt"
		;
connectAttr "bdx_12_78mm_flat_na.sfBrushShape" "dipPaintingShape.bsh[12].bshp";
connectAttr "tx_0_reddish.sfPaintColor" "dipPaintingShape.pts[0].pc";
connectAttr "tx_0_reddish.sfPaintOpacity" "dipPaintingShape.pts[0].pvis";
connectAttr "tx_0_reddish.sfPaintTravel" "dipPaintingShape.pts[0].ptvl";
connectAttr "tx_1_sky.sfPaintColor" "dipPaintingShape.pts[1].pc";
connectAttr "tx_1_sky.sfPaintOpacity" "dipPaintingShape.pts[1].pvis";
connectAttr "tx_1_sky.sfPaintTravel" "dipPaintingShape.pts[1].ptvl";
connectAttr "tx_2_orange.sfPaintColor" "dipPaintingShape.pts[2].pc";
connectAttr "tx_2_orange.sfPaintOpacity" "dipPaintingShape.pts[2].pvis";
connectAttr "tx_2_orange.sfPaintTravel" "dipPaintingShape.pts[2].ptvl";
connectAttr "tx_3_purple.sfPaintColor" "dipPaintingShape.pts[3].pc";
connectAttr "tx_3_purple.sfPaintOpacity" "dipPaintingShape.pts[3].pvis";
connectAttr "tx_3_purple.sfPaintTravel" "dipPaintingShape.pts[3].ptvl";
connectAttr "tx_4_turqoise.sfPaintColor" "dipPaintingShape.pts[4].pc";
connectAttr "tx_4_turqoise.sfPaintOpacity" "dipPaintingShape.pts[4].pvis";
connectAttr "tx_4_turqoise.sfPaintTravel" "dipPaintingShape.pts[4].ptvl";
connectAttr "tx_5_green.sfPaintColor" "dipPaintingShape.pts[5].pc";
connectAttr "tx_5_green.sfPaintOpacity" "dipPaintingShape.pts[5].pvis";
connectAttr "tx_5_green.sfPaintTravel" "dipPaintingShape.pts[5].ptvl";
connectAttr "tx_6_canary.sfPaintColor" "dipPaintingShape.pts[6].pc";
connectAttr "tx_6_canary.sfPaintOpacity" "dipPaintingShape.pts[6].pvis";
connectAttr "tx_6_canary.sfPaintTravel" "dipPaintingShape.pts[6].ptvl";
connectAttr "tx_7_black.sfPaintColor" "dipPaintingShape.pts[7].pc";
connectAttr "tx_7_black.sfPaintOpacity" "dipPaintingShape.pts[7].pvis";
connectAttr "tx_7_black.sfPaintTravel" "dipPaintingShape.pts[7].ptvl";
connectAttr "sourceDipShape1_SC71.out" "dipPaintingShape.scrvs[0]";
connectAttr "sourceDipShape2_SC71.out" "dipPaintingShape.scrvs[1]";
connectAttr "sourceDipShape3_SC71.out" "dipPaintingShape.scrvs[2]";
connectAttr "sourceDipShape4_SC71.out" "dipPaintingShape.scrvs[3]";
connectAttr "sourceDipShape1_SC72.out" "dipPaintingShape.scrvs[4]";
connectAttr "sourceDipShape2_SC72.out" "dipPaintingShape.scrvs[5]";
connectAttr "sourceDipShape3_SC72.out" "dipPaintingShape.scrvs[6]";
connectAttr "sourceDipShape4_SC72.out" "dipPaintingShape.scrvs[7]";
connectAttr "sourceDipShape1_SC73.out" "dipPaintingShape.scrvs[8]";
connectAttr "sourceDipShape2_SC73.out" "dipPaintingShape.scrvs[9]";
connectAttr "sourceDipShape3_SC73.out" "dipPaintingShape.scrvs[10]";
connectAttr "sourceDipShape4_SC73.out" "dipPaintingShape.scrvs[11]";
connectAttr "sourceDipShape1_SC74.out" "dipPaintingShape.scrvs[12]";
connectAttr "sourceDipShape2_SC74.out" "dipPaintingShape.scrvs[13]";
connectAttr "sourceDipShape3_SC74.out" "dipPaintingShape.scrvs[14]";
connectAttr "sourceDipShape4_SC74.out" "dipPaintingShape.scrvs[15]";
connectAttr "sourceDipShape1_SC75.out" "dipPaintingShape.scrvs[16]";
connectAttr "sourceDipShape2_SC75.out" "dipPaintingShape.scrvs[17]";
connectAttr "sourceDipShape3_SC75.out" "dipPaintingShape.scrvs[18]";
connectAttr "sourceDipShape4_SC75.out" "dipPaintingShape.scrvs[19]";
connectAttr "sourceDipShape1_SC76.out" "dipPaintingShape.scrvs[20]";
connectAttr "sourceDipShape2_SC76.out" "dipPaintingShape.scrvs[21]";
connectAttr "sourceDipShape3_SC76.out" "dipPaintingShape.scrvs[22]";
connectAttr "sourceDipShape4_SC76.out" "dipPaintingShape.scrvs[23]";
connectAttr "sourceDipShape1_SC77.out" "dipPaintingShape.scrvs[24]";
connectAttr "sourceDipShape2_SC77.out" "dipPaintingShape.scrvs[25]";
connectAttr "sourceDipShape3_SC77.out" "dipPaintingShape.scrvs[26]";
connectAttr "sourceDipShape4_SC77.out" "dipPaintingShape.scrvs[27]";
connectAttr "sourceDipShape1_SC78.out" "dipPaintingShape.scrvs[28]";
connectAttr "sourceDipShape2_SC78.out" "dipPaintingShape.scrvs[29]";
connectAttr "sourceDipShape3_SC78.out" "dipPaintingShape.scrvs[30]";
connectAttr "sourceDipShape4_SC78.out" "dipPaintingShape.scrvs[31]";
connectAttr "sourceDipShape1_SC79.out" "dipPaintingShape.scrvs[32]";
connectAttr "sourceDipShape2_SC79.out" "dipPaintingShape.scrvs[33]";
connectAttr "sourceDipShape3_SC79.out" "dipPaintingShape.scrvs[34]";
connectAttr "sourceDipShape4_SC79.out" "dipPaintingShape.scrvs[35]";
connectAttr "|dipPaintingGroup|jpos.msg" "|dipPaintingGroup|ikHandle.hsj";
connectAttr "effector1.hp" "|dipPaintingGroup|ikHandle.hee";
connectAttr "ikRPsolver.msg" "|dipPaintingGroup|ikHandle.hsv";
connectAttr "ikPvcon.ctx" "|dipPaintingGroup|ikHandle.pvx";
connectAttr "ikPvcon.cty" "|dipPaintingGroup|ikHandle.pvy";
connectAttr "ikPvcon.ctz" "|dipPaintingGroup|ikHandle.pvz";
connectAttr "ikPcon.ctx" "|dipPaintingGroup|ikHandle.tx";
connectAttr "ikPcon.cty" "|dipPaintingGroup|ikHandle.ty";
connectAttr "ikPcon.ctz" "|dipPaintingGroup|ikHandle.tz";
connectAttr "|dipPaintingGroup|ikHandle.pim" "ikPvcon.cpim";
connectAttr "|dipPaintingGroup|jpos.pm" "ikPvcon.ps";
connectAttr "|dipPaintingGroup|jpos.t" "ikPvcon.crp";
connectAttr "rack_P7Z.t" "ikPvcon.tg[0].tt";
connectAttr "rack_P7Z.rp" "ikPvcon.tg[0].trp";
connectAttr "rack_P7Z.rpt" "ikPvcon.tg[0].trt";
connectAttr "rack_P7Z.pm" "ikPvcon.tg[0].tpm";
connectAttr "ikPvcon.w0" "ikPvcon.tg[0].tw";
connectAttr "rack_P7.t" "ikPvcon.tg[1].tt";
connectAttr "rack_P7.rp" "ikPvcon.tg[1].trp";
connectAttr "rack_P7.rpt" "ikPvcon.tg[1].trt";
connectAttr "rack_P7.pm" "ikPvcon.tg[1].tpm";
connectAttr "ikPvcon.w1" "ikPvcon.tg[1].tw";
connectAttr "expression1.out[4]" "ikPvcon.w0";
connectAttr "expression1.out[5]" "ikPvcon.w1";
connectAttr "|dipPaintingGroup|ikHandle.pim" "ikPcon.cpim";
connectAttr "|dipPaintingGroup|ikHandle.rp" "ikPcon.crp";
connectAttr "|dipPaintingGroup|ikHandle.rpt" "ikPcon.crt";
connectAttr "rack_RZ.t" "ikPcon.tg[0].tt";
connectAttr "rack_RZ.rp" "ikPcon.tg[0].trp";
connectAttr "rack_RZ.rpt" "ikPcon.tg[0].trt";
connectAttr "rack_RZ.pm" "ikPcon.tg[0].tpm";
connectAttr "ikPcon.w0" "ikPcon.tg[0].tw";
connectAttr "rack_R.t" "ikPcon.tg[1].tt";
connectAttr "rack_R.rp" "ikPcon.tg[1].trp";
connectAttr "rack_R.rpt" "ikPcon.tg[1].trt";
connectAttr "rack_R.pm" "ikPcon.tg[1].tpm";
connectAttr "ikPcon.w1" "ikPcon.tg[1].tw";
connectAttr "expression1.out[2]" "ikPcon.w0";
connectAttr "expression1.out[3]" "ikPcon.w1";
connectAttr "pasted__layer8.di" "dipProjection.do";
connectAttr "groupId1.id" "bpx_0_4mm_round_softShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_0_4mm_round_softShape.iog.og[0].gco"
		;
connectAttr "groupId3.id" "bpx_1_6mm_round_synShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_1_6mm_round_synShape.iog.og[0].gco";
connectAttr "groupId5.id" "bpx_2_8mm_round_softShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_2_8mm_round_softShape.iog.og[0].gco"
		;
connectAttr "groupId7.id" "bpx_3_10mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_3_10mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId9.id" "bpx_4_13mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_4_13mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId11.id" "bpx_5_15mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_5_15mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId13.id" "bpx_6_17mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_6_17mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId15.id" "bpx_7_20mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_7_20mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId17.id" "bpx_8_28mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_8_28mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId19.id" "bpx_9_35mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_9_35mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId21.id" "bpx_10_40mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_10_40mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId23.id" "bpx_11_52mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_11_52mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId25.id" "bpx_12_78mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bpx_12_78mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId2.id" "bdx_0_4mm_round_softShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_0_4mm_round_softShape.iog.og[0].gco"
		;
connectAttr "groupId4.id" "bdx_1_6mm_round_synShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_1_6mm_round_synShape.iog.og[0].gco";
connectAttr "groupId6.id" "bdx_2_8mm_round_softShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_2_8mm_round_softShape.iog.og[0].gco"
		;
connectAttr "groupId8.id" "bdx_3_10mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_3_10mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId10.id" "bdx_4_13mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_4_13mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId12.id" "bdx_5_15mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_5_15mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId14.id" "bdx_6_17mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_6_17mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId16.id" "bdx_7_20mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_7_20mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId18.id" "bdx_8_28mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_8_28mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId20.id" "bdx_9_35mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_9_35mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId22.id" "bdx_10_40mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_10_40mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId24.id" "bdx_11_52mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_11_52mm_flat_naShape.iog.og[0].gco";
connectAttr "groupId26.id" "bdx_12_78mm_flat_naShape.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "bdx_12_78mm_flat_naShape.iog.og[0].gco";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "girderShaderSG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pasted__cable_04SG1.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "sx_reddish_SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "sx_sky_SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "sx_orange_SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "sx_purple_SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "sx_turqoise_SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "sx_green_SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "sx_canary_SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "sx_black_SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pasted__girderShaderSG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pasted__cable_04SG2.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pasted__typeBlinn1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pasted__cable_04SG3.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "girderShaderSG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pasted__cable_04SG1.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "sx_reddish_SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "sx_sky_SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "sx_orange_SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "sx_purple_SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "sx_turqoise_SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "sx_green_SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "sx_canary_SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "sx_black_SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pasted__girderShaderSG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pasted__cable_04SG2.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pasted__typeBlinn1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pasted__cable_04SG3.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "place2dTexture1.o" "ramp1.uv";
connectAttr "place2dTexture1.ofs" "ramp1.fs";
connectAttr "girderShaderSG.msg" "materialInfo37.sg";
connectAttr "girderShader.msg" "materialInfo37.m";
connectAttr "girderShader.oc" "girderShaderSG.ss";
connectAttr "dipTargetShape.iog" "girderShaderSG.dsm" -na;
connectAttr "toolChangeTargetShape.iog" "girderShaderSG.dsm" -na;
connectAttr "brushBaseShape.iog" "girderShaderSG.dsm" -na;
connectAttr "homeTargetShape.iog" "girderShaderSG.dsm" -na;
connectAttr "pasted__cable_04SG1.msg" "pasted__materialInfo43.sg";
connectAttr "pasted__rubber1.msg" "pasted__materialInfo43.m";
connectAttr "pasted__rubber1.oc" "pasted__cable_04SG1.ss";
connectAttr "layerManager.dli[1]" "layer1.id";
connectAttr "dipPaintingGroup.zeroPosition" "expression1.in[0]";
connectAttr ":time1.o" "expression1.tim";
connectAttr "jPcon.msg" "expression1.obm";
connectAttr "layerManager.dli[2]" "pasted__layer8.id";
connectAttr "mainPaintingGroup.zeroPosition" "pasted__expression5.in[0]";
connectAttr "BLZ.tx" "pasted__expression5.in[1]";
connectAttr "BLZ.ty" "pasted__expression5.in[2]";
connectAttr "BLZ.tz" "pasted__expression5.in[3]";
connectAttr "TLZ.tx" "pasted__expression5.in[4]";
connectAttr "TLZ.ty" "pasted__expression5.in[5]";
connectAttr "TLZ.tz" "pasted__expression5.in[6]";
connectAttr "BL.tx" "pasted__expression5.in[7]";
connectAttr "BL.ty" "pasted__expression5.in[8]";
connectAttr "BL.tz" "pasted__expression5.in[9]";
connectAttr "TL.tx" "pasted__expression5.in[10]";
connectAttr "TL.ty" "pasted__expression5.in[11]";
connectAttr "TL.tz" "pasted__expression5.in[12]";
connectAttr ":time1.o" "pasted__expression5.tim";
connectAttr "|mainPaintingGroup|jpos.msg" "pasted__expression5.obm";
connectAttr "pasted__cable_04SG2.msg" "pasted__materialInfo44.sg";
connectAttr "pasted__rubber2.msg" "pasted__materialInfo44.m";
connectAttr "pasted__rubber2.oc" "pasted__cable_04SG2.ss";
connectAttr "pasted__girderShaderSG.msg" "pasted__materialInfo37.sg";
connectAttr "pasted__girderShader.msg" "pasted__materialInfo37.m";
connectAttr "pasted__girderShader.oc" "pasted__girderShaderSG.ss";
connectAttr "pasted__typeBlinn1SG.msg" "pasted__materialInfo45.sg";
connectAttr "pasted__typeBlinn1.msg" "pasted__materialInfo45.m";
connectAttr "pasted__typeBlinn1.oc" "pasted__typeBlinn1SG.ss";
connectAttr "tx_0_reddish.sfPaintColor" "sx_reddish.c";
connectAttr "sx_reddish.oc" "sx_reddish_SG.ss";
connectAttr "tx_0_reddishShape.iog" "sx_reddish_SG.dsm" -na;
connectAttr "sx_reddish_SG.msg" "materialInfo39.sg";
connectAttr "sx_reddish.msg" "materialInfo39.m";
connectAttr "tx_1_sky.sfPaintColor" "sx_sky.c";
connectAttr "sx_sky.oc" "sx_sky_SG.ss";
connectAttr "tx_1_skyShape.iog" "sx_sky_SG.dsm" -na;
connectAttr "sx_sky_SG.msg" "materialInfo40.sg";
connectAttr "sx_sky.msg" "materialInfo40.m";
connectAttr "tx_2_orange.sfPaintColor" "sx_orange.c";
connectAttr "sx_orange.oc" "sx_orange_SG.ss";
connectAttr "tx_2_orangeShape.iog" "sx_orange_SG.dsm" -na;
connectAttr "sx_orange_SG.msg" "materialInfo41.sg";
connectAttr "sx_orange.msg" "materialInfo41.m";
connectAttr "tx_3_purple.sfPaintColor" "sx_purple.c";
connectAttr "sx_purple.oc" "sx_purple_SG.ss";
connectAttr "tx_3_purpleShape.iog" "sx_purple_SG.dsm" -na;
connectAttr "sx_purple_SG.msg" "materialInfo42.sg";
connectAttr "sx_purple.msg" "materialInfo42.m";
connectAttr "tx_4_turqoise.sfPaintColor" "sx_turqoise.c";
connectAttr "sx_turqoise.oc" "sx_turqoise_SG.ss";
connectAttr "tx_4_turqoiseShape.iog" "sx_turqoise_SG.dsm" -na;
connectAttr "sx_turqoise_SG.msg" "materialInfo43.sg";
connectAttr "sx_turqoise.msg" "materialInfo43.m";
connectAttr "tx_5_green.sfPaintColor" "sx_green.c";
connectAttr "sx_green.oc" "sx_green_SG.ss";
connectAttr "tx_5_greenShape.iog" "sx_green_SG.dsm" -na;
connectAttr "sx_green_SG.msg" "materialInfo44.sg";
connectAttr "sx_green.msg" "materialInfo44.m";
connectAttr "tx_6_canary.sfPaintColor" "sx_canary.c";
connectAttr "sx_canary.oc" "sx_canary_SG.ss";
connectAttr "tx_6_canaryShape.iog" "sx_canary_SG.dsm" -na;
connectAttr "sx_canary_SG.msg" "materialInfo45.sg";
connectAttr "sx_canary.msg" "materialInfo45.m";
connectAttr "tx_7_black.sfPaintColor" "sx_black.c";
connectAttr "sx_black.oc" "sx_black_SG.ss";
connectAttr "tx_7_blackShape.iog" "sx_black_SG.dsm" -na;
connectAttr "sx_black_SG.msg" "materialInfo46.sg";
connectAttr "sx_black.msg" "materialInfo46.m";
connectAttr "pasted__cable_04SG3.msg" "pasted__materialInfo46.sg";
connectAttr "pasted__rubber3.msg" "pasted__materialInfo46.m";
connectAttr "pasted__rubber3.oc" "pasted__cable_04SG3.ss";
connectAttr "r_trayShape.iog" "pasted__cable_04SG3.dsm" -na;
connectAttr "|brushes|dipCurves|defaultSource|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC.pmat";
connectAttr "|brushes|dipCurves|defaultSource|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC.pmat";
connectAttr "|brushes|dipCurves|defaultSource|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC.pmat";
connectAttr "|brushes|dipCurves|defaultSource|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC.pmat";
connectAttr "cShape9.ws" "cShape9_SC.crv";
connectAttr "mainProjectionShape.wm" "cShape9_SC.pmat";
connectAttr "cShape8.ws" "cShape8_SC.crv";
connectAttr "mainProjectionShape.wm" "cShape8_SC.pmat";
connectAttr "cShape7.ws" "cShape7_SC.crv";
connectAttr "mainProjectionShape.wm" "cShape7_SC.pmat";
connectAttr "cShape6.ws" "cShape6_SC.crv";
connectAttr "mainProjectionShape.wm" "cShape6_SC.pmat";
connectAttr "cShape5.ws" "cShape5_SC.crv";
connectAttr "mainProjectionShape.wm" "cShape5_SC.pmat";
connectAttr "cShape4.ws" "cShape4_SC.crv";
connectAttr "mainProjectionShape.wm" "cShape4_SC.pmat";
connectAttr "cShape3.ws" "cShape3_SC.crv";
connectAttr "mainProjectionShape.wm" "cShape3_SC.pmat";
connectAttr "cShape2.ws" "cShape2_SC.crv";
connectAttr "mainProjectionShape.wm" "cShape2_SC.pmat";
connectAttr "cShape1.ws" "cShape1_SC.crv";
connectAttr "mainProjectionShape.wm" "cShape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p04_b06_c0Shape0_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p04_b06_c0Shape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p04_b06_c0Shape2_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p04_b06_c0Shape3_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p02_b02_c0Shape0_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p02_b02_c0Shape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p02_b02_c0Shape2_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p02_b02_c0Shape3_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p05_b07_c0Shape0_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p05_b07_c0Shape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p05_b07_c0Shape2_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p05_b07_c0Shape3_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p07_b05_c0Shape0_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p07_b05_c0Shape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p07_b05_c0Shape2_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p07_b05_c0Shape3_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p06_b08_c0Shape0_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p06_b08_c0Shape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p06_b08_c0Shape2_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p06_b08_c0Shape3_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p01_b04_c0Shape0_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p01_b04_c0Shape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p01_b04_c0Shape2_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p01_b04_c0Shape3_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p03_b01_c0Shape0_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p03_b01_c0Shape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p03_b01_c0Shape2_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p03_b01_c0Shape3_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p06_b03_c0Shape0_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p06_b03_c0Shape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p06_b03_c0Shape2_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p06_b03_c0Shape3_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p03_b09_c0Shape0_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p03_b09_c0Shape1_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p03_b09_c0Shape2_SC.pmat";
connectAttr "dipProjectionShape.wm" "dcx_p03_b09_c0Shape3_SC.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC1.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC1.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC1.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC1.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC2.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC2.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC2.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC2.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC3.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC3.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC3.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC3.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC4.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC4.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC4.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC4.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC5.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC5.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC5.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC5.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC6.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC6.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC6.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC6.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC7.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC7.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC7.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC7.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC8.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC8.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC8.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC8.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC9.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC9.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC9.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC9.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC10.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC10.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC10.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC10.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC11.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC11.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC11.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC11.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC12.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC12.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC12.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC12.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC13.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC13.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC13.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC13.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC14.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC14.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC14.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC14.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC15.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC15.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC15.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC15.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC16.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC16.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC16.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC16.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC17.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC17.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC17.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC17.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC18.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC18.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC18.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC18.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC19.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC19.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC19.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC19.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC20.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC20.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC20.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC20.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC21.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC21.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC21.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC21.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC22.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC22.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC22.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC22.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC23.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC23.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC23.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC23.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC24.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC24.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC24.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC24.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC25.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC25.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC25.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC25.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC26.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC26.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC26.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC26.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC27.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC27.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC27.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC27.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC28.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC28.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC28.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC28.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC29.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC29.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC29.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC29.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC30.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC30.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC30.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC30.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC31.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC31.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC31.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC31.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC32.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC32.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC32.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC32.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC33.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC33.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC33.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC33.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC34.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC34.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC34.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC34.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC35.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC35.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC35.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC35.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC36.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC36.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC36.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC36.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC37.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC37.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC37.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC37.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC38.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC38.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC38.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC38.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC39.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC39.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC39.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC39.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC40.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC40.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC40.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC40.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC41.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC41.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC41.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC41.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC42.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC42.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC42.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC42.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC43.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC43.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC43.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC43.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC44.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC44.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC44.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC44.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC45.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC45.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC45.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC45.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC46.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC46.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC46.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC46.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC47.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC47.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC47.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC47.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC48.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC48.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC48.pmat";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC48.pmat";
connectAttr "|brushes|dipCurves|bdcx_0_4mm_round_soft|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC58.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC58.pmat";
connectAttr "|brushes|dipCurves|bdcx_0_4mm_round_soft|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC58.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC58.pmat";
connectAttr "|brushes|dipCurves|bdcx_0_4mm_round_soft|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC58.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC58.pmat";
connectAttr "|brushes|dipCurves|bdcx_0_4mm_round_soft|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC58.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC58.pmat";
connectAttr "|brushes|dipCurves|bdcx_1_6mm_round_syn|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC59.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC59.pmat";
connectAttr "|brushes|dipCurves|bdcx_1_6mm_round_syn|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC59.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC59.pmat";
connectAttr "|brushes|dipCurves|bdcx_1_6mm_round_syn|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC59.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC59.pmat";
connectAttr "|brushes|dipCurves|bdcx_1_6mm_round_syn|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC59.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC59.pmat";
connectAttr "|brushes|dipCurves|bdcx_2_8mm_round_soft|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC60.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC60.pmat";
connectAttr "|brushes|dipCurves|bdcx_2_8mm_round_soft|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC60.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC60.pmat";
connectAttr "|brushes|dipCurves|bdcx_2_8mm_round_soft|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC60.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC60.pmat";
connectAttr "|brushes|dipCurves|bdcx_2_8mm_round_soft|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC60.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC60.pmat";
connectAttr "|brushes|dipCurves|bdcx_3_10mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC61.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC61.pmat";
connectAttr "|brushes|dipCurves|bdcx_3_10mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC61.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC61.pmat";
connectAttr "|brushes|dipCurves|bdcx_3_10mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC61.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC61.pmat";
connectAttr "|brushes|dipCurves|bdcx_3_10mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC61.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC61.pmat";
connectAttr "|brushes|dipCurves|bdcx_4_13mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC62.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC62.pmat";
connectAttr "|brushes|dipCurves|bdcx_4_13mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC62.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC62.pmat";
connectAttr "|brushes|dipCurves|bdcx_4_13mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC62.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC62.pmat";
connectAttr "|brushes|dipCurves|bdcx_4_13mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC62.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC62.pmat";
connectAttr "|brushes|dipCurves|bdcx_5_15mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC63.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC63.pmat";
connectAttr "|brushes|dipCurves|bdcx_5_15mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC63.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC63.pmat";
connectAttr "|brushes|dipCurves|bdcx_5_15mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC63.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC63.pmat";
connectAttr "|brushes|dipCurves|bdcx_5_15mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC63.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC63.pmat";
connectAttr "|brushes|dipCurves|bdcx_6_17mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC64.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC64.pmat";
connectAttr "|brushes|dipCurves|bdcx_6_17mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC64.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC64.pmat";
connectAttr "|brushes|dipCurves|bdcx_6_17mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC64.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC64.pmat";
connectAttr "|brushes|dipCurves|bdcx_6_17mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC64.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC64.pmat";
connectAttr "|brushes|dipCurves|bdcx_7_20mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC65.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC65.pmat";
connectAttr "|brushes|dipCurves|bdcx_7_20mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC65.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC65.pmat";
connectAttr "|brushes|dipCurves|bdcx_7_20mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC65.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC65.pmat";
connectAttr "|brushes|dipCurves|bdcx_7_20mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC65.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC65.pmat";
connectAttr "|brushes|dipCurves|bdcx_8_28mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC66.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC66.pmat";
connectAttr "|brushes|dipCurves|bdcx_8_28mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC66.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC66.pmat";
connectAttr "|brushes|dipCurves|bdcx_8_28mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC66.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC66.pmat";
connectAttr "|brushes|dipCurves|bdcx_8_28mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC66.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC66.pmat";
connectAttr "|brushes|dipCurves|bdcx_9_35mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC67.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC67.pmat";
connectAttr "|brushes|dipCurves|bdcx_9_35mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC67.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC67.pmat";
connectAttr "|brushes|dipCurves|bdcx_9_35mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC67.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC67.pmat";
connectAttr "|brushes|dipCurves|bdcx_9_35mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC67.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC67.pmat";
connectAttr "|brushes|dipCurves|bdcx_10_40mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC68.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC68.pmat";
connectAttr "|brushes|dipCurves|bdcx_10_40mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC68.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC68.pmat";
connectAttr "|brushes|dipCurves|bdcx_10_40mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC68.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC68.pmat";
connectAttr "|brushes|dipCurves|bdcx_10_40mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC68.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC68.pmat";
connectAttr "|brushes|dipCurves|bdcx_11_52mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC69.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC69.pmat";
connectAttr "|brushes|dipCurves|bdcx_11_52mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC69.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC69.pmat";
connectAttr "|brushes|dipCurves|bdcx_11_52mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC69.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC69.pmat";
connectAttr "|brushes|dipCurves|bdcx_11_52mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC69.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC69.pmat";
connectAttr "|brushes|dipCurves|bdcx_12_78mm_flat_na|sourceDip1|sourceDipShape1.ws" "sourceDipShape1_SC70.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC70.pmat";
connectAttr "|brushes|dipCurves|bdcx_12_78mm_flat_na|sourceDip2|sourceDipShape2.ws" "sourceDipShape2_SC70.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC70.pmat";
connectAttr "|brushes|dipCurves|bdcx_12_78mm_flat_na|sourceDip3|sourceDipShape3.ws" "sourceDipShape3_SC70.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC70.pmat";
connectAttr "|brushes|dipCurves|bdcx_12_78mm_flat_na|sourceDip4|sourceDipShape4.ws" "sourceDipShape4_SC70.crv"
		;
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC70.pmat";
connectAttr "dcx_p04_b06_c0Shape0.ws" "sourceDipShape1_SC71.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC71.pmat";
connectAttr "dcx_p04_b06_c0Shape1.ws" "sourceDipShape2_SC71.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC71.pmat";
connectAttr "dcx_p04_b06_c0Shape2.ws" "sourceDipShape3_SC71.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC71.pmat";
connectAttr "dcx_p04_b06_c0Shape3.ws" "sourceDipShape4_SC71.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC71.pmat";
connectAttr "dcx_p02_b02_c0Shape0.ws" "sourceDipShape1_SC72.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC72.pmat";
connectAttr "dcx_p02_b02_c0Shape1.ws" "sourceDipShape2_SC72.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC72.pmat";
connectAttr "dcx_p02_b02_c0Shape2.ws" "sourceDipShape3_SC72.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC72.pmat";
connectAttr "dcx_p02_b02_c0Shape3.ws" "sourceDipShape4_SC72.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC72.pmat";
connectAttr "dcx_p05_b07_c0Shape0.ws" "sourceDipShape1_SC73.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC73.pmat";
connectAttr "dcx_p05_b07_c0Shape1.ws" "sourceDipShape2_SC73.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC73.pmat";
connectAttr "dcx_p05_b07_c0Shape2.ws" "sourceDipShape3_SC73.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC73.pmat";
connectAttr "dcx_p05_b07_c0Shape3.ws" "sourceDipShape4_SC73.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC73.pmat";
connectAttr "dcx_p07_b05_c0Shape0.ws" "sourceDipShape1_SC74.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC74.pmat";
connectAttr "dcx_p07_b05_c0Shape1.ws" "sourceDipShape2_SC74.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC74.pmat";
connectAttr "dcx_p07_b05_c0Shape2.ws" "sourceDipShape3_SC74.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC74.pmat";
connectAttr "dcx_p07_b05_c0Shape3.ws" "sourceDipShape4_SC74.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC74.pmat";
connectAttr "dcx_p06_b08_c0Shape0.ws" "sourceDipShape1_SC75.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC75.pmat";
connectAttr "dcx_p06_b08_c0Shape1.ws" "sourceDipShape2_SC75.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC75.pmat";
connectAttr "dcx_p06_b08_c0Shape2.ws" "sourceDipShape3_SC75.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC75.pmat";
connectAttr "dcx_p06_b08_c0Shape3.ws" "sourceDipShape4_SC75.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC75.pmat";
connectAttr "dcx_p01_b04_c0Shape0.ws" "sourceDipShape1_SC76.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC76.pmat";
connectAttr "dcx_p01_b04_c0Shape1.ws" "sourceDipShape2_SC76.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC76.pmat";
connectAttr "dcx_p01_b04_c0Shape2.ws" "sourceDipShape3_SC76.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC76.pmat";
connectAttr "dcx_p01_b04_c0Shape3.ws" "sourceDipShape4_SC76.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC76.pmat";
connectAttr "dcx_p03_b01_c0Shape0.ws" "sourceDipShape1_SC77.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC77.pmat";
connectAttr "dcx_p03_b01_c0Shape1.ws" "sourceDipShape2_SC77.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC77.pmat";
connectAttr "dcx_p03_b01_c0Shape2.ws" "sourceDipShape3_SC77.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC77.pmat";
connectAttr "dcx_p03_b01_c0Shape3.ws" "sourceDipShape4_SC77.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC77.pmat";
connectAttr "dcx_p06_b03_c0Shape0.ws" "sourceDipShape1_SC78.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC78.pmat";
connectAttr "dcx_p06_b03_c0Shape1.ws" "sourceDipShape2_SC78.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC78.pmat";
connectAttr "dcx_p06_b03_c0Shape2.ws" "sourceDipShape3_SC78.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC78.pmat";
connectAttr "dcx_p06_b03_c0Shape3.ws" "sourceDipShape4_SC78.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC78.pmat";
connectAttr "dcx_p03_b09_c0Shape0.ws" "sourceDipShape1_SC79.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape1_SC79.pmat";
connectAttr "dcx_p03_b09_c0Shape1.ws" "sourceDipShape2_SC79.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape2_SC79.pmat";
connectAttr "dcx_p03_b09_c0Shape2.ws" "sourceDipShape3_SC79.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape3_SC79.pmat";
connectAttr "dcx_p03_b09_c0Shape3.ws" "sourceDipShape4_SC79.crv";
connectAttr "dipProjectionShape.wm" "sourceDipShape4_SC79.pmat";
connectAttr "homeTarget.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[0].dn";
connectAttr "dipTarget.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[1].dn";
connectAttr "toolChangeTargetShape.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[2].dn"
		;
connectAttr "dipTargetShape.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[3].dn";
connectAttr "toolChangeTarget.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[4].dn"
		;
connectAttr "mainPaintingShape.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[5].dn"
		;
connectAttr "homeTargetShape.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[6].dn";
connectAttr "girderShaderSG.pa" ":renderPartition.st" -na;
connectAttr "pasted__cable_04SG1.pa" ":renderPartition.st" -na;
connectAttr "pasted__girderShaderSG.pa" ":renderPartition.st" -na;
connectAttr "pasted__cable_04SG2.pa" ":renderPartition.st" -na;
connectAttr "pasted__typeBlinn1SG.pa" ":renderPartition.st" -na;
connectAttr "sx_reddish_SG.pa" ":renderPartition.st" -na;
connectAttr "sx_sky_SG.pa" ":renderPartition.st" -na;
connectAttr "sx_orange_SG.pa" ":renderPartition.st" -na;
connectAttr "sx_purple_SG.pa" ":renderPartition.st" -na;
connectAttr "sx_turqoise_SG.pa" ":renderPartition.st" -na;
connectAttr "sx_green_SG.pa" ":renderPartition.st" -na;
connectAttr "sx_canary_SG.pa" ":renderPartition.st" -na;
connectAttr "sx_black_SG.pa" ":renderPartition.st" -na;
connectAttr "pasted__cable_04SG3.pa" ":renderPartition.st" -na;
connectAttr "girderShader.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__rubber1.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__girderShader.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__rubber2.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__typeBlinn1.msg" ":defaultShaderList1.s" -na;
connectAttr "sx_reddish.msg" ":defaultShaderList1.s" -na;
connectAttr "sx_sky.msg" ":defaultShaderList1.s" -na;
connectAttr "sx_orange.msg" ":defaultShaderList1.s" -na;
connectAttr "sx_purple.msg" ":defaultShaderList1.s" -na;
connectAttr "sx_turqoise.msg" ":defaultShaderList1.s" -na;
connectAttr "sx_green.msg" ":defaultShaderList1.s" -na;
connectAttr "sx_canary.msg" ":defaultShaderList1.s" -na;
connectAttr "sx_black.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__rubber3.msg" ":defaultShaderList1.s" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "ramp1.msg" ":defaultTextureList1.tx" -na;
connectAttr "ramp1.oc" ":lambert1.it";
connectAttr "bpx_0_4mm_round_softShape.iog.og[0]" ":initialShadingGroup.dsm" -na
		;
connectAttr "bdx_0_4mm_round_softShape.iog.og[0]" ":initialShadingGroup.dsm" -na
		;
connectAttr "bpx_1_6mm_round_synShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_1_6mm_round_synShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_2_8mm_round_softShape.iog.og[0]" ":initialShadingGroup.dsm" -na
		;
connectAttr "bdx_2_8mm_round_softShape.iog.og[0]" ":initialShadingGroup.dsm" -na
		;
connectAttr "bpx_3_10mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_3_10mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_4_13mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_4_13mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_5_15mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_5_15mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_6_17mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_6_17mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_7_20mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_7_20mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_8_28mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_8_28mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_9_35mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_9_35mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_10_40mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_10_40mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_11_52mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_11_52mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bpx_12_78mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "bdx_12_78mm_flat_naShape.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "groupId1.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId2.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId3.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId4.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId5.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId6.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId7.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId8.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId9.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId10.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId11.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId12.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId13.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId14.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId15.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId16.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId17.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId18.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId19.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId20.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId21.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId22.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId23.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId24.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId25.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId26.msg" ":initialShadingGroup.gn" -na;
connectAttr "ikRPsolver.msg" ":ikSystem.sol" -na;
connectAttr "pasted__ikRPsolver.msg" ":ikSystem.sol" -na;
// End of strokeCurveTest.ma
