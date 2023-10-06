#include "data_grid_manager.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include "data_grid_hub.h"

using namespace godot;

void DataGridTemplate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "p_radius", "p_cell_size", "p_curve"), &DataGridTemplate::initialize);
}

DataGridTemplate::DataGridTemplate() {
}

DataGridTemplate::~DataGridTemplate() {
}

void DataGridTemplate::initialize(int p_radius, int p_cell_size, Ref<MathCurve> p_curve) {
	radius = p_radius;
	datagrid.instantiate();
	datagrid->set_cell_size(p_cell_size);
	datagrid->set_size_in_cells(Size2i(2 * radius + 1, 2 * radius + 1)); // uneven number of cells to get grid with a center cell
	datagrid->radiate_value_at_position(Point2i(radius, radius), radius, p_curve, 1.0);
}

void DataGridManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_world_size", "world_size"), &DataGridManager::set_world_size);
	ClassDB::bind_method(D_METHOD("get_world_size"), &DataGridManager::get_world_size);
	ClassDB::bind_method(D_METHOD("set_datagrid_count", "datagrid_count"), &DataGridManager::set_datagrid_count);
	ClassDB::bind_method(D_METHOD("get_datagrid_count"), &DataGridManager::get_datagrid_count);
	ClassDB::bind_method(D_METHOD("set_cell_size", "cell_size"), &DataGridManager::set_cell_size);
	ClassDB::bind_method(D_METHOD("get_cell_size"), &DataGridManager::get_cell_size);
	ClassDB::bind_method(D_METHOD("set_datagrid_size", "datagrid_size"), &DataGridManager::set_datagrid_size, DEFVAL(Size2i(0, 0))); 
	ClassDB::bind_method(D_METHOD("get_datagrid_size"), &DataGridManager::get_datagrid_size);
	ClassDB::bind_method(D_METHOD("emit_datagrid_size_changed"), &DataGridManager::emit_datagrid_size_changed); //deprecated, was to inform shader
	ClassDB::bind_method(D_METHOD("set_update_frequency", "update_frequency"), &DataGridManager::set_update_frequency); //deprecated, components keep time
	ClassDB::bind_method(D_METHOD("get_update_frequency"), &DataGridManager::get_update_frequency); //deprecated, components keep time

	ClassDB::bind_method(D_METHOD("initialize_templates", "min_radius", "max_radius", "steps"), &DataGridManager::initialize_templates);
	ClassDB::bind_method(D_METHOD("get_template"), &DataGridManager::get_template);
	ClassDB::bind_method(D_METHOD("update"), &DataGridManager::update);
	ClassDB::bind_method(D_METHOD("emit_updated", "datagrid_collection"), &DataGridManager::emit_updated);
	
	ClassDB::bind_method(D_METHOD("snap_global_postion_to_cell_center", "p_global_position"), &DataGridManager::snap_global_postion_to_cell_center);
	ClassDB::bind_method(D_METHOD("find_corner_from_center", "p_global_position", "p_datagrid_center"), &DataGridManager::find_corner_from_center);

	ClassDB::bind_method(D_METHOD("global_position_to_datagrid_index", "global_position"), &DataGridManager::global_position_to_datagrid_index);
	ClassDB::bind_method(D_METHOD("grid_position_in_bounds", "data_grid_position"), &DataGridManager::grid_position_in_bounds);
	ClassDB::bind_method(D_METHOD("world_position_to_cell_in_data_grid", "world_position", "data_grid_position"), &DataGridManager::world_position_to_cell_in_data_grid);
	ClassDB::bind_method(D_METHOD("get_touched_datagrids", "center_cell", "radius"), &DataGridManager::get_touched_datagrids); 
	ClassDB::bind_method(D_METHOD("add_datagrid_centered_to_collection", "grid_to_add", "layer", "global_position", "magnitude", "registering"), &DataGridManager::add_datagrid_centered_to_collection, DEFVAL(1.0f), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("add_into_datagrid_from_collection", "grid_to_add_into", "layer", "global_position", "magnitude"), &DataGridManager::add_into_datagrid_from_collection, DEFVAL(1.0f));
	
	ClassDB::bind_method(D_METHOD("add_datagrid_layer_to_collection", "datagrid_position", "layer", "datagrid"), &DataGridManager::add_datagrid_layer_to_collection);
	ClassDB::bind_method(D_METHOD("has_datagrid_layer", "datagrid_position", "layer"), &DataGridManager::has_datagrid_layer);
	ClassDB::bind_method(D_METHOD("get_datagrid_layer", "datagrid_position", "layer"), &DataGridManager::get_datagrid_layer);
	ClassDB::bind_method(D_METHOD("filter_datagrid_layers", "datagrid_position", "filter_layers"), &DataGridManager::filter_datagrid_layers);

	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "world_size"), "set_world_size", "get_world_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "datagrid_count"), "set_datagrid_count", "get_datagrid_count");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "datagrid_size"), "set_datagrid_size", "get_datagrid_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::FLOAT, "update_frequency"), "set_update_frequency", "get_update_frequency");

	ADD_SIGNAL(MethodInfo("updated", PropertyInfo(Variant::DICTIONARY, "datagrid_collection")));
	ADD_SIGNAL(MethodInfo("datagrid_size_changed", PropertyInfo(Variant::VECTOR2I, "datagrid_size")));
}

