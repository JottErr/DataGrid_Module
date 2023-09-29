#include "data_grid_component.h"
#include "data_grid_hub.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void DataGridComponent::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			Engine *engine = Engine::get_singleton();
			if (!engine->is_editor_hint() && engine->has_singleton("DataGridHub")) {
				DataGridHub *hub = Object::cast_to<DataGridHub>(Engine::get_singleton()->get_singleton("DataGridHub"));
				hub->add_component_data_res(data_resource);
			}
		} break;
	}
}

void DataGridComponent::set_data_resource(Ref<DataGridCompRef> p_data_resource) {
	if (p_data_resource == data_resource) {
		return;
	}
	data_resource = p_data_resource;
	if (data_resource.is_valid()) {
		data_resource->set_component(this);
	}
}

Ref<DataGridCompRef> DataGridComponent::get_data_resource() {
	return data_resource;
}

void DataGridComponent::set_layers(const TypedArray<int32_t> &p_layers) {
	data_resource->set_layers(p_layers);
}

const Array &DataGridComponent::get_layers() const {
	return data_resource->get_layers();
}

void DataGridComponent::set_radius(float p_radius) {
	data_resource->set_radius(p_radius);
}

float DataGridComponent::get_radius() const {
	return data_resource->get_radius();
}

void DataGridComponent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_data_resource", "data_resource"), &DataGridComponent::set_data_resource);
	ClassDB::bind_method(D_METHOD("get_data_resource"), &DataGridComponent::get_data_resource);
	ClassDB::bind_method(D_METHOD("set_layers", "layers"), &DataGridComponent::set_layers);
	ClassDB::bind_method(D_METHOD("get_layers"), &DataGridComponent::get_layers);
	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &DataGridComponent::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &DataGridComponent::get_radius);
	
	ClassDB::add_property("DataGridComponent", PropertyInfo(Variant::OBJECT, "data_resource"), "set_data_resource", "get_data_resource");
}

DataGridComponent::DataGridComponent() {
}

DataGridComponent::~DataGridComponent() {
}