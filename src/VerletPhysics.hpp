#ifndef RADIUMPROJECT_VERLETPHYSICS_H
#define RADIUMPROJECT_VERLETPHYSICS_H

// include the Engine/entity/component/system/animation interface
#include <Engine/FrameInfo.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/System.hpp>

// include the keyframe animation interface
#include <Core/Animation/KeyFramedValueInterpolators.hpp>
#include <Core/Tasks/Task.hpp>
#include <Core/Tasks/TaskQueue.hpp>

#include <Engine/Data/DrawPrimitives.hpp>
#include <Engine/Rendering/RenderObjectTypes.hpp>
#include <Engine/Data/PlainMaterial.hpp>
#include <Gui/Viewer/CameraManipulator.hpp>
#include "VerletParticle.hpp"

#define SAMEMASS 1


class VerletPhysics : public Ra::Engine::Scene::System {
public:
    virtual void generateTasks(Ra::Core::TaskQueue *q,
                               const Ra::Engine::FrameInfo &info);

};

#endif //RADIUMPROJECT_VERLETPHYSICS_H