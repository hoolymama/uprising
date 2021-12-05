#ifndef cImgOverride_H
#define cImgOverride_H

#include <vector>

#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MPxShadingNodeOverride.h>

#define cimg_display 0
#include "CImg.h"
using namespace cimg_library;

class cImgOverride : public MHWRender::MPxShadingNodeOverride
{
public:
	static MHWRender::MPxShadingNodeOverride *creator(const MObject &obj);

	virtual ~cImgOverride();

	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual MString fragmentName() const;
	virtual void getCustomMappings(
	  MHWRender::MAttributeParameterMappingList &mappings);

	virtual void updateDG();
	virtual void updateShader(
	  MHWRender::MShaderInstance &shader,
	  const MHWRender::MAttributeParameterMappingList &mappings);


private:
	cImgOverride(const MObject &obj);

	MString fFragmentName;
	MObject fObject;
	MString fFileName;


	// CImg<unsigned char> *m_img;
	std::vector<unsigned char> m_img;
	int m_width;
	int m_height;
	int m_spectrum;






	const MHWRender::MSamplerState *fSamplerState;

	mutable MString fResolvedMapName;
	mutable MString fResolvedSamplerName;
};



#endif