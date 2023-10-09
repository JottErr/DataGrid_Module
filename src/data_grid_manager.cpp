#include "data_grid_manager.h"
#include "data_grid_hub.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

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
}

void DataGridManager::initialize_templates(int min_radius, int max_radius, int steps) {
	// curve as parameter, new int type parameter, ENUM.types (PROX; THREAT; INTEREST; FLAT)
	// Array templates becomes Dict templates <type, Array>
	Ref<MathCurve> tcurve;
	tcurve.instantiate();

	templates.resize(max_radius - min_radius);
	for (int radius = min_radius; radius < max_radius; radius += steps) {
		Ref<InfluenceMap> imap;
		imap.instantiate();
		imap->set_cell_size(cell_size);
		imap->set_size_in_cells(Size2i(2 * radius + 1, 2 * radius + 1)); // uneven number of cells to get grid with a center cell
		imap->radiate_value_at_position(Point2i(radius, radius), radius, tcurve, 1.0);
		templates[radius - min_radius] = InfluenceMapTemplate(radius, imap);
	}
}

Ref<InfluenceMap> DataGridManager::get_template(int p_radius) const {
	for (int i = 0; i < templates.size(); i++) {
		const InfluenceMapTemplate &t = templates[i];
		if (p_radius <= t.radius) {
			return t.datagrid;
		}
	}
	const InfluenceMapTemplate &t = templates[templates.size() - 1];
	return t.datagrid;
}

void DataGridManager::_notification(int p_what) {
	switch (p_what) {
		case  NOTIFICATION_ENTER_TREE: {
			if (!Engine::get_singleton()->is_editor_hint()) {
				set_process_mode(PROCESS_MODE_INHERIT);
			}
		} break;
	}
}

void DataGridManager::_process(float p_delta) {
	Engine *engine = Engine::get_singleton();
	if (engine->is_editor_hint()) {
		return;
	}
	DataGridHub *hub = Object::cast_to<DataGridHub>(Engine::get_singleton()->get_singleton("DataGridHub"));
	TypedArray<DataGridCompRef> nodes = hub->get_registered_components_data_res();
	TypedArray<int> freed_components;
	//TypedArray<Vector3> deregistered_grids; add maps on deregister, remove on register, leftover is only deregister this frame, ask if all 0 to remove
	//missing refs to grids or their relative index offsets from touched_grids
	int amount_nodes = nodes.size();
	int updates_per_frame = 1; // move to .h, create set, get, export
	int updates_so_far = 0;
	for (int i = 0; i < amount_nodes; i++) {
		Ref<DataGridCompRef> component_data = nodes[i];
		if (!component_data->is_registerable()) {
			continue;
		}
		updates_so_far++;
		// Deregister
		if (component_data->is_registered()) {
			Vector2 global_position = component_data->get_registered_position();
			Ref<InfluenceMap> template_grid = get_template(component_data->get_registered_radius() / cell_size);
			add_datagrid_centered_to_collection(template_grid, component_data->get_registered_layer(), global_position, -1.0, false);
			component_data->set_registered(false);
		}
		// Component still valid?
		DataGridComponent *component = component_data->get_component();
		if (component == nullptr || !(component->is_inside_tree())) {
			freed_components.append(i);
			if (updates_so_far < updates_per_frame) {
				continue;
			} 
			break;
		}
		// Register
		Vector2 global_position = component_data->get_global_position();
		Ref<InfluenceMap> template_grid = get_template(component_data->get_radius() / cell_size);
		add_datagrid_centered_to_collection(template_grid, component_data->get_layer(), global_position, 1.0);
		component_data->on_registered(global_position, component_data->get_layer(), component_data->get_radius());
		if (updates_so_far < updates_per_frame) {
			continue;
		} 
		break;
	}
	hub->remove_components(freed_components);
	emit_updated(datagrid_collection);
}


void DataGridManager::emit_updated(const Dictionary &p_datagrid_collection) {
	emit_signal("updated", p_datagrid_collection);
}

