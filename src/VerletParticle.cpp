#include "VerletParticle.hpp"

#include <glm/glm.hpp>
#include <Core/Math/GlmAdapters.hpp>

VerletParticle::VerletParticle( const std::string &name,
                               Ra::Engine::Scene::Entity *entity,
                               Ra::Core::Geometry::TriangleMesh &&mesh, Ra::Core::Vector3f &pos, bool lock ) :
        Ra::Engine::Scene::TriangleMeshComponent( name,
                                                 entity,
                                                 std::move( mesh ) ),
        m_transform( Ra::Core::Transform::Identity(), 0_ra ) {

    position = pos; // Current position
    prev = pos; // Previous position
    initPos = pos; // Initial position - Necessary for translation
    mass = 1.f; // 1kg
    drag = 0.999; // rho * C * Area - simplified drag for this example
    force = Ra::Core::Vector3f( 0.f, 0.f, 0.f );
    temp = Ra::Core::Vector3f( 0.f, 0.f, 0.f );
    m_ro = getRoMgr()->getRenderObject( m_roIndex );
    strength = 0.5f; // Strength of springs
    isLocked = lock;
}

//! [Update Particle]
void VerletParticle::updateSM( Scalar t ) {
    if( !isLocked ){
        Ra::Core::Vector3f v = ( position - prev ) * drag; // Velocity
        prev = position;
        position = position + v;
        for( auto b : behaviors ) {
            position = position + b;
        }
        for( auto a : attractionBehaviors ) {
            auto delta = a.first - position;
            float dist = delta.x() * delta.x() + delta.y() * delta.y() + delta.z() * delta.z();
            float radiusSq = a.second.first * a.second.first;
            if(dist < radiusSq){
                auto f = scale( normalizeTo( delta, 1.f - dist / radiusSq ), a.second.second * t );
                position = position + f;
            }
        }
        for( auto a : repulsionBehaviors ) {
            auto delta = a.first - position;
            float dist = delta.x() * delta.x() + delta.y() * delta.y() + delta.z() * delta.z();
            float radiusSq = a.second.first * a.second.first;
            if(dist < radiusSq){
                auto f = scale( normalizeTo( delta, 1.f - dist / radiusSq ), a.second.second * t );
                position = position - f;
            }
        }
        updateConstraints( v );
    }
}

void VerletParticle::updateDM( Scalar t ) {
    if( !isLocked ){
        for( auto b : behaviors ) {
            auto scaledForce = scale( b, dtAgent * dtAgent ); // Scale Gravity force
            force = force + scaledForce;
            scaleVelocity();
            temp = position;
            position = position + ( ( position - prev ) + scale( force, mass ) );
            prev = temp;
            force.setZero();
        }
        for( auto a : attractionBehaviors ) {
            auto delta = a.first - position;
            float dist = delta.x() * delta.x() + delta.y() * delta.y() + delta.z() * delta.z();
            float radiusSq = a.second.first * a.second.first;
            if(dist < radiusSq){
                auto f = scale( normalizeTo( delta, 1.f - dist / radiusSq ), a.second.second * t );
                position = position + f;
            }
        }
        /*for( auto a : repulsionBehaviors ) {
            auto delta = a.first - position;
            float dist = delta.x() * delta.x() + delta.y() * delta.y() + delta.z() * delta.z();
            float radiusSq = a.second.first * a.second.first;
            if(dist < radiusSq){
                auto f = scale( normalizeTo( delta, 1.f - dist / radiusSq ), a.second.second * t );
                position = position - f;
            }
        }*/
        Ra::Core::Vector3f v = ( position - prev ) + scale( force, mass );
        updateConstraints( v );
    }
}
//! [Update Particle]

