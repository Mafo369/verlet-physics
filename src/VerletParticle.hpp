#ifndef RADIUMPROJECT_VERLETPARTICLE_H
#define RADIUMPROJECT_VERLETPARTICLE_H

#include <Engine/Scene/GeometryComponent.hpp>
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Core/Animation/KeyFramedValueController.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>

#include "VerletPhysics.hpp"

class VerletParticle : public Ra::Engine::Scene::TriangleMeshComponent {
public:
    VerletParticle( const std::string &name,
                   Ra::Engine::Scene::Entity *entity,
                   Ra::Core::Geometry::TriangleMesh &&mesh, Ra::Core::Vector3f &pos, bool lock );

    //! [Update Particle]
    void updateSM( Scalar t ); // Update when physics system type = 1 -> all objects have the same mass
    void updateDM( Scalar t ); // Update when physics system type = 0 -> objects have different mass
    //! [Update Particle]

    //! [Springs]
    void addSpring( VerletParticle *particle, Ra::Engine::Rendering::RenderObjectManager *renderObjMan );
    //! [Springs]

    //! [Behaviors]
    void addBehavior( Ra::Core::Vector3f b );
    void addAttractionBehavior( Ra::Core::Vector3f attractor, float radius, float strength );
    //! [Behaviors]

    //! [Constraints]
    void addConstraint( Ra::Core::Aabb c );
    void updateConstraints( Ra::Core::Vector3f v );
    //! [Constraints]

    //! [Vector Manipulation]
    Ra::Core::Vector3f normalizeTo(Ra::Core::Vector3f v, float len);
    Ra::Core::Vector3f scale( Ra::Core::Vector3f vec, float s );
    void interpolateToSelf( float f );
    void scaleVelocity();
    //! [Vector Manipulation]

    //! [Setters]
    void setMass( float m );
    //! [Setters]


    /// Attributes
    Ra::Core::Vector3f position, prev, temp, force;
    float mass, drag, strength, r;
    Scalar dtAgent;
    bool isLocked;
    Ra::Core::Vector3f initPos;

    /// The render object to animate
    std::shared_ptr <Ra::Engine::Rendering::RenderObject> m_ro;

    /// The Keyframes for the render object's transform.
    Ra::Core::Animation::KeyFramedValue <Ra::Core::Transform> m_transform;

    /// The springs to other particles
    std::vector<std::pair<VerletParticle*, Ra::Engine::Rendering::RenderObject*>> springs;

    /// Length of the springs
    std::vector<float> restLength;

    /// Constant Behaviors list
    std::vector<Ra::Core::Vector3f> behaviors;

    // Attraction Behaviors list
    std::vector<std::pair<Ra::Core::Vector3f, std::pair<float, float>>> attractionBehaviors;

    /// Constraints list (Box Constraints)
    std::vector<Ra::Core::Aabb> constraints;
};

#endif //RADIUMPROJECT_VERLETPARTICLE_H