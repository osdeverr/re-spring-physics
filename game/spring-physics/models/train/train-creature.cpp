
#include "train-creature.hpp"
#include "../builder/wheel.hpp"

TrainCreature::TrainCreature(World *world, const Matrix4f& transform): m_world(world) {
    ModelBuilder::Builder builder;

    builder.get_state().set_matrix(transform);

    int connection = build_locomotive(builder);
    m_locomotive_end = builder.current_vertex();

    for(int i = 0; i < m_wagons; i++) {
        connection = build_wagon(builder, connection);
    }

    builder.calculate_mass();

    m_steering_spring_length = builder.get_spring_length(m_steering_springs[0]);

    m_creature = std::make_unique<Creature>(m_world, builder.get_config());
    m_creature->make_visible();
}

int TrainCreature::build_wagon(ModelBuilder::Builder &builder, int connection) {
    int x = builder.get_vertex_pos(connection)[0] + m_connection_length / 2;
    int front_axis = build_axis(x + 1.0, builder);
    int back_axis = build_axis(x + m_wagon_length, builder);

    connect_axles(builder, front_axis, back_axis);

    int forecourt = builder.vertex(x + m_wagon_length + m_connection_length, 1.0f, 0.0f);

    builder.springs_between(forecourt, back_axis);
    builder.springs_between(forecourt, back_axis + 1);
    builder.springs_between(forecourt, back_axis + 2);
    builder.springs_between(forecourt, back_axis + 3);

    // Connect steering rod with the connection in front
    builder.springs_between(connection, front_axis + 6);
    builder.springs_between(connection, front_axis + 7);

    // Lock steering of back axle
    builder.spring(back_axis + 6, back_axis + 1);

    int forecourt2 = builder.vertex(x + m_wagon_length + m_connection_length + 2.0f, 1.0f, 0.0f);
    builder.springs_between(forecourt2, forecourt);

    return forecourt2;
}

int TrainCreature::build_locomotive(ModelBuilder::Builder &builder) {
    int old_frame = -1;

    for(int i = 0; i < m_locomotive_axles; i++) {
        float x = (float)i * 2.0f;
        int frame_start = build_axis(x, builder);

        if(i == 1) {
            builder.jet(old_frame, frame_start, 40.0f);
            builder.jet(old_frame + 1, frame_start + 1, 40.0f);
        }

        if(i == m_locomotive_axles - 1) {
            builder.jet(frame_start, old_frame, 40.0f);
            builder.jet(frame_start + 1, old_frame + 1, 40.0f);
        }

        m_steering_springs.push_back(builder.spring(frame_start + 6, frame_start + 1));

        if(i > 0) {
            connect_axles(builder, old_frame, frame_start);
        }
        old_frame = frame_start;
    }

    int forecourt = builder.vertex(2.0f * m_locomotive_axles + 0.5f, 1.0f, 0.0f);

    builder.springs_between(forecourt, old_frame);
    builder.springs_between(forecourt, old_frame + 1);
    builder.springs_between(forecourt, old_frame + 2);
    builder.springs_between(forecourt, old_frame + 3);

    int forecourt2 = builder.vertex(2.0f * m_locomotive_axles + 2.5f, 1.0f, 0.0f);
    builder.springs_between(forecourt2, forecourt);

    return forecourt2;
}

void TrainCreature::connect_axles(ModelBuilder::Builder &builder, int frame_a, int frame_b) {
    for(int j = 0; j < 4; j++) {
        builder.spring(frame_a + j, frame_b + j);
    }

    builder.spring(frame_a, frame_b + 2);
    builder.spring(frame_a + 1, frame_b + 3);
    builder.spring(frame_a + 2, frame_b);
    builder.spring(frame_a + 2, frame_b + 1);
    builder.spring(frame_a + 2, frame_b + 3);
    builder.spring(frame_a + 3, frame_b + 2);
}

int TrainCreature::build_axis(float x, ModelBuilder::Builder &builder) {
    int frame_start = builder.current_vertex();
    builder.vertex(x, 2.0f, -m_frame_width);
    builder.vertex(x, 2.0f, m_frame_width);
    int left_wheel_anchor = builder.vertex(x, 1.0f, -m_frame_width);
    int right_wheel_anchor = builder.vertex(x, 1.0f, m_frame_width);

    builder.springs_between_range(frame_start, builder.current_vertex());

    int left_wheel_support = builder.vertex(x, 1.0f, -m_frame_width - m_wheel_width);
    int right_wheel_support = builder.vertex(x, 1.0f, m_frame_width + m_wheel_width);

    int left_wheel_steering = builder.vertex(x - 1.0f, 1.0f, -m_frame_width);
    int right_wheel_steering = builder.vertex(x - 1.0f, 1.0f, m_frame_width);

    builder.get_state().m_vertex_floor_friction = 2.0f;

    ModelBuilder::build_wheel(builder, left_wheel_anchor, left_wheel_support, 0.5f, 16, 0.5);
    ModelBuilder::build_wheel(builder, right_wheel_anchor, right_wheel_support, 0.5f, 16, 0.5);

    builder.get_state().m_vertex_floor_friction = 0.5f;

    builder.springs_between(left_wheel_anchor, left_wheel_steering, left_wheel_support, frame_start);
    builder.springs_between(right_wheel_anchor, right_wheel_steering, right_wheel_support, frame_start + 1);

    builder.spring(left_wheel_steering, right_wheel_steering);

    return frame_start;
}

void TrainCreature::set_controls(Vec3f controls) {
    float steering = controls.x;
    float throttle = controls.y;

    float steering_max = 0.1f;

    for (int i = 0; i < m_locomotive_axles; i++) {
        int spring = m_steering_springs[i];
        float spring_length = m_steering_spring_length;
        float steering_factor = steering * (((float)i / (float)m_locomotive_axles) * 2.0f - 1.0f) * steering_max;

        m_creature->get_springs()[spring]->get_physics_spring()->m_target_length = m_steering_spring_length * (1 + steering_factor);
    }

    float forward = std::max(0.0f, throttle);
    float backward = std::max(0.0f, -throttle);

    m_creature->get_jet_objects()[2]->get_physics_jet()->m_force = m_creature->get_jet_objects()[0]->get_max_force() * forward;
    m_creature->get_jet_objects()[3]->get_physics_jet()->m_force = m_creature->get_jet_objects()[1]->get_max_force() * forward;
    m_creature->get_jet_objects()[0]->get_physics_jet()->m_force = m_creature->get_jet_objects()[2]->get_max_force() * backward;
    m_creature->get_jet_objects()[1]->get_physics_jet()->m_force = m_creature->get_jet_objects()[3]->get_max_force() * backward;


}

void TrainCreature::set_throttle(float throttle) {

}