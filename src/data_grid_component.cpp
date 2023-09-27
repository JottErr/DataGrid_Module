#include "data_grid_component.h"
#include "data_grid_hub.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void DataGridComponent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_layers", "layers"), &DataGridComponent::set_layers);
	ClassDB::bind_method(D_METHOD("get_layers"), &DataGridComponent::get_layers);
	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &DataGridComponent::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &DataGridComponent::get_radius);
	ClassDB::bind_method(D_METHOD("is_registered"), &DataGridComponent::is_registered);
	ClassDB::bind_method(D_METHOD("set_registered", "registered"), &DataGridComponent::set_registered);
	ClassDB::bind_method(D_METHOD("set_registered_position", "position"), &DataGridComponent::set_registered_position);
	ClassDB::bind_method(D_METHOD("get_registered_position"), &DataGridComponent::get_registered_position);
	ClassDB::bind_method(D_METHOD("set_registered_layers", "layers"), &DataGridComponent::set_registered_layers);
	ClassDB::bind_method(D_METHOD("get_registered_layers"), &DataGridComponent::get_registered_layers);
	ClassDB::bind_method(D_METHOD("set_registered_radius", "radius"), &DataGridComponent::set_registered_radius);
	ClassDB::bind_method(D_METHOD("get_registered_radius"), &DataGridComponent::get_registered_radius);

	ClassDB::add_property("DataGridComponent", PropertyInfo(Variant::ARRAY, "layers"), "set_layers", "get_layers");
	ClassDB::add_property("DataGridComponent", PropertyInfo(Variant::FLOAT, "radius"), "set_radius", "get_radius");
	ClassDB::add_property("DataGridComponent", PropertyInfo(Variant::BOOL, "registered"), "set_registered", "is_registered");
	ClassDB::add_property("DataGridComponent", PropertyInfo(Variant::VECTOR2, "registered_position"), "set_registered_position", "get_registered_position");
	ClassDB::add_property("DataGridComponent", PropertyInfo(Variant::ARRAY, "registered_layers"), "set_registered_layers", "get_registered_layers");
	ClassDB::add_property("DataGridComponent", PropertyInfo(Variant::FLOAT, "registered_radius"), "set_registered_radius", "get_registered_radius");
}

DataGridComponent::DataGridComponent() {
	radius = 0.0;
	registered = false;
	registered_position = Vector2(0, 0);
	registered_radius = 0.0;
}

DataGridComponent::~DataGridComponent() {
}

void DataGridComponent::_enter_tree() {
	Engine *engine = Engine::get_singleton();
	if (!engine->is_editor_hint() && engine->has_singleton("DataGridHub")) {
		DataGridHub *hub = Object::cast_to<DataGridHub>(Engine::get_singleton()->get_singleton("DataGridHub"));
		hub->add_component(this);
	}
}
