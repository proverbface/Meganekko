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

package com.eje_c.meganekko;

/**
 * Name map for material shaders.
 * <p/>
 * <p/>
 * A {@link Mesh} specifies a {@linkplain SceneObject scene object's}
 * surface geometry; a {@link Material} specifies a scene object's surface
 * appearance. A {@link Material} specifies a shader by
 * {@link MaterialShaderId id}, and contains named values. These names are
 * not necessarily the same as the names of the attributes and uniforms in the
 * shader program: the methods of this class let you map names from materials to
 * programs.
 */
public class MaterialMap extends HybridObject implements ShaderMaps {
    MaterialMap(long ptr) {
        super(ptr);
    }

    private static native void addTextureKey(long customShader, String variableName, String key);

    private static native void addAttributeFloatKey(long customShader, String variableName, String key);

    private static native void addAttributeVec2Key(long customShader, String variableName, String key);

    private static native void addAttributeVec3Key(long customShader, String variableName, String key);

    private static native void addAttributeVec4Key(long customShader, String variableName, String key);

    private static native void addUniformFloatKey(long customShader, String variableName, String key);

    private static native void addUniformVec2Key(long customShader, String variableName, String key);

    private static native void addUniformVec3Key(long customShader, String variableName, String key);

    private static native void addUniformVec4Key(long customShader, String variableName, String key);

    private static native void addUniformMat4Key(long customShader, String variableName, String key);

    public void addTextureKey(String variableName, String key) {
        addTextureKey(getNative(), variableName, key);
    }

    /**
     * Link a float in a material to this GL program.
     *
     * @param variableName The variable name in the GL program.
     * @param key          The float key in the material.
     */
    public void addAttributeFloatKey(String variableName, String key) {
        addAttributeFloatKey(getNative(), variableName, key);
    }

    /**
     * Link a vec2 in a material to this GL program.
     *
     * @param variableName The variable name in the GL program.
     * @param key          The vec2 key in the material.
     */
    public void addAttributeVec2Key(String variableName, String key) {
        addAttributeVec2Key(getNative(), variableName, key);
    }

    /**
     * Link a vec3 in a material to this GL program.
     *
     * @param variableName The variable name in the GL program.
     * @param key          The vec3 key in the material.
     */
    public void addAttributeVec3Key(String variableName, String key) {
        addAttributeVec3Key(getNative(), variableName, key);
    }

    /**
     * Link a vec4 in a material to this GL program.
     *
     * @param variableName The variable name in the GL program.
     * @param key          The vec4 key in the material.
     */
    public void addAttributeVec4Key(String variableName, String key) {
        addAttributeVec4Key(getNative(), variableName, key);
    }

    public void addUniformFloatKey(String variableName, String key) {
        addUniformFloatKey(getNative(), variableName, key);
    }

    public void addUniformVec2Key(String variableName, String key) {
        addUniformVec2Key(getNative(), variableName, key);
    }

    public void addUniformVec3Key(String variableName, String key) {
        addUniformVec3Key(getNative(), variableName, key);
    }

    public void addUniformVec4Key(String variableName, String key) {
        addUniformVec4Key(getNative(), variableName, key);
    }

    /**
     * Link a mat4 in a material to this GL program.
     *
     * @param variableName The variable name in the GL program.
     * @param key          The mat4 key in the material.
     */
    public void addUniformMat4Key(String variableName, String key) {
        addUniformMat4Key(getNative(), variableName, key);
    }
}