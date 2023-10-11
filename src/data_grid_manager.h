#ifndef DATAGRIDMANAGER_H
#define DATAGRIDMANAGER_H

#include <godot_cpp/classes/node.hpp>
#include "math_curve.h"
#include "influence_map.h"
#include "data_grid_component.h"
#include "godot_cpp/templates/local_vector.hpp"
#include "godot_cpp/templates/hash_map.hpp"

namespace godot {

class DataGridManager : public Node {
	GDCLASS(DataGridManager, Node)

private:
	Size2i world_size;
	Size2i datagrid_count;
	int cell_size;
	Size2i datagrid_size;

	Dictionary datagrid_collection;

	struct InfluenceMapTemplate {
		int radius;
		Ref<InfluenceMap> datagrid;
		
		InfluenceMapTemplate() {}
		InfluenceMapTemplate(int p_radius, Ref<InfluenceMap> p_datagrid) {
			radius = p_radius;
			datagrid = p_datagrid;
		}
	};

	Ref<InfluenceMap> out_of_boundaries_template;
	HashMap<int, LocalVector<InfluenceMapTemplate>> hashed_templates;

protected:
	void _notification(int p_what);
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
	
	void create_templates(int p_type, int min_radius, int max_radius, const Ref<MathCurve> &p_curve);
	Ref<InfluenceMap> get_template(int p_type, int p_radius) const;

	void _process(float p_delta);
	void emit_updated(const Dictionary &datagrid_collection);

	void add_datagrid_layer_to_collection(const Point2i &p_datagrid_position, int p_layer, const Ref<InfluenceMap> &p_datagrid);
	bool has_datagrid_layer(const Point2i &p_datagrid_position, int p_layer) const;
	Ref<InfluenceMap> get_datagrid_layer(const Point2i &p_datagrid_position, int p_layer) const;
	Dictionary filter_datagrid_layers(const Point2i &p_datagrid_position, const Array &filter_layers) const;

	Vector2i global_position_to_datagrid_index(const Vector2i &p_global_position) const;
	bool grid_position_in_bounds(const Vector2i &p_data_grid_position) const;
	Vector2i world_position_to_cell_in_data_grid(const Vector2 &p_world_position, const Vector2i &p_data_grid_position) const;
	Array get_touched_datagrids(const Vector2i &p_center_cell, int p_radius) const;
	void add_datagrid_centered_to_collection(const Ref<InfluenceMap> &grid_to_add, int p_layer, const Point2 &p_global_position, float p_magnitude = 1.0f, bool registering = true);
	void add_into_datagrid_from_collection(const Ref<InfluenceMap> &grid_to_add_into, int p_layer, const Point2 &p_global_position, float p_magnitude = 1.0f);
	void mark_cells_outside_boundaries(const Ref<InfluenceMap> &p_imap, Vector2i p_global_position, float p_magnitude = 1.0f);
	
	Vector2 snap_global_postion_to_cell_center(const Vector2 &p_global_position) const;
	Vector2 find_corner_from_center(const Vector2 &p_global_position, Vector2i p_datagrid_center);
};
}

#endif // DATAGRIDMANAGER_H