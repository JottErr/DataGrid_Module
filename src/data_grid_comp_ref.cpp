
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

void DataGridCompRef::set_layers(const TypedArray<int32_t> &p_layers) {
	layers = p_layers;
}

const Array &DataGridCompRef::get_layers() const {
	return layers;
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

void DataGridCompRef::set_registered_layers(const TypedArray<int32_t> &p_layers) {
	registered_layers = p_layers;
}

const Array &DataGridCompRef::get_registered_layers() const {
	return registered_layers;
}

void DataGridCompRef::set_registered_radius(float p_radius) {
	registered_radius = p_radius;
}

float DataGridCompRef::get_registered_radius() const {
	return registered_radius;
}

void DataGridCompRef::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_component", "component"), &DataGridCompRef::set_component);
	ClassDB::bind_method(D_METHOD("get_component"), &DataGridCompRef::get_component);
	ClassDB::bind_method(D_METHOD("component_is_valid"), &DataGridCompRef::component_is_valid);
	ClassDB::bind_method(D_METHOD("set_layers", "layers"), &DataGridCompRef::set_layers);
	ClassDB::bind_method(D_METHOD("get_layers"), &DataGridCompRef::get_layers);
	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &DataGridCompRef::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &DataGridCompRef::get_radius);
	ClassDB::bind_method(D_METHOD("is_registered"), &DataGridCompRef::is_registered);
	ClassDB::bind_method(D_METHOD("set_registered", "registered"), &DataGridCompRef::set_registered);
	ClassDB::bind_method(D_METHOD("set_registered_position", "position"), &DataGridCompRef::set_registered_position);
	ClassDB::bind_method(D_METHOD("get_registered_position"), &DataGridCompRef::get_registered_position);
	ClassDB::bind_method(D_METHOD("set_registered_layers", "layers"), &DataGridCompRef::set_registered_layers);
	ClassDB::bind_method(D_METHOD("get_registered_layers"), &DataGridCompRef::get_registered_layers);
	ClassDB::bind_method(D_METHOD("set_registered_radius", "radius"), &DataGridCompRef::set_registered_radius);
	ClassDB::bind_method(D_METHOD("get_registered_radius"), &DataGridCompRef::get_registered_radius);

	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::ARRAY, "layers"), "set_layers", "get_layers");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::FLOAT, "radius"), "set_radius", "get_radius");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::BOOL, "registered"), "set_registered", "is_registered");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::VECTOR2, "registered_position"), "set_registered_position", "get_registered_position");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::ARRAY, "registered_layers"), "set_registered_layers", "get_registered_layers");
	ClassDB::add_property("DataGridCompRef", PropertyInfo(Variant::FLOAT, "registered_radius"), "set_registered_radius", "get_registered_radius");
}

DataGridCompRef::DataGridCompRef() {
	set_local_to_scene(true);
	radius = 0.0;
	registered = false;
	registered_position = Vector2(0, 0);
	registered_radius = 0.0;
}

DataGridCompRef::~DataGridCompRef() {
}