
#include "airplane-creature.hpp"
#include <vector>
#include "../creatures/creature.hpp"
#include "builder/model-builder.hpp"
#include "builder/wheel.hpp"

AirplaneCreature::AirplaneCreature(World *world, const Matrix4f &transform) : m_world(world) {
    ModelBuilder::Builder builder;

    builder.get_state().set_matrix(Matrix4f::scale_matrix(0.3f, 0.3f, 0.3f)
                                   * Matrix4f::translation_matrix(0.0f, 1.0f, 0.0f)
                                   * transform);

    // Build a cube with 8 vertices and 12 edges

    builder.vertex((6.3 + -1.0), 0.0f, -2.0f); // front, bottom, left 0
    builder.vertex((8.0 + 1.0), -1.5f, -2.0f); // back, bottom, left 1
    builder.vertex((2.0 + -1.0), 2.0f, -1.0f); // front, top, left 2
    builder.vertex((7.0 + 1.0), 2.0f, -1.0f); // back, top, left 3
    builder.vertex((6.3 + -1.0), 0.0f, 2.0f); // front, bottom, right 4
    builder.vertex((8.0 + 1.0), -1.5f, 2.0f); // back, bottom, right 5
    builder.vertex((2.0 + -1.0), 2.0f, 1.0f); // front, top, right 6
    builder.vertex((7.0 + 1.0), 2.0f, 1.0f); // back, top, right 7

    builder.springs_between_range(0, 8);

    // A plane oriented towards x-negative direction

    // Add wings to the cube

    builder.vertex(11.0f, 2.0f, -10.0f); // 8
    builder.vertex(11.0f, 2.0f, 10.0f); // 9

    // Connect wings to the cube

    builder.spring(0, 8);
    builder.spring(1, 8);
    builder.spring(2, 8);
    builder.spring(3, 8);
    builder.spring(4, 9);
    builder.spring(5, 9);
    builder.spring(6, 9);
    builder.spring(7, 9);

    // Add a tail to the cube

    builder.vertex(23.0f, 2.0f, 0.0f); // 10

    // Add some anchors behind the cube, on the y=2, x=11 line
    // Connect them to the nearest cube vertices and wingtips

    builder.vertex(11.0f, 2.0f, -3.0f); // 11
    builder.vertex(11.0f, 2.0f, 3.0f); // 12

    builder.spring(1, 11);
    builder.spring(5, 11);
    builder.spring(3, 11);
    builder.spring(7, 11);
    builder.spring(8, 11);

    builder.spring(1, 12);
    builder.spring(5, 12);
    builder.spring(3, 12);
    builder.spring(7, 12);
    builder.spring(9, 12);

    builder.spring(11, 12);

    // Add the secondary tail mount point, connect it to those anchors and the bottom near two edges of the cube

    builder.vertex(15.0f, 2.0f, 0.0f); // 13

    builder.spring(1, 13);
    builder.spring(5, 13);
    builder.spring(11, 13);
    builder.spring(12, 13);

    // Connect tail to the secondary tail mount point

    builder.spring(10, 13);

    // Add four more anchors below the tail anchors to form a prolonged triangle, connect them all to each other

    builder.vertex(23.0f, 0.0f, -2.0f); // 14
    builder.vertex(23.0f, 0.0f, 2.0f); // 15
    builder.vertex(15.0f, 0.0f, -2.0f); // 16
    builder.vertex(15.0f, 0.0f, 2.0f); // 17

    builder.spring(10, 14);
    builder.spring(10, 15);
    builder.spring(10, 16);
    builder.spring(10, 17);

    builder.spring(13, 14);
    builder.spring(13, 15);
    builder.spring(13, 16);
    builder.spring(13, 17);

    builder.spring(14, 15);
    builder.spring(14, 16);
    builder.spring(14, 17);

    builder.spring(15, 16);
    builder.spring(15, 17);

    builder.spring(16, 17);

    // Connect the closest bottom of the cube to the bottom anchors

    builder.spring(1, 16);
    builder.spring(1, 17);
    builder.spring(5, 16);
    builder.spring(5, 17);

    // Connect bottom anchors to additional wing anchors (nodes 11 and 12)

    builder.spring(11, 16);
    builder.spring(11, 17);
    builder.spring(12, 16);
    builder.spring(12, 17);

    // Add vertical stabilizer

    builder.vertex(23.0f, 6.0f, 0); // 18

    // Connect vertical stabilizer node to the tail anchors

    builder.spring(10, 18);
    builder.spring(13, 18);
    builder.spring(14, 18);
    builder.spring(15, 18);

    // Add horizontal stabilizers

    builder.vertex(23.0f, 2.0f, -6.0f); // 19
    builder.vertex(23.0f, 2.0f, 6.0f); // 20

    // Connect horizontal stabilizers to the tail anchors

    builder.spring(10, 19);
    builder.spring(10, 20);
    builder.spring(13, 19);
    builder.spring(13, 20);
    builder.spring(14, 19);
    builder.spring(14, 20);
    builder.spring(15, 19);
    builder.spring(15, 20);

    // Add ailerons. They should be connected to wingtips and anchors behind the cube

    builder.vertex(14.0f, 2.0f, -5.0f); // 21
    builder.vertex(14.0f, 2.0f, 5.0f); // 22

    builder.spring(8, 21);
    builder.spring(9, 22);
    builder.spring(11, 21);
    builder.spring(12, 22);

    // Aileron support. Located just under the aileron nodes

    builder.vertex(11.0f, 0.0f, -2.0f); // 23
    builder.vertex(11.0f, 0.0f, 2.0f); // 24

    builder.spring(21, 23);
    builder.spring(8, 23);
    builder.spring(11, 23);
    builder.spring(22, 24);
    builder.spring(9, 24);
    builder.spring(12, 24);

    // Add aileron muscles. They are connected to the aileron support nodes and the lower tail anchors

    m_left_aileron_spring = builder.spring(23, 16);
    m_right_aileron_spring = builder.spring(24, 17);

    // Add elevators

    builder.vertex(26.0f, 2.0f, -3.0f); // 25
    builder.vertex(26.0f, 2.0f, 3.0f); // 26

    builder.spring(10, 25);
    builder.spring(10, 26);

    // Elevator support. Located just under the elevator nodes

    builder.vertex(26.0f, 0.0f, -2.0f); // 27
    builder.vertex(26.0f, 0.0f, 2.0f); // 28

    builder.spring(25, 27);
    builder.spring(26, 28);
    builder.spring(19, 27);
    builder.spring(20, 28);
    builder.spring(25, 19);
    builder.spring(26, 20);
    builder.spring(10, 27);
    builder.spring(10, 28);

    // Add elevator muscles. They are connected to the elevator support nodes and the lower tail anchors

    m_left_elevator_spring = builder.spring(27, 14);
    m_right_elevator_spring = builder.spring(28, 15);

    // Add the wing and tail surfaces

    // Wing (node 8 for left wing, node 9 for right wing)
    builder.surface(8, 2, 3);
    builder.surface(9, 6, 7);

    // Secondary wing surfaces (With use of those anchors behind the cube, indexed 11 and 12)
    builder.surface(8, 11, 3);
    builder.surface(9, 12, 7);

    // Tail (node 10 for tail, node 13 for secondary tail mount point,
    // node 18 for vertical stabilizer, nodes 19 and 20 for horizontal stabilizers)

    builder.surface(10, 13, 18);
    builder.surface(10, 19, 13);
    builder.surface(20, 10, 13);

    // Aileron surfaces (node 22 for left aileron, node 24 for right aileron)

    builder.surface(8, 21, 11);
    builder.surface(9, 22, 12);

    // Elevator surfaces (nodes 27 and 29 for left elevator, nodes 28 and 30 for right elevator)

    builder.surface(19, 10, 25);
    builder.surface(20, 10, 26);

    // Add the wheels (build them from nodes and springs)
    // There are two wheels laying on the axle between nodes 1 and 5

    builder.push_state();
    builder.get_state().m_spring_strength = 50.0f;
    builder.get_state().m_spring_damping = 0.5f;
    builder.get_state().m_vertex_floor_friction = 1.0f;

    ModelBuilder::build_wheel(builder, 1, 5, 1.5f, 16);
    ModelBuilder::build_wheel(builder, 5, 1, 1.5f, 16);

    builder.pop_state();

    builder.get_state().m_vertex_floor_friction = 0.2;

    // Add a low-friction node in the tail that will be used to slide the tail on the ground

    int tail_node = builder.vertex(23.0f, -1.5f, 0.0f);

    // Fix the tail node in place
    builder.spring(13, tail_node);
    builder.spring(14, tail_node);
    builder.spring(15, tail_node);

    // Add jet engines

    float top_thrust_fraction = 0.3;
    float bottom_fraction = 1 - top_thrust_fraction;

    builder.jet(0, 16, m_engine_power / 2 * bottom_fraction);
    builder.jet(2, 3, m_engine_power / 2 * top_thrust_fraction);
    builder.jet(4, 17, m_engine_power / 2 * bottom_fraction);
    builder.jet(6, 7, m_engine_power / 2 * top_thrust_fraction);

    builder.calculate_mass();

    m_left_aileron_muscle_length = builder.get_spring_length(m_left_aileron_spring);
    m_right_aileron_muscle_length = builder.get_spring_length(m_right_aileron_spring);

    m_left_elevator_muscle_length = builder.get_spring_length(m_left_elevator_spring);
    m_right_elevator_muscle_length = builder.get_spring_length(m_right_elevator_spring);

    m_creature = std::make_unique<Creature>(m_world, builder.get_config());
    m_creature->make_visible();
}

void AirplaneCreature::set_throttle(float throttle) {
    for (auto &jet: m_creature->get_jet_objects()) {
        jet->get_physics_jet()->m_force = jet->get_max_force() * throttle;
    }
}

void AirplaneCreature::set_controls(Vec3f controls) {
// Update muscle inputs
    m_creature->get_springs()[m_left_aileron_spring]->get_physics_spring()->m_target_length =
            m_left_aileron_muscle_length * (1.0 + controls.x * 0.1);
    m_creature->get_springs()[m_right_aileron_spring]->get_physics_spring()->m_target_length =
            m_right_aileron_muscle_length * (1.0 - controls.x * 0.1);

    m_creature->get_springs()[m_left_elevator_spring]->get_physics_spring()->m_target_length =
            m_left_elevator_muscle_length * (1.0 + controls.y * 0.3);

    m_creature->get_springs()[m_right_elevator_spring]->get_physics_spring()->m_target_length =
            m_right_elevator_muscle_length * (1.0 + controls.y * 0.3);
}
