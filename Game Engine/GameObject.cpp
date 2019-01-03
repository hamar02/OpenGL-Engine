#include "GameObject.h"

GameObject::GameObject()
{
	name = "EmptyGameObject";
	transform=Transform();
    isActive = true;
    isRenderable = false;
    hasLight = false;

	//OOBB
	OOBoundingBox.centre = glm::vec4(0,0,0,1);
	OOBoundingBox.x_hx = glm::vec4(1, 0, 0, 0.5);
	OOBoundingBox.y_hy = glm::vec4(0, 1, 0, 0.5);
	OOBoundingBox.z_hz = glm::vec4(0, 0, 1, 0.5);

}

GameObject::GameObject(glm::vec3 otherPosition)
{
	name = "EmptyGameObject";
	transform = Transform();
	isActive = true;
	isRenderable = false;
	hasLight = false;
	transform.position = otherPosition;
	
	//OOBB
	OOBoundingBox.centre = glm::vec4(0, 0, 0, 1);
	OOBoundingBox.x_hx = glm::vec4(1, 0, 0, 0.5);
	OOBoundingBox.y_hy = glm::vec4(0, 1, 0, 0.5);
	OOBoundingBox.z_hz = glm::vec4(0, 0, 1, 0.5);

}

GameObject::GameObject(int otherAssetID)
{
	name = "EmptyGameObject";
	transform = Transform();
	assetID = otherAssetID;
    isActive = true;
    isRenderable = false;
    hasLight = false;

}

GameObject::~GameObject()
{
}

void GameObject::updateMaterialAndMeshFilterPointers() {
    bool matTest = false;
    bool meshTest = false;

    for (int i = 0; i < components.size(); i++) {
        Material* temp = getComponent<Material>();
        if (temp != nullptr) {
            materialComponent = temp;
            matTest = true;

        }
    }

    for (int i = 0; i < components.size(); i++) {
        MeshFilter* temp = getComponent<MeshFilter>();
        if (temp != nullptr) {
            meshFilterComponent = temp;
            meshTest = true;

        }
    }

    if (meshTest==true && matTest==true) {
        isRenderable = true;
    } else {
        isRenderable = false;
    }
}

void GameObject::updateHasLight() {
    bool foundLight = false;

    for (int i = 0; i < components.size(); i++) {
       
        Light* temp = getComponent<Light>();
        if (temp != nullptr) {
            foundLight = true;
			lightComponent = temp;

        }
    }

   
    if (foundLight == true) {
        hasLight = true;
    } else {
        hasLight = false;
    }
}

void GameObject::addComponent(Component* otherComponent)
{
	//check if component exist
	int index = -1;
	for (int i = 0; i < components.size(); i++)
	{
		if (*otherComponent == *components[i]) {
			index = i;
		}
	}
	//add if not
	if (index == -1) {
        otherComponent->gameObject = this;
		components.push_back(otherComponent);
	}
    updateMaterialAndMeshFilterPointers();
    updateHasLight();
}

void GameObject::deleteComponent(Component* otherComponent)
{
	//find component
	int index = -1;
	for (int i = 0; i < components.size(); i++)
	{
		if (*otherComponent == *components[i]) {
			index = i;
		}
	}
	if (index != -1) {
		components.erase(components.begin() + index);
	}
    updateMaterialAndMeshFilterPointers();
}

const bool GameObject::getIsRenderable() {
    return isRenderable;
}

glm::mat4 GameObject::calculateWorldMatrix()
{
	glm::mat4 world_matrix = glm::mat4(1);
	world_matrix = glm::translate(world_matrix, transform.position);
	//world_matrix = glm::rotate(world_matrix, glm::radians(dT), glm::vec3(0.0f, 1.0f, 0.0f));

	return world_matrix;
}



void GameObject::uniformWorldMatrix(unsigned int othershaderProgram)
{
	//Tranformation
	//Matrices

	GLuint uniformLoc = glGetUniformLocation(othershaderProgram, "world_matrix");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(calculateWorldMatrix()));


}
