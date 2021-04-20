#ifndef RADIUMPROJECT_VERLETPARTICLE_H
#define RADIUMPROJECT_VERLETPARTICLE_H

#include <Engine/Scene/GeometryComponent.hpp>
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Core/Animation/KeyFramedValueController.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>

#include "VerletPhysics.hpp"

class VerletParticle : public Ra::Engine::Scene::TriangleMeshComponent {
public:
    VerletParticle(const std::string &name,
                   Ra::Engine::Scene::Entity *entity,
                   Ra::Core::Geometry::TriangleMesh &&mesh, Ra::Core::Vector3f &pos, bool lock, float length );

    Ra::Core::Vector3f scale(Ra::Core::Vector3f vec, float s);

    void interpolateToSelf(float f);

    void scaleVelocity();

    void update(Scalar t);

    /// Attributes
    Ra::Core::Vector3f position, prev, temp, force;
    float mass, drag, restLength, strength, r;
    Scalar dtAgent;
    Ra::Core::Vector3f gravity;
    bool isLocked;
    Ra::Core::Vector3f initPos;

    /// The render object to animate
    std::shared_ptr <Ra::Engine::Rendering::RenderObject> m_ro;

    /// The Keyframes for the render object's transform.
    Ra::Core::Animation::KeyFramedValue <Ra::Core::Transform> m_transform;

    std::vector<std::pair<VerletParticle*, Ra::Engine::Rendering::RenderObject*>> springs;

};


#endif //RADIUMPROJECT_VERLETPARTICLE_H