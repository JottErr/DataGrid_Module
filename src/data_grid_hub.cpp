#include "data_grid_hub.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

DataGridHub *DataGridHub::singleton = nullptr;

void DataGridHub::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_component", "p_datagrid_component"), &DataGridHub::add_component_data_res);
	ClassDB::bind_method(D_METHOD("get_registered_components_data_res"), &DataGridHub::get_registered_components_data_res);
	ClassDB::bind_method(D_METHOD("remove_components", "p_indices"), &DataGridHub::remove_components);
}

DataGridHub *DataGridHub::get_singleton() {
	return singleton;
}

DataGridHub::DataGridHub() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

DataGridHub::~DataGridHub() {
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

void DataGridHub::add_component_data_res(Ref<DataGridCompRef> p_datagrid_component_data_res) {
	registered_components.append(p_datagrid_component_data_res);
}

void DataGridHub::remove_components(const TypedArray<int> &p_indices) {
	for (int i = p_indices.size() - 1; i > -1; i--) {
		int index = p_indices[i];
		registered_components.remove_at(index);
		UtilityFunctions::print(UtilityFunctions::str("Removed component at index ", index));
	}
}