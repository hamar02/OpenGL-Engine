#pragma once
#include <vector>
#include "Component.h"
#include <string>
#include "Transform.h"
#include "MeshFilter.h"
#include "Material.h"
#include "Light.h"
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PRIMITIVE_GEOMETRY.h"



class GameObject : public Component
{
public:
	GameObject();
	GameObject(glm::vec3 otherPosition);
	GameObject(int otherAssetID);
	~GameObject();

	int assetID;
    bool isActive;
    bool hasLight;
	std::string name;
	Transform transform;
	std::vector<Component*> components;
    Material *materialComponent;
    MeshFilter *meshFilterComponent;
	Light *lightComponent;
	OOBB OOBoundingBox;

    void updateMaterialAndMeshFilterPointers();
    void updateHasLight();
	void addComponent(Component* otherComponent);
	void deleteComponent(Component* otherComponent);
    const bool getIsRenderable();
	glm::mat4 calculateWorldMatrix();
	void uniformWorldMatrix(unsigned int othershaderProgram);

	template <class T>
	T* getComponent() {
		for (int i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<T*>(components[i]) != nullptr)
			{
				T *test = dynamic_cast<T*>(components[i]);
				return test;
			}
		}

		return nullptr;
	}
private:
    bool isRenderable;
};
