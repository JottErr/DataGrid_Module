#include "data_grid_manager.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void DataGridTemplate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "p_radius", "p_cell_size", "p_curve"), &DataGridTemplate::initialize);
}

godot::DataGridTemplate::DataGridTemplate() {
}

godot::DataGridTemplate::~DataGridTemplate() {
}

void godot::DataGridTemplate::initialize(int p_radius, int p_cell_size, Ref<MathCurve> p_curve) {
	radius = p_radius;
	datagrid.instantiate();
	datagrid->set_cell_size(p_cell_size);
	datagrid->set_size_in_cells(Size2i(2 * radius + 1, 2 * radius + 1)); // uneven number of cells to get grid with a center cell
	datagrid->radiate_value_at_position(Point2i(radius, radius), radius, p_curve, 1.0);
}

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
	ClassDB::bind_method(D_METHOD("initialize_templates", "min_radius", "max_radius", "steps"), &DataGridManager::initialize_templates);
	ClassDB::bind_method(D_METHOD("get_template"), &DataGridManager::get_template);

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

void DataGridManager::initialize_templates(int min_radius, int max_radius, int steps) {
	Ref<MathCurve> tcurve;
	tcurve.instantiate();

	templates.resize(max_radius - min_radius);
	for (int radius = min_radius; radius < max_radius; radius += steps) {
		Ref<DataGridTemplate> datagrid_template;
		datagrid_template.instantiate();
		datagrid_template->initialize(radius, cell_size, tcurve);
		templates[radius - min_radius] = datagrid_template;
	}
}

Ref<DataGrid> DataGridManager::get_template(int p_radius) const {
	for (int i = 0; i < templates.size(); i++) {
		Ref<DataGridTemplate> t = templates[i];
		if (p_radius <= t->radius) {
			return t->datagrid;
		}
	}
	Ref<DataGridTemplate> t = templates[templates.size() - 1];
	return t->datagrid;
}