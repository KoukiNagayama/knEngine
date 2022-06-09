﻿
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerRendererDX9.DeviceObject.h"
#include "EffekseerRendererDX9.RendererImplemented.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerRendererDX9
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DeviceObject::DeviceObject(RendererImplemented* renderer)
	: m_renderer(renderer)
{
	ES_SAFE_ADDREF(m_renderer);
	m_renderer->m_deviceObjects.insert(this);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DeviceObject::~DeviceObject()
{
	m_renderer->m_deviceObjects.erase(this);
	ES_SAFE_RELEASE(m_renderer);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RendererImplemented* DeviceObject::GetRenderer() const
{
	return m_renderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererDX9
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
