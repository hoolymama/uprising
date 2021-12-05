
#include <maya/MFnDependencyNode.h>
#include <maya/MRenderUtil.h>

#include <maya/MFragmentManager.h>
#include <maya/MShaderManager.h>
#include <maya/MTextureManager.h>

#include <maya/MStateManager.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MFnPluginData.h>


#include "cImgOverride.h"
#include "cImgShader.h"
#include "cImgData.h"
#include "errorMacros.h"



////////////////////////////////////////////////////////////////////////////
// Override Implementation
////////////////////////////////////////////////////////////////////////////
MHWRender::MPxShadingNodeOverride *cImgOverride::creator(
  const MObject &obj)
{
	return new cImgOverride(obj);
}

cImgOverride::cImgOverride(const MObject &obj)
	: MPxShadingNodeOverride(obj)
	, fObject(obj)
	, fFragmentName("")
	, fFileName("")
	, fSamplerState(NULL)
	, fResolvedMapName("")
	, fResolvedSamplerName("")
	, m_img()
	, m_width(0)
	, m_height(0)
	, m_spectrum(0)

{

	static const MString sFragmentName("cImgPluginFragment");
	const char *sFragmentBody =
#include "cImgPluginFragment.xml"

	  static const MString sFragmentOutputName("cImgPluginFragmentOutput");
	const char *sFragmentOutputBody =
#include "cImgPluginFragmentOutput.xml"

	  static const MString sFragmentGraphName("cImgPluginGraph");
	const char *sFragmentGraphBody =
#include "cImgPluginGraph.xml"
	  // Register fragments with the manager if needed
	  //
	  MHWRender::MRenderer * theRenderer = MHWRender::MRenderer::theRenderer();
	if (theRenderer)
	{
		MHWRender::MFragmentManager *fragmentMgr =
		  theRenderer->getFragmentManager();
		if (fragmentMgr)
		{
			// Add fragments if needed
			bool fragAdded = fragmentMgr->hasFragment(sFragmentName);
			bool structAdded = fragmentMgr->hasFragment(sFragmentOutputName);
			bool graphAdded = fragmentMgr->hasFragment(sFragmentGraphName);
			if (!fragAdded)
			{
				fragAdded = (sFragmentName == fragmentMgr->addShadeFragmentFromBuffer(
				               sFragmentBody, false));
			}
			if (!structAdded)
			{
				structAdded = (sFragmentOutputName == fragmentMgr->addShadeFragmentFromBuffer(
				                 sFragmentOutputBody, false));
			}
			if (!graphAdded)
			{
				graphAdded = (sFragmentGraphName == fragmentMgr->addFragmentGraphFromBuffer(
				                sFragmentGraphBody));
			}
			if (fragAdded && structAdded && graphAdded)
			{
				fFragmentName = sFragmentGraphName;
			}
		}
	}
}

cImgOverride::~cImgOverride()
{
	MHWRender::MStateManager::releaseSamplerState(fSamplerState);
	fSamplerState = NULL;
}

MHWRender::DrawAPI cImgOverride::supportedDrawAPIs() const
{
	return MHWRender::kOpenGL | MHWRender::kDirectX11 |
	       MHWRender::kOpenGLCoreProfile;
}

MString cImgOverride::fragmentName() const
{
	// Reset cached parameter names since the effect is being rebuilt
	fResolvedMapName = "";
	fResolvedSamplerName = "";

	return fFragmentName;
}

void cImgOverride::getCustomMappings(
  MHWRender::MAttributeParameterMappingList &mappings)
{
	// Set up some mappings for the parameters on the file texture fragment,
	// there is no correspondence to attributes on the node for the texture
	// parameters.
	MHWRender::MAttributeParameterMapping mapMapping( "map", "", false, true);
	mappings.append(mapMapping);
	MHWRender::MAttributeParameterMapping textureSamplerMapping(	"textureSampler",
	    "", false, true);
	mappings.append(textureSamplerMapping);
}

