#pragma once

//For use by application/outside of engine client only
//Include for Game File

#include "Core/Base/EngineApplication.h"
#include "Core/Base/Layer.h"
#include "Core/Base/Log.h"
#include "Core/Imgui/ImGuiLayer.h"

#include "Core/Base/Input.h"
#include "Core/Base/KeyCode.h"
#include "Core/Base/MouseCode.h"
#include "Core/Base/TimeStep.h"

#include "Core/Renderer/OrthographicCameraController.h"

#include "Core/Scene/Scene.h"
#include "Core/Scene/Entity.h"
#include "Core/Scene/Component.h"


//Rendering related
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/RendererCommand.h"

#include "Core/Renderer/Buffer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/SubTexture2D.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/OrthographicCamera.h"

// -------EntryPoint---------------
//#include "Core/Base/EntryPoint.h"
//----------------------------------