//! [Springs]
void VerletParticle::addSpring( VerletParticle *particle, Ra::Engine::Rendering::RenderObjectManager *renderObjMan ) {
    auto line = Ra::Engine::Rendering::RenderObject::createRenderObject(
            "line",
            this,
            Ra::Engine::Rendering::RenderObjectType::Geometry,
            Ra::Engine::Data::DrawPrimitives::Line( position, particle->position,
                                                   Ra::Core::Utils::Color::Red() ),
            {} );
    auto plainMaterial = std::make_shared<Ra::Engine::Data::PlainMaterial>( "Plain Material" );
    plainMaterial->m_perVertexColor = true;
    line->setMaterial( plainMaterial );
    renderObjMan->addRenderObject( line );
    //line->setVisible( false );
    springs.push_back( std::make_pair( particle, line ) );
    restLength.push_back( glm::distance( Ra::Core::toGlm( position ) , Ra::Core::toGlm( particle->position ) ) );
}
//! [Springs]

//! [Behaviors]
void VerletParticle::addBehavior( Ra::Core::Vector3f b ) {
    behaviors.push_back( b );
}

void VerletParticle::addAttractionBehavior( Ra::Core::Vector3f attractor, float radius, float strengthAt ) {
    attractionBehaviors.push_back( std::make_pair( attractor, std::make_pair( radius, strengthAt ) ) );
}

void VerletParticle::addRepulsionBehavior( Ra::Core::Vector3f repulsor, float radius, float strengthAt ) {
    repulsionBehaviors.push_back( std::make_pair( repulsor, std::make_pair( radius, strengthAt ) ) );
}
//! [Behaviors]

//! [Constraints]
void VerletParticle::addConstraint( Ra::Core::Aabb c ) {
    constraints.push_back( c );
}

void VerletParticle::updateConstraints( Ra::Core::Vector3f v ) {
    for( auto c : constraints ) {
        if ( position.x() > c.max().x() ) {
            position = Ra::Core::Vector3f( c.max().x(), position.y(), position.z() );
            prev = Ra::Core::Vector3f( position.x() + v.x(), prev.y(), prev.z() );
        } else if ( position.x() < c.min().x() ) {
            position = Ra::Core::Vector3f( c.min().x(), position.y(), position.z() );
            prev = Ra::Core::Vector3f( position.x() + v.x(), prev.y(), prev.z() );
        }
        if ( position.y() > c.max().y() ) {
            position = Ra::Core::Vector3f( position.x(), c.max().y(), position.z() );
            prev = Ra::Core::Vector3f( prev.x(), position.y() + v.y(), prev.z() );
        } else if ( position.y() < c.min().y() ) {
            position = Ra::Core::Vector3f( position.x(), c.min().y(), position.z() );
            prev = Ra::Core::Vector3f( prev.x(), position.y() + v.y(), prev.z() );
        }
        if ( position.z() > c.max().z() ) {
            position = Ra::Core::Vector3f( position.x(), position.y(), c.max().z() );
            prev = Ra::Core::Vector3f( prev.x(), prev.y(), position.z() + v.z() );
        } else if ( position.z() < c.min().z() ) {
            position = Ra::Core::Vector3f( position.x(), position.y(), c.min().z() );
            prev = Ra::Core::Vector3f( prev.x(), prev.y(), position.z() + v.z() );
        }
    }
}
//! [Constraints]

//! [Vector Manipulation]
Ra::Core::Vector3f VerletParticle::normalizeTo( Ra::Core::Vector3f v, float len ){
    float mag = std::sqrt( v.x() * v.x() + v.y() * v.y() + v.z() * v.z() );
    if( mag > 0 ){
        mag = len/mag;
        v *= mag;
    }
    return v;
}


Ra::Core::Vector3f VerletParticle::scale( Ra::Core::Vector3f vec, float s ) {
    return Ra::Core::Vector3f( vec.x() * s, vec.y() * s, vec.z() * s );
}

void VerletParticle::interpolateToSelf( float f ) {
    prev.x() = prev.x() + ( ( position.x() - prev.x() ) * f );
    prev.y() = prev.y() + ( ( position.y() - prev.y() ) * f );
    prev.z() = prev.z() + ( ( position.z() - prev.z() ) * f );
}

void VerletParticle::scaleVelocity() {
    interpolateToSelf( 1.f - drag );
}
//! [Vector Manipulation]

//! [Setters]
void VerletParticle::setMass( float m ) {
    mass = m;
}
//! [Setters]
