#include "VerletPhysics.hpp"
#include <Engine/Scene/ComponentMessenger.hpp>
#include <Core/RaCore.hpp>

void VerletPhysics::generateTasks( Ra::Core::TaskQueue* q,
                                const Ra::Engine::FrameInfo& info ) {

    //! [ Update points ]
    // Create a new task which wil call c->spin() when executed.
    for (size_t i = 0; i < m_components.size(); i++) {
        auto particle = static_cast<VerletParticle*>( m_components[i].second );
        particle->dtAgent = info.m_dt;
        q->registerTask(new Ra::Core::FunctionTask(
                std::bind(&VerletParticle::update, particle, info.m_animationTime), "spin"));
    }
    //! [ Update points ]

    //! [ Update springs ]
    for( int z = 0; z < 3; z++ ) { // Loop several times to accelerate equilibrium
        for (size_t i = 0; i < m_components.size(); i++) {
            auto particle = static_cast<VerletParticle*>( m_components[i].second );
            for (size_t j = 0; j < particle->springs.size(); j++) {
                auto spring = static_cast<VerletParticle*>( particle->springs[j].first );
                if( SAMEMASS ) {
                    // First method with all masses equal
                    Ra::Core::Vector3f dv = spring->position - particle->position;
                    float distance = std::sqrt( dv.x() * dv.x() + dv.y() * dv.y() + dv.z() * dv.z() );
                    float difference = particle->restLength[j] - distance;
                    float percent = difference / distance / 2;
                    Ra::Core::Vector3f offset = dv * percent;
                    if ( !particle->isLocked )
                        particle->position = particle->position - offset;
                    if ( !spring->isLocked )
                        spring->position = spring->position + offset;


                    // Second method with all masses equal
                    /*Ra::Core::Vector3f delta = objs[i]->springs[j].first->position - objs[i]->position;
                    float deltaLength = std::sqrt(
                        delta.x() * delta.x() + delta.y() * delta.y() + delta.z() * delta.z() );
                    float diff = ( deltaLength - objs[i]->restLength[j] ) / deltaLength;
                    if (!objs[i]->isLocked)
                        objs[i]->position = objs[i]->position + ( delta * 0.5f * diff );
                    if (!objs[i]->springs[j].first->isLocked)
                        objs[i]->springs[j].first->position =
                            objs[i]->springs[j].first->position - ( delta * 0.5f * diff );*/

                }
                else {
                    // Method for different masses
                    Ra::Core::Vector3f delta = spring->position - particle->position;
                    float deltaLength = std::sqrt( delta.x() * delta.x() + delta.y() * delta.y() + delta.z() * delta.z() );
                    float invMass1 = 1.f / particle->mass;
                    float invMass2 = 1.f / spring->mass;
                    float diff = ( deltaLength - particle->restLength[j] )
                            / ( deltaLength * ( invMass1 + invMass2 ) ) * particle->strength;
                    if (!particle->isLocked)
                        particle->position = particle->position + ( invMass1 * delta * diff );
                    if (!spring->isLocked)
                        spring->position = spring->position - ( invMass2 * delta * diff );
                }
            }
        }
    }
    //! [ Update springs ]

    //! [ Render ]
    for (size_t i = 0; i < m_components.size(); i++) {
        auto particle = static_cast<VerletParticle*>( m_components[i].second );
        //! [ Render Springs ]
        for( size_t j = 0; j < particle->springs.size(); j++ ) {

            auto meshPtr = particle->springs[j].second->getMesh().get();
            auto oldMesh = dynamic_cast<Ra::Engine::Data::LineMesh*>( meshPtr );

            if(oldMesh != nullptr){

                auto &tab = oldMesh->getCoreGeometry().verticesWithLock();

                tab[0] = particle->position;
                tab[1] = particle->springs[j].first->position;

                oldMesh->getCoreGeometry().verticesUnlock();

            }

        }
        //! [ Render Springs ]

        //! [ Render Particles ]
        /// Translate object to position
        Ra::Core::Transform T = Ra::Core::Transform::Identity();
        T.translate( particle->position - particle->initPos );
        particle->m_transform.insertKeyFrame( info.m_animationTime, T );

        //! [Fetch transform from KeyFramedValue]
        auto nT = particle->m_transform.at(info.m_animationTime, Ra::Core::Animation::linearInterpolate<Ra::Core::Transform>);
        //! [Fetch transform from KeyFramedValue]
        particle->m_ro->setLocalTransform(nT);
        //! [ Render Particles ]
    }
    //! [ Render ]
}
