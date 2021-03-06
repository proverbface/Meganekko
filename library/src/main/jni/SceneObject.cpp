/* Copyright 2015 Samsung Electronics Co., LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 * Objects in a scene.
 ***************************************************************************/

#include "includes.h"
#include "SceneObject.h"

#include "RenderData.h"
#include "Mesh.h"

namespace mgn {
    SceneObject::SceneObject() : HybridObject(),
        position(Vector3f()),
        scale(Vector3f(1, 1, 1)),
        rotation(Quatf()),
        renderData(nullptr),
        parent(nullptr),
        children(),
        visible(true),
        inFrustum(false),
        queryCurrentlyIssued(false),
        visCount(0),
        lodMinRange(0),
        lodMaxRange(MAXFLOAT),
        usingLod(false) {

    // Occlusion query setup
    queries = new GLuint[1];
    glGenQueries(1, queries);
}

SceneObject::~SceneObject() {
    delete queries;
}

void SceneObject::AttachRenderData(SceneObject* self, RenderData* renderData) {
    if (renderData) {
        DetachRenderData();
    }

    SceneObject* ownerObject(renderData->GetOwnerObject());
    if (ownerObject) {
        ownerObject->DetachRenderData();
    }

    this->renderData = renderData;
    renderData->SetOwnerObject(self);
}

void SceneObject::DetachRenderData() {
    if (renderData) {
        renderData->RemoveOwnerObject();
        renderData = nullptr;
    }
}

void SceneObject::AddChildObject(SceneObject* self, SceneObject* child) {
    for (SceneObject* parent = this->parent; parent; parent = parent->parent) {
        if (child == parent) {
            std::string error =
                    "SceneObject::addChildObject() : cycle of scene objects is not allowed.";
            throw error;
        }
    }
    children.push_back(child);
    child->parent = self;
}

void SceneObject::RemoveChildObject(SceneObject* child) {
    if (child->parent == this) {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
        child->parent = nullptr;
    }
}

int SceneObject::GetChildrenCount() const {
    return children.size();
}

SceneObject* SceneObject::GetChildByIndex(int index) {
    if (index < children.size()) {
        return children[index];
    } else {
        std::string error = "SceneObject::getChildByIndex() : Out of index.";
        throw error;
    }
}

void SceneObject::SetVisible(bool visibility = true) {

    //HACK
    //If checked every frame, queries may return
    //an inconsistent result when used with bounding boxes.

    //We need to make sure that the object's visibility status is consistent before
    //changing the status to avoid flickering artifacts.

    if (visibility == true)
        visCount++;
    else
        visCount--;

    if (visCount > checkFrames) {
        this->visible = true;
        visCount = 0;
    } else if (visCount < (-1 * checkFrames)) {
        this->visible = false;
        visCount = 0;
    }
}

bool SceneObject::IsColliding(SceneObject *sceneObject) {

    //Get the transformed bounding boxes in world coordinates and check if they intersect
    //Transformation is done by the getTransformedBoundingBoxInfo method in the Mesh class

    float thisObjectBoundingBox[6], checkObjectBoundingBox[6];

    OVR::Matrix4f this_object_model_matrix = this->GetRenderData()->GetOwnerObject()->GetMatrixWorld();
    this->GetRenderData()->GetMesh()->GetTransformedBoundingBoxInfo(&this_object_model_matrix, thisObjectBoundingBox);

    OVR::Matrix4f check_object_model_matrix = sceneObject->GetRenderData()->GetOwnerObject()->GetMatrixWorld();
    sceneObject->GetRenderData()->GetMesh()->GetTransformedBoundingBoxInfo(&check_object_model_matrix, checkObjectBoundingBox);

    bool result = (thisObjectBoundingBox[3] > checkObjectBoundingBox[0]
            && thisObjectBoundingBox[0] < checkObjectBoundingBox[3]
            && thisObjectBoundingBox[4] > checkObjectBoundingBox[1]
            && thisObjectBoundingBox[1] < checkObjectBoundingBox[4]
            && thisObjectBoundingBox[5] > checkObjectBoundingBox[2]
            && thisObjectBoundingBox[2] < checkObjectBoundingBox[5]);

    return result;
}

void SceneObject::SetPosition(const Vector3f& position) {
    this->position = position;
    Invalidate(false);
}

void SceneObject::SetScale(const Vector3f& scale) {
    this->scale = scale;
    Invalidate(false);
}

void SceneObject::SetRotation(const Quatf& rotation) {
    this->rotation = rotation;
    Invalidate(true);
}

const Matrix4f & SceneObject::GetMatrixWorld() {
    
    if (matrixWorldNeedsUpdate) {
        UpdateMatrixWorld();
        matrixWorldNeedsUpdate = false;
    }
        
    return matrixWorld;
}

void SceneObject::UpdateMatrixWorld() {

    UpdateMatrixLocal();

    if (GetParent() != nullptr) {
        this->matrixWorld = GetParent()->GetMatrixWorld() * matrixLocal;
    } else {
        this->matrixWorld = matrixLocal;
    }
}

void SceneObject::UpdateMatrixLocal() {

    Matrix4f translationMatrix = Matrix4f::Translation(position);
    Matrix4f rotationMatrix = Matrix4f(rotation);
    Matrix4f scaleMatrix = Matrix4f::Scaling(scale);
    matrixLocal = translationMatrix * rotationMatrix * scaleMatrix;

}

float inline sign(float a) {
    return a >= 0 ? 1.0f : -1.0f;
}

void SceneObject::SetMatrixLocal(const Matrix4f & matrix) {


    float xs = sign(matrix.M[0][0] * matrix.M[1][0] * matrix.M[2][0] * matrix.M[3][0]);
    float ys = sign(matrix.M[0][1] * matrix.M[1][1] * matrix.M[2][1] * matrix.M[3][1]);
    float zs = sign(matrix.M[0][2] * matrix.M[1][2] * matrix.M[2][2] * matrix.M[3][2]);

    Vector3f newScale(
            xs * Vector3f(matrix.M[0][0], matrix.M[1][0], matrix.M[2][0]).Length(),
            ys * Vector3f(matrix.M[0][1], matrix.M[1][1], matrix.M[2][1]).Length(),
            zs * Vector3f(matrix.M[0][2], matrix.M[1][2], matrix.M[2][2]).Length());

    Matrix3f rotationMatrix(
            matrix.M[0][0] / newScale.x, matrix.M[0][1] / newScale.y, matrix.M[0][2] / newScale.z,
            matrix.M[1][0] / newScale.x, matrix.M[1][1] / newScale.y, matrix.M[1][2] / newScale.z,
            matrix.M[2][0] / newScale.x, matrix.M[2][1] / newScale.y, matrix.M[2][2] / newScale.z);

    position = matrix.GetTranslation();
    scale = newScale;
    rotation = Quatf(rotationMatrix);

    Invalidate(true);
}

void SceneObject::Invalidate(bool rotationUpdated) {

    if (!matrixWorldNeedsUpdate) {
        std::vector<SceneObject*> objects = GetChildren();
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            (*it)->Invalidate(false);
        }
    }

    matrixWorldNeedsUpdate = true;

    if (rotationUpdated) {
        // scale rotation if needed to avoid overflow
        static const float threshold = sqrt(FLT_MAX) / 2.0f;
        static const float scale_factor = 0.5f / sqrt(FLT_MAX);
        if (rotation.w > threshold || rotation.x > threshold
                || rotation.y > threshold || rotation.z > threshold) {
            rotation.w *= scale_factor;
            rotation.x *= scale_factor;
            rotation.y *= scale_factor;
            rotation.z *= scale_factor;
        }
    }
}

}
