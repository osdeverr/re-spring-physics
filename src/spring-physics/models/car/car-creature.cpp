
#include "car-creature.hpp"
#include "../builder/model-builder.hpp"
#include "../builder/full-cube.hpp"
#include "../builder/wheel.hpp"
#include "car-factory.hpp"

CarCreature::CarCreature(World *world, const Matrix4f& transform): m_world(world) {
    ModelBuilder::Builder builder;

    builder.get_state().set_matrix(transform);

    CarFactory factory(builder);
    factory.build();

    m_back_steering_spring = factory.m_back_steering_spring;
    m_front_steering_spring = factory.m_front_steering_spring;

    m_back_steering_spring_length = builder.get_spring_length(m_back_steering_spring);
    m_front_steering_spring_length = builder.get_spring_length(m_front_steering_spring);

    m_creature = std::make_unique<Creature>(m_world, builder.get_config());
    m_creature->make_visible();
}

void CarCreature::set_controls(Vec3f controls) {
    float steering = controls.x;
    float throttle = controls.y;

    float steering_max = 0.2f;

    float back_steering_length = m_back_steering_spring_length + steering * steering_max;

    m_creature->get_springs()[m_back_steering_spring]->get_physics_spring()->m_target_length = back_steering_length;

    float front_steering_length = m_front_steering_spring_length - steering * steering_max;

    m_creature->get_springs()[m_front_steering_spring]->get_physics_spring()->m_target_length = front_steering_length;

    float forward = std::max(0.0f, throttle);
    float backward = std::max(0.0f, -throttle);

    m_creature->get_jet_objects()[0]->get_physics_jet()->m_force = m_creature->get_jet_objects()[0]->get_max_force() * forward;
    m_creature->get_jet_objects()[1]->get_physics_jet()->m_force = m_creature->get_jet_objects()[1]->get_max_force() * forward;
    m_creature->get_jet_objects()[2]->get_physics_jet()->m_force = m_creature->get_jet_objects()[2]->get_max_force() * backward;
    m_creature->get_jet_objects()[3]->get_physics_jet()->m_force = m_creature->get_jet_objects()[3]->get_max_force() * backward;


}

void CarCreature::set_throttle(float throttle) {

}
