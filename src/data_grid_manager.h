#ifndef DATAGRIDMANAGER_H
#define DATAGRIDMANAGER_H

#include <godot_cpp/classes/node.hpp>
#include "data_grid.h"
#include "data_grid_component.h"

namespace godot {

class DataGridTemplate : public Resource {
	GDCLASS(DataGridTemplate, Resource)

protected:
	static void _bind_methods();

public:
	DataGridTemplate();
	~DataGridTemplate();
	int radius;
	Ref<DataGrid> datagrid;
	void initialize(int p_radius, int p_cell_size, Ref<MathCurve> p_curve);
};

class DataGridManager : public Node {
	GDCLASS(DataGridManager, Node)

private:
	Size2i world_size; // Size of the covered area in world units
	Size2i datagrid_count; // Number of DataGrids across the covered area
	int cell_size; // Size of a cell in world units
	Size2i datagrid_size; // Size of each DataGrid in cells
	
	float update_frequency;

	Dictionary datagrid_collection;
	TypedArray<DataGridTemplate> templates;

protected:
	static void _bind_methods();

public:
	DataGridManager();
	~DataGridManager();
	
	void set_world_size(const Size2i &p_world_size);
	const Size2i &get_world_size() const { return world_size; }
	void set_datagrid_count(const Size2i &p_datagrid_count);
	const Size2i &get_datagrid_count() const { return datagrid_count; }
	void set_cell_size(int p_cell_size);
	int get_cell_size() const { return cell_size; }
	void set_datagrid_size(const Size2i &p_datagrid_size = Size2i(0, 0));
	const Size2i &get_datagrid_size() const { return datagrid_size; }
	void emit_datagrid_size_changed();
	void set_update_frequency(float p_update_frequency) { update_frequency = p_update_frequency; }
	float get_update_frequency() const { return update_frequency; }
	
	void initialize_templates(int min_radius, int max_radius, int steps);
	Ref<DataGrid> get_template(int p_radius) const;
	void update();
	void emit_updated(const Dictionary &datagrid_collection);

	Vector2i world_position_to_grid_position(const Vector2i &p_world_position) const;
	bool grid_position_in_bounds(const Vector2i &p_data_grid_position) const;
	Vector2i world_position_to_cell_in_data_grid(const Vector2 &p_world_position, const Vector2i &p_data_grid_position) const;
	Array get_touched_datagrids(const Vector2i &p_center_cell, int p_radius) const;
	void add_datagrid_centered_to_collection(const Ref<DataGrid> &grid_to_add, int p_layer, const Point2 &p_global_position, float p_magnitude = 1.0f, bool add_new = true);
	void add_into_datagrid_from_collection(const Ref<DataGrid> &grid_to_add_into, int p_layer, const Point2 &p_global_position, float p_magnitude = 1.0f);
	
	void add_datagrid_layer_to_collection(const Point2i &p_datagrid_position, int p_layer, const Ref<DataGrid> &p_datagrid);
	bool has_datagrid_layer(const Point2i &p_datagrid_position, int p_layer) const;
	Ref<DataGrid> get_datagrid_layer(const Point2i &p_datagrid_position, int p_layer) const;
	Dictionary filter_datagrid_layers(const Point2i &p_datagrid_position, const Array &filter_layers) const;
};
}

#endif // DATAGRIDMANAGER_H