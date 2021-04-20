#include "VerletParticle.hpp"
#include "VerletPhysics.hpp"
#include "VerletSpring.hpp"

// Include Radium base application and its simple Gui
#include <Gui/BaseApplication.hpp>
#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

#include <glm/glm.hpp>

float w = 1.0;

void verletCloth( Ra::Gui::BaseApplication &app, VerletPhysics* sys, int rows, int cols ) {

    float l = glm::distance( glm::vec3( -10.f, 0.f, -10.f ), glm::vec3( -9.f, 0.f, -10 )  );

    float radius = 0.1f;

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
            auto c = new VerletParticle( "Cube Mesh", e, std::move( cube ), pos, lock, l );
            //! [Create a KeyFramedGeometryComponent component with the cube]
            //! [Create the demo animated component]
            objects.push_back( std::make_pair( e,c ) );
            x = x + w;
        }
        z = z + w;
    }

    for( int i = 0; i < rows; i++ ) {
        for (int j = 0; j < cols; j++) {
            std::vector<Spring> springs;
            if( i != cols-1 ){
                auto line = Ra::Engine::Rendering::RenderObject::createRenderObject(
                        "test_line",
                        objects[i*cols + j].second,
                        Ra::Engine::Rendering::RenderObjectType::Geometry,
                        Ra::Engine::Data::DrawPrimitives::Line(objects[i*cols + j].second->position, objects[(i+1)*cols + j].second->position,
                                                               Ra::Core::Utils::Color::Red()),
                        {});
                auto plainMaterial = std::make_shared<Ra::Engine::Data::PlainMaterial>("Plain Material");
                plainMaterial->m_perVertexColor = true;
                line->setMaterial(plainMaterial);
                app.m_engine->getRenderObjectManager()->addRenderObject( line );
                line->setVisible( false );
                springs.push_back( std::make_pair( objects[(i+1)*cols + j].second, line ) );
            }
            if( j != rows-1 ){
                auto line = Ra::Engine::Rendering::RenderObject::createRenderObject(
                        "test_line",
                        objects[i*cols + j].second,
                        Ra::Engine::Rendering::RenderObjectType::Geometry,
                        Ra::Engine::Data::DrawPrimitives::Line(objects[i*cols + j].second->position, objects[(i*cols) + (j+1)].second->position,
                                                               Ra::Core::Utils::Color::Red()),
                        {});
                auto plainMaterial = std::make_shared<Ra::Engine::Data::PlainMaterial>("Plain Material");
                plainMaterial->m_perVertexColor = true;
                line->setMaterial(plainMaterial);
                app.m_engine->getRenderObjectManager()->addRenderObject( line );
                line->setVisible( false );
                springs.push_back( std::make_pair( objects[(i*cols) + (j+1)].second, line ) );
            }
            objects[i*cols + j].second->springs = springs;

            //! [add the component to the animation system]
            sys->addComponent( objects[i*cols + j].first, objects[i*cols + j].second );
        }
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

    int rows = 20;
    int cols = 20;
    verletCloth( app, sys, rows, cols );

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]

    return app.exec();
}
