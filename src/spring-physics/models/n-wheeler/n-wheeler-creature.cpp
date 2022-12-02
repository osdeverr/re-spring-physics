
#include "n-wheeler-creature.hpp"
#include "../builder/model-builder.hpp"
#include "../builder/wheel.hpp"

NWheelerCreature::NWheelerCreature(World *world, const Matrix4f& transform): m_world(world) {
    ModelBuilder::Builder builder;

    builder.get_state().set_matrix(transform);

    float frame_width = 2.0f;
    float wheel_width = 0.5f;

    int old_frame = -1;

    for(int i = 0; i < m_axles; i++) {
        float x = (float)i * 2.0f;

        int frame_start = builder.current_vertex();
        builder.vertex(x, 2.0f, -frame_width);
        builder.vertex(x, 2.0f, frame_width);
        int left_wheel_anchor = builder.vertex(x, 1.0f, -frame_width);
        int right_wheel_anchor = builder.vertex(x, 1.0f, frame_width);

        builder.springs_between_range(frame_start, builder.current_vertex());

        int left_wheel_support = builder.vertex(x, 1.0f, -frame_width - wheel_width);
        int right_wheel_support = builder.vertex(x, 1.0f, frame_width + wheel_width);

        int left_wheel_steering = builder.vertex(x + 1.0f, 1.0f, -frame_width);
        int right_wheel_steering = builder.vertex(x + 1.0f, 1.0f, frame_width);

        builder.get_state().m_vertex_floor_friction = 2.0f;

        ModelBuilder::build_wheel(builder, left_wheel_anchor, left_wheel_support, 0.5f, 16, 0.5);
        ModelBuilder::build_wheel(builder, right_wheel_anchor, right_wheel_support, 0.5f, 16, 0.5);

        builder.get_state().m_vertex_floor_friction = 0.5f;

        builder.springs_between(left_wheel_anchor, left_wheel_steering, left_wheel_support, frame_start);
        builder.springs_between(right_wheel_anchor, right_wheel_steering, right_wheel_support, frame_start + 1);

        m_steering_springs.push_back(builder.spring(left_wheel_steering, right_wheel_anchor));

        builder.spring(left_wheel_steering, right_wheel_steering);

        if(i == 1) {
            builder.jet(old_frame, frame_start, 40.0f);
            builder.jet(old_frame + 1, frame_start + 1, 40.0f);
        }

        if(i == m_axles - 1) {
            builder.jet(frame_start, old_frame, 40.0f);
            builder.jet(frame_start + 1, old_frame + 1, 40.0f);
        }
//
        if(i > 0) {
            for(int j = 0; j < 4; j++) {
                builder.spring(old_frame + j, frame_start + j);
            }

            builder.spring(old_frame, frame_start + 2);
            builder.spring(old_frame + 1, frame_start + 3);
            builder.spring(old_frame + 2, frame_start);
            builder.spring(old_frame + 2, frame_start + 1);
            builder.spring(old_frame + 2, frame_start + 3);
            builder.spring(old_frame + 3, frame_start + 2);
        }
        old_frame = frame_start;
    }

    builder.calculate_mass();

    m_steering_spring_length = builder.get_spring_length(m_steering_springs[0]);

    m_creature = std::make_unique<Creature>(m_world, builder.get_config());
    m_creature->make_visible();
}

void NWheelerCreature::set_controls(Vec3f controls) {
    float steering = controls.x;
    float throttle = controls.y;

    float steering_max = 0.1f;

    for (int i = 0; i < m_axles; i++) {
        int spring = m_steering_springs[i];
        float steering_factor = steering * (((float)i / (float)m_axles) * 2.0f - 1.0f) * steering_max;

        m_creature->get_springs()[spring]->get_physics_spring()->m_target_length = m_steering_spring_length * (1 + steering_factor);
    }

    float forward = std::max(0.0f, throttle);
    float backward = std::max(0.0f, -throttle);

    m_creature->get_jet_objects()[0]->get_physics_jet()->m_force = m_creature->get_jet_objects()[0]->get_max_force() * forward;
    m_creature->get_jet_objects()[1]->get_physics_jet()->m_force = m_creature->get_jet_objects()[1]->get_max_force() * forward;
    m_creature->get_jet_objects()[2]->get_physics_jet()->m_force = m_creature->get_jet_objects()[2]->get_max_force() * backward;
    m_creature->get_jet_objects()[3]->get_physics_jet()->m_force = m_creature->get_jet_objects()[3]->get_max_force() * backward;


}

void NWheelerCreature::set_throttle(float throttle) {

}
