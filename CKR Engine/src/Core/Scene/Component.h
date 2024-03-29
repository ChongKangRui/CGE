#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_QUAT_DATA_WXYZ
#include <Glm/gtx/quaternion.hpp>

#include "Core/Renderer/OrthographicCamera.h"
#include "SceneCamera.h"
#include "Core/Renderer/Texture.h"
#include "ScriptableEntity.h"

namespace GE {
	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		TagComponent(const std::string& tag) : Tag(tag) {}

	};
	struct TransformComponent {
		glm::mat4 Transform{ 1.0f };
		glm::vec3 Position = { 0.0f,0.0f,0.0f };
		glm::vec3 Rotation = { 0.0f,0.0f,0.0f };
		glm::vec3 Scale = { 1.0f,1.0f,1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		TransformComponent(const glm::vec3& position) : Position(position) {}


		glm::mat4 GetTransform() const
		{
			auto position = glm::translate(glm::mat4(1.0f), Position);

			/*auto rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0,0,1 });*/
			auto rotation = glm::toMat4(glm::quat(Rotation));

			auto scale = glm::scale(glm::mat4(1.0f), Scale);

			return  position * rotation * scale;


		}
	};

	struct SpriteComponent {
		glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
		float TilingFactor = 1.0f;
		Ref<Texture2D> Texture;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;

		SpriteComponent(const glm::vec4& color) : Color(color) {}

	};

	struct CameraComponent {
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		CameraComponent(const glm::mat4& projection) {}


	};

	struct NativeScriptComponent {
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

	
		template<typename T>
		void Bind() {
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance;  nsc->Instance = nullptr; };


		}
	};

}