#include "data_grid_manager.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void DataGridManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_world_size"), &DataGridManager::get_world_size);
	ClassDB::bind_method(D_METHOD("set_world_size", "world_size"), &DataGridManager::set_world_size);
	ClassDB::bind_method(D_METHOD("get_datagrid_count"), &DataGridManager::get_datagrid_count);
	ClassDB::bind_method(D_METHOD("set_datagrid_count", "datagrid_count"), &DataGridManager::set_datagrid_count);
	ClassDB::bind_method(D_METHOD("get_cell_size"), &DataGridManager::get_cell_size);
	ClassDB::bind_method(D_METHOD("set_cell_size", "cell_size"), &DataGridManager::set_cell_size);
	ClassDB::bind_method(D_METHOD("get_datagrid_size"), &DataGridManager::get_datagrid_size);
	ClassDB::bind_method(D_METHOD("set_datagrid_size", "datagrid_size"), &DataGridManager::set_datagrid_size);
	ClassDB::bind_method(D_METHOD("get_update_frequency"), &DataGridManager::get_update_frequency);
	ClassDB::bind_method(D_METHOD("set_update_frequency", "update_frequency"), &DataGridManager::set_update_frequency);
	ClassDB::bind_method(D_METHOD("get_datagrid"), &DataGridManager::get_datagrid);

	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "world_size"), "set_world_size", "get_world_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "datagrid_count"), "set_datagrid_count", "get_datagrid_count");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "datagrid_size"), "set_datagrid_size", "get_datagrid_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::FLOAT, "update_frequency"), "set_update_frequency", "get_update_frequency");
}

DataGridManager::DataGridManager() {
	world_size = Size2i(1, 1);
	datagrid_count = Size2i(1, 1);
	cell_size = 1;
	datagrid_size = Size2i(1, 1);
	datagrid.instantiate();
	datagrid->set_cell_size(cell_size);
	datagrid->set_size_in_cells(datagrid_size);

	update_frequency = 1.0;
}

DataGridManager::~DataGridManager() {
}

void DataGridManager::set_world_size(const Size2i &p_world_size) {
	world_size = p_world_size;
	datagrid_size = world_size / (datagrid_count * cell_size);
	datagrid->set_cell_size(cell_size);
	datagrid->set_size_in_cells(datagrid_size);
}

void DataGridManager::set_cell_size(int p_cell_size) {
	cell_size = p_cell_size;
	datagrid_size = world_size / (datagrid_count * cell_size);
	datagrid->set_cell_size(cell_size);
	datagrid->set_size_in_cells(datagrid_size);
}