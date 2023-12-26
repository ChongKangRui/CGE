#pragma once
#include "Scene.h"


namespace GE {
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRunTime(const std::string& filepath);
	
		bool Deserialize(const std::string& filepath);
		bool DeserializeRunTime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;



	};
}
