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
 * Renders a scene, a screen.
 ***************************************************************************/
#include "includes.h"

#ifndef RENDERER_H_
#define RENDERER_H_

#include "util/GL.h"
#include "mesh.h"
#include "OESShader.h"

namespace mgn
{
class Camera;
class Scene;
class SceneObject;
class RenderData;
class RenderTexture;

class Renderer
{
private:
    Renderer();

public:

    static void RenderEyeView(Scene* scene, std::vector<SceneObject*>,
            OESShader * oesShader,
            const OVR::Matrix4f &eyeViewMatrix,
            const OVR::Matrix4f &eyeProjectionMatrix,
            const OVR::Matrix4f &eyeViewProjection,
            const int eye);

private:
    static void RenderRenderData(RenderData* renderData,
            const OVR::Matrix4f& viewMatrix,
            const OVR::Matrix4f& projectionMatrix,
            OESShader * oesShader, const int eye);

    static void OcclusionCull(Scene* scene, std::vector<SceneObject*> sceneObjects);
    static void FrustumCull(Scene* scene, const OVR::Vector3f& cameraPosition,
            std::vector<SceneObject*> sceneObjects,
            std::vector<RenderData*>& renderDataVector, const OVR::Matrix4f &vpMatrix,
            OESShader * oesShader);
    static void BuildFrustum(float frustum[6][4], float mvpMatrix[16]);
    static bool IsCubeInFrustum(float frustum[6][4], const BoundingBoxInfo & vertexLimit);

    static void SetFaceCulling(int cull_face);

    Renderer(const Renderer& renderEngine);
    Renderer(Renderer&& renderEngine);
    Renderer& operator=(const Renderer& renderEngine);
    Renderer& operator=(Renderer&& renderEngine);
};

}
#endif