void DataGridManager::add_datagrid_layer_to_collection(const Point2i &p_datagrid_position, int p_layer, const Ref<InfluenceMap> &p_datagrid) {
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

Ref<InfluenceMap> DataGridManager::get_datagrid_layer(const Point2i &p_datagrid_position, int p_layer) const {
	if (!has_datagrid_layer(p_datagrid_position, p_layer)) {
		return nullptr;
	}
	Dictionary layer_stack = datagrid_collection[p_datagrid_position];
	Ref<InfluenceMap> layer = layer_stack[p_layer];
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

Vector2i DataGridManager::global_position_to_datagrid_index(const Vector2i &p_global_position) const {
	// if InfluenceMap index is never negative, this can be simplified
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

void DataGridManager::add_datagrid_centered_to_collection(const Ref<InfluenceMap> &grid_to_add, int p_layer, const Point2 &p_global_position, float p_magnitude, bool registering) {
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
			Ref<InfluenceMap> new_datagrid;
			new_datagrid.instantiate();
			new_datagrid->set_cell_size(cell_size);
			new_datagrid->set_size_in_cells(Vector2i(datagrid_size));
			add_datagrid_layer_to_collection(this_grid_index, p_layer, new_datagrid);
		}
		Ref<InfluenceMap> datagrid = get_datagrid_layer(this_grid_index, p_layer);
		Vector2i offset = (-1 * index_offset) * datagrid_size;
		datagrid->add_grid_centered_at_pos(grid_to_add, grid_cell_index, p_magnitude, offset);
	}	
}

void DataGridManager::add_into_datagrid_from_collection(const Ref<InfluenceMap> &grid_to_add_into, int p_layer, const Point2 &p_global_position, float p_magnitude) {
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
		Ref<InfluenceMap> datagrid = get_datagrid_layer(this_grid_index, p_layer);
		Vector2i offset = (-1 * index_offset) * datagrid_size;
		grid_to_add_into->add_from_pos_in_grid(datagrid, grid_cell_index, p_magnitude, offset);
	}
}

Vector2 DataGridManager::snap_global_postion_to_cell_center(const Vector2 &p_global_position) const {
	return Vector2(p_global_position / cell_size).floor() * cell_size + Vector2(cell_size, cell_size) / 2.0;
}

Vector2 DataGridManager::find_corner_from_center(const Vector2 &p_global_position, Vector2i p_datagrid_center) {
	Vector2 center_world_index = Vector2(p_global_position / cell_size).floor();
	Vector2 corner_pos = (center_world_index - p_datagrid_center) * cell_size;
	return corner_pos;
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

	ClassDB::bind_method(D_METHOD("initialize_templates", "min_radius", "max_radius", "steps"), &DataGridManager::initialize_templates);
	ClassDB::bind_method(D_METHOD("get_template"), &DataGridManager::get_template);
	
	ClassDB::bind_method(D_METHOD("emit_updated", "datagrid_collection"), &DataGridManager::emit_updated);
	
	ClassDB::bind_method(D_METHOD("add_datagrid_layer_to_collection", "datagrid_position", "layer", "datagrid"), &DataGridManager::add_datagrid_layer_to_collection);
	ClassDB::bind_method(D_METHOD("has_datagrid_layer", "datagrid_position", "layer"), &DataGridManager::has_datagrid_layer);
	ClassDB::bind_method(D_METHOD("get_datagrid_layer", "datagrid_position", "layer"), &DataGridManager::get_datagrid_layer);
	ClassDB::bind_method(D_METHOD("filter_datagrid_layers", "datagrid_position", "filter_layers"), &DataGridManager::filter_datagrid_layers);

	ClassDB::bind_method(D_METHOD("global_position_to_datagrid_index", "global_position"), &DataGridManager::global_position_to_datagrid_index);
	ClassDB::bind_method(D_METHOD("grid_position_in_bounds", "data_grid_position"), &DataGridManager::grid_position_in_bounds);
	ClassDB::bind_method(D_METHOD("world_position_to_cell_in_data_grid", "world_position", "data_grid_position"), &DataGridManager::world_position_to_cell_in_data_grid);
	ClassDB::bind_method(D_METHOD("get_touched_datagrids", "center_cell", "radius"), &DataGridManager::get_touched_datagrids); 
	ClassDB::bind_method(D_METHOD("add_datagrid_centered_to_collection", "grid_to_add", "layer", "global_position", "magnitude", "registering"), &DataGridManager::add_datagrid_centered_to_collection, DEFVAL(1.0f), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("add_into_datagrid_from_collection", "grid_to_add_into", "layer", "global_position", "magnitude"), &DataGridManager::add_into_datagrid_from_collection, DEFVAL(1.0f));
	
	ClassDB::bind_method(D_METHOD("snap_global_postion_to_cell_center", "p_global_position"), &DataGridManager::snap_global_postion_to_cell_center);
	ClassDB::bind_method(D_METHOD("find_corner_from_center", "p_global_position", "p_datagrid_center"), &DataGridManager::find_corner_from_center);

	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "world_size"), "set_world_size", "get_world_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "datagrid_count"), "set_datagrid_count", "get_datagrid_count");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "datagrid_size"), "set_datagrid_size", "get_datagrid_size");

	ADD_SIGNAL(MethodInfo("updated", PropertyInfo(Variant::DICTIONARY, "datagrid_collection")));
}

DataGridManager::DataGridManager() {
	set_process_mode(PROCESS_MODE_DISABLED);
	world_size = Size2i(1, 1);
	datagrid_count = Size2i(1, 1);
	cell_size = 1;
	datagrid_size = Size2i(1, 1);
}

DataGridManager::~DataGridManager() {
}