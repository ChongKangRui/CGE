#include "gepch.h"
#include "SceneSerializer.h"

#include "Entity.h"

#include "Component.h"


#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	/*template<>
	struct convert<Hazel::UUID>
	{
		static Node encode(const Hazel::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, Hazel::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};*/

}


namespace GE {

	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v) {

		// flow mean gonna output it as {x,y,z}
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene)
	{
		
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {

		out << YAML::BeginMap; // Entity
		out << YAML::Key<<"Entity" << YAML::Value << "128378977"; // Entity id

		
		if (entity.HasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;



			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>()) {
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();

			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;


			

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteComponent>()) {
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;
			auto& sp = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "Color" << YAML::Value << sp.Color;


			out << YAML::EndMap;
		}

		out << YAML::EndMap;


	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Name";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID) {

			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);

			}
		);
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

	}
	void SceneSerializer::SerializeRunTime(const std::string& filepath)
	{
		GE_CORE_ASSERT(false, "Not implemented");

	}
	bool  SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strstream;
		strstream << stream.rdbuf();

		YAML::Node data = YAML::Load(strstream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		GELog_Trace("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {

				uint64_t uuid =123123;

				std::string name;
				auto tagcomponent = entity["TagComponent"];

				if (tagcomponent)
					name = tagcomponent["Tag"].as<std::string>();

				GELog_Trace("Deserializing entity with ID '{0}', name : {1}", uuid, name);
				Entity deserializedEntity = m_Scene->CreateEntity(name);


				
				auto transformComp = entity["TransformComponent"];
				if (transformComp) {
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();

					tc.Position = transformComp["Translation"].as<glm::vec3>();
					tc.Rotation = transformComp["Rotation"].as<glm::vec3>();
					tc.Scale = transformComp["Scale"].as<glm::vec3>();

				}

				auto cameraComponent = entity["CameraComponent"];

				if (cameraComponent) {

					auto& cam = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];

					cam.Primary = cameraComponent["Primary"].as<bool>();


					cam.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
					cam.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
					
					cam.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cam.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cam.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());
					cam.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					cam.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cam.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());
				}

				auto spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteComponent>();
					src.Color = spriteComponent["Color"].as<glm::vec4>();
					/*if (spriteRendererComponent["TexturePath"])
					{
						std::string texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
						auto path = Project::GetAssetFileSystemPath(texturePath);
						src.Texture = Texture2D::Create(path.string());
					}

					if (spriteRendererComponent["TilingFactor"])
						src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();*/
				}
				
				
				//entity["camera"]
			}

		}

		return true;
	}
	bool SceneSerializer::DeserializeRunTime(const std::string& filepath)
	{
		GE_CORE_ASSERT(false, "Not implemented");
		return false;
	}
}