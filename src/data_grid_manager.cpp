#include "data_grid_manager.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

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
	ClassDB::bind_method(D_METHOD("set_world_size", "world_size"), &DataGridManager::set_world_size);
	ClassDB::bind_method(D_METHOD("get_world_size"), &DataGridManager::get_world_size);
	ClassDB::bind_method(D_METHOD("set_datagrid_count", "datagrid_count"), &DataGridManager::set_datagrid_count);
	ClassDB::bind_method(D_METHOD("get_datagrid_count"), &DataGridManager::get_datagrid_count);
	ClassDB::bind_method(D_METHOD("set_cell_size", "cell_size"), &DataGridManager::set_cell_size);
	ClassDB::bind_method(D_METHOD("get_cell_size"), &DataGridManager::get_cell_size);
	ClassDB::bind_method(D_METHOD("set_datagrid_size", "datagrid_size"), &DataGridManager::set_datagrid_size, DEFVAL(Size2i(0, 0))); 
	ClassDB::bind_method(D_METHOD("get_datagrid_size"), &DataGridManager::get_datagrid_size);
	ClassDB::bind_method(D_METHOD("emit_datagrid_size_changed"), &DataGridManager::emit_datagrid_size_changed);
	ClassDB::bind_method(D_METHOD("set_update_frequency", "update_frequency"), &DataGridManager::set_update_frequency);
	ClassDB::bind_method(D_METHOD("get_update_frequency"), &DataGridManager::get_update_frequency);
	ClassDB::bind_method(D_METHOD("initialize_templates", "min_radius", "max_radius", "steps"), &DataGridManager::initialize_templates);
	ClassDB::bind_method(D_METHOD("get_template"), &DataGridManager::get_template);
	ClassDB::bind_method(D_METHOD("update"), &DataGridManager::update);
	ClassDB::bind_method(D_METHOD("emit_updated", "datagrid_collection"), &DataGridManager::emit_updated);
	ClassDB::bind_method(D_METHOD("world_position_to_grid_position", "world_position"), &DataGridManager::world_position_to_grid_position);
	ClassDB::bind_method(D_METHOD("grid_position_in_bounds", "data_grid_position"), &DataGridManager::grid_position_in_bounds);
	ClassDB::bind_method(D_METHOD("world_position_to_cell_in_data_grid", "world_position", "data_grid_position"), &DataGridManager::world_position_to_cell_in_data_grid);

	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "world_size"), "set_world_size", "get_world_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "datagrid_count"), "set_datagrid_count", "get_datagrid_count");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::VECTOR2I, "datagrid_size"), "set_datagrid_size", "get_datagrid_size");
	ClassDB::add_property("DataGridManager", PropertyInfo(Variant::FLOAT, "update_frequency"), "set_update_frequency", "get_update_frequency");

	ADD_SIGNAL(MethodInfo("updated", PropertyInfo(Variant::OBJECT, "datagrid_collection")));
	ADD_SIGNAL(MethodInfo("datagrid_size_changed", PropertyInfo(Variant::VECTOR2I, "datagrid_size")));

}

DataGridManager::DataGridManager() {
	world_size = Size2i(1, 1);
	datagrid_count = Size2i(1, 1);
	cell_size = 1;
	datagrid_size = Size2i(1, 1);
	datagrid_collection.instantiate();
	update_frequency = 1.0;
}

DataGridManager::~DataGridManager() {
}

void DataGridManager::set_world_size(const Size2i &p_world_size) {
	world_size = p_world_size;
	set_datagrid_size();
	datagrid_collection->clear();
}

void DataGridManager::set_cell_size(int p_cell_size) {
	cell_size = p_cell_size;
	set_datagrid_size();
	datagrid_collection->clear();
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

Vector2i DataGridManager::world_position_to_grid_position(const Vector2i &p_world_position) const {
	// if manager position is not 0,0 get_relative_position = p_world_position - manager_position
	int neg_x = p_world_position.x < 0 ? 1 : 0;
	int neg_y = p_world_position.y < 0 ? 1 : 0;
	Vector2i result;
	result.x = (p_world_position.x + neg_x) / (datagrid_size.x * cell_size) - neg_x;
	result.y = (p_world_position.y + neg_y) / (datagrid_size.y * cell_size) - neg_y;
	return result;
}

bool DataGridManager::grid_position_in_bounds(const Vector2i &p_data_grid_position) const {
	return p_data_grid_position >= Vector2i(0, 0) && p_data_grid_position <= datagrid_count - Vector2i(1,  1);
}

Vector2i DataGridManager::world_position_to_cell_in_data_grid(const Vector2 &p_world_position, const Vector2i &p_data_grid_position) const {
	return Vector2i((p_world_position - p_data_grid_position * datagrid_size * cell_size) / cell_size);
}


void DataGridManager::update() {
	SceneTree *tree = get_tree();
	Array nodes = tree->get_nodes_in_group("DataComponents");
	for (int i = 0; i < nodes.size(); i++) {
		Variant node = nodes[i];
		DataGridComponent *component = Object::cast_to<DataGridComponent>(node);
		// De-Register
		if (component->is_registered()) {
			Vector2 world_position = component->get_registered_position();
			Vector2i data_grid_position = world_position_to_grid_position(world_position);
			Vector2i cell_in_data_grid = world_position_to_cell_in_data_grid(world_position, data_grid_position);
			int radius = component->get_registered_radius() / cell_size;
			int amount_layers = component->get_registered_layers().size();
			for (int i = 0; i < amount_layers; i++) {
				int layer = component->get_layers()[i];
				if (datagrid_collection->has_layer_at_pos(data_grid_position, layer)) {
					Ref<DataGrid> datagrid = datagrid_collection->get_layer_at_pos(data_grid_position, layer);
					datagrid->add_grid_at_pos(get_template(radius), cell_in_data_grid, -1.0);
				}
			}
		}
		// Register
		Vector2 world_position = component->get_global_position();
		Vector2i data_grid_position = world_position_to_grid_position(world_position);
		if  (!grid_position_in_bounds(data_grid_position)) {
			component->set_registered(false);
			continue;
		}
		Vector2i cell_in_data_grid = world_position_to_cell_in_data_grid(world_position, data_grid_position);
		int radius = component->get_radius() / cell_size;
		int amount_layers = component->get_layers().size();
		for (int i = 0; i < amount_layers; i++) {
			int layer = component->get_layers()[i];
			if (!datagrid_collection->has_layer_at_pos(data_grid_position, layer)) {
				Ref<DataGrid> datagrid;
				datagrid.instantiate();
				datagrid->set_cell_size(cell_size);
				datagrid->set_size_in_cells(Vector2i(datagrid_size));
				datagrid_collection->add_datagrid_to_collection(data_grid_position, layer, datagrid);
			}
			Ref<DataGrid> datagrid = datagrid_collection->get_layer_at_pos(data_grid_position, layer);
			datagrid->add_grid_at_pos(get_template(radius), cell_in_data_grid, 1.0);
		}
		component->set_registered(true);
		component->set_registered_position(world_position);
		component->set_registered_layers(component->get_layers());
		component->set_registered_radius(component->get_radius());
	}
	emit_updated(datagrid_collection);
}

void DataGridManager::emit_updated(const Ref<DataGridCollection> &p_datagrid_collection) {
	emit_signal("updated", p_datagrid_collection);
}