DataGridManager::DataGridManager() {
	world_size = Size2i(1, 1);
	datagrid_count = Size2i(1, 1);
	cell_size = 1;
	datagrid_size = Size2i(1, 1);
	update_frequency = 1.0;
}

DataGridManager::~DataGridManager() {
}

void DataGridManager::set_world_size(const Size2i &p_world_size) {
	world_size = p_world_size;
	set_datagrid_size();
	datagrid_collection.clear();
}

void DataGridManager::set_datagrid_count(const Size2i &p_datagrid_count) {
	datagrid_count = p_datagrid_count;
	set_datagrid_size();
	datagrid_collection.clear();
}

void DataGridManager::set_cell_size(int p_cell_size) {
	cell_size = p_cell_size;
	set_datagrid_size();
	datagrid_collection.clear();
}

void DataGridManager::set_datagrid_size(const Size2i &p_datagrid_size) {
	datagrid_size = world_size / (datagrid_count * cell_size);
	emit_datagrid_size_changed();
}

void DataGridManager::emit_datagrid_size_changed() {
	emit_signal("datagrid_size_changed", datagrid_size);
}

void DataGridManager::initialize_templates(int min_radius, int max_radius, int steps) {
	Ref<MathCurve> tcurve;
	tcurve.instantiate();

	int limit = max_radius + 1; //including given max_radius feels more intuitive
	templates.resize(limit - min_radius);
	for (int radius = min_radius; radius < limit; radius += steps) {
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

Vector2i DataGridManager::global_position_to_datagrid_index(const Vector2i &p_global_position) const {
	// if DataGrid index is never negative, this can be simplified
	// if manager global position is not 0,0 get_relative_position = p_global_position - manager_position
	int neg_x = p_global_position.x < 0 ? 1 : 0;
	int neg_y = p_global_position.y < 0 ? 1 : 0;
	Vector2i result;
	result.x = (p_global_position.x + neg_x) / (datagrid_size.x * cell_size) - neg_x;
	result.y = (p_global_position.y + neg_y) / (datagrid_size.y * cell_size) - neg_y;
	return result;
}

bool DataGridManager::grid_position_in_bounds(const Vector2i &p_data_grid_position) const {
	bool positive = p_data_grid_position.x >= 0 && p_data_grid_position.y >= 0;
	return positive && (p_data_grid_position.x < datagrid_count.x) && (p_data_grid_position.y < datagrid_count.y);
}

Vector2i DataGridManager::world_position_to_cell_in_data_grid(const Vector2 &p_world_position, const Vector2i &p_data_grid_position) const {
	return Vector2i((p_world_position - p_data_grid_position * datagrid_size * cell_size) / cell_size);
}

Array DataGridManager::get_touched_datagrids(const Vector2i &p_center_cell, int p_radius) const {
	//Only works for direct neighbours, if (radius > datagrid_size+2) a second neighbour could be touched 
	Array result;
	result.append(Vector2i(0, 0));

	bool north = p_center_cell.y - p_radius < 0;
	bool south = p_center_cell.y + p_radius > datagrid_size.y - 1;
	bool west = p_center_cell.x - p_radius < 0;
	bool east = p_center_cell.x + p_radius > datagrid_size.x - 1;

	if (north) { result.append(Vector2i(0, -1)); }
	if (south) { result.append(Vector2i(0, 1)); }
	if (west) { result.append(Vector2i(-1, 0)); }
	if (east) { result.append(Vector2i(1, 0)); }

	if (north && west) { result.append(Vector2i(-1, -1)); }
	if (north && east) { result.append(Vector2i(1, -1)); }
	if (south && west) { result.append(Vector2i(-1, 1)); }
	if (south && east) { result.append(Vector2i(1, 1)); }

	return result;
}

void DataGridManager::add_datagrid_centered_to_collection(const Ref<DataGrid> &grid_to_add, int p_layer, const Point2 &p_global_position, float p_magnitude, bool registering) {
	Vector2i datagrid_index = global_position_to_datagrid_index(p_global_position);
	Vector2i grid_cell_index = world_position_to_cell_in_data_grid(p_global_position, datagrid_index);
	int radius = grid_to_add->get_center().x;
	Array touched_grids = get_touched_datagrids(grid_cell_index, radius);
	int amount_grids = touched_grids.size();
	for (int i = 0; i < amount_grids; i++) {
		Vector2i index_offset = touched_grids[i];
		Vector2i this_grid_index = datagrid_index + index_offset;
		if (!grid_position_in_bounds(this_grid_index)) {
			continue;
		}
		if (!has_datagrid_layer(this_grid_index, p_layer)) {
			if (!registering) {
				continue;
			}
			Ref<DataGrid> new_datagrid;
			new_datagrid.instantiate();
			new_datagrid->set_cell_size(cell_size);
			new_datagrid->set_size_in_cells(Vector2i(datagrid_size));
			add_datagrid_layer_to_collection(this_grid_index, p_layer, new_datagrid);
		}
		Ref<DataGrid> datagrid = get_datagrid_layer(this_grid_index, p_layer);
		Vector2i offset = (-1 * index_offset) * datagrid_size;
		datagrid->add_grid_centered_at_pos(grid_to_add, grid_cell_index, p_magnitude, offset);
	}	
}

void DataGridManager::add_into_datagrid_from_collection(const Ref<DataGrid> &grid_to_add_into, int p_layer, const Point2 &p_global_position, float p_magnitude) {
	Vector2i datagrid_index = global_position_to_datagrid_index(p_global_position);
	Vector2i grid_cell_index = world_position_to_cell_in_data_grid(p_global_position, datagrid_index);
	int radius = grid_to_add_into->get_center().x;
	Array touched_grids = get_touched_datagrids(grid_cell_index, radius);
	int amount_grids = touched_grids.size();
	
	for (int i = 0; i < amount_grids; i++) {
		Vector2i index_offset = touched_grids[i];
		Vector2i this_grid_index = datagrid_index + index_offset;
		if (!grid_position_in_bounds(this_grid_index)) {
			continue;
		}
		if (!has_datagrid_layer(this_grid_index, p_layer)) {
			continue;
		}
		Ref<DataGrid> datagrid = get_datagrid_layer(this_grid_index, p_layer);
		Vector2i offset = (-1 * index_offset) * datagrid_size;
		grid_to_add_into->add_from_pos_in_grid(datagrid, grid_cell_index, p_magnitude, offset);
	}
}

void DataGridManager::add_datagrid_layer_to_collection(const Point2i &p_datagrid_position, int p_layer, const Ref<DataGrid> &p_datagrid) {
	if (!datagrid_collection.has(p_datagrid_position)) {
		Dictionary layer_stack;
		layer_stack[p_layer] = p_datagrid;
		datagrid_collection[p_datagrid_position] = layer_stack;
		return;
	}
	Dictionary layer_stack = datagrid_collection[p_datagrid_position];
	if (!layer_stack.has(p_layer)) {
		layer_stack[p_layer] = p_datagrid;
	}
}

bool DataGridManager::has_datagrid_layer(const Point2i &p_datagrid_position, int p_layer) const {
	if (!datagrid_collection.has(p_datagrid_position)) {
		return false;
	}
	Dictionary layer_stack = datagrid_collection[p_datagrid_position];
	return layer_stack.has(p_layer);
}

Ref<DataGrid> DataGridManager::get_datagrid_layer(const Point2i &p_datagrid_position, int p_layer) const {
	if (!has_datagrid_layer(p_datagrid_position, p_layer)) {
		return nullptr;
	}
	Dictionary layer_stack = datagrid_collection[p_datagrid_position];
	Ref<DataGrid> layer = layer_stack[p_layer];
	return layer;
}

Dictionary DataGridManager::filter_datagrid_layers(const Point2i &p_datagrid_position, const Array &filter_layers) const {
	Dictionary result;
	if (datagrid_collection.has(p_datagrid_position)) {
		Dictionary all_layers = datagrid_collection[p_datagrid_position];
		if (filter_layers.is_empty()) {
			return all_layers;
		}
		for (int i = 0; i < filter_layers.size(); i++) {
			int layer = filter_layers[i];
			if (all_layers.has(layer)) {
				result[layer] = all_layers[layer];
			}
		}
	}
	return result;
}

void DataGridManager::update() {
	Engine *engine = Engine::get_singleton();
	if (engine->is_editor_hint() || !(engine->has_singleton("DataGridHub"))) {
		return;
	}
	DataGridHub *hub = Object::cast_to<DataGridHub>(Engine::get_singleton()->get_singleton("DataGridHub"));
	TypedArray<DataGridCompRef> nodes = hub->get_registered_components_data_res();
	TypedArray<int> removed_nodes;
	for (int i = 0; i < nodes.size(); i++) {
		Ref<DataGridCompRef> component_data = nodes[i];
		// Deregister
		if (component_data->is_registered()) {
			Vector2 global_position = component_data->get_registered_position();
			Ref<DataGrid> template_grid = get_template(component_data->get_registered_radius() / cell_size);
			add_datagrid_centered_to_collection(template_grid, component_data->get_registered_layer(), global_position, -1.0, false);
			component_data->set_registered(false);
		}
		// Component still valid?
		DataGridComponent *comp_ptr = component_data->get_component();
		if (comp_ptr == nullptr || !(comp_ptr->is_inside_tree())) {
			removed_nodes.append(i);
			continue;
		}
		// Register
		Vector2 global_position = component_data->get_global_position();
		Ref<DataGrid> template_grid = get_template(component_data->get_radius() / cell_size);
		add_datagrid_centered_to_collection(template_grid, component_data->get_layer(), global_position, 1.0);
		component_data->on_registered(global_position, component_data->get_layer(), component_data->get_radius());
	}
	hub->remove_components(removed_nodes);
	emit_updated(datagrid_collection);
}

void DataGridManager::emit_updated(const Dictionary &p_datagrid_collection) {
	emit_signal("updated", p_datagrid_collection);
}

Vector2 DataGridManager::snap_global_postion_to_cell_center(const Vector2 &p_global_position) const {
	return Vector2(p_global_position / cell_size).floor() * cell_size + Vector2(cell_size, cell_size) / 2.0;
}

Vector2 DataGridManager::find_corner_from_center(const Vector2 &p_global_position, Vector2i p_datagrid_center) {
	Vector2 center_world_index = Vector2(p_global_position / cell_size).floor();
	Vector2 corner_pos = (center_world_index - p_datagrid_center) * cell_size;
	return corner_pos;
}
