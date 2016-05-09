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
 
#include "includes.h"

/***************************************************************************
 * Things which can be attached to a scene object.
 ***************************************************************************/

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "HybridObject.h"

namespace mgn {
class SceneObject;

class Component: public HybridObject {
public:
    Component() :
            HybridObject(), owner_object_() {
    }

    Component(SceneObject* owner_object) :
            owner_object_(owner_object) {
    }

    virtual ~Component() {
    }

    SceneObject* GetOwnerObject() const {
        return owner_object_;
    }

    void SetOwnerObject(SceneObject* owner_object) {
        owner_object_ = owner_object;
    }

    void RemoveOwnerObject() {
        owner_object_ = NULL;
    }

private:
    Component(const Component& component);
    Component(Component&& component);
    Component& operator=(const Component& component);
    Component& operator=(Component&& component);

private:
    SceneObject* owner_object_;
};

}
#endif
