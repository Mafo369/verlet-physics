#include "VerletParticle.hpp"
#include "VerletPhysics.hpp"

// Include Radium base application and its simple Gui
#include <Gui/BaseApplication.hpp>
#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

float w = 1.0;

void verletCloth( Ra::Gui::BaseApplication &app, VerletPhysics* sys, int rows, int cols ) {
    float radius = 0.01f;

    std::vector<std::pair<Ra::Engine::Scene::Entity*, VerletParticle*>> objects;
    float z = -( rows / 2 );
    for( int i = 0; i < rows; i++ ) {
        float x = -( cols / 2 );
        for( int j = 0; j < cols; j++ ) {
            //! [Create the demo animated component]
            //! [Creating the cube]
            auto pos = Ra::Core::Vector3f( x, 15.f, z );

            auto halfExts = Ra::Core::Vector3f(radius, radius, radius);
            Ra::Core::Aabb aabb(-halfExts, halfExts);
            aabb.translate(pos);

            auto cube = Ra::Core::Geometry::makeSharpBox(aabb, Ra::Core::Utils::Color::Red() );

            //! [Creating the cube]

            //! [Create the engine entity for the cube]
            auto e = app.m_engine->getEntityManager()->createEntity( "Green cube " + std::to_string( i ) + " " + std::to_string( j ) );
            //! [Create the engine entity for the cube]

            bool lock = false;
            if( ( i == rows-1 && j == cols-1 ) || ( i == rows-1 && j == 0 ) || (i == 0 && j == 0) || ( i == 0 && j == cols-1 ) )
                lock = true;

            //! [Create a KeyFramedGeometryComponent component with the cube]
            auto c = new VerletParticle( "Cube Mesh", e, std::move( cube ), pos, lock );
            //! [Create a KeyFramedGeometryComponent component with the cube]
            //! [Create the demo animated component]
            objects.push_back( std::make_pair( e,c ) );
            x = x + w;
        }
        z = z + w;
    }

    for( int i = 0; i < rows; i++ ) {
        for (int j = 0; j < cols; j++) {
            auto particle = objects[i*cols + j].second;
            if( i != cols-1 ){
                particle->addSpring( objects[(i+1)*cols + j].second, app.m_engine->getRenderObjectManager() );
            }
            if( j != rows-1 ){
                particle->addSpring( objects[(i*cols) + (j+1)].second, app.m_engine->getRenderObjectManager() );
            }
            //! [add the component to the animation system]
            sys->addComponent( objects[i*cols + j].first, objects[i*cols + j].second );
        }
    }
}

void createVerletParticle( Ra::Core::Vector3f pos, float radius, Ra::Gui::BaseApplication &app, std::vector<std::pair<Ra::Engine::Scene::Entity*, VerletParticle*>> &objects ){
    //! [Create the demo animated component]
    //! [Creating the cube]

    auto halfExts = Ra::Core::Vector3f(radius, radius, radius);
    Ra::Core::Aabb aabb(-halfExts, halfExts);
    aabb.translate(pos);

    auto cube = Ra::Core::Geometry::makeSharpBox(aabb, Ra::Core::Utils::Color::Red() );

    //! [Creating the cube]

    //! [Create the engine entity for the cube]
    auto e = app.m_engine->getEntityManager()->createEntity( "Green cube " + std::to_string( 0 ) + " " + std::to_string( 1 ) );
    //! [Create the engine entity for the cube]


    //! [Create a KeyFramedGeometryComponent component with the cube]
    auto c = new VerletParticle( "Cube Mesh", e, std::move( cube ), pos, false );
    //! [Create a KeyFramedGeometryComponent component with the cube]
    //! [Create the demo animated component]
    objects.push_back( std::make_pair( e,c ) );
}

void verletBox( Ra::Gui::BaseApplication &app, VerletPhysics* sys ) {
    float radius = 0.01f;

    std::vector<std::pair<Ra::Engine::Scene::Entity*, VerletParticle*>> objects;

    createVerletParticle( Ra::Core::Vector3f( -5.0, 6.0, 0.0 ) , radius, app, objects ); //0
    createVerletParticle( Ra::Core::Vector3f( -4.0, 6.0, 0.0 ) , radius, app, objects ); //1
    createVerletParticle( Ra::Core::Vector3f( -5.0, 7.0, 0.0 ) , radius, app, objects ); //2
    createVerletParticle( Ra::Core::Vector3f( -4.0, 7.0, 0.0 ) , radius, app, objects ); //3
    createVerletParticle( Ra::Core::Vector3f( -5.0, 6.0, 1.0 ) , radius, app, objects ); //4
    createVerletParticle( Ra::Core::Vector3f( -4.0, 6.0, 1.0 ) , radius, app, objects ); //5
    createVerletParticle( Ra::Core::Vector3f( -5.0, 7.0, 1.0 ) , radius, app, objects ); //6
    createVerletParticle( Ra::Core::Vector3f( -4.0, 7.0, 1.0 ) , radius, app, objects ); //7

    auto particle = objects[0].second;
    particle->addSpring( objects[1].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[2].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[4].second, app.m_engine->getRenderObjectManager() );


    particle = objects[7].second;
    particle->addSpring( objects[5].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[0].second, app.m_engine->getRenderObjectManager() );

    particle = objects[2].second;
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[5].second, app.m_engine->getRenderObjectManager() );

    particle = objects[5].second;
    particle->addSpring( objects[4].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[1].second, app.m_engine->getRenderObjectManager() );

    particle = objects[4].second;
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );

    particle = objects[1].second;
    particle->addSpring( objects[3].second, app.m_engine->getRenderObjectManager() );
    particle->addSpring( objects[6].second, app.m_engine->getRenderObjectManager() );

    createVerletParticle( Ra::Core::Vector3f( 0.0, 0.0, 0.0 ) , radius, app, objects ); //7


    //! [add the component to the animation system]
    for(size_t i = 0; i < objects.size(); i++ ){
        sys->addComponent( objects[i].first, objects[i].second );
    }
}

int main( int argc, char* argv[] ) {
    //! [Creating the application]
    Ra::Gui::BaseApplication app( argc, argv );
    app.initialize( Ra::Gui::SimpleWindowFactory {} );

    //![Parameterize the Engine  time loop]
    app.m_engine->setRealTime( true );
    app.m_engine->play( true );
    app.m_targetFPS = 60;

    //! [Create the demo animation system]
    VerletPhysics* sys = new VerletPhysics;
    app.m_engine->registerSystem( "Verlet Physics", sys );

    int rows = 10;
    int cols = 10;
    verletCloth( app, sys, rows, cols );
    //verletBox( app, sys );

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]

    return app.exec();
}
