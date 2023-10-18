
#include "influence_area_data.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void InfluenceAreaData::set_area_node(InfluenceArea *p_area_node) {
	area_node = p_area_node;
}

InfluenceArea *InfluenceAreaData::get_area_node() const {
	return area_node;
}

bool InfluenceAreaData::area_node_is_valid() const {
	return (area_node != nullptr && area_node->is_inside_tree());
}

Vector2 InfluenceAreaData::get_global_position() const {
	if (!area_node_is_valid()) {
		return Vector2();
	}
	return area_node->get_global_position();
}

void InfluenceAreaData::set_influence_type(int p_influence_type) {
	influence_type = p_influence_type;
}

int InfluenceAreaData::get_influence_type() const {
	return influence_type;
}

void InfluenceAreaData::set_layer(int p_layer) {
	layer = p_layer;
}

int InfluenceAreaData::get_layer() const {
	return layer;
}

void InfluenceAreaData::set_radius(float p_radius) {
	radius = p_radius;
}

float InfluenceAreaData::get_radius() const {
	return radius;
}

void InfluenceAreaData::set_registered(bool p_registered) {
	registered = p_registered;
}

bool InfluenceAreaData::is_registered() const {
	return registered;
}

void InfluenceAreaData::set_registered_position(const Vector2 &p_position) {
	registered_position = p_position;
}

const Vector2 &InfluenceAreaData::get_registered_position() const {
	return registered_position;
}

void InfluenceAreaData::set_registered_layer(int p_layer) {
	registered_layer = p_layer;
}

int InfluenceAreaData::get_registered_layer() const {
	return registered_layer;
}

void InfluenceAreaData::set_registered_radius(float p_radius) {
	registered_radius = p_radius;
}

float InfluenceAreaData::get_registered_radius() const {
	return registered_radius;
}

void InfluenceAreaData::on_registered(const Vector2 &p_position, int p_layer, float p_radius) {
	registered = true;
	registered_position = p_position;
	registered_layer = p_layer;
	registered_radius = p_radius;
	registerable = false;
}

void InfluenceAreaData::set_registerable(bool p_registerable) {
	registerable = p_registerable;
}

bool InfluenceAreaData::is_registerable() const {
	return registerable;
}

void InfluenceAreaData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_area_node", "area_node"), &InfluenceAreaData::set_area_node);
	ClassDB::bind_method(D_METHOD("get_area_node"), &InfluenceAreaData::get_area_node);
	ClassDB::bind_method(D_METHOD("area_node_is_valid"), &InfluenceAreaData::area_node_is_valid);
	ClassDB::bind_method(D_METHOD("set_influence_type", "influence_type"), &InfluenceAreaData::set_influence_type);
	ClassDB::bind_method(D_METHOD("get_influence_type"), &InfluenceAreaData::get_influence_type);
	ClassDB::bind_method(D_METHOD("set_layer", "layer"), &InfluenceAreaData::set_layer);
	ClassDB::bind_method(D_METHOD("get_layer"), &InfluenceAreaData::get_layer);
	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &InfluenceAreaData::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &InfluenceAreaData::get_radius);
	ClassDB::bind_method(D_METHOD("is_registered"), &InfluenceAreaData::is_registered);
	ClassDB::bind_method(D_METHOD("set_registered", "registered"), &InfluenceAreaData::set_registered);
	ClassDB::bind_method(D_METHOD("set_registered_position", "position"), &InfluenceAreaData::set_registered_position);
	ClassDB::bind_method(D_METHOD("get_registered_position"), &InfluenceAreaData::get_registered_position);
	ClassDB::bind_method(D_METHOD("set_registered_layer", "layer"), &InfluenceAreaData::set_registered_layer);
	ClassDB::bind_method(D_METHOD("get_registered_layer"), &InfluenceAreaData::get_registered_layer);
	ClassDB::bind_method(D_METHOD("set_registered_radius", "radius"), &InfluenceAreaData::set_registered_radius);
	ClassDB::bind_method(D_METHOD("get_registered_radius"), &InfluenceAreaData::get_registered_radius);
	ClassDB::bind_method(D_METHOD("on_registered", "position", "layer", "radius"), &InfluenceAreaData::on_registered);
	ClassDB::bind_method(D_METHOD("set_registerable", "registerable"), &InfluenceAreaData::set_registerable);
	ClassDB::bind_method(D_METHOD("is_registerable"), &InfluenceAreaData::is_registerable);

	ClassDB::add_property("InfluenceAreaData", PropertyInfo(Variant::INT, "influence_type"), "set_influence_type", "get_influence_type");
	ClassDB::add_property("InfluenceAreaData", PropertyInfo(Variant::INT, "layer"), "set_layer", "get_layer");
	ClassDB::add_property("InfluenceAreaData", PropertyInfo(Variant::FLOAT, "radius"), "set_radius", "get_radius");
	ClassDB::add_property("InfluenceAreaData", PropertyInfo(Variant::BOOL, "registered"), "set_registered", "is_registered");
	ClassDB::add_property("InfluenceAreaData", PropertyInfo(Variant::VECTOR2, "registered_position"), "set_registered_position", "get_registered_position");
	ClassDB::add_property("InfluenceAreaData", PropertyInfo(Variant::INT, "registered_layer"), "set_registered_layer", "get_registered_layer");
	ClassDB::add_property("InfluenceAreaData", PropertyInfo(Variant::FLOAT, "registered_radius"), "set_registered_radius", "get_registered_radius");
}

InfluenceAreaData::InfluenceAreaData() {
	set_local_to_scene(true);
	influence_type = 0;
	layer = 0;
	radius = 0.0;
	registered = false;
	registered_position = Vector2(0, 0);
	registered_layer = 0;
	registered_radius = 0.0;
	registerable = true;
}


InfluenceAreaData::~InfluenceAreaData() {
}