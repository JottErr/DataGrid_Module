#include "imap_hub.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

IMapHub *IMapHub::singleton = nullptr;

IMapHub *IMapHub::get_singleton() {
	return singleton;
}

void IMapHub::set_world_grid_manager(IMapManager *p_world_grid_manager) {
	world_grid_manager = p_world_grid_manager;
}

IMapManager *IMapHub::get_world_grid_manager() const {
	return world_grid_manager;
}

void IMapHub::add_influence_area_data(Ref<InfluenceAreaData> p_datagrid_component_data_res) {
	registered_components.append(p_datagrid_component_data_res);
}

void IMapHub::remove_components(const TypedArray<int> &p_indices) {
	for (int i = p_indices.size() - 1; i > -1; i--) {
		int index = p_indices[i];
		registered_components.remove_at(index);
	}
}

void IMapHub::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_world_grid_manager", "p_world_grid_manager"), &IMapHub::set_world_grid_manager);
	ClassDB::bind_method(D_METHOD("get_world_grid_manager"), &IMapHub::get_world_grid_manager);
	ClassDB::bind_method(D_METHOD("add_influence_area_data", "p_datagrid_component"), &IMapHub::add_influence_area_data);
	ClassDB::bind_method(D_METHOD("get_registered_components_data_res"), &IMapHub::get_registered_components_data_res);
	ClassDB::bind_method(D_METHOD("remove_components", "p_indices"), &IMapHub::remove_components);
}

IMapHub::IMapHub() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

IMapHub::~IMapHub() {
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}