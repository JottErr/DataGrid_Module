
#include "data_grid_comp_ref.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void DataGridCompRef::set_component(DataGridComponent *p_datagrid_component) {
	component = p_datagrid_component;
}

DataGridComponent *DataGridCompRef::get_component() const {
	return component;
}

bool DataGridCompRef::component_is_valid() const {
	return (component != nullptr && component->is_inside_tree());
}

Vector2 DataGridCompRef::get_global_position() const {
	if (!component_is_valid()) {
		return Vector2();
	}
	return component->get_global_position();
}

void DataGridCompRef::set_layer(int p_layer) {
	layer = p_layer;
}

int DataGridCompRef::get_layer() const {
	return layer;
}

void DataGridCompRef::set_radius(float p_radius) {
	radius = p_radius;
}

float DataGridCompRef::get_radius() const {
	return radius;
}

void DataGridCompRef::set_registered(bool p_registered) {
	registered = p_registered;
}

bool DataGridCompRef::is_registered() const {
	return registered;
}

void DataGridCompRef::set_registered_position(const Vector2 &p_position) {
	registered_position = p_position;
}

const Vector2 &DataGridCompRef::get_registered_position() const {
	return registered_position;
}

void DataGridCompRef::set_registered_layer(int p_layer) {
	registered_layer = p_layer;
}

int DataGridCompRef::get_registered_layer() const {
	return registered_layer;
}

void DataGridCompRef::set_registered_radius(float p_radius) {
	registered_radius = p_radius;
}

float DataGridCompRef::get_registered_radius() const {
	return registered_radius;
}

void DataGridCompRef::on_registered(const Vector2 &p_position, int p_layer, float p_radius) {
	registered = true;
	registered_position = p_position;
	registered_layer = p_layer;
	registered_radius = p_radius;
	registerable = false;
}

void DataGridCompRef::set_registerable(bool p_registerable) {
	registerable = p_registerable;
}

bool DataGridCompRef::is_registerable() const {
	return registerable;
}

void DataGridCompRef::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_component", "component"), &DataGridCompRef::set_component);
	ClassDB::bind_method(D_METHOD("get_component"), &DataGridCompRef::get_component);
	ClassDB::bind_method(D_METHOD("component_is_valid"), &DataGridCompRef::component_is_valid);
	ClassDB::bind_method(D_METHOD("set_layer", "layer"), &DataGridCompRef::set_layer);
	ClassDB::bind_method(D_METHOD("get_layer"), &DataGridCompRef::get_layer);
	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &DataGridCompRef::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &DataGridCompRef::get_radius);
	ClassDB::bind_method(D_METHOD("is_registered"), &DataGridCompRef::is_registered);
	ClassDB::bind_method(D_METHOD("set_registered", "registered"), &DataGridCompRef::set_registered);
	ClassDB::bind_method(D_METHOD("set_registered_position", "position"), &DataGridCompRef::set_registered_position);
	ClassDB::bind_method(D_METHOD("get_registered_position"), &DataGridCompRef::get_registered_position);
	ClassDB::bind_method(D_METHOD("set_registered_layer", "layer"), &DataGridCompRef::set_registered_layer);
	ClassDB::bind_method(D_METHOD("get_registered_layer"), &DataGridCompRef::get_registered_layer);
	ClassDB::bind_method(D_METHOD("set_registered_radius", "radius"), &DataGridCompRef::set_registered_radius);
	ClassDB::bind_method(D_METHOD("get_registered_radius"), &DataGridCompRef::get_registered_radius);
	ClassDB::bind_method(D_METHOD("on_registered", "position", "layer", "radius"), &DataGridCompRef::on_registered);
	ClassDB::bind_method(D_METHOD("set_registerable", "registerable"), &DataGridCompRef::set_registerable);
	ClassDB::bind_method(D_METHOD("is_registerable"), &DataGridCompRef::is_registerable);

	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::INT, "layer"), "set_layer", "get_layer");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::FLOAT, "radius"), "set_radius", "get_radius");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::BOOL, "registered"), "set_registered", "is_registered");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::VECTOR2, "registered_position"), "set_registered_position", "get_registered_position");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::INT, "registered_layer"), "set_registered_layer", "get_registered_layer");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::FLOAT, "registered_radius"), "set_registered_radius", "get_registered_radius");
}

DataGridCompRef::DataGridCompRef() {
	set_local_to_scene(true);
	radius = 0.0;
	registered = false;
	registered_position = Vector2(0, 0);
	registered_radius = 0.0;
	registerable = true;
}

DataGridCompRef::~DataGridCompRef() {
}