void cImgOverride::updateDG()
{
	// Pull the file name from the DG for use in updateShader
	MStatus st;
	MFnDependencyNode nodeFn(fObject, &st);
	if (st.error()) {
		return;
	}

	MObject dImgData;


	MPlug imgDataPlug = nodeFn.findPlug("input", true, &st);
	if (st.error()) {
		return;
	}

	st = imgDataPlug.getValue(dImgData);
	if (st.error()) {
		return;
	}

	cImgData *imgData = 0;
	MFnPluginData fnImgData( dImgData, &st );
	if (st.error()) {
		return;
	}

	imgData = (cImgData *)fnImgData.data();

	CImg<unsigned char> *pImg = imgData->fImg;
	m_width = pImg->width();
	m_height = pImg->height();
	m_spectrum = pImg->spectrum();

	int size = m_width * m_height * 1;
	m_img.clear();
	if (m_spectrum == 3) {
		size = m_width * m_height * 4;
	}
	m_img.reserve(size);
	for (int y = m_height - 1; y >= 0; --y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			for (int c = 0; c < m_spectrum; ++c)
			{
				m_img.push_back((*pImg)(x, y, 0, c));
			}
			if (m_spectrum == 3) {
				m_img.push_back(255);
			}
		}
	}


	// m_img = *(imgData->fImg);
	// cerr << " ptr to m_img: " << m_img << endl;



	// cerr << "w:" << width << " - h:" << height << " - s:" << spectrum << endl;

}

void cImgOverride::updateShader(
  MHWRender::MShaderInstance &shader,
  const MHWRender::MAttributeParameterMappingList &mappings)
{
	// Handle resolved name caching
	if (fResolvedMapName.length() == 0)
	{
		const MHWRender::MAttributeParameterMapping *mapping =
		  mappings.findByParameterName("map");
		if (mapping)
		{
			fResolvedMapName = mapping->resolvedParameterName();
		}
	}
	if (fResolvedSamplerName.length() == 0)
	{
		const MHWRender::MAttributeParameterMapping *mapping =
		  mappings.findByParameterName("textureSampler");
		if (mapping)
		{
			fResolvedSamplerName = mapping->resolvedParameterName();
		}
	}


	if (fResolvedMapName.length() > 0 && fResolvedSamplerName.length() > 0)
	{
		// Set sampler to linear-wrap
		if (!fSamplerState)
		{
			MHWRender::MSamplerStateDesc desc;
			desc.filter = MHWRender::MSamplerState::kAnisotropic;
			desc.maxAnisotropy = 16;
			fSamplerState = MHWRender::MStateManager::acquireSamplerState(desc);
		}
		if (fSamplerState)
		{
			shader.setParameter(fResolvedSamplerName, *fSamplerState);
		}

		// Set texture
		MHWRender::MRenderer *renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			MHWRender::MTextureManager *textureManager =
			  renderer->getTextureManager();
			if (textureManager)
			{

				const MString sCImgTextureName("cImgTextureName");

				if (m_width && m_height && m_spectrum) {

					MHWRender::MTextureDescription desc;
					desc.setToDefault2DTexture();
					desc.fWidth = m_width;
					desc.fHeight = m_height;
					// desc.fFormat = MHWRender::kR8G8B8X8;
					if (m_spectrum == 3) {
						desc.fFormat = MHWRender::kR8G8B8X8;
					}
					else {
						desc.fFormat = MHWRender::kR8_UNORM;
					}
					desc.fTextureType = MHWRender::kImage2D;

					desc.fMipmaps = 0;


					MHWRender::MTexture *texture = textureManager->acquireTexture(
					                                 sCImgTextureName,
					                                 desc,
					                                 (const void *)&m_img[0],
					                                 false);


					if (texture)
					{
						MHWRender::MTextureAssignment textureAssignment;
						textureAssignment.texture = texture;
						shader.setParameter(fResolvedMapName, textureAssignment);

						// release our reference now that it is set on the shader
						textureManager->releaseTexture(texture);
					}



				}




			}
		}
	}
}
