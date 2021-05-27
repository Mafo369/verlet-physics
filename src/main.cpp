#include "VerletParticle.hpp"
#include "VerletPhysics.hpp"

// Include Radium base application and its simple Gui
#include <Gui/BaseApplication.hpp>
#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

float w = 1.0;


void verletCloth( Ra::Gui::BaseApplication &app, VerletPhysics* sys, int rows, int cols ) {
    float radius = 0.1f;

    std::vector<std::pair<Ra::Engine::Scene::Entity*, VerletParticle*>> objects;
    float z = -( rows / 2 );
    for( int i = 0; i < rows; i++ ) {
        float x = -( cols / 2 );
        for( int j = 0; j < cols; j++ ) {
            //! [Create the animated component]

            //! [Creating the cube]
            auto pos = Ra::Core::Vector3f( x, 40.f, z );
            auto halfExts = Ra::Core::Vector3f(radius, radius, radius);
            Ra::Core::Aabb aabb(-halfExts, halfExts);
            aabb.translate(pos);

            auto cube = Ra::Core::Geometry::makeSharpBox(aabb, Ra::Core::Utils::Color::Red() );
            //! [Creating the cube]

            //! [Create the engine entity for the cube]
            auto e = app.m_engine->getEntityManager()->createEntity( "Green cube " + std::to_string( i ) + " "
                    + std::to_string( j ) );
            //! [Create the engine entity for the cube]

            //! [Lock particle]
            bool lock = false;
            if( ( i == rows-1 && j == cols-1 ) || ( i == rows-1 && j == 0 ) || (i == 0 && j == 0)
                || ( i == 0 && j == cols-1 ) )
                lock = true;
            //! [Lock particle]

            //! [Create a VerletParticle component with the cube]
            auto c = new VerletParticle( "Cube Mesh", e, std::move( cube ), pos, lock );
            //! [Create a VerletParticle component with the cube]

            //! [Modify attributes and add behaviors/constraints]
            //c->setMass( 50.f );
            auto gravity = Ra::Core::Vector3f(0.0, -0.01, 0.0);
            //auto gravity = Ra::Core::Vector3f(0.0, -9.81, 0.0); // 9.81m/s^2 down in the Y-axis
            c->addBehavior( gravity );
            //! [Modify attributes and add behaviors/constraints]

            //! [Create the demo animated component]

            objects.push_back( std::make_pair( e,c ) );
            x = x + w;
        }
        z = z + w;
    }

    for( int i = 0; i < rows; i++ ) {
        for (int j = 0; j < cols; j++) {
            //! [Access particle]
            auto particle = objects[i*cols + j].second;
            //! [Access particle]

            //! [Add Spring]
            if( i != cols-1 ){
                particle->addSpring( objects[(i+1)*cols + j].second, app.m_engine->getRenderObjectManager() );
            }
            if( j != rows-1 ){
                particle->addSpring( objects[(i*cols) + (j+1)].second, app.m_engine->getRenderObjectManager() );
            }
            //! [Add Spring]

            //! [Add the component to the animation system]
            sys->addComponent( objects[i*cols + j].first, objects[i*cols + j].second );
            //! [Add the component to the animation system]
        }
    }
}

void createVerletParticle( Ra::Core::Vector3f pos, float radius, Ra::Gui::BaseApplication &app,
                           std::vector<std::pair<Ra::Engine::Scene::Entity*, VerletParticle*>> &objects,
                           bool lock=false, Ra::Core::Utils::Color col=Ra::Core::Utils::Color::Red()){
    //! [Create the demo animated component]

    //! [Creating the cube]
    auto halfExts = Ra::Core::Vector3f(radius, radius, radius);
    Ra::Core::Aabb aabb(-halfExts, halfExts);
    aabb.translate(pos);

    auto cube = Ra::Core::Geometry::makeSharpBox(aabb, col );
    //! [Creating the cube]

    //! [Create the engine entity for the cube]
    auto e = app.m_engine->getEntityManager()->createEntity( "Green cube " + std::to_string( 0 ) + " "
            + std::to_string( 1 ) );
    //! [Create the engine entity for the cube]


    //! [Create a VerletParticle component with the cube]
    auto c = new VerletParticle( "Cube Mesh", e, std::move( cube ), pos, lock );
    //! [Create a VerletParticle component with the cube]

    //! [Modify attributes and add behaviors/constraints]
    auto gravity = Ra::Core::Vector3f(0.0, -0.001, 0.0);
    //auto gravity = Ra::Core::Vector3f(0.0, -9.81, 0.0); // 9.81m/s^2 down in the Y-axis
    //c->addBehavior( gravity );
    c->prev = pos + 1.5f * Ra::Core::Vector3f(-0.05f, 0.0f, -0.01f );
    c->addAttractionBehavior(Ra::Core::Vector3f(0.0, 3.0, 0.0), 15.f, 0.001f);
    c->addRepulsionBehavior(Ra::Core::Vector3f(0.0, 3.0, 0.0), 7.f, 0.001f);

    auto min = Ra::Core::Vector3f( -5.0, 0.0 + 0.1f, -5.0 );
    auto max = Ra::Core::Vector3f( 5.0, 8.0, 5.0 );
    Ra::Core::Aabb aabb_c( min, max );
    //c->addConstraint( aabb_c );
    //! [Modify attributes and add behaviors/constraints]

    //! [Create the demo animated component]

    objects.push_back( std::make_pair( e,c ) );
}

