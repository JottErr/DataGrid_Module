#include "influence_area.h"
#include "data_grid_hub.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void InfluenceArea::set_data_resource(Ref<DataGridCompRef> p_data_resource) {
	if (p_data_resource == data_resource) {
		return;
	}
	data_resource = p_data_resource;
	if (data_resource.is_valid()) {
		data_resource->set_component(this);
	}
}

Ref<DataGridCompRef> InfluenceArea::get_data_resource() {
	return data_resource;
}

void InfluenceArea::set_layer(int p_layer) {
	data_resource->set_layer(p_layer);
}

int InfluenceArea::get_layer() const {
	return data_resource->get_layer();
}

void InfluenceArea::set_radius(float p_radius) {
	data_resource->set_radius(p_radius);
}

float InfluenceArea::get_radius() const {
	return data_resource->get_radius();
}

void InfluenceArea::set_time_since_registration(int p_time) {
	time_since_registration = p_time;
}

int InfluenceArea::get_time_since_registration() const {
	return time_since_registration;
}

void InfluenceArea::set_registration_frequency(float p_freq) {
	registration_frequency = p_freq;
}

float InfluenceArea::get_registration_frequency() const {
	return registration_frequency;
}

void InfluenceArea::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			Engine *engine = Engine::get_singleton();
			if (!engine->is_editor_hint() && engine->has_singleton("DataGridHub")) {
				DataGridHub *hub = Object::cast_to<DataGridHub>(Engine::get_singleton()->get_singleton("DataGridHub"));
				hub->add_component_data_res(data_resource);
				set_process_mode(PROCESS_MODE_INHERIT);
			}
		} break;
		case NOTIFICATION_EXIT_TREE: {
			data_resource->set_registerable(true);
			data_resource->set_component(nullptr);
		} break;
	}
}

void InfluenceArea::_process(float p_delta) {
	time_since_registration += p_delta;
	if (time_since_registration > registration_frequency) {
		time_since_registration = 0.0;
		if (get_global_position() != data_resource->get_registered_position()) {
			data_resource->set_registerable(true);
		}
	}
}

void InfluenceArea::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_data_resource", "data_resource"), &InfluenceArea::set_data_resource);
	ClassDB::bind_method(D_METHOD("get_data_resource"), &InfluenceArea::get_data_resource);
	ClassDB::bind_method(D_METHOD("set_layer", "layer"), &InfluenceArea::set_layer);
	ClassDB::bind_method(D_METHOD("get_layer"), &InfluenceArea::get_layer);
	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &InfluenceArea::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &InfluenceArea::get_radius);
	ClassDB::bind_method(D_METHOD("set_time_since_registration", "time"), &InfluenceArea::set_time_since_registration);
	ClassDB::bind_method(D_METHOD("get_time_since_registration"), &InfluenceArea::get_time_since_registration);
	ClassDB::bind_method(D_METHOD("set_registration_frequency", "frequency"), &InfluenceArea::set_registration_frequency);
	ClassDB::bind_method(D_METHOD("get_registration_frequency"), &InfluenceArea::get_registration_frequency);
	
	ClassDB::add_property("InfluenceArea", PropertyInfo(Variant::OBJECT, "data_resource", PROPERTY_HINT_RESOURCE_TYPE, "DataGridCompRef"), "set_data_resource", "get_data_resource");
	ClassDB::add_property("InfluenceArea", PropertyInfo(Variant::FLOAT, "registration_frequency"), "set_registration_frequency", "get_registration_frequency");
	ClassDB::add_property("InfluenceArea", PropertyInfo(Variant::INT, "time_since_registration"), "set_time_since_registration", "get_time_since_registration");
}

InfluenceArea::InfluenceArea() {
	set_process_mode(PROCESS_MODE_DISABLED);
	time_since_registration = 0.0;
	registration_frequency = 1.0;
}

InfluenceArea::~InfluenceArea() {
}