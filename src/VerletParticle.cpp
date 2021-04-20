#include "VerletParticle.hpp"

VerletParticle::VerletParticle( const std::string &name,
                               Ra::Engine::Scene::Entity *entity,
                               Ra::Core::Geometry::TriangleMesh &&mesh, Ra::Core::Vector3f &pos, bool lock,
                               float length ) :
        Ra::Engine::Scene::TriangleMeshComponent(name,
                                                 entity,
                                                 std::move(mesh)),
        m_transform(Ra::Core::Transform::Identity(), 0_ra) {

    position = pos;
    prev = pos;
    initPos = pos; // Necessary for translation
    if ( SAMEMASS ) {
        mass = 1.f; // 1kg
        drag = 0.999; // rho * C * Area - simplified drag for this example
        gravity = Ra::Core::Vector3f(0.0, -0.01, 0.0); // 9.81m/s^2 down in the Y-axis
    } else {
        mass = 1000.f; // 1kg
        drag = 0.999; // rho * C * Area - simplified drag for this example
        gravity = Ra::Core::Vector3f(0.0, -9.81, 0.0); // 9.81m/s^2 down in the Y-axis
        force = Ra::Core::Vector3f(0.f, 0.f, 0.f);
        temp = Ra::Core::Vector3f(0.f, 0.f, 0.f);
    }
    m_ro = getRoMgr()->getRenderObject(m_roIndex);
    restLength = length;
    strength = 0.8f;
    isLocked = lock;
    //m_ro->setVisible(false);
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

void VerletParticle::update( Scalar t ) {
    // Update Point
    if( !isLocked ){
        if( SAMEMASS ) {
            Ra::Core::Vector3f v = ( position - prev ) * drag;
            prev = position;
            position = position + v;
            position = position + gravity;
        }
        else {
            auto scaledForce = scale( gravity, dtAgent * dtAgent ); // Scale Gravity force
            // Update Particles
            force = force + scaledForce;
            scaleVelocity();
            temp = position;
            position = position + ( ( position - prev ) + scale( force, mass ) );
            prev = temp;
            force.setZero();
        }

        // Check bounds collision in x, y and z
        /*int box = 10;
        if( position.x() > box ) {
            position = Ra::Core::Vector3f( box, position.y(), position.z() );
            //position = Ra::Core::Vector3f( 2.f*box - position.x(), position.y(), position.z() );
            prev = Ra::Core::Vector3f( position.x() + v.x() * bounce, prev.y(), prev.z() );
        }
        else if ( position.x() < -box ) {
            position = Ra::Core::Vector3f( -box, position.y(), position.z() );
            prev = Ra::Core::Vector3f( position.x() + v.x() * bounce, prev.y(), prev.z() );
        }
        if( position.y() > box ) {
            position = Ra::Core::Vector3f( position.x(), box, position.z() );
            prev = Ra::Core::Vector3f( prev.x(), position.y() + v.y() * bounce, prev.z() );
        }
        else if ( position.y() < -box ) {
            position = Ra::Core::Vector3f( position.x(), -box, position.z() );
            prev = Ra::Core::Vector3f( prev.x(), position.y() + v.y() * bounce, prev.z() );
        }
        if( position.z() > box ) {
            position = Ra::Core::Vector3f( position.x(), position.y(), box );
            prev = Ra::Core::Vector3f( prev.x(), prev.y(), position.z() + v.z() * bounce );
        }
        else if ( position.z() < -box ) {
            position = Ra::Core::Vector3f( position.x(), position.y(), -box );
            prev = Ra::Core::Vector3f( prev.x(), prev.y(), position.z() + v.z() * bounce );
        }*/
    }
}