void verletBox( Ra::Gui::BaseApplication &app, VerletPhysics* sys ) {
    float radius = 0.1f;

    std::vector<std::pair<Ra::Engine::Scene::Entity*, VerletParticle*>> objects;

    //! [Create Particles with behaviors and constraints]
    createVerletParticle( Ra::Core::Vector3f( -0.5, 6.0, 0.0 ) , radius, app, objects ); //0
    createVerletParticle( Ra::Core::Vector3f( 0.5, 6.0, 0.0 ) , radius, app, objects ); //1
    createVerletParticle( Ra::Core::Vector3f( -0.5, 7.0, 0.0 ) , radius, app, objects ); //2
    createVerletParticle( Ra::Core::Vector3f( 0.5, 7.0, 0.0 ) , radius, app, objects ); //3
    createVerletParticle( Ra::Core::Vector3f( -0.5, 6.0, 1.0 ) , radius, app, objects ); //4
    createVerletParticle( Ra::Core::Vector3f( 0.5, 6.0, 1.0 ) , radius, app, objects ); //5
    createVerletParticle( Ra::Core::Vector3f( -0.5, 7.0, 1.0 ) , radius, app, objects ); //6
    createVerletParticle( Ra::Core::Vector3f( 0.5, 7.0, 1.0 ) , radius, app, objects ); //7
    //! [Create Particles with behaviors and constraints]

    //! [Add Springs]
    auto particle = objects[0].second;
    particle->addSpring( objects[1].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[2].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[4].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[5].second, app.m_engine->getRenderObjectManager() );


    particle = objects[7].second;
    particle->addSpring( objects[5].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[0].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[1].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[4].second, app.m_engine->getRenderObjectManager() );

    particle = objects[2].second;
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[5].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[4].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[1].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[7].second, app.m_engine->getRenderObjectManager() );

    particle = objects[5].second;
    particle->addSpring( objects[4].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[1].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );

    particle = objects[4].second;
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[1].second, app.m_engine->getRenderObjectManager() );

    particle = objects[1].second;
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );

    particle = objects[3].second;
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );
    //! [Add Springs]

    //! [Create particle at the center of the scene for correct camera position]
    createVerletParticle( Ra::Core::Vector3f( 0.0, 0.0, 0.0 ) , 0.01, app, objects,
                          true ); //8
    //! [Create particle at the center of the scene for correct camera position]

    //! [Create attractor - for visual purposes only]
    createVerletParticle( Ra::Core::Vector3f(0.0, 3.0, 0.0) , 0.1, app, objects, true,
                          Ra::Core::Utils::Color::Blue() ); //9
    //objects[9].second->addRepulsionBehavior(Ra::Core::Vector3f(0.0, 3.0, 0.0), 10.f, 10.0f);
    //! [Create attractor - for visual purposes only]

    //! [Add the component to the animation system]
    for(size_t i = 0; i < objects.size(); i++ ){
        sys->addComponent( objects[i].first, objects[i].second );
    }
    //! [Add the component to the animation system]
}

void verletGravity( Ra::Gui::BaseApplication &app, VerletPhysics* sys ) {
    float radius = 0.1f;

    std::vector<std::pair<Ra::Engine::Scene::Entity*, VerletParticle*>> objects;

    //! [Create Particles with behaviors and constraints]
    createVerletParticle( Ra::Core::Vector3f( -0.5, 6.0, 0.0 ) , radius, app, objects ); //0
    createVerletParticle( Ra::Core::Vector3f( 0.5, 7.0, 0.0 ) , radius, app, objects ); //1
    //! [Create Particles with behaviors and constraints]

    //! [Add Springs]
    auto particle = objects[0].second;
    particle->addSpring( objects[1].second, app.m_engine->getRenderObjectManager() );
    //! [Add Springs]

    //! [Create particle at the center of the scene for correct camera position]
    createVerletParticle( Ra::Core::Vector3f( 0.0, 0.0, 0.0 ) , 0.01, app, objects,
                          true ); //2
    //! [Create particle at the center of the scene for correct camera position]

    //! [Create attractor - for visual purposes only]
    //createVerletParticle( Ra::Core::Vector3f(0.0, 3.0, 0.0) , 0.1, app, objects, true,
    //                      Ra::Core::Utils::Color::Blue()); //3
    //! [Create attractor - for visual purposes only]

    //! [Add the component to the animation system]
    for(size_t i = 0; i < objects.size(); i++ ){
        sys->addComponent( objects[i].first, objects[i].second );
    }
    //! [Add the component to the animation system]
}

int main( int argc, char* argv[] ) {
    //! [Creating the application]
    Ra::Gui::BaseApplication app( argc, argv );
    app.initialize( Ra::Gui::SimpleWindowFactory {} );
    app.m_mainWindow->resize(1900,1060);

    //![Parameterize the Engine  time loop]
    app.m_engine->setRealTime( true );
    app.m_engine->play( true );
    app.m_targetFPS = 60;

    //! [Create the physics system]
    VerletPhysics* sys = new VerletPhysics;
    //sys->type = 0;
    app.m_engine->registerSystem( "Verlet Physics", sys );
    //! [Create the physics system]

    //! [Create objects]
    int rows = 30;
    int cols = 30;
    //verletCloth( app, sys, rows, cols );
    verletBox( app, sys );
    //verletGravity( app, sys );
    //! [Create objects]

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]

    return app.exec();
}
